"""
任务2：基础问答功能
调用已部署的大模型API完成基础问答，支持对话模式
"""

import ollama


class SimpleQA:
    def __init__(self, model_name="qwen2:7b"):
        """
        初始化问答系统
        model_name: 使用的模型名称，默认使用qwen2:7b
        """
        self.model_name = model_name
        self.conversation_history = []

    def chat(self, question):
        """
        单次问答
        """
        try:
            response = ollama.chat(
                model=self.model_name, messages=[{"role": "user", "content": question}]
            )
            answer = response["message"]["content"]
            return answer
        except Exception as e:
            return f"错误: {str(e)}"

    def chat_with_history(self, question):
        """
        带历史上下文的对话
        """
        # 添加用户问题到历史
        self.conversation_history.append({"role": "user", "content": question})

        try:
            # 调用模型，传入完整对话历史
            response = ollama.chat(
                model=self.model_name, messages=self.conversation_history
            )
            answer = response["message"]["content"]

            # 添加模型回答到历史
            self.conversation_history.append({"role": "assistant", "content": answer})

            return answer
        except Exception as e:
            return f"错误: {str(e)}"

    def clear_history(self):
        """清空对话历史"""
        self.conversation_history = []


def main():
    """测试基础问答功能"""
    print("=" * 50)
    print("基础问答功能测试")
    print("=" * 50)

    # 初始化问答系统
    qa = SimpleQA()

    print("\n【单次问答测试】")
    question1 = "什么是人工智能？"
    print(f"问题: {question1}")
    answer1 = qa.chat(question1)
    print(f"回答: {answer1}\n")

    print("\n【对话模式测试】")
    print("开始多轮对话（输入 'quit' 退出）\n")

    while True:
        question = input("你: ")
        if question.lower() in ["quit", "exit", "退出"]:
            break

        answer = qa.chat_with_history(question)
        print(f"AI: {answer}\n")


if __name__ == "__main__":
    main()
