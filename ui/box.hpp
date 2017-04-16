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
//@  "targets":[{"name":"box.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"box.o","rel":"implementation"}]
//@	}

#ifndef TIGER_BOX_HPP
#define TIGER_BOX_HPP

#include "container.hpp"
#include <memory>

namespace Tiger
	{
	class Box final:public Container
		{
		public:
			static constexpr unsigned short FILL=1;
			static constexpr unsigned short EXPAND=2;
			struct InsertMode
				{
				unsigned short padding;
				unsigned short flags;
				};

			explicit Box(Container& parent,bool vertical);
			~Box();

			virtual	Box& add(void* handle);
			virtual void show();

			Box& homogenous(bool status) noexcept;
			Box& insertMode(const InsertMode& mode) noexcept;

			Box& alignment(float x) noexcept;

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif // TIGER_Box_HPP
