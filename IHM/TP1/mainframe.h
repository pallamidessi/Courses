
#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wx.h"
#include <wx/toolbar.h>
#include <wx/tbarbase.h>

enum {MENU_NEW,MENU_OPEN,MENU_VERSION,MENU_SAVE,MENU_QUIT,MENU_WIDTHLINE,MENU_COLOR,MENU_TRIANGLE,MENU_TOOLBAR,TOOLBAR_TOOLS,MENU_CHECK};

class CMainFrame: public wxFrame {
public:
	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

public:
	void CreateMyToolbar();

private:
	wxToolBar *m_toolbar;
private:


	DECLARE_EVENT_TABLE();


}; //MyFrame

#endif
