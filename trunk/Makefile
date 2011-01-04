PREFIX=/tmp
OUT = a.out
BIN = levana
DLIB = levana.so
SLIB = liblevana.a
LUA_VERSION = 5.1

none:
	@echo "Please do"
	@echo "  make PLATFORM"
	@echo "where PLATFORM is one of these:"
	@echo "  linux mac mingw"

linux: build bin lib

mac:
	make -f make/Makefile.macosx build

mingw:
	make -f Makefile.mingw build

build:
	cd src && make all

bin: src/$(OUT)
	install -d bin
	cp src/a.out bin/$(BIN)

lib: src/$(DLIB) src/$(SLIB)
	install -d lib
	cp src/{$(DLIB),$(SLIB)} lib/

install:
	install -d $(PREFIX)/bin
	install bin/$(BIN) $(PREFIX)/bin/$(BIN)
	install -d $(PREFIX)/lib
	install lib/$(SLIB) $(PREFIX)/lib/
	install -d $(PREFIX)/lib/lua/$(LUA_VERSION)
	install lib/$(DLIB) $(PREFIX)/lib/lua/$(LUA_VERSION)/

clean:
	cd src && make clean

distclean:
	cd src && make distclean
	rm -rf bin lib

src/$(OUT): build
src/$(DLIB): build
src/$(SLIB): build

