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

#include "uicontext.hpp"
#include "window.hpp"
#include "tabview.hpp"
#include "filtereditor.hpp"
#include "simulationeditor.hpp"
#include "simulationview.hpp"
#include "../engine/simulation.hpp"

namespace Tiger
	{
	class Ui
		{
		public:
			typedef Ui Self;

			explicit Ui(UiContext& ctx,Window& mainwin):
				 m_tabs(mainwin)
				,m_filter_edit(m_tabs.tabTitle("Filter editor"),0)
				,m_sim_edit(m_tabs.tabTitle("Simulation setup"),0)
				,m_sim_view(m_tabs.tabTitle("Simulation"))
				,r_ctx(ctx)
				,r_mainwin(mainwin)
				{
				mainwin.callback(*this);
				m_filter_edit.callback(*this);
				m_sim_edit.callback(*this);
				}

			void closing(Window& ui_owner)
				{
				if(m_filter_edit.askSave())
					{r_ctx.exit();}
				}

			void stateChanged(FilterEditor<Self>& editor)
				{
				std::string title_string(editor.filenameSrc());
				if(editor.dirty())
					{title_string+=" * ";}
				r_mainwin.title(title_string.c_str());
				}

			void submit(FilterEditor<Self>& editor)
				{
				m_sim.reset(new Simulation(editor.filenameBinary(),""));
				m_sim_view.simulation(*m_sim.get());
				m_sim_edit.simulation(*m_sim.get());
				m_tabs.activate(1);
				m_tabs.show();
				}

			void submit(SimulationEditor<Self>& simedit)
				{
				if(m_sim)
					{
					m_sim->imagesLoad(m_sim_edit.imagesStaged());
					m_sim_view.simulation(*m_sim.get());
					m_tabs.activate(2);
					m_tabs.show();
					}
				else
					{
					m_filter_edit.help();
					m_tabs.activate(0);
					}
				}

		private:
			std::unique_ptr<Simulation> m_sim;
			TabView m_tabs;
				FilterEditor<Self> m_filter_edit;
				SimulationEditor<Self> m_sim_edit;
				SimulationView m_sim_view;
			
			UiContext& r_ctx;
			Window& r_mainwin;
		};
	}

int main(int argc, char *argv[])
	{
	Tiger::UiContext ctx;
	Tiger::Window mainwin("Tiger",0);
	Tiger::Ui ui(ctx,mainwin);
	mainwin.show();
	ctx.run();
	return 0;
	}
