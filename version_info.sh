#!/usr/bin/env bash

# 获取当前仓库的 Git 哈希值
git_hash=$(git rev-parse --short HEAD 2>/dev/null)

# 获取当前仓库的 Git 版本标签
git_tag=$(git describe --tags --abbrev=0 2>/dev/null)

# 获取当前的 Git 分支名
git_branch="master"

# 获取当前仓库最新提交的时间
git_commit_time=$(git log -1 --format=%cd --date='format:%Y-%m-%d %H:%M:%S' 2>/dev/null)

# 获取中国时区的当前时间作为构建时间
build_time=$(TZ='Asia/Shanghai' date '+%Y-%m-%d %H:%M:%S')

# 生成 version.cpp 文件
cat > version.cpp <<EOF
#include "version.h"

namespace version{
    constexpr std::string_view git_hash = "$git_hash";
    constexpr std::string_view git_tag = "$git_tag";
    constexpr std::string_view git_branch = "$git_branch";
    constexpr std::string_view git_commit_time = "$git_commit_time";
    constexpr std::string_view build_time = "$build_time";
};
EOF

# 提示生成成功
echo "version.cpp 文件已生成:"
cat version.cpp
