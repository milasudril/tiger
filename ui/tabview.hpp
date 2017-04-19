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
//@  "targets":[{"name":"tabview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"tabview.o","rel":"implementation"}]
//@	}

#ifndef TIGER_TABVIEW_HPP
#define TIGER_TABVIEW_HPP

#include "container.hpp"
#include <utility>

namespace Tiger
	{
	class TabView:public Container
		{
		public:
			explicit TabView(Container& parent);
			~TabView();

			TabView& operator=(TabView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			TabView(TabView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			TabView& add(void* handle);
			TabView& show();
			TabView& sensitive(bool val);
			TabView& tabTitle(const char* t);
			void* toplevel() const;

		protected:
			class Impl;
			explicit TabView(TabView::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // TIGER_TabView_HPP
