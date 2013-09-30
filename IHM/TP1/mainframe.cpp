#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/accel.h>

#include "mainframe.h"



BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
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
