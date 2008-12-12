bin_PROGRAMS = garden

garden_SOURCES = ../src/cloud.c ../src/cloud.h ../src/config.h ../src/display.c ../src/display.h ../src/display_init.c ../src/display_init.h ../src/ebullet.c ../src/ebullet.h ../src/eclass.c ../src/enemy.c ../src/enemy.h ../src/game.c ../src/game.h ../src/globvars.h ../src/input.c ../src/input.h ../src/level.c ../src/level.h ../src/main.c ../src/menu.c ../src/menu.h ../src/palette.c ../src/palette.h ../src/pbullet.c ../src/pbullet.h ../src/sound.c ../src/sound.h ../src/stuff.c ../src/stuff.h ../src/system.h

# This is telling the program where to find datafiles during compilation. Define the datafile directory name in configure.ac if you don't like it
#AM_CPPFLAGS = -D'DATADIR(append)=TO_STRING($(datadir)/@DATADIR_NAME@/append)'
if WINDOWS_VERSION
#no datadir #defined
garden_SOURCES += ../src/garden.rc
garden_LDFLAGS = -static
else #unix version :-)
AM_CPPFLAGS = -D'DATADIR="$(datadir)/@DATADIR_NAME@"'
endif

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@ 
