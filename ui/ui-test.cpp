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
#include "sourceview.hpp"
#include "simulationeditor.hpp"
#include "box.hpp"
#include "../engine/blob.hpp"

using namespace Tiger;

class FilterEditor
	{
	public:
		FilterEditor(Container& cnt):m_box(cnt,1)
			,m_toolbar(m_box,0,0)
			,m_src_view(m_box.insertMode({0,Box::FILL|Box::EXPAND}))
			{
			m_toolbar.append("New").append("Open").append("Save")
				.append("Compile").append("Use").callback(*this);
			}

		void operator()(ButtonList<FilterEditor>& src,Button& btn)
			{
			btn.state(0);
			}

	private:
		Box m_box;
			ButtonList<FilterEditor> m_toolbar;
			SourceView m_src_view;
	};

TIGER_BLOB(char,example,"engine/example.cpp");

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx;
	Tiger::Window mainwin("Tiger test",0);
	auto mainwin_cb=[&ctx](Tiger::Window& window)
		{ctx.exit();};
	Tiger::TabView tabs(mainwin);
	FilterEditor fileedit(tabs.tabTitle("Filter editor"));
	Tiger::SimulationEditor simedit(tabs.tabTitle("Simulation setup"),0);
/*	srcv.lineNumbers(1);
	srcv.highlight("foo.cpp");
	srcv.content(example_begin);*/
	mainwin.callback(mainwin_cb);
	mainwin.show();
	ctx.run();
	return 0;
	}
