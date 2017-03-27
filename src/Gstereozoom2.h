#ifndef GSTEREOZOOM2_H_
#define GSTEREOZOOM2_H_

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

#include <wx/app.h>

class StereoApp : public wxApp
{
public:
	virtual bool OnInit();
private:
};

#endif /*GSTEREOZOOM2_H_*/