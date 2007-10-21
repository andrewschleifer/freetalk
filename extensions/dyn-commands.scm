;;; dyn-commands.scm: Dynamic Commands support.
;;; author: Anand Babu <ab@zresearch.com>
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

(define dynamic-command-registry '())

(define (add-command! func command syntax description)
  "Adds a dynamic command"
  (if (procedure? func)
      (set! dynamic-command-registry 
	    (assoc-set! dynamic-command-registry
			command
			(list func syntax description)))
      (display (string-append (_ "freetalk: error: command [")
			      command
			      (_ "] not bound to any procedure\n")))))

(define (remove-command! command)
  "Removes a dynamic command"
  (set! dynamic-command-registry 
	(assoc-remove! dynamic-command-registry
		       command)))

(define (dynamic-command-proc command args)
  ((lambda (cmd-entry)
     (if cmd-entry
        (begin
          ((car cmd-entry) args)
          (ft-hook-return)))) (assoc-ref dynamic-command-registry command)))
(add-hook! ft-command-hook dynamic-command-proc)
	
(define (help args)
  "display help message"
  (let ((command-name (sans-surrounding-whitespace args))
	(command-doc (assoc-ref dynamic-command-registry (sans-surrounding-whitespace args))))
    (if (not (string-null? args))
	(if (not (list? command-doc))
	    (display (_ "no such command\n"))
	    (display (string-append command-name
				    " - "
				    (cadr command-doc)
				    "\n\t"
				    (caddr command-doc)
				    "\n"
				    )))
	(for-each (lambda (command-entry)
		    (display (string-append (car command-entry)
					    " - "
					    (caddr command-entry)
					    "\n\t"
					    (cadddr command-entry)
					    "\n"
					    )))
		  (sort dynamic-command-registry 
			(lambda (a b) 
			  (string<? (car a) (car b))))))))

(add-command! help "help" "help [COMMAND]" "show help")
(add-command! help "/help" "/help [COMMAND]" "show help")