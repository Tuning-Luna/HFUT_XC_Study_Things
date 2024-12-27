"""
演示字符串的三种定义方式：
- 单引号定义法
- 双引号定义法
- 三引号定义法
"""

# 单引号定义法，使用单引号进行包围
name = '黑马程序员'
print(type(name))
# 双引号定义法
name = "黑马程序员"
print(type(name))
# 三引号定义法，写法和多行注释是一样的
name = """
我是
黑马
程序员
"""
print(type(name))


# 在字符串内 包含双引号
name = '"黑马程序员"'
print(name)
# 在字符串内 包含单引号
name = "'黑马程序员'"
print(name)
# 使用转义字符 \ 解除引号的效用
name = "\"黑马程序员\""
print(name)
name = '\'黑马程序员\''
print(name)

