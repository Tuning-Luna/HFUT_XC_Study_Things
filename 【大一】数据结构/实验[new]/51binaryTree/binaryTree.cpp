#if  0
//二叉树--二叉链表存储 
#include <iostream>  
#include <string>
#include <fstream>
#include <queue>	
#include <stack>	
using namespace std;
#include<stdlib.h> 
#include "stdio.h"
#include "time.h"
#include "binaryTree.h"	//二叉树二叉链表存储文件 
#include "CreateBiTree.h"   //先序+中序序列创建二叉树


//#include "..\btTraverseNR.h"   //非递归遍历算法实现



int main(int argc, char* argv[])
{
	int len;	//遍历序列长度 
	string preStr, inStr, posStr;
	//BiTree BT;
	btNode* T = NULL, * p;
	btNode* pL, * pR;
	ElemType x;
	int nChoice;
	//ElemType InArr[NODENUM];  //InArr[0]不用

	char strLine[NODENUM][3]; //文件数据读入到数组		
	int  nArrLen = 0;           //数组实际长度，即结点数
	int  nR = 0;
	int i;

	do
	{
		//创建菜单
		cout << "****** 二叉树二叉链表结构测试程序 ******" << endl;
		cout << "*  0--退出                             *" << endl;
		cout << "*  1--先序中序序列交互创建二叉树       *" << endl;
		cout << "*  2--先序中序序列文件创建二叉树       *" << endl;
		cout << "*  3--后序中序序列交互创建二叉树       *" << endl;
		cout << "*  4--后序中序序列文件创建二叉树       *" << endl;
		cout << "*  5--先序孩子信息交互创建二叉树       *" << endl;
		cout << "*  6--完全二叉树序列创建二叉树         *" << endl;
		cout << "*  7--打印二叉树                       *" << endl;
		cout << "*  8--销毁二叉树                       *" << endl;
		cout << "*--------------------------------------*" << endl;
		cout << "*  9--遍历二叉树（递归）               *" << endl;
		cout << "*  10--遍历二叉树（非递归）            *" << endl;

		cout << "*  8--求二叉树高度（深度）             *" << endl;
		cout << "*  9--求结点层次                       *" << endl;
		cout << "*  10-- 判定空树                       *" << endl;
		cout << "*  11--求双亲结点                      *" << endl;
		cout << "*  12--求孩子结点                      *" << endl;
		cout << "*  13--求兄弟结点                      *" << endl;
		cout << "*  14--查找结点                        *" << endl;
		cout << "*  15--求结点数量                      *" << endl;
		cout << "*  16--                                *" << endl;
		cout << "*  17--                                *" << endl;
		cout << "*  18--                                *" << endl;
		cout << "*                                      *" << endl;
		cout << "****************************************" << endl;
		cout << "请选择操作(0-14)：";
		cin >> nChoice;
		switch (nChoice)
		{
		case 0:  //退出程序
			system("cls");  //清除屏幕
			cout << "<-- 程序退出 -->" << endl;  //选择退出
			break;
		case 1:
			system("cls");  //清除屏幕
			if (T != NULL)     //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			cout << "请分2行分别输入先序和中序序列：" << endl;
			cin >> preStr;	//先序序列			
			cin >> inStr;		//中序序列			
			len = preStr.length();	//求序列长度 						
			createBiTree(T, preStr, inStr, 0, 0, len);	//创建二叉树				
			cout << "<-- 二叉树创建完成 -->" << endl;
			break;
		case 2:  //先序、中序序列数据文件创建二叉树
			system("cls");  //清除屏幕
			if (T != NULL)  //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			char strFileName[100];
			cout << "请输入二叉树数据文件名(i.e. bt3.in)：";
			cin >> strFileName;  //控制台输入数据文件名			
			readFile(strFileName, preStr, inStr);
			len = preStr.length();	//求序列长度 						
			createBiTree(T, preStr, inStr, 0, 0, len);	//创建二叉树
			cout << "<-- 二叉树创建完成 -->" << endl;
			break;
		case 3:	//后序、中序序列交互创建二叉树 
			system("cls");  //清除屏幕
			if (T != NULL)     //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			cout << "请分2行分别输入后序和中序序列：" << endl;
			cin >> posStr;	//后序序列			
			cin >> inStr;		//中序序列			
			len = posStr.length();	//求序列长度 						
			creBTpos(T, posStr, inStr, len - 1, 0, len);	//创建二叉树				
			cout << "<-- 二叉树创建完成 -->" << endl;
			break;
		case 4:  //后序、中序序列数据文件创建二叉树
			system("cls");  //清除屏幕
			if (T != NULL)  //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			//char strFileName[100];
			cout << "请输入二叉树数据文件名(i.e. bt3.in)：";
			cin >> strFileName;  //控制台输入数据文件名			
			readFile(strFileName, posStr, inStr);
			len = posStr.length();	//求序列长度 						
			creBTpos(T, posStr, inStr, len - 1, 0, len);	//创建二叉树
			cout << "<-- 二叉树创建完成 -->" << endl;
			break;
		case 5:	//先序、孩子信息交互创建二叉树 
			system("cls");  //清除屏幕
			if (T != NULL)     //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}

			cout << "请按先序序列输入二叉树结点，（‘/’表示无子树）:" << endl;

			cin >> preStr;
			len = preStr.length();
			i = 0;
			crePreChi(T, preStr, i, len);	//方式一：一行读入到数组 

			//crePreChi1( T );	//方式二：分次读入结点 
			cout << "<-- 二叉树创建完成 -->" << endl;
			break;
		case 6:   //从完全二叉树序列数组创建二叉树
			system("cls");  //清除屏幕
			if (T != NULL)     //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			//交互输入产生补全的完全二叉树结点序列数组
			cout << "请输入补全的完全二叉树序列，（‘/’表示无子树）:" << endl;
			cin >> inStr;
			len = inStr.length();
			creBtCom(T, inStr, 0, len);	//创建二叉树			
			cout << "<--二叉树创建完毕。-->" << endl;
			break;



		case 8:  //销毁二叉树
			system("cls");  //清除屏幕
			if (T != NULL)
				destroyBt(T);  //销毁二叉树			
			cout << "<-- 二叉树已经销毁！-->" << endl;
			break;

		case 9:  //遍历二叉树--递归
			system("cls");  //清除屏幕			
			if (!T)
			{
				cout << "<--二叉树为空树。-->" << endl;
				break;
			}
			cout << "二叉树遍历结果：" << endl;
			cout << "先序次序：";
			preOrder(T);   //递归先序遍历
			cout << endl;
			cout << "中序次序：";
			inOrder(T);    //递归中序遍历
			cout << endl;
			cout << "后序次序：";
			postOrder(T);  //递归后序遍历
			cout << endl;
			cout << "层次次序：";
			hieOrder(T);	//层次遍历 
			cout << endl;
			break;
		case 10:  //非递归遍历二叉树
			system("cls");  //清除屏幕			
			if (!T)
			{
				cout << "<--二叉树为空树。-->" << endl;
				break;
			}
			cout << "二叉树遍历结果：" << endl;
			cout << "先序次序：";
			preOrderNR(T);   //非递归先序遍历
			cout << endl;
			cout << "中序次序：";
			inOrderNR(T);    //非递归中序遍历
			cout << endl;
			cout << "后序次序：";
			postOrderNR(T);  //非递归后序遍历
			cout << endl;
			cout << "层次次序：";
			hieOrder(T);	//层次遍历
			cout << endl;
			break;


			/*




					case 8:  //求二叉树高度（深度）
						system("cls");  //清除屏幕
						cout<<"二叉树高度（深度）："<<btHeight(T)<<endl;//求二叉树深度
						break;
					case 9:  //求元素x对应结点层次，并打印其祖先结点
						if(isEmpty(T))
						{
							cout<<"<--二叉树为空树。-->"<<endl;
							break;
						}
						cout<<"请输入目标结点元素值：x=";
						cin>>x;
						cout<<"结点 x="<<x<<" 的祖先结点：";
						getNodeLevel(T,  x, nR, 1);    //递归求结点层次
						cout<<endl;
						if(nR>0)
							cout<<"结点 x="<<x<<" 的层数为："<<nR<<endl;
						else
							cout<<"结点 x="<<x<<" 不在树上。"<<endl;
						break;
					case 10:  //判断是否空树
						system("cls");  //清除屏幕
						if(isEmpty(T))
							cout<<"<-- 二叉树为空树。-->"<<endl;
						else
							cout<<"<-- 非空二叉树。-->"<<endl;
						break;
					case 11:  //求双亲结点
						system("cls");  //清除屏幕
						if(isEmpty(T))
						{
							cout<<"<--空树，求双亲结点失败。-->"<<endl;
							break;
						}
						cout<<"请输入当前结点元素值：x=";
						cin>>x;
						p=NULL;
						getParent(T, x, p);  //搜索双亲结点
						if(p==NULL)
							cout<<"x="<<x<<"的双亲结点不存在。"<<endl;
						else
							cout<<"x="<<x<<"的双亲结点为："<<p->data<<endl;

						break;
					case 12:  //求结点的孩子结点
						system("cls");  //清除屏幕
						if(isEmpty(T))
						{
							cout<<"<--二叉树为空树。-->"<<endl;
							break;
						}
						cout<<"请输入结点元素值：";
						cin>>x;
						p=NULL;
						nR=getChildren(T,x,pL,pR);	//去孩子结点

						if(nR==0)
							cout<<"当前空树，取孩子结点失败。"<<endl;
						else if(nR==1)
							cout<<"目标元素不在树上，取孩子结点失败。"<<endl;
						else
						{
							if(pL)
								cout<<"元素x="<<x<<"对应的左孩子结点存在，值为："<<pL->data<<"。"<<endl;
							else
								cout<<"元素x="<<x<<"对应的左孩子结点不存在(空)。"<<endl;
							if(pR)
								cout<<"元素x="<<x<<"对应的右孩子结点存在，值为："<<pR->data<<"。"<<endl;
							else
								cout<<"元素x="<<x<<"对应的右孩子结点不存在(空)。"<<endl;
						}
						break;
					case 13:  //求兄弟结点
						system("cls");  //清除屏幕
						if(isEmpty(T))
						{
							cout<<"<--二叉树为空树。-->"<<endl;
							break;
						}
						cout<<"请输入结点元素值：x=";
						cin>>x;
						nR=getSibling(T,x,pL,pR);

						switch(nR)
						{
						case 0:
							cout<<"当前空树，取兄弟结点失败。"<<endl;
							break;
						case 1:
							cout<<"目标结点为根结点，取兄弟结点失败。"<<endl;
							break;
						case 2:
							cout<<"目标元素不在树上，取兄弟结点失败。"<<endl;
							break;
						case 3:   //可能存在右兄弟
							if(pR)
								cout<<"元素x="<<x<<"结点的右兄弟结点存在，值为："<<pR->data<<"。"<<endl;
							else
								cout<<"元素x="<<x<<"结点的右兄弟不存在(空)！"<<endl;
							break;
						case 4:   //可能存在左兄弟
							if(pL)
								cout<<"元素x="<<x<<"结点的左弟结点存在，值为："<<pL->data<<"。"<<endl;
							else
								cout<<"元素x="<<x<<"结点的左兄弟不存在(空)！"<<endl;
							break;
						}
						break;

					case 14:  //按元素值搜索节点，返回节点指针
						system("cls");  //清除屏幕
						if(isEmpty(T))
						{
							cout<<"<--二叉树为空树。-->"<<endl;
							break;
						}
						cout<<"请输入要搜索的结点元素值(x)：";
						cin>>x;

						btSearch(T, x, p);
						if(p!=NULL)
							cout<<"元素 x="<<x<<" 结点找到！"<<endl;
						else
							cout<<"元素 x="<<x<<" 不在树上。"<<endl;
						p=NULL;  //恢复空指针
						break;
					case 15:  //求结点数
						system("cls");  //清除屏幕
						nR=0;
						getNodeNumber(T,nR);     //求总结点数
						cout<<"二叉树结点数："<<nR<<endl;

						nR=getNodeNumber1(T);     //求总结点数
						cout<<"二叉树结点数："<<nR<<endl;  //方法二求解

						nR=0;
						getLeafNumber(T,nR);     //求叶子数
						cout<<"叶子结点数量："<<nR<<endl;

						nR=0;
						get2DegreeNumber(T,nR);    //求2度结点数
						cout<<"2度结点数量："<<nR<<endl;

						nR=0;
						get1DegreeNumber(T,nR);    //求1度结点数
						cout<<"1度结点数量："<<nR<<endl;
						break;
			 */
		default:
			cout << "功能选择错误，请在0到5之间选择，==>" << endl;
			break;
		}

	} while (nChoice != 0);  //循环显示菜单，nMenu==0退出

	if (T != NULL)
		destroyBt(T);    //销毁二叉树


	return 0;
}
#endif 




