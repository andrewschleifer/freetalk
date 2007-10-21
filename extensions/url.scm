;;; url.scm: pipe session to urlview

;;; author: Anand V. Avati <avati@zresearch.com>

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

(define urlview-cmd "urlview ")

;;; history path
(define url-history-path "HISTORY PATH WILL BE SET IN POST-STARTUP")
(define url-session-file "SESSION FILE NAME WILL BE SET IN POST-STARTUP")

(define (url-post-startup success)
  "post startup hook"
  (if success
    (begin
    (set! url-history-path (string-append
		      (ft-get-config-dir)
		      "/history/"
		      (ft-get-jid)))
    (set! url-session-file (string-append history-path "/SESSION")))))

(add-hook! ft-login-hook url-post-startup)

(define (/urlview args)
  "urlview history session"
;  (set! args (list->strlist args))
  (if (= (string-length args) 0)
      (system (string-append urlview-cmd url-session-file))
;      (if (= (string-length args) 1)
	  (system (string-append urlview-cmd url-history-path "/" args))))

(add-command! /urlview "/urlview" "/urlview [BUDDY]" "handle URLs")
