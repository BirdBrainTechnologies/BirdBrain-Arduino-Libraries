@echo OFF

reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS=32BIT || set OS=64BIT

cd files\WindowsDrivers
if %OS%==32BIT HummingbirdDuoDriverInstall32_bit.exe
if %OS%==64BIT HummingbirdDuoDriverInstall64_bit.exe