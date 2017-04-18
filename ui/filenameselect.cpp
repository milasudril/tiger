//@	 {"targets":[{"name":"filenameselect.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "filenameselect.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

bool Tiger::filenameSelect(Container& cnt,std::string& filename_in
	,FileselectMode mode)
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
