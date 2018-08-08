@echo off

if exist "%USERPROFILE%\.xboot\sdcard.img" goto RunQemu
    mkdir "%USERPROFILE%\.xboot"
    unzip.exe sdcard.zip -d "%USERPROFILE%\.xboot"

:RunQemu
qemu-system-aarch64.exe -machine virt -cpu cortex-a57 -m 256M -name "ARM Virtual Machine For Cortex-A57" -S -gdb tcp::10000,ipv4 -show-cursor -rtc base=localtime -serial vc -sd "%USERPROFILE%\.xboot\sdcard.img" -kernel ..\..\..\output\xboot
