//*********************************************//
//*    ˳����ͷ�ļ����ļ�����seqList.h      *//
//*    -- �˰汾ȫ������ָ�뷵��ֵ            *//
//*    -- �˰汾��iostream.h��Щ��������֧��  *//
//*********************************************//

#include <iostream.h>  //C++ͷ�ļ�
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"

#define MAXLEN 100  //�������Ա���������

typedef int elementType; //��������Ԫ�ص���������

//*********************************************//
//*          ����˳���Ľṹ                 *//
//*   2��������ɣ�data[]����������Ԫ��     *//
//*                listLen��ʾ����Ԫ�ظ���    *//
//*   ��typedef���䶨��Ϊ��������seqList      *//
//*********************************************//
typedef struct sList
{
	elementType data[MAXLEN];
	int listLen;
} seqList;

//1. ��ʼ����
//*********************************************//
//* �������ܣ���ʼ��˳���--�ÿձ�            *//
//* ����������seqList����ָ�룬ָ��˳���     *//
//* �� �� ֵ����                              *//
//* �� �� ����seqList.h  �� �� ����listInitial*//
//*********************************************//
void initialList(seqList *L)
{
	L->listLen=0;
}

//2. �����
//*********************************************//
//* �������ܣ�����ȣ���Ԫ�ظ���            *//
//* ����������seqList���ͱ���                 *//
//* �� �� ֵ�����ͣ���ĳ���                  *//
//* �� �� ����seqList.h  �� �� ����listLength *//
//*********************************************//
int listLength(seqList L)
{
	return L.listLen;
}

//3. ��ȡԪ��
//*********************************************//
//* �������ܣ���������ţ�ȡ������Ԫ��        *//
//* ���������seqList L��Ϊ��ǰ˳���         *//
//*           int i, ָ��Ԫ�����             *//
//* ���������elementType* x�����ػ�ȡ��Ԫ��  *// 
//* �� �� ֵ��bool��0��ȡԪ��ʧ�ܣ�1�ɹ�      *//
//* �� �� ����seqList.h  �� �� ����getElement *//
//*********************************************//
bool getElement(seqList L, int i, elementType* x)
{
	if(i<1 || i>L.listLen)
		return 0;
	else
	{
		(*x)=L.data[i-1];
		return 1;
	}		
}

//4. ��λԪ��
//*********************************************//
//* �������ܣ�����Ԫ�أ���ȡ���ڱ��е����    *//
//* ���������seqList L��Ϊ��ǰ˳���         *//
//*           elementType x��������Ԫ��       *//
//* �����������                              *// 
//* �� �� ֵ��Ԫ����ţ�0��ʾԪ�ز��ڱ���     *//
//* �� �� ����seqList.h  �� �� ����listLocate *//
//*********************************************//
int listLocate(seqList L, elementType x)
{
	int i;
	for(i=0; i<L.listLen; i++)
		if(L.data[i]==x)
			return i+1;
    return 0; 
}

//5. ����Ԫ��
//*********************************************//
//* �������ܣ��ڸ���λ�ã��������Ԫ��        *//
//* ���������seqList* L��Ϊ��ǰ˳���ָ��    *//
//*           elementType x�������Ĳ���Ԫ��   *//
//*           int i�������Ĳ���λ��           *//
//* ���������seqList* L��Ϊ��ǰ˳���ָ��    *// 
//* �� �� ֵ����������0��������1������λ�÷Ƿ�*//
//*           2������ɹ�                     *//
//* �� �� ����seqList.h  �� �� ����listInsert *//
//*********************************************//
int listInsert(seqList *L, elementType x, int i)
{
	int j;
	if(L->listLen==MAXLEN)
		return 0;  //����������0
	else if(i<1 || i>L->listLen+1)
		return 1;  //��ų�����Χ������1
	else
	{
		for(j=L->listLen-1; j>=i-1; j--)
			L->data[j+1]=L->data[j];   //ѭ�����Ʊ�Ԫ��
		L->data[i-1]=x;   //����Ԫ�� x
		L->listLen++;     //������1  
		return 2;         //�ɹ����룬����ֵ2  
	}
}

//6.ɾ��Ԫ��
//*********************************************//
//* �������ܣ�ɾ������ָ��λ�õ�Ԫ��          *//
//* ���������seqList* L��Ϊ��ǰ˳���ָ��    *//
//*           int i��������ɾ��λ��           *//
//* ���������seqList* L��Ϊ��ǰ˳���ָ��    *// 
//* �� �� ֵ����������0��������1��ɾ��λ�÷Ƿ�*//
//*           2��ɾ���ɹ�                     *//
//* �� �� ����seqList.h  �� �� ����listDelete *//
//*********************************************//
int listDelete(seqList *L, int i )
{
	int j;
	if(L->listLen<=0)
		return 0; //�ձ�����ֵ0
	else if(i<1 || i>L->listLen)
		return 1;  //ɾ������Ų�����Ч��Χ��
	else
	{
		for(j=i; j<L->listLen; j++ )
			L->data[j-1]=L->data[j]; //ѭ��ǰ�Ʊ�Ԫ��
		L->listLen--;  //�޸ı���
		return 2;  //�ɹ�ɾ��������ֵ2.
	}
}

//-------------* ���º���Ϊ�����õĸ������� *----------------------//

//7. ��ӡ��������Ԫ��
//*********************************************//
//* �������ܣ���Ļ��ӡ˳���������Ԫ��        *//
//* ���������seqList L��Ϊ��ǰ˳���         *//
//* �����������                              *// 
//* �� �� ֵ����                              *//
//* �� �� ����seqList.h  �� �� ����listPrint  *//
//*********************************************//
void printList(seqList L)
{
	int i;
	for(i=0;i<L.listLen;i++)
		cout<<L.data[i]<<"\t"; //Ԫ��֮�����Ʊ���ָ�
	cout<<endl;
}

//8. ������������Ԫ��--�����������
void listInputC(seqList* pL)
{
	if(pL->listLen>0)
	{
		cout<<"˳����Ѿ����ڣ����ȳ�ʼ����������Ԫ�ء�"<<endl;
		return;
	}
	
	elementType x;
	
	cout<<"����������Ԫ��(������-9999�˳�):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999)
	{
		pL->data[pL->listLen]=x;
		pL->listLen++;

		cout<<"x=";
		cin>>x;
	}
}

//9. ������������Ԫ��--ָ��Ԫ�ظ���
void listInputC1(seqList* pL)
{
	if(pL->listLen>0)
	{
		cout<<"˳����Ѿ����ڣ����ȳ�ʼ����������Ԫ�ء�"<<endl;
		return;
	}
	int i,n;  //Ԫ�ظ���
	cout<<"������Ԫ�ظ�����n=";
	cin>>n;
	cout<<"����������Ԫ�أ�"<<endl;

	for(i=0;i<n;i++)
	{
		cin>>pL->data[i];
		pL->listLen++;
	}
}

//10. ��������������Ԫ��
void listInputArr(seqList* pL)
{
	if(pL->listLen>0)
	{
		cout<<"˳����Ѿ����ڣ����ȳ�ʼ����������Ԫ�ء�"<<endl;
		return;
	}	
	int i,n; 	
	elementType arr[6]={2,8,16,20,26,50};
	n=sizeof(arr)/sizeof(*arr);	
	
	for(i=0;i<n;i++)
	{
		pL->data[i]=arr[i];
		pL->listLen++;
	}
}

//11. ���ļ���������Ԫ��
int listInputFile(char fileName[], seqList* pL)
{
	FILE* pFile;     //����˳�����ļ�ָ��
	char str[1000];  //��Ŷ���һ���ı����ַ���
	char strTemp[10]; //�ж��Ƿ�ע����
	char* ss; 

	pFile=fopen(fileName,"r");
	if(!pFile)
	{
		cout<<"�ļ�"<<fileName<<"��ʧ�ܡ�"<<endl;//printf("�ļ�CBiTree.CBT��ʧ�ܣ�\n");
		return false;
	}
	
	ss=fgets(str,1000,pFile);
	strncpy(strTemp,str,2);
	while((ss!=NULL) && (strstr(strTemp,"//")!=NULL) )  //����ע����
	{
		ss=fgets(str,1000,pFile);
		strncpy(strTemp,str,2);
		//cout<<strTemp<<endl;
	}
	    //ѭ��������str��Ӧ���Ѿ����ļ���ʶ���ж��ļ���ʽ
	//cout<<str<<endl;
	if(strstr(str,"seqList")==NULL)
	{
		printf("�򿪵��ļ���ʽ����\n");
		fclose(pFile); //�ر��ļ�
		return false;
	}

	//���¿�ʼ��ȡԪ�����ݣ�һ��һ��Ԫ������
	while(fgets(str,1000,pFile)!=NULL)
	{
		pL->data[pL->listLen]=atoi(str);//(elementType)str;
		pL->listLen++;
	}
	fclose(pFile); //�ر��ļ�
	return true;

}
//��������봴�����ұ�ʼ------------------------------------------------------------------
//���������˳���
void rndCList(seqList &L)
{
	int i;
	int n,m;
	
	L.listLen=0;
	cout<<"������Ҫ�����������������n=";
	cin>>n;
	
	if(n>MAXLEN-1)
	{
		cout<<"��Ҫ���������������������˲��ұ���"<<MAXLEN-1<<"������˳���ʧ�ܡ�"<<endl;
		return;
	
	}	
	cout<<"����������������С����������100��������������100��m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//�������������
	//srand((unsigned)GetTickCount());	//�������������
	for(i=0;i<n;i++)                //�����д�������A[]
		L.data[i]=rand()%m;
    L.listLen=n;                      //����Ϊn
	cout<<endl;
}
//��������봴�����ұ����------------------------------------------------------------------
