
all: ../bin/jsoni

CXXFLAGS := -std=c++14

../bin/jsoni: main.cpp *.hpp
	make main
	mkdir -p ../bin
	mv main ../bin/jsoni
