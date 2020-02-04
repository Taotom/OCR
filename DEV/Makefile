# Makefile

CC = gcc ../Segmentation/rlsa.c ../Segmentation/segmentation.c ../Segmentation/build.c ../Processing/processing.c ../Processing/pixel_operations.c ../Struct/matrix.c ../Struct/tree.c -lSDL -lSDL_image -Wall
CPPFLAGS =
CFLAGS =
LDFLAGS =
LDLIBS =

segmentation:
	mkdir Para
	mkdir Line
	mkdir Word
	mkdir Char
	make main_seg

clean_seg:
	rm -rf Para
	rm -rf Line
	rm -rf Word
	rm -rf Char	
	rm main_seg
	rm text.txt
	

clean:	
	rm *.o
	rm *.d
	rm *.gch

# END
