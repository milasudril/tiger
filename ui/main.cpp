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
//@	{"targets":[{"name":"../tiger-ui","type":"application"}]}
#include "window.hpp"
#include "rangeview.hpp"
#include "range.hpp"
#include "uimain.hpp"
#include "box.hpp"
#include <cstdio>

int main(int argc, char *argv[])
	{
	Tiger::uiInit(argc,argv);
	Tiger::Window mainwin("Tiger",1);
	Tiger::Box box(mainwin,0);
	Tiger::RangeView rv(box);
	auto my_callback=[](Tiger::RangeView& rv)
		{
		auto r=rv.range();
		printf("\r%.7f %.7f",r.min(),r.max());
		fflush(stdout);
		};
	rv.callback(my_callback);
	mainwin.show();
	return 0;
	}