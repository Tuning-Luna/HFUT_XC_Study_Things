//*********************************************//
//*    顺序表的头文件，文件名：seqList.h      *//
//*    -- 此版本全部采用指针返回值            *//
//*    -- 此版本的iostream.h有些编译器不支持  *//
//*********************************************//

#include <iostream.h>  //C++头文件
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"

#define MAXLEN 100  //定义线性表的最大容量

typedef int elementType; //定义数据元素的数据类型

//*********************************************//
//*          定义顺序表的结构                 *//
//*   2个分量组成，data[]数组存放数据元素     *//
//*                listLen表示表中元素个数    *//
//*   用typedef将其定义为数据类型seqList      *//
//*********************************************//
typedef struct sList
{
	elementType data[MAXLEN];
	int listLen;
} seqList;

//1. 初始化表
//*********************************************//
//* 函数功能：初始化顺序表--置空表            *//
//* 函数参数：seqList类型指针，指向顺序表     *//
//* 返 回 值：空                              *//
//* 文 件 名：seqList.h  函 数 名：listInitial*//
//*********************************************//
void initialList(seqList *L)
{
	L->listLen=0;
}

//2. 求表长度
//*********************************************//
//* 函数功能：求表长度，即元素个数            *//
//* 函数参数：seqList类型变量                 *//
//* 返 回 值：整型，表的长度                  *//
//* 文 件 名：seqList.h  函 数 名：listLength *//
//*********************************************//
int listLength(seqList L)
{
	return L.listLen;
}

//3. 获取元素
//*********************************************//
//* 函数功能：按给定序号，取出表中元素        *//
//* 输入参数：seqList L，为当前顺序表         *//
//*           int i, 指定元素序号             *//
//* 输出参数：elementType* x，返回获取的元素  *// 
//* 返 回 值：bool，0：取元素失败，1成功      *//
//* 文 件 名：seqList.h  函 数 名：getElement *//
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

//4. 定位元素
//*********************************************//
//* 函数功能：给定元素，获取其在表中的序号    *//
//* 输入参数：seqList L，为当前顺序表         *//
//*           elementType x，给定的元素       *//
//* 输出参数：无                              *// 
//* 返 回 值：元素序号，0表示元素不在表中     *//
//* 文 件 名：seqList.h  函 数 名：listLocate *//
//*********************************************//
int listLocate(seqList L, elementType x)
{
	int i;
	for(i=0; i<L.listLen; i++)
		if(L.data[i]==x)
			return i+1;
    return 0; 
}

//5. 插入元素
//*********************************************//
//* 函数功能：在给定位置，插入给定元素        *//
//* 输入参数：seqList* L，为当前顺序表指针    *//
//*           elementType x，给定的插入元素   *//
//*           int i，给定的插入位置           *//
//* 输出参数：seqList* L，为当前顺序表指针    *// 
//* 返 回 值：整型数，0：表满；1：插入位置非法*//
//*           2：插入成功                     *//
//* 文 件 名：seqList.h  函 数 名：listInsert *//
//*********************************************//
int listInsert(seqList *L, elementType x, int i)
{
	int j;
	if(L->listLen==MAXLEN)
		return 0;  //表满，返回0
	else if(i<1 || i>L->listLen+1)
		return 1;  //序号超出范围，返回1
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
//*********************************************//
//* 函数功能：删除表中指定位置的元素          *//
//* 输入参数：seqList* L，为当前顺序表指针    *//
//*           int i，给定的删除位置           *//
//* 输出参数：seqList* L，为当前顺序表指针    *// 
//* 返 回 值：整型数，0：表满；1：删除位置非法*//
//*           2：删除成功                     *//
//* 文 件 名：seqList.h  函 数 名：listDelete *//
//*********************************************//
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

//-------------* 以下函数为测试用的辅助函数 *----------------------//

//7. 打印表中所有元素
//*********************************************//
//* 函数功能：屏幕打印顺序表中所有元素        *//
//* 输入参数：seqList L，为当前顺序表         *//
//* 输出参数：无                              *// 
//* 返 回 值：无                              *//
//* 文 件 名：seqList.h  函 数 名：listPrint  *//
//*********************************************//
void printList(seqList L)
{
	int i;
	for(i=0;i<L.listLen;i++)
		cout<<L.data[i]<<"\t"; //元素之间以制表符分割
	cout<<endl;
}

//8. 交互输入数据元素--特殊输入结束
void listInputC(seqList* pL)
{
	if(pL->listLen>0)
	{
		cout<<"顺序表已经存在，请先初始化，再输入元素。"<<endl;
		return;
	}
	
	elementType x;
	
	cout<<"请输入数据元素(整数，-9999退出):"<<endl;
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

//9. 交互输入数据元素--指定元素个数
void listInputC1(seqList* pL)
{
	if(pL->listLen>0)
	{
		cout<<"顺序表已经存在，请先初始化，再输入元素。"<<endl;
		return;
	}
	int i,n;  //元素个数
	cout<<"请输入元素个数：n=";
	cin>>n;
	cout<<"请输入数据元素："<<endl;

	for(i=0;i<n;i++)
	{
		cin>>pL->data[i];
		pL->listLen++;
	}
}

//10. 从数组输入数据元素
void listInputArr(seqList* pL)
{
	if(pL->listLen>0)
	{
		cout<<"顺序表已经存在，请先初始化，再输入元素。"<<endl;
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

//11. 从文件输入数据元素
int listInputFile(char fileName[], seqList* pL)
{
	FILE* pFile;     //定义顺序表的文件指针
	char str[1000];  //存放读出一行文本的字符串
	char strTemp[10]; //判断是否注释行
	char* ss; 

	pFile=fopen(fileName,"r");
	if(!pFile)
	{
		cout<<"文件"<<fileName<<"打开失败。"<<endl;//printf("文件CBiTree.CBT打开失败！\n");
		return false;
	}
	
	ss=fgets(str,1000,pFile);
	strncpy(strTemp,str,2);
	while((ss!=NULL) && (strstr(strTemp,"//")!=NULL) )  //跳过注释行
	{
		ss=fgets(str,1000,pFile);
		strncpy(strTemp,str,2);
		//cout<<strTemp<<endl;
	}
	    //循环结束，str中应该已经是文件标识，判断文件格式
	//cout<<str<<endl;
	if(strstr(str,"seqList")==NULL)
	{
		printf("打开的文件格式错误！\n");
		fclose(pFile); //关闭文件
		return false;
	}

	//以下开始读取元素数据，一行一个元素数据
	while(fgets(str,1000,pFile)!=NULL)
	{
		pL->data[pL->listLen]=atoi(str);//(elementType)str;
		pL->listLen++;
	}
	fclose(pFile); //关闭文件
	return true;

}
//随机数输入创建查找表开始------------------------------------------------------------------
//随机数创建顺序表
void rndCList(seqList &L)
{
	int i;
	int n,m;
	
	L.listLen=0;
	cout<<"请输入要产生的随机数个数，n=";
	cin>>n;
	
	if(n>MAXLEN-1)
	{
		cout<<"您要求产生的随机数个数超出了查找表长度"<<MAXLEN-1<<"，创建顺序表失败。"<<endl;
		return;
	
	}	
	cout<<"请输入控制随机数大小参数，比如100以内数，请输入100，m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//产生随机数种子
	//srand((unsigned)GetTickCount());	//产生随机数种子
	for(i=0;i<n;i++)                //随机数写入排序表A[]
		L.data[i]=rand()%m;
    L.listLen=n;                      //表长度为n
	cout<<endl;
}
//随机数输入创建查找表结束------------------------------------------------------------------
