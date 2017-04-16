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

#ifndef TIGER_SEPARATOR_HPP
#define TIGER_SEPARATOR_HPP

#include <memory>

namespace Tiger
	{
	class Container;

	class Separator
		{
		public:
			explicit Separator(Container& container,bool vertical) noexcept;
			~Separator();

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
