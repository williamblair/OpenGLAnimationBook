CC=g++
CFLAGS=-std=c++11
INCDIRS=-I. -I..
LIBDIRS=
LIBS=-lSDL2 -lGLEW -lGL
SOURCES=main.cpp        \
        Shader.cpp      \
        Attribute.cpp   \
        Uniform.cpp     \
        IndexBuffer.cpp \
        Draw.cpp        \
        Texture.cpp     \
        ../stb_image_impl.cpp
TARGET=main
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCDIRS) $(LIBDIRS) $(LIBS)

