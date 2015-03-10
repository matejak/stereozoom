NUL =
SPTH = $(top_srcdir)/src

bin_PROGRAMS = stereozoom2

if WANT_GUI
bin_PROGRAMS += gstereozoom2
endif

lib_LTLIBRARIES = libstereozoom2.la

libstereozoom2_la_SOURCES = \
	$(SPTH)/Image_group.h \
	$(SPTH)/Image_group.cpp \
	$(SPTH)/Rectangle.h \
	$(SPTH)/Rectangle.cpp \
	$(SPTH)/Image.h \
	$(SPTH)/Image.cpp \
	$(SPTH)/stereozoom2.h \
	$(SPTH)/stereozoom2.cpp \
	$(SPTH)/stereozoom2_internal.h \
	$(NUL)

include_HEADERS = \
	$(SPTH)/Image_group.h \
	$(SPTH)/Rectangle.h \
	$(SPTH)/Image.h \
	$(SPTH)/stereozoom2.h \
	$(NUL)

libstereozoom2_la_LIBADD = $(allegro_LIBS)

stereozoom2_SOURCES = \
	$(SPTH)/Rectangle.h \
	$(SPTH)/Image.h \
	$(SPTH)/Image_group.h \
	$(SPTH)/Image_group.h \
	$(SPTH)/Rectangle.h \
	$(SPTH)/stereozoom2.h \
	$(SPTH)/frontend.cpp \
	$(SPTH)/stereozoom2_internal.h \
	$(NUL)

stereozoom2_LDADD = libstereozoom2.la $(allegro_LIBS) 

gstereozoom2_SOURCES = \
	$(SPTH)/Rectangle.h \
	$(SPTH)/Image.h \
	$(SPTH)/Image_group.h \
	$(SPTH)/Base_gui.h \
	$(SPTH)/Base_gui.cpp \
	$(SPTH)/Main_gui.h \
	$(SPTH)/Main_gui.cpp \
	$(SPTH)/Gstereozoom2.h \
	$(SPTH)/Gstereozoom2.cpp \
	$(SPTH)/stereozoom2_internal.h \
	$(NUL)

if WINDOWS_VERSION
gstereozoom2_SOURCES += \
	$(SPTH)/stereozoom2-msw.rc \
	$(NUL)
endif

gstereozoom2_LDADD = libstereozoom2.la $(WX_LIBS) $(allegro_LIBS)

AM_CPPFLAGS = $(WX_CPPFLAGS) 
# the gstereozoom needs this to display the icon in the About dialog
AM_CPPFLAGS += -D'DATADIR(append)=TO_STRING($(datadir)/@DATA_SUBDIR@/append)'

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@
