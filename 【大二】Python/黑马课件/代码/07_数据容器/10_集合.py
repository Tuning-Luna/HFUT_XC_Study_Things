"""
演示数据容器集合的使用
"""

# 定义集合
my_set = {"传智教育", "黑马程序员", "itheima", "传智教育", "黑马程序员", "itheima", "传智教育", "黑马程序员", "itheima"}
my_set_empty = set()        # 定义空集合
print(f"my_set的内容是：{my_set}, 类型是：{type(my_set)}")
print(f"my_set_empty的内容是：{my_set_empty}, 类型是：{type(my_set_empty)}")

# 添加新元素
my_set.add("Python")
my_set.add("传智教育")      #
print(f"my_set添加元素后结果是：{my_set}")
# 移除元素
my_set.remove("黑马程序员")
print(f"my_set移除黑马程序员后，结果是：{my_set}")
# 随机取出一个元素
my_set = {"传智教育", "黑马程序员", "itheima"}
element = my_set.pop()
print(f"集合被取出元素是：{element}, 取出元素后：{my_set}")

# 清空集合, clear
my_set.clear()
print(f"集合被清空啦，结果是：{my_set}")

# 取2个集合的差集
set1 = {1, 2, 3}
set2 = {1, 5, 6}
set3 = set1.difference(set2)
print(f"取出差集后的结果是：{set3}")
print(f"取差集后，原有set1的内容：{set1}")
print(f"取差集后，原有set2的内容：{set2}")


# 消除2个集合的差集
set1 = {1, 2, 3}
set2 = {1, 5, 6}
set1.difference_update(set2)
print(f"消除差集后，集合1结果：{set1}")
print(f"消除差集后，集合2结果：{set2}")

# 2个集合合并为1个
set1 = {1, 2, 3}
set2 = {1, 5, 6}
set3 = set1.union(set2)
print(f"2集合合并结果：{set3}")
print(f"合并后集合1：{set1}")
print(f"合并后集合2：{set2}")

# 统计集合元素数量len()
set1 = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5}
num = len(set1)
print(f"集合内的元素数量有：{num}个")

# 集合的遍历
# 集合不支持下标索引，不能用while循环
# 可以用for循环
set1 = {1, 2, 3, 4, 5}
for element in set1:
    print(f"集合的元素有：{element}")



