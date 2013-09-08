#
#
OBJ = main.o vect.o hash.o
EXECUTABLE = simulation
C_STD = c99
.PHONY: clean all
all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJ)
	gcc -o $(EXECUTABLE) $(OBJ) -lm -std=$(C_STD)
main.o : main.c vect.h particle.h
	gcc -o main.o -c main.c -lm -std=$(C_STD)
vect.o : vect.c vect.h HashTable.h
	gcc -o vect.o -c vect.c -lm -std=$(C_STD)
hash.o : HashTable.c HashTable.h vect.h particle.h
	gcc -o hash.o -c HashTable.c -lm -std=$(C_STD)

clean : 
	rm $(OBJ) $(EXECUTABLE) *~
