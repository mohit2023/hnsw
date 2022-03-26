#include <bits/stdc++.h>

using namespace std;

int main() {
    int k=5;
    fstream in1,in2;
    in1.open("output.txt");
    in2.open("user_gt.txt");
    vector<vector<int>> pred(10000,vector<int>(k));
    for(int i=0;i<10000;i++){
        for(int j=0;j<k;j++){
            in1>>pred[i][j];
        }
    }
    vector<vector<int>> given(10000);
    int itr=0;
    string line;
    while(getline(in2,line)) {
        // cout<<line<<"\n";
        int n=line.size();
        char str[n+1];
        strcpy(str,line.c_str());
        char* temp = strtok(str," ");
        vector<string> tokens;
        while(temp != NULL){
            tokens.push_back(temp);
            temp = strtok(NULL," ");
        }
        for(int j=0;j<tokens.size();j++){
            given[itr].push_back(stoi(tokens[j]));
        }
        itr++;
    }

    double p=0;
    double r=0;
    for(int i=0;i<10000;i++){
        sort(pred[i].begin(), pred[i].end());
        sort(given[i].begin(), given[i].end());
        vector<int> v(pred.size() + given.size());
        vector<int>::iterator it, st;
  
        int x=0,y=0;
        double count=0;
        while(x<pred[i].size() && y<given[i].size()){
            if(pred[i][x]==given[i][y]){
                count++;
                x++;y++;
            } else if(pred[i][x]<given[i][y]) {
                x++;
            } else {
                y++;
            }
        }
        // cout<<count<<"\n";
        p+= (count/k);
        r+= (count/given[i].size());
    }    

    p = p/10000;
    r = r/10000;

    cout<<p<<" is precision \n";
    cout<<r<<" is recall \n";

    return 0;
}