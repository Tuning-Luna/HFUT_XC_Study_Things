"""
演示异常、模块、包的综合案例练习
"""
# 创建my_utils 包， 在包内创建：str_util.py 和 file_util.py 2个模块，并提供相应的函数

import my_utils.str_util
from my_utils import file_util

print(my_utils.str_util.str_reverse("黑马程序员"))
print(my_utils.str_util.substr("itheima", 0, 4))


file_util.append_to_file("D:/test_append.txt", "itheima")
file_util.print_file_info("D:/test_append.txt")
