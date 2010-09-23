/* $Id$ */
/* 
 * Copyright (C) 2003-2007 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
 */
#ifndef __PJMEDIA_TRANSPORT_SRTP_H__
#define __PJMEDIA_TRANSPORT_SRTP_H__

/**
 * @file srtp.h
 * @brief transport SRTP encapsulates secure media transport.
 */

#include <pjmedia/transport.h>


PJ_BEGIN_DECL


/**
 * Crypto option.
 */
typedef enum pjmedia_srtp_crypto_option
{
    /** When this flag is specified, encryption will be disabled. */
    PJMEDIA_SRTP_NO_ENCRYPTION  = 1,

    /** When this flag is specified, authentication will be disabled. */
    PJMEDIA_SRTP_NO_AUTHENTICATION  = 2

} pjmedia_srtp_crypto_option;


/**
 * This structure describes an individual crypto setting.
 */
typedef struct pjmedia_srtp_crypto
{
    /** Optional key. If empty, a random key will be autogenerated. */
    pj_str_t	key;

    /** Crypto name.   */
    pj_str_t	name;

    /* Flags, bitmask from #pjmedia_srtp_crypto_option */
    unsigned	flags;

} pjmedia_srtp_crypto;


/**
 * This enumeration specifies the behavior of the SRTP transport regarding
 * media security offer and answer.
 */
typedef enum pjmedia_srtp_use
{
    /**
     * When this flag is specified, SRTP will be disabled, and the transport
     * will reject RTP/SAVP offer.
     */
    PJMEDIA_SRTP_DISABLED,

    /**
     * When this flag is specified, SRTP will be advertised as optional and
     * incoming SRTP offer will be accepted.
     */
    PJMEDIA_SRTP_OPTIONAL,

    /**
     * When this flag is specified, the transport will require that RTP/SAVP
     * media shall be used.
     */
    PJMEDIA_SRTP_MANDATORY

} pjmedia_srtp_use;


/**
 * Settings to be given when creating SRTP transport. Application should call
 * #pjmedia_srtp_setting_default() to initialize this structure with its 
 * default values.
 */
typedef struct pjmedia_srtp_setting
{
    /**
     * Specify the usage policy. Default is PJMEDIA_SRTP_OPTIONAL.
     */
    pjmedia_srtp_use use;

    /**
     * Specify whether the SRTP transport should close the member transport 
     * when it is destroyed. Default: PJ_TRUE.
     */
    pj_bool_t			close_member_tp;

    /**
     * Specify the number of crypto suite settings.
     */
    unsigned			crypto_count;

    /**
     * Specify individual crypto suite setting.
     */
    pjmedia_srtp_crypto		crypto[8];

} pjmedia_srtp_setting;


/**
 * Initialize SRTP setting with its default values.
 *
 * @param opt	SRTP setting to be initialized.
 */
PJ_DECL(void) pjmedia_srtp_setting_default(pjmedia_srtp_setting *opt);


/**
 * Create an SRTP media transport.
 *
 * @param endpt	    The media endpoint instance.
 * @param tp	    The actual media transport to send and receive 
 *		    RTP/RTCP packets. This media transport will be
 *		    kept as member transport of this SRTP instance.
 * @param opt	    Optional settings. If NULL is given, default
 *		    settings will be used.
 * @param p_tp	    Pointer to receive the transport SRTP instance.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_create(
				       pjmedia_endpt *endpt,
				       pjmedia_transport *tp,
				       const pjmedia_srtp_setting *opt,
				       pjmedia_transport **p_tp);


/**
 * Manually start SRTP session with the given parameters. Application only
 * needs to call this function when the SRTP transport is used without SDP
 * offer/answer. When SDP offer/answer framework is used, the SRTP transport
 * will be started/stopped by #pjmedia_transport_media_start() and 
 * #pjmedia_transport_media_stop() respectively.
 *
 * Please note that even if an RTP stream is only one direction, application
 * will still need to provide both crypto suites, because it is needed by 
 * RTCP.

 * If application specifies the crypto keys, the keys for transmit and receive
 * direction MUST be different.
 *
 * @param srtp	    The SRTP transport.
 * @param tx	    Crypto suite setting for transmit direction.
 * @param rx	    Crypto suite setting for receive direction.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_start(
					    pjmedia_transport *tp,
					    const pjmedia_srtp_crypto *tx,
					    const pjmedia_srtp_crypto *rx);

/**
 * Stop SRTP session.
 *
 * @param srtp	    The SRTP media transport.
 *
 * @return	    PJ_SUCCESS on success.
 *
 * @see #pjmedia_transport_srtp_start() 
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_stop(pjmedia_transport *tp);


/**
 * Query member transport of SRTP.
 *
 * @param srtp		    The SRTP media transport.
 *
 * @return		    member media transport.
 */
PJ_DECL(pjmedia_transport*) pjmedia_transport_srtp_get_member(
						    pjmedia_transport *tp);


PJ_END_DECL

#endif /* __PJMEDIA_TRANSPORT_SRTP_H__ */