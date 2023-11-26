#!/bin/bash

# 指定可执行文件的路径
# ../bin/executable"
echo
echo "### See far travel farther 元 ###"
echo

# 使用shell命令获取当前目录的绝对路径
CURRENT_PATH=$(pwd)

# 使用shell命令获取上一级目录的路径 Project目录
PRJ_PATH=$(dirname $CURRENT_PATH)

# 获取bin目录路径
BIN_PATH=$(cd $PRJ_PATH/bin && pwd)

# 获取执行文件的绝对路径
EXECUTABLE="$BIN_PATH/executable"
echo "$EXECUTABLE"
echo
echo "################# run executable start $(date) ###################"
# 执行bin目录下的文件
$EXECUTABLE

echo "################# run executable end $(date) ###################"
