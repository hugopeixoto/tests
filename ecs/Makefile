all: bobbing vtable novtable ecs-arraypertype ecs-arrayperarchetype

clean:
	rm -f bobbing vtable novtable ecs-arraypertype ecs-arrayperarchetype

vtable: vtable.cc
	g++ -std=c++17 vtable.cc -o vtable

novtable: novtable.cc
	g++ -std=c++17 novtable.cc -o novtable

ecs-arraypertype: ecs-arraypertype.cc
	g++ -std=c++17 ecs-arraypertype.cc -o ecs-arraypertype

ecs-arrayperarchetype: ecs-arrayperarchetype.cc
	g++ -std=c++17 ecs-arrayperarchetype.cc -o ecs-arrayperarchetype

test: all
	./vtable 42
	./novtable 42
	./ecs-arraypertype 42
	./ecs-arrayperarchetype 42

grind: all
	valgrind --tool=cachegrind --branch-sim=yes ./vtable 42
	valgrind --tool=cachegrind --branch-sim=yes ./novtable 42
	valgrind --tool=cachegrind --branch-sim=yes ./ecs-arraypertype 42
	valgrind --tool=cachegrind --branch-sim=yes ./ecs-arrayperarchetype 42
