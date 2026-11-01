"""
演示嵌套应用for循环
"""

# 坚持表白100天，每天都送10朵花
# range
i = 0
for i in range(1, 101):
    print(f"今天是向小美表白的第{i}天，加油坚持。")

    # 写内层的循环了
    for j in range(1, 11):
        print(f"给小美送的第{j}朵玫瑰花")

    print("小美我喜欢你")

print(f"第{i}天，表白成功")



