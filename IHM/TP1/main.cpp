#include <iostream>
#include "wx/wx.h" 
#include "mainframe.h"
#include "dialogs.h"
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/tbarbase.h>


class MyApp: public wxApp 
{
	virtual bool OnInit();
	CMainFrame *m_MainFrame;
};


IMPLEMENT_APP(MyApp)


bool MyApp::OnInit() 
{
	m_MainFrame = new CMainFrame( wxString("Fenêtre", wxConvUTF8), wxPoint(50,50), wxSize(450,340) );

	m_MainFrame->CreateMyToolbar();
	m_MainFrame->Show(TRUE);
	
	wxMenuBar *menu_bar = new wxMenuBar;

	wxMenu *file_menu = new wxMenu;
	wxMenu *display_menu = new wxMenu;
	wxMenu *option_menu = new wxMenu;
	wxMenu *help_menu = new wxMenu;

	menu_bar->Append(file_menu, wxT("Fichier"));
	menu_bar->Append(display_menu, wxT("Affichage"));
	menu_bar->Append(option_menu, wxT("Option"));
	menu_bar->Append(help_menu, wxT("Fichier"));

	file_menu->Append(MENU_NEW,wxT("Nouveau\tCtrl-N"));
	file_menu->AppendSeparator();
	file_menu->Append(MENU_OPEN,wxT("Ouvrir\tCtrl-O"));
	file_menu->Append(MENU_SAVE,wxT("Sauvegarder\tCtrl-S"));
	file_menu->AppendSeparator();
	file_menu->Append(MENU_QUIT,wxT("Quitter\tCtrl-X"));
	
	option_menu->Append(MENU_WIDTHLINE,wxT("Epaisseur trait"));
	option_menu->Append(MENU_COLOR,wxT("Couleur"));
	option_menu->Append(MENU_TRIANGLE,wxT("Gestion Triangle"));

	help_menu->Append(MENU_VERSION,wxT("Version"));
	
	display_menu->AppendCheckItem(MENU_TOOLBAR,wxT("Afficher la barre d'outil"));
	display_menu->Check(MENU_TOOLBAR,TRUE);


	menu_bar->Enable(MENU_TRIANGLE,false);
	
	m_MainFrame->SetMenuBar(menu_bar);
	
	VersionDialog vdlg(p_MainFrame,-1,wxT("Version"));
	vdlg.ShowModal();
	return TRUE;
} 


