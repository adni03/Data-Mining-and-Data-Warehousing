// DBSCAM ALGORITHM
#include<bits/stdc++.h>
using namespace std;

vector<int> data;
int numEle = 25, minPts = 5, eps = 5;
vector<vector<int> > clusters;

void generateRandPoints(){
	for(int i=0; i<numEle; i++){
		float num = rand()%numEle;
		data.push_back(num);
	}
}

void printData(){
	for(int i=0; i<data.size(); i++){
		cout<<data[i]<<" ";
	}
}

int isNotVisited(vector<int> visited){
	for(int i=0; i<visited.size(); i++){
		if(visited[i] == 0) return 1;
	}
	return 0;
}

void doClustering(){
	vector<int> visited(numEle, 0);
	while(isNotVisited(visited)){
		int i = rand()%data.size();
		if(!visited[i]){
			visited[i] = 1;
			int cnt=0;
			vector<int> N;
			for(int j=0; j<data.size(); j++){
				if(i == j) continue;
				if(fabs(data[i] - data[j]) <= eps){
					cnt++;
					N.push_back(data[j]);
				}
			}
			if(cnt >= minPts){
				vector<int> row;
				cnt=0;
				row.push_back(data[i]);
				clusters.push_back(row);
				int j = rand()%N.size();
				if(!visited[j]){
					visited[j] = 1;
					vector<int> NN;
					for(int j=0; j<data.size(); j++){
						if(i == j) continue;
						if(fabs(data[i] - data[j]) <= eps){
							cnt++;
							NN.push_back(data[j]);
						}
					}
					if(cnt >= minPts){
						for(int k=0; k<NN.size(); k++){
							N.push_back(NN[k]);
						}
					}
				}
			}
			else{
				N.clear();
				data.erase(data.begin() + i);
			}
		}
	}
}

int main(){
	generateRandPoints();
	cout<<"Random data generated successfully! : \n";
	printData();
	doClustering();
	return 0;
}
