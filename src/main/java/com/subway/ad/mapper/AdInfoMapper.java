package com.subway.ad.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.subway.ad.entity.AdInfo;
import org.apache.ibatis.annotations.Param;
import org.springframework.stereotype.Component;

import java.util.List;

/**
 * @Entity com.subway.ad.entity.AdInfo
 */
@Component
public interface AdInfoMapper extends BaseMapper<AdInfo> {

    //List<AdInfo> selectPages(PageDto page);

    List<AdInfo> selectPages(@Param("pageNum") Integer pageNum,@Param("pageSize") Integer pageSize);
}




