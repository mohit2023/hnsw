#include <bits/stdc++.h>

using namespace std;

int main(){
	int num;
	int L =0;
	int D = 0;
	int U = 0;
	float fnum;
	char buffer[4];
	ifstream MyReadFile;
	ofstream MyWriteFile;
	
	MyReadFile.open("max_level.txt");
	MyWriteFile.open("max_level.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();

	MyReadFile.open("index.txt");
	MyWriteFile.open("index.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();
	
	MyReadFile.open("indptr.txt");
	MyWriteFile.open("indptr.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();
	
	MyReadFile.open("ep.txt");
	MyWriteFile.open("ep.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();
	
	MyReadFile.open("level.txt");
	MyWriteFile.open("level.bin");
	while(MyReadFile >> num){
		L++;
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();
	
	MyReadFile.open("level_offset.txt");
	MyWriteFile.open("level_offset.bin");
	while(MyReadFile >> num){
		MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();
	
	MyReadFile.open("vect.txt");
	MyWriteFile.open("vect.bin");
	while(MyReadFile >> fnum){
		D++;
		MyWriteFile.write((char*)&fnum,sizeof(float));
	}
	MyReadFile.close();
	MyWriteFile.close();
	MyReadFile.clear();
	MyWriteFile.clear();
	D = D/L;
	
	/*MyReadFile.open("user.txt");
	//MyWriteFile.open("vect.bin");
	while(MyReadFile >> fnum){
		U++;
		//MyWriteFile.write((char*)&num,4);
	}
	MyReadFile.close();
	//MyWriteFile.close();
	MyReadFile.clear();
	//MyWriteFile.clear();
	U = U/D;*/
	
	MyWriteFile.open("items.bin");
	MyWriteFile.write((char*)&L,4);
	//MyWriteFile.write((char*)&U,4);
	MyWriteFile.write((char*)&D,4);
	MyWriteFile.close();
	MyWriteFile.clear();

	cout<<L<<"\n"<<D<<"\n";
	return 0;
}
