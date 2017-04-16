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
//@  "targets":[{"name":"scrolledwindow.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"scrolledwindow.o","rel":"implementation"}]
//@	}

#ifndef TIGER_SCROLLEDWINDOW_HPP
#define TIGER_SCROLLEDWINDOW_HPP

#include "container.hpp"
#include <memory>

namespace Tiger
	{
	class ScrolledWindow final:public Container
		{
		public:
			explicit ScrolledWindow(Container& parent);
			~ScrolledWindow();

			virtual	ScrolledWindow& add(void* handle);
			virtual void show();

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
