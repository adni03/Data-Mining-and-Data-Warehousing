//FP GROWTH ALGORITHM
#include<bits/stdc++.h>
using namespace std;

fstream fout, fin;
vector<vector<int> > transactions;
int maxTrans = 500, maxItems = 5 /*20*/, numTrans = 9 /*1 + rand()%maxTrans*/;
int support = 0.3*numTrans, confidence=50;
const int maxK=5;
set<set<int> > sets;
vector<int> suppCount;
vector<vector<int> > oneItems, v, c;
vector<vector<pair<vector<int>, int> > > condPatternBase;

struct trnode{
	struct trnode* next[5];
	int item;
	int count;
};

typedef struct trnode* TRNODE;

bool comp(vector<int> a, vector<int> b){
	return a[1] > b[1];
}

bool comp2(vector<int> a, vector<int> b){
	return a[1] < b[1];
}

//UTIL FUNCTION USED TO LOAD ENTIRE DATABASE INTO A VECTOR
//OF SETS TO MAKE ACCESSING ITEM IN A TRANSACTION EASIER
void loadIntoTSet(){
	fout.open("database2.txt", ios::in);
	vector<int> s;
	int tId, data;
	vector<int> count;
	for(int i=0; i<maxItems; i++) count.push_back(0);
	for(int i=0; i<maxItems; i++) suppCount.push_back(-1);
	/** code takes database.txt as input and writes it to array of sets **/
	fout>>tId;
	while(!fout.eof()){
		fout>>data;
		if(data == -1){
			transactions.push_back(s);
			s.clear();
			fout>>tId;
		}
		else{
			s.push_back(data);
			count[data-1]++;
		}
	}
	for(int i=0; i<maxItems; i++){
		if(count[i] >= support){
			suppCount[i] = count[i];
		}
	}
	/** end of code **/
	fin.close();
	fout.close();
}

void sortFrequentOneItemSets(){
	for(int i=0; i<suppCount.size(); i++){
		vector<int> row;
		if(suppCount[i] != -1){
			row.push_back(i+1);
			row.push_back(suppCount[i]);
			oneItems.push_back(row);
		}
	}
	sort(oneItems.begin(), oneItems.end(), comp);
	cout<<"\nSorted Frequent 1 itemsets : \n";
	for(int i=0; i<oneItems.size(); i++){
		for(int j=0; j<oneItems[i].size(); j++){
			cout<<oneItems[i][j]<<" ";
		}
		cout<<"\n";
	}
}

void cleanDatabase(){
	for(int i=0; i<transactions.size(); i++){
		vector<int> newRow(transactions[i].size(), -1);
		int p=0;
		for(int j=0; j<oneItems.size(); j++){
			if(find(transactions[i].begin(), transactions[i].end(), oneItems[j][0]) != transactions[i].end()) newRow[p++] = oneItems[j][0];
		}
		transactions[i].clear();
		transactions[i] = newRow;
	}
	cout<<"Clean Database : \n";
	for(int i=0; i<transactions.size(); i++){
		for(int j=0; j<transactions[i].size(); j++){
			cout<<transactions[i][j]<<" ";
		}
		cout<<"\n";
	}
}

void insertIntoTrie(TRNODE &T, vector<int> v, int cnt = 1, int item = 0, int pos=0){
	if(pos < v.size()){
		if(!T){
			T = new(trnode);
			T->item = item;
			T->count = cnt;
			for(int i=0; i<maxItems; i++) T->next[i] = NULL;
			pos++;
		}
		else{
			if(T->item == item){
				T->count += cnt;
				pos++;
			}
		}
		insertIntoTrie(T->next[v[pos]-1], v, cnt, v[pos], pos);
	}
}

TRNODE constructFPTree(){
	TRNODE T = NULL;
	T = new(trnode);
	T->count = -1;
	T->item = -1;
	for(int i=0; i<maxItems; i++) T->next[i] = NULL;
	for(int i=0; i<transactions.size(); i++){
		insertIntoTrie(T, transactions[i]);
	}
	return T;
}

void printTree(TRNODE &T){
	queue<TRNODE> q;
	q.push(T);
	q.push(NULL);
	while(q.front() != q.back()){
		TRNODE temp = q.front();
		q.pop();
		if(!temp){
			cout<<"\n";
			temp = q.front();
			q.pop();
			q.push(NULL);
		}
		cout<<temp->item<<" "<<temp->count<<"\t";
		for(int i=0; i<maxItems; i++){
			if(temp->next[i]) q.push(temp->next[i]);
		}
	}
}

void generateAllPathsUtil(TRNODE &T, int item, vector<vector<int> > &v, vector<int> temp, vector<int> &cnts){
	if(T){
		if(T->item == item){
			if(temp.size()){
				v.push_back(temp);
				cnts.push_back(T->count);
			}
			return ;
		}
		else if(T->item != -1) temp.push_back(T->item);
		for(int i=0; i<maxItems; i++){
			if(T->next[i]){
				generateAllPathsUtil(T->next[i], item, v, temp, cnts);
			}
		}
	}
}

void generateAllPaths(TRNODE &T){
	vector<vector<int> > v;
	vector<int> temp, cnts;
	vector<pair<vector<int>, int> > row;
	for(int i=oneItems.size()-1; i>=0; --i){
		generateAllPathsUtil(T, oneItems[i][0], v, temp, cnts);
		for(int j=0; j<v.size(); j++){
			row.push_back({v[j], cnts[j]});
		}
		condPatternBase.push_back(row);
		row.clear();
		v.clear();
		cnts.clear();
		temp.clear();
	}
}

void printPaths(){
	for(int i=0; i<condPatternBase.size(); i++){
		for(int j=0; j<condPatternBase[i].size(); j++){
			pair<vector<int>, int> p = condPatternBase[i][j];
			for(int k=0; k<p.first.size(); k++){
				cout<<p.first[k]<<" ";
			}
			cout<<"\t"<<p.second<<"\t";
		}
		cout<<"\n";
	}
}

void retrieveFrequentItems(TRNODE &T, vector<int> &temp, vector<int> &cnts){
	if(T){
		if(T->count >= support && find(temp.begin(), temp.end(), T->item) == temp.end()){
			temp.push_back(T->item);
			cnts.push_back(T->count);
		}
		else{
			for(int j=0; j<temp.size(); j++){
				if(temp[j] == T->item) cnts[j] += T->count;
			}
		}
		for(int i=0; i<maxItems; i++){
			retrieveFrequentItems(T->next[i], temp, cnts);
		}
	}
}

void constructCondFPTree(){
	vector<int> temp, cnts;
	for(int i=0; i<condPatternBase.size(); i++){
		TRNODE T = NULL;
		T = new(trnode);
		T->count = -1;
		T->item = -1;
		for(int i=0; i<maxItems; i++) T->next[i] = NULL;
		for(int j=0; j<condPatternBase[i].size(); j++){
			pair<vector<int>, int> p = condPatternBase[i][j];
			insertIntoTrie(T, p.first, p.second);
		}
		retrieveFrequentItems(T, temp, cnts);
		if(temp.size()){
			v.push_back(temp);
			c.push_back(cnts);
		}
		cnts.clear();
		temp.clear();
	}
}

vector<vector<vector<int> > > generateSubsets(vector<vector<int> > v){
	vector<int> ent;
	vector<vector<int> > row;
	vector<vector<vector<int> > > subsets;
	for(int i=0; i<v.size(); i++){
		if(v[i].size() > 2){
			for(int j=0; j<v[i].size(); j++){
				for(int k=0; k<v[i].size(); k++){
					if(j != k) ent.push_back(v[i][k]);
				}
				row.push_back(ent);
				ent.clear();
			}
			subsets.push_back(row);
			row.clear();
		}
	}
	return subsets;
}

void generateMiningRules(){
	int o;
	for(int i=0; i<v.size(); i++){
		v[i].push_back(oneItems[oneItems.size()-1-i][0]);
	}
	vector<vector<vector<int> > > subsets = generateSubsets(v);
	for(int i=0; i<subsets.size(); i++){
		for(int j=0; j<subsets[i].size(); j++){
			for(int k=0; k<subsets[i][j].size(); k++){
				cout<<subsets[i][j][k]<<" ";
			}
			cout<<"\t"<<c[i][0]<<"\n";
		}
	}
	for(int i=0; i<v.size(); i++){
		for(int j=0; j<v[i].size(); j++){
			cout<<v[i][j]<<" ";
		}
		cout<<"\n";
	}
}

int main(){
	cout<<"Minimum support is : "<<support;
	cout<<"\nDatabase created successfully!";
	loadIntoTSet();
	sortFrequentOneItemSets();
	cleanDatabase();
	cout<<"Database cleaned and sorted!";
	TRNODE T = constructFPTree();
	cout<<"\nConstruction of FP tree done!";
	cout<<"\nPrinting tree : \n";
	printTree(T);
	generateAllPaths(T);
	cout<<"\nAll paths generated! : \n";
	printPaths();
	constructCondFPTree();
	cout<<"Conditional Tree constructed!";
	cout<<"\nMining rules : \n";
	generateMiningRules();
	return 0;
}
