"""
演示集合的课后练习题
my_list = ['黑马程序员', '传智播客', '黑马程序员', '传智播客',
    'itheima', 'itcast', 'itheima', 'itcast', 'best']
"""
my_list = ['黑马程序员', '传智播客', '黑马程序员', '传智播客',
    'itheima', 'itcast', 'itheima', 'itcast', 'best']

# 定义一个空集合
my_set = set()

# 通过for循环遍历列表
for element in my_list:
    # 在for循环中将列表的元素添加至集合
    my_set.add(element)

# 最终得到元素去重后的集合对象，并打印输出
print(f"列表的内容是：{my_list}")
print(f"通过for循环后，得到的集合对象是：{my_set}")
