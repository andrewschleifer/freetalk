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

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <regex.h>

void interpreter_init (void);
int interpreter (char *line);

static inline char * complete_none (const char *txt, int state)
{
  return NULL;
}

char ** ft_auto_complete (const char *txt, int start, int end);

#endif /* __INTERPRETER_H__ */
