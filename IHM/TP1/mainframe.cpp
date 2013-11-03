#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/accel.h>
#include "mainframe.h"


/*Function table for CMainFrame (Menu and toolbar)*/
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
	EVT_MENU(MENU_CHECK,CMainFrame::OnMENU_DRAWINGMODE)
END_EVENT_TABLE()


/**
* /brief		Constructor of CMainFrame
*	/details	Use wxFrame constructor
*
*	@param	title title of the CMainFrame instance
*	@param	pos position for the wxWidget environnement
*	@param	size size for the wxWidget environnemnt
**/
  CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size) 
{
	num_tri=0;
	epaisseurTraitCourant=3;
	is_drawing=false;
	couleurCourante=new wxColour(wxT("RGB(0,255,0)"));
} //constructor

/**
* /brief		Create and attach a toolbar to the CMainFrame instance
*	/details	Use external image for the buttons
*
**/
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

/**
* /brief		Function triggered when the drawing button is pressed
*	/details	Enable or disable drawing depending of the button state
*
*	@param	event The event which triggered the button
**/

void CMainFrame::OnMENU_DRAWINGMODE(wxCommandFunction& event){
	if(is_drawing==false)
		is_drawing=true;
	else
		is_drawing=false;
}
/**
* /brief		Function triggered when the "new" item menu or toolbar icon is 
* 					pressed
*	/details	Destroy all triangle contained in the triangle array
*
*	@param	event The event sent when the item menu or the icon was pressed
**/
void CMainFrame::OnMENU_NEW(wxCommandFunction& event){
	num_tri=0;
  GetMenuBar()->Enable(MENU_TRIANGLE,false);
}

/**
* /brief		Function triggered when the "open" item menu or toolbar icon is 
* 					pressed
*	/details	Load all triangle contained in the specified .tri file in the triangle array
*
*	@param	event The event sent when the item menu or the icon was pressed
**/

void CMainFrame::OnMENU_OPEN(wxCommandFunction& event){
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
      GetMenuBar()->Enable(MENU_TRIANGLE,true);
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
/**
* /brief		Function triggered when the "save" item menu or toolbar icon is 
* 					pressed
*	/details	Save (and overwrite) all triangle contained in the triangle array to the specified .tri file
*
*	@param	event The event sent when the item menu or the icon was pressed
**/


void CMainFrame::OnMENU_SAVE(wxCommandFunction& event){
  int i;

	wxFileDialog fd(this,wxT("Enregistrer vers :"),wxT("."),wxT(""),wxT("*.tri"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  std::ofstream fs((char*)fd.GetPath().c_str(), std::ios::out);

	if (!fs)
	{
		wxString errormsg, caption;
		errormsg.Printf(wxT("Unable to save file "));
		errormsg.Append(fd.GetPath());
		caption.Printf(wxT("Erreur"));
		wxMessageDialog msg(this, errormsg, caption, wxOK | wxCENTRE | wxICON_ERROR);
		msg.ShowModal();
		return ;
	}

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

/**
* /brief		Function triggered when the "Quit" item menu is clicked on
*	/details	Quit the application 
*
*	@param	event The event sent when the item menu was pressed
**/

void CMainFrame::OnMENU_QUIT(wxCommandFunction& event){
  Close(TRUE);
}

/**
* /brief		Function triggered when the "open" item menu pressed
*	/details	Show or hide the toolbar depending on the check menu item
*
*	@param	event The event sent when the item menu was pressed
**/

void CMainFrame::OnMENU_CHECK(wxCommandFunction& event){
  wxToolBar* tool=NULL;

  if ((tool=GetToolBar())!=NULL) {
    if (tool->IsShown()==true) 
      tool->Show(false);
    else
      tool->Show(true);
	
  }
}

/**
* /brief		Function triggered when the "Epaisseur" item menu is clicked on
*	/details	Create and show a WidthLineDialog box
*
*	@param	event The event sent when the item menu was pressed
**/
void CMainFrame::OnMENU_WIDTHLINE(wxCommandFunction& event){
  WidthLineDialog wdlg(this,-1,wxT("Epaisseur"));
  wdlg.ShowModal();
}
/**
* /brief		Function triggered when the "Gestion Triangle" item menu is clicked on
*	/details	Create and show a TriangleDialog box
*
*	@param	event The event sent when the item menu was pressed
**/
void CMainFrame::OnMENU_TRIANGLE(wxCommandFunction& event){
  TriangleDialog tdlg(this,-1,wxT("Gestion des triangles"));
  tdlg.ShowModal();
}
/**
* /brief		Function triggered when the "Couleur" item menu is clicked on
*	/details	Create and show a ColorDialog box
*
*	@param	event The event sent when the item menu was pressed
**/
void CMainFrame::OnMENU_COLOR(wxCommandFunction& event){
  ColorDialog cdlg(this,-1,wxT("Couleur"));
  cdlg.ShowModal();
}
/**
* /brief		Function triggered when the "Version" item menu is clicked on
*	/details	Create and show a VersionDialog box
*
*	@param	event The event sent when the item menu was pressed
**/
void CMainFrame::OnMENU_VERSION(wxCommandFunction& event){

  VersionDialog vdlg(this,-1,wxT("Version"));
  vdlg.ShowModal();
}
/**
* /brief		Get the current line width 
*	/details	Getter for CMainFrame member epaisseurTraitCourant
*
* @return	epaisseurTraitCourant The current line width (int)
**/
int CMainFrame::get_width(){
  return epaisseurTraitCourant;	
}

/**
* /brief		Get the current colour 
*	/details	Getter for CMainFrame member couleurCourante;
*
* @return	couleurCourante The current colour (wxColour*)
**/
wxColour* CMainFrame::get_color(){
  return couleurCourante;
}

/**
* /brief		To known is the drawing mode is enabled or not
*	/details	Getter for CMainFrame member is_drawing
*
* @return	is_drawing Return True if drawing mode is enble, false otherwise
**/
bool CMainFrame::get_drawing_active(){
  return is_drawing;
}

/**
* /brief		Set the line width of the CMainFrame instance
*	/details	Setter for CMainFrame member epaisseurTraitCourant
*
*	@param	width The new line width
**/
void CMainFrame::set_width(int width){
  epaisseurTraitCourant=width;
}

/**
* /brief		Set the current coloour of the CMainFrame instance
*	/details	Setter for CMainFrame member couleurCourante using wxC2S_NAME convention
*
*	@param	color wxString designing the new color 
**/
void CMainFrame::set_color(wxString color){
  couleurCourante->Set(color);
}

/**
* /brief		Set the drawing mode of the CMainFrame instance
*	/details	Setter for CMainFrame member is_drawing
*
*	@param	mode The mode to set 
**/
void CMainFrame::set_drawing(bool mode){
  is_drawing=mode;
}

/**
* /brief		Get the number of active triangle in the triangle array
*	/details	Getter for CMainFrame member numTri
*
* @return	numTri Return the number of active triangle
**/
int CMainFrame::get_num_tri(){
   return num_tri;
}

/**
* /brief		Get the triangle given by the index in the Triangle array 
*	/details	Getter for CMainFrame member tabTri
*
* @return	tri The wanted triangle
**/
Triangle CMainFrame::get_triangle(int index){
 // if (index>num_tri) {
 //   return NULL;
 // }
 // else {
    return tab_tri[index];  
 // }
}
/**
* /brief		Copy the given Triangle to the Triangle array
*	/details	The triangle is not inserted if the array is full
*
*	@param	t The Triangle to insert
**/
void CMainFrame::copy_triangle_to_tab(Triangle t){
	if (num_tri!=5) {
			tab_tri[num_tri].p1.x=t.p1.x;
			tab_tri[num_tri].p1.y=t.p1.y;

			tab_tri[num_tri].p2.x=t.p2.x;
			tab_tri[num_tri].p2.y=t.p2.y;

			tab_tri[num_tri].p3.x=t.p3.x;
			tab_tri[num_tri].p3.y=t.p3.y;


			tab_tri[num_tri].thickness=epaisseurTraitCourant;
			tab_tri[num_tri].colour.Set(couleurCourante->Red(),couleurCourante->Green(),couleurCourante->Blue());
		}
	
	num_tri++;

}
