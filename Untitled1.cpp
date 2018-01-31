#include<iostream>
#include<fstream>
#include<ostream>
#include<sstream>
#include<cstring>
#include<vector>
using namespace std;

struct tree_node
{
	tree_node *parent;
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
tree_node *best_leaf;
vector<solution> select_sequence;
int max_sum =0;
void read_in_data()
{
	//ifstream in(argv[0]);
	ifstream in("test3.txt");
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
		
		//cout<<substring_item<<" "<<substring_weight<<endl;//
		substring.push_back(substring_item);
		weight.push_back(substring_weight);	
		
		substr.clear();		
	}
}
void dfs(tree_node *parent,string temp_string,int sum,int pos,string spell)
{
	tree_node *node=new tree_node;
	node->parent = parent;
	node->pos = pos;
	node->spell = spell;
	
	if( sum>max_sum )
	{
		max_sum = sum;
		best_leaf = node;
	}	
//	cout<<"--------------------------------"<<endl;
//	cout<<"此节点的字符串为"<<temp_string<<endl;
//	cout<<"此节点的累计和为"<<sum<<endl;
//	cout<<"此节点的父亲节点为"<<parent->left_string<<endl; 
//	cout<<"此节点的地址为"<<node<<endl;
//	cout<<"--------------------------------"<<endl;
	for (int index=0;index<temp_string.size();index++)
	{
		//cout<<"现在的字符串是"<<temp_string<<endl; 
		for(int i=0;i<substring.size();i++)
		{
			int	start_pos = temp_string.find(substring[i]);
			if(start_pos==index)
			{
				string try_cut_s = temp_string.substr(0,start_pos)+ temp_string.substr(start_pos+substring[i].size()); 
				dfs(node,try_cut_s,sum+weight[i],start_pos,substring[i]);
			}
		}
	}
		
}
void traceback(tree_node *root)
{
	tree_node *node = best_leaf;
	while(node!=root)
	{
		solution temp;
		temp.position = node->pos;
		temp.spell = node->spell;
		select_sequence.push_back(temp);
		//cout<<node->spell<<" "<<node->pos<<endl;
		node = node->parent;
	}
	for(int i=select_sequence.size()-1;i>=0;i--)
		cout<<select_sequence[i].position<<" "<<select_sequence[i].spell<<endl;
}
void dfs_start(tree_node *root)
{
	for(int index=0;index<s.size();index++)
	{
		//cout<<"最外一层的！"<<current_string<<endl; 
		for(int i=0;i<substring.size();i++)
		{ 
			//cout<<substring[i]<<endl;
			int start_pos = s.find(substring[i]);
			if(start_pos==index)
			{
				//cout<<" find "<< substring[i]<<" in"<<start_pos<<endl; 
			
				string try_cut_s = "";
				//要删除的字符串前面+删除的字符串后面的 
				try_cut_s = s.substr(0,start_pos)+ s.substr(start_pos+substring[i].size()); 
				//cout<<try_cut_s<<endl;
				dfs(root,try_cut_s,weight[i],start_pos,substring[i]);//dfs(tree_node *parent,string temp_string,int sum)
			}
		}		
	}
}
void prin_out_result()
{
	tree_node root;
	dfs_start(&root);
	traceback(&root);	
	cout<<max_sum<<endl; 
}
int main(int argc,char *argv[])
{
	
	read_in_data();
	prin_out_result();
	//oftream out(argv[1]);
}
