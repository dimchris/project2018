utils = $(wildcard FileUtils/*.cpp) \
	$(wildcard UtilClasses/*.cpp)
model = $(wildcard  Model/*cpp)
metrics = $(wildcard Merics/*cpp)
hashing = $(wildcard Hashing/*cpp)
hyper = $(wildcard HyperCube/*cpp)
cluster = $(wildcard cluster/*cpp cluster/assign/*cpp cluster/evaluate/*cpp cluster/init/*cpp cluster/update/*cpp)
lsh = $(wildcard LSH/*cpp)
recommend = $(wildcard tweets/*cpp)
lsh_main = lsh.cpp
hyper_main = cube.cpp
cluster_main = cluster.cpp
recommend_main = recommendation.cpp

lsh_main_obj = ${lsh_main:.cpp=.o}
lsh_obj = $(utils:.cpp=.o) $(model:.cpp=.o) $(metrics:.cpp=.o) $(hashing:.cpp=.o) $(lsh:.cpp=.o)
hyper_main_obj = $(hyper_main:.cpp=.o)
hyper_obj =  $(lsh_obj) $(hyper:.cpp=.o)
cluster_obj = $(cluster:.cpp=.o)
cluster_main_obj = $(cluster_main:.cpp=.o)
recommend_obj = $(recommend_main:.cpp=.o) $(recommend:.cpp=.o)


LDFLAGS= -lm -w

CXX=g++
CXXFLAGS=-g -w -MMD -std=c++11

lsh.x: $(lsh_main_obj) $(lsh_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

cube.x: $(hyper_main_obj) $(hyper_obj) $(lsh_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

cluster.x: $(cluster_main_obj) $(cluster_obj) $(hyper_obj) $(lsh_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

recommendation.x: $(recommend_obj) $(cluster_obj) $(lsh_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	-rm *.o $(lsh_obj) $(hyper_obj)
