package com.subway.ad.service;

import com.subway.ad.dot.LoginDot;

/**
 * @author lingqu
 * @date 2022/4/30
 * @apiNote
 */
public interface LoginService {
    boolean toLogin(LoginDot loginDot);

    boolean toRegister(LoginDot loginDot);
}
