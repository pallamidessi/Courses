#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "wx/wx.h"
#include "wx/dialog.h"
#include "mainframe.h"
#include <wx/toolbar.h>
#include <wx/tbarbase.h>

class VersionDialog: public wxDialog{
	
	public :
 		VersionDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT=10000};
	private :
		
		
	DECLARE_EVENT_TABLE();	
};

#endif
