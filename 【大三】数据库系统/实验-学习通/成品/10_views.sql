-- ============================================================================
-- 实验一：视图操作（步骤5·视图部分）
-- 数据库：jwgl2023217273
-- ============================================================================

USE jwgl2023217273;

-- ============================================================================
-- 1. 建立视图 S_C_GRADE：每个学生的学号、姓名、选修的课名及成绩
-- 三表 JOIN 视图，方便后续查询学生选课详情
-- ============================================================================
CREATE VIEW S_C_GRADE AS
SELECT S.Sno, S.Sname, C.Cname, SC.Grade
FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Course C ON SC.Cno = C.Cno;

-- 验证视图
SELECT * FROM S_C_GRADE;

-- ============================================================================
-- 2. 建立视图 COMPUTE_AVG_GRADE：所有计算机专业学生的学号、选修课程号及平均成绩
-- 使用窗口函数 AVG() OVER(PARTITION BY Sno) 计算每个学生的平均成绩
-- ============================================================================
CREATE VIEW COMPUTE_AVG_GRADE AS
SELECT S.Sno,
       SC.Cno,
       AVG(SC.Grade) OVER (PARTITION BY S.Sno) AS AvgGrade
FROM Student S
    JOIN Class C ON S.ClsNO = C.ClsNO
    JOIN SC ON S.Sno = SC.Sno
WHERE C.Specialty = '计算机应用';

-- 验证视图
SELECT * FROM COMPUTE_AVG_GRADE;

-- ============================================================================
-- 3. 删除视图 COMPUTE_AVG_GRADE
-- ============================================================================
DROP VIEW IF EXISTS COMPUTE_AVG_GRADE;

-- 验证已删除
SHOW FULL TABLES WHERE Table_type = 'VIEW';

-- ============================================================================
-- 4. 定义视图 V_NUM_AVG：各专业学生人数、平均年龄
-- ============================================================================
CREATE VIEW V_NUM_AVG AS
SELECT C.Specialty,
       COUNT(S.Sno)   AS StudentCount,
       AVG(S.Sage)     AS AvgAge
FROM Class C
    LEFT JOIN Student S ON C.ClsNO = S.ClsNO
GROUP BY C.Specialty;

-- 验证视图
SELECT * FROM V_NUM_AVG;

-- ============================================================================
-- 5. 定义视图 V_YEAR：反映学生出生年份
-- 出生年份 = 当前年份 - 年龄
-- 注意：此计算为近似值（忽略具体出生月和当前月份的关系）
-- ============================================================================
CREATE VIEW V_YEAR AS
SELECT Sno,
       Sname,
       Sage,
       (YEAR(CURDATE()) - Sage) AS BirthYear
FROM Student;

-- 验证视图
SELECT * FROM V_YEAR;

-- ============================================================================
-- 6. 定义视图 V_AVG_S_G：各位学生选修课程的门数及平均成绩
-- ============================================================================
CREATE VIEW V_AVG_S_G AS
SELECT Sno,
       COUNT(*)          AS CourseCount,
       AVG(Grade)        AS AvgGrade
FROM SC
GROUP BY Sno;

-- 验证视图
SELECT * FROM V_AVG_S_G;

-- ============================================================================
-- 7. 定义视图 V_AVG_C_G：各门课程的选修人数及平均成绩
-- ============================================================================
CREATE VIEW V_AVG_C_G AS
SELECT SC.Cno,
       C.Cname,
       COUNT(SC.Sno)     AS StudentCount,
       AVG(SC.Grade)     AS AvgGrade
FROM SC
    JOIN Course C ON SC.Cno = C.Cno
GROUP BY SC.Cno, C.Cname;

-- 验证视图
SELECT * FROM V_AVG_C_G;

-- ============================================================================
-- 8. 查询平均成绩为90分以上的学生学号、姓名和成绩
-- 利用视图 V_AVG_S_G 进行查询
-- ============================================================================
SELECT V.Sno, S.Sname, V.AvgGrade
FROM V_AVG_S_G V
    JOIN Student S ON V.Sno = S.Sno
WHERE V.AvgGrade >= 90;

-- ============================================================================
-- 9. 查询各课成绩均大于平均成绩的学生学号、姓名、课程和成绩
-- "各课成绩均大于平均成绩"的含义：
--   对某个学生而言，他选修的每一门课的成绩都超过了该门课的平均成绩
-- 使用 NOT EXISTS + 子查询：不存在任何一门课的成绩 ≤ 该课平均成绩
-- ============================================================================
SELECT S.Sno, S.Sname, C.Cname, SC.Grade
FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Course C ON SC.Cno = C.Cno
WHERE NOT EXISTS (
    -- 这个子查询查找该学生是否存在 ≤ 课程平均分的记录
    SELECT 1
    FROM SC SC2
        JOIN V_AVG_C_G V ON SC2.Cno = V.Cno
    WHERE SC2.Sno = S.Sno
        AND SC2.Grade <= V.AvgGrade
)
ORDER BY S.Sno, C.Cname;

-- ============================================================================
-- 10. 按专业统计各专业平均成绩在80分以上的人数，结果按降序排列
-- 步骤：先算每个学生的平均成绩 → 筛选>80 → 按专业分组统计 → 降序
-- ============================================================================
SELECT C.Specialty,
       COUNT(DISTINCT S.Sno) AS ExcellentStudentCount
FROM Student S
    JOIN Class C ON S.ClsNO = C.ClsNO
    JOIN V_AVG_S_G V ON S.Sno = V.Sno
WHERE V.AvgGrade >= 80
GROUP BY C.Specialty
ORDER BY ExcellentStudentCount DESC;

-- ============================================================================
-- 11. 定义信息专业学生基本情况视图 V_IS
-- ★ 注意 ★ 当前数据库中 Class 表的专业包括：计算机应用、数学、物理
--         没有"信息"专业的数据，因此 V_IS 查询结果将为空。
--         本脚本仍按题目要求定义视图。
-- 通过视图更新学生姓名：
--   UPDATE V_IS SET Sname = 'S1_MMM' WHERE Sno = 'S1';
--   UPDATE V_IS SET Sname = 'S4_MMM' WHERE Sno = 'S4';
-- ★ 注意 ★ 学号 'S1'、'S4' 是模板数据，实际数据中学号为8位数字（如20190101）。
--         若表中没有对应学号，UPDATE 将不会影响任何行。
-- ============================================================================

-- 创建视图 V_IS
CREATE VIEW V_IS AS
SELECT S.Sno, S.Sname, S.Ssex, S.Sage, S.ClsNO, S.Saddr
FROM Student S
    JOIN Class C ON S.ClsNO = C.ClsNO
WHERE C.Specialty = '信息';

-- 验证视图（预期为空，因为无"信息"专业数据）
SELECT * FROM V_IS;

-- 通过视图 V_IS 更新学生姓名（按题目要求执行）
UPDATE V_IS SET Sname = 'S1_MMM' WHERE Sno = 'S1';
UPDATE V_IS SET Sname = 'S4_MMM' WHERE Sno = 'S4';

-- 查询更新结果
SELECT * FROM V_IS;

-- ============================================================================
-- 12. 通过视图 V_IS 新增一个学生记录并查询结果
-- INSERT INTO V_IS VALUES ('S12', 'YAN XI', 19, 'IS');
-- ★ 注意 ★
--   1) 原始数据为4个值，但 Student 表有6列（Sno, Sname, Ssex, Sage, ClsNO, Saddr），
--      缺少 Ssex 和 Saddr 的值。
--   2) 'S12' 长度为3，而 Sno 定义为 CHAR(8)。
--   3) 'IS' 不是有效的班级号（ClsNO 为 CHAR(6)）。
--   4) 插入时若 ClsNO='IS' 不在 Class 表中，将触发外键约束错误。
--   此处按题目要求补充缺失的字段值以完成插入。
-- ============================================================================
INSERT INTO V_IS (Sno, Sname, Ssex, Sage, ClsNO, Saddr) VALUES
    ('S12', 'YAN XI', '男', 19, 'IS', NULL);

-- 查询插入结果
SELECT * FROM V_IS;

-- ============================================================================
-- 13. 通过视图 V_AVG_S_G 将学号为"S1"的平均成绩改为90分，是否可以实现？
-- ★ 答案：不可以实现 ★
-- 原因：
--   1) V_AVG_S_G 是一个包含聚合函数（COUNT、AVG）和 GROUP BY 的视图，
--      属于"不可更新视图"（non-updatable view）。
--   2) MySQL 不允许对包含聚合函数、GROUP BY、DISTINCT 等的视图执行
--      UPDATE/INSERT/DELETE 操作，因为数据库无法确定如何将聚合值的修改
--      映射回底层基表中的具体行。
--   3) "平均成绩"是计算字段，由多条 SC 记录的 Grade 值计算而来，
--      无法通过修改一个聚合值来确定应修改哪些原始成绩记录。
--
-- 以下语句将失败（用于验证）：
-- UPDATE V_AVG_S_G SET AvgGrade = 90 WHERE Sno = 'S1';
-- 错误信息：The target table V_AVG_S_G of the UPDATE is not updatable
-- ============================================================================
-- 取消下行注释以验证视图不可更新：
-- UPDATE V_AVG_S_G SET AvgGrade = 90 WHERE Sno = 'S1';

-- ============================================================================
-- 查看所有已创建的视图
-- ============================================================================
SHOW FULL TABLES WHERE Table_type = 'VIEW';
