/*
 * Gstereozoom.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * Gstereozoom.cpp is is part of stereozoom
 *
 * This program is free software;
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * stereozoom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with stereozoom.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include "stereozoom_internal.h"

#include "Main_gui.h"

#ifdef  HAVE_WINDOWS_H
#include <winalleg.h>
#include <wx/msw/winundef.h>
#endif // HAVE_WINDOWS_H

#include "wxStereozoom.h"

IMPLEMENT_APP(StereoApp)

bool StereoApp::OnInit()
{
	wxInitAllImageHandlers();

	Main_frame * Main_gui = new Main_window;

	SetTopWindow(Main_gui);
	Main_gui->Show();
	return true;

}
