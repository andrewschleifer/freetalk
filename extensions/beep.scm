;;; beep.scm: Beep on specific events.
;;; author: Anand Babu <ab@zresearch.com>
;;; copyright 2005 Z RESEARCH, Inc. http://www.zresearch.com

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

(if (= (system "beep -f 1 -l 0 >> /dev/null 2>&1") 0)
    (add-hook! ft-message-receive-hook 
	       (lambda (time from nickname message)
		 (system "beep -f 600 -l 10; beep -f 800 -l 10; beep -f 200 -l 10&")))
    (add-hook! ft-message-receive-hook 
	       (lambda (time from nickname message)
		 (ft-beep 200 20))))

