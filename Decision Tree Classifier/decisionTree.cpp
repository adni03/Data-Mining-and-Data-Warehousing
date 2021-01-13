// DECISION TREE CLASSIFICATION ALGORITHM
#include<bits/stdc++.h>
using namespace std;

vector<vector<string> > database;
vector<vector<string> > attributes;
int trainTuples, numAttributes;

struct dtnode{
	string name;
	int isLeaf;
	vector<string> types;
	struct dtnode* next[50];
	struct dtnode* desc;
	int numTypes;
};

typedef struct dtnode* DTPTR;

vector<string> convertToVector(set<string> s){
	set<string> :: iterator i = s.begin();
	vector<string> v;
	for(; i != s.end(); i++){
		v.push_back(*i);
	}
	return v;
}

void extractDatabase(){
	fstream fin;
	string str;
	int i=0;
	vector<string> row;
	fin.open("DTbase.txt", ios::in);
	fin>>trainTuples>>numAttributes;
	while(i < trainTuples){
		while(1){
			fin>>str;
			if(str == "/") break;
			row.push_back(str);
		}
		database.push_back(row);
		row.clear();
		++i;
	}
}

void printDatabase(){
	for(int i=0; i<database.size(); i++){
		for(int j=0; j<database[i].size(); j++){
			cout<<database[i][j]<<" ";
		}
		cout<<"\n";
	}
}

void createAttributeList(){
	set<string> s;
	for(int i=1; i<database[0].size()-1; i++){
		vector<string> row;
		row.push_back(database[0][i]);
		for(int j=1; j<database.size(); j++){
			s.insert(database[j][i]);
		}
		vector<string> temp = convertToVector(s);
		for(int k=0; k<temp.size(); k++){
			row.push_back(temp[k]);
		}
		attributes.push_back(row);
		s.clear();
	}
}

int allOfSameClass(vector<vector<string> > D){
	string str = D[1][D[0].size()-1];
	for(int i=1; i<D.size(); i++){
		string str2 = D[i][D[i].size()-1];
		if(str2 != str) return 0;
	}
	return 1;
}

vector<string> subSetList(){
	vector<string> list;
	for(int i=1; i<database[0].size()-1; i++){
		list.push_back(database[0][i]);
	}
	return list;
}

string majorityVote(vector<vector<string> > D){
	int cnt[2] = {0};
	for(int i=1; i<D.size(); i++){
		if(D[i][D[0].size()-1] == "no") cnt[0]++;
		else cnt[1]++;
	}
	return cnt[0] > cnt[1] ? "no" : "yes";
}

float calcProbability(vector<vector<string> > D, int x){
	int cnt = 0;
	for(int i=1; i<D.size(); i++){
		if(x == 0){
			if(D[i][D[0].size()-1] == "no") cnt++;
		}
		else{
			if(D[i][D[0].size()-1] == "yes") cnt++;
		}
	}
	return (float)cnt/(D.size()-1);
}

float calcInfo(vector<vector<string> > D){
	float infoD;
	for(int i=0; i<2; i++){
		float pi = calcProbability(D, i);
		infoD += -1*pi*log2(pi);
	}
	return infoD;
}

vector<float> calcInfoa(vector<vector<string> > D, int i){
	map<string, int> m;
	vector<float> v;
	float cnt[2] = {0};
	for(int j=1; j<attributes[i].size(); j++){
		string s = attributes[i][j];
		for(int k=1; k<D.size(); k++){
			if(D[k][i+1] == s){
				if(D[k][D[0].size()-1] == "no") cnt[0]++;
				else cnt[1]++;
			}
		}
		float p = 0;
		for(int y=0; y<2; y++){
			if(cnt[y] != 0) p += -1*(cnt[y]/(cnt[0]+cnt[1]))*log2(cnt[y]/(cnt[0]+cnt[1]));
		}
		v.push_back(p);
		cnt[0] = cnt[1] = 0;
	}
	return v;
}

int InformationGain(vector<vector<string> > D, vector<string> list, int listCnt){
	float infoD = 0;
	float infoa[attributes.size() - listCnt] = {0};
	float gain[attributes.size() - listCnt] = {0};
	int numClass = 2, pos;
	infoD = calcInfo(D);
	for(int i=listCnt; i<attributes.size(); i++){
		map<string, float> m;
		for(int j=1; j<D.size(); j++){
			if(m.find(D[j][i+1]) == m.end()) m.insert({D[j][i+1], 1});
			else m[D[j][i+1]]++;
		}
		vector<float> v = calcInfoa(D, i);
		map<string, float> :: iterator itr = m.begin();
		for(int t=0; itr!=m.end(); itr++, t++){
			float val = (float)(itr->second/(D.size()-1));
			infoa[i] += val*v[t];
		}
	}
	float max = -1;
	for(int i=listCnt; i<attributes.size(); i++){
		gain[i] = infoD - infoa[i];
		if(gain[i] > max){
			max = gain[i];
			pos = i;
		}
	}
	return pos;
}

vector<vector<string> > partition(vector<vector<string> > D, string s, int split){
	vector<vector<string> > subD;
	subD.push_back(D[0]);
	for(int i=1; i<D.size(); i++){
		if(D[i][split+1] == s) subD.push_back(D[i]);
	}
	return subD;
}

void constructTree(DTPTR &T, vector<vector<string> > D, vector<string> list, int listCnt=0){
	T = new(dtnode);
	for(int i=0; i<50; i++){
		T->next[i] = NULL;
	}
	T->desc = NULL;
	if(allOfSameClass(D)){
		T->name = D[1][D[0].size()-1];
		T->isLeaf = 1;
		return ;
	}
	if(list.size() == listCnt){
		T->isLeaf = 1;
		T->name = majorityVote(D);
		return ;
	}
	int split = InformationGain(D, list, listCnt);
	T->name = list[split];
	listCnt++;
	T->isLeaf = 0;
	for(int i=1; i<attributes[split].size(); i++){
		T->types.push_back(attributes[split][i]);
		T->numTypes++;
	}
	for(int i=1; i<attributes[split].size(); i++){
		vector<vector<string> > subD = partition(D, attributes[split][i], split);
		if(subD.size() == 0){
			T->desc = new(dtnode);
			T->desc->isLeaf = 1;
			T->desc->name = majorityVote(subD);
		}
		else{
			constructTree(T->next[i-1], subD, list, listCnt);
		}
	}
}

string testInput(DTPTR T, vector<vector<string> > input){
	if(T){
		if(T->isLeaf) return T->name;
		for(int i=0; i<input.size(); i++){
			if(input[i][0] != T->name) continue;
			for(int j=0; j<T->types.size(); j++){
				if(T->types[j] == input[i][1]){
					return testInput(T->next[j], input);	
				}
			}
		}
	}
}

void printTree(DTPTR T){
	queue<DTPTR> q;
	q.push(T);
	q.push(NULL);
	while(q.front() != q.back()){
		DTPTR Temp = q.front();
		q.pop();
		if(!Temp){
			cout<<"\n\n";
			Temp = q.front();
			q.pop();
			q.push(NULL);
		}
		cout<<Temp->name<<"\t";
		for(int i=0; i<50; i++){
			if(Temp->next[i]) q.push(Temp->next[i]);
		}
	}
}

void getInput(vector<vector<string> > &input){
	vector<string> row;
	for(int i=0; i<4; i++){
		string str;
		cin>>str;
		row.push_back(str);
		cin>>str;
		row.push_back(str);
		input.push_back(row);
		row.clear();
	}
}

int main(){
	extractDatabase();
	cout<<"Database extracted!";
	cout<<"\nDatabase is : \n";
	printDatabase();
	createAttributeList();
	cout<<"\nCreating tree : \n";
	DTPTR T = NULL;
	vector<string> list = subSetList();
	constructTree(T, database, list);
	cout<<"\nTree constructed successfully!";
	vector<vector<string> > input;
	cout<<"\nEnter test tuple : \n";
	getInput(input);
	cout<<"On testing : "<<testInput(T, input);
	return 0;
}
