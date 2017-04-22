//@	{
//@	 "targets":[{"name":"simulationview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"simulationview.o","rel":"implementation"}]
//@	}

#ifndef TIGER_SIMULATIONVIEW_HPP
#define TIGER_SIMULATIONVIEW_HPP

#include "imageview.hpp"
#include "buttonlist.hpp"

namespace Tiger
	{
	class Simulation;

	class SimulationView
		{
		public:
			typedef SimulationView Self;
			SimulationView(Container& cnt);
			void clicked(ButtonList<Self>& list,Button& btn);
			SimulationView& simulation(Simulation& sim);

		private:
			Box m_top;
				ButtonList<Self> m_toolbar;
				Box m_lower;
					ButtonList<Self> m_img_selector;
					ImageView m_img_view;
			int m_ch_current;

			virtual void run(){}
			virtual void pause(){}
			virtual void reset(){}
		};
	}

#endif // TIGER_SIMULATIONVIEW_HPP
