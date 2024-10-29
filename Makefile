CXX=g++
CC=gcc
CXXFLAGS=-std=c++17 -I./ann/include
LDFLAGS=-L./ann/lib -lANN -lm
SRC=main.cpp CustomANNKdTree.cpp Polygon.cpp RangeSearch.cpp Utilities.cpp
SRC_C=Orientation.c
OBJ=$(SRC:.cpp=.o) $(SRC_C:.c=.o)
EXEC=rangesearch

all: ann $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

%.o: %.c
	$(CC) -c $< 

ann:
	if [ ! -d "./ann" ]; then \
		curl -o ann_1.1.2.tar.gz https://www.cs.umd.edu/~mount/ANN/Files/1.1.2/ann_1.1.2.tar.gz; \
		tar zxf ann_1.1.2.tar.gz; \
		mv ann_1.1.2 ann; \
		(cd ann && make linux-g++ && cd ..); \
		rm ann_1.1.2.tar.gz; \
	fi

clean:
	rm -f $(OBJ) $(EXEC)
	# rm -rf ann*
	rm -rf *.out
	rm -rf kdtree.txt
	find test/ -name *'.png' -type f -delete
	find test/ -name 'my_output_points.json' -type f -delete



.PHONY: all clean ann
