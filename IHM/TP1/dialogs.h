#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "wx/wx.h"
#include "wx/dialog.h"
#include "wx/slider.h"
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

class WidthLineDialog: public wxDialog{
	
	public :
 		WidthLineDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_WIDTHLINE=10001,ID_SLIDER_WIDTH=10000};
	private :
		
		
	DECLARE_EVENT_TABLE();	
};

class ColorDialog: public wxDialog{
	
	public :
 		ColorDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT=10000};
	private :
		
		
	DECLARE_EVENT_TABLE();	
};

class TriangleDialog: public wxDialog{
	
	public :
 		VersionDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT=10000};
	private :
		
		
	DECLARE_EVENT_TABLE();	
};

class ProprietyDialog: public wxDialog{
	
	public :
 		VersionDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT=10000};
	private :
		
		
	DECLARE_EVENT_TABLE();	
};


#endif


#endif


#endif


#endif


#endif
