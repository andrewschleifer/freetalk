/*
  Author: Vikas Gorur <vikas@80x25.org>
  (C) 2005,2006 Freetalk core team
  
  callbacks.c: Callback functions
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.
    
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
    
  You should have received a copy of the GNU General Public
  License along with this program; if not, write to the Free
  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301 USA
*/
#ifndef __ROSTER_H__
#define __ROSTER_H__
#include <glib.h>

/*
  Ref. Section 9., "Subscription states",
  RFC 3921
*/

typedef enum {FT_SUBSCRIPTION_NONE,
	      FT_SUBSCRIPTION_NONE_PENDING_OUT,
	      FT_SUBSCRIPTION_NONE_PENDING_IN,
	      FT_SUBSCRIPTION_NONE_PENDING_OUT_IN,
	      FT_SUBSCRIPTION_FROM,
	      FT_SUBSCRIPTION_FROM_PENDING_OUT,
	      FT_SUBSCRIPTION_TO,
	      FT_SUBSCRIPTION_TO_PENDING_IN,
	      FT_SUBSCRIPTION_BOTH} FtSubscriptionState;

typedef struct
{
  char *jid;
  FtSubscriptionState subscription;
  gboolean is_online;
  char *nickname;
  char *show_msg;
  char *status_msg;
  char *resource; /* resource is client software name */
} FtRosterItem;

void ft_roster_init (LmConnection *conn);
void ft_roster_cb (LmMessage *msg);
void ft_roster_presence_cb (LmMessage *msg);
void ft_roster_foreach (GFunc func, gpointer userdata);

void ft_roster_add (char *jid, char *nickname);
void ft_roster_remove (char *jid);

GSList *ft_roster_get (void);
void ft_roster_retrieve (LmConnection *conn);
void ft_roster_flush ();

FtRosterItem *ft_roster_lookup (const char *jid);
void ft_roster_retrieve (LmConnection *conn);
void ft_roster_set_nickname (char *jid, char *nickname);

#endif /* __ROSTER_H__ */
