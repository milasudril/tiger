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

	class SimulationViewBase
		{
		public:
			typedef SimulationViewBase Self;
			SimulationViewBase(Container& cnt);
			void clicked(ButtonList<Self>& list,Button& btn);
			SimulationViewBase& simulation(Simulation& sim);
			void running(bool status);

		private:
			Box m_top;
				ButtonList<Self> m_toolbar;
				Box m_lower;
					ButtonList<Self> m_img_selector;
					ImageView m_img_view;
			Image m_img_disp;
			int m_ch_current;

			virtual void run(){}
			virtual void pause(){}
			virtual void reset(){}
		};

	template<class Callback>
	class SimulationView:public SimulationViewBase
		{
		public:
			SimulationView(Container& cnt,int id):SimulationViewBase(cnt),m_id(id)
				,r_cb(nullptr)
				{}

			void run()
				{
				if(r_cb!=nullptr)
					{r_cb->run(*this);}
				}

			void pause()
				{
				if(r_cb!=nullptr)
					{r_cb->pause(*this);}
				}

			void reset()
				{
				if(r_cb!=nullptr)
					{r_cb->reset(*this);}
				}

			SimulationView& callback(Callback& cb) noexcept
				{
				r_cb=&cb;
				return *this;
				}

		private:
			int m_id;
			Callback* r_cb;
		};
	}

#endif // TIGER_SIMULATIONVIEW_HPP
