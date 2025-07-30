@echo off
:: LZMA_VERSION从参数传入,不然默认为9.9.9

rem 默认版本号
set "DEFAULT_VERSION=9.9.9"

rem 如果第一个参数为空，则使用默认版本号
if "%1"=="" (
    set "LZMA_VERSION=%DEFAULT_VERSION%"
) else (
    set "LZMA_VERSION=%1"
)

echo Building with version: %LZMA_VERSION%
python -m build