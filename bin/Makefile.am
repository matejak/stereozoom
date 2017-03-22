NUL =

bin_PROGRAMS = stereozoom

if WANT_GUI
bin_PROGRAMS += wxstereozoom
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
	src/stereozoom.h \
	src/stereozoom.cpp \
	src/stereozoom_internal.h \
	$(NUL)

include_HEADERS = \
	src/Change.h \
	src/Image_group.h \
	src/Rectangle.h \
	src/Image.h \
	src/stereozoom.h \
	$(NUL)

libstereozoom_la_LIBADD = $(allegro_LIBS)

stereozoom_SOURCES = \
	src/Change.h \
	src/Rectangle.h \
	src/Image.h \
	src/Image_group.h \
	src/Image_group.h \
	src/Rectangle.h \
	src/stereozoom.h \
	src/frontend.cpp \
	src/stereozoom_internal.h \
	$(NUL)

stereozoom_LDADD = libstereozoom.la $(allegro_LIBS) 

wxstereozoom_SOURCES = \
	src/Change.h \
	src/Rectangle.h \
	src/Image.h \
	src/Image_group.h \
	src/Base_gui.h \
	src/Base_gui.cpp \
	src/Main_gui.h \
	src/Main_gui.cpp \
	src/wxStereozoom.h \
	src/wxStereozoom.cpp \
	src/stereozoom_internal.h \
	$(NUL)

if WINDOWS_VERSION
wxstereozoom_SOURCES += \
	src/stereozoom-msw.rc \
	$(NUL)
endif

wxstereozoom_LDADD = libstereozoom.la $(WX_LIBS) $(allegro_LIBS)

AM_CPPFLAGS = $(WX_CPPFLAGS) 
# the wxstereozoom needs this to display the icon in the About dialog
AM_CPPFLAGS += -D'DATADIR(append)=TO_STRING($(datadir)/@DATA_SUBDIR@/append)'

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@
