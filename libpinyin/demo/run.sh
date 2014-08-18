#!/bin/sh

make

chmod +x ./zhuyinDynamic
chmod +x ./zhuyinStatic

echo ""

echo "Test zhuyinDynamic:"
export LD_LIBRARY_PATH=../
cat input.txt | ./zhuyinDynamic ../dict 1

echo ""

echo  "Test zhuyinStatic:"
cat input.txt | ./zhuyinStatic ../dict 1
