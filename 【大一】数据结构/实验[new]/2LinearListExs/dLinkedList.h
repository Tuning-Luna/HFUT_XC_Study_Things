//double linked list definition and algorithm

typedef int elementType;

typedef struct dllNode
{
	elementType     data;
	struct dllNode *prior;
	struct dllNode *next;
}node;

//ͷ�巨����˫����
void createDllH(node *&L)
{
	node *p;
	elementType x;

	L=new node;
	L->prior=L;
	L->next=L;
	cout<<"������Ԫ�����ݣ�������-9999�˳���:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //��̬�����ڴ棬�����½ڵ�
		p->data=x;   //д��Ԫ������
		
		p->next=L->next;     //ͷ�������
		L->next->prior=p;        
		p->prior=L;
		L->next=p;

		cout<<"x=";
		cin>>x;
	}
}

//β�巨����˫����
void createDllR(node *&L)
{
	node *p,*R;
	elementType x;
	L=new node;
	L->prior=L;
	L->next=L;
	R=L;
	cout<<"������Ԫ�����ݣ�������-9999�˳���:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //��̬�����ڴ棬�����½ڵ�
		p->data=x;   //д��Ԫ������
		
		p->prior=L->prior;   //p->priorָԭβ���
		p->next=L;           //p->nextָͷ���
		L->prior->next=p;    //ԭβ����nextָp
		L->prior=p;          //L->priorָp
		
		cout<<"x=";
		cin>>x;
	}
}

//�����ͷ�巨��������ʼ------------------------------------------------------------------
//5�����ͷ�巨����
void rndCList(node *&L)
{
	int i;
	int n,m;
	node *p;

	L=new node;  //����ͷ�ڵ㣬��̬����heap�������ڴ棬���һ���ڵ㣨ͷ��㣩
	L->prior=L;
	L->next=L;   //�γ�ѭ��

	cout<<"������Ҫ�����������������n=";
	cin>>n;
	
	cout<<"����������������С����������100��������������100��m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//�������������
	//srand((unsigned)GetTickCount());	//�������������
	for(i=0;i<n;i++)
	{
		p=new node;
		p->data=rand()%m;     //�����д����

		p->next=L->next;      //ͷ�����
		L->next->prior=p;
		L->next=p;
		p->prior=L;
    }
	cout<<endl;
}
//�����ͷ�巨�����������------------------------------------------------------------------

//��ʼ��
void initialList(node *&L)
{
	L=new node;
	L->prior=L;
	L->next=L;
}

//�����
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

//�������Ԫ��
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

//��Ԫ��ֵ����Ԫ��
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

//����Ԫ��
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
	if(p==L && k!=i)   //��p==L��k==iʱ������λ����Ȼ�Ϸ������Ҫ��������Ϊβ���
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

//ɾ��Ԫ��
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


//��ӡ����
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

//����˫����
void destroyList(node *&L)
{
	node *p, *u;
	if(L==NULL)  //��δ����
		return;

	p=L;
	L->prior->next=NULL;   //�Ͽ�ѭ��
	while(p)
	{
		u=p;
		p=p->next;
		delete u;	
	}
	L=NULL;
}