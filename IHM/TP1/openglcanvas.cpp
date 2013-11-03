#include "openglcanvas.h"
BEGIN_EVENT_TABLE(OpenGLCanvas,wxGLCanvas)
  EVT_PAINT(OpenGLCanvas::OnPaint)
  EVT_SIZE(OpenGLCanvas::OnSize)
  EVT_ERASE_BACKGROUND(OpenGLCanvas::OnEraseBackground)
  EVT_MOTION(OpenGLCanvas::OnMouseMove)
  EVT_LEFT_DOWN(OpenGLCanvas::OnLeftDown)
  EVT_LEFT_UP(OpenGLCanvas::OnLeftUp)
  EVT_RIGHT_DOWN(OpenGLCanvas::OnRightDown)
	EVT_MENU(POPUP_PROP_TRIANGLE,OpenGLCanvas::OnContextPptes)
	EVT_MENU(POPUP_DEL_TRIANGLE,OpenGLCanvas::OnContextSuppr)
END_EVENT_TABLE()

OpenGLCanvas::OpenGLCanvas(wxWindow *parent, wxWindowID id,
      const wxPoint& pos, const wxSize& size,
      long style, const wxString& name):
    wxGLCanvas(parent, id, pos, size, style, name){
      
	is_first_point=true;
	is_second_point=false;
	is_third_point=false;

	drawn=new Triangle();
	selected_tri=-1;
}

OpenGLCanvas::~OpenGLCanvas(void)
{
}

void OpenGLCanvas::OnPaint( wxPaintEvent& event ){
     wxPaintDC dc(this);

         SetCurrent();
         Draw();
         SwapBuffers();
}

void OpenGLCanvas::OnSize( wxSizeEvent& event ){
  wxGLCanvas::OnSize(event);
  int w, h;
  GetClientSize(&w, &h);
  glViewport(0, 0, (GLint) w, (GLint) h);
}

void OpenGLCanvas::OnEraseBackground( wxEraseEvent& event ){}

void OpenGLCanvas::Draw(){
  int w, h;
	int i;
  int num_triangle;
  Triangle current;
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  GetClientSize(&w, &h);
  glOrtho(-w/2., w/2., -h/2., h/2., -1., 3.);


  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glClearColor( .3f, .4f, .6f, 1 );
  glClear( GL_COLOR_BUFFER_BIT);
  
  num_triangle=((CMainFrame*)GetParent())->get_num_tri();
  
  for (i = 0; i < num_triangle; i++) {
    current=((CMainFrame*)GetParent())->get_triangle(i);
    
    /*Triangle background*/
    glColor3d(current.colour.Red(),current.colour.Green(),current.colour.Blue());
    glBegin(GL_TRIANGLES);
    glVertex2d(current.p1.x,current.p1.y);
    glVertex2d(current.p2.x,current.p2.y);
    glVertex2d(current.p3.x,current.p3.y);
    glEnd();
    
    /*Triangle outlines*/
    glColor3d(0,0,0);
    glLineWidth(current.thickness);
    glBegin(GL_LINE_LOOP);
    glVertex2d(current.p1.x,current.p1.y);
    glVertex2d(current.p2.x,current.p2.y);
    

    glVertex2d(current.p2.x,current.p2.y);
    glVertex2d(current.p3.x,current.p3.y);

    glVertex2d(current.p3.x,current.p3.y);
    glVertex2d(current.p1.x,current.p1.y);
		glEnd();
  }
	/*Dessin du triangle en cours*/
	if (is_first_point==false && is_second_point==true) {
    int thickness =((CMainFrame*)GetParent())->get_width();
		glColor3d(0,0,0);
		glLineWidth(thickness);
		glBegin(GL_LINES);
		glVertex2d(drawn->p1.x,drawn->p1.y);
		glVertex2d(cursor.x,cursor.y);
		glEnd();
	}else if (is_second_point==false && is_third_point==true) {
    wxColour* cur_colour=((CMainFrame*)GetParent())->get_color();
		/*Triangle background*/
    glColor3d(cur_colour->Red(),cur_colour->Green(),cur_colour->Blue());
    glBegin(GL_TRIANGLES);
		glVertex2d(drawn->p1.x,drawn->p1.y);
		glVertex2d(drawn->p2.x,drawn->p2.y);
		glVertex2d(cursor.x,cursor.y);
    glEnd();
    
    /*Triangle outlines*/
    glColor3d(0,0,0);
    glLineWidth(((CMainFrame*)GetParent())->get_width());

    glBegin(GL_LINE_LOOP);
		glVertex2d(drawn->p1.x,drawn->p1.y);
		glVertex2d(drawn->p2.x,drawn->p2.y);
    
		glVertex2d(drawn->p2.x,drawn->p2.y);
		glVertex2d(cursor.x,cursor.y);

		glVertex2d(cursor.x,cursor.y);
		glVertex2d(drawn->p1.x,drawn->p1.y);
		glEnd();
	}
}


void OpenGLCanvas::OnMouseMove(wxMouseEvent& event){

  int w, h;
  GetClientSize(&w, &h);

	cursor.x=event.GetX();
	cursor.y=event.GetY();

	if(cursor.x<w/2.)
		cursor.x=-(w/2.-cursor.x);
	else
		cursor.x=(cursor.x-w/2.);
	if(cursor.y>h/2.)
		cursor.y=-(cursor.y-h/2.);
	else
		cursor.y=(h/2.-cursor.y);

	
  wxPaintDC dc(this);

         SetCurrent();
         Draw();
         SwapBuffers();
}

void OpenGLCanvas::OnLeftDown(wxMouseEvent& event){
  int w, h;
  GetClientSize(&w, &h);

	if (((CMainFrame*)GetParent())->get_num_tri()==5) {
		return;
	}
	
	if(((CMainFrame*)GetParent())->is_drawing==false){
		return;
	}

	if(is_first_point==true){
		drawn->p1.x=event.GetX();
		drawn->p1.y=event.GetY();
		
		if(drawn->p1.x<w/2.)
			drawn->p1.x=-(w/2.-drawn->p1.x);
		else
			drawn->p1.x=(drawn->p1.x-w/2.);
		if(drawn->p1.y>h/2.)
			drawn->p1.y=-(drawn->p1.y-h/2.);
		else
			drawn->p1.y=(h/2.-drawn->p1.y);
		
		is_first_point=false;
		is_second_point=true;
	}
	else if(is_second_point==true) {
		drawn->p2.x=event.GetX();
		drawn->p2.y=event.GetY();
		
		if(drawn->p2.x<w/2.)
			drawn->p2.x=-(w/2.-drawn->p2.x);
		else
			drawn->p2.x=(drawn->p2.x-w/2.);
		if(drawn->p2.y>h/2.)
			drawn->p2.y=-(drawn->p2.y-h/2.);
		else
			drawn->p2.y=(h/2.-drawn->p2.y);
		
		is_second_point=false;
		is_third_point=true;
	}
	else if (is_third_point==true) {
		drawn->p3.x=event.GetX();
		drawn->p3.y=event.GetY();
		
		if(drawn->p3.x<w/2.)
			drawn->p3.x=-(w/2.-drawn->p3.x);
		else
			drawn->p3.x=(drawn->p3.x-w/2.);
		if(drawn->p3.y>h/2.)
			drawn->p3.y=-(drawn->p3.y-h/2.);
		else
			drawn->p3.y=(h/2.-drawn->p3.y);
		
		((CMainFrame*)GetParent())->copy_triangle_to_tab(*drawn);

		is_third_point=false;
		is_first_point=true;
    ((CMainFrame*)GetParent())->GetMenuBar()->Enable(MENU_TRIANGLE,true);
	}
}
void OpenGLCanvas::OnLeftUp(wxMouseEvent& event){}
void OpenGLCanvas::OnRightDown(wxMouseEvent& event){
	int res=EstDansTriangle(event.GetX(),event.GetY());
	wxMenu MainMenu;
	
	if(res!=-1){
		wxMenuItem* ProprietyTriangle=new wxMenuItem(&MainMenu,POPUP_PROP_TRIANGLE,wxT("Propriete de ce triangle"));
		wxMenuItem* DeleteTriangle=new wxMenuItem(&MainMenu,POPUP_DEL_TRIANGLE,wxT("Supprimer ce triangle"));
			
		MainMenu.Append(ProprietyTriangle);
		MainMenu.Append(DeleteTriangle);
		
		PopupMenu( &MainMenu, event.GetX(), event.GetY());
	
	}		
	else{

		wxMenu* File=new wxMenu;
		wxMenu* Management=new wxMenu;
		wxMenu* CurrentValue=new wxMenu;

		File->Append(MENU_OPEN,wxT("Ouvrir fichier"));
		File->Append(MENU_SAVE,wxT("Sauvegarder fichier"));

		
		Management->Append(MENU_TRIANGLE,wxT("Gestion des triangles"));

		CurrentValue->Append(MENU_COLOR,wxT("Couleurs Courantes"));
		CurrentValue->Append(MENU_WIDTHLINE,wxT("Epaisseur Courante"));

		MainMenu.Append(POPUP_FILE,wxT("Fichiers"),File);
		MainMenu.Append(POPUP_MANAGEMENT,wxT("Gestion"),Management);
		MainMenu.Append(POPUP_CURRENT_VALUES,wxT("Valeurs courantes"),CurrentValue);
		
		PopupMenu( &MainMenu, event.GetX(), event.GetY());
	}
}

int OpenGLCanvas::EstDansTriangle(int x, int y){
  	
		int w,h,i;
  	GetClientSize(&w, &h);
		
		if(x<w/2.)
			x=-(w/2.-x);
		else
			x=(x-w/2.);

		if(y>h/2.)
			y=-(y-h/2.);
		else
			y=(h/2.-y);

	for (i = 0; i < ((CMainFrame*)GetParent())->get_num_tri(); i++) {
		if(((CMainFrame*)GetParent())->get_triangle(i).IsPointInTriangle(x,y)){
			selected_tri=i;
			return i;
		}
	}
	return -1;
}

void OpenGLCanvas::OnContextPptes(wxCommandEvent& event){
	ProprietyDialog pdlg(this,-1,wxT("Propriété"));
	pdlg.isFromPopup=true;	
	Triangle tri=((CMainFrame*)GetParent())->get_triangle(selected_tri);
	wxString name;
	wxColour cur_colour=tri.colour;
	
	if(cur_colour.Red())
		pdlg.radio->SetSelection(0);
	else if(cur_colour.Green()){
		pdlg.radio->SetSelection(1);
	}
	else if(cur_colour.Blue()){
		pdlg.radio->SetSelection(2);
	}
	
	name.Printf(wxT("Triangle %d"),selected_tri);

	pdlg.textCtrl->ChangeValue(name);
	pdlg.spin->SetValue((int)tri.thickness);

	pdlg.ShowModal();
}

void OpenGLCanvas::OnContextSuppr(wxCommandEvent& event){
	int i;

	if (selected_tri==((CMainFrame*)GetParent())->num_tri-1) {
		((CMainFrame*)GetParent())->num_tri-=1;
	}
	else{
		for (i = selected_tri; i < ((CMainFrame*)GetParent())->num_tri; i++) {
			((CMainFrame*)GetParent())->tab_tri[i]=((CMainFrame*)GetParent())->tab_tri[i+1];
		}
		((CMainFrame*)GetParent())->num_tri-=1;
	}

	if(((CMainFrame*)GetParent())->num_tri==0){
    ((CMainFrame*)GetParent())->GetMenuBar()->Enable(MENU_TRIANGLE,false);
	}
	
	
	((CMainFrame*)GetParent())->Update();

}
