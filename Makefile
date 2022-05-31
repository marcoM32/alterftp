CC = gcc
WR = windres
CFLAGS = -Wall

.PHONY : clean-win clean

# Gnu/Linux build

alterftp : alterftp.c config_file.c debug.c
	$(CC) $< -o $@ $(CFLAGS)

install : alterftp
	cp $^ /usr/bin/$^
	chmod a+x /usr/bin/$^

clean :
	@rm -f *~ alterftp

# Windows build

resource.o : resource.rc
	$(WR) resource.rc -o $@

alterftp-win : alterftp.c config_file.c debug.c resource.o
	$(CC) alterftp.c resource.o -o $@ $(CFLAGS)

clean-win :
	@erase /F *.exe resource.o

test1 : alterftp
	./$^ help

test2 : alterftp
	./$^ init file1 file2

test3 : alterftp
	./$^ send

test4 : alterftp
	./$^ mkdir file1 file2 file3

test5 : alterftp
	./$^ rmdir file1 file2 file3

%.txt :
	@touch $@

test6 : alterftp file1.txt file2.txt
	./$< send file1.txt file2.txt testing/test.txt
	@rm -f *.txt

test7 : alterftp
	./$< delete file1.txt file2.txt testing/test.txt

test : test1 test3 test4 test5 test6
