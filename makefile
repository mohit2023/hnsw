compile:
	g++ convert.cpp -o convert
	mpic++ -std=c++17 -fopenmp -o hnsw main.cpp

convert:
	./convert

run:
	mpirun -np 1 ./hnsw

clean:
	rm hnsw convert *.bin