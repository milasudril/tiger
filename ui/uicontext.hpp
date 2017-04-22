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
//@	 "targets":[{"name":"uicontext.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"uicontext.o","rel":"implementation"}]
//@	}

#ifndef TIGER_UICONTEXT_HPP
#define TIGER_UICONTEXT_HPP

namespace Tiger
	{
	class UiContext
		{
		public:
			UiContext(const UiContext&)=delete;
			UiContext& operator=(const UiContext&)=delete;
			UiContext();
			~UiContext();
			void exit();
			void run();
			
			template<class Callback>
			void uiUpdate(Callback& cb)
				{
				auto cb_wrapper=[](void* cb_obj)
					{
					auto obj=reinterpret_cast<Callback*>(cb_obj);
					obj->uiUpdate();
					return 0;
					};
				ui_update(cb_wrapper,&cb);
				}

		private:
			class Impl;
			Impl* m_impl;
			UiContext(Impl& impl):m_impl(&impl){}
			typedef int (*UiUpdate)(void* cb);
			void ui_update(UiUpdate update,void* cb);
		};
	}

#endif