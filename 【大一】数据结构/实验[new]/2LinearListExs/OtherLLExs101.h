//其它单链表问题求解测试算法（非教材例题、习题）

//1．设计算法查找带头结点单链表倒数第k个结点（即从最后一个结点倒着数第k个），如果找到返回结点的元素值，找不到则返回NULL。
//算法思想：如果链表长度大于等于k，用两根相距k的指针p和pk
//当p指向尾结点时，pk指向倒数第k个结点
//对结点进行循环，p初始指向第一个元素结点，并用i计数
//当计数到k时，pk开始指向第一个元素结点
//此后p与pk同步移动，当p指最后结点时，pk指倒数第k结点
elementType InverseKnum(node *L, int k)
{
	int i=0;
	node *p, *pk=NULL;    //pk指针指向倒数第k个结点
	p=L->next;            //p初始指向第一个元素结点

	if(k<=0)
		return NULL;

	while(p)
	{
		i++;              //计数p指过的结点
		if(i==k)          //p指第k个结点，让pk指第1个结点，两个指针相距k个结点
			pk=L->next;
		else if(i>k)
			pk=pk->next;
		p=p->next;	
	}

	if(pk)
		return pk->data;   //找到倒数第k个结点，返回元素值
	else
		return NULL;
}
