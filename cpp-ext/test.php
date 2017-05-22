<?php
dl('pinyin.so');

$master = new Pinyin();

$code = <<<EOD
import tushare as ts
df = ts.get_tick_data('600848',date='2017-01-09')
df1 = df.head(10)
json = df1.to_json()
a = 6

df1 = ts.get_sina_dd('600848', date='2017-05-05') #默认400手
t = type(df1)
EOD;

echo "waiting for ... " . time(). PHP_EOL;
$master->eval($code);
//$json = $master->var("json");
//var_dump(json_decode($json, true));

//$master->eval("print(t)");
//$master->print("df");
$master->print("df1");
//$master->print(t);
echo time() . PHP_EOL;
echo "fasdfas....." . PHP_EOL;

$m = new Python();
$m->eval("print('aaaa111111')");

