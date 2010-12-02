PREFIX=/tmp
OUT = a.out
BIN = levana
DLIB = liblevana.so
SLIB = liblevana.a

all: build bin lib

build:
	cd src && make all

bin: src/$(OUT)
	install -d bin
	cp src/a.out bin/$(BIN)

lib: src/$(DLIB) src/$(SLIB)
	install -d lib
	cp src/{$(DLIB),$(SLIB)} lib/

install:
	install -d $(PREFIX)/{bin,lib}
	install src/levana $(PREFIX)/bin
	install src/levana.so $(PREFIX)/lib

clean:
	cd src && make clean

distclean:
	cd src && make distclean
	rm -rf bin lib

src/$(OUT): build
src/$(DLIB): build
src/$(SLIB): build

