CC=g++
CFLAGS=-std=c++11 -g
INCDIRS=-I. -I..
LIBDIRS=
LIBS=-lSDL2 -lGLEW -lGL
SOURCES=main.cpp            \
        Sample.cpp          \
        Application.cpp     \
        Shader.cpp          \
        Attribute.cpp       \
        Uniform.cpp         \
        IndexBuffer.cpp     \
        Draw.cpp            \
        Texture.cpp         \
        GLTFLoader.cpp      \
        Track.cpp           \
        TransformTrack.cpp  \
        DebugDraw.cpp       \
        ../stb_image_impl.cpp \
        ../cgltf_impl.cpp
TARGET=main
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCDIRS) $(LIBDIRS) $(LIBS)

