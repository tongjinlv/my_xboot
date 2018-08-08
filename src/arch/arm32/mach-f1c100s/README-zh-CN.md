# 全志F1C100S

***

## 编译源码，生成的目标文件在output目录下
```shell
make clean
make CROSS_COMPILE=/path/to/arm-eabi- PLATFORM=arm32-f1c100s
```

## 烧写到RAM中并运行
```shell
sunxi-fel spl xboot.bin; sunxi-fel -p write 0x80000000 xboot.bin; sunxi-fel exec 0x80000000;
```

## 烧写普通镜像到SPI Flash
```shell
sunxi-fel -p spiflash-write 0 xboot.bin
```

## 烧写压缩镜像到SPI Flash
```shell
sunxi-fel -p spiflash-write 0 xboot.bin.z
```

## 关于sunxi-fel工具

sunxi-fel工具默认并没有支持F1C100S，这里需要自行下载源码并编译

## 下载sunxi-fel源码
```shell
git clone https://github.com/Icenowy/sunxi-tools.git
git checkout -b f1c100s-spiflash origin/f1c100s-spiflash
```

## 编译安装sunxi-fel
```shell
make
sudo make install
```

# 相关网址

- XBOOT官方首页
  https://xboot.github.io/

- XBOOT源码(完全开源)，寄存在github，请直接`fork`一份代码，如需申请合并，直接`pull request`即可
  https://github.com/xboot/xboot

- XBOOT运行效果展示页面，首次接触的用户，建议观看，对XBOOT有个直观的感受
  https://github.com/xboot/xboot/blob/master/documents/xboot-show-case.md

- XBOOT系统开发指南
  https://github.com/xboot/xboot/blob/master/documents/xboot-system-development-guide.md

- XBOOT应用开发指南
  https://github.com/xboot/xboot/blob/master/documents/xboot-application-development-guide.md

- XBOOT官方QQ群，大佬聚集，请踊跃加入
  [658250248](https://jq.qq.com/?_wv=1027&k=5BOkXYO) (2000人)