//�����Ա����һ��Ԫ�ص��±�last����listLenʵ��˳���

#define MAXLEN 100

typedef int elementType;

typedef struct sllLast
{
	elementType data[MAXLEN];
	int last;
}seqList;

//��ʼ��
void initialList(seqList &S)
{
	S.last=-1;
}

//�����
int listLength(seqList S)
{
	return S.last+1;
}

//�����ȡԪ��
bool getElement(seqList S,int i,elementType &x)
{ 
	if(i<1 || i>S.last+1)    //iΪԪ�ر�ţ���Ч��Χ��1--S.last+1֮��
		return false;
	else
	{
		x=S.data[i-1];
		return true;	
	}
}

//����Ԫ��x���ɹ�������Ԫ�ر�ţ�ʧ�ܣ�����0
int listLocate(seqList S,elementType x)
{
	int i;
	for(i=0;i<=S.last;i++)
	{
		if(S.data[i]==x)
			return i+1;   //�ҵ���ת��ΪԪ�ر�����	
	}
	return 0;
}

//����Ԫ��
int listInsert(seqList &S,elementType x, int i)
{
	int k;
	if(S.last>MAXLEN-1)
		return 0;  //����������0
	else if(i<1 || i>S.last+2)
		return 1;  //����λ�ò鴦��Χ������1
	else
	{
		for(k=S.last;k>=i-1;k--)
			S.data[k+1]=S.data[k];
		S.data[i-1]=x;
		S.last++;
		return 2;	
	}
}

//ɾ��Ԫ��
int listDelete(seqList &S,int i)
{
	int k;
	if(S.last==-1)
		return 0;   //�ձ�����0
	else if(i<1 || i>S.last+1)
		return 1;   //ɾ��Ԫ�ر�ų�����Χ������1
	else
	{
		for(k=i;k<=S.last;k++)
			S.data[k-1]=S.data[k];
		S.last--;
		return 2;	
	}
}

//7. ��ӡ��������Ԫ��
void printList(seqList L)
{
	int i;
	for(i=0;i<=L.last;i++)
		cout<<L.data[i]<<"\t"; //Ԫ��֮�����Ʊ���ָ�
	cout<<endl;
}

//8. ������������Ԫ��--�����������
void listInputC(seqList &L)
{
	if(L.last>=0)
	{
		cout<<"˳����Ѿ����ڣ����ȳ�ʼ����������Ԫ�ء�"<<endl;
		return;
	}
	
	elementType x;
	
	cout<<"����������Ԫ��(������-9999�˳�):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999)
	{
		L.last++;
		L.data[L.last]=x;		

		cout<<"x=";
		cin>>x;
	}
}

//���������˳���
void rndCList(seqList &L)
{
	int i;
	int n,m;
	
	L.last=-1;
	cout<<"������Ҫ�����������������n=";
	cin>>n;
	
	if(n>MAXLEN-1)
	{
		cout<<"��Ҫ���������������������˲��ұ���"<<MAXLEN-1<<"������˳���ʧ�ܡ�"<<endl;
		return;
	
	}	
	cout<<"����������������С����������100��������������100��m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//�������������
	//srand((unsigned)GetTickCount());	//�������������
	for(i=0;i<n;i++)                //�����д�������A[]
		L.data[i]=rand()%m;
    L.last=n-1;                      //����Ϊn
	cout<<endl;
}
