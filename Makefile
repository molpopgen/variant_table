CPPFLAGS=-std=c++11
CXXFLAGS=-std=c++11

all: VariantMatrix.o
	$(CXX) $(CXXFLAGS) -o simpletest simpletest.o VariantMatrix.o
	

VariantMatrix.o: VariantMatrix.hpp simpletest.o
simpletest.o: VariantMatrix.hpp
