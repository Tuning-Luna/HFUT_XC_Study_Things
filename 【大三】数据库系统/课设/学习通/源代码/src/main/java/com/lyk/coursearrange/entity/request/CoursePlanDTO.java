package com.lyk.coursearrange.entity.request;

import lombok.Data;

/**
 * @author: 15760
 * @Date: 2020/4/3
 * @Descripe:
 */
@Data
public class CoursePlanDTO {

    /**
     * 学期
     */
    private String semester;

    /**
     * 年级
     */
    private String gradeNo;

    /**
     * 班级编号
     */
    private String classNo;

    /**
     * 课程名称
     */
    private String courseName;
    /**
     * 固定时间的话时间是什么时候
     */
    private String classTime;
    /**
     * 讲师名字
     */
    private String realname;


}
