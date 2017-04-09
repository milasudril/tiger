//@	{"targets":[{"name":"test.so","type":"lib_dynamic"}]}

#include "sdk.hpp"

PARAMETERS("amount");
CHANNELS("red","green","blue");

float mean(float a,float b,float c,float d,float e)
	{return 0.20f*(a+b+c+d+e);}

void process(const Tiger::ProcessData& data)
	{
	auto w=data.width();
	auto h=data.height();
	auto amount=data.param<P("amount")>();
	for(int y=1;y<h-1;++y)
		{
		for(int x=1;x<w-1;++x)
			{
			data.value_next<C("red")>(x,y)=amount*mean(data.source<C("red")>(x+1,y)
				,data.source<C("red")>(x-1,y)
				,data.source<C("red")>(x,y+1)
				,data.source<C("red")>(x,y-1)
				,data.source<C("red")>(x,y)) + (1.0f - amount)*data.source<C("red")>(x,y);

			data.value_next<C("green")>(x,y)=amount*mean(data.source<C("green")>(x+1,y)
				,data.source<C("green")>(x-1,y)
				,data.source<C("green")>(x,y+1)
				,data.source<C("green")>(x,y-1)
				,data.source<C("green")>(x,y)) + (1.0f - amount)*data.source<C("green")>(x,y);

			data.value_next<C("blue")>(x,y)=amount*mean(data.source<C("blue")>(x+1,y)
				,data.source<C("blue")>(x-1,y)
				,data.source<C("blue")>(x,y+1)
				,data.source<C("blue")>(x,y-1)
				,data.source<C("blue")>(x,y)) + (1.0f - amount)*data.source<C("blue")>(x,y);
			}
		}
	}
