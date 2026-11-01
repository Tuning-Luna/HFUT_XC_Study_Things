//#include "stdio.h"

#define MaxLen 100

typedef char elementType;

typedef struct
{
	elementType data[MaxLen];
	int listLen;
} seqList;


//创建一个空表
void initialList(seqList *L)
{
	L->listLen=0;
}



//求表长度
int listLength(seqList L)
{
	return L.listLen;
}

//按序号求元素
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

//按值查找元素
int listLocate(seqList L, elementType x)
{
	int i;
	for(i=0; i<L.listLen; i++)
		if(L.data[i]==x)
			return i+1;
    return 0; 
}

//插入元素
int listInsert(seqList *L, elementType x, int i)
{
	int j;
	if(L->listLen==MaxLen)
		return(0);  //表满，返回0
	else if(i<1 || i>L->listLen+1)
		return(1);  //序号超出范围，返回1
	else
	{
		for(j=L->listLen-1; j>=i-1; j--)
			L->data[j+1]=L->data[j];   //循环后移表元素
		L->data[i-1]=x;   //插入元素 x
		L->listLen++;     //表长度增1  
		return 2;         //成功插入，返回值2  
	}

}

//6.删除元素
int listDelete(seqList *L, int i )
{
	int j;
	if(L->listLen<=0)
		return 0; //空表，返回值0
	else if(i<1 || i>L->listLen)
		return 1;  //删除的序号不在有效范围内
	else
	{
		for(j=i; j<L->listLen; j++ )
			L->data[j-1]=L->data[j]; //循环前移表元素
		L->listLen--;  //修改表长度
		return 2;  //成功删除，返回值2.
	}
}

//创建顺序表
void CreateSeqList(seqList & L)
{
	elementType x;
	int i=0;
	cout<<"创建顺序表==>请输入数据元素（'-100'结束）："<<endl;
	cin>>x;
	while(x!=-100)
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //长度+1

		cin>>x;
	}
}

//创建顺序串
void CreateSeqListStr(seqList & L)
{
	elementType x;
	int i=0;
	cout<<"创建顺序串==>请输入数据元素（'#'结束）："<<endl;
	cin>>x;
	while(x!='#')
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //长度+1

		cin>>x;
	}
}