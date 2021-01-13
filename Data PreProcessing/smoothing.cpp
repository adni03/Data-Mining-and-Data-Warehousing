// DATA PREPROCESSING
#include<bits/stdc++.h>
using namespace std;

int numBins = 5, binSize = 5, numEle = numBins*binSize;
vector<int> data;
vector<vector<int> > bin;

void generateRandPoints(){
	for(int i=0; i<numEle; i++){
		int num = rand()%numEle;
		data.push_back(num);
	}
}

vector<int> calculateBinMeans(){
	vector<int> mean;
	for(int i=0; i<bin.size(); i++){
		int x = 0;
		for(int j=0; j<bin[i].size(); j++){
			x += bin[i][j];
		}
		mean.push_back(x/bin[i].size());
	}
	return mean;
}

void doBinning(int ch){
	vector<int> row, means;
	int j=0;
	sort(data.begin(), data.end());
	if(ch == 1){
		for(int i=1; i<=data.size(); i++){
			row.push_back(data[i-1]);
			if(i%binSize == 0){
				bin.push_back(row);
				row.clear();
			}
		}
		means = calculateBinMeans();
		for(int i=1; i<=data.size(); i++){
			data[i-1] = means[j];
			if(i%binSize == 0) j++;
		}
	}
	else{
		int i=0, j=0;
		while(i < data.size()){
			int min = data[i], max = data[i+binSize-1];
			for(j = i; j < i + binSize ; j++){
				if(fabs(data[j] - min) < fabs(data[j] - max)) data[j] = min;
				else data[j] = max;
			}
			i = j;
		}
	}
}

void printData(){
	for(int i=0; i<data.size(); i++){
		cout<<data[i]<<" ";
	}
}

int main(){
	generateRandPoints();
	cout<<"Data points generated! : \n";
	sort(data.begin(), data.end());
	printData();
	cout<<"\nMean value replacement binning : \n";
	doBinning(1);
	printData();
	data.clear();
	generateRandPoints();
	cout<<"\nNew Data : \n";
	sort(data.begin(), data.end());
	printData();
	cout<<"\nBoundary value replacement binning : \n";
	doBinning(2);
	printData();
	return 0;
}
