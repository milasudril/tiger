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
			Simulation(const char* filter,const char* objdir);
			~Simulation();
			Simulation& run(unsigned long long n_iter) noexcept;

			Simulation& sourceLoad(const std::vector<Channel>& files)
				{
				m_source=imagesLoad(files,m_filter);
				return *this;
				}

			Simulation& initLoad(const std::vector<Channel>& files)
				{
				m_current=imagesLoad(files,m_filter);
				return *this;
				}

			Simulation& imagesStore(const std::vector<Channel>& files)
				{
				imagesStore(m_filter,m_state,files);
				return *this;
				}

			Simulation& paramsLoad(const std::vector<Parameter>& params);

			Simulation& paramsStore();

		private:
			static Image imagesLoad(const std::vector<Channel>& files
				,const Tiger::Filter& f);
			static void imagesStore(const Filter& f,const FilterState& d
				,const std::vector<Channel>& files);

			Filter m_filter;
			FilterState m_state;
			Image m_source;
			Image m_current;
			Image m_next;
			unsigned long long m_frame_current;
		};
	}

#endif // TIGER_SIMULATION_hpp
