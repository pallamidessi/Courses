
#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wx.h"
#include "openglcanvas.h"
#include <cstring> 
#include <fstream> 
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <wx/filedlg.h>
#include "dialogs.h"
#include "triangle.h"

enum {MENU_NEW,MENU_OPEN,MENU_VERSION,MENU_SAVE,MENU_QUIT,MENU_WIDTHLINE,MENU_COLOR,MENU_TRIANGLE,MENU_TOOLBAR,TOOLBAR_TOOLS,MENU_CHECK,ID_CANVAS,POPUP_FILE,POPUP_MANAGEMENT,POPUP_CURRENT_VALUES,POPUP_PROP_TRIANGLE,POPUP_DEL_TRIANGLE};

class OpenGLCanvas;
class CMainFrame: public wxFrame {
public:
  int num_tri;
  Triangle tab_tri[5];

	int epaisseurTraitCourant;
	wxColour* couleurCourante;
	bool is_drawing;
	
	int get_width();
	int get_num_tri();
	wxColour* get_color();
	bool get_drawing_active();
	Triangle get_triangle(int index);
	
	void set_width(int width);
	void set_color(wxString color);
	void set_drawing(bool b);
	void copy_triangle_to_tab(Triangle t);

	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void CreateMyToolbar();
	OpenGLCanvas* drawing_zone;
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
	void OnMENU_DRAWINGMODE(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();


}; //MyFrame

#endif
