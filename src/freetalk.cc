/*
  (C) 2005,2006 Freetalk core team <http://www.gnu.org/software/freetalk/>
  
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <glib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <loudmouth/loudmouth.h>
#include <guile/gh.h>
#include <getopt.h>

#ifdef HAVE_ARGP
#include <argp.h>
#endif /* HAVE_ARGP */

#include "freetalk.h"
#include "callbacks.h"
#include "util.h"
#include "roster.h"
#include "interpreter.h"
#include "commands.h"
#include "extensions.h"
#include "register.h"
#include "compat.h"

ft_state state;

GSList *key_bindings = NULL;

typedef struct {
  char key;
  char *command;
} key_binding;

static void
state_init (void)
{
  do_set_prompt ("~\\/~ ");
  state.roster = NULL;
  state.script = NULL;
  state.config_dir = (char *)calloc (1, strlen (getenv ("HOME")) + 1 + strlen (".freetalk") + 1);
  sprintf (state.config_dir, "%s/.freetalk", getenv ("HOME"));
  state.need_ssl = 1; /* turn on ssl by default */
  state.f_state = NULL;
  state.download_dirname = (char *)calloc (1, strlen (getenv ("HOME")) + 1 + strlen (".freetalk/downloads") + 1);
  sprintf (state.download_dirname, "%s/.freetalk/downloads", getenv ("HOME"));
  state.last = time(NULL);
}

scm_t_catch_handler catcher ()
{
  PRINTF("%s",_("No such command or buddy. See /help"));
  return NULL;
}

static void
process_line (char *line)
{
  char *eval_str;

  if (!line)
    return;

  if (*line) {
    const char *curr_buddy = do_get_current_buddy ();
    if (curr_buddy)
      if (strlen (line) > (strlen (curr_buddy) + 1))
	add_history (line);
  } else {
    do_set_current_buddy (NULL);
  }

  eval_str = strdup (line);

  state.async_printf = 0;
  if (interpreter (line) != 0) {
    gh_eval_str_with_catch (eval_str, (scm_t_catch_handler) catcher);
    scm_force_output (scm_current_output_port ());
  }
  g_free (eval_str);
  state.async_printf = 1;
}

gboolean
stdin_input_cb (GIOChannel *chan, GIOCondition *cond, gpointer conn)
{
  rl_callback_read_char ();
  return TRUE;
}

static int
insert_current_buddy (void)
{
  if (state.current_buddy) {
    rl_insert_text (state.current_buddy);
    rl_insert_text (" ");
    rl_redisplay ();
  }
  return 0;
}

void
interface_init (void)
{
  GIOChannel *chan;
  
  interpreter_init ();
  
  rl_callback_handler_install (state.prompt, process_line);
  rl_pre_input_hook = insert_current_buddy;
  rl_attempted_completion_function = ft_auto_complete;
  rl_completion_entry_function = complete_none;
  
  state.async_printf = 1;

  chan = g_io_channel_unix_new (0);
  g_io_add_watch (chan, G_IO_IN, (GIOFunc) stdin_input_cb, NULL);
}

const char *argp_program_version = PACKAGE_NAME " " PACKAGE_VERSION;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

static error_t
parse_opts (int key, char *arg, struct argp_state *_state)
{
  switch (key) {
  case 'j':
    do_set_jid (arg);
    break;
  case 's':
    state.script = arg;
    break;
  case 'r':
    ft_register ();
    break; /* not reached */
  default:
    /* hack to allow args to script */
    if (!state.script)
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static void
mode_init (void)
{
  int i = 1;

  while (i < state.argc) {
    if (!strcmp (state.argv[i], "-s")) {
      if (state.argv[i+1])
	state.script = state.argv[i+1];
    }
    i++;
  }
}

static void
args_init (void)
{
  struct {
    char *f[2];
  } f;
  static char doc[] = "Freetalk is a console based jabber client/bot with a readline interface and guile extensions";
  static char argp_doc[] = " ";
  static struct argp_option options [] = {
    {"jid", 'j', "JABBERID", 0, "user@domain Jabber ID" },
    {"script", 's', "SCRIPTFILE", 0, "Freetalk script" },
    {"register", 'r', 0, 0, "Register an account with a server"},
    { 0, }
  };
  static struct argp argp = { options, parse_opts, argp_doc, doc };

  argp_parse (&argp, state.argc, state.argv, 0, 0, &f);
}

static void
inner_main (void *closure, int argc, char **argv)
{
  check_first_run ();

  state_init ();
  mode_init ();

  extensions_init ();
  //loudscream_init ();
  
  if (!state.script) {
    load_default_config (); /* ~/.freetalk/freetalk.scm */
    args_init ();
    ft_load ("init.scm");
    ft_load ("login.scm");

    interface_init ();

    do_main_loop ();
  } else {
    ft_load (state.script);
  }
}

/*
 * Debugging hint: use LM_DEBUG=NET ./freetalk to see all the traffic
 * on the wire
 */

int
main (int argc, char **argv)
{
  /* Trigerring Gettext */
  setlocale(LC_ALL,"");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  state.argc = argc;
  state.argv = argv;
  scm_boot_guile (argc, argv, inner_main, 0);
  return 0;
}

/* function called when we receive a jabber:iq:version message */
int ft_msg_iq_version_cb(LmMessage *msg)
{
  const char *from = lm_message_node_get_attribute (msg->node, "from");
  const char *id   = lm_message_node_get_attribute (msg->node, "id");
  LmMessage *send_msg;
  LmMessageNode *query, *name, *version;
  
  send_msg = lm_message_new_with_sub_type (from,
				      LM_MESSAGE_TYPE_IQ,
				      LM_MESSAGE_SUB_TYPE_RESULT);
  
  lm_message_node_set_attribute (send_msg->node,
				 "id", id);
  query = lm_message_node_add_child (send_msg->node, "query", NULL);
  lm_message_node_set_attribute (query, 
				 "xmlns", "jabber:iq:version");
  
  name = lm_message_node_add_child (query, "name", PACKAGE_NAME);
  version = lm_message_node_add_child (query, "version", PACKAGE_VERSION);

  int result = lm_connection_send (state.conn, send_msg, NULL);
  lm_message_node_unref (version);
  lm_message_node_unref (name);
  lm_message_node_unref (query);
  lm_message_unref (send_msg);
  return result;
}

/* function called when we receive a jabber:iq:last message */
int ft_msg_iq_last_cb(LmMessage *msg)
{
  const char *from = lm_message_node_get_attribute (msg->node, "from");
  const char *id   = lm_message_node_get_attribute (msg->node, "id");
  LmMessage *send_msg;
  LmMessageNode *query;
  
  char seconds[256];
  snprintf( seconds, sizeof(seconds)-1, "%ld", time(NULL) - state.last );

  send_msg = lm_message_new_with_sub_type (from,
				      LM_MESSAGE_TYPE_IQ,
				      LM_MESSAGE_SUB_TYPE_RESULT);
  
  lm_message_node_set_attribute (send_msg->node,
				 "id", id);
  query = lm_message_node_add_child (send_msg->node, "query", NULL);
  
  lm_message_node_set_attribute (query, 
				 "seconds", seconds );
  lm_message_node_set_attribute (query, 
				 "xmlns", "jabber:iq:last" );
  
  int result = lm_connection_send (state.conn, send_msg, NULL);
  lm_message_node_unref (query);
  lm_message_unref (send_msg);
  return result;
}

int ft_key_bound (int count, int key)
{
  GSList *list;
  for (list = key_bindings; list; list = list->next) {
    key_binding *b = (key_binding*) list->data;
    if (b->key == key) {
      scm_c_eval_string (b->command);
      scm_force_output (scm_current_output_port ());
      return 0;
    }
  } 
  return 1;
}

void ft_bind_key (char key, char *command)
{
  GSList *list;
  for (list = key_bindings; list; list = list->next) {
    key_binding *b = (key_binding*) list->data;
    if (b->key == key) {
      char *old = b->command;
      b->command = strdup (command);
      free (old);
      return;
    }
  } 
  key_binding *b = (key_binding*) malloc (sizeof (key_binding));
  b->key = key;
  b->command = strdup (command);
  rl_bind_key (key, &ft_key_bound);
  key_bindings = g_slist_append (key_bindings, b);
}

void ft_bind_to_ctrl_key (char key, char *command)
{
  ft_bind_key (CTRL (key), command);
}
