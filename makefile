compile:
	./compile.sh

run:
	./HNSWpred.sh /mnt/d/sem6/col380/assign/a3/converted 2 user.txt output.txt

data:
	./DataSetup.sh /mnt/d/sem6/col380/assign/a3/input_data /mnt/d/sem6/col380/assign/a3/converted

clean:
	rm -rf main converted hnsw convert *.bin

out:
	rm output.txt