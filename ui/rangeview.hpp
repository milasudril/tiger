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

namespace Tiger
	{
	class Container;
	class Range;
	class RangeView
		{
		public:
			RangeView(const RangeView& rhs)=delete;
			RangeView& operator=(const RangeView& rhs)=delete;

			RangeView(Container& container) noexcept;
			~RangeView();
			
			template<class RangeViewCallback>
			RangeView& callback(RangeViewCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,RangeView& self)
					{
					auto x=reinterpret_cast<RangeViewCallback*>(rvc);
					(*x)(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			Range range() const noexcept;
			RangeView& range(const Range& range) noexcept;

		private:
			typedef void (*Callback)(void* cb_obj,RangeView& self);
			RangeView& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;
		};
	}

#endif // TIGER_RANGEVIEW_HPP
