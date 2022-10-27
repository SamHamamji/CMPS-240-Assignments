make:
	gcc -o a.out src/reducer.c src/splitter.c src/worker.c src/main.c

clean:
	rm a.out