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
#include <gtksourceview/gtksource.h>

using namespace Tiger;

class SourceView::Impl:private SourceView
	{
	public:
		explicit Impl(Container& cnt);
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

	private:
		GtkSourceView* m_handle;
		GtkCssProvider* m_style;
		mutable char* m_content;
	};

SourceView::SourceView(Container& cnt)
	{m_impl=new Impl(cnt);}

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
	return *this;
	}

SourceView::Impl::Impl(Container& cnt):SourceView(*this)
	{
	auto widget=gtk_source_view_new();
	m_handle=GTK_SOURCE_VIEW(widget);
	auto scroll=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll),GTK_SHADOW_IN);
	gtk_source_view_set_tab_width(m_handle,4);

	m_style=gtk_css_provider_new();
	gtk_css_provider_load_from_data(m_style,"*{font-size:9pt;font-family:\"Inconsolata\",monospace}",-1,NULL);
	auto context=gtk_widget_get_style_context(widget);
	gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(m_style),
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	g_object_ref_sink(m_handle);
	gtk_container_add(GTK_CONTAINER(scroll),widget);
	cnt.add(scroll);
	m_content=nullptr;
	}

SourceView::Impl::~Impl()
	{
	m_impl=nullptr;
	if(m_content!=nullptr)
		{g_free(m_content);}
	auto context=gtk_widget_get_style_context(GTK_WIDGET(m_handle));
	gtk_style_context_remove_provider(context,GTK_STYLE_PROVIDER(m_style));
	g_object_unref(m_style);
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	}

const char* SourceView::Impl::content() const
	{
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_start_iter(buffer,&start);
	gtk_text_buffer_get_end_iter(buffer,&end);
	if(m_content!=nullptr)
		{g_free(m_content);}
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
