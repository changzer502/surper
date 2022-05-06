package com.subway.ad.controller;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.subway.ad.entity.AdInfo;
import com.subway.ad.entity.PageDto;
import com.subway.ad.service.AdInfoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * @author lingqu
 * @date 2022/4/30
 * @apiNote
 */
@Controller
public class AdController {
    @Autowired
    private AdInfoService adInfoService;

    @GetMapping("/index")
    public String toAd() {
        return "index";
    }

    @RequestMapping("/addAd")
    public String addAd(AdInfo adInfo, HttpServletRequest request, HttpServletResponse response, Model model){
        boolean res = adInfoService.addAd(adInfo);
        if (res){
            Object attribute = request.getSession().getAttribute("session-user");
            System.out.println(attribute);
            model.addAttribute("username",attribute);
            return "index";
        }
        return "err";
    }

    @RequestMapping("/updateAd")
    public String pudateAd(AdInfo adInfo, HttpServletRequest request, HttpServletResponse response, Model model){
        boolean res = adInfoService.pudateAd(adInfo);
        if (res){
            Object attribute = request.getSession().getAttribute("session-user");
            System.out.println(attribute);
            model.addAttribute("username",attribute);
            return "index";
        }
        return "err";
    }

    @RequestMapping("/search")
    @ResponseBody
    public List<AdInfo> searchAd(Integer id, HttpServletRequest request, HttpServletResponse response, Model model){
        return adInfoService.searchAd(id);
    }

    @RequestMapping("/deleteAd")
    @ResponseBody
    public Page<AdInfo> deleteAd(PageDto pageDto, Integer id, HttpServletRequest request, HttpServletResponse response, Model model){
        boolean res = adInfoService.deleteAd(id);
        if (res){
            return adInfoService.getAllAd(pageDto);
        }
        return null;
    }

    @GetMapping("/ad")
    @ResponseBody
    public Page<AdInfo> getAllAd(PageDto pageDto){
        return adInfoService.getAllAd(pageDto);
    }

}
