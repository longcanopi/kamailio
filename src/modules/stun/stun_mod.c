/*
 * Copyright (C) 2013 Crocodile RCS Ltd
 *
 * This file is part of Kamailio, a free SIP server.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Kamailio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * Kamailio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
/*!
 * \file
 * \brief STUN :: Module interface
 * \ingroup stun
 */

#include "../../core/dprint.h"
#include "../../core/events.h"
#include "../../core/ip_addr.h"
#include "../../core/sr_module.h"
#include "kam_stun.h"
#include "config.h"

MODULE_VERSION

static int mod_init(void);
static int stun_msg_receive(sr_event_param_t *evp);

/* clang-format off */
struct module_exports exports = {
	"stun",          /* module name */
	DEFAULT_DLFLAGS, /* dlopen flags */
	0,               /* exported functions */
	0,               /* exported parameters */
	0,               /* exported rpc functions */
	0,               /* exported pseudo-variables */
	0,               /* response handling function */
	mod_init,        /* module init function */
	0,               /* per-child init function */
	0                /* module destroy function */
};
/* clang-format on */

static int mod_init(void)
{
	if(sr_event_register_cb(SREV_STUN_IN, stun_msg_receive) != 0) {
		LM_ERR("registering STUN receive call-back\n");
		return -1;
	}

	if(cfg_declare("stun", stun_cfg_def, &default_stun_cfg, cfg_sizeof(stun),
			   &stun_cfg)) {
		LM_ERR("declaring config framework variable\n");
		return -1;
	}
	default_stun_cfg.stun_active = 1;

	return 0;
}

int stun_msg_receive(sr_event_param_t *evp)
{
	stun_event_info_t *sev = (stun_event_info_t *)evp->data;
	return process_stun_msg(sev->buf, sev->len, sev->rcv);
}
