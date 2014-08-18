<?php
$user = get_current_user();

$obj  = new Pinyin();
$obj->loadDict("/home/$user/local/pinyin/dict/dict.dat", Pinyin::TY_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/dyz.dat", Pinyin::DYZ_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/duoyong.dat", Pinyin::DY_DICT);
$obj->loadDict("/home/$user/local/pinyin/dict/dz_pro.dat", Pinyin::BME_DICT);

//$obj->loadDict("/home/work/local/pinyin/dict/dict_tone.dat", Pinyin::TY_TONE_DICT);
//$obj->loadDict("/home/work/local/pinyin/dict/dyz_tone.dat", Pinyin::DYZ_TONE_DICT);

$str = "大家会计60好80";

$str = preg_replace('/(\w+)/', '\'$1\'', $str);
$str = trim($str, "'");
var_dump($str);
preg_match_all('/([\x{4e00}-\x{9fa5}]+)/iu', $str, $matches);

$gbkItems = array();
foreach($matches[1] as $item) {
    $gbkItems[] = iconv("UTF-8", "GBK", $item);
}

$pinyinItems = $obj->multiConvert($gbkItems);

var_dump($matches[1], $gbkItems, $pinyinItems);

var_dump($result = str_replace($matches[1], $pinyinItems, $str));


exit;






var_dump($obj->convert(iconv("UTF-8", "GBK", "重庆重量")));
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆南京市长江大桥财务会议会计"))));
var_dump($obj->multiConvert(array(iconv("UTF-8", "GBK", "重庆"), iconv("UTF-8", "GBK", "重量"))));
var_dump($obj->exactConvert(iconv("UTF-8", "GBK", "中华人民共和国")));

function getPinyin($chars)
{
    
}


// $reult = $obj->generateDict("/home/work/local/pinyin/dict/dict.txt", "/home/work/tmp/dict.dat");
// if ($reult) {
//     echo PHP_EOL . "Generate complete." . PHP_EOL;
// }
