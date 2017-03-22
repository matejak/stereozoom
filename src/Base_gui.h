///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 16 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __BASE_GUI_H__
#define __BASE_GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/hyperlink.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////

#define ID_BUTTON_EXIT 1000
#define ID_BUTTON_ABOUT 1001
#define ID_BUTTON_START 1002
#define ID_BUTTON_CLOSE_ABOUT 1003
#define ID_CHECK_PREVIEW 1004
#define ID_USE_IMAGE 1005

///////////////////////////////////////////////////////////////////////////////
/// Class Main_frame
///////////////////////////////////////////////////////////////////////////////
class Main_frame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText12;
		wxSpinCtrl* Spin_matrix_w;
		wxStaticText* m_staticText13;
		wxSpinCtrl* Spin_matrix_h;
		wxStaticText* Text_image_1;
		wxSpinCtrl* Spin_imgres_w;
		wxStaticText* Text_image_2;
		wxSpinCtrl* Spin_imgres_h;
		wxStaticText* Text_image_3;
		wxButton* preset_small;
		wxButton* preset_big;
		wxButton* Button_exit;
		wxButton* Button_about;
		wxButton* Button_start;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Matrix_resize( wxSpinEvent& event ) { event.Skip(); }
		virtual void setPresetSmall( wxCommandEvent& event ) { event.Skip(); }
		virtual void setPresetBig( wxCommandEvent& event ) { event.Skip(); }
		virtual void Exit_clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void About_clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void Start_clicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		Main_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~Main_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Dialog_about
///////////////////////////////////////////////////////////////////////////////
class Dialog_about : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* Text_title_1;
		wxStaticText* Text_title_2;
		wxStaticBitmap* Bitmap_icon;
		wxStaticText* Text_descr;
		wxHyperlinkCtrl* link_stereozoom;
		wxButton* Button_close;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Close_about( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		Dialog_about( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About stereozoom2"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~Dialog_about();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Panel_image
///////////////////////////////////////////////////////////////////////////////
class Panel_image : public wxPanel 
{
	private:
	
	protected:
		wxCheckBox* Check_preview;
		wxPanel* Image_space;
		wxStaticLine* Line_image;
		wxFilePickerCtrl* File_picker;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Toggle_preview( wxCommandEvent& event ) { event.Skip(); }
		virtual void Dragdrop_start( wxMouseEvent& event ) { event.Skip(); }
		virtual void Repaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void Chosen_file( wxFileDirPickerEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* Check_use;
		
		Panel_image( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~Panel_image();
	
};

#endif //__BASE_GUI_H__
