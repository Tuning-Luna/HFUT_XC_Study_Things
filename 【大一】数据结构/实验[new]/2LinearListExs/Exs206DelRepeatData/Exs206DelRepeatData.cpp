// Exs206DelRepeatData.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "iostream.h"

#include "..\seqList.h"

//删除重复元素
void DeleteRepeatData(seqList & L)
{
	int i,j;
	if(L.listLen==0)
	{
		cout<<"当前顺序表空！"<<endl;
		return;
	}
	if(L.listLen==1)
	{
		cout<<"当前顺序表只有一个元素！"<<endl;
		return;
	}
	i=0;	
	while(i<L.listLen-1)
	{
		for(j=L.listLen-1; j>i; j--)  //从后往前删除，效率较高
		{
			if(L.data[i]==L.data[j])  //元素重复，调用删除
			{
				listDelete(&L,j+1); //调用删除函数，下标差1，所以+1
				//int k;
				//for(k=j; k<L.listLen-1; k++)
				//	L.data[k]=L.data[k+1];
				//L.listLen--;
			}
		}

		i++;
	}

  
}


int main(int argc, char* argv[])
{
	seqList L;
	int i,j;
	elementType x;

	initialList(&L);  //初始化顺序表

	cout<<"创建顺序表==>请输入数据元素（'-100'结束）："<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //长度+1

		cin>>x;
	}
	

	//打印表中数据
	cout<<"当前表元素：";
	for(i=0;i<L.listLen;i++)
		cout<<L.data[i]<<", ";
	cout<<endl;


	DeleteRepeatData(L);  //调用删除重复元素
	
	//打印删除重复元素后的顺序表
	cout<<"删除重复元素后顺序表：";
	for(i=0; i<L.listLen; i++)
		cout<<L.data[i]<<", ";
	cout<<endl;

	return 0;
}

