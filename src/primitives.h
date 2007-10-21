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

#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include <guile/gh.h>

SCM ex_load (SCM scm_file);

SCM ex_get_jid (void);
SCM ex_set_jid (SCM scm_jid);

SCM ex_get_server (void);
SCM ex_set_server (SCM scm_server);

SCM ex_get_password (void);
SCM ex_set_password (SCM scm_password);

SCM ex_get_sslconn (void);
SCM ex_set_sslconn (SCM scm_ssl);

SCM ex_connect (void);
SCM ex_disconnect (void);

SCM ex_send_message (SCM scm_to, SCM scm_msg);
SCM ex_send_message_no_hook (SCM scm_to, SCM scm_msg);

SCM ex_set_daemon (void);
SCM ex_get_daemon (void);

SCM ex_set_prompt (SCM scm_prompt);
SCM ex_get_prompt (void);

SCM ex_get_port (void);
SCM ex_set_port (SCM scm_port);

SCM ex_display (SCM scm_line);

SCM ex_get_conn_status (void);

SCM ex_add_buddy (SCM scm_jid);
SCM ex_remove_buddy (SCM scm_jid);

SCM ex_get_roster_list (void);
SCM ex_get_roster_status_msg (SCM scm_jid);
SCM ex_get_roster_is_online (SCM scm_jid);
SCM roster_item_to_list (FtRosterItem *item); /* For internal use only */
SCM ex_roster_lookup (SCM scm_jid);

SCM ex_hook_return (void);

SCM ex_get_current_buddy (void);
SCM ex_set_current_buddy (SCM scm_bud);

SCM ex_get_status_msg (void);
SCM ex_set_status_msg (SCM scm_status);

SCM ex_reset_fs_state (void);

SCM ex_dict_append (SCM scm_word);
SCM ex_dict_prepend (SCM scm_word);
SCM ex_dict_insert (SCM scm_word);
SCM ex_dict_remove (SCM scm_word);

SCM ex_version (void);

SCM ex_login_blocking (void);

SCM ex_cli_on (void);

SCM ex_main_loop (void);

SCM ex_load_default_config (void);

SCM ex_get_config_dir (void);

SCM ex_subscription_allow (SCM jid);
SCM ex_subscription_deny (SCM jid);

SCM ex_beep (SCM freq, SCM msec);

SCM ex_give_repl (void);

SCM ex_quit (SCM exit_code);

SCM ex_send_file (SCM reciever, SCM file_name);
SCM ex_set_allow_file (SCM cookie, SCM file_name);

SCM ex_change_password (SCM newpass);

SCM ex_roster_set_nickname (SCM jid, SCM nickname);

SCM ex_bind_to_ctrl_key (SCM key, SCM command);

#endif /* __PRIMITIVES_H__ */