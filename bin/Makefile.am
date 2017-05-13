NUL =

pkglib_LTLIBRARIES = 
bin_PROGRAMS = stereozoom

if WANT_GUI
if wx_BACKEND
bin_PROGRAMS += wxstereozoom

wxstereozoom_LDADD = libstereozoom.la $(WX_LIBS)

wxstereozoom_CPPFLAGS = $(WX_CPPFLAGS) 
# the wxstereozoom needs this to display the icon in the About dialog
wxstereozoom_CPPFLAGS += -D'DATADIR(append)=TO_STRING($(datadir)/@DATA_SUBDIR@/append)'
endif
if gtk_BACKEND
bin_PROGRAMS += gstereozoom

gstereozoom_LDADD = $(GTK_LIBS)

gstereozoom_CPPFLAGS = $(GTK_CFLAGS) 
gstereozoom_VALAFLAGS = --pkg gtk+-3.0 --target-glib 2.38 --gresources $(abs_top_srcdir)/rsrc/gstereozoom.xml
# the gstereozoom needs this to display the icon in the About dialog

gstereozoom_CPPFLAGS += -D'DATADIR(append)=TO_STRING($(datadir)/@DATA_SUBDIR@/append)'
gstereozoom_LDFLAGS = -rdynamic
endif
endif

lib_LTLIBRARIES = libstereozoom.la

libstereozoom_la_SOURCES = \
	src/Change.h \
	src/Change.cpp \
	src/Image_group.h \
	src/Image_group.cpp \
	src/Rectangle.h \
	src/Rectangle.cpp \
	src/Image.h \
	src/Image.cpp \
	src/MessageService.h \
	src/MessageService.cpp \
	src/stereozoom.h \
	src/stereozoom_internal.h \
	$(NUL)

if allegro4_BACKEND
pkglib_LTLIBRARIES += allegro4.la

allegro4_la_SOURCES = \
	src/module_interface.h \
	src/Allegro4.h \
	src/Allegro4.cpp \
	$(NUL)

allegro4_la_LDFLAGS = -module
allegro4_la_CPPFLAGS = -DMODULE_NAME=allegro4
allegro4_la_LIBADD = $(allegro4_LIBS)
endif

if allegro5_BACKEND
pkglib_LTLIBRARIES += allegro5.la

allegro5_la_SOURCES = \
	src/module_interface.h \
	src/Allegro5.h \
	src/Allegro5.cpp \
	$(NUL)

allegro5_la_LDFLAGS = -module
allegro5_la_CPPFLAGS = -DMODULE_NAME=allegro5
allegro5_la_LIBADD = $(allegro5_LIBS)
endif

if DevIL_BACKEND
pkglib_LTLIBRARIES += devil.la

devil_la_SOURCES = \
	src/module_interface.h \
	src/ILLoader.h \
	src/ILLoader.cpp \
	$(NUL)

devil_la_LDFLAGS = -module
devil_la_CPPFLAGS = -DMODULE_NAME=devil
devil_la_LIBADD = $(DevIL_LIBS)
endif

include_HEADERS = \
	src/Change.h \
	src/Image_group.h \
	src/Rectangle.h \
	src/Image.h \
	src/stereozoom.h \
	$(NUL)

libstereozoom_la_LIBADD = -lltdl

stereozoom_SOURCES = \
	src/Change.h \
	src/Rectangle.h \
	src/Image.h \
	src/Image_group.h \
	src/Image_group.h \
	src/Rectangle.h \
	src/stereozoom.h \
	src/stereozoom.cpp \
	src/frontend.cpp \
	src/stereozoom_internal.h \
	$(NUL)

stereozoom_LDADD = libstereozoom.la

wxstereozoom_SOURCES = \
	src/Base_gui.h \
	src/Base_gui.cpp \
	src/Main_gui.h \
	src/Main_gui.cpp \
	src/wxStereozoom.h \
	src/wxStereozoom.cpp \
	src/stereozoom.cpp \
	$(NUL)


gstereozoom-resources.c: ../rsrc/gstereozoom.xml ../rsrc/gstereozoom-main_window.glade ../rsrc/gstereozoom-image_panel.glade
	glib-compile-resources $< --target=$@ --sourcedir=$(srcdir)/rsrc --c-name _my --generate-source

gstereozoom_SOURCES = \
	src/gstereozoom.vala \
	gstereozoom-resources.c \
	$(NUL)

if WINDOWS_VERSION
wxstereozoom_SOURCES += \
	src/stereozoom-msw.rc \
	$(NUL)
endif

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@
