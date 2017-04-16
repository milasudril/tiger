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
//@	 {"targets":[{"name":"simulationeditor.o","type":"object"}]}

#include "simulationeditor.hpp"
#include "../engine/simulation.hpp"

using namespace Tiger;

SimulationEditor::SimulationEditor(Container& cnt,int id):m_id(id),r_sim(nullptr)
	,m_top(cnt,0)
		,m_left(m_top,1)
			,m_init_label(m_left,"Initial conditions")
			,m_init_list(m_left.insertMode({4,Box::FILL|Box::EXPAND}),0,1)
		,m_right(m_top.insertMode({4,Box::FILL|Box::EXPAND}),1)
			,m_param_label(m_right,"Parameters")
			,m_params(m_right.insertMode({0,Box::FILL|Box::EXPAND}),0,ParamDataDescriptor{})
	{
	m_init_list.callback(*this);
	}

void SimulationEditor::operator()(ButtonList<SimulationEditor>& list,Button& btn)
	{
	printf("Clicked %d:%d\n",list.id(),btn.id());
	}


SimulationEditor& SimulationEditor::simulation(Simulation& sim)
	{
	sim.paramsList([this](const Simulation& sim,const char* const& name
		,float& value)
		{m_params.recordAppend(name,value);});

	sim.channelsList([this](const Simulation& sim,const char* ch)
		{
		m_init_list.append(ch);
		});

	r_sim=&sim;
	return *this;
	}
