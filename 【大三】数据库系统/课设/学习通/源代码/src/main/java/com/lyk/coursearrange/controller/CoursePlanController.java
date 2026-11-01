package com.lyk.coursearrange.controller;


import com.alibaba.fastjson.JSON;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.service.additional.query.impl.QueryChainWrapper;
import com.lyk.coursearrange.common.ServerResponse;
import com.lyk.coursearrange.dao.ClassTaskDao;
import com.lyk.coursearrange.entity.*;
import com.lyk.coursearrange.entity.request.ClassTaskDTO;
import com.lyk.coursearrange.entity.request.CoursePlanDTO;
import com.lyk.coursearrange.entity.response.CoursePlanVo;
import com.lyk.coursearrange.service.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


@RestController
public class CoursePlanController {

    @Autowired
    private CourseInfoService courseInfoService;
    @Autowired
    private CoursePlanService coursePlanService;

    @Autowired
    private TeacherService teacherService;
    @Autowired
    private ClassTaskService classTaskService;
    @Autowired
    private ClassroomService classroomService;
    @Autowired
    private ClassTaskDao classTaskDao;






    /**
     * 根据班级查询课程表
     * @param classNo
     * @return
     */
    @GetMapping("/courseplan/{classno}")
    public ServerResponse queryCoursePlanByClassNo(@PathVariable("classno") String classNo) {
        QueryWrapper<CoursePlan> wrapper = new QueryWrapper<CoursePlan>().eq("class_no", classNo).orderByAsc("class_time");
        List<CoursePlan> coursePlanList = coursePlanService.list(wrapper);
        List<CoursePlanVo> coursePlanVos = new LinkedList<>();
        coursePlanList.forEach(v->{
            CoursePlanVo coursePlanVo = JSON.parseObject(JSON.toJSONString(v), CoursePlanVo.class);
            coursePlanVo.setCourseInfo(courseInfoService.getOne(new QueryWrapper<CourseInfo>().eq("course_no",v.getCourseNo())));
            coursePlanVo.setTeacher(teacherService.getOne(new QueryWrapper<Teacher>().eq("teacher_no",v.getTeacherNo())));
            coursePlanVos.add(coursePlanVo);
        });
        return ServerResponse.ofSuccess(coursePlanVos);
    }


    //手动添加课表
    @PostMapping("/addcourseplan")
    public ServerResponse addCourseplan(@RequestBody() CoursePlanDTO c) {
//        System.out.println(c);
        CoursePlan coursePlan = new CoursePlan();
        coursePlan.setSemester(c.getSemester());
        coursePlan.setGradeNo(c.getGradeNo());
        coursePlan.setClassNo(c.getClassNo());
//        coursePlan.setCourseNo(c.getCourseName());
//        coursePlan.setTeacherNo(c.getRealname());
        coursePlan.setClassTime(c.getClassTime());


        int id = -1;
        int week_num = -1;
        //进行查询class_task
        QueryWrapper<ClassTask> wrapper = new QueryWrapper<>();
        QueryWrapper<ClassTask> classTaskQueryWrapper = wrapper
                .eq("semester",c.getSemester())
                .eq("class_no",c.getClassNo())
                .eq("course_name",c.getCourseName())
                .eq("realname",c.getRealname());
        List<ClassTask> classTaskList = classTaskService.list(classTaskQueryWrapper);
        System.out.println(classTaskList.toString());
        int sign = 0;
        for (ClassTask classTask : classTaskList) {
            sign = sign+1;
            System.out.println("123456654321");
            coursePlan.setCourseNo(classTask.getCourseNo());
            coursePlan.setTeacherNo(classTask.getTeacherNo());
            coursePlan.setWeeksSum(classTask.getWeeksSum());
            Boolean deleted = classTask.getDeleted();
            int myInt = deleted ? 1 : 0;
            coursePlan.setDeleted(myInt);
            coursePlan.setCreateTime(classTask.getCreateTime());
            coursePlan.setUpdateTime(classTask.getUpdateTime());

            id = classTask.getId();
            week_num = classTask.getWeeksNumber()+1;

            break;
        }
        if (sign ==0){
            return ServerResponse.ofError("手动添加课程任务失败");
        }

        UpdateWrapper<ClassTask> updateWrapper = new UpdateWrapper<>();
        updateWrapper.eq("id",id).set("weeks_number", week_num);
        classTaskService.update(null,updateWrapper);



        //进行查询classroom
        QueryWrapper<Classroom> classRoomQueryWrapper = new QueryWrapper<>();
        List<Classroom> classRoomList = classroomService.list(classRoomQueryWrapper);
        System.out.println("this is classRoomList");
        System.out.println(classRoomList.toString());
        for (Classroom classroom : classRoomList) {
            coursePlan.setClassroomNo(classroom.getClassroomNo());
            System.out.println("------------------");
            System.out.println("------------------");
            System.out.println("------------------");
            System.out.println(classroom.getClassroomNo());
            System.out.println("------------------");
            System.out.println("------------------");
            System.out.println("------------------");
            boolean b = coursePlanService.save(coursePlan);
            if (b) {
                System.out.println("手动添加课程任务成功");
                return ServerResponse.ofSuccess("手动添加课程任务成功");

            }
        }


        return ServerResponse.ofError("手动添加课程任务失败");
    }


    //手动删除课表
    @PostMapping("/deletecourseplan")
    public ServerResponse deleteCourseplan(@RequestBody() CoursePlanDTO c) {
        System.out.println(c);
        //删除course_plan表中为GradeNo、ClassNo、ClassTime
        QueryWrapper<CoursePlan> wrapper = new QueryWrapper<>();
        wrapper.eq("semester",c.getSemester());
        wrapper.eq("class_no",c.getClassNo());
        wrapper.eq("class_time",c.getClassTime());

//        Map<String,Object>map = new HashMap<>();
//        map.put("semester",c.getSemester());
//        map.put("class_no",c.getClassNo());
//        map.put("class_time",c.getClassTime());

//
//        int b = classTaskDao.deleteByMap(map);
        boolean b = coursePlanService.remove(wrapper);



        if (b) {
            return ServerResponse.ofSuccess("手动删除课程任务成功");
        }
        return ServerResponse.ofError("手动删除课程任务失败");
    }
}

