"""
主动写一段错误代码，演示异常的出现
"""

# 通过open，读取一个不存在的文件
f = open("D:/abc.txt", "r", encoding="UTF-8")
