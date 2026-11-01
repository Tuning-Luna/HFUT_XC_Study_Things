


//先序、中序序列交互创建二叉树
void createBiTree(btNode *& T, string &preStr,string &inStr, int i,int j,int len)
{		//i子树先序起始下标；j子树中序起始下标，len当前子树序列的长度(子树结点数) 
	int m=inStr.find(preStr[i]);	//查找子树根结点在中序中位置 
	if(m==-1) 	return;	//创建失败	
	if(len>0)
	{		
		T=new btNode;		//创建新结点 
		T->data=preStr[i];
		T->lChild=NULL;
		T->rChild=NULL;
		createBiTree(T->lChild,preStr,inStr,i+1,j,m-j);		//创建左子树
		createBiTree(T->rChild,preStr,inStr,i+(m-j)+1,m+1,len-1-(m-j));	//创建右子树
			//m-j左子树结点数（左子树序列长度）。
			//len-1-(m-j)右子树结点数（右子树序列长度），创建右子树中的+1、-1是计入根结点的位置 		
	}
}

//先序、中序序列文件读入创建
bool readFile(string fName,string &preStr, string &inStr)
{
	ifstream ifs;	//输入输出文件流
	ifs.open(fName,ios::in) ;
	if (!ifs.is_open())
    {
        cout << "文件打开失败" << endl;
        return false;
    }
    getline(ifs,preStr);	//读入先序序列
	getline(ifs,inStr);		//读入中序序列 
	
	ifs.close();
	return true;
 } 
 
//后序、中序序列交互创建二叉树
void creBTpos(btNode *& T, string &posStr,string &inStr, int i,int j,int len)
{		//i子树后序最后下标；j子树中序起始下标，len当前子树序列的长度(子树结点数) 
	int m=inStr.find(posStr[i]);	//查找子树根结点在中序中位置 
	if(m==-1) 	return;	//创建失败	
	if(len>0)
	{		
		T=new btNode;		//创建新结点 
		T->data=posStr[i];
		T->lChild=NULL;
		T->rChild=NULL;				
		creBTpos(T->lChild,posStr,inStr,i-(len-(m-j)-1)-1,j,m-j);	//创建左子树
		creBTpos(T->rChild,posStr,inStr,i-1,m+1,len-(m-j)-1);		//创建右子树
			//m-j左子树结点数（左子树序列长度）。
			//len-(m-j)-1右子树结点数（右子树序列长度），创建右子树中的+1、-1是计入根结点的位置 		
	}
}

//方式一：一行读入信息到数组。先序、孩子信息交互创建二叉树--一行输入 
void crePreChi(btNode *& T, string preStr, int &i,int len )
{	
	if(i>=len)	return;
	if(preStr[i]=='/'){
		i++;	//控制读取下一个结点 
		return;    //空树，退出
	}		
	T=new btNode;
	T->data=preStr[i];
	T->lChild=NULL;
	T->rChild=NULL;
	i++;	//控制读取下一个结点
	crePreChi(T->lChild,preStr,i,len);
	crePreChi(T->rChild,preStr,i,len);	
}  
//方式二:分次读入信息。先序、孩子信息交互创建二叉树
void crePreChi1(btNode *& T )
{	
	ElemType x;	 
	cin>>x;
	if(x=='/')
		return;    //空树，退出
	T=new btNode;
	T->data=x;
	T->lChild=NULL;
	T->rChild=NULL;
	crePreChi1(T->lChild);
	crePreChi1(T->rChild);	
} 


//补全为完全二叉树然后创建 
void creBtCom(btNode* &T, string &comStr, int i, int n)
{
	//T--为根结点指针
	//comStr--为按完全二叉树存储的树的结点数组
	//i--当前结点序号
	//n--二叉树结点总数
	if(i>=n)
		return;
	if(comStr[i]!='/')       //有效数据创建结点
	{
		T=new btNode;       //创建根结点
		T->data=comStr[i];   //结点赋值
		T->lChild=NULL;
		T->rChild=NULL;
	}
	creBtCom(T->lChild, comStr, 2*i+1, n);  //递归创建T的左子树
	creBtCom(T->rChild, comStr, 2*i+2, n); 	//递归创建T的右子树
}

 

