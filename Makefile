
EXE = node.out
OBJS = src/main.o src/application.o src/graph_drawing_utils.o
OBJS += src/imgui_impl_glfw_gl3.o src/imgui_dock.o 
OBJS += ./external/imgui/imgui.o ./external/imgui/imgui_demo.o ./external/imgui/imgui_draw.o
OBJS += ./external/gl3w/GL/gl3w.o

UNAME_S := $(shell uname -s)


ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS = -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS = -I./external/imgui -I./external/gl3w `pkg-config --cflags glfw3`
	CXXFLAGS += -Wall -Wformat -std=c++11
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -lglfw3

	CXXFLAGS = -I../../ -I../libs/gl3w -I/usr/local/Cellar/glew/1.10.0/include -I/usr/local/include
	CXXFLAGS += -Wall -Wformat
#	CXXFLAGS += -D__APPLE__
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), MINGW64_NT-6.3)
   ECHO_MESSAGE = "Windows"
   LIBS = -lglfw3 -lgdi32 -lopengl32 -limm32

   CXXFLAGS = -I../../ -I../libs/gl3w `pkg-config --cflags glfw3`
   CXXFLAGS += -Wall -Wformat
   CFLAGS = $(CXXFLAGS)
endif


.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS) $(LIBS)

clean:
	rm $(EXE) $(OBJS)
