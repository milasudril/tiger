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
//@	{"targets":[{"name":"../tiger-ui-test","type":"application"}]}

#include "uicontext.hpp"
#include "mapview.hpp"
#include "window.hpp"

#include <string>
#include <map>
#include <cstdio>

class MyMapData
	{
	public:
		typedef std::string key_type;
		typedef float mapped_type;
		const std::string& keyString(const std::string& str) const noexcept
			{return str;}
		std::string valueString(float x) const noexcept
			{
			std::string ret;
			ret.reserve(16);
			sprintf(const_cast<char*>(ret.data()),"%.9g",x);
			return std::move(ret);
			}
		const char* key() const noexcept
			{return "Name";}
		const char* value() const noexcept
			{return "Value";}
	};

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx(argc,argv);
	Tiger::Window mainwin("Tiger Test",1);
	auto mainwin_cb=[&ctx](Tiger::Window& window)
		{ctx.exit();};
	MyMapData map_data;
	Tiger::MapView<MyMapData> mv(mainwin,map_data);
	auto mv_cb=[](Tiger::MapView<MyMapData>& src,float* address	
		,const char* value_new)
		{*address=strtof(value_new,nullptr);};
	mv.callback(mv_cb);
	std::map<std::string,float> vals;
	vals["pi"]=3.14f;
	vals["e"]=2.72f;
	vals["Answer to the ultimate question about universe life and everything"]=42.0f;
		{
		auto ptr=vals.begin();
		auto ptr_end=vals.end();
		while(ptr!=ptr_end)
			{
			mv.recordAppend(ptr->first,ptr->second);
			++ptr;
			}
		}

	mv.valueAlignment(1.0f);

	mainwin.callback(mainwin_cb);
	mainwin.show();
	ctx.run();
	return 0;
	}