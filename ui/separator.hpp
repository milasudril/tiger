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
//@  "targets":[{"name":"separator.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"separator.o","rel":"implementation"}]
//@	}

#ifndef TIGER_DECORATOR_HPP
#define TIGER_DECORATOR_HPP

namespace Tiger
	{
	class Container;

	class Separator
		{
		public:
			Separator(const Separator& rhs)=delete;
			Separator& operator=(const Separator& rhs)=delete;

			explicit Separator(Container& container,bool vertical) noexcept;
			~Separator();

		private:
			class Impl;
			Impl* m_impl;
		};
	}

#endif // TIGER_RANGEVIEW_HPP
