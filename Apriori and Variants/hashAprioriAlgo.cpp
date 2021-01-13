//APRIORI ALGORITHM
#include<bits/stdc++.h>
using namespace std;

fstream fout, fin;
vector<set<int> > transactions;
struct hashMap{
	int cnt;
	set<set<int> > sts;	
}hashmap[7];
int maxTrans = 500, maxItems = 5 /*20*/, numTrans = 9 /*1 + rand()%maxTrans*/;
int support = 0.3*numTrans, confidence=50, curK=0;
const int maxK=4;
const char names [maxK][100] = {"L1.txt", "L2.txt", "L3.txt", "L4.txt"};
const char acnames [maxK][100] = {"Rules1.txt", "Rules2.txt", "Rules3.txt", "Rules4.txt"};
fstream file[maxK];
set<set<int> > sets;
vector<int> suppCount;

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

int hashfunc(int x, int y){
	return (x*10 + y)%7;
}

//WRITES TO APPROPRIATE FILE
void writeToFile(vector<int> s, int sup, int k){
	fstream f;
	curK = k;
	f.open(names[k-1], ios::out | ios::app);
	for(int i=0; i<s.size(); i++){
		f<<s[i]<<" ";
	}
	f<<"\t"<<sup<<"\t-1\n";
}

//WRITES TO ASSOCIATION RULES TEXT FILE
void writeToAssocFile(vector<int> a, vector<int> b, int conf, int k){
	fstream f;
	if(conf >= confidence){
		f.open(acnames[k-1], ios::out | ios::app);
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

//TRAVERSES THROUGH THE TRANSACTION DATABASE SET TO FIND
//SUPPORT COUNT OF THE GIVEN SUBSET AND RETURNS THE SUPPORT
//COUNT
int getSupport(vector<int> v, int &supp){
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

// LOADS GIVEN CANDIDATE SET OF TRANSACTIONS INTO A SET
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

//GIVEN A SUBSET S, IT CALCULATES ITS SUPPORT COUNT
//BY SCANNING THE SUPPCOUNT VECTOR AND RETURNS THAT VALUE
void calcSupport(set<int> &s, int &sup){
	set<set<int> > :: iterator i = sets.begin();
	set<set<int> > :: iterator k = sets.begin();
	set<int> :: iterator j;
	set<int> :: iterator l;
	int cnt=0, flag=0;
	for(; i != sets.end(); i++){
		flag=0;
		set<int> b = *i;
		j = b.begin();
		l = s.begin();
		while(j != b.end() && l != s.end()){
			if(*l != *j){
				flag = 1;
				break;
			}
			l++;
			j++;
		}
		if(flag == 0){
			while(k != i){
				k++;
				cnt++;
			}
		}
	}
	sup = suppCount[cnt];
}

//GENERATES SUBSETS OF GIVEN FREQUENT ITEM SET AND THEN CHECKS IF EACH SUBSET IS FREQUENT OR NOT
//IF NOT, FLAG BECOMES 1, INDICATING THAT THAT PARTICULAR FREQUENT ITEM SET HAS TO BE
//PRUNED. IT IS ALSO USED FOR GENERATING SUBSET FOR ASSOCIATION RULE GENERATION. 
void generateSubsets(vector<int> items, vector<int> row, int k, int &flag, int ch, int pos=0){
	if(row.size() > 0 && row.size() < k && items.size() - row.size() < k && ch == 1){
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
		float x = getSupport(a, supp);
		supp=0;
		float y = getSupport(items, supp);
		int conf = (y/x)*100;
		writeToAssocFile(a, b, conf, k);
	}
	else if(ch == 2){
		if(row.size() == k){
			int hashval = hashfunc(row[0], row[1]);
			hashmap[hashval].cnt++;
			set<int> s;
			for(int i=0; i<row.size(); i++){
				s.insert(row[i]);
			}
			hashmap[hashval].sts.insert(s);
		}
	}
	for(int i=pos; i<items.size(); i++){
		row.push_back(items[i]);
		generateSubsets(items, row, k, flag, ch, i+1);
		row.pop_back();
	}
}

void generate_1Subsets(vector<int> items, int k, int ch, int &flag){
	int sup=0;
	for(int i=0; i<items.size(); i++){
		set<int> s;
		vector<int> v;
		for(int j=0; j<items.size(); j++){
			if(i != j){
				s.insert(items[j]);
				v.push_back(items[j]);
			}
		}
		calcSupport(s, sup);
		if(sup < support) flag = 1;
	}
}

//UTIL FUNCTION USED TO LOAD ENTIRE DATABASE INTO A VECTOR
//OF SETS TO MAKE ACCESSING ITEM IN A TRANSACTION EASIER
void loadIntoTSet(){
	fout.open("dataBase.txt", ios::in);
	fin.open("L1.txt", ios::out);
	set<int> s;
	int tId, data;
	vector<int> count;
	for(int i=0; i<maxItems; i++) count.push_back(0);
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
			s.insert(data);
			count[data-1]++;
		}
	}
	for(int i=0; i<maxItems; i++){
		if(count[i] >= support) fin<<i+1<<"\t"<<count[i]<<"\t"<<-1<<"\n";
	}
	/** end of code **/
	fin.close();
	fout.close();
}

//GENERATES SUBSETS AND CHECKS IF EACH OF THOSE SUBSETS ARE FREQUENT
//OR NOT. IF NOT IT RETURNS 0 AKA PRUNE THAT FREQUENT ITEM SET.
bool Prune(set<int> s, int k){
	vector<int> v, row;
	set<int> :: iterator i = s.begin();
	int flag=0;
	for(; i != s.end(); i++){
		v.push_back(*i);
	}
	generate_1Subsets(v, k, 1, flag);
	return flag ? false : true;
}

//CHECKS IF FIRST K-1 ARE COMMON
int isCommon(set<int> a, set<int> b){
	int cnt=0;
	set<int> :: iterator i = b.begin();
	set<int> :: iterator j = a.begin();
	while(i != b.end() && j != a.end()){
		if(*i == *j) cnt++;
		else break;
		++i;
		++j;
	}
	return cnt;
}

//UNION OF GIVEN TWO SETS
set<int> Union(set<int> a, set<int> b){
	set<int> :: iterator i = b.begin();
	for( ; i != b.end(); i++){
		a.insert(*i);
	}
	return a;
}

//CLEARS SETS
void clear(){
	sets.clear();
	suppCount.clear();
}

//DRIVER FUNCTION FOR APRIORI ALGORITHM
void generateFSets(int k){
	clear();
	loadIntoSets(k-1);
	set<set<int> > :: iterator a = sets.begin();
	set<set<int> > :: iterator b = sets.begin();
	int sup;
	for(; a != sets.end(); a++){
		set<int> l1 = *a;
		for(b = a; b != sets.end(); b++){
			set<int> l2 = *b;
			if(isCommon(*a, *b) == k-2){
				set<int> c = Union(*a, *b);
				set<int> :: iterator i = c.begin();
				if(Prune(c, k)){
					vector<int> v;
					set<int> :: iterator i = c.begin();
					sup=0;
					for(; i != c.end(); i++){
						v.push_back(*i);
					}
					sup = getSupport(v, sup);
					if(sup >= support) writeToFile(v, sup, k);
				}
			}
		}
	}
}

void hashForKItemSets(int k){
	int flag=0;
	vector<int> temp;
	for(int i=0; i<transactions.size(); i++){
		vector<int> row, buff;
		set<int> :: iterator itr = transactions[i].begin();
		for(; itr != transactions[i].end(); itr++){
			row.push_back(*itr);
		}
		generateSubsets(row, temp, k, flag, 2);
	}
	fstream f;
	f.open("L2.txt", ios::out | ios::app);
	for(int i=0; i<7; i++){
		if(hashmap[i].cnt >= support){
			set<set<int> > :: iterator itr = hashmap[i].sts.begin();
			for(; itr != hashmap[i].sts.end(); itr++){
				set<int> s = *itr;
				set<int> :: iterator j = s.begin();
				for(; j != s.end(); j++){
					f<<*j<<" ";
				}
				f<<"\t"<<hashmap[i].cnt<<"\t"<<"-1\n";
			}
		}
	}
}

//GENERATES ASSOCIATION RULES
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
			generateSubsets(its, row, p, flag, 1);
		}
		clear();
	}
}
/*
	*********************** ITEM SET FILE FORMAT ********************

		ITEM SETS		SUPPORT COUNT		END OF LINE (-1)
		---				---					-1
		---				---					-1
	
	*********************** ASSOCIATION RULE FILE FORMAT ********************
	
		ITEM SET A	 --->	ITEM SET B		CONFIDENCE			END OF LINE (-1)
		---					---				---					-1
		---					---				---					-1
		
*/

int main(){
	//createDatabase(fout, maxTrans, maxItems, numTrans);
	cout<<"Minimum support is : "<<support;
	cout<<"\nDatabase created successfully!";
	loadIntoTSet();
	cout<<"\nDatabase inverted successfully!";
	hashForKItemSets(2);
	for(int i=3; i<=4; i++){
		generateFSets(i);
	}
	cout<<"\nFrequent item sets generated successfully!";
	generateAssocRules();
	cout<<"\nAssociation Rules generated successfully!";
	return 0;
}
