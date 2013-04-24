<?php
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
