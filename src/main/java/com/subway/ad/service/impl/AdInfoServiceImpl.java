package com.subway.ad.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.subway.ad.entity.AdInfo;
import com.subway.ad.entity.PageDto;
import com.subway.ad.mapper.AdInfoMapper;
import com.subway.ad.service.AdInfoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 *
 */
@Service
public class AdInfoServiceImpl extends ServiceImpl<AdInfoMapper, AdInfo>
    implements AdInfoService {

    @Autowired
    private AdInfoMapper adInfoMapper;

    @Override
    public boolean addAd(AdInfo adInfo) {
        int insert = adInfoMapper.insert(adInfo);
        return insert>0;
    }

    @Override
    public Page<AdInfo> getAllAd(PageDto pageDto) {
        Page<AdInfo> page = new Page<>(pageDto.getPageNum(),pageDto.getPageSize());
        QueryWrapper<AdInfo> queryWrapper = new QueryWrapper<>();
        queryWrapper.orderByAsc("ad_id");
        List<AdInfo> page1 = adInfoMapper.selectPages(pageDto.getPageNum()*pageDto.getPageSize(),pageDto.getPageSize());
        Page<AdInfo> page2 = adInfoMapper.selectPage(page, queryWrapper);
        page2.setRecords(page1);
        return page2;

    }

    @Override
    public boolean pudateAd(AdInfo adInfo) {
        int i = adInfoMapper.updateById(adInfo);
        return i > 0;
    }

    @Override
    public boolean deleteAd(Integer id) {
        int i = adInfoMapper.deleteById(id);
        return i > 0;
    }

    @Override
    public List<AdInfo> searchAd(Integer adId) {
        QueryWrapper<AdInfo> queryWrapper = new QueryWrapper<AdInfo>();
        queryWrapper.eq("ad_id", adId);
        return adInfoMapper.selectList(queryWrapper);
    }
}




