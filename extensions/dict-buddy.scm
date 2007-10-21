;;; dist-buddy.scm: talk to dict buddy
;;; author: Anand Babu <ab@gnu.org.in>
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

; (fh-register-command! '("dict" "dict [OPTIONS]\n\t- ask dictionary server"))

(define (/dict args)
  (if (= (string-length args) 0)
      (system "dict --help")
      (begin
	; (fh-set-current-target-buddy! "dict" "send")  
	(system (string-append "dict -P more \"" args "\"")))))
	      
(add-command! /dict "/dict" "/dict [OPTIONS] [WORD]" "lookup in dictionary")

