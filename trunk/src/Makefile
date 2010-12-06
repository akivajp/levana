CXX = g++
SRC = app.cpp entry.cpp frame.cpp icon.cpp menu.cpp
OBJS = $(SRC:%.cpp=%.o)
OUT = a.out
DLIB = levana.so
SLIB = liblevana.a
PCH = prec.h.gch
WX_CXXFLAGS=`wx-config --cxxflags`
WX_LIBS = `wx-config --libs`
CXXFLAGS = $(WX_CXXFLAGS) -fPIC
LIBS = $(WX_LIBS) -llua -lluabind

all: pch tags bin lib

bin: $(OUT)

lib: $(SLIB) $(DLIB)

# precompiled header
pch: $(PCH)

clean:
	rm -rf tags *.o *.gch

distclean: clean
	rm -f $(OUT) $(DLIB) $(SLIB)

tags: $(SRC) $(SRC:%.cpp=levana/%.hpp)
	ctags -R .

$(PCH): prec.h
	$(CXX) $(CXXFLAGS) $<

$(OUT): $(OBJS) main.o
	$(CXX) -o $(OUT) main.o $(OBJS) $(LIBS)

$(SLIB): $(OBJS)
	ar r $(SLIB) $(OBJS)
	ranlib $(SLIB)

$(DLIB): $(OBJS)
	$(CXX) -shared -o $(DLIB) $(OBJS) $(LIBS)

main.o: main.cpp levana/app.hpp
app.o: app.cpp levana/app.hpp
entry.o: entry.cpp levana/entry.hpp
frame.o: frame.cpp levana/frame.hpp
icon.o: icon.cpp levana/icon.hpp
menu.o: menu.cpp levana/menu.hpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

