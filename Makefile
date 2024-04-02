BIN=YOUR_PROJECT_NAME

CC_FLAGS  = -Isrc/include -std=c23
CXX_FLAGS = -Isrc/include -Ilib/ -std=c++23 -fmodules-ts
LD_FLAGS  = -lm 
CC=gcc
CXX=g++
LD=gcc
AR=ar

LIBS=
include lib/make.mk

cxx_sources=$(wildcard src/*.cpp)
c_sources=$(wildcard src/*.c)
objects=$(patsubst src/%.cpp,bin/obj/%.cpp.o,$(cxx_sources)) $(patsubst src/%.c,bin/obj/%.c.o,$(c_sources)) $(LIBS)


all: bin/$(BIN)
	@rm -r bin/obj
	
keep: bin/$(BIN)

clean:
	rm -r bin/obj/


bin/$(BIN): $(objects)
	$(LD)  -o $@ $^ $(LD_FLAGS)

bin/obj/%.cpp.o: */%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

bin/obj/%.c.o: */%.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CC_FLAGS)
