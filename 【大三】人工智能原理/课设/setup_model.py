"""
任务1：本地化大模型部署
使用Ollama部署Qwen2模型
"""

import subprocess
import sys


def check_ollama_installed():
    """检查Ollama是否已安装"""
    try:
        result = subprocess.run(["ollama", "--version"], capture_output=True, text=True)
        return result.returncode == 0
    except FileNotFoundError:
        return False


def pull_model(model_name="qwen2:7b"):
    """拉取模型"""
    print(f"正在拉取模型: {model_name}")
    print("这可能需要一些时间，请耐心等待...")

    try:
        result = subprocess.run(["ollama", "pull", model_name], check=True, text=True)
        print(f"\n模型 {model_name} 拉取成功！")
        return True
    except subprocess.CalledProcessError as e:
        print(f"拉取模型失败: {e}")
        return False


def test_model(model_name="qwen2:7b"):
    """测试模型是否正常工作"""
    print(f"\n正在测试模型: {model_name}")

    try:
        import ollama

        response = ollama.chat(
            model=model_name,
            messages=[{"role": "user", "content": "你好，请简单介绍一下你自己"}],
        )
        answer = response["message"]["content"]
        print(f"\n模型响应: {answer}")
        print("\n✓ 模型测试成功！")
        return True
    except Exception as e:
        print(f"模型测试失败: {e}")
        return False


def main():
    print("=" * 50)
    print("本地大模型部署脚本")
    print("=" * 50)

    # 检查Ollama是否安装
    print("\n1. 检查Ollama安装状态...")
    if not check_ollama_installed():
        print("\n❌ Ollama未安装！")
        print("\n请先安装Ollama:")
        print("Windows: 访问 https://ollama.com/download 下载安装")
        print("Mac/Linux: curl -fsSL https://ollama.com/install.sh | sh")
        sys.exit(1)
    else:
        print("✓ Ollama已安装")

    # 拉取模型
    model_name = "qwen2:7b"
    print(f"\n2. 拉取模型: {model_name}")
    if not pull_model(model_name):
        sys.exit(1)

    # 测试模型
    print("\n3. 测试模型...")
    if not test_model(model_name):
        sys.exit(1)

    print("\n" + "=" * 50)
    print("部署完成！现在可以运行 simple_qa.py 和 rag_qa.py 了")
    print("=" * 50)


if __name__ == "__main__":
    main()
