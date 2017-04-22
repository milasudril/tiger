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
//@	 "targets":[{"name":"buttonlist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"buttonlist.o","rel":"implementation"}]
//@	}

#ifndef TIGER_BUTTONLIST_HPP
#define TIGER_BUTTONLIST_HPP

#include "scrolledwindow.hpp"
#include "box.hpp"
#include "button.hpp"
#include <vector>
#include <algorithm>

namespace Tiger
	{
	class ButtonListImpl
		{
		public:
			explicit ButtonListImpl(Container& cnt,int id,bool vertical):m_id(id)
				,m_scroll(cnt)
					,m_box_main(m_scroll,vertical)
						,m_box(m_box_main,vertical)
				{
				m_box.homogenous(1).insertMode({0,Box::EXPAND|Box::FILL});
				}

			ButtonListImpl& clear() noexcept
				{
				m_buttons.clear();	
				return *this;
				}
			
			int id() const noexcept
				{return m_id;}

			auto begin() noexcept
				{return m_buttons.begin();}

			auto end() noexcept
				{return m_buttons.end();}

			Button& operator[](int k) noexcept
				{return m_buttons[k];}

			ButtonListImpl& append(const char* text)
				{
				m_buttons.push_back(Button(m_box,static_cast<int>(m_buttons.size())
					,text));
				return *this;
				}

			ButtonListImpl& append(const char* const* labels);

			Button& back() noexcept
				{return m_buttons.back();}

			size_t size() const noexcept
				{return m_buttons.size();}

		private:
			int m_id;
			ScrolledWindow m_scroll;
				Box m_box_main;
					Box m_box;
					
			std::vector<Button> m_buttons;
		};

	template<class Callback>
	class ButtonList
		{
		public:
			explicit ButtonList(Container& cnt,int id,bool vertical):r_callback(nullptr)
				,m_impl(cnt,id,vertical)
				{}

			ButtonList& clear() noexcept
				{
				m_impl.clear();	
				return *this;
				}

			ButtonList& append(const char* text)
				{
				m_impl.append(text);
				return *this;
				}

			ButtonList& callback(Callback& cb) noexcept
				{
				r_callback=&cb;
				std::for_each(m_impl.begin(),m_impl.end()
					,[this](Button& btn)
						{btn.callback(*this);});

				return *this;
				}

			void clicked(Button& btn)
				{r_callback->clicked(*this,btn);}
			
			int id() const noexcept
				{return m_impl.id();}

			auto begin() noexcept
				{return m_impl.begin();}

			auto end() noexcept
				{return m_impl.end();}

			Button& operator[](int k) noexcept
				{return m_impl[k];}

			ButtonList& append(const char* const* labels)
				{
				m_impl.append(labels);
				return *this;
				}

			size_t size() const noexcept
				{return m_impl.size();}

		private:
			Callback* r_callback;
			ButtonListImpl m_impl;
		};
	}

#endif // TIGER_BUTTONLIST_HPP
