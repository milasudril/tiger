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
//@  "targets":[{"name":"button.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"button.o","rel":"implementation"}]
//@	}

#ifndef TIGER_BUTTON_HPP
#define TIGER_BUTTON_HPP

#include <memory>

namespace Tiger
	{
	class Container;

	class Button
		{
		public:
			Button(const Button& rhs)=delete;
			Button& operator=(const Button& rhs)=delete;

			explicit Button(Container& container,int id,const char* label) noexcept;
			~Button();
			
			template<class Callback>
			Button& callback(Callback& cb)
				{
				auto cb_wrapper=[](void* rvc,Button& self)
					{
					auto x=reinterpret_cast<Callback*>(rvc);
					(*x)(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			const char* label() const noexcept;

			Button& label(const char* text);

			Button& width(int n) noexcept;
			
			int id() const noexcept;

		private:
			typedef void (*Callback)(void* cb_obj,Button& self);
			Button& callback(Callback cb,void* cb_obj);
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif // TIGER_RANGEVIEW_HPP
