utils = $(wildcard FileUtils/*.cpp) \
	$(wildcard UtilClasses/*.cpp)
model = $(wildcard  Model/*cpp)
metrics = $(wildcard Merics/*cpp)
hashing = $(wildcard Hashing/*cpp)
hyper = $(wildcard HyperCube/*cpp)
lsh = $(wildcard LSH/*cpp)
lsh_main = lsh.cpp
hyper_main = cube.cpp

lsh_main_obj = ${lsh_main:.cpp=.o}
lsh_obj = $(utils:.cpp=.o) $(model:.cpp=.o) $(metrics:.cpp=.o) $(hashing:.cpp=.o) $(lsh:.cpp=.o)
hyper_obj = $(hyper_main:.cpp=.o) $(lsh_obj) $(hyper:.cpp=.o)

LDFLAGS= -lm -w

CXX=g++
CXXFLAGS=-g -w -MMD -std=c++11

lsh.x: $(lsh_main_obj) $(lsh_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

cube.x: $(hyper_obj) $(lsh_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	-rm *.o $(lsh_obj) $(hyper_obj)
