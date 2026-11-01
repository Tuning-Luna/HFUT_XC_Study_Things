// Exs201LinearListLast.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include "iostream.h"  //C++头文件

#include "stdlib.h"
#include "time.h"


#include "..\linearListLast.h"

int main(int argc, char* argv[])
{
	seqList L;
	int i,m;
	elementType x;

	L.last=-1;

	int nChoice=-1; //操作选择
	do
	{
		//显示主菜单
		cout<<"**********顺序表测试程序***********"<<endl;
		cout<<"* 0-退出                          *"<<endl;
		cout<<"* 1-交互创建顺序表                *"<<endl;
		cout<<"* 2-随机数创建顺序表              *"<<endl;
		cout<<"* 3-文件创建数据表                *"<<endl;
		cout<<"* 4-打印顺序表                    *"<<endl;
		cout<<"*---------------------------------*"<<endl;
		cout<<"* 5-初始化顺序表                  *"<<endl;
		cout<<"* 6-求表长度                      *"<<endl;
		cout<<"* 7-按序号求元素                  *"<<endl;
		cout<<"* 8-查找给定元素                  *"<<endl;
		cout<<"* 9-插入元素                      *"<<endl;
		cout<<"* 10-删除元素                     *"<<endl;
		cout<<"***********************************"<<endl;

		cout<<"请输入数字选择操作：";
		cin>>nChoice;

		switch(nChoice)
		{
		case 0:  //退出
			cout<<"当前选择操作：退出。"<<endl;
			cout<<"<-- 程序退出 -->"<<endl;  //选择退出
			break;

		case 1:  //交互创建顺序表--方法2
			system("cls");  //清除屏幕

			if(L.last!=-1)
			{
				cout<<"表L未初始化，或已经存在。请先初始化，再输入元素。"<<endl;
				break;
			}
			
			listInputC(L);  //结束符结束创建
			                 //listInputArr(&L);
			                 //listInputC1(&L);  //元素数结束创建
			cout<<"当前表长度："<<L.last+1<<endl;
			cout<<"当前表中元素："<<endl;
			printList(L);
			break;
			
		case 2:  //随机数创建顺序表
			system("cls");  //清除屏幕

			if(L.last!=-1)
			{
				cout<<"表L未初始化，或已经存在。请先初始化，再输入元素。"<<endl;
				break;
			}
			
			rndCList(L);   //随机数创建顺序表

			cout<<"当前表长度："<<L.last+1<<endl;
			cout<<"当前表中元素："<<endl;
			printList(L);
			break;		
		case 4:  //打印表元素
			system("cls");  //清除屏幕
			cout<<"当前表长度："<<L.last+1<<endl;
			cout<<"当前表中元素："<<endl;
			printList(L);
			break;		
		case 5:  //初始化顺序表
			system("cls");  //清除屏幕
			initialList(L);
			cout<<"顺序表L初始化完成。"<<endl;
			break;
/*		case 11:  //从文本文件输入表元素数据
			system("cls");  //清除屏幕
			if(L.last!=-1)
			{
				cout<<"表L未初始化，或已经存在。请先初始化，再输入元素。"<<endl;
				break;
			}			
			
			char strFileName[100];
			cout<<"请输入顺序表数据文件名(seqList.seq)：";
			cin>>strFileName;  //控制台输入数据文件名			

			listInputFile(strFileName,&L);  //从文件输入数据元素
			cout<<"当前表长度："<<L.listLen<<endl;
			cout<<"当前表中元素："<<endl;
			printList(L);

			break;			
*/			
		case 6:  //求表长度
			system("cls");  //清除屏幕
			cout<<"表长度为："<<listLength(L)<<endl;
			break;
		case 7:
			system("cls");  //清除屏幕
			cout<<"请输入元素序号（1-n）i=";
			cin>>i;
			
			if(getElement(L, i, x))
				cout<<"L中第"<<i<<"个元素是："<<x<<endl;
			else
				cout<<"按序号取元素失败。"<<endl;
			break;
		case 8:  //给定元素，查找位置
            system("cls");  //清除屏幕
			cout<<"请输入要查找的元素x=";
			cin>>x;

			i=listLocate(L,x); //调用查找函数

			if(i==0)
				cout<<"查找的元素x="<<x<<"不在表L中。"<<endl;
			else
				cout<<"查找的元素x="<<x<<"在表L中的位置："<<i<<endl;

			cout<<"当前表中元素："<<endl;
			printList(L);
			break;
		case 9:  //插入元素
			system("cls");  //清除屏幕
			cout<<"请输入要插入的元素x=";
			cin>>x;
			cout<<"请输入要插入的位置i=";
			cin>>i;

			cout<<"原来表长度："<<L.last+1<<endl;
			cout<<"原来表中元素："<<endl;
			printList(L);

			m=listInsert(L,x,i); //调用插入函数
			if(m==2)
				cout<<"恭喜您！元素x="<<x<<"插入成功！"<<endl;
			else if(m==1)
				cout<<"插入位置超出范围，元素插入失败！"<<endl;
			else
				cout<<"表空间已满，不能插入新的元素。"<<endl;
			cout<<"当前表长度："<<L.last+1<<endl;
			cout<<"当前表中元素："<<endl;
			printList(L);
			break;
		case 10:  //删除元素
			system("cls");  //清除屏幕
			cout<<"请输入删除元素的位置i=";
			cin>>i;

			cout<<"原来表长度："<<L.last+1<<endl;
			cout<<"原来表中元素："<<endl;
			printList(L);
			
			m=listDelete(L,i); //调用删除函数

			if(m==2)
				cout<<"恭喜您！删除元素成功！"<<endl;
			else if(m==1)
				cout<<"删除位置超出范围，删除元素失败！"<<endl;
			else
				cout<<"空表，没有元素可供删除。"<<endl;
			cout<<"当前表长度："<<L.last+1<<endl;
			cout<<"当前表中元素："<<endl;
			printList(L);

			break;

		default:
			cout<<"功能选择错误，请在0到10之间选择，==>"<<endl;
			break;
        }
	
	}while(nChoice!=0);
	
	return 0;
}
