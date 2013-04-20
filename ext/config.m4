dnl $Id$
dnl config.m4 for extension pinyin

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(pinyin, for pinyin support,
dnl Make sure that the comment is aligned:
[  --with-pinyin             Include pinyin support])

PHP_ARG_WITH(baidu-pinyin, for baidu-pinyin library support,
dnl Make sure that the comment is aligned:
[  --with-baidu-pinyin             Include baidu pinyin support])


dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(pinyin, whether to enable pinyin support,
dnl Make sure that the comment is aligned:
dnl [  --enable-pinyin           Enable pinyin support])

if test "$PHP_PINYIN" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-pinyin -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR_BAIDU_PINYIN="/include/IPYNotation.h"  # you most likely want to change this
  if test -r $PHP_BAIDU_PINYIN/$SEARCH_FOR_BAIDU_PINYIN; then # path given as parameter
     PINYIN_DIR=$PHP_BAIDU_PINYIN
  else # search default path list
     AC_MSG_CHECKING([for pinyin files in default path])
     for i in $SEARCH_PATH ; do
       if test -r $i/$SEARCH_FOR_BAIDU_PINYIN; then
         PINYIN_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
  fi
  
  dnl
  if test -z "$PINYIN_DIR"; then
     AC_MSG_RESULT([not found])
     AC_MSG_ERROR([Please reinstall the baidu pinyin distribution])
  fi

  PHP_REQUIRE_CXX()

  PHP_ADD_INCLUDE($PINYIN_DIR/include)
  PHP_ADD_INCLUDE($PINYIN_DIR)

  PHP_ADD_LIBPATH($PINYIN_DIR"/lib")
  PHP_ADD_LIBPATH($PINYIN_DIR)

  PHP_ADD_LIBRARY(PYNotation, 1, PINYIN_SHARED_LIBADD)
  
  PHP_SUBST(PINYIN_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pinyin, pinyin.cpp, $ext_shared)
fi
