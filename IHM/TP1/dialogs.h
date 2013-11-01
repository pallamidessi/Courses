#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "wx/wx.h"
#include "wx/event.h"
#include "wx/dialog.h"
#include "wx/slider.h"
#include "wx/spinctrl.h"
#include "wx/radiobox.h"
#include "wx/radiobut.h"
#include "wx/listbox.h"
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
		enum{ID_TEXT_WIDTHLINE=10001,ID_SLIDER_WIDTH=10002};
	private :
		void OnWIDGET_SLIDER(wxScrollEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};

class ColorDialog: public wxDialog{
	
	public :
 		ColorDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_COLOR=10003,ID_RADIO_COLOR=10004};
	private :
		void OnWIDGET_RADIO(wxCommandEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};

class TriangleDialog: public wxDialog{
	
	public :
 		TriangleDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_TRIANGLE=10005,ID_PROP_BUTTON=10006,ID_DEL_BUTTON=10007,ID_LISTBOX_TRIANGLE=10008};
	private :
		wxRadioBox* radio;
		void OnWIDGET_PROPRIETY(wxCommandEvent& event);
		void OnWIDGET_DELETE(wxCommandEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};

class ProprietyDialog: public wxDialog{
	
	public :
		wxSpinCtrl* spin;
		wxRadioBox* radio;
 		ProprietyDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_PROPRIETY1=10009,ID_TEXT_PROPRIETY2=10010,ID_RADIO_PROPRIETY=10011,ID_TEXTCTRL=10012,ID_SPINCTRL=10013};
	private :
		void OnWIDGET_RADIO(wxCommandEvent& event);
		void OnWIDGET_SPIN(wxSpinEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};


#endif

