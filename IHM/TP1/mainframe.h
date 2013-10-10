
#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wx.h"
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include "dialogs.h"

enum {MENU_NEW,MENU_OPEN,MENU_VERSION,MENU_SAVE,MENU_QUIT,MENU_WIDTHLINE,MENU_COLOR,MENU_TRIANGLE,MENU_TOOLBAR,TOOLBAR_TOOLS,MENU_CHECK};

class CMainFrame: public wxFrame {
public:
	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

public:
	void CreateMyToolbar();

private:
	wxToolBar *m_toolbar;
	void OnMENU_NEW(wxCommandEvent& event);
	void OnMENU_OPEN(wxCommandEvent& event);
	void OnMENU_SAVE(wxCommandEvent& event);
	void OnMENU_QUIT(wxCommandEvent& event);
	void OnMENU_CHECK(wxCommandEvent& event);
	void OnMENU_VERSION(wxCommandEvent& event);
	void OnMENU_WIDTHLINE(wxCommandEvent& event);
	void OnMENU_COLOR(wxCommandEvent& event);
	void OnMENU_TRIANGLE(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();


}; //MyFrame

#endif
