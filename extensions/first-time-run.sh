#!/bin/sh
#
# first-time-run.sh: prepare ~/.freetalk directory
# author: Anand Babu <ab@gnu.org.in>
# copyright 2005 FreeTalk Core Team
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2, or (at
# your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA

_init ()
{
    [ -z "${HOME}" ] && {
	echo "FATAL: \$HOME environment variable not present" >&2;
	return 1;
    }
    CONFIG_BASE="${HOME}/.freetalk";
    return 0;
}

normal ()
{
    [ -d "${CONFIG_BASE}" ] && return 0;
    echo "Welcome to freetalk! Running for the first time?";
    echo "Performing some one-time initializations ... ";
    mkdir -p "${CONFIG_BASE}" || {
	echo "FATAL: Could not create ${HOME}/.freetalk directory" >&2;
	return 1;
    }
    cat >> "${CONFIG_BASE}/freetalk.scm" <<EOF
; (ft-set-jid! "anand.avati@gmail.com")
; (ft-set-server! "talk.google.com")
; (ft-set-password! "p455w0rd")
; (ft-set-sslconn! #t)
; (ft-set-port! 5223)
; (add-hook! ft-login-hook
;            (lambda (status)
;              (if status
;                  (begin
;                    (ft-set-prompt! "~\\\/~ ")
;                    (ft-set-status-msg! "chat I use GNU Freetalk")))))
;;;
;;; Let ctrl-a display full roster, ctrl-e who i am, ctrl-h online buddies
; (ft-bind-to-ctrl-key #\a "(/who \"all\")")
; (ft-bind-to-ctrl-key #\e "(/whoami \"\")")
; (ft-bind-to-ctrl-key #\h "(/who \"\")")
EOF
    mkdir -p "${CONFIG_BASE}/extensions";
    mkdir -p "${CONFIG_BASE}/downloads";
    mkdir -p "${CONFIG_BASE}/history";
}

reset ()
{
    BACKUP=0;

    while [ -d "${HOME}/.freetalk-backup-${BACKUP}" ]
    do
      BACKUP=$((${BACKUP} + 1 ));
    done

    mv -v "${HOME}/.freetalk" "${HOME}/.freetalk-backup-${BACKUP}";
    echo "Creating fresh state ${HOME}/.freetalk";
    normal >/dev/null;

    echo -e "\nNOTE: Archieved previous state to ${HOME}/.freetalk-backup-${BACKUP} !!!\n";
}

main ()
{
    local FUNC;

    [ -z "$1" ] && FUNC=normal;
    [ "$1" = "reset" ] && FUNC=reset;

    ${FUNC};
}
_init "$@" && main "$@"