<?php
$obj = new Pinyin();
$obj->loadDict("/home/work/local/pinyin/dict/dict.dat", Pinyin::TONE_DICT);
var_dump($obj->convert(iconv("UTF-8", "GBK", "重庆重量")));
?>
