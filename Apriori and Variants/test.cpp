#include<bits/stdc++.h>
using namespace std;

int main(){
	vector<int> a;
	vector<int> b;
	for(int i=0; i<3; i++){
		a.push_back(i);
		b.push_back(2*i);
	}
	if(a == b) cout<<"hello";
	return 0;
}
