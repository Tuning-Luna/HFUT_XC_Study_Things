package com.lyk.coursearrange.entity.response;

import com.lyk.coursearrange.entity.LocationInfo;
import lombok.Data;

/**
 * @Descripe:
 */
@Data
public class LocationVO extends LocationInfo {

    private String teachBuildName;

    private String gradeName;
}
