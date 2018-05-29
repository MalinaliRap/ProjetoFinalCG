#OBJS specifies which files to compile as part of the project
# OBJS = exemplo.cpp
OBJ = projetofinal
EXT = .c
#code.c

#CC specifies which compiler we're using
CC = gcc
# CC = clang

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -Wpedantic -g

#LINKER_FLAGS specifies the libraries we're linking against
#-lXmu -lXext -lXi -lX11 -lm
#-lm -lpthread -lSDL2
LINKER_FLAGS = -lglut -lGL -lGLU -lm

all: objects
# routine to run
objects: 
	$(CC) $(OBJ)$(EXT) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ)
# example code to clean stuff
#clean:
#	rm $(OBJ)