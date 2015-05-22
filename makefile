all:
	gcc tofile.c filters.c samples.c main.c -o sndtosine -lm -lsndfile -lfftw3 -g -Wall
clean:
	rm sndtosine
