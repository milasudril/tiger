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
//@	{
//@	"targets":[{"name":"simulation.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"simulation.o","rel":"implementation"}]
//@	}

#ifndef TIGER_SIMULATION_HPP
#define TIGER_SIMULATION_HPP

#include "filter.hpp"
#include "image.hpp"
#include <vector>

namespace Tiger
	{
	class Channel;
	class Parameter;
	class Simulation
		{
		public:
			typedef const char* ParamName;

			Simulation(const char* filter,const char* objdir);

			template<class ProcessCallback>
			Simulation& run(ProcessCallback&& pc) noexcept
				{return run(pc);}
	
			template<class ProcessCallback>
			Simulation& run(ProcessCallback& pc) noexcept
				{
				auto cb=[](void* processcallback,const Simulation& sim
					,unsigned long long iter_count)
					{
					auto obj=reinterpret_cast<ProcessCallback*>(processcallback);
					return (*obj)(sim,iter_count);
					};
				return run(cb,&pc);
				}

			Simulation& imagesLoad(const std::vector<Channel>& files_src
				,const std::vector<Channel>& files_init);

			Simulation& imagesStore(const std::vector<Channel>& files)
				{
				imagesStore(m_filter,m_state,files);
				return *this;
				}

			Simulation& paramsLoad(const std::vector<Parameter>& params);

			template<class Paramproc>
			const Simulation& paramsList(Paramproc&& proc) const
				{return paramsList(proc);}

			template<class Paramproc>
			const Simulation& paramsList(Paramproc& proc) const
				{
				auto cb=[](void* paramproc,const Simulation& sim,const ParamName& name,const float& value)
					{
					auto obj=reinterpret_cast<Paramproc*>(paramproc);
					(*obj)(sim,name,value);
					};
				return paramsList(cb,&proc);
				}

			template<class Paramproc>
			Simulation& paramsList(Paramproc&& proc)
				{return paramsList(proc);}

			template<class Paramproc>
			Simulation& paramsList(Paramproc& proc)
				{
				auto cb=[](void* paramproc,const Simulation& sim,const ParamName& name,float& value)
					{
					auto obj=reinterpret_cast<Paramproc*>(paramproc);
					(*obj)(sim,name,value);
					};
				return paramsList(cb,&proc);
				}

			template<class ChannelProc>
			const Simulation& channelsList(ChannelProc&& proc) const
				{return channelsList(proc);}

			template<class ChannelProc>
			const Simulation& channelsList(ChannelProc& proc) const
				{
				auto cb=[](void* channelproc,const Simulation& sim,const char* name)
					{
					auto obj=reinterpret_cast<ChannelProc*>(channelproc);
					(*obj)(sim,name);
					};
				return channelsList(cb,&proc);
				}

			unsigned long long frameStart() const noexcept
				{return m_frame_current;}

		private:
			static Image imagesLoad(const std::vector<Channel>& files
				,const Tiger::Filter& f);
			static void imagesStore(const Filter& f,const FilterState& d
				,const std::vector<Channel>& files);

			typedef bool(*ProcessMonitor)(void* processcallback,const Simulation& sim
				,unsigned long long iter_count);
			Simulation& run(ProcessMonitor monitor,void* processcallback) noexcept;

			typedef void(*ParamsListCallback)(void* paramproc,const Simulation&,const ParamName&,const float&);
			const Simulation& paramsList(ParamsListCallback cb,void* paramproc) const;

			typedef void(*ParamsListByRefCallback)(void* paramproc,const Simulation&,const ParamName&,float&);
			Simulation& paramsList(ParamsListByRefCallback cb,void* paramproc);

			typedef void(*ChannelsListCallback)(void* chproc,const Simulation&,const char*);
			const Simulation& channelsList(ChannelsListCallback cb,void* chproc) const;

			FilterState m_state;
			Image m_source;
			Image m_current;
			Image m_next;
			std::vector<float> m_params;
			Filter m_filter;
			unsigned long long m_frame_current;
		};
	}

#endif // TIGER_SIMULATION_hpp
