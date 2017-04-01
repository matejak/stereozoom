NUL =

pkglib_LTLIBRARIES = 
bin_PROGRAMS = stereozoom2

if wx_BACKEND
bin_PROGRAMS += gstereozoom2
endif

lib_LTLIBRARIES = libstereozoom2.la

libstereozoom2_la_SOURCES = \
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
	src/stereozoom2.h \
	src/stereozoom2.cpp \
	src/stereozoom2_internal.h \
	$(NUL)

if allegro4_BACKEND
pkglib_LTLIBRARIES += allegro4.la

allegro4_la_SOURCES = \
	src/module_interface.h \
	src/AllegroImage.h \
	src/AllegroImage.cpp \
	src/AllegroImageGroup.h \
	src/AllegroImageGroup.cpp \
	$(NUL)

allegro4_la_LDFLAGS = -module
allegro4_la_CPPFLAGS = -DMODULE_NAME=allegro4
allegro4_la_LIBADD = $(allegro4_LIBS)
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
	src/stereozoom2.h \
	$(NUL)

libstereozoom2_la_LIBADD = -lltdl

stereozoom2_SOURCES = \
	src/Change.h \
	src/Rectangle.h \
	src/Image.h \
	src/Image_group.h \
	src/Image_group.h \
	src/Rectangle.h \
	src/stereozoom2.h \
	src/frontend.cpp \
	src/stereozoom2_internal.h \
	$(NUL)

stereozoom2_LDADD = libstereozoom2.la

gstereozoom2_SOURCES = \
	src/Change.h \
	src/Rectangle.h \
	src/Image.h \
	src/Image_group.h \
	src/Base_gui.h \
	src/Base_gui.cpp \
	src/Main_gui.h \
	src/Main_gui.cpp \
	src/Gstereozoom2.h \
	src/Gstereozoom2.cpp \
	src/stereozoom2_internal.h \
	$(NUL)

if WINDOWS_VERSION
gstereozoom2_SOURCES += \
	src/stereozoom2-msw.rc \
	$(NUL)
endif

gstereozoom2_LDADD = libstereozoom2.la $(WX_LIBS)

AM_CPPFLAGS = $(WX_CPPFLAGS) 
# the gstereozoom needs this to display the icon in the About dialog
AM_CPPFLAGS += -D'DATADIR(append)=TO_STRING($(datadir)/@DATA_SUBDIR@/append)'

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@
