# 314CA Nedelcu Andrei David
# Makefile

# ***************************************
# ******* COMPILER SETUP ****************
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
NAME = main
OBJECTS = main.o hashtable.o entities.o commands.o utils.o

# ***************************************
# ******* EXECUTE BUILD ****************
build: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

main.o: main.c
	$(CC) -c $(CFLAGS) $^

hashtable.o: hashtable.c
	$(CC) -c $(CFLAGS) $^

entities.o: entities.c
	$(CC) -c $(CFLAGS) $^

commands.o: commands.c
	$(CC) -c $(CFLAGS) $^

utils.o: utils.c
	$(CC) -c $(CFLAGS) $^

clean: 
	rm $(OBJECTS)
	rm $(NAME)

pack:
	zip -FSr 314CA_NedelcuAndreiDavid_Tema2.zip *.c *.h README Makefile
	
.PHONY: clean build
