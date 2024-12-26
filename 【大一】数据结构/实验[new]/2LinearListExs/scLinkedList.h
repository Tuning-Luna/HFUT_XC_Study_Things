//��ѭ�������弰�����㷨

typedef int elementType;

typedef struct scNode
{
	elementType    data;
	struct scNode *next;
}node;

//��ʼ����ѭ������
void initialList(node *&L)
{
	L=new node;
	L->next=L;
}

//�����
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

//�����ȡԪ��
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

//��Ԫ��ֵ����Ԫ��
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

//����Ԫ��
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
	if(p->next==L && j!=i-1)   //p->next==L��j==i-1ʱ������λ���ԺϷ������鵽���
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

//ɾ��Ԫ��
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
		return false;    //ɾ��λ�� i ������Χ��ɾ��ʧ�ܣ�����false
	else
	{    //��ʱ��j==i-1��pָ��ai-1��㣬p->nextָҪɾ����Ŀ����ai
		u=p->next;
		p->next=u->next;  //���Ŀ���㣬��p->next=p->next->next;
		delete u;
		return true;	
	}
}

//ͷ�巨����������ѭ������
void createSCLLH(node *&L)
{
	node *p;
	elementType x;

	L=new node;
	L->next=L;     //�γ�ѭ��

	cout<<"������Ԫ�����ݣ�������-9999�˳���:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //��̬�����ڴ棬�����½ڵ�
		p->data=x;  //д��Ԫ������
		
		p->next=L->next;     //ͷ�������
		L->next=p;        
		
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

		p->next=L->next;      //ͷ������½��
		L->next=p;
    }
	cout<<endl;
}
//�����ͷ�巨�����������------------------------------------------------------------------


//β�巨����������ѭ������
void createSCLLR(node *&L)
{
	node *p,*R;
	elementType x;

	L=new node;
	L->next=L;     //�γ�ѭ��
	R=L;           //��βָ��

	cout<<"������Ԫ�����ݣ�������-9999�˳���:"<<endl;
	cout<<"x=";
	cin>>x;	
	while(x!=-9999)
	{   
		p=new node;  //��̬�����ڴ棬�����½ڵ�
		p->data=x;  //д��Ԫ������
		
		R->next=p;     //β�������
		R=p;        
		
		cout<<"x=";
		cin>>x;
	}
	R->next=L;     //�γ�ѭ��
}


//���ٵ�ѭ������
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
//��ӡ��ѭ������
void printList(node* &L)
{
	node *p;
	//cout<<"��ǰ����Ϊ��"<<listLength(L)<<endl;
	cout<<"��ǰ����Ԫ�أ�"<<endl;		
	p=L->next;
	while(p!=L)
	{
		cout<<p->data<<"\t";
		p=p->next;
	}
	cout<<endl;
}

