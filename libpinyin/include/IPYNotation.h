#ifndef IPYNOTATION_H
#define IPYNOTATION_H

#include <vector>
#include <string>

using namespace std;

namespace NLP
{
	namespace Pinyin
	{
		class IPYNotation
		{
			public:
           /**
			* @brief 加载多音单字词典       
			* @return : 是否成功
			**/
			virtual bool  loadDyzDict(const char* path) = 0;
			/**
			* @brief 加载特殊姓氏词典       
			* @return : 是否成功
			**/
			virtual bool loadDYDict(const char* path) = 0;
			/**
			* @brief 加载BME频率词典       
			* @return : 是否成功
			**/
			virtual bool loadBMEDict(const char* path) = 0;
			/**
			* @brief 加载带声调多音单字词典       
			* @return : 是否成功
			**/
			virtual bool loadToneDyzDict(const char* path) = 0 ;
			/**
			* @brief 生成词典   
			* @oriPath[in] 明文词表，词\t拼音，并按词有序排列，行去重，不能有中英文混合词条，否则索引混乱，不能有非法字符（汉字以外的，如：问号等）词条长度小于CHARACTERS_MAX_LENGTH 150
			* @binaryPath[out] 生成的二进制词典
			* @return : 是否成功
			**/
			virtual bool generateDict(const char* oriPath,const char* binaryPath) = 0 ;
			/**
			* @brief 加载词典
			* @return : 是否成功
			**/
			virtual bool loadDict(const char* path)= 0 ;
			/**
			* @brief 加载带声调词典
			* @return : 是否成功
			**/
			virtual bool loadToneDict(const char* path) = 0;
			/**
			* @brief 完全匹配字典
			* @str[in]	输入的字符串
			* @_result[out] 输出拼音串
			* @return : 是否成功
			**/
			virtual bool exactToPY(const char* str,vector<string>* _result) = 0;
			/**
			* @brief 完全匹配带声调字典
			* @str[in]	输入的字符串
			* @_result[out] 输出拼音串
			* @return : 是否成功
			**/
			virtual bool exactToTonePY(const char* str,vector<string>* _result) = 0;
			/**
			* @brief 将字符串转换为拼音串（可识别中英文混合串）
			* @str[in]	输入的字符串
			* @_result[out] 输出拼音串
			* @return : 是否成功
			**/
			virtual bool convertToPY(const char* str,vector<string>* _result) = 0;
			/**
			* @brief 将字符串转换为带声调的拼音串（可识别中英文混合串）
			* @str[in]	输入的字符串
			* @_result[out] 输出拼音串
			* @return : 是否成功
			**/
			virtual bool convertToTonePY(const char* str,vector<string>* _result) = 0;
			/**
			* @brief 将字符串转换为拼音串（可识别中英文混合串）
			* @str[in]	输入多组字符串
			* @_result[out] 输出每组字符串对应的拼音串
			* @return : 是否全部转换成功，有一个错则false
			**/
			virtual bool convertToPY(const vector<string> strs,vector<vector<string>* >* _results) = 0;
			 /**
			* @brief 人名特殊姓氏读音纠错
			* @str[in]	输入的字符串
			* @_result[out] 输出拼音串
			* @return : 是否成功
			**/
			virtual bool perNameCorrection(const char*,vector<string>* _result) = 0;  
			virtual ~IPYNotation(){};
        };
	}
}

#endif

