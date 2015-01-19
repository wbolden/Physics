PROJECTNAME :=physics

CC := gcc
CFLAGS := -std=c++11 -Wall $(ARGS) `pkg-config --cflags glfw3`

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

$(PROJECTNAME): main.o Display.o Input.o Scene.o
	$(NVCC) $(NVCCLINKFLAGS) -o $@ $^ $(LIBPATH) $(CUDALIBPATH) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $(INCLUDES) $< 

%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $(INCLUDES) $<

clean:
	rm *.o
	rm $(PROJECTNAME)