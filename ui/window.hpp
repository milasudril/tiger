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

#include "container.hpp" // Base class: Tiger::Container

namespace Tiger
	{
	class Window final:public Container
		{
		public:
			Window(const Window& rhs)=delete;
			Window& operator=(const Window& rhs)=delete;
			Window(const char* title,bool mainwin);
			~Window();

			virtual	Window& add(void* handle);
			virtual void show();
			const char* title() const noexcept;
			Window& title(const char* title_new);

		private:
			class Impl;
			Impl* m_impl;
		};
	}

#endif // TIGER_WINDOW_HPP
