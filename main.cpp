#include <bits/stdc++.h>
#include <mpi.h>
#include <omp.h> 

using namespace std;

int k = 2;

float cosine_dist(vector<float>& q, vector<float>& vec){
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

void SearchLayer(vector<float>& q, vector<pair<float,int>>& top_k, vector<int>& indptr, vector<int>& index, vector<int>& level_offset, int lc, unordered_map<int,int>& visited, vector<vector<float>>& vect){
	vector<int> candidates(top_k.size());
	for(int i=0;i<candidates.size();i++){
		candidates[i] = top_k[i].second;
	}
	int len = candidates.size();
	while(len > 0){
		int ep = candidates[len - 1];
		len--; candidates.pop_back();
		int start = indptr[ep] + level_offset[lc];
		int end = indptr[ep] + level_offset[lc+1];
		for(int pxi = start;pxi<end;pxi++){
			int px = index[pxi];
			if(px == -1 || visited[px] == 1){
				continue;
			}
			visited[px] = 1;
			float dist = cosine_dist(q,vect[px]);
			if(dist > top_k.front().first && top_k.size() >= k){
				continue;
			}
			top_k.push_back(make_pair(dist,px));
			push_heap(top_k.begin(),top_k.end());
			if(top_k.size() > k){
				pop_heap(top_k.begin(),top_k.end());
				top_k.pop_back();
			}
			candidates.push_back(px);
			len++;
		}
	}
	return ;
}

void QueryHNSW(vector<float>& q, vector<pair<float,int>>& top_k, int ep, vector<int>& indptr, vector<int>& index, vector<int>& level_offset, int max_level, vector<vector<float>>& vect){
	// make_heap(top_k.begin(),top_k.end());
	top_k.push_back({cosine_dist(q,vect[ep]), ep});
	push_heap(top_k.begin(),top_k.end());
	unordered_map<int,int> visited;
	visited[ep] = 1;
	int L = max_level;
	for(int level = L-1; level>=0;level--){
		SearchLayer(q,top_k,indptr,index,level_offset,level,visited,vect);
	}
	return ;
}

int main(int argc, char* argv[]){

	if(argc!=5){
		cout<<"INVALID ARGS\n";
		return 0;
	}

	string outpath = argv[1];
	k = stoi(argv[2]);
	string user_file = argv[3];
	string output_file = argv[4];

	int max_level, ep, num, L, U, D;
	float ftemp;
	vector<int> level_offset;
	vector<int> index;
	vector<vector<float>> user;
	ifstream ifs;

	ifs.open(outpath+"/items.bin", ios::in | ios::binary);
	ifs.read((char*)&max_level, 4);
	ifs.read((char*)&ep, 4);
	ifs.read((char*)&L,4);
	ifs.read((char*)&D,4);
	ifs.close();
	
	int itr=0;
	vector<int> level(L);
	ifs.open(outpath+"/level.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		level[itr++] = num-1;
	}
	ifs.close();
	
	ifs.open(outpath+"/level_offset.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		level_offset.push_back(num);
	}
	ifs.close();
	
	vector<int> indptr(L+1);
	itr=0;
	ifs.open(outpath+"/indptr.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		indptr[itr++] = num;
	}
	ifs.close();
	
	ifs.open(outpath+"/index.bin", ios::in | ios::binary);
	while(ifs.read((char*)&num, 4)){
		index.push_back(num);
	}
	ifs.close();
	
	vector<vector<float>> vect(L,vector<float>(D));
	ifs.open(outpath+"/vect.bin");
	for(int i=0;i<L;i++){
		for(int j=0;j<D;j++){
			ifs.read((char*)&ftemp, sizeof(float));
			vect[i][j]=ftemp;
		}
	}
	ifs.close();
	
	ifs.open(user_file);
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
	
	int rank,size;
	
	MPI_Init(NULL,NULL);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int numt;
	#pragma omp parallel
	{
		#pragma omp single
		numt = omp_get_num_threads();
	}
	// cout<<numt<<"\n";
	// int numt=1;

	int total = size;
	int per_thread = U/total;
	int extra = U%total;
	
	int perNode[size+1];
	for(int i=0;i<size;i++){
		int start = per_thread*i;
		start = start + (i<extra ? i : extra);
		perNode[i] = start;
	}
	perNode[size] = U;
	
	int data[perNode[rank+1]-perNode[rank]][k];
	
	int total_user_omp = perNode[rank+1]-perNode[rank];
	int per_omp_thread = total_user_omp/numt;
	int extra_omp = total_user_omp%numt;
	int perThread[numt+1];
	perThread[0] = perNode[rank];
	for(int i=0;i<numt;i++){
		int start = per_omp_thread*i;
		start = start + (i<extra_omp ? i : extra_omp);
		perThread[i] = perNode[rank] + start;
	}
	perThread[numt] = perNode[rank+1];
	#pragma omp parallel num_threads(numt)
	{
		int tid = omp_get_thread_num();
		for (int i=perThread[tid];i<perThread[tid+1];i++){
			
			vector<pair<float,int>> top_k;
			QueryHNSW(user[i],top_k,ep,indptr,index,level_offset,max_level,vect);
			sort_heap(top_k.begin(),top_k.end());
			int j = 0;
			for(const auto &itr : top_k){
				data[i-perThread[0]][j++] = itr.second;
			}
		}
	}

	// for(int i=0;i<U;i++) {
	// 	for(int j=0;j<k;j++) {
	// 		cout<<data[i][j]<<" ";
	// 	}
	// 	cout<<"\n";
	// }
	ofstream MyWriteFile(output_file);
	// TODO: write output in parallel
	MyWriteFile.close();
	
	MPI_Finalize();

	return 0;
}
