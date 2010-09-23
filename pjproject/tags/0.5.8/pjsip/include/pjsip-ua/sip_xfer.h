/* $Id$ */
/* 
 * Copyright (C) 2003-2006 Benny Prijono <benny@prijono.org>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#ifndef __PJSIP_XFER_H__
#define __PJSIP_XFER_H__



/**
 * @file sip_xfer.h
 * @brief SIP Transfer (REFER, RFC 3515)
 */
#include <pjsip-simple/evsub.h>
#include <pjsip/sip_msg.h>

/**
 * @defgroup PJSUA_XFER Call Transfer
 * @ingroup PJSIP_HIGH_UA
 * @brief Provides call transfer functionality.
 * @{
 *
 * This implements call transfer functionality to INVITE sessions. The call
 * transfer functionality uses SIP Event Subscription framework for
 * managing call transfer status.
 *
 * Application must link with <b>pjsip-ua</b> AND <b>pjsip-simple</b> static
 * libraries to use call transfer functionality.
 */


PJ_BEGIN_DECL


/**
 * Declaration for REFER method constant.
 */
extern const pjsip_method pjsip_refer_method;


/**
 * Initialize the REFER subsystem.
 * This currently does very little (only register REFER as supported method).
 */
PJ_DECL(pj_status_t) pjsip_xfer_init_module(pjsip_endpoint *endpt);



/**
 * Create transferer (sender of REFER request).
 *
 * @param dlg		The underlying dialog to use.
 * @param user_cb	Pointer to callbacks to receive presence subscription
 *			events.
 * @param p_evsub	Pointer to receive the presence subscription
 *			session.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_create_uac( pjsip_dialog *dlg,
					    const pjsip_evsub_user *user_cb,
					    pjsip_evsub **p_evsub );


/**
 * Create transferee (receiver of REFER request).
 *
 * @param dlg		The underlying dialog to use.
 * @param user_cb	Pointer to callbacks to receive presence subscription
 *			events.
 * @param rdata		The incoming SUBSCRIBE request that creates the event 
 *			subscription.
 * @param p_evsub	Pointer to receive the presence subscription
 *			session.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_create_uas( pjsip_dialog *dlg,
					    const pjsip_evsub_user *user_cb,
					    pjsip_rx_data *rdata,
					    pjsip_evsub **p_evsub );

/**
 * Call this function to create request to initiate REFER subscription,
 * to refresh subscription, or to unsubscribe. For request other than
 * the initial REFER request, "refer_to_uri" argument may be NULL.
 *
 * @param sub		Client subscription instance.
 * @param refer_to_uri	URI to be put to the Refer-To header. This argument
 *			may be NULL for subsequent REFER requests.
 * @param p_tdata	Pointer to receive the request.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_initiate( pjsip_evsub *sub,
					  const pj_str_t *refer_to_uri,
					  pjsip_tx_data **p_tdata);


/**
 * Accept the incoming REFER request by sending 2xx response.
 *
 * @param sub		Server subscription instance.
 * @param rdata		The incoming subscription request message.
 * @param st_code	Status code, which MUST be 2xx.
 * @param hdr_list	Optional list of headers to be added in the response.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_accept( pjsip_evsub *sub,
					pjsip_rx_data *rdata,
				        int st_code,
					const pjsip_hdr *hdr_list );


/**
 * For notifier, create NOTIFY request to subscriber, and set the state 
 * of the subscription. 
 *
 * @param sub		The server subscription (notifier) instance.
 * @param state		New state to set.
 * @param xfer_st_code	The call status code to be reported with the NOTIFY
 *			request.
 * @param xfer_st_text	Optional call status text to be reported with the 
 *			NOTIFY request. If the value is NULL, default 
 *			status text will be used.
 * @param p_tdata	Pointer to receive the request.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_notify( pjsip_evsub *sub,
					pjsip_evsub_state state,
					int xfer_st_code,
					const pj_str_t *xfer_st_text,
					pjsip_tx_data **p_tdata);


/**
 * Create NOTIFY request to reflect current subscription status. Application
 * can only call this function after it has sent NOTIFY before.
 * This will also re-send the last "message/sipfrag" body that was sent
 * in the previous NOTIFY.
 *
 * @param sub		Server subscription object.
 * @param p_tdata	Pointer to receive request.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_current_notify( pjsip_evsub *sub,
					        pjsip_tx_data **p_tdata );



/**
 * Send request message that was previously created with initiate(), notify(),
 * or current_notify(). Application may also send request created with other
 * functions, e.g. authentication. But the request MUST be either request
 * that creates/refresh subscription or NOTIFY request.
 *
 *
 * @param sub		The event subscription object.
 * @param tdata		Request message to be send.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_xfer_send_request( pjsip_evsub *sub,
					      pjsip_tx_data *tdata);


PJ_END_DECL

/**
 * @}
 */

#endif	/* __PJSIP_XFER_H__ */
