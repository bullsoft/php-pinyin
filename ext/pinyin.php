<?php
$obj = new Pinyin();
$obj->loadDict("/home/work/local/pinyin/dict/dict.dat", Pinyin::TONE_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/dyz.dat", Pinyin::TONE_DYZ_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/duoyong.dat", Pinyin::TONE_DUOYONG_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/dz_pro.dat", Pinyin::BME_DICT);
var_dump($obj->convert(iconv("UTF-8", "GBK", "重庆重量")));
?>
