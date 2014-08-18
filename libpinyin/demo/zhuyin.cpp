// zhuyin.cpp : Defines the entry point for the console application.
//

#include "IPYFactory.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

using namespace std;
using namespace NLP::Pinyin;
void  printUsage() {
    cout << "Usage:" ;
    cout << " " << "./zhuyin [OPTION] dictPath [outPath] [maxOutSize]" << endl;
    cout << "\t" << "OPTION:" << endl;
    cout << "\t" << "  -z, --zhuyin. default option, need {dict.dat duoyong.dat dyz.dat dz_pro.dat} in dictPath" << endl;
    cout << "\t" << "  -b, --build-dict. need dict.txt in dictPath,generate dict.data" << endl;
    cout << "\t" << "  -t, --tone. result has tone, need {dict_tone.dat dyz_tone.dat} in dictPath" << endl;
    cout << "\t" << "dictPath: the path of dict file" << endl;
    cout << "\t" << "outPath: needed if OPTION=-b, dict.data will generate in the outPath" << endl;
    cout << "\t" << "maxOutSize: default 1, the max number of probably pinyin output" << endl;
}

int main(int argc, const char* argv[])
{
	/*
	if(argc<3)
	{
		cout<<"usage:"<<endl;
		cout<<"\t"<<"./build_dict dict.txt[in] dict.dat[out]"<<endl;
		return -1;
	}
	PYNotation *pynotation=new PYNotation();
	pynotation->generateDict(argv[1],argv[2]);*/
	
	if(argc<3)
	{
//		cout<<"usage:"<<endl;
//		cout<<"\t"<<"./zhuyin dyz.dat[in] dict.dat[in] duoyong_dict[in] dz_dict[in] max_out_size[int]"<<endl;
//		cout<<"\t"<<"max_out_size为输出最大的个数上限，0为全部输出，最小为1"<<endl;
        printUsage();
		return -1;
	}
    string strDictPaht;
	int maxsize=1;
    if (argc==3) {
        strDictPaht = argv[1];
        maxsize=atoi(argv[2]);
    }
    
	IPYNotation *pynotation=IPYFactory::getPYInstance();
	if(!pynotation->loadDyzDict( (strDictPaht+"/dyz.dat").c_str()) ) {
		cout<<"Error: load tone dyz dict fail..."<<endl;
		return -1;
	}
	if(!pynotation->loadDict((strDictPaht+"/dict.dat").c_str())) {
		cout<<"Error: load tone dict fail..."<<endl;
		return -1;
	}
	if(!pynotation->loadDYDict((strDictPaht+"/duoyong.dat").c_str())) {
		cout<<"Error: load duoyong dict fail..."<<endl;
		return -1;
	}
	if(!pynotation->loadBMEDict((strDictPaht+"/dz_pro.dat").c_str())) {
		cout<<"Error: load BME dict fail..."<<endl;
		return -1;
	}

	if(maxsize<0)
	{
		cout<<"Error: max_out_size is out of range!"<<endl;
		return -1;
    }


    string line;
	while(getline(std::cin,line))
	{
		if(!line.size())
            break;
		istringstream is(line);
		string characters;
		is>>characters;
		vector<string> py_result;
		if(pynotation->exactToPY(characters.c_str(),&py_result))
		{
			cout<<characters;
			vector<string>::iterator _it = py_result.begin();
			//cout<<"\t"<<(*_it);
			int i=0;
			for(vector<string>::iterator _sit=py_result.begin();_sit!=py_result.end();_sit++)
			{
				cout<<"\t"<<(*_sit);
				i++;
				if(i>=maxsize&&maxsize!=0)
					break;
			}
			cout<<endl;
		}
		else
			cout<<"Error: wrong character,cannot convert - "<<line<<endl;
	}
	delete pynotation;
	return 0;
}

