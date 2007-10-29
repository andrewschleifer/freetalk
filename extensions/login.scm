;;; login.scm: connect and login to the server
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

(use-modules (ice-9 string-fun))
;(use-modules (ice-9 readline))
;(activate-readline)

(and (string=? (ft-get-jid) "")
     (ft-set-jid! (sans-surrounding-whitespace 
		   (and (display "Jabber ID: ") 
			(read-line)))))

;; check if both user and domain are present
(if (not (string-rindex (ft-get-jid) #\@))
    (begin
      (display (string-append "freetalk: Jabber ID ["
			      (ft-get-jid)
			      "] should contain full user@domain\n"))
      (exit 1)))

(define (domain->server domain)
  (cond ((string=? domain "gmail.com") "talk.google.com")
	((string=? domain "google.com") "talk.google.com")
	(else domain)))

(or (string=? (ft-get-jid) "")
    (and (string=? (ft-get-server) "")
	 (split-discarding-char #\@ (ft-get-jid) 
				(lambda (jid domain) 
				  (ft-set-server! (domain->server domain))))))

(and (not (string=? (ft-get-jid) "")) (not (string=? (ft-get-server) ""))
     (ft-connect))

(add-hook! ft-login-hook (lambda (success)
			   (and success
				(ft-set-prompt! (string-append 
						 (ft-get-jid)
						 "> ")))))

(add-hook! ft-disconnect-hook (lambda (reason)
				(ft-set-prompt! "~\\/~ ")))
