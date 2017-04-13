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
//@	{"targets":[{"name":"entry.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "entry.hpp"
#include "range.hpp"
#include "container.hpp"
#include <gtk/gtk.h>
#include <algorithm>

using namespace Tiger;

class Entry::Impl
	{
	public:
		Impl(Container& cnt,Entry& owner);
		~Impl();

		void callback(Callback cb,void* cb_obj)
			{
			m_cb=cb;
			cb_obj=m_cb_obj;
			}

		const char* content() const noexcept
			{return gtk_entry_get_text(m_handle);}

		void content(const char* text) noexcept
			{return gtk_entry_set_text(m_handle,text);}

	private:
		Callback m_cb;
		void* m_cb_obj;
		Entry& r_owner;
		GtkEntry* m_handle;

		static gboolean focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data);
	};

Entry::Entry(Container& cnt) noexcept
	{
	m_impl=new Impl(cnt,*this);
	}

Entry::~Entry()
	{
	if(m_impl!=nullptr)
		{delete m_impl;}
	}

Entry& Entry::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}

const char* Entry::content() const noexcept
	{return m_impl->content();}

Entry& Entry::content(const char* text)
	{
	m_impl->content(text);
	return *this;
	}


Entry::Impl::Impl(Container& cnt,Entry& owner):m_cb(nullptr)
	,r_owner(owner)
	{
	printf("Entry %p ctor\n",this);

	auto widget=gtk_entry_new();
	gtk_widget_set_size_request(widget,32,32);
	g_signal_connect(widget,"focus-out-event",G_CALLBACK(focus_callback),this);
	m_handle=GTK_ENTRY(widget);
	cnt.add(widget);
	}

Entry::Impl::~Impl()
	{
	printf("Entry %p dtor\n",this);
	}

gboolean Entry::Impl::focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	if(state->m_cb!=nullptr)
		{(state->m_cb)(state->m_cb_obj,state->r_owner);}
	return FALSE;
	}
