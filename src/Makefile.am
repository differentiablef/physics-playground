## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GTK_VISUALIZATION_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gtk_visualization

gtk_visualization_SOURCES = \
	main.cc \
	display-area.cc \
	display-area.h \
	spatial-object.cc \
	spatial-object.h \
	common.h \
	model.cc \
	model.h \
	particle.cc \
	particle.h \
	edge.cc \
	edge.h \
	math-common.h \
	math-common.cc

gtk_visualization_LDFLAGS = 

gtk_visualization_LDADD = $(GTK_VISUALIZATION_LIBS)


