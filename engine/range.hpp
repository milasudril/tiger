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
//@	{"targets":[{"name":"range.hpp","type":"include"}]}

#ifndef TIGER_RANGE_HPP
#define TIGER_RANGE_HPP

#include <cassert>
#include <cmath>

namespace Tiger
	{
	class Range
		{
		public:
			Range():m_min(0),m_max(1){}

			explicit Range(double mi,double ma)
				{
				m_min=mi;
				m_max=ma;
				}

			double min() const noexcept
				{return m_min;}

			double max() const noexcept
				{return m_max;}

			Range& max(double x) noexcept
				{
				m_max=x;
				return *this;
				}

			Range& min(double x) noexcept
				{
				m_min=x;
				return *this;
				}

			double length() const noexcept	
				{return m_max - m_min;}

			double mid() const noexcept
				{return 0.5*(m_max + m_min);}

			bool valid() const noexcept
				{return m_max >= m_min;}

			void invalidate()
				{
				m_min=INFINITY;
				m_max=-INFINITY;
				}

		private:
			double m_min;
			double m_max;
		};
	}

#endif // TIGER_RANGE_HPP
