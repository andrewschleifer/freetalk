;;; pipe.scm: pipe the output of program to remote buddy.
;;; author: Anand Babu <ab@zresearch.com>
;;; Copyright (c) 2005 Z RESEARCH, Inc. http://www.zresearch.com

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

;;; this pipe.scm extension will be automatically loaded through
;;; init.scm

(use-modules (ice-9 popen))

(define (send-message-pipe buddy cmd)
  "send the output of PROGRAM to remote buddy"
  (let*
      ((port (open-input-pipe cmd))
       (line (read-line port)))
    (while (not (eof-object? line))
	   (ft-send-message buddy line)
	   (display line)(newline)
;	   (usleep 100000) ; for proper message sequencing
	   (set! line (read-line port)))
    (close-pipe port)))

(define (/pipe args)
  "dynamic command interface to pipe facility"
  (let* ((args-list (split-discarding-char #\space args (lambda (x y) (list x y))))
	 (buddy     (car args-list))
	 (cmd       (cadr args-list)))
    (if (> (string-length cmd) 0)
	(send-message-pipe buddy cmd)
	(ft-display (_ "usage: /pipe BUDDY COMMAND [OPTIONS]")))))

(add-command! /pipe "/pipe" 
	      "/pipe BUDDY COMMAND [OPTIONS]"
	      "send the output of COMMAND to BUDDY")
