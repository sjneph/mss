CC	= g++
SFLAGS	= -static -ansi -Wall -pedantic -O3
OBJDIR	= objects

SOURCE1	= builtin.mss.example1.cpp
SOURCE2	= builtin.mss.example2.cpp
SOURCE3	= stl.mss.example1.cpp
SOURCE4	= stl.mss.example2.cpp
SOURCE5	= stl.mss.example3.cpp
BIN	= ../bin

NAME1	= builtin.mss.example1
NAME2	= builtin.mss.example2
NAME3	= stl.mss.example1
NAME4	= stl.mss.example2
NAME5	= stl.mss.example3

.cpp.o:; $(CC) -c $(SFLAGS) $<

all:
	mkdir -p $(BIN)
	$(CC) -o $(BIN)/$(NAME1) $(SFLAGS) $(SOURCE1)
	$(CC) -o $(BIN)/$(NAME2) $(SFLAGS) $(SOURCE2)
	$(CC) -o $(BIN)/$(NAME3) $(SFLAGS) $(SOURCE3)
	$(CC) -o $(BIN)/$(NAME4) $(SFLAGS) $(SOURCE4)
	$(CC) -o $(BIN)/$(NAME5) $(SFLAGS) $(SOURCE5)

clean:
	rm -f $(BIN)/$(NAME1)
	rm -f $(BIN)/$(NAME2)
	rm -f $(BIN)/$(NAME3)
	rm -f $(BIN)/$(NAME4)
	rm -f $(BIN)/$(NAME5)
