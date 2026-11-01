// Exs208InterSetOrderedList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//【问题】A、B为集合，递增有序，求解A=A∩B，要求时间性能最好。
//【算法分析】
//审题：A、B为集合，说明两个表中都没有重复元素
//      A=A∩B，即要求要利用A表的空间保存交集元素，而不是创建一个新表
//设置两个指针ia、ib分别指向A、B表当前处理的元素；
//设置一个指针i指示已经求取的交集元素在A的表头的最后元素位置；
//比较A、B表当前元素，会出现以下三种情况
//(1) A.data[ia]==B.data[ib]，则ia或ib是交集元素，如果ia!=i+1，将ia元素复制到i+1,然后：ia++，ib++，i++
//(2) A.data[ia]<B.data[ib]，当前元素非交集元素，只需移动ia，即ia++
//(3) A.data[ia]>B.data[ib]，当前元素非交集元素，只需移动ib，即ib++
//重复以上过程，直至A、B中至少一个表结束。
//修改A表长度为i+1
  
#include "stdafx.h"
#include "iostream.h"

#include "..\seqList.h"


void InterSet(seqList &A, seqList &B)
{
	int i=-1;  //为了最后更新交集元素表长度操作一致，初始化为-1
	int ia=0, ib=0;  //A、B表当前元素的数组下标
	while(ia<A.listLen && ib<B.listLen)
	{
		if(A.data[ia]==B.data[ib])  //ia和ib指示的是交集元素
		{
			if(ia!=i+1)  //ia元素复制到i+1，否则ia位置即目标位置，不需复制元素
				A.data[i+1]=A.data[ia];
			i++;
			ia++;
			ib++;
		}
		else if(A.data[ia]<B.data[ib])  //以下为非交集元素处理
			ia++;
		else
			ib++;
	}
	A.listLen=i+1;  //更新A表长度，使等于交集元素个数。
}




int main(int argc, char* argv[])
{
	seqList A, B;
	int i=-1, ia=0, ib=0;
	elementType x;

	initialList(&A);  //初始化顺序表
	initialList(&B);  //初始化顺序表

	cout<<"创建顺序表A==>请输入数据元素（'-100'结束）："<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		A.data[i]=x;
		i++;
		A.listLen++;  //长度+1

		cin>>x;
	}
	
	cout<<"创建顺序表B==>请输入数据元素（'-100'结束）："<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		B.data[i]=x;
		i++;
		B.listLen++;  //长度+1

		cin>>x;
	}

	//打印表中数据
	cout<<"当前A表元素：";
	for(i=0;i<A.listLen;i++)
		cout<<A.data[i]<<", ";
	cout<<endl;

	cout<<"当前B表元素：";
	for(i=0;i<B.listLen;i++)
		cout<<B.data[i]<<", ";
	cout<<endl;

    InterSet(A,B);  //调用求交集函数

	cout<<"求交集后A表元素：";
	for(i=0;i<A.listLen;i++)
		cout<<A.data[i]<<", ";
	cout<<endl;

	return 0;
}

