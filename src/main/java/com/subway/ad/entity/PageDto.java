package com.subway.ad.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @author lingqu
 * @date 2022/5/2
 * @apiNote
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class PageDto {
    private Integer pageNum;
    private Integer pageSize;
}
