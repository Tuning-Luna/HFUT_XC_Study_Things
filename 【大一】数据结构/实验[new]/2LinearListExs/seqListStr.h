//#include "stdio.h"

#define MaxLen 100

typedef char elementType;

typedef struct
{
	elementType data[MaxLen];
	int listLen;
} seqList;


//����һ���ձ�
void initialList(seqList *L)
{
	L->listLen=0;
}



//�����
int listLength(seqList L)
{
	return L.listLen;
}

//�������Ԫ��
bool getElement(seqList L, int i, elementType &x)
{
	if(i<1 || i>L.listLen)
		return 0;
	else
	{
		x=L.data[i-1];
		return 1;
	}		
}

//��ֵ����Ԫ��
int listLocate(seqList L, elementType x)
{
	int i;
	for(i=0; i<L.listLen; i++)
		if(L.data[i]==x)
			return i+1;
    return 0; 
}

//����Ԫ��
int listInsert(seqList *L, elementType x, int i)
{
	int j;
	if(L->listLen==MaxLen)
		return(0);  //����������0
	else if(i<1 || i>L->listLen+1)
		return(1);  //��ų�����Χ������1
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

//����˳���
void CreateSeqList(seqList & L)
{
	elementType x;
	int i=0;
	cout<<"����˳���==>����������Ԫ�أ�'-100'��������"<<endl;
	cin>>x;
	while(x!=-100)
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //����+1

		cin>>x;
	}
}

//����˳��
void CreateSeqListStr(seqList & L)
{
	elementType x;
	int i=0;
	cout<<"����˳��==>����������Ԫ�أ�'#'��������"<<endl;
	cin>>x;
	while(x!='#')
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //����+1

		cin>>x;
	}
}