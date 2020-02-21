build: battleship.c
	gcc battleship.c -o battleship -lncurses -Wall
run: battleship
	./battleship input.txt
clean:
	rm -f battleship
