
#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wx.h"
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <wx/filedlg.h>
#include "dialogs.h"
#include "triangle.h"

enum {MENU_NEW,MENU_OPEN,MENU_VERSION,MENU_SAVE,MENU_QUIT,MENU_WIDTHLINE,MENU_COLOR,MENU_TRIANGLE,MENU_TOOLBAR,TOOLBAR_TOOLS,MENU_CHECK};

class CMainFrame: public wxFrame {
public:
  int num_tri=0;
  Triangle tab_tri[5];

	int epaisseurTraitCourant=1;
	wxColour couleurCourante= new wxCoulour(wxT("green"));
	bool is_drawing=false;
	
	int get_width();
	wxColour get_color();
	bool get_drawing_active();
	
	void set_width(int width);
	void set_color(String color);
	void set_drawing(bool b);

	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
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
