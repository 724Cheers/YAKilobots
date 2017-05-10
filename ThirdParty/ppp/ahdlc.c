/*																www.mycal.com
*---------------------------------------------------------------------------
* ahdlc.c - Ahdlc receive and transmit processor for PPP engine.
*
*---------------------------------------------------------------------------
* Version
*		0.1 Original Version Jan 11, 1998
*
*---------------------------------------------------------------------------
*
* Copyright (C) 1998, Mycal Labs www.mycal.com
*
*---------------------------------------------------------------------------
*/
/*
* Copyright (c) 2003, Mike Johnson, Mycal Labs, www.mycal.net
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*      This product includes software developed by Mike Johnson/Mycal Labs
*		www.mycal.net.
* 4. The name of the author may not be used to endorse or promote
*    products derived from this software without specific prior
*    written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* This file is part of the Mycal Modified uIP TCP/IP stack.
*
* $Id: ahdlc.c,v 1.3 2005/01/26 23:36:22 oliverschmidt Exp $
*
*/

/*			*/
/* include files 	*/
/*			*/

#include "PPP/ppp.h"
#include "vTaskPppHandler.h"

void ppp_arch_putchar(u8_t c);
//#if 0
//#define DEBUG1(x)
//#else
//#include <stdio.h>
//#define DEBUG1(x) debug_printf x
//#endif

//#define	PACKET_TX_DEBUG	1

/*---------------------------------------------------------------------------
* ahdlc flags bit defins, for ahdlc_flags variable
---------------------------------------------------------------------------*/
/* Escaped mode bit */
#define AHDLC_ESCAPED		0x1
/* Frame is ready bit */
#define	AHDLC_RX_READY		0x2				
//#define	AHDLC_RX_ASYNC_MAP	0x4
//#define AHDLC_TX_ASYNC_MAP	0x8
//#define AHDLC_PFC		0x10
//#define AHDLC_ACFC		0x20

/*---------------------------------------------------------------------------
* Private Local Globals
*	10 bytes	- standard
*			- with counters
---------------------------------------------------------------------------*/
/* running tx CRC */
u16_t ahdlc_tx_crc;
/* running rx CRC */
u16_t ahdlc_rx_crc;
/* number of rx bytes processed, cur frame */
u16_t ahdlc_rx_count;
u16_t ahdlc_tx_count = 0;
/* ahdlc state flags, see above */
u8_t ahdlc_flags;

/*
* The following can be optimized out
*/
u8_t *ahdlc_rx_buffer = NULL;			/* What to do here? +++ */
u16_t ahdlc_max_rx_buffer_size = 0;

u8_t *ahdlc_tx_buffer = NULL;			/* What to do here? +++ */
u16_t ahdlc_max_tx_buffer_size = 0;

/*
* Optional statistics counters.
*/
//#ifdef AHDLC_COUNTERS
//u8_t ahdlc_rx_tobig_error;
//#endif

/*---------------------------------------------------------------------------*/
/* Simple and fast CRC16 routine for embedded processors.
*	Just slightly slower than the table lookup method but consumes
*	almost no space.  Much faster and smaller than the loop and
*	shift method that is widely used in the embedded space.
*	Can be optimized even more in .ASM
*
*	data = (crcvalue ^ inputchar) & 0xff;
*	data = (data ^ (data << 4)) & 0xff;
*	crc = (crc >> 8) ^ ((data << 8) ^ (data <<3) ^ (data >> 4))
*/
/*---------------------------------------------------------------------------*/
static u16_t
crcadd(u16_t crcvalue, u8_t c)
{
	u16_t b;

	b = (crcvalue ^ c) & 0xFF;
	b = (b ^ (b << 4) & 0xFF);
	b = (b << 8) ^ (b << 3) ^ (b >> 4);

	return ((crcvalue >> 8) ^ b);
}
/*---------------------------------------------------------------------------*/
/* ahdlc_rx_ready() - resets the ahdlc engine to the beginning of frame
*	state.
*/
/*---------------------------------------------------------------------------*/
void
ahdlc_rx_ready(void)
{
	ahdlc_rx_count = 0;
	ahdlc_rx_crc = 0xffff;
	ahdlc_flags |= AHDLC_RX_READY;
}
/*---------------------------------------------------------------------------*/
/* ahdlc_init(buffer, buffersize) - this initializes the ahdlc engine to
*	allow for rx frames.
*/
/*---------------------------------------------------------------------------*/
void
ahdlc_init(u8_t *rxbuffer, u16_t maxrxbuffersize, u8_t *txbuffer, u16_t maxtxbuffersize)
{
	ahdlc_flags = 0;
	ahdlc_rx_buffer = rxbuffer;
	ahdlc_max_rx_buffer_size = maxrxbuffersize;
	ahdlc_tx_buffer = txbuffer;
	ahdlc_max_tx_buffer_size = maxtxbuffersize;

	/* ahdlc_async_map = 0; */
//#ifdef AHDLC_COUNTERS
//	ahdlc_rx_tobig_error = 0;
//#endif
	ahdlc_rx_ready();
}
/*---------------------------------------------------------------------------*/
/* ahdlc receive function - This routine processes incoming bytes and tries
*	to build a PPP frame.
*
*	Two possible reasons that ahdlc_rx will not process characters:
*		o Buffer is locked - in this case ahdlc_rx returns 1, char
*			sending routing should retry.
*/
/*---------------------------------------------------------------------------*/
u8_t
ahdlc_rx(u8_t c)
{

	/* check to see if PPP packet is useable, we should have hardware
	flow control set, but if host ignores it and sends us a char when
	the PPP Receive packet is in use, discard the character. +++ */

	if (ahdlc_flags & AHDLC_RX_READY) {
		/* are we in escaped mode? */
		if (ahdlc_flags & AHDLC_ESCAPED) {
			/* set escaped to FALSE */
			ahdlc_flags &= ~AHDLC_ESCAPED;

			/* if value is 0x7e then silently discard and reset receive packet */
			if (c == 0x7e) {
				ahdlc_rx_ready();
				return 0;
			}
			/* incomming char = itself xor 20 */
			c = c ^ 0x20;
		}
		else if (c == 0x7e) {
			/* handle frame end */
			if (ahdlc_rx_crc == (u16_t)CRC_GOOD_VALUE) {
				//	DEBUG1(("\nReceiving packet with good crc value, len %d\n",ahdlc_rx_count));
				/* remove CRC bytes from packet */
				ahdlc_rx_count -= 2;

				/* lock PPP buffer */
				ahdlc_flags &= ~AHDLC_RX_READY;
				/*
				* upcall routine must fully process frame before return
				*	as returning signifies that buffer belongs to AHDLC again.
				*/
				/* Send up packet */
				ppp_upcallX(/*(u16_t)ahdlc_rx_buffer[0],*/
					(u8_t *)ahdlc_rx_buffer,
					(u16_t)ahdlc_rx_count);
				ahdlc_rx_ready();
				return 0;
			}
			ahdlc_rx_ready();
			return 0;
		}else if (c == 0x7d) {
			/* handle escaped chars*/
			ahdlc_flags |= PPP_ESCAPED;
			return 0;
		}

		/* try to store char if not to big */
		if (ahdlc_rx_count >= ahdlc_max_rx_buffer_size /*PPP_RX_BUFFER_SIZE*/) {
//#ifdef AHDLC_COUNTERS			
//			++ahdlc_rx_tobig_error;
//#endif
			ahdlc_rx_ready();
		}else {
			/* Add CRC in */
			ahdlc_rx_crc = crcadd(ahdlc_rx_crc, c);
			/* Store char */
			ahdlc_rx_buffer[ahdlc_rx_count++] = c;
		}
	}
	else {
		/* we are busy and didn't process the character. */
		//    DEBUG1(("Busy/not active\n"));
		return 1;
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
/* ahdlc_tx_char(char) - write a character to the serial device,
* escape if necessary.
*
* Relies on local global vars	:	ahdlc_tx_crc, ahdlc_flags.
* Modifies local global vars	:	ahdlc_tx_crc.
*/
/*---------------------------------------------------------------------------*/
void
ahdlc_tx_char(/*u16_t protocol, */u8_t c)
{
	/* add in crc */
	ahdlc_tx_crc = crcadd(ahdlc_tx_crc, c);
	/*
	* See if we need to escape char, we always escape 0x7d and 0x7e, in the case
	* of char < 0x20 we only support async map of default or none, so escape if
	* ASYNC map is not set.  We may want to modify this to support a bitmap set
	* ASYNC map.
	*/
	if ((c == 0x7d) || (c == 0x7e) /*||
		((c < 0x20) && ((protocol == (u16_t)LCP) ||
		(ahdlc_flags & PPP_TX_ASYNC_MAP) == 0))*/) {
		/* send escape char and xor byte by 0x20 */
		ppp_arch_putchar(0x7d);
		c ^= 0x20;
	}
	ppp_arch_putchar(c);
}
/*---------------------------------------------------------------------------*/
/* ahdlc_tx(protocol,buffer,len) - Transmit a PPP frame.
*	Buffer contains protocol data, ahdlc_tx addes address, control and
*	protocol data.
*
* Relies on local global vars	:	ahdlc_tx_crc, ahdlc_flags.
* Modifies local global vars	:	ahdlc_tx_crc.
*/
/*---------------------------------------------------------------------------*/
u16_t
ahdlc_tx(/*u16_t protocol, u8_t *header, */u8_t *buffer,
/*u16_t headerlen, */u16_t datalen)
{
	u16_t i;
	u8_t c;

//	DEBUG1(("\nAHDLC_TX - transmit frame, protocol 0x%04x, length %d\n", protocol, datalen + headerlen));

//#if PACKET_TX_DEBUG
////	DEBUG1(("\n"));
//	for (i = 0; i < headerlen; ++i) {
////		DEBUG1(("0x%02x ", header[i]));
//	}
//	for (i = 0; i < datalen; ++i) {
////		DEBUG1(("0x%02x ", buffer[i]));
//	}
////	DEBUG1(("\n\n"));
//#endif

	/* Check to see that physical layer is up, we can assume is some
	cases */

	/* write leading 0x7e */
	ppp_arch_putchar(0x7e);

	/* set initial CRC value */
	ahdlc_tx_crc = 0xffff;

	/* Write frame bytes */
	for (i = 0; i < datalen; ++i) {
		/* Get next byte from buffer */
		c = buffer[i];
		/* Write it...*/
		ahdlc_tx_char(/*protocol, */c);
	}

	/* send crc, lsb then msb */
	i = ahdlc_tx_crc ^ 0xffff;
	ahdlc_tx_char(/*protocol, */(u8_t)(i & 0xff));
	ahdlc_tx_char(/*protocol, */(u8_t)((i >> 8) & 0xff));

	/* write trailing 0x7e, probably not needed but it doesn't hurt*/
	ppp_arch_putchar(0x7e);
	u16_t cur_ahdlc_tx_count = ahdlc_tx_count;
	ahdlc_tx_count = 0;
	return cur_ahdlc_tx_count;
}
/*---------------------------------------------------------------------------*/

void
ppp_arch_putchar(u8_t c)
{
	if(ahdlc_tx_count >= ahdlc_max_tx_buffer_size) return;
	ahdlc_tx_buffer[ahdlc_tx_count] = c;
	ahdlc_tx_count++;
}
