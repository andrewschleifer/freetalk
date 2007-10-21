#  build and install the .info pages
info_TEXINFOS = freetalk.texi
freetalk_TEXINFOS = authors.texi gpl.texi fdl.texi permissions.texi

man_MANS = freetalk.1
 
freetalk.1: freetalk.man
	-cp freetalk.man freetalk.1
EXTRA_DIST = freetalk.man
CLEANFILES = *~ 
DISTCLEANFILES = .deps/*.P *.1 *.info *vti


