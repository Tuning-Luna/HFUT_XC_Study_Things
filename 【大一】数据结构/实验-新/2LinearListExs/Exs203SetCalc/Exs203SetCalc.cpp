// Exs203SetCalc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdlib.h"
#include "iostream.h"
#include "..\SeqList.h"

//求C=A∩B
void interSet(seqList A, seqList B, seqList &C)
{
	int i;
	for(i=0;i<A.listLen;i++)
	{
		if(listLocate(B,A.data[i])!=0)   //A.data[i]在B中出现，是交集元素，插入C中
			listInsert(&C,A.data[i],C.listLen+1);	
	}
}
//求C=A∪B
void mergeSet(seqList A, seqList B, seqList &C)
{
	int i;
	for(i=0;i<A.listLen;i++)
	{
		listInsert(&C,A.data[i],C.listLen+1);
	}
   
	for(i=0;i<B.listLen;i++)
	{
		if(listLocate(A,B.data[i])==0)      //检查x是否在A中出现，未出现元素插入C
			listInsert(&C,B.data[i],C.listLen+1);
	}
}
//求C=A-B，在A中，但不在B中的元素
void differenceSet(seqList A,seqList B,seqList &C)
{
	int i;
	for(i=0;i<A.listLen;i++)
	{
		if(listLocate(B,A.data[i])==0)
			listInsert(&C,A.data[i],C.listLen+1);  //A.data[i]不在B中，插入C	
	}
}

//利用线性表创建集合
void  createSet(seqList &L)
{
	
	elementType x;
	
	initialList(&L);  //为能重复创建，先清楚表中原有元素
	cout<<"请输入数据元素(整数，-9999退出):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999 && listLength(L)<MAXLEN)
	{
		if(listLength(L)==MAXLEN)     //顺序表满，退出
		{
			cout<<"Sorry，顺序表已满，不能再将元素入队！"<<endl;
			break;
		}
		
		if(listLocate(L,x)==0)  //元素x不重复，插入
			listInsert(&L,x,L.listLen+1);  //调用插入函数，注意插入位置i
		else
			cout<<"当前输入数字 x="<<x<<" 重复，请重新选择输入！"<<endl;

		cout<<"x=";
		cin>>x;
	}
}

int main(int argc, char* argv[])
{
	seqList A,B,C;

	initialList(&A);
	initialList(&B);
	
	
	cout<<"创建集合A。";
	createSet(A);
	cout<<"创建集合B。";
	createSet(B);

	
	cout<<"集合A元素：";
	printList(A);
	cout<<"集合B元素：";
	printList(B);
	     //求交集
	initialList(&C);
	interSet(A,B,C);
	cout<<"交集C元素：";
	printList(C);
	     //求并集	
	initialList(&C);
	mergeSet(A,B,C);	
	cout<<"并集C元素：";
	printList(C);
	     //求差集
	initialList(&C);
	differenceSet(A,B,C);
	cout<<"差集C元素：";
	printList(C);

	return 0;
}
