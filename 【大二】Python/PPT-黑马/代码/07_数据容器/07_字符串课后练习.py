"""
字符串课后练习演示
"itheima itcast boxuegu"
"""
my_str = "itheima itcast boxuegu"
# 统计字符串内有多少个"it"字符
num = my_str.count("it")
print(f"字符串{my_str}中有{num}个it字符")
# 将字符串内的空格，全部替换为字符："|"
new_my_str = my_str.replace(" ", "|")
print(f"字符串{my_str}被替换空格后，结果是：{new_my_str}")
# 并按照"|"进行字符串分割，得到列表
my_str_list = new_my_str.split("|")
print(f"字符串{new_my_str}按照|分割后结果是：{my_str_list}")
