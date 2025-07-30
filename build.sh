#!/bin/bash
# 设置默认版本号
DEFAULT_VERSION="9.9.9"

# 检查第一个命令行参数($1)是否存在。如果不存在，则使用默认版本。
# -z "$1" 用于判断字符串$1是否为空
if [ -z "$1" ]; then
  VERSION=$DEFAULT_VERSION
else
  VERSION=$1
fi

echo "Building with version: $VERSION"

# 导出环境变量并执行构建命令
export LZMA_VERSION=$VERSION
python -m build
