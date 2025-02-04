CC = gcc
WR = windres
CFLAGS = -Wall

.PHONY : clean-win clean

# Gnu/Linux build

alterftp : alterftp.c config.c
	$(CC) alterftp.c config.c -o "alterftp#" $(CFLAGS)

install :
	cp "alterftp#" /usr/bin/alterftp#
	chmod a+x /usr/bin/alterftp#

clean :
	@rm -f *~ "alterftp#"

# Windows build

resource.o : resource.rc
	$(WR) resource.rc -o $@

alterftp-win : resource.o alterftp.c config.c
	$(CC) alterftp.c config.c resource.o -o "alterftp#win.exe" $(CFLAGS)

clean-win :
	@erase /F *.exe resource.o
