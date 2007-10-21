;;; mr-oxford.scm: Remembers and completes all known words from
;;;               disctionary and what ever you have typed.
;;; author: Anand Avati.
;;; Ported from mr-oxford.scm of Freehoo by Anand Babu

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

(use-modules (ice-9 rdelim))
(use-modules (ice-9 string-fun))


(display "Loading dictionary [/usr/share/dict/words]... ")
(let ((word-count 0))
  (catch 'system-error
	 (lambda ()
	   (for-each-line (lambda (word)
			    (ft-dict-prepend! word)
			    (set! word-count (1+ word-count)))
			  "/usr/share/dict/words"))
	 (lambda args #f))
  (display (string-append "[" (number->string word-count) "] words\n")))

;; Add sent-words to readline context"

(add-hook! ft-message-send-hook 
	   (lambda (to msg)
	     (map ft-dict-insert! (sentence->words msg))))

