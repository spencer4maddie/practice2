#include<iostream> 
#include<fstream>   
#include<cstring>  
#include<sstream> 
#include<map>
#include<vector>
#include<cmath>
using namespace std;  
double TFmatrix[1300][2800];
bool OHmatrix[1300][2800];
double IDFmatrix[1300][2800];
int emo_index= 7;
int vol_num=0;
int sen_num=0; 
const int max_vol=2800;
const int max_sen=1300;
string volcab[max_vol];
double IDF[max_vol];
map<string,int> vol_base; 
vector <string> dataset[max_sen];

struct entity
{
	int row;
	int col;
	int app;
};
vector <entity> smatrix;
void createVOL()
{
	ifstream fin("C:\\Users\\sagni\\Desktop\\大三上选课\\人工智能\\实验课\\lab1 数据集处理\\semeval.txt");
	if(!fin)
	{
		cout<<"Erro while open file"<<endl;
		return;
	}
	else
		cout<<"Open succeed !"<<endl;
	
	int num; 
	string emo_chunck,sentence,vol;
	stringstream str;
	while(fin>>num)//数字是每一个篇章的分割 
	{
		for(int i=0;i<emo_index;i++)//读取感情文段，并丢掉。 
		{
			fin>>emo_chunck;
			//cout<<emo_chunck<<" ";
		}
		//cout<<endl;
		//fin.get();
		str.clear();//恢复文件流可用 
		getline(fin,sentence);//需要读入的句子存入sentence  
		str << sentence;//把sentence送入流，方便读入 
		while(str >> vol)//进行流读入，不用人为切割 
		{
			cout<<vol<<" ";//用于检测是否有读入 
			dataset[sen_num].push_back(vol);//提取出数据集 
			//创建词汇库表
			//检测这个词是否已经出现过，从前面开始查看。 
			bool flag=true;
			for(int i=0;i<vol_num;i++)
			{
				if(volcab[i]==vol)
				{
					flag=false;
					break;
				}
			} 
			if(flag)
			{ 
				volcab[vol_num++]=vol;
			}		 
		}
		++sen_num;	
		cout<<endl;
		cout<<vol_num<<" & "<<sen_num<<endl;
	}
	fin.close();
	
	//convert_map()，为了加速查找 
	for(int i=0; i<vol_num;i++) 
    { 
        vol_base.insert(make_pair(volcab[i],i)); 
    } 
}

void createOneHot()
{
	memset(OHmatrix,false,sizeof(OHmatrix));
	string vol;
	for(int row=0;row<sen_num;row++)//遍历文本 
	{
		for(int j=0;j<dataset[row].size();j++)//遍历文本中的每个单词 
		{
			vol=dataset[row][j];
			int index=vol_base.find(vol)->second;//找到其在词汇库中对应的下标 
			OHmatrix[row][index]=true;//在矩阵中记录 
		}
	}
	/*	ofstream fout("C:\\Users\\sagni\\Desktop\\大三上选课\\人工智能\\实验课\\lab1 数据集处理\\onehot.txt");
	for(int i=0;i<sen_num;i++)
	{
		for(int j=0;j<vol_num;j++)
		{
			fout<<OHmatrix[i][j]<<" ";
		}
		fout<<endl;
	}*/	
}
void createTF()
{
	memset(TFmatrix,0,sizeof(TFmatrix));
	for(int row=0;row<sen_num;row++)
	{
		for(int j=0;j<dataset[row].size();j++)//每个单词进行累计 
		{
			string vol=dataset[row][j];
			int index=vol_base.find(vol)->second;//找到其对应词汇表中的下标 
			TFmatrix[row][index]++;
		}
		for(int j=0;j<vol_num;j++)//计算出现概率 
		{
			TFmatrix[row][j]/=dataset[row].size(); 
		}
	}
	/*ofstream fout("C:\\Users\\sagni\\Desktop\\大三上选课\\人工智能\\实验课\\lab1 数据集处理\\TF.txt");
		for(int i=0;i<sen_num;i++)
	{
		for(int j=0;j<vol_num;j++)
		{
			fout<<TFmatrix[i][j]<<" ";
		}
		fout<<endl;
	}	*/	
}
void createIDF()
{
	memset(IDF,0,sizeof(IDF));
	memset(IDFmatrix,0,sizeof(IDFmatrix));
	for(int row=0;row<sen_num;row++)//遍历one-hot矩阵 
	{
		for(int j=0;j<vol_num;j++)
		{
			if(OHmatrix[row][j]==true)//单词出现在文本中 
			IDF[j]++;//累计 
		}	
	}
	for(int j=0;j<vol_num;j++)//利用公式计算IDF 
	{
		IDF[j]=log(sen_num/(1+IDF[j])) / log(2);
		//cout<<IDF[j]<<" ";		
	}
	for(int i=0;i<sen_num;i++)
	{
		for(int j=0;j<vol_num;j++)
		{
			IDFmatrix[i][j] =TFmatrix[i][j] *IDF[j];
			//if(IDFmatrix[i][j]!=0)
			//cout<<IDFmatrix[i][j]<<" ";
		}
		//cout<<endl;
	}
	/*ofstream fout("C:\\Users\\sagni\\Desktop\\大三上选课\\人工智能\\实验课\\lab1 数据集处理\\TFIDF.txt");
	for(int i=0;i<sen_num;i++)
	{
		for(int j=0;j<vol_num;j++)
		{
			fout<<IDFmatrix[i][j]<<" ";
		}
		fout<<endl;
	}*/	
}

void createSmatrix()
{
	entity info={sen_num,vol_num,0};//第一个元素存放整个矩阵的信息，元组数尚不确定，后面补回 
	smatrix.push_back(info);
	int valid=0;
	for(int row=0;row<sen_num;row++)
	{
		for(int col=0;col<vol_num;col++)
		{
			if(OHmatrix[row][col]==true)//检测到有效值的矩阵项，新建一个单元，从后塞入到元组中 
			{
				valid++;//元组数目增加 
				entity e={row,col,1};
				smatrix.push_back(e);	
			}
		}	
	}
	smatrix[0].app=valid;//补回元组数目 
	//cout<<valid<<endl;
	/*ofstream fout("C:\\Users\\sagni\\Desktop\\大三上选课\\人工智能\\实验课\\lab1 数据集处理\\smatrix.txt"); 
	for(int i=0;i<=valid;i++)
		fout<<"["<<smatrix[i].row<<","<<smatrix[i].col<<","<<smatrix[i].app<< "]"<<endl;*/
}



int main()
{
	createVOL();
	createOneHot();
	createTF();
	createIDF();
	createSmatrix();
}
