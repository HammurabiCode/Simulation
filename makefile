OBJ = main.o vect.o HashTable.o Particle.o Granular.o Demon.o mat.o quat.o pov.o
EXECUTABLE = simulation.exe
OUT_DIR = ./out
INC_DIR = ./include
SRC_DIR = ./src
C_STD = c99
CXX = gcc
CFLAG = -I$(INC_DIR) -std=$(C_STD) -lm 
.PHONY: clean all
all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJ)
	$(CXX) -o $(OUT_DIR)/$(EXECUTABLE) $(OUT_DIR)/$(OBJ) $(CFLAG)

%.o:$(SRC_DIR)/%.c
	$(CXX) -c -o $(OUT_DIR)/$@ $^ $(CFLAG)
clean:
	rm $(OUT_DIR)*.o *.exe

#main.o : $(SRC_DIR)/main.c $(INC_DIR)/vect.h $(INC_DIR)/Demon.h $(INC_DIR)/pov.h
#	$(CXX) -o $(OUT_DIR)/main.o -c $(SRC_DIR)main.c

#HashTable.o : $(SRC_DIR)/HashTable.c $(INC_DIR)/HashTable.h
#	$(CXX) -o $(OUT_DIR)/HashTable.o -c $(SRC_DIR)HashTable.c

#Demon.o : $(SRC_DIR)/Demon.c $(INC_DIR)/Granular.h
#	$(CXX) -o $(OUT_DIR)/Demon.o -c $(SRC_DIR)Demon.c
#
#Granular.o : $(SRC_DIR)/Granular.c $(INC_DIR)/Granular.h
#	$(CXX) -o $(OUT_DIR)/Granular.o -c $(SRC_DIR)Granular.c
#
#Particle.o : $(SRC_DIR)/Particle.c $(INC_DIR)/Particle.h $(INC_DIR)/coef.h
#	$(CXX) -o $(OUT_DIR)/Particle.o -c $(SRC_DIR)/Particle.c
#
#quat.o : $(SRC_DIR)/quat.c $(INC_DIR)/quat.h $(INC_DIR)/vect.h
##	$(CXX) -o $(OUT_DIR)/quat.o -c $(SRC_DIR)/quat.c
#
#mat.o : $(SRC_DIR)/mat.c $(INC_DIR)/mat.h $(INC_DIR)/vect.h
#	$(CXX) -o $(OUT_DIR)/mat.o -c $(SRC_DIR)/mat.c
#
#vect.o : $(SRC_DIR)/vect.c $(INC_DIR)/vect.h
#	$(CXX) -o $(OUT_DIR)/vect.o -c $(SRC_DIR)/vect.c
#
#pov.o : $(SRC_DIR)/pov.c $(INC_DIR)/pov.h $(INC_DIR)/vect.h
#	$(CXX) -o $(OUT_DIR)/pov.o -c $(SRC_DIR)/pov.c
