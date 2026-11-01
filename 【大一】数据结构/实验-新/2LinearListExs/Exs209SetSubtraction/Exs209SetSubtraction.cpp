// Exs209SetSubtraction.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "iostream.h"

#include "..\seqList.h"

void SetSubtraction(seqList & A, seqList & B)
{
	int i=-1;  //指示A中已经处理的最后元素
	int ia=0,ib=0;  //指示A、B中，当前待处理的元素，初始指向第一个元素
	while(ia<A.listLen && ib<B.listLen)
	{
		if(A.data[ia]==B.data[ib])  //非A-B中元素，ia、ib同时后移
		{
			ia++;
			ib++;
		}
		else if(A.data[ia]>B.data[ib])  //此时，ia指示元素可能在B中ib指示的元素后面，移动ib
			ib++;
		else  //此为，A.data[ia]<B.data[ib]，因为递增性，ia指示的元素不可能在B中。所以ia指示元素在A-B中。
		{   //如果(i+1)==ia，说明ia元素不需迁移位置，直接为A-B中元素
			//ia指示元素，复制到目标位置
			if(i+1!=ia)
				A.data[i+1]=A.data[ia];
			
			i++;  //A-B集合最后元素指示器后移
			ia++; //A的指示器后移
		}
	}

	while(ia<A.listLen)  //此时B已经扫描结束，但A尚未结束，A中剩下部分元素全部在A-B中
	{
		if(i+1!=ia)
			A.data[i+1]=A.data[ia];
			
		i++;  //A-B集合最后元素指示器后移
		ia++; //A的指示器后移
	}
	A.listLen=i+1;  //更新表A的长度，使等于|A-B|

}

//求A-B
void SetSubtraction1(seqList & A, seqList & B)
{
	int ia=0, ib=0;  //ia、ib指示A、B表中当前元素。初始指向第一个元素。
	while(ia<A.listLen && ib<B.listLen)
	{
		if(A.data[ia]<B.data[ib])  //元素不在B中，移动A到下一个元素
			ia++;
		else if(A.data[ia]>B.data[ib])  //A元素可能在B中，移动B到下一个元素
		{
			ib++;
			if(ib>=B.listLen)
				break;
		}
		else  //A.data[ia]==B.data[ib]，删除A中元素，同时移动指针
		{
			listDelete(&A, ia+1);
			//ia++;  不能增加ia，因为A已经往前移动一个元素
			ib++;
			if(ib>=B.listLen)
				break;
		}		
	}
}

int main(int argc, char* argv[])
{
	seqList A, B;
	int i;

	initialList(&A);
	initialList(&B);

	cout<<"创建顺序表A："<<endl;
	listInputC(&A);
	cout<<"创建顺序表B："<<endl;
	listInputC(&B);

	cout<<"A=";
	for(i=0; i<A.listLen; i++)
		cout<<A.data[i]<<", ";
	cout<<endl;
	cout<<"B=";
	for(i=0; i<B.listLen; i++)
		cout<<B.data[i]<<", ";
	cout<<endl;


	//求A-B
	SetSubtraction(A,B);
	//SetSubtraction1(A,B);

	cout<<"A-B=";
	for(i=0; i<A.listLen; i++)
		cout<<A.data[i]<<", ";
	cout<<endl;


	
	
	return 0;
}
