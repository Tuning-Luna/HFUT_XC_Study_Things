#if  0
//������--��������洢 
#include <iostream>  
#include <string>
#include <fstream>
#include <queue>	
#include <stack>	
using namespace std;
#include<stdlib.h> 
#include "stdio.h"
#include "time.h"
#include "binaryTree.h"	//��������������洢�ļ� 
#include "CreateBiTree.h"   //����+�������д���������


//#include "..\btTraverseNR.h"   //�ǵݹ�����㷨ʵ��



int main(int argc, char* argv[])
{
	int len;	//�������г��� 
	string preStr, inStr, posStr;
	//BiTree BT;
	btNode* T = NULL, * p;
	btNode* pL, * pR;
	ElemType x;
	int nChoice;
	//ElemType InArr[NODENUM];  //InArr[0]����

	char strLine[NODENUM][3]; //�ļ����ݶ��뵽����		
	int  nArrLen = 0;           //����ʵ�ʳ��ȣ��������
	int  nR = 0;
	int i;

	do
	{
		//�����˵�
		cout << "****** ��������������ṹ���Գ��� ******" << endl;
		cout << "*  0--�˳�                             *" << endl;
		cout << "*  1--�����������н�������������       *" << endl;
		cout << "*  2--�������������ļ�����������       *" << endl;
		cout << "*  3--�����������н�������������       *" << endl;
		cout << "*  4--�������������ļ�����������       *" << endl;
		cout << "*  5--��������Ϣ��������������       *" << endl;
		cout << "*  6--��ȫ���������д���������         *" << endl;
		cout << "*  7--��ӡ������                       *" << endl;
		cout << "*  8--���ٶ�����                       *" << endl;
		cout << "*--------------------------------------*" << endl;
		cout << "*  9--�������������ݹ飩               *" << endl;
		cout << "*  10--�������������ǵݹ飩            *" << endl;

		cout << "*  8--��������߶ȣ���ȣ�             *" << endl;
		cout << "*  9--������                       *" << endl;
		cout << "*  10-- �ж�����                       *" << endl;
		cout << "*  11--��˫�׽��                      *" << endl;
		cout << "*  12--���ӽ��                      *" << endl;
		cout << "*  13--���ֵܽ��                      *" << endl;
		cout << "*  14--���ҽ��                        *" << endl;
		cout << "*  15--��������                      *" << endl;
		cout << "*  16--                                *" << endl;
		cout << "*  17--                                *" << endl;
		cout << "*  18--                                *" << endl;
		cout << "*                                      *" << endl;
		cout << "****************************************" << endl;
		cout << "��ѡ�����(0-14)��";
		cin >> nChoice;
		switch (nChoice)
		{
		case 0:  //�˳�����
			system("cls");  //�����Ļ
			cout << "<-- �����˳� -->" << endl;  //ѡ���˳�
			break;
		case 1:
			system("cls");  //�����Ļ
			if (T != NULL)     //��ֹ�ظ�����������ڴ�й¶
			{
				cout << "�������Ѿ����ڣ��������´��������������ٴ�����" << endl;
				break;
			}
			cout << "���2�зֱ�����������������У�" << endl;
			cin >> preStr;	//��������			
			cin >> inStr;		//��������			
			len = preStr.length();	//�����г��� 						
			createBiTree(T, preStr, inStr, 0, 0, len);	//����������				
			cout << "<-- ������������� -->" << endl;
			break;
		case 2:  //�����������������ļ�����������
			system("cls");  //�����Ļ
			if (T != NULL)  //��ֹ�ظ�����������ڴ�й¶
			{
				cout << "�������Ѿ����ڣ��������´��������������ٴ�����" << endl;
				break;
			}
			char strFileName[100];
			cout << "����������������ļ���(i.e. bt3.in)��";
			cin >> strFileName;  //����̨���������ļ���			
			readFile(strFileName, preStr, inStr);
			len = preStr.length();	//�����г��� 						
			createBiTree(T, preStr, inStr, 0, 0, len);	//����������
			cout << "<-- ������������� -->" << endl;
			break;
		case 3:	//�����������н������������� 
			system("cls");  //�����Ļ
			if (T != NULL)     //��ֹ�ظ�����������ڴ�й¶
			{
				cout << "�������Ѿ����ڣ��������´��������������ٴ�����" << endl;
				break;
			}
			cout << "���2�зֱ����������������У�" << endl;
			cin >> posStr;	//��������			
			cin >> inStr;		//��������			
			len = posStr.length();	//�����г��� 						
			creBTpos(T, posStr, inStr, len - 1, 0, len);	//����������				
			cout << "<-- ������������� -->" << endl;
			break;
		case 4:  //�����������������ļ�����������
			system("cls");  //�����Ļ
			if (T != NULL)  //��ֹ�ظ�����������ڴ�й¶
			{
				cout << "�������Ѿ����ڣ��������´��������������ٴ�����" << endl;
				break;
			}
			//char strFileName[100];
			cout << "����������������ļ���(i.e. bt3.in)��";
			cin >> strFileName;  //����̨���������ļ���			
			readFile(strFileName, posStr, inStr);
			len = posStr.length();	//�����г��� 						
			creBTpos(T, posStr, inStr, len - 1, 0, len);	//����������
			cout << "<-- ������������� -->" << endl;
			break;
		case 5:	//���򡢺�����Ϣ�������������� 
			system("cls");  //�����Ļ
			if (T != NULL)     //��ֹ�ظ�����������ڴ�й¶
			{
				cout << "�������Ѿ����ڣ��������´��������������ٴ�����" << endl;
				break;
			}

			cout << "�밴�������������������㣬����/����ʾ��������:" << endl;

			cin >> preStr;
			len = preStr.length();
			i = 0;
			crePreChi(T, preStr, i, len);	//��ʽһ��һ�ж��뵽���� 

			//crePreChi1( T );	//��ʽ�����ִζ����� 
			cout << "<-- ������������� -->" << endl;
			break;
		case 6:   //����ȫ�������������鴴��������
			system("cls");  //�����Ļ
			if (T != NULL)     //��ֹ�ظ�����������ڴ�й¶
			{
				cout << "�������Ѿ����ڣ��������´��������������ٴ�����" << endl;
				break;
			}
			//�������������ȫ����ȫ�����������������
			cout << "�����벹ȫ����ȫ���������У�����/����ʾ��������:" << endl;
			cin >> inStr;
			len = inStr.length();
			creBtCom(T, inStr, 0, len);	//����������			
			cout << "<--������������ϡ�-->" << endl;
			break;



		case 8:  //���ٶ�����
			system("cls");  //�����Ļ
			if (T != NULL)
				destroyBt(T);  //���ٶ�����			
			cout << "<-- �������Ѿ����٣�-->" << endl;
			break;

		case 9:  //����������--�ݹ�
			system("cls");  //�����Ļ			
			if (!T)
			{
				cout << "<--������Ϊ������-->" << endl;
				break;
			}
			cout << "���������������" << endl;
			cout << "�������";
			preOrder(T);   //�ݹ��������
			cout << endl;
			cout << "�������";
			inOrder(T);    //�ݹ��������
			cout << endl;
			cout << "�������";
			postOrder(T);  //�ݹ�������
			cout << endl;
			cout << "��δ���";
			hieOrder(T);	//��α��� 
			cout << endl;
			break;
		case 10:  //�ǵݹ����������
			system("cls");  //�����Ļ			
			if (!T)
			{
				cout << "<--������Ϊ������-->" << endl;
				break;
			}
			cout << "���������������" << endl;
			cout << "�������";
			preOrderNR(T);   //�ǵݹ��������
			cout << endl;
			cout << "�������";
			inOrderNR(T);    //�ǵݹ��������
			cout << endl;
			cout << "�������";
			postOrderNR(T);  //�ǵݹ�������
			cout << endl;
			cout << "��δ���";
			hieOrder(T);	//��α���
			cout << endl;
			break;


			/*




					case 8:  //��������߶ȣ���ȣ�
						system("cls");  //�����Ļ
						cout<<"�������߶ȣ���ȣ���"<<btHeight(T)<<endl;//����������
						break;
					case 9:  //��Ԫ��x��Ӧ����Σ�����ӡ�����Ƚ��
						if(isEmpty(T))
						{
							cout<<"<--������Ϊ������-->"<<endl;
							break;
						}
						cout<<"������Ŀ����Ԫ��ֵ��x=";
						cin>>x;
						cout<<"��� x="<<x<<" �����Ƚ�㣺";
						getNodeLevel(T,  x, nR, 1);    //�ݹ�������
						cout<<endl;
						if(nR>0)
							cout<<"��� x="<<x<<" �Ĳ���Ϊ��"<<nR<<endl;
						else
							cout<<"��� x="<<x<<" �������ϡ�"<<endl;
						break;
					case 10:  //�ж��Ƿ����
						system("cls");  //�����Ļ
						if(isEmpty(T))
							cout<<"<-- ������Ϊ������-->"<<endl;
						else
							cout<<"<-- �ǿն�������-->"<<endl;
						break;
					case 11:  //��˫�׽��
						system("cls");  //�����Ļ
						if(isEmpty(T))
						{
							cout<<"<--��������˫�׽��ʧ�ܡ�-->"<<endl;
							break;
						}
						cout<<"�����뵱ǰ���Ԫ��ֵ��x=";
						cin>>x;
						p=NULL;
						getParent(T, x, p);  //����˫�׽��
						if(p==NULL)
							cout<<"x="<<x<<"��˫�׽�㲻���ڡ�"<<endl;
						else
							cout<<"x="<<x<<"��˫�׽��Ϊ��"<<p->data<<endl;

						break;
					case 12:  //����ĺ��ӽ��
						system("cls");  //�����Ļ
						if(isEmpty(T))
						{
							cout<<"<--������Ϊ������-->"<<endl;
							break;
						}
						cout<<"��������Ԫ��ֵ��";
						cin>>x;
						p=NULL;
						nR=getChildren(T,x,pL,pR);	//ȥ���ӽ��

						if(nR==0)
							cout<<"��ǰ������ȡ���ӽ��ʧ�ܡ�"<<endl;
						else if(nR==1)
							cout<<"Ŀ��Ԫ�ز������ϣ�ȡ���ӽ��ʧ�ܡ�"<<endl;
						else
						{
							if(pL)
								cout<<"Ԫ��x="<<x<<"��Ӧ�����ӽ����ڣ�ֵΪ��"<<pL->data<<"��"<<endl;
							else
								cout<<"Ԫ��x="<<x<<"��Ӧ�����ӽ�㲻����(��)��"<<endl;
							if(pR)
								cout<<"Ԫ��x="<<x<<"��Ӧ���Һ��ӽ����ڣ�ֵΪ��"<<pR->data<<"��"<<endl;
							else
								cout<<"Ԫ��x="<<x<<"��Ӧ���Һ��ӽ�㲻����(��)��"<<endl;
						}
						break;
					case 13:  //���ֵܽ��
						system("cls");  //�����Ļ
						if(isEmpty(T))
						{
							cout<<"<--������Ϊ������-->"<<endl;
							break;
						}
						cout<<"��������Ԫ��ֵ��x=";
						cin>>x;
						nR=getSibling(T,x,pL,pR);

						switch(nR)
						{
						case 0:
							cout<<"��ǰ������ȡ�ֵܽ��ʧ�ܡ�"<<endl;
							break;
						case 1:
							cout<<"Ŀ����Ϊ����㣬ȡ�ֵܽ��ʧ�ܡ�"<<endl;
							break;
						case 2:
							cout<<"Ŀ��Ԫ�ز������ϣ�ȡ�ֵܽ��ʧ�ܡ�"<<endl;
							break;
						case 3:   //���ܴ������ֵ�
							if(pR)
								cout<<"Ԫ��x="<<x<<"�������ֵܽ����ڣ�ֵΪ��"<<pR->data<<"��"<<endl;
							else
								cout<<"Ԫ��x="<<x<<"�������ֵܲ�����(��)��"<<endl;
							break;
						case 4:   //���ܴ������ֵ�
							if(pL)
								cout<<"Ԫ��x="<<x<<"������ܽ����ڣ�ֵΪ��"<<pL->data<<"��"<<endl;
							else
								cout<<"Ԫ��x="<<x<<"�������ֵܲ�����(��)��"<<endl;
							break;
						}
						break;

					case 14:  //��Ԫ��ֵ�����ڵ㣬���ؽڵ�ָ��
						system("cls");  //�����Ļ
						if(isEmpty(T))
						{
							cout<<"<--������Ϊ������-->"<<endl;
							break;
						}
						cout<<"������Ҫ�����Ľ��Ԫ��ֵ(x)��";
						cin>>x;

						btSearch(T, x, p);
						if(p!=NULL)
							cout<<"Ԫ�� x="<<x<<" ����ҵ���"<<endl;
						else
							cout<<"Ԫ�� x="<<x<<" �������ϡ�"<<endl;
						p=NULL;  //�ָ���ָ��
						break;
					case 15:  //������
						system("cls");  //�����Ļ
						nR=0;
						getNodeNumber(T,nR);     //���ܽ����
						cout<<"�������������"<<nR<<endl;

						nR=getNodeNumber1(T);     //���ܽ����
						cout<<"�������������"<<nR<<endl;  //���������

						nR=0;
						getLeafNumber(T,nR);     //��Ҷ����
						cout<<"Ҷ�ӽ��������"<<nR<<endl;

						nR=0;
						get2DegreeNumber(T,nR);    //��2�Ƚ����
						cout<<"2�Ƚ��������"<<nR<<endl;

						nR=0;
						get1DegreeNumber(T,nR);    //��1�Ƚ����
						cout<<"1�Ƚ��������"<<nR<<endl;
						break;
			 */
		default:
			cout << "����ѡ���������0��5֮��ѡ��==>" << endl;
			break;
		}

	} while (nChoice != 0);  //ѭ����ʾ�˵���nMenu==0�˳�

	if (T != NULL)
		destroyBt(T);    //���ٶ�����


	return 0;
}
#endif 




