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
//@  "targets":[{"name":"paned.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paned.o","rel":"implementation"}]
//@	}

#ifndef TIGER_PANED_HPP
#define TIGER_PANED_HPP

#include "container.hpp"
#include <utility>

namespace Tiger
	{
	class Paned:public Container
		{
		public:
			static constexpr unsigned short RESIZE=1;
			static constexpr unsigned short SHRINK_ALLOWED=2;

			struct InsertMode
				{unsigned short flags;};

			explicit Paned(Container& parent,bool vertical);
			~Paned();

			Paned& operator=(Paned&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Paned(Paned&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Paned& add(void* handle);
			Paned& show();
			Paned& sensitive(bool val);
			void* toplevel() const;
			Paned& insertMode(const InsertMode& mode) noexcept;

		protected:
			class Impl;
			explicit Paned(Paned::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // TIGER_Paned_HPP
