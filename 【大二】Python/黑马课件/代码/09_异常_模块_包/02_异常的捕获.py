"""
演示捕获异常
"""

# 基本捕获语法
# try:
#     f = open("D:/abc.txt", "r", encoding="UTF-8")
# except:
#     print("出现异常了，因为文件不存在，我将open的模式，改为w模式去打开")
#     f = open("D:/abc.txt", "w", encoding="UTF-8")

# 捕获指定的异常
# try:
#     print(name)
#     # 1 / 0
# except NameError as e:
#     print("出现了变量未定义的异常")
#     print(e)
# 捕获多个异常
# try:
#     # 1 / 0
#     print(name)
# except (NameError, ZeroDivisionError) as e:
#     print("出现了变量未定义 或者 除以0的异常错误")
# 未正确设置捕获异常类型，将无法捕获异常

# 捕获所有异常
try:
    f = open("D:/123.txt", "r", encoding="UTF-8")
except Exception as e:
    print("出现异常了")
    f = open("D:/123.txt", "w", encoding="UTF-8")
else:
    print("好高兴，没有异常。")
finally:
    print("我是finally，有没有异常我都要执行")
    f.close()
