bin_PROGRAMS = stereozoom2

if WANT_GUI
bin_PROGRAMS += gstereozoom2
endif

stereozoom2_SOURCES = ../src/Rectangle.h ../src/Image.h ../src/Image_group.h ../src/Image_group.h ../src/Rectangle.h ../src/stereozoom2.h ../src/frontend.cpp ../src/stereozoom2_internal.h
stereozoom2_LDADD = ../lib/libstereozoom2.la $(allegro_LIBS) 

gstereozoom2_SOURCES = ../src/Rectangle.h ../src/Image.h ../src/Image_group.h ../src/Base_gui.h ../src/Base_gui.cpp ../src/Main_gui.h ../src/Main_gui.cpp ../src/Gstereozoom2.h ../src/Gstereozoom2.cpp ../src/stereozoom2_internal.h 

if WINDOWS_VERSION
gstereozoom2_SOURCES += ../src/stereozoom2.rc
endif

gstereozoom2_LDADD = ../lib/libstereozoom2.la $(WX_LIBS) $(allegro_LIBS)

AM_CPPFLAGS = $(WX_CPPFLAGS) 
# the gstereozoom needs this to display the icon in the About dialog
AM_CPPFLAGS += -D'DATADIR(append)=TO_STRING($(datadir)/@DATA_SUBDIR@/append)'

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@
