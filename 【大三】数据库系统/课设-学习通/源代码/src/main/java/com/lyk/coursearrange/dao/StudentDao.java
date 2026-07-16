package com.lyk.coursearrange.dao;

import com.lyk.coursearrange.entity.Student;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.springframework.web.bind.annotation.PathVariable;

/**
 * <p>
 *  Mapper 接口
 * </p>
 */
public interface StudentDao extends BaseMapper<Student> {

    @Select("SELECT * FROM student WHERE student_no=#{account} AND password=#{password}" +
            "        UNION" +
            "        SELECT * FROM student WHERE username=#{account} AND password=#{password}" +
            "        UNION" +
            "        SELECT * FROM student WHERE realname=#{account} AND password=#{password}")
    Student studentLogin(@Param("account") String username, @Param("password") String password);

    @Select("select count(*) from student where date(create_time) = #{yesday}")
    int studentReg(@Param("yesday") String yesday);

}
