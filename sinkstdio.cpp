//@	{"targets":[{"name":"sinkstdio.o","type":"object"}]}

#include "sinkstdio.hpp"

void Tiger::printRange(const char* begin,const char* end,FILE* sink)
	{
	while(begin!=end)
		{
		putc(*begin,sink);
		++begin;
		}
	}
