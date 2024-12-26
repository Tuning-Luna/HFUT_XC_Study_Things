// OtherLLExs101.cpp : Defines the entry point for the console application.
//
//�Ǳ��̲����⡢ϰ���������Ŀ�Ĳ���

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++ͷ�ļ�

#include "..\LinkedList.h"

#include "..\OtherLLExs101.h"    //��������������

int main(int argc, char* argv[])
{
	node *L, *p;
	int i,m;
	elementType x;

	//initialList(L);
	L=NULL;

	int nChoice=-1; //����ѡ��
	do
	{
		//��ʾ���˵�
		cout<<"******�������������������Գ���*******"<<endl;
		cout<<"* 0-�˳�                              *"<<endl;
		cout<<"* 1-����ͷ�巨��������                *"<<endl;
		cout<<"* 2-����β�巨��������                *"<<endl;
		cout<<"* 3-�������������                    *"<<endl;
		cout<<"* 4-�����ļ���������                  *"<<endl;
		cout<<"* 5-��ӡ����                          *"<<endl;
		cout<<"* 6-��ʼ��������                      *"<<endl;
        cout<<"* 7-��������                          *"<<endl;
		cout<<"*--------��������������㷨-----------*"<<endl;
		
		cout<<"* 7-�����                      *"<<endl;
		cout<<"* 8-�������Ԫ��                  *"<<endl;
		cout<<"* 9-���Ҹ���Ԫ��                  *"<<endl;
		cout<<"* 10-����Ԫ��                     *"<<endl;
		cout<<"* 11-ɾ��Ԫ��                     *"<<endl;
		
		cout<<"***********************************"<<endl;

		cout<<"����������ѡ�������";
		cin>>nChoice;

		switch(nChoice)
		{
		case 0:  //�˳�
			cout<<"��ǰѡ��������˳���"<<endl;
			cout<<"<-- �����˳� -->"<<endl;  //ѡ���˳�
			break;

		case 1:  //����ͷ�崴������
			system("cls");  //�����Ļ
			if(L!=NULL)
				destroyList(L);    //������ԭ����			

			createListH4(L);       //��������������

			printList(L);

			break;
		case 2:  //����β�崴������
			system("cls");  //�����Ļ

			if(L!=NULL)
				destroyList(L);    //������ԭ����
			
			createListR6(L);       //��������������

			printList(L);

			break;
			
		case 3:  //�������������
			system("cls");  //�����Ļ

			if(L!=NULL)
				destroyList(L);    //������ԭ����
			
			rndCList(L);   //�����ͷ�巨��������

		
			printList(L);
			break;		
/*		case 4:  //���ı��ļ������Ԫ������
			system("cls");  //�����Ļ
			if(L->next!=NULL)
			{
				cout<<"��Lδ��ʼ�������Ѿ����ڡ����ȳ�ʼ����������Ԫ�ء�"<<endl;
				break;
			}			
			
			char strFileName[100];
			cout<<"������˳��������ļ���(seqList.seq)��";
			cin>>strFileName;  //����̨���������ļ���			

			//listInputFile(strFileName,L);  //���ļ���������Ԫ��
			
			printList(L);

			break;			
*/		
		case 5:  //��ӡ��Ԫ��
			system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}
			printList(L);
			break;		
		case 6:  //��ʼ��˳���
			system("cls");  //�����Ļ
			if(L!=NULL)
				destroyList(L);
			initialList(L);
			cout<<"˳���L��ʼ����ɡ�"<<endl;
			break;			
		case 7:  //��������
			system("cls");  //�����Ļ

			destroyList(L);
			cout<<"<-- ��������� -->"<<endl; 

			break;

		case 8:  //1������㷨���Ҵ�ͷ��㵥��������k����㣨�������һ����㵹������k����������ҵ����ؽ���Ԫ��ֵ���Ҳ����򷵻�NULL��
		    system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}
			cout<<"�����뵹�������� k=";
			cin>>m;
			
			x=InverseKnum(L,m);
			if(x)
				cout<<"�������� "<<m<<" �����Ԫ��Ϊ��"<<x<<endl;
			else
				cout<<"�������� "<<m<<" ����㲻���ڡ�"<<endl;
			break;
		
/*		case 7:  //�����
			system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}
			cout<<"����Ϊ��"<<listLength(L)<<endl;
			break;
		case 8:
			system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}
			cout<<"������Ԫ����ţ�1-n��i=";
			cin>>i;
			p=getElement(L, i);
			if(p)
				cout<<"L�е�"<<i<<"��Ԫ���ǣ�"<<p->data<<"��"<<endl;
			else
				cout<<"�����ȡԪ��ʧ�ܡ�"<<endl;
			printList(L);
			break;
		case 9:  //����Ԫ�أ�����λ��
            system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}
			cout<<"������Ҫ���ҵ�Ԫ��x=";
			cin>>x;

			if(listLocate(L,x,&m))
				cout<<"���ҳɹ���Ԫ��x="<<x<<"�ڱ�L�е�λ�ã�"<<m<<"��"<<endl;
			else
				cout<<"����ʧ�ܣ�Ԫ��x="<<x<<"���ڱ�L�С�"<<endl;

			printList(L);
			break;
		case 10:  //����Ԫ��
			system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}
			cout<<"������Ҫ�����Ԫ��x=";
			cin>>x;
			cout<<"������Ҫ�����λ��i=";
			cin>>i;

			printList(L);

			if(listInsert(L,i,x))
				cout<<"����ɹ���Ԫ��x="<<x<<"�ɹ����뵽"<<i<<"λ�á�"<<endl;
			else
				cout<<"����ʧ�ܡ�"<<endl;
			
			printList(L);
			break;
		case 11:  //ɾ��Ԫ��
			system("cls");  //�����Ļ
			if(L==NULL)
			{
				cout<<"��L�����ڡ�"<<endl;
				break;
			}

			cout<<"������ɾ��Ԫ�ص�λ��i=";
			cin>>i;

			printList(L);
			
			if(listDelete(L,i))
				cout<<"ɾ���ɹ���"<<endl;
			else
				cout<<"ɾ��ʧ�ܡ�"<<endl;
			
			printList(L);

			break;
*/
		default:
			cout<<"����ѡ���������0��10֮��ѡ��==>"<<endl;
			break;
        }
	
	}while(nChoice!=0);

	destroyList(L);
	_CrtDumpMemoryLeaks();  //debug ģʽ�¼���Ƿ��ڴ�й©
	
	return 0;
}
