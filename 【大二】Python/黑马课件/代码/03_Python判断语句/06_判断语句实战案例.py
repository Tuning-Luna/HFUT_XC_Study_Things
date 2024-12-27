"""
演示判断语句的实战案例：终极猜数字
"""

# 1. 构建一个随机的数字变量
import random
num = random.randint(1, 10)

guess_num = int(input("输入你要猜测的数字："))

# 2. 通过if判断语句进行数字的猜测
if guess_num == num:
    print("恭喜，第一次就猜中了")
else:
    if guess_num > num:
        print("你猜测的数字大了")
    else:
        print("你猜测的数字小了")

    guess_num = int(input("再次输入你要猜测的数字："))

    if guess_num == num:
        print("恭喜，第二次猜中了")
    else:
        if guess_num > num:
            print("你猜测的数字大了")
        else:
            print("你猜测的数字小了")

        guess_num = int(input("第三次输入你要猜测的数字："))

        if guess_num == num:
            print("第三次猜中了")
        else:
            print("三次机会用完了，没有猜中。")
