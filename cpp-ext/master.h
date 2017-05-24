/**
 *  Pinyin.h
 *  Class that is exported to PHP space
 */

/**
 *  Include guard
 */
#pragma once

#include "IPYFactory.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
using namespace std;
using namespace NLP::Pinyin;

/**
 *  Class definition
 */
class Master : public Php::Base
{
private:
    IPYNotation *_py = 0;
    bool _tone = false;

 public:
    /**
     *  Constructor
     */
    Master()
    {
      _py = IPYFactory::getPYInstance();
    }

    /**
     *  Destructor
     */
    ~Master() {
      if(_py) delete _py;
    }


    void __construct(Php::Parameters &params) {
      string strDictPath;

      if(!params.empty()) {
        strDictPath = params[0].stringValue();
        if(strDictPath.empty()) {
          strDictPath = Php::ini_get("pinyin.dict_path").stringValue();
        }
        if(params.size() < 2) {
          _tone = false;
        } else {
          _tone = params[1].boolValue();
        }
      } else {
        strDictPath = Php::ini_get("pinyin.dict_path").stringValue();
        _tone = Php::ini_get("pinyin.dict_tone").boolValue();
      }
      _loadDict(strDictPath);
    }

    bool _loadDict(string strDictPath) {
      if(!_py->loadDyzDict( (strDictPath+"/dyz.dat").c_str()) ) {
        throw Php::Exception("Error to load DyzDict");
      }
      if(!_py->loadDict((strDictPath+"/dict.dat").c_str())) {
        throw Php::Exception("Error to load Dict");
      }

      if(!_py->loadDYDict((strDictPath+"/duoyong.dat").c_str())) {
        throw Php::Exception("Error to load DYDict");
      }
      if(!_py->loadBMEDict((strDictPath+"/dz_pro.dat").c_str())) {
        throw Php::Exception("Error to load DZDict");
      }

      if (_tone == true) {
        if(!_py->loadToneDyzDict((strDictPath+"/dyz_tone.dat").c_str())) {
          throw Php::Exception("Error to load DyzToneDict");
        }
        if(!_py->loadToneDict((strDictPath+"/dict_tone.dat").c_str())) {
          throw Php::Exception("Error to load DictTone");
        }
      }
      return true;
    }

    Php::Value _convert(Php::Value word) {
      string encoding;
      string characters;
      bool result;
      Php::Value array;
      vector<string> py_result;

      if(word.size() == 0) {
        throw Php::Exception("The word can not be empty");
      }

      encoding = Php::call("mb_detect_encoding", word, "UTF-8, GBK, CP936").stringValue();

      //std::cout << encoding << std::endl;
      if("UTF-8" == encoding) {
        characters = Php::call("mb_convert_encoding", word, "GBK", "UTF-8").stringValue();
      } else if("GBK" == encoding || "CP936" == encoding) {
        characters = word.stringValue();
      } else {
        throw Php::Exception("Character encoding must be utf-8 or gbk");
      }

      if(_tone) {
        //std::cout << "tone is true" << std::endl;
        result = _py->convertToTonePY(characters.c_str(), &py_result);
      } else {
        result = _py->convertToPY(characters.c_str(), &py_result);
      }

      if(result) {
        int i = 0;
        for(vector<string>::iterator _sit = py_result.begin(); _sit != py_result.end(); _sit++) {
          //std::cout << (*_sit) << std::endl;
          array[i++] = (*_sit);
        }
        if(array.size() > 0) {
          return array[0];
        }
      }
      return false;
    }

    Php::Value convert(Php::Parameters &params) {
      return _convert(params[0]);
    }

    Php::Value multiConvert(Php::Parameters &params) {
      Php::Array words = params[0];
      Php::Value array;

      for (auto &iter : words)
      {
        Php::Value py = _convert(iter.second);
        if(py.isString() && py.size() > 0) {
          array[iter.first] = py;
        } else {
          array[iter.first] = false;
        }
      }
      return array;
    }

    /**
     * WARNING:::: PHP-CPP do not support reference-value
     * So this method was not exported to PHP-Userland
     */
    Php::Value safeConvert(Php::Parameters &params) {
      Php::Value array;
      Php::Value word = params[0];
      Php::Value utf8Word;
      string encoding;

      encoding = Php::call("mb_detect_encoding", word, "UTF-8, GBK, CP936").stringValue();

      if("UTF-8" == encoding) {
        utf8Word = word;
      } else if("GBK" == encoding || "CP936" == encoding) {
        utf8Word = Php::call("mb_convert_encoding", word, "UTF-8", "GBK");
      } else {
        throw Php::Exception("Character encoding must be utf-8 or gbk");
      }

      Php::Value matches;
      bool result;
      // PHP-CPP do not support reference-value
      result = Php::call("preg_match_all", string("/([\\x{4e00}-\\x{9fa5}]+)/iu"), utf8Word, matches).boolValue();
      if(result > 0) {
        Php::Value m = matches[1];
        for (auto &iter : m)
          {
            Php::Value py = _convert(iter.second);
            if(py.isString() && py.size() > 0) {
              array[iter.first] = py;
            } else {
              array[iter.first] = false;
            }
          }
        return Php::call("str_replace", m, array, utf8Word);
      }
      return false;
    }

    Php::Value generateDict(Php::Parameters &params) {
      const char * txt = params[0];
      const char * dat = params[1];

      if(_py->generateDict(txt, dat)) {
        return true;
      }
      return false;
    }

    /**
     *  Cast to a string
     *  @return const char *
     */
    const char *__toString() const
    {
        return "Chinese Pinyin extension for php. Made by BullSoft.org";
    }
};

