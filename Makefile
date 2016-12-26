CFLAGS = -ansi -pedantic -Wall -Werror -Wundef -Wstrict-prototypes -g -fPIC -std=c99 -O2 -march=native
DEPS = -lcurl -ljansson

libtgapi.so: src/tgapi.o src/tgparse.o
	$(CC) $^ -shared -o src/$@ $(DEPS)

docs:
	doxygen doxygen/Doxyfile

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

install: libtgapi.so
	cp src/libtgapi.so /usr/lib
	cp src/tgapi.h /usr/include
	cp src/tgtypes.h /usr/include
	cp src/tgparse.h /usr/include

uninstall:
	rm /usr/lib/libtgapi.so
	rm /usr/include/tgapi.h
	rm /usr/include/tgtypes.h
	rm /usr/include/tgparse.h

clean:
	rm -f src/*.o
