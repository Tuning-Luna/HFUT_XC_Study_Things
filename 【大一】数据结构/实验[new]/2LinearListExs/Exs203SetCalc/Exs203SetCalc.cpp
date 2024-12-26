// Exs203SetCalc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdlib.h"
#include "iostream.h"
#include "..\SeqList.h"

//��C=A��B
void interSet(seqList A, seqList B, seqList &C)
{
	int i;
	for(i=0;i<A.listLen;i++)
	{
		if(listLocate(B,A.data[i])!=0)   //A.data[i]��B�г��֣��ǽ���Ԫ�أ�����C��
			listInsert(&C,A.data[i],C.listLen+1);	
	}
}
//��C=A��B
void mergeSet(seqList A, seqList B, seqList &C)
{
	int i;
	for(i=0;i<A.listLen;i++)
	{
		listInsert(&C,A.data[i],C.listLen+1);
	}
   
	for(i=0;i<B.listLen;i++)
	{
		if(listLocate(A,B.data[i])==0)      //���x�Ƿ���A�г��֣�δ����Ԫ�ز���C
			listInsert(&C,B.data[i],C.listLen+1);
	}
}
//��C=A-B����A�У�������B�е�Ԫ��
void differenceSet(seqList A,seqList B,seqList &C)
{
	int i;
	for(i=0;i<A.listLen;i++)
	{
		if(listLocate(B,A.data[i])==0)
			listInsert(&C,A.data[i],C.listLen+1);  //A.data[i]����B�У�����C	
	}
}

//�������Ա�������
void  createSet(seqList &L)
{
	
	elementType x;
	
	initialList(&L);  //Ϊ���ظ����������������ԭ��Ԫ��
	cout<<"����������Ԫ��(������-9999�˳�):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999 && listLength(L)<MAXLEN)
	{
		if(listLength(L)==MAXLEN)     //˳��������˳�
		{
			cout<<"Sorry��˳��������������ٽ�Ԫ����ӣ�"<<endl;
			break;
		}
		
		if(listLocate(L,x)==0)  //Ԫ��x���ظ�������
			listInsert(&L,x,L.listLen+1);  //���ò��뺯����ע�����λ��i
		else
			cout<<"��ǰ�������� x="<<x<<" �ظ���������ѡ�����룡"<<endl;

		cout<<"x=";
		cin>>x;
	}
}

int main(int argc, char* argv[])
{
	seqList A,B,C;

	initialList(&A);
	initialList(&B);
	
	
	cout<<"��������A��";
	createSet(A);
	cout<<"��������B��";
	createSet(B);

	
	cout<<"����AԪ�أ�";
	printList(A);
	cout<<"����BԪ�أ�";
	printList(B);
	     //�󽻼�
	initialList(&C);
	interSet(A,B,C);
	cout<<"����CԪ�أ�";
	printList(C);
	     //�󲢼�	
	initialList(&C);
	mergeSet(A,B,C);	
	cout<<"����CԪ�أ�";
	printList(C);
	     //��
	initialList(&C);
	differenceSet(A,B,C);
	cout<<"�CԪ�أ�";
	printList(C);

	return 0;
}
