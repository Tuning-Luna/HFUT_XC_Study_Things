-- ============================================================================
-- 实验一：数据更新与删除操作（步骤7续）
-- 数据库：jwgl2023217273
-- ============================================================================

USE jwgl2023217273;

-- ----------------------------------------------------------------------------
-- 1. 将班级号为 'CS01' 且年龄小于20岁的学生的班级号改为 'CS02'
-- 使用 UPDATE 语句，WHERE 条件限定目标行
-- ----------------------------------------------------------------------------
-- 查看修改前的数据
SELECT Sno, Sname, Sage, ClsNO FROM Student WHERE ClsNO = 'CS01';

-- 执行更新
UPDATE Student
SET ClsNO = 'CS02'
WHERE ClsNO = 'CS01' AND Sage < 20;

-- 查看修改后的数据
SELECT Sno, Sname, Sage, ClsNO FROM Student;

-- ----------------------------------------------------------------------------
-- 2. 删除所有年龄大于20岁且班级号为 'CS02' 的学生的记录
-- 注意：DELETE 会同时删除 SC 表中关联的选课记录（ON DELETE CASCADE）
-- ----------------------------------------------------------------------------
-- 查看删除前的数据
SELECT * FROM Student WHERE Sage > 20 AND ClsNO = 'CS02';

-- 执行删除
DELETE FROM Student
WHERE Sage > 20 AND ClsNO = 'CS02';

-- 查看删除后的数据
SELECT * FROM Student;

-- ----------------------------------------------------------------------------
-- 3. 插入一条新记录
-- 学号: 20171101, 姓名: 张三, 性别: 男, 年龄: 19, 班级编号: CS01
-- ----------------------------------------------------------------------------
INSERT INTO Student (Sno, Sname, Ssex, Sage, ClsNO) VALUES
    ('20171101', '张三', '男', 19, 'CS01');

-- 验证插入结果
SELECT * FROM Student;

-- ----------------------------------------------------------------------------
-- 4. 将年龄最小的学生的家庭地址去掉（设为 NULL）
-- 使用子查询找出最小年龄，可能有多名学生同龄
-- ----------------------------------------------------------------------------
-- 查看修改前的数据
SELECT Sno, Sname, Sage, Saddr FROM Student ORDER BY Sage ASC;

-- 执行更新：将年龄等于最小年龄的所有学生的地址设为 NULL
UPDATE Student
SET Saddr = NULL
WHERE Sage = (SELECT MIN(Sage) FROM Student);

-- 查看修改后的数据
SELECT Sno, Sname, Sage, Saddr FROM Student ORDER BY Sage ASC;

-- ----------------------------------------------------------------------------
-- 5. 将平均年龄最小的班级编号改为 'GL01'
-- 步骤：先计算每个班级的平均年龄 → 找出最小平均值 → 更新对应班级号
-- ----------------------------------------------------------------------------
-- 查看修改前各班级的平均年龄
SELECT ClsNO, AVG(Sage) AS AvgAge FROM Student GROUP BY ClsNO;

-- 执行更新（使用子查询找出平均年龄最小的班级）
UPDATE Class
SET ClsNO = 'GL01'
WHERE ClsNO = (
    SELECT ClsNO
    FROM (
        SELECT ClsNO, AVG(Sage) AS AvgAge
        FROM Student
        GROUP BY ClsNO
        ORDER BY AvgAge ASC
        LIMIT 1
    ) AS min_avg_class
);

-- 注意：由于 Student.ClsNO 有外键约束 ON UPDATE CASCADE，
-- Class 表 ClsNO 的变更会自动级联更新 Student 表中的对应值

-- 查看修改后的结果
SELECT * FROM Class;
SELECT Sno, Sname, ClsNO FROM Student;
