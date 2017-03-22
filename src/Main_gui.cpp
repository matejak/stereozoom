/*
 * Main_gui.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * Main_gui.cpp is is part of stereozoom2
 *
 * This program is free software;
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * stereozoom2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with stereozoom2.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#define TO_STRING(str) #str

#ifndef DATADIR
#define DATADIR(x) TO_STRING(./x)
#endif //DATADIR

#include "Main_gui.h"
#include <sstream>



bool MyWxFileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if(filenames.IsEmpty())
		return false;

	Owner->File_drop(filenames[0]);
	return true;
}

Main_window::Main_window():
	Main_frame(NULL, -1, wxT("gStereozoom^2")), images_count(XY<unsigned int>(0, 0)), main_sizer(this->GetSizer()), grid_sizer(nullptr)
{
	Grid_images.SetFlexibleDirection(wxBOTH);
	Regroup_panels();
}


void Main_window::setupSizer()
{
	auto new_grid_sizer = new wxGridSizer(images_count[X], images_count[Y]);
	populateGridSizer(new_grid_sizer);
	clearGridSizer();
	grid_sizer = new_grid_sizer;
	main_sizer->Insert(0, grid_sizer, 1, wxEXPAND);
}


void Main_window::clearAllImagePanels()
{
	wxWindow * current_panel = nullptr;
	auto it = known_panels.begin()
	while (it != known_panels.end())
	{
		current_panel = it->second;
		it = known_panels.erase(it);
		delete current_panel;
	}
}


void Main_window::populateGridSizer(wxSizer * sizer)
{
	createMissingImagePanels()
	for (unsigned int i = 0; i < images_count[X]; i++)
	{
		for (unsigned int j = 0; j < images_count[Y]; j++)
		{
			auto position = std::make_pair(i, j)
			sizer->Add(known_panels[position]);
		}
	}
}


void Main_window::Regroup_panels()
{
	int spin_x = Spin_matrix_w->GetValue(),
		spin_y = Spin_matrix_h->GetValue();
	const unsigned int old_size[] = { images_count[X], images_count[Y] };
	unsigned int current_size[Z];
	copy_array<unsigned int, Z>(old_size, current_size);
	const int new_size[] = { spin_x, spin_y };
	unsigned int point[Z];
	// this is needed in order to make smart vectorized stuff
	void (wxFlexGridSizer::* add[Z]) (size_t, int) = { & wxFlexGridSizer::AddGrowableCol, & wxFlexGridSizer::AddGrowableRow};
	void (wxFlexGridSizer::* remove[Z]) (size_t) = { & wxFlexGridSizer::RemoveGrowableCol, & wxFlexGridSizer::RemoveGrowableRow};

	wxSizerFlags flags(1);
	flags.Expand();
	for (int i = 0; i <= Y; i++)
	{
		int other_coord = (i + 1) % Z;
		int diff = new_size[i] - old_size[i];
		if (diff > 0)	//something has been added
		{
			for (int j = 0; j < diff; j++)
			{
				current_size[i] += 1;
				copy_array<unsigned int, Z>(current_size, point);
				point[i]--; // no buffer overflow, please
				for (int k = 0; k < current_size[other_coord]; k++)
				{// Now adding a row/column. 'k' goes through "the other than i" coord
					point[other_coord] = k;
					wxGBPosition pos(point[Y], point[X]);

					Image_panel * image = new Image_panel(this, point[X], point[Y]);
					Grid_images.Add(image, pos, wxDefaultSpan, wxEXPAND);	//and to the sizer
				}//end for (int k = 0; k < current_size[(i + 1) % 2]; k++)
				(Grid_images.*add[i])(point[i], 0);
			}//endfor (j = 0; j < diff; j++)
		}// endif(diff > 0)
		else
		{// have to remove some stuff
			diff = - diff;
			for (int j = 0; j < diff; j++)
			{
				current_size[i] -= 1;
				copy_array<unsigned int, Z>(current_size, point);
				//point[i]--; // we get rid of somethging, so don't uncomment this!
				for (int k = 0; k < current_size[other_coord]; k++)
				{// Now adding a row/column. 'k' goes through "the other than i" coord
					point[other_coord] = k;
					wxGBPosition pos(point[Y], point[X]);

					wxWindow * panel = Grid_images.FindItemAtPosition(pos)->GetWindow();
					Grid_images.Detach(panel);
					panel->Destroy();
				}//end for (int k = 0; k < current_size[(i + 1) % 2]; k++)
				(Grid_images.*remove[i])(point[i]);
			}//endfor (j = 0; j < diff; j++)
		}// endelse(diff > 0)
	}

	Grid_images.SetCols(spin_x);
	Grid_images.SetRows(spin_y);

	images_count[X] = spin_x;
	images_count[Y] = spin_y;

	//Grid_images->Layout();
	this->Fit();
	this->GetSizer()->RecalcSizes();
}


void Main_window::createMissingImagePanels()
{
	for (unsigned int i = 0; i < images_count[X]; i++)
	{
		for (unsigned int j = 0; j < images_count[Y]; j++)
		{
			auto coord_index = pair<unsigned int, unsigned int>(i, j);
			if (known_panels.find(coord_index) == known_panels.end())
			{
				known_panels.emplace(coord_index, new Image_panel(this, i, j));
			}
		}
	}
}


void Main_window::Start_clicked( wxCommandEvent& event )
{
	std::stringstream command(std::stringstream::out);
	command << "-a\n" << Spin_imgres_w->GetValue() << "x" << Spin_imgres_h->GetValue() << "\n";
	command << "-m\n" << Spin_matrix_w->GetValue() << "," << Spin_matrix_h->GetValue() << "\n";

	Image_panel * impanel = 0;
	for (int i = 0; i < Spin_matrix_w->GetValue() * Spin_matrix_h->GetValue(); i++)
	{
		impanel = (Image_panel *)(Grid_images.GetItem(i)->GetWindow());
		if (impanel->Check_use->GetValue() == false)
			command << "-s\n1\n";
		else
		{
			impanel = (Image_panel *)(Grid_images.GetItem(i)->GetWindow());
			command << "-f\n" << impanel->Get_filename().mb_str() << "\n";
		}//endelse(impanel->Check_use->GetValue() == false)
	}
	// check whether the stereozoom2 window is going to fit in the desktop
	int wres, hres;
	wxDisplaySize(& wres, & hres);
	if (Spin_matrix_w->GetValue() * Spin_imgres_w->GetValue() >= wres ||
	    Spin_matrix_h->GetValue() * Spin_imgres_h->GetValue() >= hres )
	{
		wxMessageDialog(this, _("Cut down the image matrix or display resolution since it won't fit to the screen like this"), _("Oversize trouble"), wxOK | wxICON_EXCLAMATION).ShowModal();
		return;
	}

	MY_timer * Update_timer = new MY_timer(this);
	//MY_timer Update_timer(this);
	//Update_timer.Start(200);

	// wxThread stereozoom_thread(wxTHREAD_JOINABLE );
	stereozoom2(command.str().c_str());

	//Update_timer.Stop();

	// do the rest
	//event.Skip();
}

void Main_window::About_clicked( wxCommandEvent& event )
{
	Dialog_about * about = new About_window(this);

	about->ShowModal();

	about->Destroy();

	// do the rest
	event.Skip();
}

Image_panel::Image_panel(Main_window * parent, int x, int y):
	Panel_image((wxWindow *)parent, -1)
{
	Dragndrop_target = new MyWxFileDropTarget(this);
	// drag and drop support
	SetDropTarget(Dragndrop_target);
}

Image_panel::Image_panel(const Image_panel & rhs):
	Panel_image((wxWindow *)rhs.GetParent(), -1)
{
	Image = rhs.Image;
	Current_file = rhs.Current_file;
	Dragndrop_target = new MyWxFileDropTarget(this);
	SetDropTarget(Dragndrop_target);
}

bool Image_panel::File_drop(wxString filename)
{
	Current_file = filename;
	File_picker->SetPath(Current_file);
	int retval = Load_image();
	if (retval) // the image could be loaded
		Check_use->SetValue(true);
	return retval;
}

bool Image_panel::Load_image()
{
	bool loaded = Image.LoadFile(Current_file);
	Refresh();
	if (!loaded)
	{// load unsuccesful
		wxMessageDialog(this, wxT("You won't see the image preview now since the format is not common. I just hope that you actually selected a valid image..."), wxT("Unusual format"), wxOK | wxICON_EXCLAMATION);
		return false;
	}
	else
		return true;
}

void Image_panel::Paint_image()
{
	wxPaintDC temp_DC(Image_space);
	if (Check_preview->GetValue() == false || Image.IsOk() == false)
		return;	//preview is not wanted

	wxImage img_temp;

	img_temp = Image.Scale(Image_space->GetSize().GetWidth(), Image_space->GetSize().GetHeight());

	temp_DC.DrawBitmap(img_temp, 0, 0);
}

void Image_panel::Chosen_file( wxFileDirPickerEvent& event )
{
	Current_file = File_picker->GetPath();	// setting the current filename
	if (Load_image())
		Check_use->SetValue(true);

	// do the rest
	event.Skip();
}

void Image_panel::Dragdrop_start( wxMouseEvent& event )
{

//	wxMessageDialog(this, _("Dragdrop started")).ShowModal();
//
	//File_data.SetData(wxDF_FILENAME, 1, & Current_file);	// setting the right filename for drag & drop
	wxFileDataObject File_data;
	File_data.AddFile(Current_file);
	wxDropSource dragSource( this );
	dragSource.SetData( File_data );
	wxDragResult result = dragSource.DoDragDrop( TRUE );

	// do the rest
	event.Skip();
}

void Image_panel::Toggle_preview( wxCommandEvent& event )
{
	if(Check_preview->IsChecked())
		Paint_image();
	else //preview is not checked
		Refresh();

	// do the rest
	event.Skip();
}

About_window::About_window(wxWindow * parent):
	Dialog_about(parent, -1)
{
	//load the logo that has to be installed somewhere...
	Bitmap_icon->SetBitmap( wxBitmap(wxT(DATADIR(logo_stereozoom2.png)), wxBITMAP_TYPE_PNG));
}
