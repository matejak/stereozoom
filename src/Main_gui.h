#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

/*
 * This file is part of stereozoom2.
 *
 * stereozoom2 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * stereozoom2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <wx/dc.h>
#include <wx/gdicmn.h>
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/dynarray.h>
#include <wx/dnd.h>
#include <wx/dataobj.h>
#include <wx/gbsizer.h>
#include <wx/timer.h>

#include "Image_group.h"
#include "Base_gui.h"

class Main_window;
class Image_panel;

class  MY_wxFileDropTarget : public wxFileDropTarget
{
public:
	MY_wxFileDropTarget(Image_panel * owner)
	{ Owner = owner; }
	/// We accept dropped files...
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
private:
	Image_panel * Owner;
};

class MY_timer : public wxTimer
{
public:
	MY_timer(wxWindow * to_update):
		To_update(to_update) {}
private:
	virtual void Notify() { (To_update - 2)->Refresh(); }
	wxWindow * To_update;
};

/// This is the class that holds the image stuff -- the preview, file picker and checkboxes
class Image_panel : public Panel_image
{
public:
	Image_panel(Main_window * parent, int x, int y);
	Image_panel(const Image_panel & rhs);
	wxString Get_filename() const  { return Current_file; }
	/// A file has been received, process it!
	bool File_drop(wxString filename);
	/// We have the right filename, so let's load the image!
	bool Load_image();
	/// How to paint on the screen
	void Paint_image();
	/// The panel has to paint now
	virtual void Repaint( wxPaintEvent& event )
	{ Paint_image(); }
	virtual void Chosen_file( wxFileDirPickerEvent& event );
	virtual void Dragdrop_start( wxMouseEvent& event );
	virtual void Toggle_preview( wxCommandEvent& event );
private:
	wxImage Image;	///< The image preview
	wxString Current_file;	///< The current filename
	wxFileDropTarget * Dragndrop_target;	///< The panel will be a target of drag and drop...
};

/// This class holds general stereozoom2 stuff and a matrix of Image_panel
class Main_window : public Main_frame
{
public:
	Main_window();
	virtual ~Main_window() {}
	/// Adds a row/column/both/none of picture panels depending where the user clicked
	void Check_dimensions(int x, int y);
	/// Regroups panels after a row was added/deleted
	void Regroup_panels();
	/// Panel matrix size modified
	virtual void Matrix_resize( wxSpinEvent& event )
	{ this->Regroup_panels(); }
	/// Start displaying the images!
	virtual void Start_clicked( wxCommandEvent& event );
	/// The About button has been pressed, display the about frame
	virtual void About_clicked( wxCommandEvent& event );
	/// Terminate gstereozoom2
	virtual void Exit_clicked( wxCommandEvent& event )
	{ this->Close(); }
private:
	wxGridBagSizer * Grid_images;	///< Pointer of the image panels sizer
	//wxFlexGridSizer * Grid_images;	///< Pointer of the image panels sizer
	int Images_size[2];	///< How are the dimensions of the image preview matrix
};


/// A shiny About dialog :-)
class About_window : public Dialog_about
{
public:
	About_window(wxWindow * parent);

	/// Somebody has clicked the close button
	virtual void Close_about( wxCommandEvent& event )
	{ this->Close(); }
private:
};

#endif /* MAIN_GUI_H_*/
