-- ============================================================================
-- 实验一：嵌套和集合查询（步骤10）
-- 数据库：jwgl2023217273
-- ============================================================================

USE jwgl2023217273;

-- ----------------------------------------------------------------------------
-- 1. 求0001课程的成绩高于"张三"的学生学号和成绩
-- 子查询：先找出张三在0001课程的成绩，再查找高于该成绩的学生
-- ----------------------------------------------------------------------------
SELECT Sno, Grade
FROM SC
WHERE Cno = '0001'
    AND Grade > (
        SELECT Grade
        FROM SC
        WHERE Cno = '0001'
            AND Sno = (SELECT Sno FROM Student WHERE Sname = '张三')
    );

-- ----------------------------------------------------------------------------
-- 2. 求其他专业中比计算机应用专业某一学生年龄小的学生信息
-- （即求其它专业中年龄小于计算机应用专业年龄最大者的学生）
-- 子查询：先求出计算机应用专业的最大年龄
-- ----------------------------------------------------------------------------
SELECT S.*
FROM Student S
    JOIN Class C ON S.ClsNO = C.ClsNO
WHERE C.Specialty <> '计算机应用'
    AND S.Sage < (
        SELECT MAX(S2.Sage)
        FROM Student S2
            JOIN Class C2 ON S2.ClsNO = C2.ClsNO
        WHERE C2.Specialty = '计算机应用'
    );

-- ----------------------------------------------------------------------------
-- 3. 求没有选修0002课程的学生姓名
-- 使用 NOT IN 子查询：找出不在选修0002课程名单中的学生
-- ----------------------------------------------------------------------------
SELECT Sname
FROM Student
WHERE Sno NOT IN (
    SELECT Sno
    FROM SC
    WHERE Cno = '0002'
);

-- ----------------------------------------------------------------------------
-- 4. 查询选修了全部课程的学生的姓名
-- 使用除法思想：学生选修的课程数 = 全部课程数
-- 注意：由于 SC 表中有课程 0007（不在 Course 表中），
--       统计时应以 Course 表为准
-- ----------------------------------------------------------------------------
SELECT S.Sname
FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Course C ON SC.Cno = C.Cno  -- 仅统计 Course 表中存在的课程
GROUP BY S.Sno, S.Sname
HAVING COUNT(DISTINCT SC.Cno) = (SELECT COUNT(*) FROM Course);

-- ----------------------------------------------------------------------------
-- 5. 求选修课超过3门课的学生学号
-- GROUP BY + HAVING COUNT(*) > 3
-- ----------------------------------------------------------------------------
SELECT Sno
FROM SC
GROUP BY Sno
HAVING COUNT(*) > 3;

-- ----------------------------------------------------------------------------
-- 6. 统计每个专业的学生人数
-- 需要 Student JOIN Class，按专业分组统计
-- 使用 LEFT JOIN 确保没有学生的专业也显示（人数为0）
-- ----------------------------------------------------------------------------
SELECT C.Specialty, COUNT(S.Sno) AS StudentCount
FROM Class C
    LEFT JOIN Student S ON C.ClsNO = S.ClsNO
GROUP BY C.Specialty
ORDER BY StudentCount DESC;

-- ----------------------------------------------------------------------------
-- 7. 统计每门课程的选课人数和考试最高分
-- 按课程分组，聚合统计 COUNT 和 MAX
-- 注意：Grade 为 NULL 的不计入 COUNT，但 MAX 会自动忽略 NULL
-- ----------------------------------------------------------------------------
SELECT SC.Cno,
       C.Cname,
       COUNT(SC.Sno) AS StudentCount,
       MAX(SC.Grade) AS MaxGrade
FROM SC
    JOIN Course C ON SC.Cno = C.Cno
GROUP BY SC.Cno, C.Cname
ORDER BY SC.Cno;

-- ----------------------------------------------------------------------------
-- 8. 查询计算机专业考试成绩最高的学生的姓名
-- 多层子查询：
--   1) 找出所有计算机专业学生的成绩
--   2) 找出最高成绩
--   3) 返回对应学生姓名
-- 考虑并列最高的情况，使用 IN 而非 =
-- ----------------------------------------------------------------------------
SELECT DISTINCT S.Sname
FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Class C ON S.ClsNO = C.ClsNO
WHERE C.Specialty = '计算机应用'
    AND SC.Grade = (
        SELECT MAX(SC2.Grade)
        FROM SC SC2
            JOIN Student S2 ON SC2.Sno = S2.Sno
            JOIN Class C2 ON S2.ClsNO = C2.ClsNO
        WHERE C2.Specialty = '计算机应用'
    );

-- ----------------------------------------------------------------------------
-- 9. 查询每个同学的必修课总学分和选修课总学分
-- 使用条件聚合（CASE WHEN + SUM）在同一个查询中分别统计
-- ----------------------------------------------------------------------------
SELECT S.Sno,
       S.Sname,
       SUM(CASE WHEN C.Property = '必修' THEN C.Ccredit ELSE 0 END) AS RequiredCredits,
       SUM(CASE WHEN C.Property = '选修' THEN C.Ccredit ELSE 0 END) AS ElectiveCredits
FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Course C ON SC.Cno = C.Cno
GROUP BY S.Sno, S.Sname
ORDER BY S.Sno;
