"""
演示if else练习题：我要买票吗
"""

# 定义键盘输入获取身高数据
height = int(input("请输入你的身高（cm）："))

# 通过if进行判断
if height > 120:
    print("您的身高超出120CM，需要买票，10元。")
else:
    print("您的身高低于120CM，可以免费游玩。")

print("祝您游玩愉快")

