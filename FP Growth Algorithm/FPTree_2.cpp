#include <bits/stdc++.h>

#define max 20
using namespace std;

int search(int a[], int n, int k){
	for(int i=0; i<n; i++){
		if(a[i] == k) return 1;	
	}
	return 0;	
}

int search(vector<int> a, int k){
	for(int i=0; i<a.size(); i++){
		if(a[i] == k) return i;
	}
	return -1;	
}

int checkEq(vector<int> a, vector<int> b){
	if(a.size() != b.size()) return 0;
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	for(int i=0; i<a.size(); i++){
		if(a[i] != b[i]) return 0;
	}
	return 1;
}

void transactionWrite(){
	srand(time(NULL));
	fstream f("database.txt", ios::out);
	int start = rand() % 100;
	for(int i=0; i<rand() % 200; i++)
	{
		int items[max] = {0}, no = rand() % max + 1, count = 0;
		while(count < no)
		{
			int item = rand() % 100;
			if(search(items, no, item) == 0 && item != 0) 
				items[count++] = item;
		}
		sort(items, items+no);
		f<<'T'<<start<<' ';
		for(int i=0; i<no; i++)
			f<<", "<<items[i]<<' ';
		f<<'|'<<endl;
		start++;
	}
	f.close();
} 

map<int, vector<int> > transactionRead()
{
	fstream f("database.txt", ios::in);
	char c1, c2;
	map<int, vector<int> > m;
	
	while(!f.eof()){
		int tid, num;
		f>>c1>>tid>>c2;
		int item;
		while(c1 != '|'){
			f>>item>>c1;
			m[item].push_back(tid);
		}
	}
	
	for(auto i = m.begin(); i != m.end(); i++){
		cout<<(*i).first<<" : ";
		vector <int> a = (*i).second;
		for(int j=0; j < a.size(); j++){
			cout<<a[j]<<", ";
		}
		cout<<endl;
	}
	f.close();
	return m;
}

struct node{
	int item, count;
	vector <node*> children;
};

typedef node* NTPTR;

int searchNode(vector<NTPTR> children, int a){
	int ind = 0;
	for(auto i = children.begin(); i != children.end(); i++, ind++){
		if((*i)->item == a) return ind;
	}
	return -1;
}

map<int, int> supportCount(map <int , vector<int> > inp, int min_sup = 2){
	map <int, int> a;
	for(auto j = inp.begin(); j != inp.end(); j++){
		for(auto i = (*j).second.begin(); i != (*j).second.end(); i++){
			if(a.find((*i)) == a.end()) a[(*i)] =  1;
			else a[(*i)] = a[(*i)] + 1; //(*j).second.size();
		}
		for(auto i = a.begin(); i != a.end();){
			if((*i).second < min_sup) a.erase(i++);
			else ++i;
		}	
	}
	return a;
}

NTPTR insertNode(NTPTR head, vector<int> a, int count = -1){
	if(head != NULL)
	{
		if(count != -1 && a[count] == head->item)
			head->count += 1;					
	} else {
		NTPTR new_node = new node();
		new_node->count = 1;
		new_node->item = a[count];
		head = new_node;
	}
	
	if(count == a.size() - 1)
		return head;
	
	int cid = searchNode(head->children, a[count + 1]);
	if(cid == -1)
		head->children.push_back(insertNode(NULL, a, count + 1));
	else
		head->children[cid] = insertNode(head->children[cid], a, count + 1);
	
	return head;
}

void print_tree(NTPTR root)
{
	if(root != NULL)
	{
		cout<<root->item<<" : "<<root->count<<endl;
		for(auto i = root->children.begin(); i != root->children.end(); i++)
			print_tree((*i));
	}
}

vector<int> support_sort(vector<int> a, map<int, int> kk)
{
	for(int i = 0; i < a.size(); i++)
		for(int j = 0; j < a.size() - i - 1; j++)
			if(kk[a[j]] < kk[a[j+1]])
			{
				int l = a[j];
				a[j] = a[j+1];
				a[j+1] = l;
			}
	return a;
}

NTPTR create_fp(map<int, vector<int> > D, map<int, int> kk, int min_sup = 2)
{
	NTPTR root = new node();
	root->item = -1;
	root->count = 0;
	for(auto i = D.begin(); i != D.end(); i++){
		vector <int> to_insert;
		for(auto j = (*i).second.begin(); j != (*i).second.end(); j++){
			if(kk.find((*j)) != kk.end() && kk[(*j)] > min_sup) to_insert.push_back((*j));
		}
		to_insert = support_sort(to_insert, kk);
		root = insertNode(root, to_insert);
	}
	return root;
}

vector<map <vector<int>, int> > final;
vector<int> curr;

void getPaths(NTPTR root, int a){
	if(root != NULL)		{
		if(root->item == a){
			map <vector<int>, int> corra;
			corra[curr] = root->count;
			final.push_back(corra);
		}
		else{
			curr.push_back(root->item);
			for(auto i = root->children.begin(); i != root->children.end(); i++){
				getPaths((*i), a);
				curr.pop_back();	
			}
		}
	} 
}

map<int, int> conditionalFP(int min_sup = 2){
	map<int, int> to_return;
	for(auto k = final.begin(); k != final.end(); k++)
	for(auto i = (*k).begin(); i != (*k).end(); i++){
		for(auto j = (*i).first.begin(); j != (*i).first.end(); j++)
			if(to_return.find((*j)) == to_return.end())
				to_return[(*j)] = (*i).second;
			else
				to_return[(*j)] = to_return[(*j)] + (*i).second;
	}
	
	for(auto i = to_return.begin(); i != to_return.end(); )
		if((*i).second < min_sup)
			to_return.erase(i++);
		else
			++i;
	return to_return;
}

void printFP(map<int, int> al, int a){
	vector<int> arr;
	for(auto i = al.begin(); i != al.end(); i++)
		arr.push_back((*i).first);
	
	int n = arr.size();
    for (int i=0; i <n; i++){
        for (int j=i; j<n; j++){
            for (int k=i; k<=j; k++)
                cout << arr[k] << " ";
 			cout<<a;
            cout<<endl;
        }
    }
    cout<<endl;
}

void mineFP(NTPTR root, map<int, int> kk, int min_sup = 2){
	for(auto i = kk.begin(); i != kk.end(); i++){
		final.clear();
		curr.clear();
		if((*i).second > min_sup){
			getPaths(root, (*i).first);
			map<int, int> al = conditionalFP();
			printFP(al, (*i).first);
		}
	}
}

int main(){
	map <int, vector<int> > D = transactionRead();
	map<int, int> kk = supportCount(D);
	NTPTR root = create_fp(D, kk);
	print_tree(root);
	mineFP(root, kk);
}
