package com.subway.ad.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 *
 * @TableName ad_info
 */
@TableName(value ="ad_info")
@Data
public class AdInfo implements Serializable {
    /**
     *
     */
    @TableId
    private Long adId;

    private String title;

    private BigDecimal price;

    private String des;

    private Long userId;

    private String ingredient;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}
