/**
 * \defgroup uipopt Configuration options for uIP
 * @{
 *
 * uIP is configured using the per-project configuration file
 * "uipopt.h". This file contains all compile-time options for uIP and
 * should be tweaked to match each specific project. The uIP
 * distribution contains a documented example "uipopt.h" that can be
 * copied and modified for each project.
 *
 * \note Contiki does not use the uipopt.h file to configure uIP, but
 * uses a per-port uip-conf.h file that should be edited instead.
 */

/**
 * \file
 * Configuration options for uIP.
 * \author Adam Dunkels <adam@dunkels.com>
 *
 * This file is used for tweaking various configuration options for
 * uIP. You should make a copy of this file into one of your project's
 * directories instead of editing this example "uipopt.h" file that
 * comes with the uIP distribution.
 */

/*
 * Copyright (c) 2001-2003, Adam Dunkels.
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
 * 3. The name of the author may not be used to endorse or promote
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
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: uipopt.h,v 1.16 2005/02/24 22:25:12 oliverschmidt Exp $
 *
 */

#ifndef __UIPOPT_H__
#define __UIPOPT_H__

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN  3412
#endif /* LITTLE_ENDIAN */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN     1234
#endif /* BIGE_ENDIAN */

/*------------------------------------------------------------------------------*/
/**
 * \defgroup uipopttypedef uIP type definitions
 * @{
 */

#include "freertos.h"

/**
 * The 8-bit unsigned data type.
 *
 * This may have to be tweaked for your particular compiler. "unsigned
 * char" works for most compilers.
 */
typedef portCHAR u8_t;

/**
 * The 16-bit unsigned data type.
 *
 * This may have to be tweaked for your particular compiler. "unsigned
 * short" works for most compilers.
 */
typedef portSHORT u16_t;

#endif /* __UIPOPT_H__ */
