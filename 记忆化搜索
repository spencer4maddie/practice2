#include<iostream>
#include<fstream>
#include<ostream>
#include<sstream>
#include<cstring>
#include<vector>
#include<map>
using namespace std;
vector<string> substring;
vector<int> weight;
string s;
struct StringInfo
{
	int max;
	int pos;
	string spell;
	string left_string;
} ;
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
map<string,StringInfo> visited;
int search_best(string current_string)
{
	int max = 0;
	int pos = 0;
	string spell = "";
	string left_string = "";
	for(int index=0;index<s.size();index++)
	{
		for(int i=0;i<substring.size();i++)
		{ 
			if(substring[i][0]==s[index])
			{
				int start_pos = current_string.find(substring[i]);
				if(start_pos==index)
				{
					string try_cut_s = s.substr(0,start_pos)+ current_string.substr(start_pos+substring[i].size()); 
					int temp_sum;
					if(visited.find(try_cut_s)!=visited.end())
					{
						temp_sum = weight[i] + visited[try_cut_s].max; 
					}
					else
						temp_sum = weight[i] + search_best(try_cut_s);
					
					if (temp_sum>max)
					{
						pos = index;
						spell = substring[i];
						left_string = try_cut_s; 
						max = temp_sum;
					}
						
				}
			}		
		}
	}
	StringInfo string_info;
	string_info.max =max; string_info.left_string = left_string; 
	string_info.pos = pos;string_info.spell =spell;
	visited.insert(make_pair(current_string,string_info));
	return max;
}
void print_out_result(char a[])
{
	int result = search_best(s);
	ofstream fout(a);
	string current_substring = s;
	while(current_substring != visited[current_substring].left_string)
	{
		fout<<visited[current_substring].pos <<" "<<visited[current_substring].spell<<endl;	
		current_substring = visited[current_substring].left_string;
	}
	fout<<result<<endl;
}
int main(int argc,char *argv[])
{
	//argv[1] = "test2.txt";
	//argv[2] = "result.txt";
	read_in_data(argv[1]);
	print_out_result(argv[2]);
}
