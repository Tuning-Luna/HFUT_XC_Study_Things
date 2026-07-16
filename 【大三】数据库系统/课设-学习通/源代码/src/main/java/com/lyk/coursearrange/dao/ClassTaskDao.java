package com.lyk.coursearrange.dao;

import com.lyk.coursearrange.entity.ClassTask;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import org.apache.ibatis.annotations.*;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
@Mapper
public interface ClassTaskDao extends BaseMapper<ClassTask> {

    // 查询指定学期的开课任务
//    @Select("SELECT * FROM class_task where semester = #{semester}")
    List<ClassTask> selectBySemester(ClassTask classTask);

    @Select("SELECT distinct class_no FROM class_task")
    List<String> selectClassNo();

    // 可能这里是columnName没传进来
    @Select("select distinct ${columnName} from class_task")
    List<String> selectByColumnName(@Param("columnName") String columnName);

    @Select("select distinct grade_no from class_task")
    List<String> selectByGradeNo();

    @Update("truncate class_task")
    void clearClassTaskOld();

}
