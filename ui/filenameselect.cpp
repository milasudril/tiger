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
//@	 {"targets":[{"name":"filenameselect.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "filenameselect.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

bool Tiger::filenameSelect(Container& cnt,std::string& filename_in,FileselectMode mode
	,filtercb cb,void* cb_obj,const char* filter_name)
	{
	auto action=mode==FileselectMode::OPEN?
		 GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE;

	const char* text_ok=mode==FileselectMode::OPEN? "Open" : "Save";
	const char* text_title=mode==FileselectMode::OPEN? "Select file to open" : "Choose filename";

	auto parent=cnt.toplevel();
	auto dlg=gtk_file_chooser_dialog_new(text_title,GTK_WINDOW(parent)
		,action,"Cancel",GTK_RESPONSE_CANCEL,text_ok,GTK_RESPONSE_ACCEPT,NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dlg),mode==FileselectMode::SAVE);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dlg),filename_in.c_str());
	auto filter=gtk_file_filter_new();
	gtk_file_filter_set_name(filter,filter_name);
	std::pair<filtercb,void*> cb_obj_gtk{cb,cb_obj};
	auto cb_gtk=[](const GtkFileFilterInfo* filter_info,gpointer data)
		{
		auto obj=reinterpret_cast<std::pair<filtercb,void*>*>(data);
		if(obj->first(obj->second,filter_info->filename))
			{return 1;}
		return 0;
		};

	gtk_file_filter_add_custom(filter,GTK_FILE_FILTER_FILENAME,cb_gtk,&cb_obj_gtk,nullptr);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dlg),filter);

	if(gtk_dialog_run(GTK_DIALOG(dlg))==GTK_RESPONSE_ACCEPT)
		{
		auto filename_out=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
		try
			{filename_in=filename_out;}
		catch(...)
			{
			gtk_widget_destroy(dlg);
			g_free(filename_out);
			throw;
			}
		gtk_widget_destroy(dlg);
		g_free(filename_out);
		return 1;
		}
	gtk_widget_destroy(dlg);
	return 0;
	}
