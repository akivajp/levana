CXX = g++
WX_CXXFLAGS=`wx-config --cxxflags` -I/usr/include/ClanLib-0.8
CXXFLAGS = $(WX_CXXFLAGS) -fPIC
SHARED= -shared
SLIB = liblev.a
OUT = a.out
PCH = prec.h.gch
WX_LIBS = `wx-config --libs std,media,gl` -lGLU
RANLIB = ranlib

CORE_SRC  = app.cpp draw.cpp entry.cpp event.cpp frame.cpp \
	    gl.cpp gui.cpp icon.cpp image.cpp media.cpp menu.cpp prim.cpp \
	    sizer.cpp taskbar.cpp util.cpp
CORE_OBJS = $(CORE_SRC:%.cpp=%.o)
CORE_DLIB = lev_core.so
CORE_LIBS = $(WX_LIBS) -llua -lluabind -lboost_thread

SOUND_SRC  = sound.cpp
SOUND_OBJS = $(SOUND_SRC:%.cpp=%.o)
SOUND_DLIB = sound.so
#SOUND_LIBS = -lsfml-audio -lsfml-system -lluabind -llua
SOUND_LIBS = -lclanSound -lclanVorbis -lclanCore -lclanSignals -llua -lluabind

ALL_SRC  = $(CORE_SRC) $(SOUND_SRC)
ALL_OBJS = $(ALL_SRC:%.cpp=%.o)
DLIB = lev.so

all: pch tags bin lib

bin: $(OUT)

#lib: $(SLIB) $(CORE_DLIB) $(SOUND_DLIB)
lib: $(SLIB) $(DLIB)

win-bin: $(WIN_OUT)

$(WIN_OUT): $(ALL_OBJS) main.o resource.rc
	$(WINDRES) -o resource.o resource.rc
	$(CXX) -o $(WIN_OUT) main.o resource.o $(SLIB) $(SOUND_LIBS) $(CORE_LIBS)

# precompiled header
pch: $(PCH)

clean:
	rm -rf tags *.o

distclean: clean
	rm -f *.out *.exe *.so *.dll *.a *.gch

tags: $(SRC) $(SRC:%.cpp=lev/%.hpp)
	ctags -R .

$(PCH): prec.h
	$(CXX) $(CXXFLAGS) $<

$(OUT): $(ALL_OBJS) main.o
	$(CXX) -o $(OUT) main.o $(ALL_OBJS) $(SOUND_LIBS) $(CORE_LIBS)

$(SLIB): $(ALL_OBJS)
	ar r $(SLIB) $(ALL_OBJS)
	$(RANLIB) $(SLIB)

$(DLIB): $(ALL_OBJS)
	$(CXX) $(SHARED) -o $(DLIB) $(ALL_OBJS) $(SOUND_LIBS) $(CORE_LIBS)

$(CORE_DLIB): $(CORE_OBJS)
	$(CXX) $(SHARED) -o $(CORE_DLIB) $(CORE_OBJS) $(CORE_LIBS)

$(SOUND_DLIB): $(SOUND_OBJS)
	$(CXX) $(SHARED) -o $(SOUND_DLIB) $(SOUND_OBJS) $(SOUND_LIBS)

app.o: app.cpp lev/app.hpp
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
sound.o: sound.cpp lev/sound.hpp
taskbar.o: taskbar.cpp lev/taskbar.hpp
util.o: util.cpp lev/util.hpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

