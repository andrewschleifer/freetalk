/*
  Author: Vikas Gorur <vikas@80x25.org>
  (C) 2005,2006 Freetalk core team
  
  presence.h
  
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

#ifndef __PRESENCE_H__
#define __PRESENCE_H__


void ft_presence_cb (LmMessage *msg);
void ft_presence_send_final (void);
void ft_presence_send_initial (void);

typedef enum {
  SUBSCRIBE,
  SUBSCRIBED,
  UNSUBSCRIBE,
  UNSUBSCRIBED
} FtPresenceMsgType;

void ft_presence_subscription_allow (char *jid);
void ft_presence_subscription_deny (char *jid);

#endif /* __PRESENCE_H__ */