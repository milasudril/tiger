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
#include <utility>

namespace Tiger
	{
	class ScrolledWindow:public Container
		{
		public:
			explicit ScrolledWindow(Container& parent);
			~ScrolledWindow();

			ScrolledWindow& operator=(ScrolledWindow& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ScrolledWindow(ScrolledWindow&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ScrolledWindow& add(void* handle);
			ScrolledWindow& show();
			ScrolledWindow& sensitive(bool val);

		protected:
			class Impl;
			Impl* m_impl;

			explicit ScrolledWindow(Impl& impl):m_impl(&impl){}
		};
	}

#endif
