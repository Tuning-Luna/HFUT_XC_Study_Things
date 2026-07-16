package com.lyk.coursearrange.dao;

import com.lyk.coursearrange.entity.LocationInfo;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.lyk.coursearrange.entity.response.LocationVO;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 *  Mapper 接口
 * </p>
 *
 */
public interface LocationInfoDao extends BaseMapper<LocationInfo> {

//    @Select("select ttbi.teach_build_no, ttbi.teach_build_name, tgi.grade_no,tgi.grade_name from teach_building ttbi join location tli on tli.teachbuild_no = ttbi.teach_build_no join grade_information tgi on tli.grade_no = tgi.grade_no limit ${page}, ${limit}")
//    List<LocationVO> locations(Integer page, Integer limit);

    @Select("select tli.id, ttbi.teach_build_no, ttbi.teach_build_name, tgi.grade_no,tgi.grade_name from teach_building ttbi join location tli on tli.teachbuild_no = ttbi.teach_build_no join grade_information tgi on tli.grade_no = tgi.grade_no where tli.deleted = 0 limit ${page}, ${limit}")
    List<LocationVO> locations(Integer page, Integer limit);

    @Select("select count(*) from teach_building ttbi join location tli on tli.teachbuild_no = ttbi.teach_build_no join grade_information tgi on tli.grade_no = tgi.grade_no")
    int count();
}
