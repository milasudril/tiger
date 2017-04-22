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
//@  "targets":[{"name":"window.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"window.o","rel":"implementation"}]
//@	}

#ifndef TIGER_WINDOW_HPP
#define TIGER_WINDOW_HPP

#include "container.hpp"
#include <utility>

namespace Tiger
	{
	class Window:public Container
		{
		public:
			explicit Window(const char* title,int id);
			~Window();

			Window& operator=(Window&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Window(Window&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Window& add(void* handle);
			Window& show();
			Window& sensitive(bool val);
			void* toplevel() const;

			const char* title() const noexcept;
			Window& title(const char* title_new);

			template<class WindowCallback>
			Window& callback(WindowCallback& cb) noexcept
				{
				auto cb_wrapper=[](void* wc,Window& self)
					{
					auto x=reinterpret_cast<WindowCallback*>(wc);
					x->closing(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,Window& self);
			Window& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;
			explicit Window(Impl& impl):m_impl(&impl){}
		};
	}

#endif
