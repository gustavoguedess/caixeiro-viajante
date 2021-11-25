
ifeq (input,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(ARGS):;@:)
endif

all:
	gcc genpoints.c -o genpoints
	g++ -o tsp tsp.cpp 

clean:
	rm -rf genpoints
	rm -rf tsp
	rm -rf tree2.txt
	rm -rf pontos.pdf
	rm -rf arvore.pdf
	rm -rf ciclo.pdf
	rm -rf input.txt
	rm -rf tree.txt
	rm -rf cycle.txt
input:
	./genpoints $(ARGS)
	tail -n +1 input.txt > input2.txt 
	gnuplot points.plot
	rm -rf input2.txt 
run:
	./tsp input.txt

pdf:
	sed '$$!N;s/$$/\n/' tree.txt > tree2.txt
	gnuplot tree.plot 
	rm -rf tree2.txt
	gnuplot cycle.plot