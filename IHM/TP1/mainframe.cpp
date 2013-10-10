#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/accel.h>

#include "mainframe.h"



BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_MENU(MENU_QUIT,CMainFrame::OnMENU_QUIT)
	EVT_MENU(MENU_OPEN,CMainFrame::OnMENU_OPEN)
	EVT_MENU(MENU_NEW,CMainFrame::OnMENU_NEW)
	EVT_MENU(MENU_SAVE,CMainFrame::OnMENU_SAVE)
	EVT_MENU(MENU_TOOLBAR,CMainFrame::OnMENU_CHECK)
	EVT_MENU(MENU_VERSION,CMainFrame::OnMENU_VERSION)
	EVT_MENU(MENU_WIDTHLINE,CMainFrame::OnMENU_WIDTHLINE)
	EVT_MENU(MENU_COLOR,CMainFrame::OnMENU_COLOR)
	EVT_MENU(MENU_TRIANGLE,CMainFrame::OnMENU_TRIANGLE)
END_EVENT_TABLE()



CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size) 
{
} //constructor

void CMainFrame::CreateMyToolbar(){
	
	m_toolbar=CreateToolBar( wxTB_HORIZONTAL,TOOLBAR_TOOLS);
	
	wxBitmap toolBarBitmaps[4];
	toolBarBitmaps[0] = wxBitmap(wxT("new.bmp"),wxBITMAP_TYPE_BMP);
	toolBarBitmaps[1] = wxBitmap(wxT("open.bmp"),wxBITMAP_TYPE_BMP);
	toolBarBitmaps[2] = wxBitmap(wxT("save.bmp"),wxBITMAP_TYPE_BMP);
	toolBarBitmaps[3] = wxBitmap(wxT("draw.bmp"),wxBITMAP_TYPE_BMP);
	
	m_toolbar->SetToolBitmapSize(wxSize(toolBarBitmaps[0].GetWidth(),toolBarBitmaps[0].GetHeight()));

	m_toolbar->AddTool(MENU_NEW, wxT("Nouveau"), toolBarBitmaps[0]);
	m_toolbar->AddTool(MENU_OPEN, wxT("Ouvrir"), toolBarBitmaps[1]);
	m_toolbar->AddTool(MENU_SAVE, wxT("Sauvegarder"), toolBarBitmaps[2]);
	m_toolbar->AddSeparator();
	m_toolbar->AddCheckTool(MENU_CHECK, wxT("Test"), toolBarBitmaps[3]);

	m_toolbar->Realize();
	SetToolBar(m_toolbar);
}

void CMainFrame::OnMENU_NEW(wxCommandEvent& event){
}

void CMainFrame::OnMENU_OPEN(wxCommandEvent& event){
}

void CMainFrame::OnMENU_SAVE(wxCommandEvent& event){
}

void CMainFrame::OnMENU_QUIT(wxCommandEvent& event){
	Close(TRUE);
}

void CMainFrame::OnMENU_CHECK(wxCommandEvent& event){
	wxToolBar* tool=NULL;

	if ((tool=GetToolBar())!=NULL) {
		if (tool->IsShown()==true) 
			tool->Show(false);
		else
			tool->Show(true);
		
	}
}

void CMainFrame::OnMENU_WIDTHLINE(wxCommandEvent& event){
}
void CMainFrame::OnMENU_TRIANGLE(wxCommandEvent& event){
}
void CMainFrame::OnMENU_COLOR(wxCommandEvent& event){
}
void CMainFrame::OnMENU_VERSION(wxCommandEvent& event){

	VersionDialog::VersionDialog vdlg(this,-1,wxT("Version"));
	
	vdlg.ShowModal();
}
