;;; dist-buddy.scm: talk to dict buddy
;;; author: Anand Avati <avati@zresearch.com>
;;; copyright 2005 FreeTalk Core Team

;;; This program is free software; you can redistribute it and/or
;;; modify it under the terms of the GNU General Public License as
;;; published by the Free Software Foundation; either version 2, or (at
;;; your option) any later version.
;;; 
;;; This program is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;;; General Public License for more details.
;;; 
;;; You should have received a copy of the GNU General Public License
;;; along with this program; if not, write to the Free Software
;;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA


(define (usual-crap get-fn set-fn! get-msg args)
  (if (= (string-length args) 0)
      (ft-display (string-append get-msg (get-fn)))
      (set-fn! args)))

(define (/prompt args)
  (usual-crap ft-get-prompt ft-set-prompt! "Current prompt: " args))
(add-command! /prompt "/prompt" "/prompt [NEWPROMPT]" "set command line prompt")

(define (/status args)
  (usual-crap ft-get-status-msg ft-set-status-msg! "Current status: " args))
(add-command! /status "/status" "/status [online|away|chat|xa|dnd] [MESSAGE]" "set status message")

(define (/server args)
  (usual-crap ft-get-server ft-set-server! "Current server: " args))
(add-command! /server "/server" "/server [HOST|IP]" "set server for next /connect")

(define (/jid args)
  (usual-crap ft-get-jid ft-set-jid! "Current JID: " args))
(add-command! /jid "/jid" "/jid [USER@SERVER]" "set jabber id for next /connect")

(define (/password args)
  (ft-set-password! (getpass "Password: ")))
(add-command! /password "/password" "/password" "set password for next /connect")

(define (/port args)
  (usual-crap 
   (lambda () (number->string (ft-get-port))) 
   (lambda (str_port) (ft-set-port! (string->number str_port)))
   "Current Port (0 = default): "
   args))
(add-command! /port "/port" "/port [PORT]" "set server port for next /connect")

(add-command! (lambda (str) 
		(if (> (string-length str) 0)
		     (ft-load (sans-surrounding-whitespace str))
		     (ft-display (_ "usage: /load [FILE]"))))
	      "/load" "/load [FILE]" "load an extension file")

(add-command! (lambda (args) (ft-reset-fs-state!)) "/setup" "/setup" "Write fresh ~/.freetalk")
