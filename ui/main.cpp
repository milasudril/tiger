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

#include <thread>

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
				,m_sim_view(m_tabs.tabTitle("Simulation"),0)
				,r_ctx(ctx)
				,r_mainwin(mainwin)
				,m_continue(0)
				{
				mainwin.callback(*this);
				m_filter_edit.callback(*this);
				m_sim_edit.callback(*this);
				m_sim_view.callback(*this);
				}

			void closing(Window& ui_owner)
				{
				if(m_filter_edit.askSave())
					{
					pause(m_sim_view);
					r_ctx.exit();
					}
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
				pause(m_sim_view);
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
					pause(m_sim_view);
					m_sim->imagesLoad(simedit.imagesStaged());
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

			void run(SimulationView<Self>& simview)
				{
				if(m_continue==0)
					{
					m_continue=1;
					m_render_thread=std::thread([this](){m_sim->run(*this);});
					simview.running(1);
					}
				}

			void pause(SimulationView<Self>& simview)
				{
				if(m_continue)
					{
					m_continue=0;
					m_render_thread.join();
					m_sim_view.running(0);
					}
				}

			void reset(SimulationView<Self>& simview)
				{
				pause(simview);
				submit(m_sim_edit);
				}

			bool operator()(const Simulation& sim,unsigned long long iter_count)
				{
				if(iter_count%16==0)
					{r_ctx.uiUpdate(*this);}
				return m_continue;
				}

			void uiUpdate()
				{m_sim_view.simulation(*m_sim.get());}

		private:
			std::unique_ptr<Simulation> m_sim;
			TabView m_tabs;
				FilterEditor<Self> m_filter_edit;
				SimulationEditor<Self> m_sim_edit;
				SimulationView<Self> m_sim_view;
			
			UiContext& r_ctx;
			Window& r_mainwin;
			bool m_continue;
			std::thread m_render_thread;
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
