CXX		= g++ -std=c++17

LDFLAGS 	= `pkg-config --cflags --libs opencv4`
OPTFLAGS	= -O3  -pthread

TARGETS		=  main


.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $< -o $@  $(LDFLAGS)

all		: $(TARGETS)

clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~