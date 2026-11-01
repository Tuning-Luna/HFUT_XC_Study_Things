//单循环链表定义及常用算法

typedef int elementType;

typedef struct scNode
{
	elementType    data;
	struct scNode *next;
}node;

//初始化单循环链表
void initialList(node *&L)
{
	L=new node;
	L->next=L;
}

//求表长度
int listLength(node *L)
{
	node *p;
	int i;
	i=0;
	p=L->next;
	while(p!=L)
	{
		i++;
		p=p->next;	
	}
	return i;
}

//按序号取元素
void getElement(node *L, int i, node *&p)
{
	int j;
	//node *p;
	p=L->next;
	j=1;
	while(j!=i && p!=L)
	{
		j++;
		p=p->next;	
	}
	if(p==L)
	{
		p=NULL;		
	}	
}

//按元素值查找元素
void listLocate(node *L, elementType x, node *&p, int &m )
{
	m=0;
	p=L->next;
	while(p!=L)
	{
		m++;
		if(p->data==x)
			break;
		else
			p=p->next;	
	}
	if(p==L)
	{
		m=0;
		p=NULL;
	}
}

//插入元素
bool listInsert(node *L, int i, elementType x)
{
	int j=0;
	node *p, *s;
	p=L;
	while(j!=i-1 && p->next!=L)
	{
		j++;
		p=p->next;	
	}
	if(p->next==L && j!=i-1)   //p->next==L，j==i-1时，插入位置仍合法，结点查到最后
		return false;
	else
	{
		s=new node;
		s->data=x;

		s->next=p->next;
		p->next=s;

		return true;
	}
}

//删除元素
bool listDelete(node *L, int i)
{
	int j=0;
	node *p, *u;
	p=L;
	while(j!=i-1 && p->next!=L)
	{
		j++;
		p=p->next;
	}
	if(p->next==L)
		return false;    //删除位置 i 超出范围，删除失败，返回false
	else
	{    //此时，j==i-1，p指向ai-1结点，p->next指要删除的目标结点ai
		u=p->next;
		p->next=u->next;  //跨过目标结点，或p->next=p->next->next;
		delete u;
		return true;	
	}
}

//头插法交互创建单循环链表
void createSCLLH(node *&L)
{
	node *p;
	elementType x;

	L=new node;
	L->next=L;     //形成循环

	cout<<"请输入元素数据（整数，-9999退出）:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //动态申请内存，产生新节点
		p->data=x;  //写入元素数据
		
		p->next=L->next;     //头插接入结点
		L->next=p;        
		
		cout<<"x=";
		cin>>x;
	}
}

//随机数头插法创建链表开始------------------------------------------------------------------
//5随机数头插法创建
void rndCList(node *&L)
{
	int i;
	int n,m;
	node *p;

	L=new node;  //产生头节点，动态的在heap上申请内存，存放一个节点（头结点）
	L->next=L;   //形成循环

	cout<<"请输入要产生的随机数个数，n=";
	cin>>n;
	
	cout<<"请输入控制随机数大小参数，比如100以内数，请输入100，m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//产生随机数种子
	//srand((unsigned)GetTickCount());	//产生随机数种子
	for(i=0;i<n;i++)
	{
		p=new node;
		p->data=rand()%m;     //随机数写入结点

		p->next=L->next;      //头插接入新结点
		L->next=p;
    }
	cout<<endl;
}
//随机数头插法创建链表结束------------------------------------------------------------------


//尾插法交互创建单循环链表
void createSCLLR(node *&L)
{
	node *p,*R;
	elementType x;

	L=new node;
	L->next=L;     //形成循环
	R=L;           //置尾指针

	cout<<"请输入元素数据（整数，-9999退出）:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //动态申请内存，产生新节点
		p->data=x;  //写入元素数据
		
		R->next=p;     //尾插接入结点
		R=p;        
		
		cout<<"x=";
		cin>>x;
	}
	R->next=L;     //形成循环
}


//销毁单循环链表
void destroyList(node *&L)
{
	node *p,*u;
	p=L->next;
	while(p!=L)
	{
		u=p;
		p=p->next;
		delete u;	
	}
	delete L;
	L=NULL;
}
//打印单循环链表
void printList(node* &L)
{
	node *p;
	//cout<<"当前表长度为："<<listLength(L)<<endl;
	cout<<"当前表中元素："<<endl;		
	p=L->next;
	while(p!=L)
	{
		cout<<p->data<<"\t";
		p=p->next;
	}
	cout<<endl;
}

