
ifeq (input,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(ARGS):;@:)
endif

.PHONY = remove-prov

all:
	gcc src/genpoints.c -o bin/genpoints
	g++ src/tsp.cpp -o bin/tsp

clean:
	rm -rf bin/*
	rm -rf build/*
input:
	./bin/genpoints $(ARGS)
	cp input.txt build/input.txt
	tail -n +1 build/input.txt > build/input2.txt 
	gnuplot lib/points.plot
	rm -rf build/input2.txt

run:
	./bin/tsp build/input.txt

result:
	sed '$$!N;s/$$/\n/' build/tree.txt > build/tree2.txt
	gnuplot lib/tree.plot 
	rm -rf build/tree2.txt
	gnuplot lib/cycle.plot

graph:
	sed '$$!N;s/$$/\n/' build/graph_complete.txt > build/tree2.txt
	gnuplot lib/tree.plot 
	rm -rf build/tree2.txt

tree:
	sed '$$!N;s/$$/\n/' tree.txt > build/tree2.txt
	gnuplot lib/tree.plot 
	rm -rf build/tree2.txt
