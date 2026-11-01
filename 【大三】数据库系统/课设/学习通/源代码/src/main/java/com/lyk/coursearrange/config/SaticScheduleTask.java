package com.lyk.coursearrange.config;

import com.lyk.coursearrange.util.CMDutil;
import org.springframework.context.annotation.Configuration;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.annotation.Scheduled;

import java.sql.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.util.Calendar;
import java.util.Timer;

@Configuration      //1.主要用于标记配置类，兼备Component的效果。
@EnableScheduling   // 2.开启定时任务
public class SaticScheduleTask {
    //3.添加定时任务
    @Scheduled(cron = "0/60 * * * * ?")
    //或直接指定时间间隔，例如：5秒
    //@Scheduled(fixedRate=5000)
    private void configureTasks() {
        Date date = new Date(System.currentTimeMillis());
        DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
        String format = dateFormat.format(date);
        format = format.replaceAll(":","_");
        format = format.substring(0,10)+'T'+format.substring(11);
        System.out.println(format);
        CMDutil.runCommand("mysqldump -uroot -pqwe123850 db_course_arrangement>D:\\db_backup\\backup"+format+".sql","C:\\Program Files\\MySQL\\MySQL Server 8.0\\bin");
        System.err.println("执行静态定时任务时间: " + LocalDateTime.now());
    }
}