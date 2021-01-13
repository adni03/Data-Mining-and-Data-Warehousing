#include<iostream>
using namespace std;
#include<bits/stdc++.h>

// assuming the entries for each attribute arent repeated in other attributes

//union values
//{
//	vector<int> values_i;
//	vector<string> values_s;
//}
int num_tuples,num_attr;
struct classs
{
	string name;
	vector<string> values_s;
};
struct attribute
{
	string name=".";
	vector<int> values_i;
	vector<string> values_s;
	bool isdis;
	bool isthere=1;
};

struct node
{
	bool isleaf=0;
	string class_name;
	string attribute_name;
	int attr_num;
	map<string,node*> children;
};
typedef struct node * BTPTR;


void get_input(attribute atris[99], classs &cl)
{
	fstream fin;
	fin.open("dtree1.txt");
	
	fin>>num_tuples;
	fin>>num_attr;
	string temp;
	fin>>temp; 
	for(int i=0;i<num_attr;i++)
	{
		string str;
		fin>>str;
//		if(i==0)
//		{
//			i--;
//			continue;
//		}
		if(str[0]=='1')
			atris[i].isdis=1;
		else
		{
			atris[i].isdis=0;
		}
			
		str.erase(0,1);
		atris[i].name=str;
	}
	
	string str;
	fin>>str;
	cl.name=str;
	for(int i=0;i<num_tuples;i++)
	{
		string temp;
		fin>>temp;
		for(int j=0;j<num_attr;j++)
		{
			if(atris[j].isdis==1)
			{
				string str;
				fin>>str;
				atris[j].values_s.push_back(str);
				
			}
			else
			{
				
				int intt;
				fin>>intt;
				atris[j].values_i.push_back(intt);
			}
			
		}
		string str;
		fin>>str;
		cl.values_s.push_back(str);
	}
	
}

int get_attr_selected(attribute attr[99],int num,classs cl)
{
	float info_d=0;
	int total=cl.values_s.size();
	map<string,int> mp;
	// use map to first count how many op values there are. store this cu we need it later?
	// after that traverse through map and calc info_d
	for(int i=0;i<cl.values_s.size();i++)
	{
		map<string,int>::iterator itr=mp.find(cl.values_s[i]);
		if(itr==mp.end())
			mp.insert({cl.values_s[i],1});
		else
			itr->second++;
	}
	map<string,int>::iterator itr=mp.begin();
	for(;itr!=mp.end();itr++)
	{
		int a=itr->second;
		int b=total;
		info_d-=( (float)a/(float)b * log2((float)a/(float)b) );
	}
//	cout<<"info_d value is = "<<info_d<<endl;
	int index;
	float gain=0;
	
	
	for(int i=0;i<num;i++) // attr[i] is current attribute being measured
	{
		if(attr[i].isthere==0)
		{
			continue;
		}
			
		map<string,int> mp1;
		// getting all the values of attr[i]
		for(int j=0;j<attr[i].values_s.size();j++)
		{
			map<string,int>::iterator itr=mp1.find(attr[i].values_s[j]);
			if(itr==mp1.end())
			{
				mp1.insert({attr[i].values_s[j],1});
			}
			else
			{
				itr->second++;
			}
		}
		map<string,int>::iterator itr=mp1.begin();
		
		float info_ad=0;
		for(;itr!=mp1.end();itr++) // for each value of the attribute
		{
			map<string,int> v; // contains the number of classes for every value
			for(int j=0;j<attr[i].values_s.size();j++)
			{
				if(itr->first==attr[i].values_s[j])
				{
					map<string,int>::iterator itr3=v.find(cl.values_s[j]);
					if(itr3==v.end())
					{
						v.insert({cl.values_s[j],1});
					}
					else
					{
						itr3->second++;
					}
				}
			}
			// now traverse along v and find info_ad
			map<string,int>::iterator itr4=v.begin();
			for(;itr4!=v.end();itr4++)
			{
				if(i==0)
				cout<<itr->first<<" "<<itr4->first<<" "<<itr4->second<<endl;
				int a=itr4->second;
				int b=itr->second;
				info_ad-=( (float)a/(float)b * log2((float)a/(float)b) );
			}
			info_ad=info_ad * ( (float)itr->second/(float) total );
		}
//		cout<<"info_d-info_ad = "<<info_d-info_ad<<endl;
		if(info_d-info_ad > gain)
		{
			gain=info_d-info_ad;
			index=i;
		}
	}
	
	// for each attribute
		// get all values in a map with freq
		// for each value
			// count number of ops
	
	return index;
}

void build_tree(BTPTR &T,attribute attr[99],int cur_numatr,classs cl)
{
	// check if all class values are same
	// if yes, mark node isleaf as=1 and mark with resp class
	int flag=1;
	for(int i=0;i<cl.values_s.size();i++)
	{
		if(cl.values_s[i]!=cl.values_s[0])
		{
			flag=0;
			break;
		}
	}
	if(flag==1)
	{
		T->isleaf=1;
		T->class_name=cl.values_s[0];
		return;
	}
	
	// check if attr list is empty
	// if it is then find the majority class
	if(cur_numatr==0)
	{
		map<string,int> mp;
		for(int i=0;i<cl.values_s.size();i++)
		{
			map<string,int>::iterator itr=mp.find(cl.values_s[i]);
			if(itr==mp.end())
			{
				mp.insert({cl.values_s[i],1});
			}
			else
			{
				itr->second++;
			}
		}
		map<string,int>::iterator itr=mp.begin();
		map<string,int>::iterator maxx=mp.begin();
		for(;itr!=mp.end();itr++)
		{
			if(itr->second>maxx->second)
				maxx=itr;
		}
		T->isleaf=1;
		T->class_name=maxx->first;
		return;
	}
	
	// get attr with highest info gain
	// make sure the one youre selecting has isthere=1
	int selected=get_attr_selected(attr,cur_numatr,cl);
//	cout<<"_______________"<<selected<<endl;
	
	// mark node with this attr, both name and number
	T->attr_num=selected;
	T->attribute_name=attr[selected].name;
	
	// get all values of the attri in a set
	set<string> s;
	for(int i=0;i<attr[selected].values_s.size();i++)
	{
		s.insert(attr[selected].values_s[i]);
	}
	
	set<string>::iterator itr=s.begin();
	for(;itr!=s.end();itr++) // for each value of selected
	{
		attribute attr_copy[cur_numatr-1];
		int sep_in2=0;
		for(int u=0;u<cur_numatr;u++)
		{
			if(u==selected)
				continue;
			attr_copy[sep_in2++]=attr[u];
		}
		// clear all values in the values_s
		for(int i=0;i<cur_numatr-1;i++)
		{
			attr_copy[i].values_s.clear();
		}
		classs cl_copy;
		cl_copy.name=cl.name;
		
		for(int j=0;j<attr[selected].values_s.size();j++) // match with set
		{
			if(*itr==attr[selected].values_s[j]) // if found
			{
				int sep_in=0;
				for(int k=0;k<cur_numatr;k++) // copy all jth index tuples to copy
				{
					if(k==selected)
						continue;
//					if(attr[k].isthere==1)
					attr_copy[sep_in++].values_s.push_back(attr[k].values_s[j]);
				}
				cl_copy.values_s.push_back(cl.values_s[j]); // copy jth tuple class also
				// copy ALL (including selected) attr values to attr_copy
				// and resp class also
			}
		}
		cout<<endl;
		for(int ii=0;ii<cur_numatr-1;ii++)
		{
			cout<<"->";
			cout<<attr_copy[ii].name<<" ";
		}
		cout<<endl;
		BTPTR T1=new node();
		T->children.insert({*itr,T1});
		attr_copy[selected].isthere=0;
		build_tree(T1,attr_copy,cur_numatr-1,cl_copy);
		// create child node and add the node.children
		// mark attr_copy[selected].isthere=0
		// call build_tree with attr_copy
	}
}




void DLR(BTPTR &T)
{
	if(T->isleaf==1)
	{
		cout<<" . "<<T->class_name<<endl;
	}
	else
	{
		cout<<"->"<<T->attribute_name<<endl;
		map<string,BTPTR>::iterator itr=T->children.begin();
		for(;itr!=T->children.end();itr++)
		{
			DLR(itr->second);
		}
	}
}

void test_ip(BTPTR &T,attribute test[99])
{
	if(T->isleaf==1)
	{
		cout<<"class is = "<<T->class_name;
		return;
	}
	for(int i=0;i<num_attr;i++)
	{
		if(test[i].name==T->attribute_name)
		{
			string go_to_child=test[i].values_s[0];
			map<string,BTPTR>::iterator itr=T->children.find(go_to_child);
			test_ip(itr->second,test);
			return;
		}
	}
}

int main()
{
	attribute atris[99];
	classs cl;
	get_input(atris,cl);
//	for(int i=0;i<num_attr;i++)
//	{
//		cout<<atris[i].name<<endl;
//		for(int j=0;j<atris[i].values_s.size();j++)
//			cout<<atris[i].values_s[j]<<" ";
//		cout<<endl;
//	}
//	cout<<endl<<"----------------------"<<endl;
//	int index=get_attr_selected(atris,4,cl);
//	cout<<"index is = "<<index;
	BTPTR T=new node;
	build_tree(T,atris,num_attr,cl);
	cout<<T->attribute_name;
	cout<<"----";
	cout<<endl<<endl<<endl;
	DLR(T);
	
	attribute test[num_attr];
	test[0].name="age";
	test[1].name="income";
	test[2].name="student";
	test[3].name="credit_rating";
	test[0].values_s.push_back("senior");
	test[1].values_s.push_back("medium");
	test[2].values_s.push_back("no");
	test[3].values_s.push_back("excellent");
	
	cout<<"--------"<<endl;
	test_ip(T,test);
	
	
}



