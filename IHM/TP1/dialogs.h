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
		/**
		* /brief		VersionDialog constructor
		*	/details	Use wxDialog constructor
		*
		*	@param	parent The containing objet pointer
		*	@param	id Id of this instance
		*	@param	title Name of the VersionDialog box
		**/
 		VersionDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT=10000};
	private :
		
		
	DECLARE_EVENT_TABLE();	
};

class WidthLineDialog: public wxDialog{
	
	public :
		/**
		* /brief		WidthLineDialog constructor
		*	/details	Use wxDialog constructor
		*
		*	@param	parent The containing objet pointer
		*	@param	id Id of this instance
		*	@param	title Name of the WidthLineDialog box
		**/
 		WidthLineDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_WIDTHLINE=10001,ID_SLIDER_WIDTH=10002};
	private :
		/**
		* /brief		Function triggered when the slider is moved
		*	/details	Slider not working in linux
		*
		*	@param	event The event sent when the slider is moved
		**/

		void OnWIDGET_SLIDER(wxScrollEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};

class ColorDialog: public wxDialog{
	
	public :
		/**
		* /brief		ColorDialog constructor
		*	/details	Use wxDialog constructor
		*
		*	@param	parent The containing objet pointer
		*	@param	id Id of this instance
		*	@param	title Name of the ColorDialog box
		**/
 		ColorDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_COLOR=10003,ID_RADIO_COLOR=10004};
	private :
		/**
		* /brief		Function triggered when the radio selection is changed 
		*	/details	Change the couleurCourante in the CMainFrame parent using the new
		*						radio selectied color
		*
		*	@param	event The event sent when the radio selection is changed
		**/
		void OnWIDGET_RADIO(wxCommandEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};

class TriangleDialog: public wxDialog{
	
	public :
		/**
		* /brief		TriangleDialog constructor
		*	/details	Use wxDialog constructor
		*
		*	@param	parent The containing objet pointer
		*	@param	id Id of this instance
		*	@param	title Name of the TriangleDialog box
		**/
 		TriangleDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_TRIANGLE=10005,ID_PROP_BUTTON=10006,ID_DEL_BUTTON=10007,ID_LISTBOX_TRIANGLE=10008};
		int selected_triangle;
	private :
		wxRadioBox* radio;
		/**
		* /brief		Function triggered when the "Propriety" button is pressed
		*	/details	Open a ProprietyDialog box, initialized with the listbox
		*						selection
		*
		*	@param	event The event sent when the "Propriety" button is pressed
		**/
		void OnWIDGET_PROPRIETY(wxCommandEvent& event);
		/**
		* /brief		Function triggered when the "Delete" button is pressed
		*	/details	Delete the triangle selected in the listbox from the traingle 
		*						array in the CMainFrame parent
		*
		*	@param	event The event sent when the "Delete" button is pressed
		**/
		void OnWIDGET_DELETE(wxCommandEvent& event);
		/**
		* /brief		Function triggered when the selection changed in the listbox
		*	/details	Changed the value of selected_tri with the index of the selected element
		*						in the listbox
		*
		*	@param	event The event sent when the listbox selection is changed
		**/
		void OnWIDGET_LISTBOX(wxCommandEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};

class ProprietyDialog: public wxDialog{
	
	public :
		bool isFromPopup;
		wxSpinCtrl* spin;
		wxRadioBox* radio;
		wxTextCtrl* textCtrl;
		/**
		* /brief		ProprietyDialog constructor
		*	/details	Use wxDialog constructor
		*
		*	@param	parent The containing objet pointer
		*	@param	id Id of this instance
		*	@param	title Name of the ProprietyDialog box
		**/
 		ProprietyDialog(wxWindow *parent, wxWindowID id,const wxString &title);
		enum{ID_TEXT_PROPRIETY1=10009,ID_TEXT_PROPRIETY2=10010,ID_RADIO_PROPRIETY=10011,ID_TEXTCTRL=10012,ID_SPINCTRL=10013};
	private :
		void OnWIDGET_RADIO(wxCommandEvent& event);
		void OnWIDGET_SPIN(wxSpinEvent& event);
		
		
	DECLARE_EVENT_TABLE();	
};


#endif

