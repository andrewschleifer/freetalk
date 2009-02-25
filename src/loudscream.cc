/*
  Copyright (c) 2005, 2006, 2007, 2008, 2009 Freetalk Core Team
  This file is part of Freetalk.
  
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

/*
  This file is the Guile interface to Loudmouth
*/

#include <guile/gh.h>
#include <glib.h>
#include <loudmouth/loudmouth.h>

#include "freetalk.h"

static scm_t_bits lm_msg_tag;
static scm_t_bits lm_msg_node_tag;

#define LM_MSG(msg)  ((LmMessage *)SCM_SMOB_DATA(msg))
#define LM_MSG_NODE(node)  ((LmMessageNode *)SCM_SMOB_DATA(node))

/*
  LmMessage
*/

size_t
ls_lm_message_free (SCM msg)
{
  lm_message_unref (LM_MSG(msg));
  return 0;
}

SCM
ls_lm_message_new (SCM to, SCM typesym)
{
  LmMessage *msg; 
  char *type = scm_symbol_to_string (typesym);

  int mtype;

  if (!g_ascii_strcasecmp (type, "message"))
    mtype = LM_MESSAGE_TYPE_MESSAGE;
  else if (!g_ascii_strcasecmp (type, "presence"))
    mtype = LM_MESSAGE_TYPE_PRESENCE;
  else if (!g_ascii_strcasecmp (type, "iq"))
    mtype = LM_MESSAGE_TYPE_IQ;
  else
    mtype = LM_MESSAGE_TYPE_MESSAGE;
  
  msg = lm_message_new (SCM_STRING_CHARS (to), mtype);
  lm_message_ref (msg);
  free (type);
  
  SCM_RETURN_NEWSMOB (lm_msg_tag, msg);
}

SCM
ls_lm_message_get_node (SCM msg)
{
  SCM_RETURN_NEWSMOB (lm_msg_node_tag, (LM_MSG(msg))->node);
}


/*
  LmMessageNode
*/

SCM
ls_lm_message_node_get_value (SCM node)
{
  return scm_from_locale_string (lm_message_node_get_value (LM_MSG_NODE(node)));
}

SCM
ls_lm_message_node_set_value (SCM node, SCM value)
{
  lm_message_node_set_value (LM_MSG_NODE(node), SCM_STRING_CHARS (value));
  return SCM_UNSPECIFIED;
}

SCM
ls_lm_message_node_get_attribute (SCM node, SCM name)
{
  return scm_from_locale_string (lm_message_node_get_attribute (LM_MSG_NODE(node),
						     SCM_STRING_CHARS (name)));
}

SCM
ls_lm_message_node_set_attribute (SCM node, SCM name, SCM value)
{
  lm_message_node_set_attribute (LM_MSG_NODE(node), SCM_STRING_CHARS (name),
				 SCM_STRING_CHARS (value));
  return SCM_UNSPECIFIED;
}

SCM
ls_lm_message_node_add_child (SCM node, SCM name, SCM value)
{
  LmMessageNode *child = lm_message_node_add_child (LM_MSG_NODE(node),
						   SCM_STRING_CHARS (name),
						   SCM_STRING_CHARS (value));
  SCM_RETURN_NEWSMOB (lm_msg_node_tag, child);
}

SCM
ls_lm_message_node_get_child (SCM node, SCM name)
{
  LmMessageNode *child = lm_message_node_get_child (LM_MSG_NODE(node),
						    SCM_STRING_CHARS (name));
  SCM_RETURN_NEWSMOB (lm_msg_node_tag, child);
}

SCM
ls_lm_message_node_find_child (SCM node, SCM name)
{
  LmMessageNode *child = lm_message_node_find_child (LM_MSG_NODE(node),
						    SCM_STRING_CHARS (name));
  SCM_RETURN_NEWSMOB (lm_msg_node_tag, child);
}

/*
  This is mainly useful for debugging
*/
SCM
ls_lm_message_node_to_string (SCM node)
{
  return scm_from_locale_string (lm_message_node_to_string (LM_MSG_NODE(node)));
}

/*
  Send a message
*/
SCM
ls_send (SCM msg)
{
  lm_connection_send (state.conn, LM_MSG(msg), NULL);
  return SCM_UNSPECIFIED;
}

static void
register_functions (void)
{
  scm_c_define_gsubr ("ls-send", 1, 0, 0, ls_send);
  
  scm_c_define_gsubr ("lm-message-new", 2, 0, 0, ls_lm_message_new);

  scm_c_define_gsubr ("lm-message-get-node", 1, 0, 0, ls_lm_message_get_node);

  scm_c_define_gsubr ("lm-message-node-get-value", 1, 0, 0, ls_lm_message_node_get_value);
  scm_c_define_gsubr ("lm-message-node-set-value", 2, 0, 0, ls_lm_message_node_set_value);
  scm_c_define_gsubr ("lm-message-node-get-attribute", 2, 0, 0, ls_lm_message_node_get_attribute);
  scm_c_define_gsubr ("lm-message-node-set-attribute", 3, 0, 0, ls_lm_message_node_set_attribute);
  scm_c_define_gsubr ("lm-message-node-add-child", 3, 0, 0, ls_lm_message_node_add_child);
  scm_c_define_gsubr ("lm-message-node-get-child", 2, 0, 0, ls_lm_message_node_get_child);
  scm_c_define_gsubr ("lm-message-node-find-child", 2, 0, 0, ls_lm_message_node_find_child);
  scm_c_define_gsubr ("lm-message-node-to-string", 1, 0, 0, ls_lm_message_node_to_string);
}

static void
register_types (void)
{
  lm_msg_tag = scm_make_smob_type ("lm-message", 0);
  scm_set_smob_free (lm_msg_tag, ls_lm_message_free);
  
  lm_msg_node_tag = scm_make_smob_type ("lm-message-node", sizeof (LmMessageNode *));
}

void
loudscream_init (void)
{
  register_functions ();
  register_types ();
}
