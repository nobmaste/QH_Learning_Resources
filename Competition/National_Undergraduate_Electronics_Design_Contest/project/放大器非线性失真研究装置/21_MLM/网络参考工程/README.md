# 电赛2020E题

#### 介绍
电子设计竞赛E题程序共享

#### 软件架构
开发芯片 STM32F103ZET6

#### 使用说明

PC1进行测量采集，显示屏需要根据自己的开发板进行移植
波形显示图是根据实测进行拟合的图像，根据需要进行修改即可。

定时器版本---
该版本进行利用了定时器进行定时，实现测量功能，显示波形、THD。频谱，波形频谱根据实际显示需要可以调整大小，画图设计模块都基本单独封装成函数，方便修改参数，调整位置。后同，PA1端口进行测试


DMA版本---
 DMA fft 画图 自动 继电器控制 全部显示最终版本
采用DMA进行传输数据，定时器产生pwm模拟输出采样时钟，精度相比定时器版本单次采样更高。PC1端口进行测试
最终版本为最后提交作品的版本，精度足够，在电压采集同时进行了拟合（板子问题），THD没有经过拟合效果良好。
上电后进行自动循环测量，通过按键可以控制停止和单次状态循环测量手动切换失真状态。
自动测量模式下进行5次测量求平均THD，单次循环模式下，只显示单次测量THD



#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
