package com.subway.ad.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.subway.ad.dot.LoginDot;
import com.subway.ad.entity.UserInfo;
import com.subway.ad.mapper.UserInfoMapper;
import com.subway.ad.service.LoginService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * @author lingqu
 * @date 2022/4/30
 * @apiNote
 */

@Service
public class LoginServiceImpl implements LoginService {
    @Autowired
    private UserInfoMapper userInfoMapper;
    @Override
    public boolean toLogin(LoginDot loginDot) {
        LambdaQueryWrapper<UserInfo> queryWrapper = new LambdaQueryWrapper<>();
        queryWrapper.eq(UserInfo::getUsername, loginDot.getUsername());
        UserInfo userInfo = userInfoMapper.selectOne(queryWrapper);
        if (userInfo!=null){
            if (userInfo.getPassword().equals(loginDot.getPassword())){
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean toRegister(LoginDot loginDot) {
        LambdaQueryWrapper<UserInfo> queryWrapper = new LambdaQueryWrapper<>();
        queryWrapper.eq(UserInfo::getUsername, loginDot.getUsername());
        UserInfo userInfo = userInfoMapper.selectOne(queryWrapper);
        if (userInfo!=null){
            return false;
        }
        UserInfo user = new UserInfo();
        user.setUsername(loginDot.getUsername());
        user.setPassword(loginDot.getPassword());
        int insert = userInfoMapper.insert(user);
        return insert>0;
    }
}
