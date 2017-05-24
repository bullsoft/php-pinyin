<?php
$p = new Pinyin("", true);

// GB2312
$a = mb_convert_encoding("大家好啊", "GB2312", "UTF-8");

// UTF-8
$b = '我们是共产主义接班人';
$c = "重庆，1重量";

var_dump($p->convert($a));
var_dump($p->convert($b));
$d= new stdClass();
var_dump($p->safeConvert($c, $d));

