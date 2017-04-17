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
//@  "targets":[{"name":"textentry.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"textentry.o","rel":"implementation"}]
//@	}

#ifndef TIGER_TEXTENTRY_HPP
#define TIGER_TEXTENTRY_HPP

#include <utility>

namespace Tiger
	{
	class Container;

	class TextEntry
		{
		public:
			explicit TextEntry(Container& container,int id);
			~TextEntry();

			TextEntry& operator=(TextEntry&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			TextEntry(TextEntry&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}
			
			template<class EntryCallback>
			TextEntry& callback(EntryCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,TextEntry& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					(*x)(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			const char* content() const noexcept;

			TextEntry& content(const char* text);

			TextEntry& width(int n) noexcept;

			TextEntry& small(bool status) noexcept;

			TextEntry& alignment(float x) noexcept;

			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,TextEntry& self);
			TextEntry& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;
			explicit TextEntry(Impl& impl):m_impl(&impl){}
		};
	}

#endif // TIGER_RANGEVIEW_HPP
