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
#include "uicontext.hpp"
#include "box.hpp"
#include "textentry.hpp"
#include "separator.hpp"
#include <cstdio>
#include <algorithm>

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx;
	Tiger::Window mainwin("Tiger",1);
	auto mainwin_cb=[&ctx](Tiger::Window& window)
		{ctx.exit();};
	mainwin.callback(mainwin_cb);
	Tiger::Box range_entries(mainwin,1);
	Tiger::TextEntry e_max(range_entries);
	range_entries.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND});
	Tiger::Box range(range_entries,0);
	range.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND});
	Tiger::Separator dec_left(range,1);
	range.insertMode(Tiger::Box::InsertMode{0,0});
	Tiger::RangeView rv(range);
	range.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND});
	Tiger::Separator dec_right(range,1);
	range_entries.insertMode(Tiger::Box::InsertMode{0,0});
	Tiger::TextEntry e_min(range_entries);
	auto rv_callback=[&e_min,&e_max](Tiger::RangeView& rv)
		{
		auto r=rv.range();
		char buffer[12];
		sprintf(buffer,"%.2e",r.min());
		e_min.content(buffer);
		sprintf(buffer,"%.2e",r.max());
		e_max.content(buffer);
		};
	auto emin_callback=[&rv](Tiger::TextEntry& entry)
		{
		auto v_max=rv.range().max();
		auto v=std::min(atof(entry.content()),v_max);
		rv.range(Tiger::Range(v,v_max));
		};
	auto emax_callback=[&rv](Tiger::TextEntry& entry)
		{
		auto v_min=rv.range().min();
		auto v=std::max(atof(entry.content()),v_min);
		rv.range(Tiger::Range(v_min,v));
		};
	rv_callback(rv);
	rv.callback(rv_callback);
	e_min.callback(emin_callback).width(8).small(1).alignment(1.0f);
	e_max.callback(emax_callback).width(8).small(1).alignment(1.0f);
	mainwin.show();
	ctx.run();
	return 0;
	}