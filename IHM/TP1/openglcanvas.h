
class OpenGLCanvas: public wxGLCanvas{

  public:
    OpenGLCanvas::OpenGLCanvas(wxWindow *parent, wxWindowID id,
                                const wxPoint& pos, const wxSize& size,
                                long style, const wxString& name):
                                wxGLCanvas(parent, id, pos, size, style, name);

    OpenGLCanvas::~OpenGLCanvas(void);

  private:
    void OnPaint( wxPaintEvent& event );
    void OnSize( wxSizeEvent& event );
    void OnEraseBackground( wxEraseEvent& event );

    DECLARE_EVENT_TABLE();
};
