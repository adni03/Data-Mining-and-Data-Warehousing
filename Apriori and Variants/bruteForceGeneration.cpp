//BRUTE FORCE ALGORITHM FOR GENERATION OF FREQUENT ITEM SETS
#include<bits/stdc++.h>
using namespace std;

fstream fout, fin;
vector<set<int> > transactions;
int maxTrans = 500, maxItems = 5 /*20*/, numTrans = 9 /*1 + rand()%maxTrans*/, support = 0.3*numTrans, confidence = 50;
const int maxK=4;
const char names [maxK][100] = {"L1.txt", "L2.txt", "L3.txt", "L4.txt"};
fstream file[maxK];
set<set<int> > sets;
vector<int> suppCount;

/******************** START OF FUNCTIONS *******************/

/************************ FUNCTION TO CREATE A RANDOM DATABASE *******************/

/*
//GENERATES UNIQUE TRANSACTION IDS
//ALL PREVIOUS TIDS ARE STORED IN A SET
//TO MAINTAIN UNQUNESS
int findTransaction(set<int> &s, int n){
	int trans = 1+rand()%n;
	while(s.find(trans) != s.end()){
		trans = 1+rand()%n;
	}
	s.insert(trans);
	return trans;
}

//GENERATES UNIQUE ITEMS PER TRANSACTION
//STORE ALL ITEMS FOR A PARTICULAR TRANSACTION
//IN A SET. IF AN ITEM IS NEW, ADD IT TO SET AND
//RETURN THE ITEM
int findItems(set<int> &s, int n){
	int item = 1+rand()%n;
	while(s.find(item) != s.end()){
		item = 1+rand()%n;
	}
	s.insert(item);
	return item;
}

//CREATES A SAMPLE DATABASE
void createDatabase(fstream &fout, int maxTrans, int maxItems, int numTrans){
	fout.open("dataBase.txt", ios::out);
	set<int> transactions, items;
	for(int i=0; i<numTrans; i++){
		int trans = findTransaction(transactions, maxTrans);
		fout<<trans<<"\t";
		int numItems = 1 + rand()%maxItems;
		for(int j=0; j<numItems; j++){
			int item = findItems(items, maxItems);
			fout<<item<<" ";
		}
		items.clear();
		fout<<-1;
		fout<<"\n";
	}
	fout.close();
}
*/

/**************************** END ***************************/

//TRAVERSES THROUGH THE TRANSACTION DATABASE SET TO FIND
//SUPPORT COUNT OF THE GIVEN SUBSET AND RETURNS THE SUPPORT
//COUNT
int checkSupport(vector<int> v, int &supp){
	int flag=0;
	for(int i=0; i<transactions.size(); i++){
		flag = 0;
		for(int j=0; j<v.size(); j++){
			if(transactions[i].find(v[j]) == transactions[i].end()){
				flag = 1;
				break;
			}
		}
		if(flag != 1) supp++;
	}
	return supp;
}

//WRITES TO APPROPRIATE FILE
void writeToFile(vector<int> s, int sup, int k){
	fstream f;
	f.open(names[k-1], ios::out | ios::app);
	for(int i=0; i<s.size(); i++){
		f<<s[i]<<" ";
	}
	f<<"\t"<<sup<<"\t-1\n";
}

//WRITES TO ASSOCIATION RULES TEXT FILE
void writeToAssocFile(vector<int> a, vector<int> b, int conf){
	fstream f;
	f.open("Rules.txt", ios::out | ios::app);
	if(conf >= confidence){
		for(int i=0; i<a.size(); i++){
		f<<a[i]<<" ";
		}
		f<<"\t";
		for(int i=0; i<b.size(); i++){
			f<<b[i]<<" ";
		}
		f<<"\t"<<conf<<"\t-1\n";
	}
}

//BRUTE FORCE METHOD THAT GENERATES ALL 'K' LENGTH SUBSETS
//OF GIVEN SET. IT THEN CHECKS IF IT IS FREQUENT AND THE CALLS
//WRITE TO FILE FUNCTION
void generateFsets(vector<int> items, vector<int> row, int k, int ch, int pos=0){
	if(ch == 1){
		if(row.size() == k){
			int supp=0;
			if(checkSupport(row, supp) >= support){
				writeToFile(row, supp, k);
			}
			return ;
		}
		for(int i=pos; i<items.size(); i++){
			row.push_back(items[i]);
			generateFsets(items, row, k, ch, i+1);
			row.pop_back();
		}
	}
	else{
		if(row.size() > 0 && row.size() < maxK-1){
			vector<int> a = row;
			vector<int> b;
			int i=0, j=0;
			while(i < items.size() && j < row.size()){
				if(items[i] != row[j]) b.push_back(items[i++]);
				else{
					++i;
					++j;
				}
			}
			while(i < items.size()){
				b.push_back(items[i++]);
			}
			int supp=0;
			float x = checkSupport(a, supp);
			supp=0;
			float y = checkSupport(items, supp);
			int conf = (y/x)*100;
			writeToAssocFile(a, b, conf);
		}
		for(int i=pos; i<items.size(); i++){
			row.push_back(items[i]);
			generateFsets(items, row, k, ch, i+1);
			row.pop_back();
		}
	}
}

//GENERATES UNIQUE TRANSACTION IDS
//ALL PREVIOUS TIDS ARE STORED IN A SET
//TO MAINTAIN UNQUNESS
int findTransaction(set<int> &s, int n){
	int trans = 1+rand()%n;
	while(s.find(trans) != s.end()){
		trans = 1+rand()%n;
	}
	s.insert(trans);
	return trans;
}

//GENERATES UNIQUE ITEMS PER TRANSACTION
//STORE ALL ITEMS FOR A PARTICULAR TRANSACTION
//IN A SET. IF AN ITEM IS NEW, ADD IT TO SET AND
//RETURN THE ITEM
int findItems(set<int> &s, int n){
	int item = 1+rand()%n;
	while(s.find(item) != s.end()){
		item = 1+rand()%n;
	}
	s.insert(item);
	return item;
}

//CREATES A SAMPLE DATABASE
void createDatabase(fstream &fout, int maxTrans, int maxItems, int numTrans){
	fout.open("dataBase.txt", ios::out);
	set<int> transactions, items;
	for(int i=0; i<numTrans; i++){
		int trans = findTransaction(transactions, maxTrans);
		fout<<trans<<"\t";
		int numItems = 1 + rand()%maxItems;
		for(int j=0; j<numItems; j++){
			int item = findItems(items, maxItems);
			fout<<item<<" ";
		}
		items.clear();
		fout<<-1;
		fout<<"\n";
	}
	fout.close();
}

//TAKES SAMPLE DATABASE AS INPUT AND CONVERTS IT TO L1
void invertDatabase(fstream &fout, fstream &fin, vector<int> &items, int maxTrans, int maxItems, int support){
	fout.open("dataBase.txt", ios::in);
	fin.open(names[0], ios::out);
	vector<int> count;
	int data, tId;
	for(int i=0; i<maxItems; i++) count.push_back(0);
	fout>>tId;
	while(!fout.eof()){
		fout>>data;
		if(data == -1 && !fout.eof()){
			fout>>tId;
			fout>>data;
		}
		if(data != -1) count[data-1]++;
	}
	for(int i=0; i<maxItems; i++){
		if(count[i] >= support){
			fin<<i+1<<"\t"<<count[i]<<" "<<-1<<"\n";
			items.push_back(i+1);
		}
	}
	fin.close();
	fout.close();
}

//UTIL FUNCTION USED TO LOAD ENTIRE DATABASE INTO A VECTOR
//OF SETS TO MAKE ACCESSING ITEM IN A TRANSACTION EASIER
void loadIntoTSet(){
	fout.open("dataBase.txt", ios::in);
	set<int> s;
	int tId, data;
	/** code takes database.txt as input and writes it to array of sets **/
	fout>>tId;
	while(!fout.eof()){
		fout>>data;
		if(data == -1){
			transactions.push_back(s);
			s.clear();
			fout>>tId;
		}
		else s.insert(data);
	}
	/** end of code **/
	fout.close();
}

void loadIntoSets(int k){
	fstream f;
	f.open(names[k-1], ios::in);
	int tId, i=0, flag=0;
	set<int> temp;
	while(!f.eof()){
		i=0;
		while(i < k){
			f>>tId;	
			if(f.eof()){
				flag=1;
				break;
			}
			temp.insert(tId);
			++i;
		}
		if(flag) break;
		f>>tId;
		suppCount.push_back(tId);
		while(tId != -1){
			f>>tId;
		}
		tId = 0;
		if(tId != -1){
			sets.insert(temp);
			temp.clear();
		}
	}
	f.close();
}

void generateAssocRules(){
	for(int p=2; p<=curK; p++){
		loadIntoSets(p);
		int conf = 0, flag;
		vector<int> its, row;
		set<set<int> > :: iterator i = sets.begin();
		for(; i != sets.end(); i++){
			set<int> a = *i;
			set<int> :: iterator j = a.begin();
			its.clear();
			for(; j != a.end(); j++){
				its.push_back(*j);
			}
			generateSubsets(its, row, p, flag, 2);
		}
		clear();
	}
}

/**************************** END OF FUNCTIONS *******************************/

int main(){
	vector<int> items, row;
	//createDatabase(fout, maxTrans, maxItems, numTrans);
	cout<<"Minimum support is : "<<support;
	cout<<"\nDatabase created successfully!";
	invertDatabase(fout, fin, items, maxTrans, maxItems, support);
	cout<<"\nDatabase inverted successfully!";
	loadIntoTSet();
	for(int i=2; i<=maxK; i++){ // for brute force frequent item set generation
		generateFsets(items, row, i, 1);
	}
	loadIntoSets(maxK-1);
	generateAssocRules();
	cout<<"\nFrequent item sets generated successfully!";
	return 0;
}
