// Exs201LinearListLast.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include "iostream.h"  //C++ͷ�ļ�

#include "stdlib.h"
#include "time.h"


#include "..\linearListLast.h"

int main(int argc, char* argv[])
{
	seqList L;
	int i,m;
	elementType x;

	L.last=-1;

	int nChoice=-1; //����ѡ��
	do
	{
		//��ʾ���˵�
		cout<<"**********˳�����Գ���***********"<<endl;
		cout<<"* 0-�˳�                          *"<<endl;
		cout<<"* 1-��������˳���                *"<<endl;
		cout<<"* 2-���������˳���              *"<<endl;
		cout<<"* 3-�ļ��������ݱ�                *"<<endl;
		cout<<"* 4-��ӡ˳���                    *"<<endl;
		cout<<"*---------------------------------*"<<endl;
		cout<<"* 5-��ʼ��˳���                  *"<<endl;
		cout<<"* 6-�����                      *"<<endl;
		cout<<"* 7-�������Ԫ��                  *"<<endl;
		cout<<"* 8-���Ҹ���Ԫ��                  *"<<endl;
		cout<<"* 9-����Ԫ��                      *"<<endl;
		cout<<"* 10-ɾ��Ԫ��                     *"<<endl;
		cout<<"***********************************"<<endl;

		cout<<"����������ѡ�������";
		cin>>nChoice;

		switch(nChoice)
		{
		case 0:  //�˳�
			cout<<"��ǰѡ��������˳���"<<endl;
			cout<<"<-- �����˳� -->"<<endl;  //ѡ���˳�
			break;

		case 1:  //��������˳���--����2
			system("cls");  //�����Ļ

			if(L.last!=-1)
			{
				cout<<"��Lδ��ʼ�������Ѿ����ڡ����ȳ�ʼ����������Ԫ�ء�"<<endl;
				break;
			}
			
			listInputC(L);  //��������������
			                 //listInputArr(&L);
			                 //listInputC1(&L);  //Ԫ������������
			cout<<"��ǰ���ȣ�"<<L.last+1<<endl;
			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);
			break;
			
		case 2:  //���������˳���
			system("cls");  //�����Ļ

			if(L.last!=-1)
			{
				cout<<"��Lδ��ʼ�������Ѿ����ڡ����ȳ�ʼ����������Ԫ�ء�"<<endl;
				break;
			}
			
			rndCList(L);   //���������˳���

			cout<<"��ǰ���ȣ�"<<L.last+1<<endl;
			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);
			break;		
		case 4:  //��ӡ��Ԫ��
			system("cls");  //�����Ļ
			cout<<"��ǰ���ȣ�"<<L.last+1<<endl;
			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);
			break;		
		case 5:  //��ʼ��˳���
			system("cls");  //�����Ļ
			initialList(L);
			cout<<"˳���L��ʼ����ɡ�"<<endl;
			break;
/*		case 11:  //���ı��ļ������Ԫ������
			system("cls");  //�����Ļ
			if(L.last!=-1)
			{
				cout<<"��Lδ��ʼ�������Ѿ����ڡ����ȳ�ʼ����������Ԫ�ء�"<<endl;
				break;
			}			
			
			char strFileName[100];
			cout<<"������˳��������ļ���(seqList.seq)��";
			cin>>strFileName;  //����̨���������ļ���			

			listInputFile(strFileName,&L);  //���ļ���������Ԫ��
			cout<<"��ǰ���ȣ�"<<L.listLen<<endl;
			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);

			break;			
*/			
		case 6:  //�����
			system("cls");  //�����Ļ
			cout<<"����Ϊ��"<<listLength(L)<<endl;
			break;
		case 7:
			system("cls");  //�����Ļ
			cout<<"������Ԫ����ţ�1-n��i=";
			cin>>i;
			
			if(getElement(L, i, x))
				cout<<"L�е�"<<i<<"��Ԫ���ǣ�"<<x<<endl;
			else
				cout<<"�����ȡԪ��ʧ�ܡ�"<<endl;
			break;
		case 8:  //����Ԫ�أ�����λ��
            system("cls");  //�����Ļ
			cout<<"������Ҫ���ҵ�Ԫ��x=";
			cin>>x;

			i=listLocate(L,x); //���ò��Һ���

			if(i==0)
				cout<<"���ҵ�Ԫ��x="<<x<<"���ڱ�L�С�"<<endl;
			else
				cout<<"���ҵ�Ԫ��x="<<x<<"�ڱ�L�е�λ�ã�"<<i<<endl;

			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);
			break;
		case 9:  //����Ԫ��
			system("cls");  //�����Ļ
			cout<<"������Ҫ�����Ԫ��x=";
			cin>>x;
			cout<<"������Ҫ�����λ��i=";
			cin>>i;

			cout<<"ԭ�����ȣ�"<<L.last+1<<endl;
			cout<<"ԭ������Ԫ�أ�"<<endl;
			printList(L);

			m=listInsert(L,x,i); //���ò��뺯��
			if(m==2)
				cout<<"��ϲ����Ԫ��x="<<x<<"����ɹ���"<<endl;
			else if(m==1)
				cout<<"����λ�ó�����Χ��Ԫ�ز���ʧ�ܣ�"<<endl;
			else
				cout<<"��ռ����������ܲ����µ�Ԫ�ء�"<<endl;
			cout<<"��ǰ���ȣ�"<<L.last+1<<endl;
			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);
			break;
		case 10:  //ɾ��Ԫ��
			system("cls");  //�����Ļ
			cout<<"������ɾ��Ԫ�ص�λ��i=";
			cin>>i;

			cout<<"ԭ�����ȣ�"<<L.last+1<<endl;
			cout<<"ԭ������Ԫ�أ�"<<endl;
			printList(L);
			
			m=listDelete(L,i); //����ɾ������

			if(m==2)
				cout<<"��ϲ����ɾ��Ԫ�سɹ���"<<endl;
			else if(m==1)
				cout<<"ɾ��λ�ó�����Χ��ɾ��Ԫ��ʧ�ܣ�"<<endl;
			else
				cout<<"�ձ�û��Ԫ�ؿɹ�ɾ����"<<endl;
			cout<<"��ǰ���ȣ�"<<L.last+1<<endl;
			cout<<"��ǰ����Ԫ�أ�"<<endl;
			printList(L);

			break;

		default:
			cout<<"����ѡ���������0��10֮��ѡ��==>"<<endl;
			break;
        }
	
	}while(nChoice!=0);
	
	return 0;
}
