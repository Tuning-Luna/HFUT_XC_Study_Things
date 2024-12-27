"""
演示文件的追加写入
"""

# 打开文件，不存在的文件
# f = open("D:/test.txt", "a", encoding="UTF-8")
# # write写入
# f.write("黑马程序员")
# # flush刷新
# f.flush()
# # close关闭
# f.close()
# 打开一个存在的文件
f = open("D:/test.txt", "a", encoding="UTF-8")
# write写入、flush刷新
f.write("\n月薪过万")
# close关闭
f.close()
