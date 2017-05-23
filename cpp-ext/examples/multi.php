<?php
$p = new Pinyin("");

$a = [
    '我们是共产主义接班人',
    "重庆会计会议",
];
var_dump($p->convert($a[0]));
var_dump($p->multiConvert($a));


