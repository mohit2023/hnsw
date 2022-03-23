#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[]){

	if(argc!=3) {
		cout<<"INVALID ARGS\n";
	}

	string in_path = argv[1];
	string out_path = argv[2];

	int num;
	int L =0;
	int D = 0;
	int max_level;
	int ep;
	float fnum;
	char buffer[4];
	ifstream MyReadFile;
	ofstream MyWriteFile;
	
	MyReadFile.open(in_path+"/max_level.txt");
	MyReadFile >> max_level;
	MyReadFile.close();

	MyReadFile.open(in_path+"/index.txt");
	MyWriteFile.open(out_path+"/index.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	
	MyReadFile.open(in_path+"/indptr.txt");
	MyWriteFile.open(out_path+"/indptr.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	
	MyReadFile.open(in_path+"/ep.txt");
	MyReadFile >> ep;
	MyReadFile.close();
	
	MyReadFile.open(in_path+"/level.txt");
	MyWriteFile.open(out_path+"/level.bin");
	while(MyReadFile >> num){
		L++;
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	
	MyReadFile.open(in_path+"/level_offset.txt");
	MyWriteFile.open(out_path+"/level_offset.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	
	MyReadFile.open(in_path+"/vect.txt");
	MyWriteFile.open(out_path+"/vect.bin");
	while(MyReadFile >> fnum){
		D++;
		MyWriteFile.write((char*)&fnum,sizeof(float));
	}
	MyReadFile.close();
	MyWriteFile.close();
	D = D/L;
	
	MyWriteFile.open(out_path+"/items.bin");
	MyWriteFile.write((char*)&max_level,4);
	MyWriteFile.write((char*)&ep,4);
	MyWriteFile.write((char*)&L,4);
	MyWriteFile.write((char*)&D,4);
	MyWriteFile.close();

	// cout<<L<<"\n"<<D<<"\n";
	return 0;
}
