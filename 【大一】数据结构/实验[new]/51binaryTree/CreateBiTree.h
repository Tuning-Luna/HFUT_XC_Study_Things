


//�����������н�������������
void createBiTree(btNode *& T, string &preStr,string &inStr, int i,int j,int len)
{		//i����������ʼ�±ꣻj����������ʼ�±꣬len��ǰ�������еĳ���(���������) 
	int m=inStr.find(preStr[i]);	//���������������������λ�� 
	if(m==-1) 	return;	//����ʧ��	
	if(len>0)
	{		
		T=new btNode;		//�����½�� 
		T->data=preStr[i];
		T->lChild=NULL;
		T->rChild=NULL;
		createBiTree(T->lChild,preStr,inStr,i+1,j,m-j);		//����������
		createBiTree(T->rChild,preStr,inStr,i+(m-j)+1,m+1,len-1-(m-j));	//����������
			//m-j����������������������г��ȣ���
			//len-1-(m-j)����������������������г��ȣ��������������е�+1��-1�Ǽ��������λ�� 		
	}
}

//�������������ļ����봴��
bool readFile(string fName,string &preStr, string &inStr)
{
	ifstream ifs;	//��������ļ���
	ifs.open(fName,ios::in) ;
	if (!ifs.is_open())
    {
        cout << "�ļ���ʧ��" << endl;
        return false;
    }
    getline(ifs,preStr);	//������������
	getline(ifs,inStr);		//������������ 
	
	ifs.close();
	return true;
 } 
 
//�����������н�������������
void creBTpos(btNode *& T, string &posStr,string &inStr, int i,int j,int len)
{		//i������������±ꣻj����������ʼ�±꣬len��ǰ�������еĳ���(���������) 
	int m=inStr.find(posStr[i]);	//���������������������λ�� 
	if(m==-1) 	return;	//����ʧ��	
	if(len>0)
	{		
		T=new btNode;		//�����½�� 
		T->data=posStr[i];
		T->lChild=NULL;
		T->rChild=NULL;				
		creBTpos(T->lChild,posStr,inStr,i-(len-(m-j)-1)-1,j,m-j);	//����������
		creBTpos(T->rChild,posStr,inStr,i-1,m+1,len-(m-j)-1);		//����������
			//m-j����������������������г��ȣ���
			//len-(m-j)-1����������������������г��ȣ��������������е�+1��-1�Ǽ��������λ�� 		
	}
}

//��ʽһ��һ�ж�����Ϣ�����顣���򡢺�����Ϣ��������������--һ������ 
void crePreChi(btNode *& T, string preStr, int &i,int len )
{	
	if(i>=len)	return;
	if(preStr[i]=='/'){
		i++;	//���ƶ�ȡ��һ����� 
		return;    //�������˳�
	}		
	T=new btNode;
	T->data=preStr[i];
	T->lChild=NULL;
	T->rChild=NULL;
	i++;	//���ƶ�ȡ��һ�����
	crePreChi(T->lChild,preStr,i,len);
	crePreChi(T->rChild,preStr,i,len);	
}  
//��ʽ��:�ִζ�����Ϣ�����򡢺�����Ϣ��������������
void crePreChi1(btNode *& T )
{	
	ElemType x;	 
	cin>>x;
	if(x=='/')
		return;    //�������˳�
	T=new btNode;
	T->data=x;
	T->lChild=NULL;
	T->rChild=NULL;
	crePreChi1(T->lChild);
	crePreChi1(T->rChild);	
} 


//��ȫΪ��ȫ������Ȼ�󴴽� 
void creBtCom(btNode* &T, string &comStr, int i, int n)
{
	//T--Ϊ�����ָ��
	//comStr--Ϊ����ȫ�������洢�����Ľ������
	//i--��ǰ������
	//n--�������������
	if(i>=n)
		return;
	if(comStr[i]!='/')       //��Ч���ݴ������
	{
		T=new btNode;       //���������
		T->data=comStr[i];   //��㸳ֵ
		T->lChild=NULL;
		T->rChild=NULL;
	}
	creBtCom(T->lChild, comStr, 2*i+1, n);  //�ݹ鴴��T��������
	creBtCom(T->rChild, comStr, 2*i+2, n); 	//�ݹ鴴��T��������
}

 

