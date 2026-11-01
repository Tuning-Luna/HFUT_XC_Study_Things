//带头结点单链表头文件
//函数结果状态代码
 #define OK 1 
 #define ERROR 0 
 #define OVERFLOW -2 
 #define MAXSIZE 100  //定义线性表的最大容量
 #define NODENUM  100       //定义最大结点数
 	 
typedef  int  Status;	//Status是函数返回值类型，其值是函数结果状态代码。 
typedef char ElemType;	//定义数据元素的数据类型 

typedef struct node
{
	ElemType data;
	struct node* lChild, *rChild;
}btNode;

//递归先序遍历
void preOrder(btNode *T)
{
	if(T)
	{
		cout<<T->data<<" ";      //访问根结点。打印当前结点元素值，替代visit(T)函数
		preOrder(T->lChild);     //先序遍历左子树
		preOrder(T->rChild);     //先序遍历右子树
	}
}

//中序遍历--递归
void inOrder(btNode *T)
{
	if(T)
	{		
		inOrder(T->lChild);      //中序遍历左子树
		cout<<T->data<<" ";      //访问根结点。打印当前结点元素值，替代visit(T)函数
		inOrder(T->rChild);      //中序遍历右子树
	}
}

//后序遍历--递归
void postOrder(btNode *T)
{
	if(T)
	{		
		postOrder(T->lChild);      //后序遍历左子树		
		postOrder(T->rChild);      //后序遍历右子树
		cout<<T->data<<" ";        //访问根结点。打印当前结点元素值，替代visit(T)函数
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

//层次遍历--使用C++STL的queue实现 
void hieOrder(btNode *T)
{
	btNode* p;
	queue<btNode*> Q;	
	Q.push(T);
	while(!Q.empty())
	{
		p=Q.front();	//取队头 
		cout<<p->data<<" ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
		if(p->lChild)
			Q.push(p->lChild);	//左孩子入队 
		if(p->rChild)
			Q.push(p->rChild);	//右孩子入队 
		Q.pop();	//队头出队	
	}
}

//非递归先序遍历
void preOrderNR(btNode *T) 
{
	stack<btNode*> S;	
	while(!S.empty() || T)
	{
		if(T){	//访问根结点，先序遍历左子树
			cout<<T->data<<" ";
			S.push(T);		//根结点入栈 
			T=T->lChild; 	//遍历左子树			
		}
		else{	//遍历右子树
			T=S.top();	//取栈顶，即子树根结点指针，准备遍历右子树
			S.pop();
			T=T->rChild;	//遍历右子树			
		}
	}
}

//非递归中序遍历
void inOrderNR(btNode* T)
{
	stack<btNode*> S;
	while(T || !S.empty())
	{
		if(T)	//遍历左子树
		{
			S.push(T);	//根结点入栈
			T=T->lChild; 
		 }
		else{
			T=S.top();	//取栈顶，子树根结点，准备遍历右子树
			cout<<T->data<<" ";	//访问根结点
			S.pop();	//根结点出栈
			T=T->rChild;	//进入右子树 
		} 
	}
 }
 
//非递归后序遍历
void postOrderNR(btNode* T)
{
	short tag[NODENUM];	//标记处理左右子树
	int i=0;
	stack<btNode*> S;
	while(T || !S.empty())  {
		if(T)	//处理左子树
		{
			S.push(T);	//根结点入栈
			tag[S.size()-1]=0;	//标记已遍历左子树
			T=T->lChild; 
		 } 
		 else{	//遍历右子树、访问根结点
		 	T=S.top();
			if(tag[S.size()-1]==0){
				tag[S.size()-1]=1;
				T=T->rChild;	//遍历右子树 
			} 
			else{	//访问根结点，退栈
				T=S.top();
				cout<<T->data<<" ";
				S.pop();
				T=NULL;		//强制返回父结点，在栈顶			
			}		 	
		 }
	}	
 } 




