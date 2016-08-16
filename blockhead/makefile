compiler = gcc-6
options = -c -O3 -Wall -Werror -Wextra 
linker = gcc-6

all_ascii: __main.o libblockhead_ascii.a
	$(linker) __main.o libblockhead_ascii.a -o blockhead
	
all_utf-8:  __main.o libblockhead_utf-8.a
	$(linker) __main.o libblockhead_utf-8.a -o blockhead
	
__main.o: main.c
	$(compiler) $(options) main.c -o __main.o
# ----------------------------------------------------------------------------------------
libblockhead_ascii.a: __computerNext.o __database.o __b-tree.o
	ar cr libblockhead_ascii.a __computerNext.o __database.o __b-tree.o
	
__computerNext.o: computerNext.h database.h b-tree.h computerNext.c database.c b-tree.c
	$(compiler) $(options) computerNext.c -o __computerNext.o
	
__database.o: database.h b-tree.h database.c b-tree.c
	$(compiler) $(options) database.c -o __database.o
	
__b-tree.o: b-tree.h b-tree.c
	$(compiler) $(options) b-tree.c -o __b-tree.o
	
# ----------------------------------------------------------------------------------------
libblockhead_utf-8.a: __computerNext_utf-8.o __database_utf-8.o __b-tree_utf-8.o __insertion-control_utf-8.o
	ar cr libblockhead_utf-8.a __computerNext_utf-8.o __database_utf-8.o __b-tree_utf-8.o __insertion-control_utf-8.o
	
__computerNext_utf-8.o: computerNext_utf-8.h database_utf-8.h b-tree_utf-8.h computerNext_utf-8.c database_utf-8.c b-tree_utf-8.c
	$(compiler) $(options) computerNext_utf-8.c -o __computerNext_utf-8.o
	
__database_utf-8.o: database_utf-8.h b-tree_utf-8.h database_utf-8.c b-tree_utf-8.c
	$(compiler) $(options) database_utf-8.c -o __database_utf-8.o
	
__b-tree_utf-8.o: b-tree_utf-8.h b-tree_utf-8.c
	$(compiler) $(options) b-tree_utf-8.c -o __b-tree_utf-8.o
	
__insertion-control_utf-8.o: insertion-control_utf-8.h insertion-control_utf-8.c
	$(compiler) $(options) insertion-control_utf-8.c -o __insertion-control_utf-8.o
# ----------------------------------------------------------------------------------------
clean:
	rm *.o
	rm *.a
	rm *.tar.gz
	
tar: libblockhead_utf-8.a libblockhead_ascii.a
	tar -czvf blockhead.tar.gz *.c *.a *.h makefile zdf-win.txt base.txt