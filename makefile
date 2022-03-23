compile:
	g++ convert.cpp -o convert
	mpic++ -std=c++17 -fopenmp -o hnsw main.cpp

run:
	mpirun -np 1 ./hnsw /mnt/d/sem6/col380/assign/a3 5 user.txt output.txt

clean:
	rm hnsw convert *.bin