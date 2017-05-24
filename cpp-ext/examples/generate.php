<?php
$p = new Pinyin();
$txt = __DIR__ . "/duoyong.txt";
$dat = __DIR__ . "/dy.dat";
if($p->generateDict($txt, $dat)) {
    echo "Generate complete" . PHP_EOL;
}

