//@	{"targets":[{"name":"test.so","type":"lib_dynamic"}]}

#include "client.hpp"

TIGER_PARAMETERS("");
TIGER_CHANNELS("red","green","blue");
TIGER_ENTRY(process);

static void process(const Tiger::ProcessDataClient& data)
	{
	auto w=data.width();
	auto h=data.height();
	for(int y=0;y<h;++y)
		{
		for(int x=0;x<w;++x)
			{
			data.value_next<C("red")>(x,y)=data.value_current<C("red")>(x,y);
			data.value_next<C("green")>(x,y)=data.value_current<C("green")>(x,y);
			data.value_next<C("blue")>(x,y)=data.value_current<C("blue")>(x,y);
			}
		}
	}
