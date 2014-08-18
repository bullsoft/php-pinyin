all: zhuyinStatic zhuyinDynamic

# 静态链接.a文件
zhuyinStatic:
	g++ -static zhuyin.cpp -o zhuyinStatic -lPYNotation -L../lib -I../include

# 动态链接.a文件
zhuyinDynamic:
	g++ zhuyin.cpp -o zhuyinDynamic -lPYNotation -L../lib -I../include

clean:
	rm -f zhuyinStatic zhuyinDynamic

.PHONY: zhuyinStatic zhuyinDynamic
