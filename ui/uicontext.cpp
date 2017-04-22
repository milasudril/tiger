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
//@	 "targets":[{"name":"uicontext.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "uicontext.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

class UiContext::Impl:public UiContext
	{
	public:
		Impl():UiContext(*this),m_stop(0)
			{gtk_init(NULL,NULL);}

		~Impl();

		void exit()
			{m_stop=1;}

		void run();

		void ui_update(UiUpdate update,void* cb)
			{
			gdk_threads_add_idle(update,cb);
			}

	private:
	//	static gboolean update_callback(gpointer data)

		volatile bool m_stop;
	};

UiContext::UiContext()
	{m_impl=new Impl;}

UiContext::~UiContext()
	{delete m_impl;}

void UiContext::exit()
	{m_impl->exit();}

void UiContext::run()
	{m_impl->run();}

void UiContext::ui_update(UiUpdate update,void* cb)
	{m_impl->ui_update(update,cb);}


UiContext::Impl::~Impl()
	{m_impl=nullptr;}

void UiContext::Impl::run()
	{
	m_stop=0;
	while(!m_stop)
		{
		gtk_main_iteration();
		}
	}
