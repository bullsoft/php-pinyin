/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Gu Weigang     <guweigang@baidu.com>                         |
  |         Shang Yuanchun <shangyuanchun@baidu.com>                     |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
}

#include "php_pinyin.h"

ZEND_DECLARE_MODULE_GLOBALS(pinyin)

static int le_pinyin_notation_link;
#define PINYIN_NOTATION_LINK_DESC "Pinyin Notation"

zend_class_entry *pinyin_ce;
enum Dict_Type;


static IPYNotation *get_pinyin_notation(zval *cls);

/* {{{ pinyin_methods[]
 *
 * Every user visible method must have an entry in pinyin_methods[].
 */

static zend_function_entry pinyin_methods[] = {
    PHP_ME(Pinyin, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Pinyin, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(Pinyin, loadDict, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Pinyin, convert, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Pinyin, multiConvert, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Pinyin, exactConvert, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
/* }}} */

/* {{{ pinyin_module_entry
 */
zend_module_entry pinyin_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"pinyin",
	pinyin_methods,
	PHP_MINIT(pinyin),
	PHP_MSHUTDOWN(pinyin),
	PHP_RINIT(pinyin),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pinyin),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pinyin),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_PINYIN_VERSION, /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PINYIN
extern "C" {
ZEND_GET_MODULE(pinyin)
}
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pinyin.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pinyin_globals, pinyin_globals)
    STD_PHP_INI_ENTRY("pinyin.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pinyin_globals, pinyin_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_pinyin_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_pinyin_init_globals(zend_pinyin_globals *pinyin_globals)
{
	pinyin_globals->global_value = 0;
	pinyin_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pinyin)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Pinyin", pinyin_methods);
    pinyin_ce = zend_register_internal_class(&ce TSRMLS_CC);

    zend_declare_class_constant_long(pinyin_ce, "TY_DICT", strlen("TY_DICT"), TY_DICT);
    zend_declare_class_constant_long(pinyin_ce, "TY_TONE_DICT", strlen("TY_TONE_DICT"), TY_TONE_DICT);    

    zend_declare_class_constant_long(pinyin_ce, "DYZ_DICT", strlen("DYZ_DICT"), DYZ_DICT);
    zend_declare_class_constant_long(pinyin_ce, "DYZ_TONE_DICT", strlen("DYZ_TONE_DICT"), DYZ_TONE_DICT);

    zend_declare_class_constant_long(pinyin_ce, "DY_DICT", strlen("DY_DICT"), DY_DICT);
    zend_declare_class_constant_long(pinyin_ce, "BME_DICT", strlen("BME_DICT"), BME_DICT);

    zend_declare_property_null(pinyin_ce, ZEND_STRL("_pynotation"), ZEND_ACC_PRIVATE TSRMLS_CC);

    // No destructor handler here, we will destroy IPYNotation in class's destructor
    le_pinyin_notation_link = zend_register_list_destructors_ex(
            NULL, NULL, PINYIN_NOTATION_LINK_DESC, module_number);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pinyin)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pinyin)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pinyin)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pinyin)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pinyin support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

PHP_METHOD(Pinyin, __construct)
{
    IPYNotation *pynotation = IPYFactory::getPYInstance();
    zval        *self       = getThis();
    zval        *z_pinyin;
    MAKE_STD_ZVAL(z_pinyin);

    ZEND_REGISTER_RESOURCE(z_pinyin, pynotation, le_pinyin_notation_link);
    zend_update_property(Z_OBJCE_P(self), self, ZEND_STRL("_pynotation"), z_pinyin TSRMLS_CC);
}

PHP_METHOD(Pinyin, __destruct)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    if (pynotation)
        delete pynotation;
}

PHP_METHOD(Pinyin, loadDict)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    char *path = NULL;
    int len;
    enum Dict_Type dict_type;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &path, &len, &dict_type) == FAILURE) {
        return;
    }

    switch(dict_type) {
        case TY_DICT:
            if(!pynotation->loadDict(path)) RETURN_FALSE;
            break;
        case TY_TONE_DICT:
            if(!pynotation->loadToneDict(path)) RETURN_FALSE;
            break;            
        case DYZ_DICT:
            if(!pynotation->loadDyzDict(path)) RETURN_FALSE;
            break;
        case DYZ_TONE_DICT:
            if(!pynotation->loadToneDyzDict(path)) RETURN_FALSE;
            break;            
        case DY_DICT:
            if(!pynotation->loadDYDict(path)) RETURN_FALSE;
            break;
        case BME_DICT:
            if(!pynotation->loadBMEDict(path)) RETURN_FALSE;
            break;
        default:
            RETURN_FALSE;
            break;
    }
    RETURN_TRUE;
}

PHP_METHOD(Pinyin, convert)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    char *characters   = NULL;
    int  len;
    zend_bool get_tone = 0;
    bool result = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &characters, &len, &get_tone) == FAILURE) {
        RETURN_NULL();
    }

    vector<string> py_result;
    if(get_tone) {
        result = pynotation->convertToTonePY(characters, &py_result);
    } else {
        result = pynotation->convertToPY(characters, &py_result); 
    }
    if(result) {
        array_init(return_value);
        vector<string>::iterator _it = py_result.begin();
        int i = 0;
        for(vector<string>::iterator _sit = py_result.begin(); _sit != py_result.end(); _sit++) {
            add_index_string(return_value, i++, (*_sit).c_str(), 1);
        }
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(Pinyin, multiConvert)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    zval        *strs, **str;
    HashTable   *strshash;
    HashPosition pointer;
    bool         result     = 0;
    int          strs_num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &strs) == FAILURE) {
        RETURN_FALSE;
    }

    vector<string> convert_strs;
    string strtmp;
    strshash = Z_ARRVAL_P(strs);
    for (zend_hash_internal_pointer_reset_ex(strshash, &pointer);
             zend_hash_get_current_data_ex(strshash, (void **)&str, &pointer) == SUCCESS;
             zend_hash_move_forward_ex(strshash, &pointer)) {
        convert_to_string_ex(str);
        strtmp.assign(Z_STRVAL_PP(str), Z_STRLEN_PP(str));
        convert_strs.push_back(strtmp);
    }

    strs_num = zend_hash_num_elements(strshash);
    vector<vector<string> * > py_results;
    py_results.reserve(strs_num);

    // py_result to store one sentence's pinyin result
    // its memory will be released when this method ends
    vector<string> py_result[strs_num];
    for(int i = 0; i < strs_num; i++) {
        py_results.push_back(&py_result[i]);
    }
    result = pynotation->convertToPY(convert_strs, &py_results); 
    if(result) {
        array_init(return_value);
        int i = 0;
        vector<vector<string> * >::iterator it = py_results.begin();
        for(; it != py_results.end(); it++) {
            for (vector<string>::iterator iit = (*it)->begin();
                     iit != (*it)->end();
                     iit++) {
                add_index_string(return_value, i++, (*iit).c_str(), 1);
            }
        }
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(Pinyin, exactConvert)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    char        *str        = NULL;
    int          len;
    zend_bool    get_tone   = 0;
    bool         result     = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &str, &len, &get_tone) == FAILURE) {
        RETURN_FALSE;
    }

    vector<string> py_result;
    if(get_tone) {
        result = pynotation->exactToTonePY(str, &py_result);
    } else {
        result = pynotation->exactToPY(str, &py_result); 
    }
    if(result) {
        array_init(return_value);
        int i = 0;
        for(vector<string>::iterator it = py_result.begin(); it != py_result.end(); it++) {
            add_index_string(return_value, i++, (*it).c_str(), 1);
        }
    } else {
        RETURN_FALSE;
    }
}

static IPYNotation *get_pinyin_notation(zval *cls)
{
    zval        *pylink;
    IPYNotation *pynotation;

    pylink     = zend_read_property(Z_OBJCE_P(cls), cls, ZEND_STRL("_pynotation"), 0 TSRMLS_CC);
    pynotation = (IPYNotation *)zend_fetch_resource(&pylink TSRMLS_CC, -1,
                     PINYIN_NOTATION_LINK_DESC, NULL, 1, le_pinyin_notation_link);

    if (!pynotation) {
        php_error_docref(NULL TSRMLS_CC,
                         E_WARNING,
                         "Wrong resource handler for IPYNotation.");
    }

    return pynotation;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4
 * vim<600: sw=4 ts=4
 */
