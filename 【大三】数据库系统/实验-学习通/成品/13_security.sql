-- ============================================================================
-- 实验三：数据库的安全性（步骤1-5）
-- 数据库：jwgl2023217273
--
-- ★ 执行前提 ★
-- 本脚本需要以具有 CREATE USER、GRANT 等管理权限的账户（如 root）执行
-- ============================================================================

USE jwgl2023217273;

-- ============================================================================
-- 1. 建立登录账户 log1、log2、log3，并将它们映射为 jwgl2023217273 中的用户
-- MySQL 8.0+ 语法：CREATE USER
-- 注意：密码仅用于实验，实际生产环境应使用强密码
-- ============================================================================

-- 先清理可能已存在的用户（方便重复执行）
DROP USER IF EXISTS 'log1'@'localhost';
DROP USER IF EXISTS 'log2'@'localhost';
DROP USER IF EXISTS 'log3'@'localhost';

-- 创建三个登录账户
CREATE USER 'log1'@'localhost' IDENTIFIED BY 'Log1_password';
CREATE USER 'log2'@'localhost' IDENTIFIED BY 'Log2_password';
CREATE USER 'log3'@'localhost' IDENTIFIED BY 'Log3_password';

-- 验证用户已创建
SELECT User, Host FROM mysql.user WHERE User IN ('log1', 'log2', 'log3');

-- ============================================================================
-- 2. 为 log1、log2、log3 授予对 Student 表的查询权（SELECT）
-- ============================================================================
GRANT SELECT ON jwgl2023217273.Student TO 'log1'@'localhost';
GRANT SELECT ON jwgl2023217273.Student TO 'log2'@'localhost';
GRANT SELECT ON jwgl2023217273.Student TO 'log3'@'localhost';

-- 刷新权限使其生效
FLUSH PRIVILEGES;

-- 查看各用户对 Student 表的权限
SHOW GRANTS FOR 'log1'@'localhost';
SHOW GRANTS FOR 'log2'@'localhost';
SHOW GRANTS FOR 'log3'@'localhost';

-- ============================================================================
-- 3. 在查询编辑器中使用 SQL 语句为 log3 授予对 Student 表的插入权
-- ============================================================================
GRANT INSERT ON jwgl2023217273.Student TO 'log3'@'localhost';

FLUSH PRIVILEGES;

-- 验证 log3 的权限
SHOW GRANTS FOR 'log3'@'localhost';

-- ============================================================================
-- 4. 建立 jwgl 数据库的用户角色 ROLE1，并将 log1、log2 添加到此角色中，
--    为此角色授予对 Student 表的修改权（UPDATE）
--    MySQL 8.0+ 支持角色（Role）功能
-- ============================================================================

-- 创建角色 ROLE1
DROP ROLE IF EXISTS 'ROLE1';
CREATE ROLE 'ROLE1';

-- 为角色 ROLE1 授予对 Student 表的修改权
GRANT UPDATE ON jwgl2023217273.Student TO 'ROLE1';

-- 将 log1、log2 添加到角色 ROLE1
GRANT 'ROLE1' TO 'log1'@'localhost';
GRANT 'ROLE1' TO 'log2'@'localhost';

FLUSH PRIVILEGES;

-- 注意：在 MySQL 8.0 中，授予角色后，
-- 用户需要在会话中执行 SET ROLE ROLE1; 来激活角色，
-- 或者设置 activate_all_roles_on_login = ON 使角色在登录时自动激活

-- 设置默认角色（使角色在登录时自动激活）
SET DEFAULT ROLE 'ROLE1' TO 'log1'@'localhost';
SET DEFAULT ROLE 'ROLE1' TO 'log2'@'localhost';

-- 验证角色及成员
SELECT * FROM mysql.role_edges;

-- ============================================================================
-- 5. 权限测试说明
--
-- 用 log2 登录（在 Navicat 中新建连接，用户名为 log2，密码为 Log2_password）：
--   log2 属于 ROLE1 角色，拥有 Student 表的 SELECT 和 UPDATE 权限，
--   因此可以修改 Student 中某个学生的信息。
--
--   示例：
--   USE jwgl2023217273;
--   UPDATE Student SET Saddr = '新地址' WHERE Sno = '20190101';
--   -- 预期：执行成功
--
-- 用 log3 登录：
--   log3 只有 SELECT 和 INSERT 权限，没有 UPDATE 权限，
--   因此执行相同的修改操作会失败。
--
--   示例：
--   USE jwgl2023217273;
--   UPDATE Student SET Saddr = '新地址' WHERE Sno = '20190101';
--   -- 预期：报错 "UPDATE command denied to user 'log3'@'localhost'"
--
-- 总结：log2 可以修改（角色 ROLE1 给了 UPDATE 权），log3 无法修改（只有 SELECT + INSERT）
-- ============================================================================
