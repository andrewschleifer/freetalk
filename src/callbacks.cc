/*
  callbacks.c: Callback functions
  
  Copyright (c) 2005, 2006, 2007, 2008, 2009, 2009 Freetalk Core Team
  This file is part of GNU Freetalk.
  
  Freetalk is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 3 of the License,
  or (at your option) any later version.
  
  Freetalk is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <glib.h> 
#include <loudmouth/loudmouth.h>
#include <readline/readline.h>

#include "util.h"
#include "freetalk.h"
#include "commands.h"
#include "callbacks.h"
#include "roster.h"
#include "presence.h"
#include "extensions.h"
#include "file_transfer.h"
#include "compat.h"

/*
  Callback for SSL verification
*/

LmSSLResponse 
ft_ssl_response_cb (LmSSL *ssl,
     LmSSLStatus st,
     gpointer user_datta)
{
  /* dont really care */
  return LM_SSL_RESPONSE_CONTINUE;
}


/*
   Called when authentication succeeds
*/
 
void
ft_authenticate_cb (LmConnection *conn, gboolean success, gpointer user_data)
{
  do_session_init (success);
}

/*
  Called when the connection is opened
*/

void
ft_connection_open_cb (LmConnection *conn, gboolean success, gpointer u)
{
  ft_state *state = (ft_state *)u;
  if (success) {
    do_set_conn_status (FT_CONN);
    PRINTF ("%s",_("Connected."));
  }
  else
    {
      do_set_conn_status (FT_DEAD);
      PRINTF ("%s",_("Could not connect.")); 
      return;
    }

  PRINTF ("%s",_("Authenticating ...")); fflush(stdout);
  lm_connection_authenticate (conn, state->jid.node, state->password,
			      state->jid.resource, ft_authenticate_cb,
			      NULL, g_free, NULL);
}

/*
  Incoming message handlers
*/

/*
  Parse the 'stamp' element in an offline message and return a string
  representation of it. The string must be freed with g_free
*/

static char *
parse_timestamp (const char *ts)
{
  /* format: "YYYY-MM-DD hh:mm:ss" */
  /* ts format: 20021209T23:51:30 */

  const char *format = "YYYY-MM-DD hh:mm:ss";
  int format_len = strlen (format);
  char *time = g_new (char, format_len);
  strftime (time, format_len, "%Y-%m-%d %I:%M%p", lm_utils_get_localtime (ts));
  return time;
}

/*
  LM_MESSAGE_TYPE_MESSAGE
*/

static LmHandlerResult
ft_msg_msg_handler (LmMessageHandler *handler, LmConnection *conn,
		    LmMessage *msg, gpointer user_data)
{
  LmMessageNode *root, *body, *x;
  const char *msg_str, *type;
  char *ts = NULL;
  std::string from;

  root = lm_message_get_node (msg);
  body = lm_message_node_get_child (root, "body");

  from = (char *) lm_message_node_get_attribute (msg->node, "from");

  /* since the file-transfer happens in-band with messages, we can no longer
   * ignore messages with no 'body' */
  if (body){
    msg_str = lm_message_node_get_value (body);
  
    type = lm_message_node_get_attribute (msg->node, "type");
    if (type && g_ascii_strcasecmp (type, "chat") != 0)
      {
	do_printf (_("[message of type '%s']"), type);
	return LM_HANDLER_RESULT_REMOVE_MESSAGE;
      }

    // Offline messages
    for (x = root->children; x != NULL; x = x->next)
      {
	if (!g_ascii_strcasecmp (x->name, "x"))
	  {
	    const char *xmlns = lm_message_node_get_attribute (x, "xmlns");
	    if (xmlns && !g_ascii_strcasecmp (xmlns, "jabber:x:delay"))
	      {
		ts = parse_timestamp ((char *)lm_message_node_get_attribute (x, "stamp"));
	      }
	  }
      }
    
    set_hook_return (0);
    /* TBD : make the below stripping of /Resource configurable */
    if (1) {
      const size_t pos = from.find("/");
      if (pos != std::string::npos)
	from.replace(pos, from.size() - pos, "");
    }
    {
      FtRosterItem *item = ft_roster_lookup (from.c_str());
      char *nickname;
      
      if (!item)
	nickname = NULL;
      else
	nickname = item->nickname;
      
      scm_run_hook (ex_message_receive_hook,
		    scm_list_n (ts ? scm_from_locale_string (ts) : scm_from_locale_string (""),
				scm_from_locale_string (from.c_str()),
				nickname ? scm_from_locale_string (nickname) : scm_from_locale_string (""),
				scm_from_locale_string (msg_str),
				SCM_UNDEFINED));
    }
    if (ts) g_free (ts);
    
    if (get_hook_return () == 1) 
      return LM_HANDLER_RESULT_REMOVE_MESSAGE;
    
    PRINTF ("%s: %s", from.c_str(), msg_str);
  } else {
    /* This logic should be looked into : FIXME */
    ft_send_file_message_data (msg);

  }
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

/*
  LM_MESSAGE_TYPE_PRESENCE
*/

static LmHandlerResult
ft_msg_presence_handler (LmMessageHandler *handler, LmConnection *conn,
			 LmMessage *msg, gpointer user_data)
{
  //  PRINTF ("[presence recieved]");
  ft_presence_cb (msg);
  /* scm_run_hook (ex_presence_receive_hook, ... ) */
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

/*
  LM_MESSAGE_TYPE_IQ
*/

static LmHandlerResult
ft_msg_iq_handler (LmMessageHandler *handler, LmConnection *conn,
		   LmMessage *msg, gpointer user_data)
{
  /* Currently the only IQ message we'll handle is the roster */
  LmMessageNode *query = lm_message_node_get_child (msg->node, "query");

  int type = lm_message_get_sub_type (msg);

  if (query) {
    const char *ns = lm_message_node_get_attribute (query, "xmlns");
    
    if (ns && !g_ascii_strcasecmp (ns, "jabber:iq:roster")) {
      ft_roster_cb (msg);
    } else if (ns && !g_ascii_strcasecmp (ns, "jabber:iq:version")) {
      if( type == LM_MESSAGE_SUB_TYPE_GET ) {
	ft_msg_iq_version_cb (msg);
      }
    } else if (ns && !g_ascii_strcasecmp (ns, "jabber:iq:last")) {
      if( type == LM_MESSAGE_SUB_TYPE_GET ) {
	ft_msg_iq_last_cb (msg);
      }
    } else if (ns && !g_ascii_strcasecmp (ns, "http://jabber.org/protocol/disco#info")) {
      if( type == LM_MESSAGE_SUB_TYPE_GET ) {
	ft_msg_sub_type_get_cb (msg);
      } else if( type == LM_MESSAGE_SUB_TYPE_RESULT ) {
	ft_msg_sub_type_result_cb (msg);
      }
    } else
      PRINTF (_("[iq received: %s (unhandled yet)]"), ns);

  } else {
    if ((LM_MESSAGE_SUB_TYPE_SET == lm_message_get_sub_type (msg)))
      ft_msg_sub_type_set_cb (msg);
    else
      PRINTF (_("[iq received: (unhandled yet)]"));
  }

  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

/*
  Disconnection handler to cleanup mess
*/

static void
ft_disconnect_function (LmConnection *conn,
			LmDisconnectReason reason,
			gpointer user_data)
{
  lm_connection_unref (state.conn);
  state.conn = NULL;
  ft_roster_flush ();
  ft_file_flush ();

  scm_run_hook (ex_disconnect_hook, scm_list_n (scm_from_int (reason), SCM_UNDEFINED));
  /* set conn_status after hook so that discon hook procedures can get
     the previous state from ft-get-conn-status. helps in deciding
     if an automatic reconnection logic (dont auto reconnect if previous
     state was not FT_AUTH, etc)
  */
  do_set_conn_status (FT_DEAD);

  /* run another hook which actually lets scripts reconnect
  */
  scm_run_hook (ex_disconnected_hook, scm_list_n (scm_from_int (reason), SCM_UNDEFINED));
  return;
}

/*
  Register handlers for each type of message
*/

void
ft_register_msg_handlers (LmConnection *conn)
{
  LmMessageHandler *handler = lm_message_handler_new ((LmHandleMessageFunction) ft_msg_msg_handler,
						      NULL, NULL);
  lm_connection_register_message_handler (conn, handler, LM_MESSAGE_TYPE_MESSAGE,
					  LM_HANDLER_PRIORITY_NORMAL);
  lm_message_handler_unref (handler);

  
  handler = lm_message_handler_new ((LmHandleMessageFunction) ft_msg_presence_handler,
				    NULL, NULL);
  lm_connection_register_message_handler (conn, handler, LM_MESSAGE_TYPE_PRESENCE,
					  LM_HANDLER_PRIORITY_NORMAL);
  lm_message_handler_unref (handler);

  
  handler = lm_message_handler_new ((LmHandleMessageFunction) ft_msg_iq_handler,
				    NULL, NULL);
  lm_connection_register_message_handler (conn, handler, LM_MESSAGE_TYPE_IQ,
					  LM_HANDLER_PRIORITY_NORMAL);
  lm_message_handler_unref (handler);

  lm_connection_set_disconnect_function (conn, ft_disconnect_function, NULL, NULL);
}
