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
        _loadDict(strDictPath);
      } else {
        strDictPath = Php::ini_get("pinyin.dict_path").stringValue();
        _tone = Php::ini_get("pinyin.dict_tone").boolValue();
        _loadDict(strDictPath);
      }

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

    Php::Value convert(Php::Parameters &params) {
      vector<string> py_result;
      string characters;
      string encoding;
      bool tone;
      Php::Value array;

      if(params.size() < 2) {
        tone = false;
      } else {
        tone = params[1];
      }

      characters = params[0].stringValue();
      encoding = Php::call("mb_detect_encoding", params[0], "UTF-8, GBK, CP936").stringValue();
      //std::cout << encoding << std::endl;
      if("UTF-8" == encoding) {
        characters = Php::call("mb_convert_encoding", params[0], "GBK", "UTF-8").stringValue();
      } else if("GBK" == encoding || "CP936" == encoding) {
        // nothing to do here...
      } else {
        throw Php::Exception("Character encoding must be utf-8 or gbk");
      }
      //Php::out << Php::call("mb_convert_encoding", characters, "UTF-8", "GBK") << std::endl;
      bool result;
      if(tone) {
        //std::cout << "tone is true" << std::endl;
        if (_tone == false) {
          throw Php::Exception("You should load tone dict first");
        }
        result = _py->convertToTonePY(characters.c_str(), &py_result);
      } else {
        result = _py->convertToPY(characters.c_str(), &py_result);
      }
      //std::cout << result << std::endl;
      if(result) {
        int i = 0;
        for(vector<string>::iterator _sit = py_result.begin(); _sit != py_result.end(); _sit++) {
          //std::cout << (*_sit) << std::endl;
          array[i++] = (*_sit);
        }
        return array;
      } else {
        return false;
      }
    }

    void generateDict(Php::Parameters &params) {

    }

    void multiConvert(Php::Parameters &params) {

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

