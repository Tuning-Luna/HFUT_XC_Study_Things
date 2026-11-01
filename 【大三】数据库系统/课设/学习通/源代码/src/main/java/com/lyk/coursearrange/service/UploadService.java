package com.lyk.coursearrange.service;

import com.lyk.coursearrange.common.ServerResponse;
import org.springframework.web.multipart.MultipartFile;


public interface UploadService {

    ServerResponse upload(MultipartFile file);

}
