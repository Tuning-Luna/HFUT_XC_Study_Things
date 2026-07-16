-- ============================================================================
-- 实验一：修改列的定义（步骤3）
-- 数据库：jwgl2023217273
-- ============================================================================

USE jwgl2023217273;

-- ----------------------------------------------------------------------------
-- 3.1 将 Student 表中 Sname 的类型从 VARCHAR(10) 改为 VARCHAR(20)
-- 注意：题目描述中写的是 "varchar(8)改成varchar(20)"，
--       但原始表定义中 Sname 长度为10，此处按实际定义执行（10→20）
-- ----------------------------------------------------------------------------
ALTER TABLE Student
    MODIFY COLUMN Sname VARCHAR(20) NOT NULL COMMENT '姓名';

-- 验证修改结果
DESCRIBE Student;

-- ----------------------------------------------------------------------------
-- 3.2 为 Course 表的 Semester 字段添加 CHECK 约束（取值在1-10之间）
-- 注意：MySQL 8.0.16+ 支持 CHECK 约束
--       由于已有 chk_semester(Semester > 0)，
--       需先删除旧约束再添加新约束
-- ----------------------------------------------------------------------------
-- 先查看现有约束名
SELECT CONSTRAINT_NAME, CHECK_CLAUSE
FROM INFORMATION_SCHEMA.CHECK_CONSTRAINTS
WHERE TABLE_NAME = 'Course';

-- 删除旧的 CHECK 约束
ALTER TABLE Course
    DROP CONSTRAINT chk_semester;

-- 添加新的 CHECK 约束（1-10）
ALTER TABLE Course
    ADD CONSTRAINT chk_semester CHECK (Semester BETWEEN 1 AND 10);

-- 验证约束
SELECT CONSTRAINT_NAME, CHECK_CLAUSE
FROM INFORMATION_SCHEMA.CHECK_CONSTRAINTS
WHERE TABLE_NAME = 'Course';

-- ----------------------------------------------------------------------------
-- 3.3 删除 Course 表中的 Semester 字段
-- ----------------------------------------------------------------------------
ALTER TABLE Course
    DROP COLUMN Semester;

-- 验证删除结果
DESCRIBE Course;

-- ----------------------------------------------------------------------------
-- 3.4 向 Course 表重新添加 Semester 字段
-- 注意：重新添加后不再有之前的 CHECK(1-10) 约束，仅保留基本定义
-- ----------------------------------------------------------------------------
ALTER TABLE Course
    ADD COLUMN Semester INT NULL COMMENT '学期';

-- 添加基本约束：学期必须大于0
ALTER TABLE Course
    ADD CONSTRAINT chk_semester CHECK (Semester > 0);

-- 验证最终结构
DESCRIBE Course;
