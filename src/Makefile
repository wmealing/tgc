CFLAGS = -ansi -pedantic -Wall -Werror -Wundef -Wstrict-prototypes -g -fPIC -std=c11 -O2 -march=native
DEPS = -lcurl -ljansson

libtgapi.so: tgapi.o tgparse.o
	$(CC) $^ -shared -o $@ $(DEPS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

install: libtgapi.so
	cp libtgapi.so /usr/lib
	cp tgapi.h /usr/include
	cp tgtypes.h /usr/include
	cp tgparse.h /usr/include

uninstall:
	rm /usr/lib/libtgapi.so
	rm /usr/include/tgapi.h
	rm /usr/include/tgtypes.h
	rm /usr/include/tgparse.h

clean:
	rm -f tgapi.o
	rm -f tgparse.o
	rm -f libtgapi.so
