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
//@	{"targets":[{"name":"sourceview.o","type":"object","pkgconfig_libs":["gtksourceview-3.0"]}]}

#include "sourceview.hpp"
#include "container.hpp"
#include "../engine/datasource.hpp"
#include <gtksourceview/gtksource.h>
#include <cstring>

using namespace Tiger;

class SourceView::Impl final:private SourceView
	{
	public:
		explicit Impl(Container& cnt,int id);
		~Impl();

		void lineNumbers(bool status) noexcept
			{gtk_source_view_set_show_line_numbers(m_handle,status);}

		void highlight(const char* filename_pattern);

		const char* content() const;

		void content(const char* text)
			{
			auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
			gtk_text_buffer_set_text(buffer,text,-1);
			}

		void content(DataSource& src);

		size_t contentSize() const
			{return strlen(content());}

		void readonly(bool status)
			{
			gtk_text_view_set_editable(GTK_TEXT_VIEW(m_handle),!status);
			}

		int id() const noexcept
			{return m_id;}

		void callback(Callback cb,void* cb_obj)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			}
	
		size_t _write(const void* buffer,size_t n_bytes);

		void wordwrap(bool status)
			{
			gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(m_handle)
				,status?GTK_WRAP_WORD:GTK_WRAP_NONE);
			}

	private:
		Callback r_cb;
		void* r_cb_obj;
		int m_id;
		GtkSourceView* m_handle;
		GtkScrolledWindow* m_scroll;
		GtkCssProvider* m_style;
		mutable char* m_content;

		static void changed_callback(GtkTextBuffer* buffer,gpointer user_data);
	};

SourceView::SourceView(Container& cnt,int id)
	{m_impl=new Impl(cnt,id);}

SourceView::~SourceView()
	{delete m_impl;}

SourceView& SourceView::lineNumbers(bool status)
	{
	m_impl->lineNumbers(1);
	return *this;
	}

SourceView& SourceView::highlight(const char* filename_pattern)
	{
	m_impl->highlight(filename_pattern);
	return *this;
	}

const char* SourceView::content() const
	{return m_impl->content();}

SourceView& SourceView::content(const char* text)
	{
	m_impl->content(text);
	return *this;
	}

SourceView& SourceView::content(DataSource&& src)
	{
	m_impl->content(src);
	return *this;
	}

SourceView& SourceView::readonly(bool status)
	{
	m_impl->readonly(status);
	return *this;
	}

size_t SourceView::contentSize() const
	{return m_impl->contentSize();}

SourceView& SourceView::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}

int SourceView::id() const noexcept
	{return m_impl->id();}

size_t SourceView::write(const void* buffer,size_t n_bytes)
	{return m_impl->_write(buffer,n_bytes);}

SourceView& SourceView::wordwrap(bool status)
	{
	m_impl->wordwrap(status);
	return *this;
	}



SourceView::Impl::Impl(Container& cnt,int id):SourceView(*this)
	{
	m_id=id;
	r_cb=nullptr;
	auto widget=gtk_source_view_new();
	m_handle=GTK_SOURCE_VIEW(widget);
	auto scroll=gtk_scrolled_window_new(NULL,NULL);
	m_scroll=GTK_SCROLLED_WINDOW(scroll);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll),GTK_SHADOW_IN);
	gtk_source_view_set_tab_width(m_handle,4);

	m_style=gtk_css_provider_new();
	gtk_css_provider_load_from_data(m_style,"*{font-size:9pt;font-family:\"Inconsolata\",monospace}",-1,NULL);
	auto context=gtk_widget_get_style_context(widget);
	gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(m_style),
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	g_signal_connect(buffer,"changed",G_CALLBACK(changed_callback),this);
	g_object_ref_sink(m_handle);
	gtk_container_add(GTK_CONTAINER(scroll),widget);
	g_object_ref_sink(m_scroll);
	cnt.add(scroll);
	m_content=nullptr;
	}

SourceView::Impl::~Impl()
	{
	r_cb=nullptr;
	m_impl=nullptr;
	if(m_content!=nullptr)
		{g_free(m_content);}
	auto context=gtk_widget_get_style_context(GTK_WIDGET(m_handle));
	gtk_style_context_remove_provider(context,GTK_STYLE_PROVIDER(m_style));
	g_object_unref(m_style);
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	gtk_widget_destroy(GTK_WIDGET(m_scroll));
	}

const char* SourceView::Impl::content() const
	{
	if(m_content!=nullptr)
		{return m_content;}
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_start_iter(buffer,&start);
	gtk_text_buffer_get_end_iter(buffer,&end);
	m_content=gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
	return m_content;
	}

void SourceView::Impl::highlight(const char* filename_pattern)
	{
	auto manager=gtk_source_language_manager_get_default();
	auto lang=gtk_source_language_manager_guess_language(manager,filename_pattern,NULL);
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buffer),lang);
	}

void SourceView::Impl::content(DataSource& src)
	{
	constexpr size_t N=4096;
	char text[N];
	size_t n=0;
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	content("");
	do
		{
		n=src.read(text,N);
		gtk_text_buffer_insert_at_cursor(buffer,text,static_cast<int>(n));
		}
	while(n==N);
	}

void SourceView::Impl::changed_callback(GtkTextBuffer* buffer,gpointer user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	g_free(self->m_content);
	self->m_content=nullptr;
	if(self->r_cb!=nullptr)
		{self->r_cb(self->r_cb_obj,*self);}
	}

size_t SourceView::Impl::_write(const void* buffer,size_t n_bytes)
	{
	auto buf=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	auto char_buff=reinterpret_cast<const char*>(buffer);
	const char* end;
//TODO: This is probably not the best thing to do
	g_utf8_validate(char_buff,n_bytes,&end);
	gtk_text_buffer_insert_at_cursor(buf,char_buff,static_cast<int>(end - char_buff));
	return n_bytes;
	}