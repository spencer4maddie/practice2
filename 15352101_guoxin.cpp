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
	ifstream fin("C:\\Users\\sagni\\Desktop\\������ѡ��\\�˹�����\\ʵ���\\lab1 ���ݼ�����\\semeval.txt");
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
	while(fin>>num)//������ÿһ��ƪ�µķָ� 
	{
		for(int i=0;i<emo_index;i++)//��ȡ�����ĶΣ��������� 
		{
			fin>>emo_chunck;
			//cout<<emo_chunck<<" ";
		}
		//cout<<endl;
		//fin.get();
		str.clear();//�ָ��ļ������� 
		getline(fin,sentence);//��Ҫ����ľ��Ӵ���sentence  
		str << sentence;//��sentence��������������� 
		while(str >> vol)//���������룬������Ϊ�и� 
		{
			cout<<vol<<" ";//���ڼ���Ƿ��ж��� 
			dataset[sen_num].push_back(vol);//��ȡ�����ݼ� 
			//�����ʻ���
			//���������Ƿ��Ѿ����ֹ�����ǰ�濪ʼ�鿴�� 
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
	
	//convert_map()��Ϊ�˼��ٲ��� 
	for(int i=0; i<vol_num;i++) 
    { 
        vol_base.insert(make_pair(volcab[i],i)); 
    } 
}

void createOneHot()
{
	memset(OHmatrix,false,sizeof(OHmatrix));
	string vol;
	for(int row=0;row<sen_num;row++)//�����ı� 
	{
		for(int j=0;j<dataset[row].size();j++)//�����ı��е�ÿ������ 
		{
			vol=dataset[row][j];
			int index=vol_base.find(vol)->second;//�ҵ����ڴʻ���ж�Ӧ���±� 
			OHmatrix[row][index]=true;//�ھ����м�¼ 
		}
	}
	/*	ofstream fout("C:\\Users\\sagni\\Desktop\\������ѡ��\\�˹�����\\ʵ���\\lab1 ���ݼ�����\\onehot.txt");
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
		for(int j=0;j<dataset[row].size();j++)//ÿ�����ʽ����ۼ� 
		{
			string vol=dataset[row][j];
			int index=vol_base.find(vol)->second;//�ҵ����Ӧ�ʻ���е��±� 
			TFmatrix[row][index]++;
		}
		for(int j=0;j<vol_num;j++)//������ָ��� 
		{
			TFmatrix[row][j]/=dataset[row].size(); 
		}
	}
	/*ofstream fout("C:\\Users\\sagni\\Desktop\\������ѡ��\\�˹�����\\ʵ���\\lab1 ���ݼ�����\\TF.txt");
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
	for(int row=0;row<sen_num;row++)//����one-hot���� 
	{
		for(int j=0;j<vol_num;j++)
		{
			if(OHmatrix[row][j]==true)//���ʳ������ı��� 
			IDF[j]++;//�ۼ� 
		}	
	}
	for(int j=0;j<vol_num;j++)//���ù�ʽ����IDF 
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
	/*ofstream fout("C:\\Users\\sagni\\Desktop\\������ѡ��\\�˹�����\\ʵ���\\lab1 ���ݼ�����\\TFIDF.txt");
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
	entity info={sen_num,vol_num,0};//��һ��Ԫ�ش�������������Ϣ��Ԫ�����в�ȷ�������油�� 
	smatrix.push_back(info);
	int valid=0;
	for(int row=0;row<sen_num;row++)
	{
		for(int col=0;col<vol_num;col++)
		{
			if(OHmatrix[row][col]==true)//��⵽��Чֵ�ľ�����½�һ����Ԫ���Ӻ����뵽Ԫ���� 
			{
				valid++;//Ԫ����Ŀ���� 
				entity e={row,col,1};
				smatrix.push_back(e);	
			}
		}	
	}
	smatrix[0].app=valid;//����Ԫ����Ŀ 
	//cout<<valid<<endl;
	/*ofstream fout("C:\\Users\\sagni\\Desktop\\������ѡ��\\�˹�����\\ʵ���\\lab1 ���ݼ�����\\smatrix.txt"); 
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
