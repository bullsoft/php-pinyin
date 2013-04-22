<?php
$obj = new Pinyin();
// $obj->loadDict("/home/work/local/pinyin/dict/dict_tone.dat", Pinyin::TY_TONE_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/dict.dat", Pinyin::TY_DICT);
// $obj->loadDict("/home/work/local/pinyin/dict/dyz_tone.dat", Pinyin::DYZ_TONE_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/dyz.dat", Pinyin::DYZ_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/duoyong.dat", Pinyin::DY_DICT);
$obj->loadDict("/home/work/local/pinyin/dict/dz_pro.dat", Pinyin::BME_DICT);
var_dump($obj->convert(iconv("UTF-8", "GBK", "重庆重量")));
?>
