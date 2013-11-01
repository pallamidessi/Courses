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
	num_tri=0;
	epaisseurTraitCourant=3;
	is_drawing=false;
	couleurCourante=new wxColour(wxT("RGB(0,255,0)"));
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
  int r,g,b;
	int i;

  wxFileDialog fd(this,wxT("Fichier à ouvrir :"),wxT("."),wxT(""),wxT("*.tri"),wxOPEN);

  if(fd.ShowModal()==wxID_OK){
    std::ifstream fo(fd.GetPath().fn_str(), std::ios::in);
    
    if (!fo)
    {
      wxString errormsg, caption;
      errormsg.Printf(wxT("Unable to open file "));
      errormsg.Append(fd.GetPath());
      caption.Printf(wxT("Erreur"));
      wxMessageDialog msg(this, errormsg, caption, wxOK | wxCENTRE | wxICON_ERROR);
      msg.ShowModal();
      return ;
    }

    fo >> num_tri;
    if (num_tri>0) {
      //activer gestion de triangle
    }
    for (i = 0; i < num_tri; i++) {
        fo>>tab_tri[i].p1.x;
        fo>>tab_tri[i].p1.y;
        
        fo>>tab_tri[i].p2.x;
        fo>>tab_tri[i].p2.y;
        
        fo>>tab_tri[i].p3.x;
        fo>>tab_tri[i].p3.y;

        fo>>r;
        fo>>g;
        fo>>b;
        tab_tri[i].colour.Set((unsigned char)r,(unsigned char)g,(unsigned char)b);
        fo>>tab_tri[i].thickness;
    }
  }

}

void CMainFrame::OnMENU_SAVE(wxCommandEvent& event){
  int i;

	wxFileDialog fd(this,wxT("Enregistrer vers :"),wxT("."),wxT(""),wxT("*.tri"),wxSAVE);
  std::ofstream fs((char*)fd.GetPath().c_str(), std::ios::out);

  fs << num_tri<<std::endl;

  if(fd.ShowModal()==wxID_OK){
    for (i = 0; i < num_tri; i++) {
      fs<<tab_tri[i].p1.x;
      fs<<tab_tri[i].p1.y;
      
      fs<<tab_tri[i].p2.x;
      fs<<tab_tri[i].p2.y;
      
      fs<<tab_tri[i].p3.x;
      fs<<tab_tri[i].p3.y;
      fs<<std::endl;
      fs<<tab_tri[i].colour.Red();
      fs<<tab_tri[i].colour.Green();
      fs<<tab_tri[i].colour.Blue();
      fs<<std::endl;
      fs<<tab_tri[i].thickness;
      fs<<std::endl;
      fs<<std::endl;
    } 
  }
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
  WidthLineDialog wdlg(this,-1,wxT("Epaisseur"));
  wdlg.ShowModal();
}
void CMainFrame::OnMENU_TRIANGLE(wxCommandEvent& event){
  TriangleDialog tdlg(this,-1,wxT("Gestion des triangles"));
  tdlg.ShowModal();
}
void CMainFrame::OnMENU_COLOR(wxCommandEvent& event){
  ColorDialog cdlg(this,-1,wxT("Couleur"));
  cdlg.ShowModal();
}
void CMainFrame::OnMENU_VERSION(wxCommandEvent& event){

  VersionDialog vdlg(this,-1,wxT("Version"));

  vdlg.ShowModal();
}

int CMainFrame::get_width(){
  return epaisseurTraitCourant;	
}

wxColour* CMainFrame::get_color(){
  return couleurCourante;
}

bool CMainFrame::get_drawing_active(){
  return is_drawing;
}

void CMainFrame::set_width(int width){
  epaisseurTraitCourant=width;
}

void CMainFrame::set_color(wxString color){
  couleurCourante->Set(color);
}

void CMainFrame::set_drawing(bool mode){
  is_drawing=mode;
}

int CMainFrame::get_num_tri(){
   return num_tri;
}

Triangle CMainFrame::get_triangle(int index){
 // if (index>num_tri) {
 //   return NULL;
 // }
 // else {
    return tab_tri[index];  
 // }
}
