## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

gtk_visualizationdocdir = ${prefix}/doc/gtk_visualization
gtk_visualizationdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(gtk_visualizationdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gtk_visualizationdocdir)
