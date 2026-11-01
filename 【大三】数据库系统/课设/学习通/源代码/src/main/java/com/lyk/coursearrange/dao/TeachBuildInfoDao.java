package com.lyk.coursearrange.dao;

import com.lyk.coursearrange.entity.TeachbuildInfo;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;


public interface TeachBuildInfoDao extends BaseMapper<TeachbuildInfo> {

    @Select("select teachbuild_no from location where grade_no = #{gradeNo}")
    String selectBuildNo(@Param("gradeNo") String gradeNo);

    @Select("select teachbuild_no from location where grade_no = #{gradeNo}")
    List<String> selectTeachBuildList(@Param("gradeNo") String gradeNo);
}
