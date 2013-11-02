#include "dialogs.h"

	BEGIN_EVENT_TABLE(VersionDialog, wxDialog)
	END_EVENT_TABLE ()

VersionDialog::VersionDialog( wxWindow *parent, wxWindowID id,
														const wxString &title=wxT("Version")) : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxVERTICAL);
	wxStaticText* item1=new wxStaticText(this,ID_TEXT,wxT("Version 1.0"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	wxButton* item2=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);

	item0->Add(item1,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item2,0,wxALIGN_CENTRE|wxALL,5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);

								
}
	
	BEGIN_EVENT_TABLE(WidthLineDialog, wxDialog)
		EVT_SCROLL(WidthLineDialog::OnWIDGET_SLIDER)
	END_EVENT_TABLE ()

WidthLineDialog::WidthLineDialog( wxWindow *parent, wxWindowID id,
														const wxString &title=wxT("Epaisseur")) : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxVERTICAL);
	wxStaticText* item1=new wxStaticText(this,ID_TEXT_WIDTHLINE,wxT("Choisir la nouvelle épaisseur de trait"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	wxButton* item2=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);
	wxSlider* item3=new wxSlider(this, ID_SLIDER_WIDTH,((CMainFrame*)GetParent())->get_width() , 1, 10,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL,wxDefaultValidator, wxT("slider"));
	
	item0->Add(item1,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item3,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item2,0,wxALIGN_CENTRE|wxALL,5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);
}

void WidthLineDialog::OnWIDGET_SLIDER(wxScrollEvent& event){
	((CMainFrame*)GetParent())->set_width(event.GetPosition());
}
								
	BEGIN_EVENT_TABLE(ColorDialog, wxDialog)
		EVT_RADIOBOX(ID_RADIO_COLOR,ColorDialog::OnWIDGET_RADIO)
	END_EVENT_TABLE ()

ColorDialog::ColorDialog( wxWindow *parent, wxWindowID id,
														const wxString &title=wxT("Couleur")) : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxVERTICAL);
	wxStaticText* item1=new wxStaticText(this,ID_TEXT_COLOR,wxT("Choisir une nouvelle couleur"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	wxButton* item2=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);
	
	wxString strs8[] = { wxT("Rouge"), wxT("Vert"), wxT("Bleu")};

	wxRadioBox* item3=new wxRadioBox(this, ID_RADIO_COLOR, wxT("Couleur"),wxDefaultPosition, wxDefaultSize, 3, strs8);
	
	if (((CMainFrame*)GetParent())->couleurCourante->Red()) {
		item3->SetSelection(0);
	}
	else if (((CMainFrame*)GetParent())->couleurCourante->Green()) {
		item3->SetSelection(1);
	}
	else if (((CMainFrame*)GetParent())->couleurCourante->Blue()) {
		item3->SetSelection(2);
	}

	item0->Add(item1,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item3,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item2,0,wxALIGN_CENTRE|wxALL,5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);

}
void ColorDialog::OnWIDGET_RADIO(wxCommandEvent& event){
	if(event.GetInt()==0)
		((CMainFrame*)GetParent())->set_color(wxT("red"));
	else if(event.GetInt()==1)
		((CMainFrame*)GetParent())->set_color(wxT("green"));
	else if(event.GetInt()==2)
		((CMainFrame*)GetParent())->set_color(wxT("blue"));
}

								
	BEGIN_EVENT_TABLE(TriangleDialog, wxDialog)
		EVT_BUTTON(ID_PROP_BUTTON,TriangleDialog::OnWIDGET_PROPRIETY)
		EVT_BUTTON(ID_DEL_BUTTON,TriangleDialog::OnWIDGET_DELETE)
		EVT_LISTBOX(ID_LISTBOX_TRIANGLE,TriangleDialog::OnWIDGET_LISTBOX)
	END_EVENT_TABLE ()

TriangleDialog::TriangleDialog( wxWindow *parent, wxWindowID id,
														const wxString &title=wxT("Gestion des triangles")) : 
														wxDialog( parent, id, title){

	selected_triangle=-1;
	wxBoxSizer* item0=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* list_container=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* button_container=new wxBoxSizer(wxVERTICAL);
	
	wxStaticText* desc_title=new wxStaticText(this,ID_TEXT_TRIANGLE,wxT("Liste des triangles"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	
	wxButton* ok_button=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);
	wxButton* propriety_button=new wxButton(this,ID_PROP_BUTTON,wxT("Propriété"),wxDefaultPosition);
	wxButton* delete_button=new wxButton(this,ID_DEL_BUTTON,wxT("Supprimer"),wxDefaultPosition);

	wxString* name=new wxString[5];
	int i;

	wxListBox* triangle_list =new wxListBox(this,ID_LISTBOX_TRIANGLE,wxDefaultPosition,wxDefaultSize, 0);
	
	for (i = 0; i < ((CMainFrame*)GetParent())->get_num_tri(); i++) {
		name[i].Printf(wxT("Triangle %d"),i);
	}

	triangle_list->InsertItems(((CMainFrame*)GetParent())->get_num_tri(),name,0);

	item0->Add(desc_title,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(list_container,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(button_container,0,wxALIGN_CENTRE|wxALL,5);

	list_container->Add(triangle_list,0,wxALIGN_CENTRE|wxALL);	
	button_container->Add(propriety_button,0,wxALIGN_CENTRE|wxALL);	
	button_container->Add(delete_button,0,wxALIGN_CENTRE|wxALL);	
	button_container->Add(ok_button,0,wxALIGN_CENTRE|wxALL);	

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);
								
}

void TriangleDialog::OnWIDGET_PROPRIETY(wxCommandEvent& event){
	
	ProprietyDialog pdlg(this,-1,wxT("Propriété"));
	Triangle tri=((CMainFrame*)GetParent())->get_triangle(selected_triangle);
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
	
	name.Printf(wxT("Triangle %d"),selected_triangle);

	pdlg.textCtrl->ChangeValue(name);
	pdlg.spin->SetValue((int)tri.thickness);
	std::cout<<tri.thickness<<std::endl;
	pdlg.ShowModal();
}

void TriangleDialog::OnWIDGET_DELETE(wxCommandEvent& event){
	int i;

	if (selected_triangle==-1) {
		return;
	}

	if (selected_triangle==((CMainFrame*)GetParent())->num_tri-1) {
		((CMainFrame*)GetParent())->num_tri-=1;
	}
	else{
		for (i = selected_triangle; i < ((CMainFrame*)GetParent())->num_tri; i++) {
			((CMainFrame*)GetParent())->tab_tri[i]=((CMainFrame*)GetParent())->tab_tri[i+1];
		}
		((CMainFrame*)GetParent())->num_tri-=1;
	}

	if(((CMainFrame*)GetParent())->num_tri==0){
    ((CMainFrame*)GetParent())->GetMenuBar()->Enable(MENU_TRIANGLE,false);
	}
	
	((CMainFrame*)GetParent())->Update();
}


void TriangleDialog::OnWIDGET_LISTBOX(wxCommandEvent& event){
	selected_triangle=event.GetSelection();	
}

	BEGIN_EVENT_TABLE(ProprietyDialog, wxDialog)
		EVT_RADIOBOX(ID_RADIO_PROPRIETY,ProprietyDialog::OnWIDGET_RADIO)
		EVT_SPINCTRL(ID_SPINCTRL,ProprietyDialog::OnWIDGET_SPIN)

	END_EVENT_TABLE ()

ProprietyDialog::ProprietyDialog( wxWindow *parent, wxWindowID id,
														const wxString &title=wxT("Propriété")) : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* radio_container=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* misc_container=new wxBoxSizer(wxVERTICAL);
	
	wxStaticText* desc_section=new wxStaticText(this,ID_TEXT_PROPRIETY1,wxT("identifiant du triangle"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	
	wxStaticText* width=new wxStaticText(this,ID_TEXT_PROPRIETY2,wxT("Epaisseur trait"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);

	wxButton* ok_button=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);

	wxString strs8[] = { wxT("Rouge"), wxT("Vert"), wxT("Bleu")};
	radio=new wxRadioBox(this, ID_RADIO_PROPRIETY, wxT("Couleur"),wxDefaultPosition, wxDefaultSize, 3, strs8);
	
 textCtrl=new wxTextCtrl(this, ID_TEXTCTRL,wxT(""),wxDefaultPosition,wxDefaultSize, 0);
	

 spin=new  wxSpinCtrl(this,ID_SPINCTRL,wxEmptyString,wxDefaultPosition, wxDefaultSize,wxSP_ARROW_KEYS,0,100, 0,wxT("wxSpinCtrl"));

	item0->Add(radio_container,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(misc_container,0,wxALIGN_CENTRE|wxALL,5);

	radio_container->Add(radio,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(desc_section,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(textCtrl,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(width,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(spin,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(ok_button,0,wxALIGN_CENTRE|wxALL);	

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);
								
}

void ProprietyDialog::OnWIDGET_RADIO(wxCommandEvent& event){
	wxString chose_colour;

	if (event.GetSelection()==0) {
		chose_colour.Printf(wxT("RGB(255,0,0)"));
	}
	else if (event.GetSelection()==1) {
		chose_colour.Printf(wxT("RGB(0,255,0)"));
	}
	else if (event.GetSelection()==2) {
		chose_colour.Printf(wxT("RGB(0,0,255)"));
	}
	
	((CMainFrame*)(GetParent()->GetParent()))->tab_tri[((TriangleDialog*)GetParent())->selected_triangle].colour.Set(chose_colour);
}

void ProprietyDialog::OnWIDGET_SPIN(wxSpinEvent& event){
	((CMainFrame*)(GetParent()->GetParent()))->tab_tri[((TriangleDialog*)GetParent())->selected_triangle].thickness=spin->GetValue();
}
