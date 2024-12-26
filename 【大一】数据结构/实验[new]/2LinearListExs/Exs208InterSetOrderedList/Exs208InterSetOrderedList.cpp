// Exs208InterSetOrderedList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//�����⡿A��BΪ���ϣ������������A=A��B��Ҫ��ʱ��������á�
//���㷨������
//���⣺A��BΪ���ϣ�˵���������ж�û���ظ�Ԫ��
//      A=A��B����Ҫ��Ҫ����A��Ŀռ䱣�潻��Ԫ�أ������Ǵ���һ���±�
//��������ָ��ia��ib�ֱ�ָ��A��B��ǰ�����Ԫ�أ�
//����һ��ָ��iָʾ�Ѿ���ȡ�Ľ���Ԫ����A�ı�ͷ�����Ԫ��λ�ã�
//�Ƚ�A��B��ǰԪ�أ�����������������
//(1) A.data[ia]==B.data[ib]����ia��ib�ǽ���Ԫ�أ����ia!=i+1����iaԪ�ظ��Ƶ�i+1,Ȼ��ia++��ib++��i++
//(2) A.data[ia]<B.data[ib]����ǰԪ�طǽ���Ԫ�أ�ֻ���ƶ�ia����ia++
//(3) A.data[ia]>B.data[ib]����ǰԪ�طǽ���Ԫ�أ�ֻ���ƶ�ib����ib++
//�ظ����Ϲ��̣�ֱ��A��B������һ���������
//�޸�A����Ϊi+1
  
#include "stdafx.h"
#include "iostream.h"

#include "..\seqList.h"


void InterSet(seqList &A, seqList &B)
{
	int i=-1;  //Ϊ�������½���Ԫ�ر��Ȳ���һ�£���ʼ��Ϊ-1
	int ia=0, ib=0;  //A��B��ǰԪ�ص������±�
	while(ia<A.listLen && ib<B.listLen)
	{
		if(A.data[ia]==B.data[ib])  //ia��ibָʾ���ǽ���Ԫ��
		{
			if(ia!=i+1)  //iaԪ�ظ��Ƶ�i+1������iaλ�ü�Ŀ��λ�ã����踴��Ԫ��
				A.data[i+1]=A.data[ia];
			i++;
			ia++;
			ib++;
		}
		else if(A.data[ia]<B.data[ib])  //����Ϊ�ǽ���Ԫ�ش���
			ia++;
		else
			ib++;
	}
	A.listLen=i+1;  //����A���ȣ�ʹ���ڽ���Ԫ�ظ�����
}




int main(int argc, char* argv[])
{
	seqList A, B;
	int i=-1, ia=0, ib=0;
	elementType x;

	initialList(&A);  //��ʼ��˳���
	initialList(&B);  //��ʼ��˳���

	cout<<"����˳���A==>����������Ԫ�أ�'-100'��������"<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		A.data[i]=x;
		i++;
		A.listLen++;  //����+1

		cin>>x;
	}
	
	cout<<"����˳���B==>����������Ԫ�أ�'-100'��������"<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		B.data[i]=x;
		i++;
		B.listLen++;  //����+1

		cin>>x;
	}

	//��ӡ��������
	cout<<"��ǰA��Ԫ�أ�";
	for(i=0;i<A.listLen;i++)
		cout<<A.data[i]<<", ";
	cout<<endl;

	cout<<"��ǰB��Ԫ�أ�";
	for(i=0;i<B.listLen;i++)
		cout<<B.data[i]<<", ";
	cout<<endl;

    InterSet(A,B);  //�����󽻼�����

	cout<<"�󽻼���A��Ԫ�أ�";
	for(i=0;i<A.listLen;i++)
		cout<<A.data[i]<<", ";
	cout<<endl;

	return 0;
}

