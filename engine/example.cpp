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
TIGER_PARAMETERS(nullptr); //Add parameter names here. These are accessed from data.param<P("name")>();
TIGER_CHANNELS("red","green","blue"); //Or suitable names
TIGER_ENTRY(process); //Define the name of the entry point

static void process(const Tiger::FilterStateClient& data,unsigned long long int frame)
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
