PROJECTNAME :=physics

CC := gcc-4.9
CFLAGS := -fdiagnostics-color=always -std=c++11 -Wall $(ARGS) `pkg-config --cflags glfw3`

NVCC := /usr/local/cuda/bin/nvcc
NVCCGENCODEFLAGS := -arch=compute_50 -code=sm_50,compute_50

NVCCFLAGS := $(NVCCGENCODEFLAGS) $(ARGS)
NVCCLINKFLAGS := $(NVCCGENCODEFLAGS)

CUDAPATH := /usr/local/cuda
CUDALIBPATH := -L$(CUDAPATH)/lib64

LIBPATH :=
GLPATH := /usr/include
LIBS := -lGLEW -lGLU `pkg-config --static --libs glfw3` -lcuda -lcudart
INCLUDES := -I$(CUDAPATH)/include -I$(GLPATH)

all: $(PROJECTNAME)

$(PROJECTNAME): main.o Display.o Input.o Scene.o Camera.o Physics.o
	$(NVCC) $(NVCCLINKFLAGS) -o $@ $^ $(LIBPATH) $(CUDALIBPATH) $(LIBS)

Display.o: Display.cpp Display.h
	$(CC) $(CFLAGS) -c $(INCLUDES) $< 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $(INCLUDES) $< 

%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $(INCLUDES) $<

clean:
	rm *.o
	rm $(PROJECTNAME)