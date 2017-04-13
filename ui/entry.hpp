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
//@  "targets":[{"name":"entry.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"entry.o","rel":"implementation"}]
//@	}

#ifndef TIGER_ENTRY_HPP
#define TIGER_ENTRY_HPP

namespace Tiger
	{
	class Container;

	class Entry
		{
		public:
			Entry(const Entry& rhs)=delete;
			Entry& operator=(const Entry& rhs)=delete;

			Entry(Container& container) noexcept;
			~Entry();
			
			template<class EntryCallback>
			Entry& callback(EntryCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,Entry& self)
					{
					auto x=reinterpret_cast<EntryCallback*>(rvc);
					(*x)(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			const char* content() const noexcept;

			Entry& content(const char* text);

		private:
			typedef void (*Callback)(void* cb_obj,Entry& self);
			Entry& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;
		};
	}

#endif // TIGER_RANGEVIEW_HPP
