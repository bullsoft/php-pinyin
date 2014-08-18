#ifndef IPYFACTORY_H
#define IPYFACTORY_H

#include "IPYNotation.h"

namespace NLP
{
	namespace Pinyin
	{
		class IPYFactory
		{
			public:
			static IPYNotation* getPYInstance();
		};
	}
}

#endif

