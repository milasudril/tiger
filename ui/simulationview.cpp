//@	 {"targets":[{"name":"simulationview.o","type":"object"}]}

#include "simulationview.hpp"
#include "../engine/simulation.hpp"

using namespace Tiger;

SimulationViewBase::SimulationViewBase(Container& cnt):
	 m_top(cnt,1)
		,m_toolbar(m_top,0,0)
		,m_lower(m_top.insertMode({2,Box::FILL|Box::EXPAND}),0)
			,m_img_selector(m_lower,1,1)
			,m_img_view(m_lower.insertMode({2,Box::FILL|Box::EXPAND}),0)
	,m_ch_current(0)
	{
	m_toolbar.append("Run").append("Pause").append("Reset").callback(*this);
	}

void SimulationViewBase::clicked(ButtonList<Self>& list,Button& btn)
	{
	if(list.id()==1)
		{
		if(m_ch_current!=-1)
			{
		//	m_img_ranges[m_ch_current]=m_img_view.range();
			list[m_ch_current].state(0);
			}

		auto id=btn.id();
		m_img_view.channel(id); //.range(m_img_ranges[id]);
		m_ch_current=id;
		btn.state(1);
		}
	else
		{
		switch(btn.id())
			{
			case 0:
				run();
				break;
			case 1:
				pause();
				btn.state(0);
				break;
			case 2:
				reset();
				btn.state(0);
				break;
			}
		}
	}

void SimulationViewBase::running(bool status)
	{m_toolbar[0].state(status);}

SimulationViewBase& SimulationViewBase::simulation(Simulation& sim)
	{
	if(sim.channelCount()!=static_cast<int>(m_img_selector.size()))
		{
		m_img_selector.clear();
		sim.channelsList([this](const Simulation& sim,const char* ch)
			{m_img_selector.append(ch);});
		m_img_selector.callback(*this);
		m_img_view.image(&sim.stateCurrent(),0);
		m_ch_current=-1;
		}
	else
		{m_img_view.image(&sim.stateCurrent(),m_ch_current==-1?0:m_ch_current);}
	return *this;
	}

