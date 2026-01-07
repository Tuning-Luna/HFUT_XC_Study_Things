"""
任务3：基于LangChain实现RAG（检索增强生成）
使用FAISS作为向量数据库，结合本地大模型实现增强问答
"""

import os
from langchain_community.document_loaders import PyPDFLoader, TextLoader
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_community.embeddings import HuggingFaceEmbeddings
from langchain_community.vectorstores import FAISS
from langchain.prompts import PromptTemplate
from langchain_community.llms import Ollama
from langchain.chains import RetrievalQA


class RAGSystem:
    def __init__(
        self,
        model_name="qwen2:7b",
        embedding_model="sentence-transformers/paraphrase-multilingual-MiniLM-L12-v2",
    ):
        """
        初始化RAG系统
        model_name: 使用的LLM模型名称
        embedding_model: 用于向量化的模型
        """
        self.model_name = model_name
        self.embedding_model = embedding_model

        # 初始化嵌入模型（用于将文本转换为向量）
        print("正在加载嵌入模型...")
        self.embeddings = HuggingFaceEmbeddings(
            model_name=embedding_model,
            model_kwargs={"device": "cpu"},  # 如果有GPU可以改为'cuda'
        )

        # 初始化LLM
        print("正在初始化大语言模型...")
        self.llm = Ollama(model=model_name)

        self.vectorstore = None
        self.qa_chain = None

    def load_documents(self, file_path):
        """
        加载文档
        支持PDF和TXT格式
        """
        print(f"正在加载文档: {file_path}")

        if file_path.endswith(".pdf"):
            loader = PyPDFLoader(file_path)
        elif file_path.endswith(".txt"):
            loader = TextLoader(file_path, encoding="utf-8")
        else:
            raise ValueError("不支持的文件格式，请使用PDF或TXT文件")

        documents = loader.load()
        print(f"已加载 {len(documents)} 个文档页面")
        return documents

    def create_vectorstore(self, documents, chunk_size=500, chunk_overlap=50):
        """
        创建向量数据库
        documents: 文档列表
        chunk_size: 文本块大小
        chunk_overlap: 文本块重叠大小
        """
        print("正在分割文档...")
        # 分割文档为小块
        text_splitter = RecursiveCharacterTextSplitter(
            chunk_size=chunk_size,
            chunk_overlap=chunk_overlap,
            length_function=len,
        )
        texts = text_splitter.split_documents(documents)
        print(f"文档已分割为 {len(texts)} 个文本块")

        print("正在创建向量数据库...")
        # 创建FAISS向量数据库
        self.vectorstore = FAISS.from_documents(texts, self.embeddings)
        print("向量数据库创建完成！")

        return self.vectorstore

    def build_qa_chain(self):
        """
        构建问答链
        """
        if self.vectorstore is None:
            raise ValueError("请先创建向量数据库")

        # 定义提示词模板
        prompt_template = """使用以下上下文信息回答用户的问题。如果你不知道答案，就说不知道，不要编造答案。

上下文信息：
{context}

问题：{question}

请用中文回答："""

        PROMPT = PromptTemplate(
            template=prompt_template, input_variables=["context", "question"]
        )

        # 创建检索问答链
        self.qa_chain = RetrievalQA.from_chain_type(
            llm=self.llm,
            chain_type="stuff",
            retriever=self.vectorstore.as_retriever(
                search_kwargs={"k": 3}
            ),  # 检索前3个最相关的文档块
            chain_type_kwargs={"prompt": PROMPT},
            return_source_documents=True,
        )

        print("问答链构建完成！")

    def query(self, question):
        """
        查询问题
        """
        if self.qa_chain is None:
            raise ValueError("请先构建问答链")

        result = self.qa_chain({"query": question})
        return {
            "answer": result["result"],
            "source_documents": result["source_documents"],
        }

    def save_vectorstore(self, path="faiss_index"):
        """保存向量数据库"""
        if self.vectorstore:
            self.vectorstore.save_local(path)
            print(f"向量数据库已保存到: {path}")

    def load_vectorstore(self, path="faiss_index"):
        """加载向量数据库"""
        self.vectorstore = FAISS.load_local(
            path, self.embeddings, allow_dangerous_deserialization=True
        )
        print(f"向量数据库已从 {path} 加载")


def main():
    """测试RAG功能"""
    print("=" * 50)
    print("RAG增强问答系统")
    print("=" * 50)

    # 初始化RAG系统
    rag = RAGSystem()

    # 检查是否已有向量数据库
    if os.path.exists("faiss_index"):
        print("\n检测到已存在的向量数据库，正在加载...")
        rag.load_vectorstore()
        rag.build_qa_chain()
    else:
        # 加载文档（请将你的文档放在documents文件夹中）
        documents_dir = "documents"
        if not os.path.exists(documents_dir):
            os.makedirs(documents_dir)
            print(f"\n请将你的文档放入 {documents_dir} 文件夹中")
            print("支持的格式：PDF (.pdf) 或 文本 (.txt)")
            return

        # 加载所有文档
        all_documents = []
        for filename in os.listdir(documents_dir):
            file_path = os.path.join(documents_dir, filename)
            if filename.endswith((".pdf", ".txt")):
                try:
                    docs = rag.load_documents(file_path)
                    all_documents.extend(docs)
                except Exception as e:
                    print(f"加载 {filename} 时出错: {e}")

        if not all_documents:
            print(f"\n{documents_dir} 文件夹中没有找到文档")
            return

        # 创建向量数据库
        rag.create_vectorstore(all_documents)

        # 保存向量数据库
        rag.save_vectorstore()

        # 构建问答链
        rag.build_qa_chain()

    # 开始问答
    print("\n" + "=" * 50)
    print("开始问答（输入 'quit' 退出）")
    print("=" * 50 + "\n")

    while True:
        question = input("你: ")
        if question.lower() in ["quit", "exit", "退出"]:
            break

        print("\n正在检索和生成答案...")
        result = rag.query(question)

        print(f"\nAI: {result['answer']}")
        print(f"\n参考来源: {len(result['source_documents'])} 个相关文档块\n")


if __name__ == "__main__":
    main()
