// Exs209SetSubtraction.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "iostream.h"

#include "..\seqList.h"

void SetSubtraction(seqList & A, seqList & B)
{
	int i=-1;  //ָʾA���Ѿ���������Ԫ��
	int ia=0,ib=0;  //ָʾA��B�У���ǰ�������Ԫ�أ���ʼָ���һ��Ԫ��
	while(ia<A.listLen && ib<B.listLen)
	{
		if(A.data[ia]==B.data[ib])  //��A-B��Ԫ�أ�ia��ibͬʱ����
		{
			ia++;
			ib++;
		}
		else if(A.data[ia]>B.data[ib])  //��ʱ��iaָʾԪ�ؿ�����B��ibָʾ��Ԫ�غ��棬�ƶ�ib
			ib++;
		else  //��Ϊ��A.data[ia]<B.data[ib]����Ϊ�����ԣ�iaָʾ��Ԫ�ز�������B�С�����iaָʾԪ����A-B�С�
		{   //���(i+1)==ia��˵��iaԪ�ز���Ǩ��λ�ã�ֱ��ΪA-B��Ԫ��
			//iaָʾԪ�أ����Ƶ�Ŀ��λ��
			if(i+1!=ia)
				A.data[i+1]=A.data[ia];
			
			i++;  //A-B�������Ԫ��ָʾ������
			ia++; //A��ָʾ������
		}
	}

	while(ia<A.listLen)  //��ʱB�Ѿ�ɨ���������A��δ������A��ʣ�²���Ԫ��ȫ����A-B��
	{
		if(i+1!=ia)
			A.data[i+1]=A.data[ia];
			
		i++;  //A-B�������Ԫ��ָʾ������
		ia++; //A��ָʾ������
	}
	A.listLen=i+1;  //���±�A�ĳ��ȣ�ʹ����|A-B|

}

//��A-B
void SetSubtraction1(seqList & A, seqList & B)
{
	int ia=0, ib=0;  //ia��ibָʾA��B���е�ǰԪ�ء���ʼָ���һ��Ԫ�ء�
	while(ia<A.listLen && ib<B.listLen)
	{
		if(A.data[ia]<B.data[ib])  //Ԫ�ز���B�У��ƶ�A����һ��Ԫ��
			ia++;
		else if(A.data[ia]>B.data[ib])  //AԪ�ؿ�����B�У��ƶ�B����һ��Ԫ��
		{
			ib++;
			if(ib>=B.listLen)
				break;
		}
		else  //A.data[ia]==B.data[ib]��ɾ��A��Ԫ�أ�ͬʱ�ƶ�ָ��
		{
			listDelete(&A, ia+1);
			//ia++;  ��������ia����ΪA�Ѿ���ǰ�ƶ�һ��Ԫ��
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

	cout<<"����˳���A��"<<endl;
	listInputC(&A);
	cout<<"����˳���B��"<<endl;
	listInputC(&B);

	cout<<"A=";
	for(i=0; i<A.listLen; i++)
		cout<<A.data[i]<<", ";
	cout<<endl;
	cout<<"B=";
	for(i=0; i<B.listLen; i++)
		cout<<B.data[i]<<", ";
	cout<<endl;


	//��A-B
	SetSubtraction(A,B);
	//SetSubtraction1(A,B);

	cout<<"A-B=";
	for(i=0; i<A.listLen; i++)
		cout<<A.data[i]<<", ";
	cout<<endl;


	
	
	return 0;
}
