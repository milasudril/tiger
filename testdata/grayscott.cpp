/*
Tiger
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
TIGER_PARAMETERS("d","F","k");
TIGER_CHANNELS("u","v");
TIGER_ENTRY(gray_scott);

static void gray_scott(const Tiger::FilterStateClient& data,unsigned long long frame)
	{
	auto w=data.width();
	auto h=data.height();
	auto d=data.param<P("d")>();
	auto F=data.param<P("F")>();
	auto k=data.param<P("k")>();

	for(int y=0;y<h;++y)
		{
		auto up  =y<h-1?y+1:0;
		auto down=y>0?y-1:h-1;

		for(int x=0;x<w;++x)
			{
			auto right=x<w-1?x+1:0;
			auto left=x>0?x-1:w-1;

		//	Exercise: Vectorize this piece of code
			auto dyy_u=data.value_current<C("u")>(x,up) 
				-2.0f*data.value_current<C("u")>(x,y)
				+data.value_current<C("u")>(x,down);

			auto dxx_u=data.value_current<C("u")>(right,y) 
				-2.0f*data.value_current<C("u")>(x,y)
				+data.value_current<C("u")>(left,y);

			auto dyy_v=data.value_current<C("v")>(x,up) 
				-2.0f*data.value_current<C("v")>(x,y)
				+data.value_current<C("v")>(x,down);

			auto dxx_v=data.value_current<C("v")>(right,y) 
				-2.0f*data.value_current<C("v")>(x,y)
				+data.value_current<C("v")>(left,y);

			auto l_u=dxx_u + dyy_u;
			auto l_v=dxx_v + dyy_v;
			auto v_u=data.value_current<C("u")>(x,y);
			auto v_v=data.value_current<C("v")>(x,y);

			data.value_next<C("u")>(x,y)=v_u + ( d*l_u - v_u*v_v*v_v + F*(1-v_u) )/16.0f;
			data.value_next<C("v")>(x,y)=v_v + ( l_v + v_u*v_v*v_v - (F+k)*v_v )/16.0f;
			}
		}
	}
