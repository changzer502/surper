function alert() {
    layer.msg("暂未开放，请使用QQ登录");
}

/**
 * APP中文名称
 * @returns {string}
 */
function APPLICATION_CN_NAME() {
    return "莫提网盘";
}

/**
 * APP英文名称
 * @returns {string}
 */
function APPLICATION_EN_NAME() {
    return "moti-cloud";
}

/**
 * 注册
 */
function register() {
    let name = $.trim($("#register-name").val());
    let email = $.trim($("#register-email").val());
    let password = $.trim($("#register-password").val());
    let confirm = $.trim($("#password-confirm").val());
    if (!name) {
        layer.msg("用户名不能为空");
        return;
    }
    if (!userNameReg(name)) {
        layer.msg("用户名格式错误，支持中文英文");
        return
    }
    if (!email) {
        layer.msg("邮箱不能为空");
        return;
    }
    if (!emailReg(email)) {
        layer.msg("请输入正确的邮箱格式");
        return;
    }
    if (!password) {
        layer.msg("密码不能为空");
        return;
    }
    if (!userPasswordReg(password)) {
        layer.msg("请输入8-16位字符，支持英文数字下划线");
        return;
    }
    if (!confirm) {
        layer.msg("确认密码不能为空");
        return;
    }
    if (password != confirm) {
        layer.msg("两次密码不一致");
        return;
    }
    if (!$("#user-notice").is(":checked")) {
        layer.msg("请认真阅读并勾选《用户条例》");
        shake("#user-notice-text");
        return;
    }
    $.ajax({
        type: "POST",
        url: "system/register",
        data: {userName: name, userEmail: email, userPassword:password},
        dataType: "json",
        success: function (data) {
            console.log(data);
            layer.msg(data['message']);
            if (data['code'] === "EXIST_NAME"){
                shake("#register-name")
            }else if (data['code'] === "EXIST_EMAIL"){
                shake("#register-email")
            }else {
                reloadTo("/confirm-mail.html?email="+email)
            }
        }
    });
}

/**
 * 重新发送激活邮件
 * @param email
 */
function reSendActivateMail(email) {
    $.ajax({
        type: "POST",
        url: "system/reSendActivateMail",
        data: {userEmail: email},
        dataType: "json",
        success: function (data) {
            console.log(data);
            layer.msg(data['message']);
            if (data['code'] === "SUCCESS"){
                // 发送成功，将按钮设置一分钟倒计时
                time("#resend-email", 60);
            }
        }
    });
}

/**
 * 登陆
 */
function login() {
    let account = $.trim($("#login-account").val());
    let password = $.trim($("#login-password").val());
    let remember = $("#remember-me").is(":checked");
    if (!account) {
        layer.msg("登陆账户不能为空");
        return;
    }
    if (!password) {
        layer.msg("密码不能为空");
        return;
    }
    if (!userPasswordReg(password)) {
        layer.msg("请输入8-16位字符，支持英文数字下划线");
        return;
    }
    $.ajax({
        type: "POST",
        url: "system/login",
        data: {account: account, password:password, remember: remember},
        dataType: "json",
        success: function (data) {
            console.log(data);
            layer.msg(data['message']);
            if (data['code'] === "UN_REGISTER"){
                setTimeout("reloadTo('/register.html')", 1000);
            }else if (data['code'] === "ERROR_PASSWORD"){
                shake("#login-password")
            }else if (data['code'] === "UN_ACTIVATE"){
                let handler = "reloadTo('/confirm-mail.html?email="+data['data']+"')";
                setTimeout(handler, 1000);
            }else {
                setTimeout("reloadTo('/dashboard.html')", 1000);
            }
        }
    });
}

/**
 * 用户条例
 */
function userNeedKnow() {
    let appName = APPLICATION_CN_NAME();
    let notice = `<div style='padding:10px 30px;'>
                    <p>请认真阅读以下条例</p>
                    <ul>
                        <li>1. xxxx</li>
                        <li>2. xxxx</li>
                        <li>3. xxxx</li>
                        <li>4. xxxx</li>
                        <li>5. xxxx</li>
                    </ul>
                  </div>`;

    layer.open({
        type: 1, // Page层类型
        area: ['1000px', '600px'],
        title: "《" + appName + "用户条例》",
        shade: 0.6, // 遮罩透明度
        anim: 1, // 0-6的动画形式，-1不开启
        content: notice,
        btn: ["我已阅读"],
        yes: function (index) {
            $('#user-notice').prop('checked', true);
            //如果设定了yes回调，需进行手工关闭
            layer.close(index);
        }
    });
}

/**
 * 抖动函数
 * @param o
 */
function shake(id) {
    let $panel = $(id);
    let box_left = 0;
    $panel.css({'left': box_left, 'position': 'relative'});
    for (let i = 0; i < 4; i++) {
        $panel.animate({left: box_left - (20 - 5 * i)}, 30);
        $panel.animate({left: box_left + (20 - 5 * i)}, 30);
    }
}

/**
 * 刷新页面
 */
function reload() {
    window.location.reload();
}

/**
 * 跳转到指定页面
 * @param url
 */
function reloadTo(url) {
    let appCnName = APPLICATION_EN_NAME();
    let href = window.location.href;
    href = href.split("/"+appCnName)[0] + "/" + appCnName + url;
    window.location.href = href;
}


/**
 * 60秒倒计时
 * @param o
 */
function time(o, wait) {
    if (wait === 0) {
        $(o).attr("disabled", false);
        $(o).html("重新发送");
    } else {
        $(o).attr("disabled", true);
        $(o).html(wait + "秒后重新发送");
        wait--;
        setTimeout(function () {time(o, wait);},1000);
    }
}