php-pinyin
==========

A PHP extension converting Chinese character to Pinyin. 

一个来自百度的汉字转拼音PHP扩库，其他的汉字转拼音方案存在两个问题：

1. 可转的汉字数有限，几千个左右
2. 不能解决多音字问题

很抱歉，由于授权问题，目前我们无法提供libpinyin库及其词典！

Install
-----------
1. $ cd /path/to/php-pinyin
2. $ /home/work/local/php/bin/phpize
3. $ ./configure --with-php-config=/home/work/local/php/bin/php-config --with-baidu-pinyin=/home/work/local/pinyin
4. $ make
5. $ make install


Usage
---------
```php
$user = get_current_user();
$obj  = new Pinyin();

//$obj->loadDict("/home/work/local/pinyin/dict/dict_tone.dat", Pinyin::TY_TONE_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/dict.dat", Pinyin::TY_DICT);
//$obj->loadDict("/home/work/local/pinyin/dict/dyz_tone.dat", Pinyin::DYZ_TONE_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/dyz.dat", Pinyin::DYZ_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/duoyong.dat", Pinyin::DY_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/dz_pro.dat", Pinyin::BME_DICT);
// var_dump($obj);

var_dump($obj->convert(iconv("UTF-8", "GBK", "重庆重量")));
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆南京市长江大桥财务会议会计"))));
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆"), iconv("UTF-8", "GBK", "重量"))));
var_dump($obj->exactConvert(iconv("UTF-8", "GBK", "中华人民共和国")));

```

Resulte will be:
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
