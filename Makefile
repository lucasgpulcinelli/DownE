CXXFLAGS  += -Wall -Wextra -Wpedantic -Iinclude -std=c++17
LDFLAGS   += -lglfw -lGL -lGLEW -lpthread

EXECUTABLE ?= build/main
ZIPFILE    ?= ../zipfile.zip

CXXFILES = $(shell find src/ -type f |grep '\.cpp')
OFILES   = $(patsubst src/%.cpp,build/obj/%.o, $(CXXFILES))

.PHONY: all clean debug

all: CXXFLAGS+=-O3
all: $(EXECUTABLE)

clean:
	@rm -f $(ZIPFILE)
	@rm -rf build/

debug: CXXFLAGS+=-g3 -DDEBUG
debug: clean
debug: $(EXECUTABLE)


$(EXECUTABLE): $(OFILES)
	@mkdir -p build
	$(CXX)  -o $@ $^ $(LDFLAGS)

build/obj/%.o: src/%.cpp src/%.h
	@mkdir -p $(shell echo $< |sed 's|src\(.*\)/[^/]*|build/obj\1|')
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/obj/%.o: src/%.cpp
	@mkdir -p $(shell echo $< |sed 's|src\(.*\)/[^/]*|build/obj\1|')
	$(CXX) $(CXXFLAGS) -c -o $@ $<

