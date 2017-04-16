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
//@	{"targets":[{"name":"buttonlist.hpp","type":"include"}]}

#ifndef TIGER_BUTTONLIST_HPP
#define TIGER_BUTTONLIST_HPP

#include "scrolledwindow.hpp"
#include "box.hpp"
#include "button.hpp"
#include <vector>

namespace Tiger
	{
	template<class Callback>
	class ButtonList
		{
		public:
			ButtonList(Container& cnt,int id,bool vertical):r_callback(nullptr)
				,m_id(id)
				,m_scroll(cnt)
				,m_box(m_scroll,vertical)
				{}

			ButtonList& clear() noexcept
				{
				m_buttons.clear();	
				return *this;
				}

			ButtonList& append(const char* text)
				{
				m_buttons.push_back(Button(m_box,static_cast<int>(m_buttons.size())
					,text));
				m_buttons.back().callback(*this);
				return *this;
				}

			ButtonList& callback(Callback& cb) noexcept
				{
				r_callback=&cb;
				return *this;
				}

			void operator()(Button& btn)
				{
				if(r_callback!=nullptr)
					{(*r_callback)(*this,btn);}
				}
			
			int id() const noexcept
				{return m_id;}

		private:
			Callback* r_callback;
			int m_id;
			ScrolledWindow m_scroll;
				Box m_box;
			std::vector<Button> m_buttons;
		};
	}

#endif // TIGER_BUTTONLIST_HPP
