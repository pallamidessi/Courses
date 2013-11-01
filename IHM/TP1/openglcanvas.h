#ifndef __OPENGLCANVAS_H
#define __OPENGLCANVAS_H

#include <wx/window.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "triangle.h"
#include "mainframe.h"
class OpenGLCanvas: public wxGLCanvas{

  public:
    OpenGLCanvas(wxWindow *parent, wxWindowID id,const wxPoint& pos, const wxSize& size,
                long style, const wxString& name);

    ~OpenGLCanvas(void);
		
		void OnMouseMove(wxMouseEvent& event);
		void OnLeftDown(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);

  private:
		void Draw();
    void OnPaint( wxPaintEvent& event );
    void OnSize( wxSizeEvent& event );
    void OnEraseBackground( wxEraseEvent& event );

    DECLARE_EVENT_TABLE();
};

#endif
