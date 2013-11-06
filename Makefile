EXEC = am2315
SRC = am2315.cpp
INC_DIR = /usr/include/ 
LIB_DIR = /usr/lib/
LDLIBS = -lbcm2835
CC = g++
CFLAGS = -Wall
LFLAGS = -L$(LIB_DIR) $(LDLIBS)
IFLAGS = -I$(INC_DIR)
OBJS = $(SRC:%.cpp=%.o)

all:$(EXEC)

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $(EXEC)

$(OBJS) : $(SRC)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(SRC)

clean:
	rm $(OBJS) *~
