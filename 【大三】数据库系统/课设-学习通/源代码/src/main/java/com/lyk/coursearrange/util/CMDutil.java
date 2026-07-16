package com.lyk.coursearrange.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

/**
 * @version: V1.0
 * @author: songyan
 * @className: CMDUtil
 * @packageName: com.googosoft.utils
 * @description: cmd命令执行工具
 * @date: 2020/10/24   10:19
 */
public class CMDutil {

    /**
     * @author: songyan
     * @methodsName: runCommand
     * @param command
     * @param dir
     * @description: 在指定目录执行指定命令
     * @return: java.util.Map<java.lang.String,java.lang.Object>
     */
    public static Map<String, Object> runCommand(String command, String dir) {
        Map<String, Object> result = new HashMap<String, Object>();
        boolean flag = true;
        StringBuilder msg = new StringBuilder();
        BufferedReader bufferedReader = null;
        Process proc = null;
        ProcessBuilder pb = new ProcessBuilder("cmd ", "/c", command);
        pb.directory(new File(dir));
        try {
            proc = pb.start();
            bufferedReader = new BufferedReader(new InputStreamReader(proc.getErrorStream(), "gbk"));
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                msg.append(line);
            }
            proc.waitFor();
        } catch (Exception ex) {
            ex.printStackTrace();
        } finally {
            if (bufferedReader != null) {
                try {
                    bufferedReader.close();
                } catch (Exception ex) {
                }
            }
            if(proc!=null){
                proc.destroy();
            }
        }
        result.put("flag", flag);
        result.put("msg", msg);
        return result;
    }

}