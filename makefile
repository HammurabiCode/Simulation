#
#
OBJ = main.o vect.o hash.o Particle.o Granular.o Demon.o mat.o quat.o pov.o
EXECUTABLE = simulation
C_STD = c99
.PHONY: clean all
all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJ)
	gcc -o $(EXECUTABLE) $(OBJ) -lm -std=$(C_STD)
main.o : main.c vect.h Particle.h
	gcc -o main.o -c main.c -lm -std=$(C_STD)
hash.o : HashTable.c HashTable.h vect.o Granular.o
	gcc -o hash.o -c HashTable.c -lm -std=$(C_STD)
Demon.o : Demon.c Granular.h vect.o Granular.o
	gcc -o Demon.o -c Demon.c -lm -std=$(C_STD)
Granular.o : Granular.c Granular.h vect.o Particle.o mat.o
	gcc -o Granular.o -c Granular.c -lm -std=$(C_STD)
Particle.o : Particle.c Particle.h coef.h vect.o
	gcc -o Particle.o -c Particle.c -lm -std=$(C_STD)
quat.o : quat.c quat.h vect.h vect.o
	gcc -o quat.o -c quat.c -lm -std=$(C_STD)
mat.o : mat.c mat.h vect.h vect.o
	gcc -o mat.o -c mat.c -lm -std=$(C_STD)
vect.o : vect.c vect.h
	gcc -o vect.o -c vect.c -lm -std=$(C_STD)
pov.o : pov.c pov.h vect.h
	gcc -o pov.o -c pov.c -lm -std=$(C_STD)


debug : $(OBJ)
	gcc -o $(EXECUTABLE) $(OBJ) -lm -std=$(C_STD) -g
main.o : main.c vect.h Particle.h
	gcc -o main.o -c main.c -lm -std=$(C_STD) -g
hash.o : HashTable.c HashTable.h vect.o Granular.o
	gcc -o hash.o -c HashTable.c -lm -std=$(C_STD) -g
Demon.o : Demon.c Granular.h vect.o Granular.o
	gcc -o Demon.o -c Demon.c -lm -std=$(C_STD) -g
Granular.o : Granular.c Granular.h vect.o Particle.o mat.o
	gcc -o Granular.o -c Granular.c -lm -std=$(C_STD) -g
Particle.o : Particle.c Particle.h coef.h vect.o
	gcc -o Particle.o -c Particle.c -lm -std=$(C_STD) -g
quat.o : quat.c quat.h vect.h vect.o
	gcc -o quat.o -c quat.c -lm -std=$(C_STD) -g
mat.o : mat.c mat.h vect.h vect.o
	gcc -o mat.o -c mat.c -lm -std=$(C_STD) -g
vect.o : vect.c vect.h
	gcc -o vect.o -c vect.c -lm -std=$(C_STD) -g
pov.o : pov.c pov.h vect.h
	gcc -o pov.o -c pov.c -lm -std=$(C_STD) -g

clean : 
	rm $(OBJ) $(EXECUTABLE) *~ .*.swp
