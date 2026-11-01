// Exs207DrdOrderedList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//删除递增有序顺序表中的重复元素
//【算法思想】
//将元素分成两个部分，左边部分为已经取得的有效元素部分，用一个下标i指示最后一个元素。
//右边部分是待处理的部分，用一个下标j指示第一个元素。
//左边下标小于i的元素已经处理好重复，等于i是当前正在处理的元素，
//将data[i]与data[j]进行比较，会出现下列情况：
//data[i]==data[j]，说明j指示的是i的重复元素，继续处理j的下一个元素，即执行j++
//data[i]<data[j]， 说明j指示元素与i的元素不同，如果i+1!=j，将j元素复制到i+1，再执行j++
//循环执行上述操作，直到表尾。
//时间复杂度：O(n)
#include "stdafx.h"
#include "iostream.h"

#include "..\seqList.h"

//删除递增顺序表重复元素
void DeleteRepeatData(seqList & L)
{
	int i,j;  //分别指向已处理部分最后元素和未处理部分第一个元素，皆为数组下标

	if(L.listLen<2)
		return;   //少于2个元素，直接退出
	i=0;  //初始化i指向第一个元素
	j=1;  //j指向第二个元素
	while(j<L.listLen)
	{
		if(L.data[i]==L.data[j])  //j为重复元素，j后移
			j++;
		else  //因为L为递增表，所以剩下情况即L.data[i]<L.data[j]，j为最终有效数据
		{
			    //如果j==i+1，说明j紧跟这i，无需移动元素，直接把i、j同时后移一位即可
			if((i+1)!=j)
				L.data[i+1]=L.data[j];  //j元素复制到i+1

			i++;  //无论那种情况，都需要同时后移i、j
			j++;
		}
	}
	L.listLen=i+1;  //修改表的实际长度
}

int main(int argc, char* argv[])
{
	seqList L;
	int i,j;
	elementType x;

	initialList(&L);  //初始化顺序表

	cout<<"创建顺序表==>请输入数据元素（'-100'结束）："<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //长度+1

		cin>>x;
	}
	

	//打印表中数据
	cout<<"当前表元素：";
	for(i=0;i<L.listLen;i++)
		cout<<L.data[i]<<", ";
	cout<<endl;


	DeleteRepeatData(L);  //调用删除重复元素
	
	//打印删除重复元素后的顺序表
	cout<<"删除重复元素后顺序表：";
	for(i=0; i<L.listLen; i++)
		cout<<L.data[i]<<", ";
	cout<<endl;

	return 0;

}

