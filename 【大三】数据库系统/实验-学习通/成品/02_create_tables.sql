-- ============================================================================
-- 实验一：创建数据表
-- 数据库：jwgl2023217273
-- 按顺序创建四张表：Class → Student → Course → SC
-- 注意：Course 表含自引用外键 Cpno → Cno，因此先创建表再通过 ALTER 添加外键
-- ============================================================================

USE jwgl2023217273;

-- ----------------------------------------------------------------------------
-- 1. 班级表 Class
-- ClsNO:    班级号，定长字符串(6)，主码
-- ClsName:  班级名，可变长字符串(16)，非空
-- Director: 辅导员，可变长字符串(10)
-- Specialty:专业，可变长字符串(30)
-- ----------------------------------------------------------------------------
CREATE TABLE Class (
    ClsNO     CHAR(6)      NOT NULL COMMENT '班级号',
    ClsName   VARCHAR(16)  NOT NULL COMMENT '班级名',
    Director  VARCHAR(10)  NULL     COMMENT '辅导员',
    Specialty VARCHAR(30)  NULL     COMMENT '专业',
    PRIMARY KEY (ClsNO)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
  COMMENT='班级表';

-- ----------------------------------------------------------------------------
-- 2. 学生表 Student
-- Sno:   学号，定长字符串(8)，主码
-- Sname: 姓名，可变长字符串(10)，非空（后续实验步骤中将修改为20）
-- Ssex:  性别，定长字符串(2)，约束取'男'或'女'
-- Sage:  年龄，微整型 TINYINT，约束取值15～45
-- ClsNO: 所在班级编号，定长字符串(6)，外键参照 Class(ClsNO)
-- Saddr: 住址，可变长字符串(20)
-- ----------------------------------------------------------------------------
CREATE TABLE Student (
    Sno    CHAR(8)      NOT NULL COMMENT '学号',
    Sname  VARCHAR(10)  NOT NULL COMMENT '姓名',
    Ssex   CHAR(2)      NULL     COMMENT '性别',
    Sage   TINYINT      NULL     COMMENT '年龄',
    ClsNO  CHAR(6)      NULL     COMMENT '所在班级编号',
    Saddr  VARCHAR(20)  NULL     COMMENT '住址',
    PRIMARY KEY (Sno),
    -- CHECK 约束：性别只能为'男'或'女'（MySQL 8.0.16+ 支持）
    CONSTRAINT chk_ssex  CHECK (Ssex IN ('男', '女')),
    -- CHECK 约束：年龄在15～45之间
    CONSTRAINT chk_sage  CHECK (Sage BETWEEN 15 AND 45),
    -- 外键约束：班级编号参照 Class 表
    CONSTRAINT fk_student_class FOREIGN KEY (ClsNO) REFERENCES Class(ClsNO)
        ON UPDATE CASCADE ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
  COMMENT='学生表';

-- ----------------------------------------------------------------------------
-- 3. 课程表 Course
-- Cno:      课程号，字符串长度4，主码
-- Cname:    课程名，字符串长度20，非空
-- Ccredit:  学分，整数，取值大于0
-- Semester: 学期，整数，取值大于0
-- Property: 课程性质，字符串长度10，默认值为'必修'
-- Cpno:     先修课，字符串长度4，外码参照 Course(Cno)
--
-- 注意：由于 Cpno 自引用 Course(Cno)，在插入数据时需注意顺序。
--       先插入 Cpno 为 NULL 的行，再插入有先修课要求的行。
--       或者先创建表不添加外键，等数据全部插入后再 ALTER 添加。
--       这里采用先创建表再 ALTER 添加外键的方式以保证灵活性。
-- ----------------------------------------------------------------------------
CREATE TABLE Course (
    Cno      CHAR(4)      NOT NULL COMMENT '课程号',
    Cname    VARCHAR(20)  NOT NULL COMMENT '课程名',
    Ccredit  INT          NULL     COMMENT '学分',
    Semester INT          NULL     COMMENT '学期',
    Property VARCHAR(10)  NULL     DEFAULT '必修' COMMENT '课程性质',
    Cpno     CHAR(4)      NULL     COMMENT '先修课',
    PRIMARY KEY (Cno),
    -- CHECK 约束：学分必须大于0
    CONSTRAINT chk_ccredit CHECK (Ccredit > 0),
    -- CHECK 约束：学期必须大于0
    CONSTRAINT chk_semester CHECK (Semester > 0)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
  COMMENT='课程表';

-- 添加自引用外键：Cpno 参照 Course(Cno)
-- 使用 ALTER TABLE 添加，便于数据插入时灵活控制顺序
ALTER TABLE Course
    ADD CONSTRAINT fk_course_cpno FOREIGN KEY (Cpno) REFERENCES Course(Cno)
        ON UPDATE CASCADE ON DELETE SET NULL;

-- ----------------------------------------------------------------------------
-- 4. 选课表 SC（Student-Course）
-- Sno:   学号，字符串长度8，复合主码的一部分，外键参照 Student(Sno)
-- Cno:   课程号，字符串长度4，复合主码的一部分，外键参照 Course(Cno)
-- Grade: 成绩，整数，取值0～100
-- ----------------------------------------------------------------------------
CREATE TABLE SC (
    Sno   CHAR(8) NOT NULL COMMENT '学号',
    Cno   CHAR(4) NOT NULL COMMENT '课程号',
    Grade INT     NULL     COMMENT '成绩',
    -- 复合主键：(Sno, Cno) 共同构成主码
    PRIMARY KEY (Sno, Cno),
    -- 外键约束：学号参照 Student 表
    CONSTRAINT fk_sc_student FOREIGN KEY (Sno) REFERENCES Student(Sno)
        ON UPDATE CASCADE ON DELETE CASCADE,
    -- 外键约束：课程号参照 Course 表
    CONSTRAINT fk_sc_course  FOREIGN KEY (Cno) REFERENCES Course(Cno)
        ON UPDATE CASCADE ON DELETE CASCADE,
    -- CHECK 约束：成绩取值0～100
    CONSTRAINT chk_grade CHECK (Grade BETWEEN 0 AND 100)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
  COMMENT='选课表';

-- ============================================================================
-- 验证表结构
-- ============================================================================
SHOW TABLES;
DESCRIBE Class;
DESCRIBE Student;
DESCRIBE Course;
DESCRIBE SC;
