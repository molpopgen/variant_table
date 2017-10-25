CPPFLAGS=-std=c++11 
CXXFLAGS=-std=c++11 -O3 -Wall -W
LDFLAGS=-Wl,-rpath,$(HOME)/anaconda3/lib

all: VariantMatrix.o VariantMatrixViews.o simpletest.o filtering.o statecounts.o hapcounts.o thetapi.o
	$(CXX) $(CXXFLAGS) -o simpletest simpletest.o VariantMatrix.o VariantMatrixViews.o filtering.o statecounts.o hapcounts.o thetapi.o $(LDFLAGS) -lgsl -lgslcblas -lsequence
	
clean:
	rm -f simpletest *.o

VariantMatrix.o: VariantMatrix.hpp simpletest.o
VariantMatrixViews.o: VariantMatrix.hpp variant_matrix_views_internal.hpp 
filtering.o: filtering.hpp VariantMatrix.hpp variant_matrix_views_internal.hpp
simpletest.o: VariantMatrix.hpp  variant_matrix_views_internal.hpp
statecounts.o: VariantMatrix.hpp VariantMatrixViews.hpp
hapcounts.o: VariantMatrix.hpp VariantMatrixViews.hpp
thetapi.o: statecounts.hpp VariantMatrix.hpp
