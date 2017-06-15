/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2013-2017 BullSoft                                     |
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
  |         Shang Yuanchun <idealities@gmail.com>                        |
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

// ZEND_DECLARE_MODULE_GLOBALS(pinyin)

static int le_pinyin_notation_link;
#define PINYIN_NOTATION_LINK_DESC "Pinyin Notation"

zend_class_entry *pinyin_ce;

static IPYNotation *get_pinyin_notation(zval *cls);
static bool is_dict_loaded(zval *cls);

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
    PHP_ME(Pinyin, generateDict, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
/* }}} */

/* {{{ pinyin_module_entry
 *
 * We have no globals and deps here.
 *
 */
zend_module_entry pinyin_module_entry = {
	STANDARD_MODULE_HEADER,
	"pinyin",
	pinyin_methods,
	PHP_MINIT(pinyin),
	PHP_MSHUTDOWN(pinyin),
	PHP_RINIT(pinyin),
	PHP_RSHUTDOWN(pinyin),
	PHP_MINFO(pinyin),
	PHP_PINYIN_VERSION,
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
    pinyin_ce = zend_register_internal_class(&ce);

    zend_declare_class_constant_long(pinyin_ce, ZEND_STRL("TY_DICT"), TY_DICT);
    zend_declare_class_constant_long(pinyin_ce, ZEND_STRL("TY_TONE_DICT"), TY_TONE_DICT);

    zend_declare_class_constant_long(pinyin_ce, ZEND_STRL("DYZ_DICT"), DYZ_DICT);
    zend_declare_class_constant_long(pinyin_ce, ZEND_STRL("DYZ_TONE_DICT"), DYZ_TONE_DICT);

    zend_declare_class_constant_long(pinyin_ce, ZEND_STRL("DY_DICT"), DY_DICT);
    zend_declare_class_constant_long(pinyin_ce, ZEND_STRL("BME_DICT"), BME_DICT);

    zend_declare_property_null(pinyin_ce, ZEND_STRL(PY_NOTATION), ZEND_ACC_PRIVATE);
    zend_declare_property_bool(pinyin_ce, ZEND_STRL(PY_DICT_LOADED), 0, ZEND_ACC_PRIVATE);

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
    zval         z_pinyin;

    ZVAL_RES(&z_pinyin, zend_register_resource(pynotation, le_pinyin_notation_link));
    zend_update_property(Z_OBJCE_P(self), self, ZEND_STRL(PY_NOTATION), &z_pinyin);
}

PHP_METHOD(Pinyin, __destruct)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    if (pynotation) delete pynotation;
}

PHP_METHOD(Pinyin, loadDict)
{
    zval        *self       = getThis();
    IPYNotation *pynotation = get_pinyin_notation(self);
    char *path = NULL;
    size_t     len;
    zend_long  dict_type; // actually its value is of type enum Dict_Type

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sl", &path, &len, &dict_type) == FAILURE) {
        RETURN_FALSE;
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

    zval loaded;
    ZVAL_BOOL(&loaded, 1);
    zend_update_property(Z_OBJCE_P(self), self, ZEND_STRL(PY_DICT_LOADED), &loaded);
    RETURN_TRUE;
}

PHP_METHOD(Pinyin, convert)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    char *characters   = NULL;
    size_t   len;
    zend_bool get_tone = 0;
    bool result = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|b", &characters, &len, &get_tone) == FAILURE) {
        RETURN_FALSE;
    }

    if (!is_dict_loaded(getThis())) {
        RETURN_FALSE;
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
            add_index_string(return_value, i++, (*_sit).c_str());
        }
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(Pinyin, multiConvert)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    zval        *strs, *content;
    zend_string *key;
    HashPosition pointer;
    bool         result     = 0;
    int          strs_num   = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &strs) == FAILURE) {
        RETURN_FALSE;
    }

    if (!is_dict_loaded(getThis())) {
        RETURN_FALSE;
    }

    vector<string> convert_strs;
    string strtmp;

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(strs), key, content) {
        zend_string *str = zval_get_string(content);
        strtmp.assign(ZSTR_VAL(str), ZSTR_LEN(str));
        convert_strs.push_back(strtmp);
        strs_num++;
        zend_string_release(str);
    } ZEND_HASH_FOREACH_END();

    vector<vector<string> * > py_results;
    py_results.reserve(strs_num);

    // py_result to store one sentence's pinyin result
#ifdef DARWIN
# define PY_NEED_DELETE_RESULT
    vector<string> *py_result = new vector<string>[strs_num];
#else
    vector<string>  py_result[strs_num];
#endif

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
                add_index_string(return_value, i++, (*iit).c_str());
            }
        }
#ifdef PY_NEED_DELETE_RESULT
        delete [] py_result;
#endif
    } else {
#ifdef PY_NEED_DELETE_RESULT
        delete [] py_result;
#endif
        RETURN_FALSE;
    }
}

PHP_METHOD(Pinyin, exactConvert)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    char        *str        = NULL;
    size_t       len;
    zend_bool    get_tone   = 0;
    bool         result     = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|b", &str, &len, &get_tone) == FAILURE) {
        RETURN_FALSE;
    }

    if (!is_dict_loaded(getThis())) {
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
            add_index_string(return_value, i++, (*it).c_str());
        }
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(Pinyin, generateDict)
{
    IPYNotation *pynotation = get_pinyin_notation(getThis());
    char  *from_str, *to_str;
    size_t from_len, to_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &from_str, &from_len, &to_str, &to_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (!is_dict_loaded(getThis())) {
        RETURN_FALSE;
    }

    if(pynotation->generateDict(from_str, to_str)) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

static IPYNotation *get_pinyin_notation(zval *cls)
{
    zval        *pylink;
    IPYNotation *pynotation;

    pylink     = zend_read_property(Z_OBJCE_P(cls), cls, ZEND_STRL(PY_NOTATION), 0, NULL);
    pynotation = (IPYNotation *)zend_fetch_resource(Z_RES_P(pylink), PINYIN_NOTATION_LINK_DESC, le_pinyin_notation_link);

    if (!pynotation) {
        php_error_docref(NULL, E_WARNING, "Wrong resource handler for IPYNotation.");
    }

    return pynotation;
}

static bool is_dict_loaded(zval *cls)
{
    zval *loaded;

    loaded = zend_read_property(Z_OBJCE_P(cls), cls, ZEND_STRL(PY_DICT_LOADED), 0, NULL);
    return loaded && Z_TYPE_P(loaded) == IS_TRUE;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4
 * vim<600: sw=4 ts=4
 */
