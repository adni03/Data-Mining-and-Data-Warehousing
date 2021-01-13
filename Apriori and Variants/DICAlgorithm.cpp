// DYNAMIC ITEMSET COUNTING
#include<bits/stdc++.h>
using namespace std;

fstream fout, fin;
vector<set<int> > transactions;
int maxTrans = 500, maxItems = 5 /*20*/, numTrans = 9 /*1 + rand()%maxTrans*/;
int support = 0.3*numTrans, confidence=50, M=3;
const int maxK=4;
const char names [maxK][100] = {"L1.txt", "L2.txt", "L3.txt", "L4.txt"};
const char acnames [maxK][100] = {"Rules1.txt", "Rules2.txt", "Rules3.txt", "Rules4.txt"};
set<set<int> > sets;
vector<int> suppCount;
struct Itemset
{
	vector<int> d;
	int stop_no;
	int support_count;
};
vector<Itemset> SS, SC, DS, DC, EDC;
vector<int> I;

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

/*psuedo Algo : 
SS : NULL : solid square : definitely frequent
SC : NULL : solid circle : definitely infrequent
/* Dynamic Itemset Counting modification of Apriori method */
/*
DS : NULL : dashed square : suspected frequent
DC : All 1- itemsets : dashed cicle : suspected infrequent

while(DS != 0 || DC != 0) do begin
	read M transactions from DB into T
	for all transaction t belonging to T, do begin
	//inc the resp. count of item sets marked dash
		for each itemset c in DS or DC, do begin
			if(c belongs to T) then
				c.counter++;
		for each itemset c in DC
			if(c.counter >= threshold) then
				move c from DC to DS
				if(any immediate superset sc of c has all its subsets in SS or DS) then
					add a new itemset sc in DC;
		end;
		for each itemset c in DS
			if(c has been counted through all transactions) then
				move it into SS;
		for each itemset c in DC
			if(c has been counted through all transactions) then
				move it into SC;
	end;
end;

Final answer : C belonging to SS
*/

//CONVERTS SET TO VECTOR
vector<int> convertSetToVector(set<int> s){
	set<int> :: iterator i = s.begin();
	vector<int> temp;
	for(; i != s.end(); i++){
		temp.push_back(*i);
	}
	return temp;
}

//prints the mentioned vector
void print(vector<int> d){
	for(int i = 0 ; i < d.size() ; i++)
	{
		cout << d[i] << " " ;
	}
}

//to check whether T contains C or not
bool isItemExist(vector<int> C, vector<int> T){
	for(int i = 0 ; i < C.size() ; i++){
		bool flag = false;
		for(int j = 0 ; j < T.size() ; j++){
			if(T[j] == C[i]){
				flag = true;
				break;
			}
		}
		if(!flag){
			return false;
		}
	}
	return true;
}

void loadIntoDC(){
    for(int it = 0; it < I.size(); it++){
    	Itemset temp;
        temp.d.push_back(I[it]);
        temp.stop_no = 0;
        temp.support_count = 0;
        DC.push_back(temp);
    }
    cout << "1 - frequent itemsets : " ;
	print(I);
	cout << "\n"; 
}

//checks if itemset d is already in Cd or not
bool has(vector<int> d, vector<int> Cd){
	if(d.size() == Cd.size()){
		for(int i = 0 ; i < d.size() ; i++){
			if(d[i] != Cd[i])
				return false;
		}
		return true;
	}
	return false;
}

//checks if itemset d is there in mentioned Container
bool contains(vector<int> d, vector<Itemset> Container){
	for(int i = 0 ; i < Container.size() ; i++){
		if( has(d, Container[i].d) ){
			return true;
		}
	}
	return false;
}

//add itemset d, to mentioned container
void add(Itemset d, vector<Itemset> &Container){
	if( !contains(d.d, Container) ) Container.push_back(d);
}

//get index of desired itemset in the container
int getIndexOfd(Itemset d, vector<Itemset> Container){
	for(int i = 0 ; i < Container.size() ; i++){
		bool flag = true;
		for(int j = 0 ; j < Container[i].d.size() ; j++){
			if(Container[i].d[j] != d.d[j]){
				flag = false;
			}
			if(!flag) break;
		}
		if(flag) return i;
	}
	return -1;
}

//remove itemset d from mentioned container
void remove(Itemset d, vector<Itemset> &Container){
	int index = getIndexOfd(d, Container);
	if(index == -1){
		cout<<"was not able to remove d from container, exiting abruptly !"<<"\n";
		exit(0);
	}
	Container.erase(Container.begin()+index);
}

//checks whether particular item is there or not in itemset d
bool itemInD(int j, vector<int> d){
	for(int i = 0 ; i < d.size() ; i++){
		if(d[i] == j){
			return true;
		}
	}
	return false;
}

//checks is k-1 length subsets are present in SS or DS
bool checkInSq(vector<int> v, int ch){
	if(ch == 1){
		for(int k=0; k<SS.size(); k++){
			if(SS[k].d == v) return true;
		}
	}
	else{
		for(int k=0; k<DS.size(); k++){
			if(DS[k].d == v) return true;
		}
	}
	return false;
}

//utility function to generate k-1 length subset and then 
//check existence in DS or SS
bool checkSupport(vector<int> v){
	int flag=0;
	for(int i=0; i<v.size(); i++){
		set<int> s;
		vector<int> o;
		for(int j=0; j<v.size(); j++){
			if(i != j){
				s.insert(v[j]);
				o.push_back(v[j]);
			}
		}
		if(checkInSq(o, 1) || checkInSq(o, 2)) flag = 1;
		else flag = 0;
	}
	return flag;
}

//to get all the immediate supersets of given itemset d
vector<Itemset> immediateSuperset(vector<int> d){
	vector<Itemset> E;
	for(int i = 0 ; i < I.size() ; i++){
		if( !itemInD(I[i], d) ){
			Itemset temp;
			vector<int> temp2 = d;
			temp2.push_back(I[i]);
			sort(temp2.begin(), temp2.end());
			temp.d = temp2;
			if(checkSupport(temp2)) E.push_back(temp);
		}
	}
	return E;
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
		if(count[i] >= support){
			I.push_back(i+1);
		}
	}
	/** end of code **/
	fin.close();
	fout.close();
}

void mainDICFunc(){
	int block_count = 0;
    int current_stop_no = 0;
	while(DC.size() != 0){
		if(block_count == transactions.size()/M ){
			block_count = 0;
		}
		for(int j = 0 ; j < M ; j++){
			/* pick up a transaction from T */
			vector<int> t = convertSetToVector(transactions[j + block_count*M]);
			cout << "current transaction t : " ;
			print(t);
			cout << "\n" ;
		
			/**************** step 1 ********************/
			for(int i = 0 ; i < DS.size() ; i++){
				DS[i].stop_no++;
				if( isItemExist(DS[i].d, t) ){
					DS[i].support_count++;
				}
			}
			for(int i = 0 ; i < DC.size() ; i++){
				DC[i].stop_no++;
				if( isItemExist(DC[i].d, t) ){
					DC[i].support_count++;
				}
			}
			
			/************** End of step 1 ****************/
			
			/************** start of step 2 **************/
			for(int i = 0 ; i < DC.size() ; i++){
				/* printing d and its support count */
				print(DC[i].d);
				cout << "->" << DC[i].support_count << "\t" <<DC[i].stop_no <<"\n";
				if( DC[i].support_count >= support ){
					/* printing E */
					cout << "printing E i.e. immediate superset of : " ;
					print(DC[i].d);
					cout << "->" << DC[i].support_count << "\n";
					add(DC[i], DS);
					vector<Itemset> E = immediateSuperset(DC[i].d); /* I contains all 1 - itemsets */
					for(int m = 0 ; m < E.size() ; m++){
						print(E[m].d);
						cout << "\n" ;
					}
					cout<<"done\n";
					for(int l = 0 ; l < E.size() ; l++){
						E[l].stop_no = 0;
						E[l].support_count = 0;
						add(E[l], EDC);
					}
					remove(DC[i], DC);
					i--;
				}
			}
			/* adding EDC content to DC */
			for(int i = 0 ; i < EDC.size() ; i++){
				add(EDC[i], DC);
			}
			EDC.clear();
			/* for all itemsets d in DS 
					if( stop_no(d) == current_stop_number)
						SS = SS U d
						DS = DS - d */
			for(int i = 0 ; i < DS.size() ; i++){
				if( DS[i].stop_no >= transactions.size() ){
					add(DS[i], SS);
					remove(DS[i], DS);
					i--;
				}
			}
			for(int i = 0 ; i < DC.size() ; i++){
				if( DC[i].stop_no >= transactions.size() ){
					remove(DC[i], DC);
					i--;
				}
			}
		}
		cout << "printing DC : " << "\n" ;
		for(int i = 0 ; i < DC.size() ; i++){
			print(DC[i].d);
			cout << "\n";
		}
		cout << "printing DS : " << "\n" ;
		for(int i = 0 ; i < DS.size() ; i++){
			print(DS[i].d);
			cout << "\n"; 
		}
		cout << "printing SC : " << "\n" ;
		for(int i = 0 ; i < SC.size() ; i++){
			print(SC[i].d);
			cout << "\n";
		}
		cout << "printing SS : " << "\n" ;
		for(int i = 0 ; i < SS.size() ; i++){
			print(SS[i].d);
			cout << "\n";
		}
		block_count++;
	}
	/* print content of SS */
	cout << "\n" ;
	cout << "Content of SS, i.e. frequent itemsets : " << "\n" ;
	for(int i = 0 ; i < SS.size() ; i++){
		print(SS[i].d);
		cout<< " -> " << SS[i].support_count << "\n";
	}
}

void writeToAppFiles(){
	fstream fout;
	for(int i=0; i<SS.size(); i++){
		fout.open(names[SS[i].d.size()-1], ios::out | ios::app);
		for(int j=0; j<SS[i].d.size(); j++){
			fout<<SS[i].d[j]<<" ";
		}
		fout<<"\t"<<SS[i].support_count<<"\t-1\n";
		fout.close();
	}
}

int main(){
	//createDatabase(fout, maxTrans, maxItems, numTrans);
	cout<<"Minimum support is : "<<support;
	cout<<"\nDatabase created successfully!";
	//invertDatabase(fout, fin, maxTrans, maxItems, support);
	cout<<"\nDatabase inverted successfully!";
	loadIntoTSet();
	loadIntoDC();
	cout<<"\nAll frequent 1-itemsets added to DC successfully!";
    mainDICFunc();
    cout<<"\nAll frequent itemsets generated successfully!";
    writeToAppFiles();
    return 0;
}

