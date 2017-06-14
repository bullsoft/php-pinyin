php-pinyin
==========

A PHP extension converting Chinese characters to Pinyin.

一个来自百度的汉字转拼音PHP扩展，其他的汉字转拼音方案存在两个问题：

1. 可转的汉字数有限，几千个左右
2. 不能解决多音字问题

Installation
============

Currently you have two ways to use php-pinyin. One depends on PHP-CPP, while another one is plain php extenstion which works with php 7.x. (For php 5.x support, please checkout the branch `legacy`)

## Method with PHP-CPP

Main improvements:
  - Depend PHP-CPP, an awesome library which wrapper Zend Engine with friendly api
  - Support PHP 7
  - This time we support `UTF-8` and `GBK` encoding
  - Add ini_setting (`pinyin.dict_path` and `pinyin.dict_tone`), you shoud not loadDict yourself.

### Install

1. Install [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP) or its [LEGACY Version](https://github.com/CopernicaMarketingSoftware/PHP-CPP-LEGACY). Before that, you need to change the Makefile,,, because PHP-CPP was written with C++11, but libpinyin was written with C++98,,, So you should build PHP-CPP with `-D_GLIBCXX_USE_CXX11_ABI=0` option, which means "Do not use Cxx11's Application Binary Interface"
2. cd /path/to/php-pinyin/cpp-ext
3. make
4. make install

## Method without PHP-CPP

This is upgraded from old php-pinyin for php 5.x.

### Install

1. $ cd /path/to/php-pinyin
2. $ /path/to/php/bin/phpize
3. $ ./configure --with-php-config=/path/to/php/bin/php-config --with-baidu-pinyin=/path/to/pinyin
4. $ make
5. $ make install

Here `/path/to/pinyin` is the directory where you copied `libpinyin` to.

Usage
-----

```php
$obj  = new Pinyin();

// UTF-8
var_dump($obj->convert("重庆重量"));
var_dump($obj->multiConvert(array("重庆南京市长江大桥财务会议会计")));

// GBK
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆"), iconv("UTF-8", "GBK", "重量"))));
```

Results will be:
```php
string(22) "chong'qing'zhong'liang"
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

This lib only support Chinese characters and english letters, or else it will return `false`. So you can write a `safeConvert` function to avoid this.

```php
$p = new Pinyin();
function safeConvert($word, $pyOnly = true) {
    global $p;
    // UTF-8 regex for Chinese
    $result = preg_match_all("/([\x{4e00}-\x{9fa5}]+)/iu", $word, $matches);
    if(!$result) {
        throw new \Exception("No Chinese characters in word");
    }

    $pys = $p->multiConvert($matches[1]);
    if($pyOnly == true) {
        return implode("'", $pys);
    } else {
        return str_replace($matches[1], $pys, $word);
    }
}
```

If you want to customize dict-files yourself and then convert them to binary-format again, do it like this:
```php
$result = $obj->generateDict("/home/work/local/pinyin/dict/dict.txt", "/home/work/tmp/dict.dat");

if($result) echo "Generate complete";
```

Feedback
--------

Issues and contributions are welcome.

Thank you!
