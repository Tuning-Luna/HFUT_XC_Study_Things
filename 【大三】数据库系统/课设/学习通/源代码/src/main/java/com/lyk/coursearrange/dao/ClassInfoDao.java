package com.lyk.coursearrange.dao;

import com.lyk.coursearrange.entity.ClassInfo;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.lyk.coursearrange.entity.response.ClassInfoVO;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 *  Mapper 接口
 * </p>
 *
 */
public interface ClassInfoDao extends BaseMapper<ClassInfo> {

    // 获得班级的人数
    @Select("select num from class_information where class_no = #{classNo}")
    int selectStuNum(@Param("classNo") String classNo);

    @Select("SELECT tci.id,tgi.grade_name,tci.class_no,tci.class_name,t.realname,tci.num FROM class_information tci join teacher t on tci.teacher = t.id join grade_information tgi on tci.remark = tgi.grade_no where tgi.grade_no = ${gradeNo} limit ${page}, ${limit}")
    List<ClassInfoVO> queryClassInfo(Integer page, Integer limit, String gradeNo);

    @Select("SELECT tci.id,tgi.grade_name,tci.class_no,tci.class_name,t.realname,tci.num FROM class_information tci join teacher t on tci.teacher = t.id join grade_information tgi on tci.remark = tgi.grade_no limit ${page}, ${limit}")
    List<ClassInfoVO> queryClassInfos(Integer page, Integer limit);

    @Select("SELECT count(tci.id) FROM class_information tci join teacher t on tci.teacher = t.id join grade_information tgi on tci.remark = tgi.grade_no where tgi.grade_no = ${gradeNo}")
    int count1(String gradeNo);

    @Select("SELECT count(tci.id) FROM class_information tci join teacher t on tci.teacher = t.id join grade_information tgi on tci.remark = tgi.grade_no")
    int count2();
}
