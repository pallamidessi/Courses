#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wx.h"
#include <wx/toolbar.h>
#include <wx/tbarbase.h>

class VersionDialog: public wxDialog{
	BEGIN_EVENT_TABLE(VersionDialog, wxDialog);
	END_EVENT_TABLE ();
	
	public :
 		VersionDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT=10000};
	private :
		DECLARE_EVENT_TABLE();
		
};

#endif
