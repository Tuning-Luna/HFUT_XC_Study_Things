// OtherLLExs101.cpp : Defines the entry point for the console application.
//
//非本教材例题、习题的其它题目的测试

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++头文件

#include "..\LinkedList.h"

#include "..\OtherLLExs101.h"    //其它问题求解测试

int main(int argc, char* argv[])
{
	node *L, *p;
	int i,m;
	elementType x;

	//initialList(L);
	L=NULL;

	int nChoice=-1; //操作选择
	do
	{
		//显示主菜单
		cout<<"******单链表其它问题求解测试程序*******"<<endl;
		cout<<"* 0-退出                              *"<<endl;
		cout<<"* 1-交互头插法创建链表                *"<<endl;
		cout<<"* 2-交互尾插法创建链表                *"<<endl;
		cout<<"* 3-随机数创建链表                    *"<<endl;
		cout<<"* 4-数据文件创建链表                  *"<<endl;
		cout<<"* 5-打印链表                          *"<<endl;
		cout<<"* 6-初始化单链表                      *"<<endl;
        cout<<"* 7-销毁链表                          *"<<endl;
		cout<<"*--------以下是问题求解算法-----------*"<<endl;
		
		cout<<"* 7-求表长度                      *"<<endl;
		cout<<"* 8-按序号求元素                  *"<<endl;
		cout<<"* 9-查找给定元素                  *"<<endl;
		cout<<"* 10-插入元素                     *"<<endl;
		cout<<"* 11-删除元素                     *"<<endl;
		
		cout<<"***********************************"<<endl;

		cout<<"请输入数字选择操作：";
		cin>>nChoice;

		switch(nChoice)
		{
		case 0:  //退出
			cout<<"当前选择操作：退出。"<<endl;
			cout<<"<-- 程序退出 -->"<<endl;  //选择退出
			break;

		case 1:  //交互头插创建链表
			system("cls");  //清除屏幕
			if(L!=NULL)
				destroyList(L);    //先销毁原链表			

			createListH4(L);       //结束符结束创建

			printList(L);

			break;
		case 2:  //交互尾插创建链表
			system("cls");  //清除屏幕

			if(L!=NULL)
				destroyList(L);    //先销毁原链表
			
			createListR6(L);       //结束符结束创建

			printList(L);

			break;
			
		case 3:  //随机数创建链表
			system("cls");  //清除屏幕

			if(L!=NULL)
				destroyList(L);    //先销毁原链表
			
			rndCList(L);   //随机数头插法创建链表

		
			printList(L);
			break;		
/*		case 4:  //从文本文件输入表元素数据
			system("cls");  //清除屏幕
			if(L->next!=NULL)
			{
				cout<<"表L未初始化，或已经存在。请先初始化，再输入元素。"<<endl;
				break;
			}			
			
			char strFileName[100];
			cout<<"请输入顺序表数据文件名(seqList.seq)：";
			cin>>strFileName;  //控制台输入数据文件名			

			//listInputFile(strFileName,L);  //从文件输入数据元素
			
			printList(L);

			break;			
*/		
		case 5:  //打印表元素
			system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}
			printList(L);
			break;		
		case 6:  //初始化顺序表
			system("cls");  //清除屏幕
			if(L!=NULL)
				destroyList(L);
			initialList(L);
			cout<<"顺序表L初始化完成。"<<endl;
			break;			
		case 7:  //销毁链表
			system("cls");  //清除屏幕

			destroyList(L);
			cout<<"<-- 表销毁完成 -->"<<endl; 

			break;

		case 8:  //1．设计算法查找带头结点单链表倒数第k个结点（即从最后一个结点倒着数第k个），如果找到返回结点的元素值，找不到则返回NULL。
		    system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}
			cout<<"请输入倒数结点个数 k=";
			cin>>m;
			
			x=InverseKnum(L,m);
			if(x)
				cout<<"链表倒数第 "<<m<<" 个结点元素为："<<x<<endl;
			else
				cout<<"链表倒数第 "<<m<<" 个结点不存在。"<<endl;
			break;
		
/*		case 7:  //求表长度
			system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}
			cout<<"表长度为："<<listLength(L)<<endl;
			break;
		case 8:
			system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}
			cout<<"请输入元素序号（1-n）i=";
			cin>>i;
			p=getElement(L, i);
			if(p)
				cout<<"L中第"<<i<<"个元素是："<<p->data<<"。"<<endl;
			else
				cout<<"按序号取元素失败。"<<endl;
			printList(L);
			break;
		case 9:  //给定元素，查找位置
            system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}
			cout<<"请输入要查找的元素x=";
			cin>>x;

			if(listLocate(L,x,&m))
				cout<<"查找成功，元素x="<<x<<"在表L中的位置："<<m<<"。"<<endl;
			else
				cout<<"查找失败，元素x="<<x<<"不在表L中。"<<endl;

			printList(L);
			break;
		case 10:  //插入元素
			system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}
			cout<<"请输入要插入的元素x=";
			cin>>x;
			cout<<"请输入要插入的位置i=";
			cin>>i;

			printList(L);

			if(listInsert(L,i,x))
				cout<<"插入成功，元素x="<<x<<"成功插入到"<<i<<"位置。"<<endl;
			else
				cout<<"插入失败。"<<endl;
			
			printList(L);
			break;
		case 11:  //删除元素
			system("cls");  //清除屏幕
			if(L==NULL)
			{
				cout<<"表L不存在。"<<endl;
				break;
			}

			cout<<"请输入删除元素的位置i=";
			cin>>i;

			printList(L);
			
			if(listDelete(L,i))
				cout<<"删除成功！"<<endl;
			else
				cout<<"删除失败。"<<endl;
			
			printList(L);

			break;
*/
		default:
			cout<<"功能选择错误，请在0到10之间选择，==>"<<endl;
			break;
        }
	
	}while(nChoice!=0);

	destroyList(L);
	_CrtDumpMemoryLeaks();  //debug 模式下检测是否内存泄漏
	
	return 0;
}
