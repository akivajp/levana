CXX = g++
SRC = app.cpp draw.cpp entry.cpp event.cpp frame.cpp gl.cpp gui.cpp icon.cpp menu.cpp sizer.cpp taskbar.cpp
OBJS = $(SRC:%.cpp=%.o)
OUT = a.out
DLIB = levana.so
SLIB = liblevana.a
PCH = prec.h.gch
WX_CXXFLAGS=`wx-config --cxxflags`
WX_LIBS = `wx-config --libs std gl`
CXXFLAGS = $(WX_CXXFLAGS) -fPIC
LIBS = $(WX_LIBS) -llua -lluabind

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

tags: $(SRC) $(SRC:%.cpp=levana/%.hpp)
	ctags -R .

$(PCH): prec.h levana/levana.hpp
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
draw.o: draw.cpp levana/draw.hpp levana/frame.hpp
entry.o: entry.cpp $(SRC:%.cpp=levana/%.hpp) levana/event.hpp
event.o: event.cpp
frame.o: frame.cpp levana/frame.hpp connect.hpp levana/icon.hpp levana/event.hpp
gl.o: gl.cpp levana/gl.hpp
gui.o: gui.cpp levana/gui.hpp
icon.o: icon.cpp levana/icon.hpp
menu.o: menu.cpp levana/menu.hpp levana/icon.hpp
sizer.o: sizer.cpp levana/sizer.hpp
taskbar.o: taskbar.cpp levana/taskbar.hpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

