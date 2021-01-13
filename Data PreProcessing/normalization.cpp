// NORMALIZATION
#include<bits/stdc++.h>
using namespace std;

vector<float> data;
float new_min = 0, new_max = 5;
int numEle = 10;
float old_min = INT_MIN, old_max = INT_MAX;

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

float newVal(int x){
	return (float)(((x - old_min)/(old_max - old_min))*(new_max - new_min) + new_min);
}

void minmaxNormalization(){
	old_min = *min_element(data.begin(), data.end());
	old_max = *max_element(data.begin(), data.end());
	for(int i=0; i<data.size(); i++){
		data[i] = newVal(data[i]);
	}
}

float calcMean(){
	float x = 0;
	for(int i=0; i<data.size(); i++){
		x += data[i];
	}
	return x/data.size();
}

float calcSD(int m){
	float x = 0;
	for(int i=0; i<data.size(); i++){
		x += pow(data[i] - m, 2);
	}
	x = x/data.size();
	return sqrt(x);
}

void zNormalization(){
	float mean = calcMean();
	float sd = calcSD(mean);
	for(int i=0; i<data.size(); i++){
		data[i] = (data[i] - mean)/sd;
	}
}

int main(){
	generateRandPoints();
	cout<<"Generated data successfully! : \n";
	printData();
	minmaxNormalization();
	cout<<"\nMin-Max Normalization complete! : \n";
	printData();
	data.clear();
	cout<<"\nGenerating new data! : \n";
	generateRandPoints();
	printData();
	zNormalization();
	cout<<"\nZ-score Normalization comeplete! : \n";
	printData();
	return 0;
}
