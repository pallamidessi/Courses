#include "openglcanvas.h"
BEGIN_EVENT_TABLE(OpenGLCanvas,wxGLCanvas)
  EVT_PAINT(OpenGLCanvas::OnPaint)
  EVT_SIZE(OpenGLCanvas::OnSize)
  EVT_ERASE_BACKGROUND(OpenGLCanvas::OnEraseBackground)
  EVT_MOTION(OpenGLCanvas::OnMouseMove)
  EVT_LEFT_DOWN(OpenGLCanvas::OnLeftDown)
  EVT_LEFT_UP(OpenGLCanvas::OnLeftUp)
END_EVENT_TABLE()

OpenGLCanvas::OpenGLCanvas(wxWindow *parent, wxWindowID id,
      const wxPoint& pos, const wxSize& size,
      long style, const wxString& name):
    wxGLCanvas(parent, id, pos, size, style, name){
      
        




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
  
  num_triangle=GetParent.get_num_tri();
  
  for (i = 0; i < num_triangle; i++) {
    current=GetParent.get_triangle(i);
    
    /*Triangle background*/
    glColor3d(current.colour.Red(),current.colour.Green(),current.colour.Blue());
    glBegin(GL_TRIANGLES);
    glVertex2d(current.p1.x,current.p1.y);
    glVertex2d(current.p2.x,current.p2.y);
    glVertex2d(current.p3.x,current.p3.y);
    glEnd();
    
    /*Triangle outlines*/
    glColor3d(255,255,255);
    glLineWidth(current.thickness);
    glBegin(GL_LINE_LOOP);
    glVertex2d(current.p1.x,current.p1.y);
    glVertex2d(current.p2.x,current.p2.y);
    

    glVertex2d(current.p2.x,current.p2.y);
    glVertex2d(current.p3.x,current.p3.y);

    glVertex2d(current.p3.x,current.p3.y);
    glVertex2d(current.p1.x,current.p1.y);

  }
}
void OpenGLCanvas::OnMouseMove(wxMouseEvent& event){}
void OpenGLCanvas::OnLeftDown(wxMouseEvent& event){}
void OpenGLCanvas::OnLeftUp(wxMouseEvent& event){}
