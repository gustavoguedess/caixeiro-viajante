
ifeq (input,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(ARGS):;@:)
endif

.PHONY = tree cycle

all:
	@mkdir -p bin
	@mkdir -p build
	gcc src/genpoints.c -o bin/genpoints
	g++ src/tsp.cpp -o bin/tsp

clean:
	rm -rf bin/*
	rm -rf build/*
input:
	./bin/genpoints $(ARGS)
	mv input.txt build/input.txt
	sed '1d' build/input.txt > build/input2.txt 
	gnuplot lib/points.plot
	rm -rf build/input2.txt

run:
	./bin/tsp build/input.txt

result: tree cycle

graph:
	sed '$$!N;s/$$/\n/' build/graph_complete.txt > build/graph_complete2.txt
	gnuplot lib/graph.plot 
	rm -rf build/graph_complete2.txt

tree:
	sed '$$!N;s/$$/\n/' build/tree.txt > build/tree2.txt
	gnuplot lib/tree.plot 
	rm -rf build/tree2.txt

cycle:
	sed '$$!N;s/$$/\n/' build/cycle.txt > build/cycle2.txt
	gnuplot lib/cycle.plot 
	rm -rf build/cycle2.txt
