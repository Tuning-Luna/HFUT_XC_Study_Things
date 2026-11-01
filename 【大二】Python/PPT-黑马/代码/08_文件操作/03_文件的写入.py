"""
演示文件的写入
"""

# 打开文件，不存在的文件, r, w, a
import time

# f = open("D:/test.txt", "w", encoding="UTF-8")
# # write写入
# f.write("Hello World!!!")       # 内容写入到内存中
# # flush刷新
# # f.flush()                       # 将内存中积攒的内容，写入到硬盘的文件中
# # close关闭
# f.close()                       # close方法，内置了flush的功能的
# 打开一个存在的文件
f = open("D:/test.txt", "w", encoding="UTF-8")
# write写入、flush刷新
f.write("黑马程序员")
# close关闭
f.close()

