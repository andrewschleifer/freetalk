;; Sample ~/.freetalk/freetalk.scm
;;
;; It sets connection parameters and tries to connect on
;; starting freetalk

; (and (string=? (ft-get-jid) "")
;      (ft-set-jid! "anand.avati@gmail.com")
;      (ft-set-password! "p4ssw0rd")
;      (ft-set-sslconn! #t)
;      (ft-set-prompt! "freetalk: ")
;      (ft-set-server! "talk.google.com"))
; 
; (add-hook! ft-login-hook
;            (lambda (status)
;              (if status
;                  (begin
;                    (ft-set-prompt! (string-append 
;                                     (car (string-split (ft-get-jid) #\@))
;                                     ": "))
;                    (ft-set-status-msg! "online")))))

;;;
;;; Let ctrl-a display full roster, ctrl-e who i am, ctrl-h online buddies
; (ft-bind-to-ctrl-key #\a "(/who \"all\")")
; (ft-bind-to-ctrl-key #\e "(/whoami \"\")")
; (ft-bind-to-ctrl-key #\h "(/who \"\")")
