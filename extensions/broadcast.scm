;;; broadcast.scm: broadcast messages to all the roster.
;;; Author: Harshavardhana Ranganath <harsha@80x25.org>

;;; Copyright (C) 2006 by Freetalk Core Team 
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
;;;
;;; broadcast.scm should be automatically loaded by init.scm
;;; usage:
;;; To broadcast a message to everbody in the list
;;;  /broadcast Hello Everybody
;;; (or) simply,
;;;  * Hello Everybody

(use-modules (ice-9 string-fun))

(define (/broadcast message)
  " Broadcast messages to all the roster "
  (let ((online-buddies (list)))
    (for-each (lambda (roster-item)
		(and (cadr roster-item)
		     (set! online-buddies 
			   (append online-buddies 
				   (list (car roster-item))))))
	      (ft-get-roster-list))
    (if (> (string-length message) 0)
	(send-messages-to-all online-buddies
			      message)
	(ft-display (_ "usage: /broadcast [MESSAGE]")))))

(add-command! /broadcast "/broadcast" "/broadcast [MESSAGE]" "Send messages to all buddies")
(add-command! /broadcast "*" "* [MESSAGE]" "Send messages to all the buddies") 