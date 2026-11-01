//用线性表最后一个元素的下标last代替listLen实现顺序表

#define MAXLEN 100

typedef int elementType;

typedef struct sllLast
{
	elementType data[MAXLEN];
	int last;
}seqList;

//初始化
void initialList(seqList &S)
{
	S.last=-1;
}

//求表长度
int listLength(seqList S)
{
	return S.last+1;
}

//按序号取元素
bool getElement(seqList S,int i,elementType &x)
{ 
	if(i<1 || i>S.last+1)    //i为元素编号，有效范围在1--S.last+1之间
		return false;
	else
	{
		x=S.data[i-1];
		return true;	
	}
}

//查找元素x，成功：返回元素编号；失败：返回0
int listLocate(seqList S,elementType x)
{
	int i;
	for(i=0;i<=S.last;i++)
	{
		if(S.data[i]==x)
			return i+1;   //找到，转换为元素编号输出	
	}
	return 0;
}

//插入元素
int listInsert(seqList &S,elementType x, int i)
{
	int k;
	if(S.last>MAXLEN-1)
		return 0;  //表满，返回0
	else if(i<1 || i>S.last+2)
		return 1;  //插入位置查处范围，返回1
	else
	{
		for(k=S.last;k>=i-1;k--)
			S.data[k+1]=S.data[k];
		S.data[i-1]=x;
		S.last++;
		return 2;	
	}
}

//删除元素
int listDelete(seqList &S,int i)
{
	int k;
	if(S.last==-1)
		return 0;   //空表，返回0
	else if(i<1 || i>S.last+1)
		return 1;   //删除元素编号超出范围，返回1
	else
	{
		for(k=i;k<=S.last;k++)
			S.data[k-1]=S.data[k];
		S.last--;
		return 2;	
	}
}

//7. 打印表中所有元素
void printList(seqList L)
{
	int i;
	for(i=0;i<=L.last;i++)
		cout<<L.data[i]<<"\t"; //元素之间以制表符分割
	cout<<endl;
}

//8. 交互输入数据元素--特殊输入结束
void listInputC(seqList &L)
{
	if(L.last>=0)
	{
		cout<<"顺序表已经存在，请先初始化，再输入元素。"<<endl;
		return;
	}
	
	elementType x;
	
	cout<<"请输入数据元素(整数，-9999退出):"<<endl;
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

//随机数创建顺序表
void rndCList(seqList &L)
{
	int i;
	int n,m;
	
	L.last=-1;
	cout<<"请输入要产生的随机数个数，n=";
	cin>>n;
	
	if(n>MAXLEN-1)
	{
		cout<<"您要求产生的随机数个数超出了查找表长度"<<MAXLEN-1<<"，创建顺序表失败。"<<endl;
		return;
	
	}	
	cout<<"请输入控制随机数大小参数，比如100以内数，请输入100，m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//产生随机数种子
	//srand((unsigned)GetTickCount());	//产生随机数种子
	for(i=0;i<n;i++)                //随机数写入排序表A[]
		L.data[i]=rand()%m;
    L.last=n-1;                      //表长度为n
	cout<<endl;
}
