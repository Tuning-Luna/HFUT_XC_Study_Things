//double linked list definition and algorithm

typedef int elementType;

typedef struct dllNode
{
	elementType     data;
	struct dllNode *prior;
	struct dllNode *next;
}node;

//头插法创建双链表
void createDllH(node *&L)
{
	node *p;
	elementType x;

	L=new node;
	L->prior=L;
	L->next=L;
	cout<<"请输入元素数据（整数，-9999退出）:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //动态申请内存，产生新节点
		p->data=x;   //写入元素数据
		
		p->next=L->next;     //头插接入结点
		L->next->prior=p;        
		p->prior=L;
		L->next=p;

		cout<<"x=";
		cin>>x;
	}
}

//尾插法创建双链表
void createDllR(node *&L)
{
	node *p,*R;
	elementType x;
	L=new node;
	L->prior=L;
	L->next=L;
	R=L;
	cout<<"请输入元素数据（整数，-9999退出）:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //动态申请内存，产生新节点
		p->data=x;   //写入元素数据
		
		p->prior=L->prior;   //p->prior指原尾结点
		p->next=L;           //p->next指头结点
		L->prior->next=p;    //原尾结点的next指p
		L->prior=p;          //L->prior指p
		
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
	L->prior=L;
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

		p->next=L->next;      //头插接入
		L->next->prior=p;
		L->next=p;
		p->prior=L;
    }
	cout<<endl;
}
//随机数头插法创建链表结束------------------------------------------------------------------

//初始化
void initialList(node *&L)
{
	L=new node;
	L->prior=L;
	L->next=L;
}

//求表长度
int listLength(node *L)
{
	int len=0;
	node *p=L->next;
	while(p!=L)
	{
		len++;
		p=p->next;	
	}
	return len;
}

//按序号求元素
void getElement(node *L, int i, node *&p)
{
	int k=1;
	p=L->next;
	while(k!=i && p!=L)
	{
		k++;
		p=p->next;	
	}
	if(p==L)
		p=NULL;
}

//按元素值查找元素
void listLocate(node *L,elementType x, node *&p, int &m)
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
		p=NULL;
		m=0;
	}
}

//插入元素
bool listInsert(node *L,int i, elementType x)
{
	int k=1;
	node *p,*s;
	p=L->next;
	while(k!=i && p!=L)
	{
		k++;
		p=p->next;
	}
	if(p==L && k!=i)   //当p==L且k==i时，插入位置仍然合法，结点要插在最后成为尾结点
		return false;
	else
	{
		s=new node;
		s->data=x;

		s->next=p;
		s->prior=p->prior;
		p->prior->next=s;
		p->prior=s;
		return true;
	}
}

//删除元素
bool listDelete(node *L, int i)
{
	int k=1;
	node *p;
	p=L->next;
	while(k!=i && p!=L)
	{
		k++;
		p=p->next;
	}
	if(p==L)
		return false;
	else
	{
		p->next->prior=p->prior;
		p->prior->next=p->next;
		delete p;
		return true;
	}
}


//打印链表
void printList(node *L)
{
	node *p;
	p=L->next;
	while(p!=L)
	{	
		cout<<p->data<<"\t";
		p=p->next;
	}
	cout<<endl;
}

//销毁双链表
void destroyList(node *&L)
{
	node *p, *u;
	if(L==NULL)  //表未创建
		return;

	p=L;
	L->prior->next=NULL;   //断开循环
	while(p)
	{
		u=p;
		p=p->next;
		delete u;	
	}
	L=NULL;
}