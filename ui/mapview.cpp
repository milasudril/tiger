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
//@	 {"targets":[{"name":"mapview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "mapview.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

class MapViewBase::Impl
	{
	public:
		explicit Impl(Container& cnt,const DataDescriptorImpl& descriptor
			,void* owner);
		~Impl();

		void clear();

		void recordAppend(const void* key,const void* value);

		void keyAlignment(float val);

		void valueAlignment(float val);

		void callback(ValueSetCallback cb,void* cb_obj)
			{
			m_value_update=cb;
			m_cb_obj=cb_obj;
			}

	private:
		StringCallback m_string_from_key;
		StringCallback m_string_from_value;
		const void* m_cb_user_data;
		ValueSetCallback m_value_update;
		void* m_cb_obj;
		void* r_owner;

		GtkTreeView* m_handle;
		GtkListStore* m_list;
		GtkCellRendererText* m_key_renderer;
		GtkCellRendererText* m_val_renderer;

		static void key_fetch(GtkTreeViewColumn* col
			,GtkCellRenderer* renderer
			,GtkTreeModel* model
			,GtkTreeIter* iter
			,gpointer user_data);

		static void value_fetch(GtkTreeViewColumn* col
			,GtkCellRenderer* renderer
			,GtkTreeModel* model
			,GtkTreeIter* iter
			,gpointer user_data);

		static gboolean selection_changed(GtkTreeSelection* selection,gpointer data);
		static void value_edited(GtkCellRendererText* cell,char* path_string
			,char* new_text,gpointer user_data);

	};

MapViewBase::MapViewBase(Container& cnt,const DataDescriptorImpl& descriptor)
	{m_impl.reset(new Impl(cnt,descriptor,this));}

MapViewBase::~MapViewBase()
	{}

MapViewBase& MapViewBase::clear()
	{
	m_impl->clear();
	return *this;
	}

MapViewBase& MapViewBase::recordAppend(const void* key,const void* value)
	{
	m_impl->recordAppend(key,value);
	return *this;
	}

MapViewBase& MapViewBase::keyAlignment(float val)
	{
	m_impl->keyAlignment(val);
	return *this;
	}

MapViewBase& MapViewBase::valueAlignment(float val)
	{
	m_impl->valueAlignment(val);
	return *this;
	}

MapViewBase& MapViewBase::callback(ValueSetCallback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}



MapViewBase::Impl::Impl(Container& cnt,const DataDescriptorImpl& descriptor
	,void* owner):m_cb_obj(nullptr),r_owner(owner)
	{
	m_string_from_key=descriptor.string_from_key;
	m_string_from_value=descriptor.string_from_value;
	m_cb_user_data=descriptor.cb_user_data;

	auto content=gtk_list_store_new(2, G_TYPE_UINT64, G_TYPE_UINT64);
	m_list=content;
	auto tree=gtk_tree_view_new_with_model(GTK_TREE_MODEL(content));
	m_handle=GTK_TREE_VIEW(tree);
	
	auto col=gtk_tree_view_column_new();
	gtk_tree_view_column_set_resizable(col,TRUE);
	gtk_tree_view_column_set_sizing(col,GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_title(col,"");
	auto renderer=gtk_cell_renderer_text_new();
	m_key_renderer=reinterpret_cast<GtkCellRendererText*>(renderer);
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(col,renderer,key_fetch,this,NULL);
	gtk_tree_view_append_column(m_handle,col);


	col=gtk_tree_view_column_new();
	gtk_tree_view_column_set_resizable(col,TRUE);
	gtk_tree_view_column_set_sizing(col,GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_title(col,"");
	renderer=gtk_cell_renderer_text_new();
	m_val_renderer=reinterpret_cast<GtkCellRendererText*>(renderer);
	if(!descriptor.value_const)
		{
		g_object_set(renderer,"editable",TRUE, NULL);
		g_signal_connect(renderer,"edited",G_CALLBACK(value_edited),this);
		}
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(col,renderer,value_fetch,this, NULL);
	gtk_tree_view_append_column(m_handle,col);

	auto key_col=gtk_tree_view_get_column(m_handle,0);
	gtk_tree_view_column_set_title(key_col,descriptor.key);

	auto val_col=gtk_tree_view_get_column(m_handle,1);
	gtk_tree_view_column_set_title(val_col,descriptor.value);

	auto select=gtk_tree_view_get_selection(m_handle);
	if(descriptor.value_const)
		{
	//For now
		gtk_tree_selection_set_mode(select,GTK_SELECTION_NONE); 
		}
	else
		{gtk_tree_selection_set_mode(select,GTK_SELECTION_SINGLE);}

	auto scroll=gtk_scrolled_window_new(NULL,NULL);
	g_object_ref_sink(tree);
	gtk_container_add(GTK_CONTAINER(scroll),tree);
	cnt.add(scroll);
	}

MapViewBase::Impl::~Impl()
	{
	m_cb_user_data=nullptr;
	m_cb_obj=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_list);
	}

void MapViewBase::Impl::clear()
	{gtk_list_store_clear(m_list);}

void MapViewBase::Impl::recordAppend(const void* key,const void* value)
	{
	GtkTreeIter i;
	gtk_list_store_append(m_list,&i);
	gtk_list_store_set(m_list,&i,0,key,1,value,-1);
	}

void MapViewBase::Impl::valueAlignment(float x)
	{
	g_object_set(m_val_renderer, "xalign", x, NULL);
	}

void MapViewBase::Impl::keyAlignment(float x)
	{
	g_object_set(m_key_renderer, "xalign", x, NULL);
	}

void MapViewBase::Impl::key_fetch(GtkTreeViewColumn* col
	,GtkCellRenderer* renderer
	,GtkTreeModel* model
	,GtkTreeIter* iter
	,gpointer user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	uintptr_t ptr=0;
	gtk_tree_model_get(GTK_TREE_MODEL(self->m_list),iter,0,&ptr,-1);
	auto str=self->m_string_from_key(self->m_cb_user_data,reinterpret_cast<const void*>(ptr));
	g_object_set(renderer, "text", str.c_str(), NULL);
	}

void MapViewBase::Impl::value_fetch(GtkTreeViewColumn* col
	,GtkCellRenderer* renderer
	,GtkTreeModel* model
	,GtkTreeIter* iter
	,gpointer user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	uintptr_t ptr=0;
	gtk_tree_model_get(GTK_TREE_MODEL(self->m_list),iter,1,&ptr,-1);
	auto str=self->m_string_from_value(self->m_cb_user_data,reinterpret_cast<const void*>(ptr));
	g_object_set(renderer, "text", str.c_str(), NULL);
	}

gboolean MapViewBase::Impl::selection_changed(GtkTreeSelection *selection, gpointer data)
	{
	return FALSE;
	}

void MapViewBase::Impl::value_edited(GtkCellRendererText* cell,char* path_string
	,char* new_text,gpointer user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	if(self->m_cb_obj!=nullptr)
		{
		auto model=gtk_tree_view_get_model(self->m_handle);
		GtkTreeIter i;
		gtk_tree_model_get_iter_from_string(model,&i,path_string);
		uintptr_t ptr=0;
		gtk_tree_model_get(GTK_TREE_MODEL(self->m_list),&i,1,&ptr,-1);
		self->m_value_update(self->r_owner,self->m_cb_obj
			,reinterpret_cast<void*>(ptr),new_text);
		}
	}
