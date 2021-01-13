// k - means DATA CLUSTERING ALGORITHM
#include<bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int> > > cluster, fcluster;
vector<vector<int> > dataset;
vector<pair<int, int> > pmeans, means, centers;
int n = 10, k = 10;

bool comp(pair<int, int> a, pair<int, int> b){
	return a.second < b.second;
}

void initializeMeans(){
	pair<int, int> p;
	p.first = p.second = 0;
	for(int i=0; i<k; i++){
		means.push_back(p);
	}
}

void initializeDataset(){
	vector<int> xs, ys;
	for(int i=0; i<n; i++){
		int x = rand()%n;
		int y = rand()%n;
		xs.push_back(x);
		ys.push_back(y);
	}
	dataset.push_back(xs);
	dataset.push_back(ys);
}

void printClusters(){
	for(int i=0; i<fcluster.size(); i++){
		cout<<"\nCenter is : "<<"<"<<centers[i].first<<","<<centers[i].second<<">"<<"\n";
		for(int j=0; j<fcluster[i].size(); j++){
			pair<int, int> p = fcluster[i][j];
			cout<<"<"<<p.first<<","<<p.second<<">"<<"\n";
		}
		cout<<"\n";
	}
	for(int i=0; i<fcluster.size(); i++){
		cout<<"Cluster : "<<i+1<<" size : "<<fcluster[i].size()<<"\n";
	}
}

void initializeKCenters(){
	set<int> uniq;
	int cnt=0;
	vector<pair<int, int> > vtemp;
	while(cnt < k){
		int val = rand()%n;
		pair<int, int> temp;
		if(uniq.find(val) == uniq.end()){
			temp.first = dataset[0][val];
			temp.second = dataset[1][val];
			vtemp.push_back(temp);
			cluster.push_back(vtemp);
			centers.push_back(temp);
			++cnt;
			uniq.insert(val);
			vtemp.clear();
		}
	}
}

void updateMeans(){
	for(int i=0; i<cluster.size(); i++){
		int xs = 0, ys = 0;
		for(int j=0; j<cluster[i].size(); j++){
			xs += cluster[i][j].first;
			ys += cluster[i][j].second;
		}
		means[i].first = xs/cluster[i].size();
		means[i].second = ys/cluster[i].size();
	}
}

int calcDistance(int x, int y, int a, int b){
	return sqrt(pow(x-a, 2) + pow(y-b, 2));
}

void addCentersToCluster(){
	for(int i=0; i<centers.size(); i++){
		vector<pair<int, int> > temp;
		temp.push_back(centers[i]);
		cluster.push_back(temp);
	}
}

void clusterDataset(){
	initializeKCenters();
	vector<pair<int, int> > dist;
	updateMeans();
	while(!(pmeans == means)){
		for(int i=0; i<dataset[0].size(); i++){
			dist.clear();
			pair<int, int> p;
			p.first = dataset[0][i];
			p.second = dataset[1][i];
			if(find(centers.begin(), centers.end(), p) != centers.end()) continue;
			for(int j=0; j<means.size(); j++){
				int d = calcDistance(means[j].first, means[j].second, dataset[0][i], dataset[1][i]);
				dist.push_back({j, d});
			}
			sort(dist.begin(), dist.end(), comp);
			cluster[dist[0].first].push_back({dataset[0][i], dataset[1][i]});
		}
		pmeans = means;
		updateMeans();
		fcluster = cluster;
		cluster.clear();
		addCentersToCluster();
	}
}

int main(){
	cout<<"Enter number of data points : ";
	cin>>n;
	cout<<"Enter number of clusters : ";
	cin>>k;
	if(n < k){
		cout<<"Incorrect number of clusters";
		return 0;
	}
	initializeMeans();
	cout<<"Means initialized!";
	initializeDataset();
	cout<<"\nDataset initialized!";
	cout<<"\nClustering algorithm starts : ";
	clusterDataset();
	cout<<"\nClusters are : \n";
	printClusters();
	return 0;
}
