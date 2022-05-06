package com.subway.ad.controller;

import com.subway.ad.dot.LoginDot;
import com.subway.ad.service.LoginService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

/**
 * @ClassName: LoginController
 * @Description: QQ登录控制器
 * @author: xw
 * @date 2020/2/21 10:54
 * @Version: 1.0
 **/
@Controller
public class LoginController {

    @Autowired
    private LoginService loginService;

    /**
     * @Description 访问项目根目录跳转到登录页面
     * @Author xw
     * @Date 11:25 2020/2/21
     * @Param []
     * @return java.lang.String
     **/
    @RequestMapping({"/","toLogin"})
    public String login(){
        return "login";
    }

    @RequestMapping("/toRegister")
    public String register(){
        return "register";
    }


    @RequestMapping("/login")
    public String toLogin(LoginDot loginDot, HttpServletRequest request, HttpServletResponse response, Model model) throws IOException {
        if("".equals(loginDot.getUsername())){
            model.addAttribute("msg","用户名不能为空");
            return "login";
        }else if("".equals(loginDot.getPassword())){
            model.addAttribute("msg","密码不能为空");
            return "login";
        }
        boolean login = loginService.toLogin(loginDot);
        if (login){
            request.getSession().setAttribute("session-user",loginDot.getUsername());
            model.addAttribute("username",loginDot.getUsername());
            return "index";
        }else {
            model.addAttribute("msg","用户名或密码错误");
            return "login";
        }

    }


    @RequestMapping("/register")
    public String toRegister(LoginDot loginDot, HttpServletRequest request, HttpServletResponse response, Model model){
        if("".equals(loginDot.getUsername())){
            model.addAttribute("msg","用户名不能为空");
            return "register";
        }else if("".equals(loginDot.getPassword())){
            model.addAttribute("msg","密码不能为空");
            return "register";
        }
        boolean login = loginService.toRegister(loginDot);
        if (!login){
            model.addAttribute("msg","用户已存在");
            return "register";
        }
        return "login";
    }


}
