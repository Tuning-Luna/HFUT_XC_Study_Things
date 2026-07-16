package com.lyk.coursearrange.config;

import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import org.apache.ibatis.reflection.MetaObject;
import org.springframework.context.annotation.Configuration;

import java.time.LocalDateTime;

/**
 * @Descripe: 自动更新
 */

@Configuration
public class MyMetaObjectHandler implements MetaObjectHandler {

    // 自动插入公公字段
    @Override
    public void insertFill(MetaObject metaObject) {
        if (metaObject.hasSetter("createTime")&&getFieldValByName("createTime",metaObject)==null) {
            setInsertFieldValByName("createTime", LocalDateTime.now(), metaObject);
            //setInsertFieldValByName("updateTime", LocalDateTime.now(), metaObject);
        }
    }

    // 自动更新公共字段
    @Override
    public void updateFill(MetaObject metaObject) {
        if (metaObject.hasSetter("updateTime")&&getFieldValByName("updateTime",metaObject)==null) {
            setUpdateFieldValByName("updateTime", LocalDateTime.now(), metaObject);
        }
    }
}
