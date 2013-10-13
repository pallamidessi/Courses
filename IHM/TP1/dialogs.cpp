#include "dialogs.h"

	BEGIN_EVENT_TABLE(VersionDialog, wxDialog)
	END_EVENT_TABLE ()

VersionDialog::VersionDialog( wxWindow *parent, wxWindowID id,
														const wxString &title="Version") : 
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
	END_EVENT_TABLE ()

WidthLineDialog::WidthLineDialog( wxWindow *parent, wxWindowID id,
														const wxString &title="Epaisseur") : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxVERTICAL);
	wxStaticText* item1=new wxStaticText(this,ID_TEXT_WIDTHLINE,wxT("Choisir la nouvelle épaisseur de trait"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	wxButton* item2=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);
	wxSlider* item3=new wxSliderwxSlider(this, ID_SLIDER_WIDTH,1 , 1, 10, const wxPoint& point = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSL_HORIZONTAL, const wxValidator& validator = wxDefaultValidator, const wxString& name = "slider")
	
	item0->Add(item1,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item3,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item2,0,wxALIGN_CENTRE|wxALL,5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);
}
								
	BEGIN_EVENT_TABLE(ColorDialog, wxDialog)
	END_EVENT_TABLE ()

ColorDialog::ColorDialog( wxWindow *parent, wxWindowID id,
														const wxString &title="Couleur") : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxVERTICAL);
	wxStaticText* item1=new wxStaticText(this,ID_TEXT_COLOR,wxT("Choisir une nouvelle couleur"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	wxButton* item2=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);
	
	wxString strs8[] = { wxT("Rouge"), wxT("Vert"), wxT("Bleu")};

	wxRadioBox* item3=wxRadioBox(this, ID_RADIO_COLOR, wxT("Couleur"),wxDefaultPosition, wxDefaultSize, 3, strs8);

	item0->Add(item1,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item3,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(item2,0,wxALIGN_CENTRE|wxALL,5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);

}
								
	BEGIN_EVENT_TABLE(TriangleDialog, wxDialog)
	END_EVENT_TABLE ()

TriangleDialog::TriangleDialog( wxWindow *parent, wxWindowID id,
														const wxString &title="Gestion des triangles") : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* list_container=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* button_container=new wxBoxSizer(wxVERTICAL);
	
	wxStaticText* desc_title=new wxStaticText(this,ID_TEXT,wxT("Liste des triangles"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	
	wxButton* ok_button=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);
	wxButton* propriety_button=new wxButton(this,ID_PROP_BUTTON,wxT("Propriété"),wxDefaultPosition);
	wxButton* delete_button=new wxButton(this,ID_DEL_BUTTON,wxT("Supprimer"),wxDefaultPosition);

	
	wxListBox* triangle_list = wxListBox(this,ID_LISTBOX_TRIANGLE,wxDefaultPosition,wxDefaultSize, 0, NULL, long style = 0);

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

ProprietyDialog::ProprietyDialog( wxWindow *parent, wxWindowID id,
														const wxString &title="Propriété") : 
														wxDialog( parent, id, title){

	wxBoxSizer* item0=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* radio_container=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* misc_container=new wxBoxSizer(wxVERTICAL);
	
	wxStaticText* desc_section=new wxStaticText(this,ID_TEXT_PROPRIETY1,wxT("identifiant du triangle"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
	
	wxStaticText* width=new wxStaticText(this,ID_TEXT_PROPRIETY2,wxT("Epaisseur trait"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);

	wxButton* ok_button=new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);

	wxString strs8[] = { wxT("Rouge"), wxT("Vert"), wxT("Bleu")};
	wxRadioBox* radio=wxRadioBox(this, ID_RADIO_PROPRIETY, wxT("Couleur"),wxDefaultPosition, wxDefaultSize, 3, strs8);
	
 wxTextCtrl* textCtrl=wxTextCtrl(this, ID_TEXTCTRL,"",wxDefaultPosition,wxDefaultSize, long style = 0);
	

 wxSpinCtrl* spin= wxSpinCtrl(this,ID_SPINCTRL,wxEmptyString,wxDefaultPosition, wxDefaultSize, long style = wxSP_ARROW_KEYS, int min = 0, int max = 100, int initial = 0, const wxString& name = _T("wxSpinCtrl"));

	item0->Add(desc_title,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(radio_container,0,wxALIGN_CENTRE|wxALL,5);
	item0->Add(misc_container,0,wxALIGN_CENTRE|wxALL,5);

	radio_container->Add(radio,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(desc_section,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(textCtrl,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(width,0,wxALIGN_CENTRE|wxALL);	
	misc_container->Add(spin,0,wxALIGN_CENTRE|wxALL);	

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	item0->Fit(this);
	item0->SetSizeHints(this);
								
}
