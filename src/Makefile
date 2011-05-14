CXX = g++
SRC = app.cpp cfg.cpp draw.cpp entry.cpp event.cpp frame.cpp \
      gl.cpp gui.cpp icon.cpp image.cpp media.cpp menu.cpp prim.cpp \
      sizer.cpp sound.cpp taskbar.cpp util.cpp
OBJS = $(SRC:%.cpp=%.o)
OUT = a.out
DLIB = lev.so
SLIB = liblev.a
PCH = prec.h.gch
WX_CXXFLAGS=`wx-config --cxxflags`
WX_LIBS = `wx-config --libs std,media,gl` -lGLU
CXXFLAGS = $(WX_CXXFLAGS) -fPIC
#LIBS = $(WX_LIBS) -llua -lluabind -lvorbisfile
LIBS = $(WX_LIBS) -llua -lluabind
SHARED= -shared

all: pch tags bin lib

bin: $(OUT)

lib: $(SLIB) $(DLIB)

win-bin: $(OUT) $(OBJS) main.o resource.rc
	$(WINDRES) -o resource.o resource.rc
	$(CXX) -o $(OUT) main.o resource.o $(OBJS) $(LIBS)

# precompiled header
pch: $(PCH)

clean:
	rm -rf tags *.o

distclean: clean
	rm -f *.out *.exe *.so *.dll *.a *.gch

tags: $(SRC) $(SRC:%.cpp=lev/%.hpp)
	ctags -R .

$(PCH): prec.h lev/lev.hpp
	$(CXX) $(CXXFLAGS) $<

$(OUT): $(OBJS) main.o
	$(CXX) -o $(OUT) main.o $(OBJS) $(LIBS)

$(SLIB): $(OBJS)
	ar r $(SLIB) $(OBJS)
	ranlib $(SLIB)

$(DLIB): $(OBJS)
	$(CXX) $(SHARED) -o $(DLIB) $(OBJS) $(LIBS)

app.o: app.cpp lev/app.hpp
cfg.o: cfg.cpp lev/cfg.hpp
draw.o: draw.cpp lev/draw.hpp lev/frame.hpp
entry.o: entry.cpp $(SRC:%.cpp=lev/%.hpp) lev/event.hpp
event.o: event.cpp
frame.o: frame.cpp lev/frame.hpp connect.hpp lev/icon.hpp lev/event.hpp
gl.o: gl.cpp lev/gl.hpp
gui.o: gui.cpp lev/gui.hpp
icon.o: icon.cpp lev/icon.hpp
image.o: image.cpp lev/image.hpp
main.o: main.cpp lev/app.hpp
media.o: media.cpp lev/media.hpp
menu.o: menu.cpp lev/menu.hpp lev/icon.hpp
prim.o: prim.cpp lev/prim.hpp
sizer.o: sizer.cpp lev/sizer.hpp
taskbar.o: taskbar.cpp lev/taskbar.hpp
util.o: util.cpp lev/util.hpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

