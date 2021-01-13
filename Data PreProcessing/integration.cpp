// DATA INTEGRATION
#include<bits/stdc++.h>
using namespace std;

int numEle = 10;
vector<int> A, B;

void generateRandPoints(int ch){
	for(int i=0; i<numEle; i++){
		int num = rand()%numEle;
		if(ch == 1) A.push_back(num);
		else B.push_back(num);
	}
}

void printData(int ch){
	for(int i=0; i<numEle; i++){
		if(ch == 1) cout<<A[i]<<" ";
		else cout<<B[i]<<" ";
	}
}

float calcMean(int ch){
	float x = 0;
	for(int i=0; i<numEle; i++){
		if(ch == 1) x += A[i];
		else x += B[i];
	}
	if(ch == 1) return x/A.size();
	else return x/B.size();
}

float calcSD(int m, int ch){
	float x = 0;
	for(int i=0; i<numEle; i++){
		if(ch == 1) x += pow(A[i] - m, 2);
		else x += pow(B[i] - m, 2);
	}
	if(ch == 1) x = x/A.size();
	else x = x/B.size();
	return sqrt(x);
}

float calcCorrelation(){
	float a = calcMean(1), b = calcMean(2);
	float sa = calcSD(a, 1), sb = calcSD(b, 2);
	float val = 0;
	for(int i=0; i<numEle; i++){
		val += (A[i] - a)*(B[i] - b);
	}
	val = val/(numEle*sa*sb);
	return val;
}

int main(){
	generateRandPoints(1);
	cout<<"Data generated for first set! : \n";
	printData(1);
	generateRandPoints(2);
	cout<<"\nData generated for second set! : \n";
	printData(2);
	float val = calcCorrelation();
	cout<<"\nCorrelation of : "<<val;
	if(val == 0) cout<<"\nData needs to be integrated!";
	else cout<<"\nDoesnt have to be integrated!";
	return 0;
}
