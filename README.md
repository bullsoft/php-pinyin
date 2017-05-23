php-pinyin
==========

A PHP extension converting Chinese characters to Pinyin. We refactor this by using PHP-CPP, older version please checkout `legacy` branch.

一个来自百度的汉字转拼音PHP扩展，其他的汉字转拼音方案存在两个问题：

1. 可转的汉字数有限，几千个左右
2. 不能解决多音字问题

Install
-----------
1. Install [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP) or its [LEGACY Version](https://github.com/CopernicaMarketingSoftware/PHP-CPP-LEGACY). Before that, you need to change the Makefile,,, because PHP-CPP was written with C++11, but libpinyin was written with C++98,,, So you should build PHP-CPP with `-D_GLIBCXX_USE_CXX11_ABI=0` option, which means "Do not use Cxx11's Application Binary Interface"
2. $ cd /path/to/php-pinyin/cpp-ext
3. $ make
4. $ make install


Usage
---------
```php
$obj  = new Pinyin();
var_dump($obj->convert(iconv("UTF-8", "GBK", "重庆重量")));
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆南京市长江大桥财务会议会计"))));
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆"), iconv("UTF-8", "GBK", "重量"))));
var_dump($obj->exactConvert(iconv("UTF-8", "GBK", "中华人民共和国")));
```

Results will be:
```php
array(1) {
  [0] =>
  string(22) "chong'qing'zhong'liang"
}
array(1) {
  [0] =>
  string(65) "chong'qing'nan'jing'shi'chang'jiang'da'qiao'cai'wu'hui'yi'kuai'ji"
}
array(2) {
  [0] =>
  string(10) "chong'qing"
  [1] =>
  string(11) "zhong'liang"
}
array(1) {
  [0] =>
  string(29) "zhong'hua'ren'min'gong'he'guo"
}
```

If you want to get the Abbr. of the whole pinyin-string, you can simply do this:

```php
echo preg_replace("/\'([a-zA-Z])[0-9a-zA-Z]*/e", "strtoupper('$1')", "'".$py_string);
```

If you want to customize dict-files yourself and then convert them to binary-format again, do it like this:
```php
$result = $obj->generateDict("/home/work/local/pinyin/dict/dict.txt", "/home/work/tmp/dict.dat");

if($result) echo "Generate complete";
```

Feedback
---------

Issues and contributions are welcome.

Thank you!
