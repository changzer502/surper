package com.subway.ad.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.subway.ad.entity.AdInfo;
import com.subway.ad.entity.PageDto;

import java.util.List;

/**
 *
 */
public interface AdInfoService extends IService<AdInfo> {

    boolean addAd(AdInfo adInfo);

    Page<AdInfo> getAllAd(PageDto pageDto);

    boolean pudateAd(AdInfo adInfo);

    boolean deleteAd(Integer id);

    List<AdInfo> searchAd(Integer adId);
}
