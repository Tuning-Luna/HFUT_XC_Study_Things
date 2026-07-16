-- ============================================================================
-- 实验二 & 实验三：触发器
-- 数据库：jwgl2023217273
--
-- 包含：
--   实验二·第5步：INSERT 触发器 instrg1
--   实验二·第6步：UPDATE 触发器 update1
--   实验三·第8步：BEFORE INSERT 触发器 S_student
-- ============================================================================

USE jwgl2023217273;

-- ============================================================================
-- 5. 创建 INSERT 触发器 instrg1
--    当向 Student 表新增一名学生时，自动将该生所在班级的 stotal 加1
--    触发时机：AFTER INSERT（插入成功后执行）
--    作用对象：Student 表的每一行
-- ============================================================================
DELIMITER //

CREATE TRIGGER instrg1
AFTER INSERT ON Student
FOR EACH ROW
BEGIN
    -- 将新增学生所在班级的总人数加1
    UPDATE Class
    SET stotal = stotal + 1
    WHERE ClsNO = NEW.ClsNO;
END //

DELIMITER ;

-- ============================================================================
-- 6. 创建 UPDATE 触发器 update1
--    当某学生所在班号发生变化时（即调到另一班级后）：
--      将其原先所在班级总人数减1
--      将新调入班级的总人数加1
--    触发时机：AFTER UPDATE（更新成功后执行）
--    条件判断：只有当 ClsNO 确实发生变化时才执行
-- ============================================================================
DELIMITER //

CREATE TRIGGER update1
AFTER UPDATE ON Student
FOR EACH ROW
BEGIN
    -- 仅当班级编号确实改变时才调整人数
    IF OLD.ClsNO <> NEW.ClsNO THEN
        -- 原班级人数减1
        UPDATE Class
        SET stotal = stotal - 1
        WHERE ClsNO = OLD.ClsNO;

        -- 新班级人数加1
        UPDATE Class
        SET stotal = stotal + 1
        WHERE ClsNO = NEW.ClsNO;
    END IF;
END //

DELIMITER ;

-- ============================================================================
-- 实验三·第8步：创建触发器 S_student
--    确保向 Student 表中插入记录时，学号的前2位必须等于"20"
--    如果不符合条件，则抛出错误信息并阻止插入
--    触发时机：BEFORE INSERT（插入前检查）
-- ============================================================================
DELIMITER //

CREATE TRIGGER S_student
BEFORE INSERT ON Student
FOR EACH ROW
BEGIN
    -- 检查学号前两位是否为"20"
    IF LEFT(NEW.Sno, 2) <> '20' THEN
        -- SIGNAL 语句用于抛出自定义错误（MySQL 5.5+）
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = '学生号前2位必须是20';
    END IF;
END //

DELIMITER ;

-- ============================================================================
-- 9. 测试触发器 S_student 是否能成功执行
--    测试1（应失败）：插入学号不以"20"开头的学生
--    测试2（应成功）：插入学号以"20"开头的学生
-- ============================================================================

-- 测试1：预期失败，错误信息"学生号前2位必须是20"
-- 取消下行注释以执行测试：
-- INSERT INTO Student (Sno, Sname, Ssex, Sage, ClsNO) VALUES
--     ('19001001', '测试生', '男', 20, 'CS01');

-- 测试2：预期成功
-- 取消下行注释以执行测试：
-- INSERT INTO Student (Sno, Sname, Ssex, Sage, ClsNO) VALUES
--     ('20001001', '合法学生', '女', 21, 'CS02');

-- 查看触发器的定义
SHOW TRIGGERS;
