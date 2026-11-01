"""
演示序列的切片的课后练习
"万过薪月，员序程马黑来，nohtyP学"
"""
my_str = "万过薪月，员序程马黑来，nohtyP学"
# 倒序字符串，切片取出
result1 = my_str[::-1][9:14]
print(f"方式1结果：{result1}")
# 切片取出，然后倒序
result2 = my_str[5:10][::-1]
print(f"方式2结果：{result2}")

# split分隔"，"  replace替换"来"为空，倒序字符串
result3 = my_str.split("，")[1].replace("来", "")[::-1]
print(f"方式3结果：{result3}")
