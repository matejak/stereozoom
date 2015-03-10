///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 10 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Base_gui.h"

///////////////////////////////////////////////////////////////////////////

Main_frame::Main_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* Sizer_1;
	Sizer_1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image matrix") ), wxVERTICAL );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Image matrix size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer13->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	Spin_matrix_w = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 64, 2 );
	bSizer13->Add( Spin_matrix_w, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("⨯"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer13->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	Spin_matrix_h = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 64, 1 );
	bSizer13->Add( Spin_matrix_h, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sbSizer3->Add( bSizer13, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	Text_image_1 = new wxStaticText( this, wxID_ANY, wxT("Displayed image width:"), wxDefaultPosition, wxDefaultSize, 0 );
	Text_image_1->Wrap( -1 );
	bSizer12->Add( Text_image_1, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	Spin_imgres_w = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 64000, 200 );
	Spin_imgres_w->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer12->Add( Spin_imgres_w, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	Text_image_2 = new wxStaticText( this, wxID_ANY, wxT("px, height:"), wxDefaultPosition, wxDefaultSize, 0 );
	Text_image_2->Wrap( -1 );
	bSizer12->Add( Text_image_2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );
	
	Spin_imgres_h = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 48000, 300 );
	Spin_imgres_h->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer12->Add( Spin_imgres_h, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	Text_image_3 = new wxStaticText( this, wxID_ANY, wxT("px."), wxDefaultPosition, wxDefaultSize, 0 );
	Text_image_3->Wrap( -1 );
	bSizer12->Add( Text_image_3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxStaticBoxSizer* sbSizer31;
	sbSizer31 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("presets") ), wxHORIZONTAL );
	
	preset_small = new wxButton( this, wxID_ANY, wxT("small"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer31->Add( preset_small, 0, wxALL, 5 );
	
	preset_big = new wxButton( this, wxID_ANY, wxT("BIG"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer31->Add( preset_big, 0, wxALL, 5 );
	
	
	bSizer12->Add( sbSizer31, 1, wxEXPAND, 5 );
	
	
	sbSizer3->Add( bSizer12, 1, wxEXPAND, 5 );
	
	
	Sizer_1->Add( sbSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	Button_exit = new wxButton( this, ID_BUTTON_EXIT, wxT("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( Button_exit, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	Button_about = new wxButton( this, ID_BUTTON_ABOUT, wxT("About"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( Button_about, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	Button_start = new wxButton( this, ID_BUTTON_START, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	Button_start->SetToolTip( wxT("launch stereozoom2, the tool to examine selected images") );
	
	bSizer2->Add( Button_start, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	
	Sizer_1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( Sizer_1 );
	this->Layout();
	Sizer_1->Fit( this );
	
	// Connect Events
	Spin_matrix_w->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( Main_frame::Matrix_resize ), NULL, this );
	Spin_matrix_h->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( Main_frame::Matrix_resize ), NULL, this );
	preset_small->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::setPresetSmall ), NULL, this );
	preset_big->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::setPresetBig ), NULL, this );
	Button_exit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::Exit_clicked ), NULL, this );
	Button_about->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::About_clicked ), NULL, this );
	Button_start->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::Start_clicked ), NULL, this );
}

Main_frame::~Main_frame()
{
	// Disconnect Events
	Spin_matrix_w->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( Main_frame::Matrix_resize ), NULL, this );
	Spin_matrix_h->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( Main_frame::Matrix_resize ), NULL, this );
	preset_small->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::setPresetSmall ), NULL, this );
	preset_big->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::setPresetBig ), NULL, this );
	Button_exit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::Exit_clicked ), NULL, this );
	Button_about->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::About_clicked ), NULL, this );
	Button_start->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Main_frame::Start_clicked ), NULL, this );
	
}

Dialog_about::Dialog_about( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* Sizer_1;
	Sizer_1 = new wxBoxSizer( wxVERTICAL );
	
	Text_title_1 = new wxStaticText( this, wxID_ANY, wxT("stereozoom2"), wxDefaultPosition, wxDefaultSize, 0 );
	Text_title_1->Wrap( -1 );
	Text_title_1->SetFont( wxFont( 16, 70, 90, 92, false, wxEmptyString ) );
	
	Sizer_1->Add( Text_title_1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	Text_title_2 = new wxStaticText( this, wxID_ANY, wxT("a multi picture viewer"), wxDefaultPosition, wxDefaultSize, 0 );
	Text_title_2->Wrap( -1 );
	Text_title_2->SetFont( wxFont( 14, 70, 90, 90, false, wxEmptyString ) );
	
	Sizer_1->Add( Text_title_2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	Bitmap_icon = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 128,128 ), 0 );
	bSizer4->Add( Bitmap_icon, 1, wxALL|wxEXPAND, 5 );
	
	Text_descr = new wxStaticText( this, wxID_ANY, wxT("This is free software (licensed under GNU GPL v3).\nProgrammed by Matěj Týč, 2008\nMay all beings be happy when using this software.\nPlease mail bubla@users.sf.net fearlessly concerning bugs and feature suggestions."), wxDefaultPosition, wxDefaultSize, 0 );
	Text_descr->Wrap( 400 );
	bSizer4->Add( Text_descr, 0, wxALL, 5 );
	
	
	Sizer_1->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	Sizer_1->Add( 0, 5, 0, wxEXPAND, 5 );
	
	link_stereozoom = new wxHyperlinkCtrl( this, wxID_ANY, wxT("stereozoom2.sf.net"), wxT("http://stereozoom2.sourceforge.net"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	Sizer_1->Add( link_stereozoom, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	Sizer_1->Add( 0, 10, 0, wxEXPAND, 5 );
	
	Button_close = new wxButton( this, ID_BUTTON_CLOSE_ABOUT, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	Sizer_1->Add( Button_close, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( Sizer_1 );
	this->Layout();
	Sizer_1->Fit( this );
	
	// Connect Events
	Button_close->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_about::Close_about ), NULL, this );
}

Dialog_about::~Dialog_about()
{
	// Disconnect Events
	Button_close->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_about::Close_about ), NULL, this );
	
}

Panel_image::Panel_image( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxStaticBoxSizer* Group_image;
	Group_image = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("image name") ), wxVERTICAL );
	
	Check_preview = new wxCheckBox( this, ID_CHECK_PREVIEW, wxT("Show preview"), wxDefaultPosition, wxDefaultSize, 0 );
	Check_preview->SetValue(true); 
	Group_image->Add( Check_preview, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	Image_space = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	Image_space->SetToolTip( wxT("use drag and drop here. You can drag files inside the form as well as files from outside to here!") );
	Image_space->SetMinSize( wxSize( 100,75 ) );
	
	Group_image->Add( Image_space, 1, wxEXPAND | wxALL, 5 );
	
	Line_image = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	Group_image->Add( Line_image, 0, wxEXPAND | wxALL, 5 );
	
	File_picker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("All image files | *.bmp;*.BMP;*.gif;*.GIF;*.jpg;*.JPG;*.jpeg;*.JPEG;*.pcx;*.PCX;*.png;*.PNG;*.tga;*.TGA;*.tif;*.TIF;*.tiff;*.TIFF;*.bmp;*.BMP|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_CHANGE_DIR|wxFLP_DEFAULT_STYLE );
	Group_image->Add( File_picker, 0, wxALL|wxEXPAND, 5 );
	
	Check_use = new wxCheckBox( this, ID_USE_IMAGE, wxT("Use the image"), wxDefaultPosition, wxDefaultSize, 0 );
	Group_image->Add( Check_use, 0, wxRIGHT|wxLEFT, 5 );
	
	
	this->SetSizer( Group_image );
	this->Layout();
	
	// Connect Events
	Check_preview->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( Panel_image::Toggle_preview ), NULL, this );
	Image_space->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( Panel_image::Dragdrop_start ), NULL, this );
	Image_space->Connect( wxEVT_PAINT, wxPaintEventHandler( Panel_image::Repaint ), NULL, this );
	File_picker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( Panel_image::Chosen_file ), NULL, this );
}

Panel_image::~Panel_image()
{
	// Disconnect Events
	Check_preview->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( Panel_image::Toggle_preview ), NULL, this );
	Image_space->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( Panel_image::Dragdrop_start ), NULL, this );
	Image_space->Disconnect( wxEVT_PAINT, wxPaintEventHandler( Panel_image::Repaint ), NULL, this );
	File_picker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( Panel_image::Chosen_file ), NULL, this );
	
}
