CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=14320042
CLASSES=helper.cpp

all: router

router: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

router_send: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM router router_send *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
