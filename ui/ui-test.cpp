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
#include "window.hpp"
#include "tabview.hpp"
#include "filtereditor.hpp"
#include "simulationeditor.hpp"

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx;
	Tiger::Window mainwin("Tiger test",0);
	auto mainwin_cb=[&ctx](Tiger::Window& window)
		{ctx.exit();};
	Tiger::TabView tabs(mainwin);
	Tiger::FilterEditor fileedit(tabs.tabTitle("Filter editor"));
	Tiger::SimulationEditor simedit(tabs.tabTitle("Simulation setup"),0);

	mainwin.callback(mainwin_cb);
	mainwin.show();
	ctx.run();
	return 0;
	}
