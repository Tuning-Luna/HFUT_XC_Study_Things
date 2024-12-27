# coding:utf8
from pyecharts.charts import Bar, Timeline
from pyecharts.globals import ThemeType
from pyecharts.options import *

f = open("D:/1960-2019全球GDP数据.csv", "r", encoding="GB2312")
lines = f.readlines()       # 读取全部数据行
lines.pop(0)                # 删除第一行
# 构建字典存数据，格式{年份:[["美国", 123], ["中国", 123], ......], 年份：[[], []], ......}
data_dict = {}
for line in lines:
    year = int(line.split(",")[0])
    country = line.split(",")[1]
    gdp = float(line.split(",")[2])
    try:
        data_dict[year].append((country, gdp))
    except KeyError:
        data_dict[year] = []
        data_dict[year].append((country, gdp))

sorted_year_list = sorted(data_dict.keys())         # 排序年份，从1960年开始
timeline = Timeline({"theme": ThemeType.LIGHT})     # 创建时间线，并设置主题
for year in sorted_year_list:                       # for循环，从1960年开始
    data_dict[year].sort(key=lambda element: element[1], reverse=True)
    year_data = data_dict[year][0:8]
    countrys = []
    gdps = []
    for country_gdp in year_data:
        countrys.append(country_gdp[0])
        gdps.append(int(country_gdp[1] / 100000000))
    bar = Bar()             # 创建柱状图
    countrys.reverse()      # 反转国家，让GDP最高的排在最上面
    gdps.reverse()          # 同步反转GDP数据
    # 设置标题
    bar.set_global_opts(title_opts=TitleOpts(title=f"{year}年全球前8 GDP国家"))
    bar.add_xaxis(countrys)             # 添加x轴数据
    bar.add_yaxis("GDP(亿)", gdps, label_opts=LabelOpts(position="right"))   # y轴数据，标签在右
    bar.reversal_axis()                 # 反转x和y轴
    timeline.add(bar, str(year))        # 时间线添加一个点，和对应的bar柱状图

# 设置自动播放
timeline.add_schema(
    play_interval=500,
    is_timeline_show=True,
    is_auto_play=True,
    is_loop_play=False
)
timeline.render("1960-2019全球GDP前8国家.html")       # 绘图
f.close()

