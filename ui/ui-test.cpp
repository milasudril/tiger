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
#include "filenameselect.hpp"
#include "../engine/srcstdio.hpp"

#include "simulationeditor.hpp"
#include "box.hpp"
#include "../engine/blob.hpp"

using namespace Tiger;

TIGER_BLOB(char,example,"engine/example.cpp");

class FilterEditor
	{
	public:
		FilterEditor(Container& cnt):m_box(cnt,1)
			,m_toolbar(m_box,0,0)
			,m_vsplit(m_box.insertMode({0,Box::FILL|Box::EXPAND}),0)
				,m_src_view(m_vsplit.insertMode({0,Box::FILL|Box::EXPAND}))
				,m_output(m_vsplit.insertMode({0,Box::FILL|Box::EXPAND}))
			{
			m_toolbar.append("New").append("Open").append("Save")
				.append("Compile").append("Load").callback(*this);
			m_src_view.highlight("foo.cpp").content(example_begin).lineNumbers(1);
			m_output.content("Click \"Compile\" or \"Load\" to compile the filter");
			}

		void operator()(ButtonList<FilterEditor>& src,Button& btn)
			{
			switch(btn.id())
				{
				case 0:
					m_src_view.content(example_begin);
					break;
				case 1:
					if(filenameSelect(m_box,m_file_current,FileselectMode::OPEN))
						{
						m_src_view.content(SrcStdio{m_file_current.c_str()});
						}
					break;
				default:
					break;
				}
			btn.state(0);
			}

	private:
		Box m_box;
			ButtonList<FilterEditor> m_toolbar;
			Box m_vsplit;
				SourceView m_src_view;
				SourceView m_output;
		std::string m_file_current;
	};

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx;
	Tiger::Window mainwin("Tiger test",0);
	auto mainwin_cb=[&ctx](Tiger::Window& window)
		{ctx.exit();};
	Tiger::TabView tabs(mainwin);
	FilterEditor fileedit(tabs.tabTitle("Filter editor"));
	Tiger::SimulationEditor simedit(tabs.tabTitle("Simulation setup"),0);

	mainwin.callback(mainwin_cb);
	mainwin.show();
	ctx.run();
	return 0;
	}
