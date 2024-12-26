//��ͷ��㵥����ͷ�ļ�
//�������״̬����
 #define OK 1 
 #define ERROR 0 
 #define OVERFLOW -2 
 #define MAXSIZE 100  //�������Ա���������
 #define NODENUM  100       //�����������
 	 
typedef  int  Status;	//Status�Ǻ�������ֵ���ͣ���ֵ�Ǻ������״̬���롣 
typedef char ElemType;	//��������Ԫ�ص��������� 

typedef struct node
{
	ElemType data;
	struct node* lChild, *rChild;
}btNode;

//�ݹ��������
void preOrder(btNode *T)
{
	if(T)
	{
		cout<<T->data<<" ";      //���ʸ���㡣��ӡ��ǰ���Ԫ��ֵ�����visit(T)����
		preOrder(T->lChild);     //�������������
		preOrder(T->rChild);     //�������������
	}
}

//�������--�ݹ�
void inOrder(btNode *T)
{
	if(T)
	{		
		inOrder(T->lChild);      //�������������
		cout<<T->data<<" ";      //���ʸ���㡣��ӡ��ǰ���Ԫ��ֵ�����visit(T)����
		inOrder(T->rChild);      //�������������
	}
}

//�������--�ݹ�
void postOrder(btNode *T)
{
	if(T)
	{		
		postOrder(T->lChild);      //�������������		
		postOrder(T->rChild);      //�������������
		cout<<T->data<<" ";        //���ʸ���㡣��ӡ��ǰ���Ԫ��ֵ�����visit(T)����
	}
}

void destroyBt(btNode *&T)
{
	if(T)
	{
		destroyBt(T->lChild);
		destroyBt(T->rChild);
		delete T;
	}
	T=NULL;
}

//��α���--ʹ��C++STL��queueʵ�� 
void hieOrder(btNode *T)
{
	btNode* p;
	queue<btNode*> Q;	
	Q.push(T);
	while(!Q.empty())
	{
		p=Q.front();	//ȡ��ͷ 
		cout<<p->data<<" ";         //���ʸ���㡣��ӡ��ǰ���Ԫ��ֵ�����visit(T)����
		if(p->lChild)
			Q.push(p->lChild);	//������� 
		if(p->rChild)
			Q.push(p->rChild);	//�Һ������ 
		Q.pop();	//��ͷ����	
	}
}

//�ǵݹ��������
void preOrderNR(btNode *T) 
{
	stack<btNode*> S;	
	while(!S.empty() || T)
	{
		if(T){	//���ʸ���㣬�������������
			cout<<T->data<<" ";
			S.push(T);		//�������ջ 
			T=T->lChild; 	//����������			
		}
		else{	//����������
			T=S.top();	//ȡջ���������������ָ�룬׼������������
			S.pop();
			T=T->rChild;	//����������			
		}
	}
}

//�ǵݹ��������
void inOrderNR(btNode* T)
{
	stack<btNode*> S;
	while(T || !S.empty())
	{
		if(T)	//����������
		{
			S.push(T);	//�������ջ
			T=T->lChild; 
		 }
		else{
			T=S.top();	//ȡջ������������㣬׼������������
			cout<<T->data<<" ";	//���ʸ����
			S.pop();	//������ջ
			T=T->rChild;	//���������� 
		} 
	}
 }
 
//�ǵݹ�������
void postOrderNR(btNode* T)
{
	short tag[NODENUM];	//��Ǵ�����������
	int i=0;
	stack<btNode*> S;
	while(T || !S.empty())  {
		if(T)	//����������
		{
			S.push(T);	//�������ջ
			tag[S.size()-1]=0;	//����ѱ���������
			T=T->lChild; 
		 } 
		 else{	//���������������ʸ����
		 	T=S.top();
			if(tag[S.size()-1]==0){
				tag[S.size()-1]=1;
				T=T->rChild;	//���������� 
			} 
			else{	//���ʸ���㣬��ջ
				T=S.top();
				cout<<T->data<<" ";
				S.pop();
				T=NULL;		//ǿ�Ʒ��ظ���㣬��ջ��			
			}		 	
		 }
	}	
 } 




