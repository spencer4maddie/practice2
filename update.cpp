#include<iostream>
#include<fstream>
#include<ostream>
#include<sstream>
#include<cstring>
#include<vector>
#include<map>
using namespace std;

struct TreeNode
{
	TreeNode *parent;
	int pos;
	string spell;
};
struct solution
{
	string spell;
	int position;
};
string s;
vector<string> substring;
vector<int> weight;
TreeNode *best_leaf;
vector<solution> select_sequence;
int max_sum =0;
void read_in_data(char argv[])
{
	ifstream in(argv);
	string info;
	string subset;
	stringstream substr;
	
	 
	getline(in,info);
	//cout<<info<<endl;//
	getline(in,s);
	//cout<<s<<endl;//
	
	
	while(getline(in,subset))
	{
		substr<<subset;
		string substring_item;
		int substring_weight;
		
		substr>> substring_item;
		substr>> substring_weight;
		
		substring.push_back(substring_item);
		weight.push_back(substring_weight);	
		
		substr.clear();		
	}
}
void dfs(TreeNode *parent,string temp_string,int sum,int pos,string spell)
{
	TreeNode *node=new TreeNode;
	node->parent = parent;
	node->pos = pos;
	node->spell = spell;
	
	if( sum>max_sum )
	{
		max_sum = sum;
		best_leaf = node;
	}	
	map<string,bool> visted;
	for (int index=0;index<temp_string.size();index++)
	{
		for(int i=0;i<substring.size();i++)
		{
			if(substring[i][0]==temp_string[index])
			{
				int	start_pos = temp_string.find(substring[i]);
				if(start_pos==index)
				{
				
					string try_cut_s = temp_string.substr(0,start_pos)+ temp_string.substr(start_pos+substring[i].size()); 
					if(visted[try_cut_s]!=true)
					{
						visted.insert(make_pair(try_cut_s,true));
						dfs(node,try_cut_s,sum+weight[i],start_pos,substring[i]);
					}
				}
			}
			
		}
	}
		
}
void traceback(TreeNode *root)
{
	TreeNode *node = best_leaf;
	while(node!=root)
	{
		solution temp;
		temp.position = node->pos;
		temp.spell = node->spell;
		select_sequence.push_back(temp);
		node = node->parent;
	}
	
}
void dfs_start(TreeNode *root)
{
	map<string,bool> visted;
	for(int index=0;index<s.size();index++)
	{
		for(int i=0;i<substring.size();i++)
		{ 
			if(substring[i][0]==s[index])
			{
				int start_pos = s.find(substring[i]);
				if(start_pos==index)
				{
					string try_cut_s = s.substr(0,start_pos)+ s.substr(start_pos+substring[i].size()); 
					if(visted[try_cut_s]!=true)
					{
						visted.insert(make_pair(try_cut_s,true));
						dfs(root,try_cut_s,weight[i],start_pos,substring[i]);
					}
				
				}
			}		
		}
	}
}
void print_out_result(char a[])
{
	TreeNode root;
	dfs_start(&root);
	traceback(&root);
	ofstream fout(a);

	for(int i=select_sequence.size()-1;i>=0;i--)
		fout<<select_sequence[i].position<<" "<<select_sequence[i].spell<<endl;	
	fout<<max_sum<<endl; 
}
int main(int argc,char *argv[])
{
	//cout<<"starting to procced"<<endl;
	read_in_data(argv[1]);
	//cout<<"read finish"<<endl;
	print_out_result(argv[2]);
	//cout<<"print finish"<<endl;
}
