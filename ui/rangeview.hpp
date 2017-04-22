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
//@  "targets":[{"name":"rangeview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"rangeview.o","rel":"implementation"}]
//@	}

#ifndef TIGER_RANGEVIEW_HPP
#define TIGER_RANGEVIEW_HPP

#include <utility>

namespace Tiger
	{
	class Container;
	class Range;
	class RangeView
		{
		public:
			explicit RangeView(Container& container,int id);
			~RangeView();

			RangeView& operator=(RangeView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			RangeView(RangeView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}
			
			template<class RangeViewCallback>
			RangeView& callback(RangeViewCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,RangeView& self)
					{
					auto x=reinterpret_cast<RangeViewCallback*>(rvc);
					x->changed(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			Range range() const noexcept;
			RangeView& range(const Range& range) noexcept;
			
			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,RangeView& self);
			RangeView& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;

			explicit RangeView(Impl& impl):m_impl(&impl){}
		};
	}

#endif // TIGER_RANGEVIEW_HPP
