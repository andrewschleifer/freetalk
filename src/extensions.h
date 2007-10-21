/*
  Author: Anand Avati <avati@zresearch.com>
  (C) 2005,2006 Freetalk core team
  
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

#ifndef __EXTENSIONS_H__
#define __EXTENSIONS_H__

#include <guile/gh.h>

extern SCM ex_message_receive_hook;
extern SCM ex_message_send_hook;
extern SCM ex_presence_receive_hook;
extern SCM ex_subscribe_receive_hook;
extern SCM ex_disconnect_hook;
extern SCM ex_command_hook;
extern SCM ex_login_hook;
extern SCM ex_quit_hook;
extern SCM ex_notify_file_hook;

int get_hook_return (void);
void set_hook_return (int hook_return_value);

void ft_load (const char *file);
void extensions_init (void);
void load_default_config (void);

#endif /* __EXTENSIONS_H__ */
