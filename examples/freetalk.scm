;; Sample ~/.freetalk/freetalk.scm
;;
;; It sets connection parameters and tries to connect on
;; starting freetalk

(and (string=? (ft-get-jid) "")
     (ft-set-jid! "anand.avati@gmail.com")
     (ft-set-password! "f00b4r")
     (ft-set-sslconn! #t)
     (ft-set-server! "talk.google.com"))

(add-hook! ft-login-hook
	   (lambda (status)
	     (if status
		 (ft-set-status-msg! 
		  "chat i use freetalk http://freetalk.nongnu.org"))))



