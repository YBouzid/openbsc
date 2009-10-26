

/* (C) 2009 by Harald Welte <laforge@gnumonks.org>
 *
 * All Rights Reserved
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#include <sys/types.h>

#include <openbsc/gsm_04_08.h>
#include <openbsc/signal.h>
#include <openbsc/gsm_subscriber.h>
#include <openbsc/chan_alloc.h>

/* RRLP MS based position request */
static const u_int8_t ms_based_pos_req[] = { 0x40, 0x01, 0x78, 0xa8 };

static int subscr_sig_cb(unsigned int subsys, unsigned int signal,
			 void *handler_data, void *signal_data)
{
	struct gsm_subscriber *subscr;
	struct gsm_lchan *lchan;

	switch (signal) {
	case S_SUBSCR_ATTACHED:
		/* A subscriber has attached. */
		subscr = signal_data;
		lchan = lchan_for_subscr(subscr);
		if (!lchan)
			break;
		gsm48_send_rr_app_info(lchan, 0x00, sizeof(ms_based_pos_req),
					ms_based_pos_req);
		break;
	}
	return 0;
}

static int paging_sig_cb(unsigned int subsys, unsigned int signal,
			 void *handler_data, void *signal_data)
{
	struct paging_signal_data *psig_data = signal_data;

	switch (signal) {
	case S_PAGING_COMPLETED:
		/* A subscriber has attached. */
		gsm48_send_rr_app_info(psig_data->lchan, 0x00, 
					sizeof(ms_based_pos_req),
					ms_based_pos_req);
		break;
	}
	return 0;
}

void on_dso_load_rrlp(void)
{
	register_signal_handler(SS_SUBSCR, subscr_sig_cb, NULL);
	register_signal_handler(SS_PAGING, paging_sig_cb, NULL);
}