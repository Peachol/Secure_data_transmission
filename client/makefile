src=$(wildcard *.cpp)
objs=$(patsubst %.cpp,%.o,$(src))
target=cltMain
LIBS=-lstdc++ -lssl -lcrypto
$(target):$(objs)
	g++ -o $@ $^ $(LIBS) 

%.o:%.cpp
	g++ -std=c++11 -c $<

.PHONY:clean
clean:
	rm -f $(target) $(objs)
