include env.mk

INC = -I/usr/include
LIB =  -L/usr/local/lib -L/usr/lib

CFLAGS = -pthread -std=c++17 $(OPT) -Wall -Werror -c $(INC)
LFLAGS = -pthread -std=c++17 $(OPT)

#DEPS = YOLOv3_gen.h
OBJS = main.o

# make sure to create bin folder --> mkdir -p ./bin    --> here -p will not created if exist
TGT  = ./bin/main        

%.o: %.cpp $(DEPS)
	$(GPP) $(CFLAGS) -o $@ $<

all : $(OBJS)
	$(GPP) $(LFLAGS) $(OBJS) -o $(TGT) $(LIB)

clean:
	rm -f *.o hailo/*.o $(TGT)

