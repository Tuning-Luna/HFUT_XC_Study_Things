package com.lyk.coursearrange.dao;

import com.lyk.coursearrange.entity.Teacher;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * <p>
 *  Mapper 接口
 * </p>
 */
public interface TeacherDao extends BaseMapper<Teacher> {

    @Select("SELECT * FROM teacher WHERE teacher_no=#{account} AND password=#{password}" +
            "        UNION" +
            "        SELECT * FROM teacher WHERE username=#{account} AND password=#{password}" +
            "        UNION" +
            "        SELECT * FROM teacher WHERE realname=#{account} AND password=#{password}")
    Teacher teacherLogin(@Param("account") String account, @Param("password") String password);

    @Select("select count(*) from teacher where date(create_time) = #{yesday}")
    int teacherReg(@Param("yesday") String yesday);

}
