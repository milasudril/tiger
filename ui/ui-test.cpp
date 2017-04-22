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

namespace Tiger
	{
	template<class Callback>
	class SimulationSetup;

	class Ui
		{
		public:
			void operator()(Window& ui_owner)
				{}

			void stateChanged(FilterEditor<Ui>& editor)
				{}

			void submit(FilterEditor<Ui>& editor)
				{}

			void submit(SimulationSetup<Ui>& simedit)
				{}

		private:
			UiContext& r_ctx;
			Window& r_mainwin;
			TabView& r_tabs;
		};
	}

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx;
	Tiger::Window mainwin("Tiger test",0);
	auto mainwin_cb=[&ctx](Tiger::Window& window)
		{ctx.exit();};
	Tiger::TabView tabs(mainwin);
	auto editor_cb=[&mainwin](Tiger::FilterEditorBase& editor)
		{
		std::string title_string(editor.filename());
		if(editor.dirty())
			{title_string+=" * ";}
		mainwin.title(title_string.c_str());
		};
	Tiger::FilterEditor<decltype(editor_cb)> fileedit(tabs.tabTitle("Filter editor"),0);
	fileedit.callback(editor_cb);
	Tiger::SimulationEditor simedit(tabs.tabTitle("Simulation setup"),0);

	mainwin.callback(mainwin_cb);
	mainwin.show();
	ctx.run();
	return 0;
	}
