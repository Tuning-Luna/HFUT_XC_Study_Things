package com.lyk.coursearrange.entity.response;

import com.lyk.coursearrange.entity.CourseInfo;
import com.lyk.coursearrange.entity.CoursePlan;
import com.lyk.coursearrange.entity.Teacher;
import lombok.Data;

/**
 * @Descripe:
 */
@Data
public class CoursePlanVo extends CoursePlan {

    private Teacher teacher;

    private CourseInfo courseInfo;
}
