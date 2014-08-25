dnl $Id$
dnl config.m4 for extension pinyin

PHP_ARG_WITH(baidu-pinyin, for baidu-pinyin library support,
[  --with-baidu-pinyin=DIR   Include baidu pinyin support])

if test "$PHP_BAIDU_PINYIN" != "no"; then

  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR_BAIDU_PINYIN="/include/IPYNotation.h"
  if test -r $PHP_BAIDU_PINYIN/$SEARCH_FOR_BAIDU_PINYIN; then
     PINYIN_DIR=$PHP_BAIDU_PINYIN
  else
     AC_MSG_CHECKING([for baidu pinyin files in default path])
     for i in $SEARCH_PATH ; do
       if test -r $i/$SEARCH_FOR_BAIDU_PINYIN; then
         PINYIN_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
  fi
  
  dnl check if we found pinyin include files
  if test -z "$PINYIN_DIR"; then
     AC_MSG_RESULT([not found])
     AC_MSG_ERROR([Please check if baidu pinyin library is specified correctly])
  fi

  PHP_REQUIRE_CXX()

  PHP_ADD_INCLUDE($PINYIN_DIR"/include")
  PHP_ADD_INCLUDE($PINYIN_DIR)

  PHP_ADD_LIBPATH($PINYIN_DIR"/lib")

  PHP_ADD_LIBRARY(PYNotation, 1, PINYIN_SHARED_LIBADD)
  
  PHP_SUBST(PINYIN_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pinyin, pinyin.cpp, $ext_shared)
fi
