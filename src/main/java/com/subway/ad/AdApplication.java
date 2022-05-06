package com.subway.ad;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@MapperScan("com.subway.ad.mapper")
public class AdApplication {

    public static void main(String[] args) {
        SpringApplication.run(AdApplication.class, args);
    }

}
