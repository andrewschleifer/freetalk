/*
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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <readline/readline.h>

typedef struct jid
{
  char *node;
  char *domain;
  char *resource;
} jid_t;

void parse_jid_string (char *jid_str, jid_t *jid);
char *second_word (char *full_line);
char *first_word (char *full_line);
void async_printf (const char *fmt, va_list ap);
void sync_printf (const char *fmt, va_list ap);
#define PRINTF do_printf
void check_first_run (void);

#endif /* __UTIL_H__ */
