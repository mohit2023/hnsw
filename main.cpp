#include <bits/stdc++.h>
#include <mpi.h>
#include <omp.h> 

using namespace std;

int k = 2;

float cosine_dist(vector<float> q, vector<float> vec){
	float ans = 0.0;
	float a = 0.0;
	float b =0.0;
	for(int i=0;i<q.size();i++){
		ans = ans + q[i]*vec[i];
		a = a + q[i]*q[i];
		b = b + vec[i]*vec[i];
	}
	a = sqrt(a);
	b = sqrt(b);
	ans = ans/(a*b);
	return 1 - ans;
}

vector<pair<float,int> > SearchLayer(vector<float> q, vector<pair <float,int> > candidates, vector<int> indptr, vector<int> index, vector<int> level_offset, int lc, unordered_map<int,int> visited, vector<vector<float> > vect){
	vector<pair<float,int> > top_k;
	for(int i=0;i<candidates.size();i++){
		top_k.push_back(candidates[i]);
	}
	while(candidates.size() > 0){
		int ep = candidates[candidates.size() - 1].second;
		candidates.pop_back();
		int start = indptr[ep] + level_offset[lc];
		int end = indptr[ep] + level_offset[lc+1];
		for(int pxi = start;pxi<end;pxi++){
			int px = index[pxi];
			if(visited[px] == 1 || px == -1){
				continue;
			}
			visited[px] = 1;
			float dist = cosine_dist(q,vect[px]);
			if(dist > top_k.front().second && top_k.size() >= k){
				continue;
			}
			top_k.push_back(make_pair(dist,px));
			push_heap(top_k.begin(),top_k.end());
			if(top_k.size() > k){
				pop_heap(top_k.begin(),top_k.end());
				top_k.pop_back();
			}
			candidates.push_back(make_pair(dist,px));
		}
	}
	return top_k;
}

vector<pair<float,int> > QueryHNSW(vector<float> q, vector<pair<float, int>> top_k, int ep, vector<int> indptr, vector<int> index, vector<int> level_offset, int max_level, vector<vector<float> > vect){
	make_heap(top_k.begin(),top_k.end());
	top_k.push_back(make_pair(cosine_dist(q,vect[ep]), ep));
	push_heap(top_k.begin(),top_k.end());
	unordered_map<int,int> visited;
	visited[ep] = 1;
	int L = max_level;
	for(int level = L-1; level>=0;level--){
		top_k = SearchLayer(q,top_k,indptr,index,level_offset,level,visited,vect);
	}
	return top_k;
}

int main(){

	int max_level, ep, num, L, U, D;
	float ftemp;
	vector<int> level_offset;
	vector<int> index;
	vector<vector<float>> user;
	ifstream ifs;

	ifs.open("items.bin", ios::in | ios::binary);
	ifs.read((char*)&L,4);
	ifs.read((char*)&D,4);
	ifs.close();
	ifs.clear();
	
	ifs.open("max_level.bin", ios::in | ios::binary);
	ifs.read((char*)&max_level, 4);
	ifs.close();
	ifs.clear();
	
	ifs.open("ep.bin", ios::in | ios::binary);
	ifs.read((char*)&ep, 4);
	ifs.close();
	ifs.clear();
	
	int itr=0;
	vector<int> level(L);
	ifs.open("level.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		level[itr++] = num-1;
	}
	ifs.close();
	ifs.clear();
	
	ifs.open("level_offset.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		level_offset.push_back(num);
	}
	ifs.close();
	ifs.clear();
	
	vector<int> indptr(L+1);
	itr=0;
	ifs.open("indptr.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		indptr[itr++] = num;
	}
	ifs.close();
	ifs.clear();
	
	ifs.open("index.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		index.push_back(num);
	}
	ifs.close();
	ifs.clear();
	
	vector<vector<float>> vect(L,vector<float>(D));
	ifs.open("vect.bin");
	for(int i=0;i<L;i++){
		for(int j=0;j<D;j++){
			ifs.read((char*)&ftemp, sizeof(float));
			vect[i][j]=ftemp;
			cout<<ftemp<<" ";
		}
		cout<<"\n";
	}
	ifs.close();
	ifs.clear();
	
	ifs.open("user.txt");
	int d = 0;
	U = 0;
	vector<float> temp(D);
	while(ifs >> ftemp){
		temp[d++] = ftemp;
		if(d==D){
			user.push_back(temp);
			U++;
			d=0;
		}
	}
	ifs.close();
	ifs.clear();
	
	int rank,size;
	
	MPI_Init(NULL,NULL);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int perNode[size+1];
	perNode[0] = 0;
	for(int i=0;i<size;i++){
		perNode[i+1] = min((i+1)*(int)(ceil(U/4)),U);
	}
	
	int data[perNode[rank+1]-perNode[rank]][k];
	
	int numt = omp_get_num_threads();
	int perThread[numt+1];
	perThread[0] = perNode[rank];
	for(int i=0;i<numt;i++){
		perThread[i+1] = perNode[rank] + min((i+1)*(int)(ceil(perNode[rank+1]-perNode[rank])/numt),perNode[rank+1]-perNode[rank]);
	}
	#pragma omp parallel num_threads(numt)
	{
		int tid = omp_get_thread_num();
		for (int i=perThread[tid];i<perThread[tid+1];i++){
			
			vector<pair<float,int> > top_k;
			//make_heap(top_k.begin(),top_k.end());
			top_k = QueryHNSW(user[i],top_k,ep,indptr,index,level_offset,max_level,vect);
			sort_heap(top_k.begin(),top_k.end());
			int j = 0;
			for(const auto &itr : top_k){
				data[i][j] = itr.second;
			}
		}
	}
	ofstream MyWriteFile("output.txt");
	MyWriteFile.close();
	
	MPI_Finalize();
	return 0;
}
