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

enum Dict_Type {
  TY_DICT  = 1,
  TY_TONE_DICT,
  DYZ_DICT,
  DYZ_TONE_DICT,
  DY_DICT,
  BME_DICT,
};

/**
 *  Class definition
 */
class Master : public Php::Base
{
private:
    IPYNotation *_py = 0;

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
      delete _py;
    }


    void __construct(Php::Parameters &params) {
      string strDictPath;
      bool tone;

      if(!params.empty()) {
        string strDictPath = params[0];
        if(strDictPath.empty()) {
          throw Php::Exception("Dict path can not be empty");
        }

        if(params.size() < 2) {
          tone = false;
        } else {
          tone = params[1];
        }
      } else {
        string strDictPath = Php::ini_get("pinyin.dict_path");
        tone = Php::ini_get("pinyin.dict_tone");
      }
      _loadDict(strDictPath, tone);
    }

    bool _loadDict(string strDictPath, bool tone = false) {
      if(!_py->loadDyzDict( (strDictPath+"/dyz.dat").c_str()) ) {
        return false;
      }
      if(!_py->loadDict((strDictPath+"/dict.dat").c_str())) {
        return false;
      }
      if(!_py->loadDYDict((strDictPath+"/duoyong.dat").c_str())) {
        return false;
      }
      if(!_py->loadBMEDict((strDictPath+"/dz_pro.dat").c_str())) {
        return false;
      }

      if (tone == true) {
        if(!_py->loadToneDyzDict( (strDictPath+"/dyz_tone.dat").c_str()) ) {
          return false;
        }
        if(!_py->loadToneDict((strDictPath+"/dict_tone.dat").c_str())) {
          return false;
        }
      }
      return true;
    }

    Php::Value convert(Php::Parameters &params) {
      vector<string> py_result;
      bool tone;
      const char *characters;

      if(params.size() < 2) {
        tone = false;
      } else {
        tone = params[1];
      }

      string encoding = Php::call("mb_detect_encoding", params[0], "UTF-8, GBK");
      if("UTF-8" == encoding) {
        characters = Php::call("mb_convert_encoding", params[0], "GBK", "UTF-8");
      } else if("GBK" == encoding) {
        characters = params[0];
      } else {
        throw Php::Exception("Character encoding must be utf-8 or gbk");
      }

      bool result;
      if(tone) {
        result = _py->convertToTonePY(characters, &py_result);
      } else {
        result = _py->convertToPY(characters, &py_result);
      }

      if(result) {
        Php::Value array;
        //vector<string>::iterator _it = py_result.begin();
        int i = 0;
        for(vector<string>::iterator _sit = py_result.begin(); _sit != py_result.end(); _sit++) {
          array[i++] = (*_sit).c_str();
        }
        return array;
      } else {
        return false;
      }
    }

    void generateDict(Php::Parameters &params) {

    }

    void loadDict(Php::Parameters &params) {

    }

    /**
     *  Cast to a string
     *  @return const char *
     */
    const char *__toString() const
    {
        return "Chinese Pinyin extension for php.";
    }
};

