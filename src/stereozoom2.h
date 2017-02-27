#ifndef STEREOZOOM2_H_
#define STEREOZOOM2_H_

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

#include <string>
#include <vector>

enum mode {GROW_DEFAULT, GROW_ROWS, GROW_COLS};
enum expected {EXP_NONE = 0x1, EXP_FILE = 0x2, EXP_SPACE = 0x4, EXP_RES = 0x8, EXP_MATRIX = 0x10, EXP_POSITION = 0x20};
enum matlage {MTL_RESOLUTION, MTL_FILENAME, MTL_FORMAT, MTL_EXPECT, MTL_MATRIX, MTL_POSITION, MTL_HELP, MTL_ERRORRE};

///An inline function that should be used to copy arrays of objects supporting operator =
/**
 */
template <class T, int num>
void copy_array(const T * source, T * dest)
{for (int i = 0; i < num; i++) dest[i] = (source ? source[i] : 0);}		//if source == 0, treat is as a 0 vector


template <class T>
class XY
{
public:
	XY<T> operator +(const XY<T> & rhs) const
	{
		XY<T> result = * this;
		result += rhs;
		return result;
	}
	XY<T> operator -() const
	{
		XY<T> result = * this;
		result *= -1;
		return result;
	}
	XY<T> operator -(const XY<T> & rhs) const
	{
		XY<T> result = * this;
		result += -rhs;
		return result;
	}
	XY<T> operator +=(const XY<T> & rhs)
	{
		XY<T> result = * this;
		x += rhs.x;
		y += rhs.y;
		return * this;
	}
	XY<T> operator *=(double rhs)
	{
		x *= rhs;
		y *= rhs;
		return * this;
	}
	XY<T> operator *=(const XY<T> & rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return * this;
	}
	XY<T> operator *(const XY<T> & rhs) const
	{
		XY<T> result = * this;
		result *= rhs;
		return result;
	}
	XY<T> operator /=(double rhs)
	{
		x *= 1 / rhs;
		y *= 1 / rhs;
		return * this;
	}
	XY<T> operator /(double rhs) const
	{
		XY<T> result = * this;
		result /= rhs;
		return result;
	}
	XY<T> operator /=(const XY<T> & rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		return * this;
	}
	XY<T> operator /(const XY<T> & rhs) const
	{
		XY<T> result = * this;
		result /= rhs;
		return result;
	}
	XY(): x(0), y(0) {}
	T x;
	T y;
};

/// This class stores the filename and coordinates of a picture
class Entry
{
public:
	Entry(std::string filename, int coords[2]):
		Filename(filename)
		{ copy_array<int,2>(coords, Coords); }
//private:
	std::string Filename;
	int Coords[2];
};

/// This is the library interface!
class stereozoom2
{
public:
	/// This constructor does all the stuff
	stereozoom2(const char * arguments);
private:
	/// Pass one word from the argument string and then the next one and so on and this function will take care of everything...
	int Parse_args(const char * input);
	/// Prints some basic help message
	void Print_help();
	/// Checks whether the image coordinates are within the bounds (Matrix_size) defined by the user
	void Apply_matrix();

	std::vector<Entry> Entries;	///< Image filenames with their coords

	int Coords[2];	///< coordinates of the current picture;
	int Matrix_size[2];	///< size of the image matrix

	int Grow_mode;	///< Are we going to fill rows, or columns?
	int Expected;	///< Are we expecting some parameter to be passed?

	int Resolution[2];	///< The current size of one displayed image
	int Max_coords[2];	///< The "bounding rectangle" of the image arrangement
};

enum corner {ULX = 0, ULY, LRX, LRY, UPPER_LEFT_X = 0, UPPER_LEFT_Y, LOWER_RIGHT_X, LOWER_RIGHT_Y};
enum coord {X, Y, Z};

extern int verbose, test;

#undef MAX
/// Safer MAX template function
template <class T>
T MAX(T  x, T  y)
{return (x > y ? x : y);}

#undef MIN
/// Safer MIN template function
template <class T>
T MIN(T  x, T  y)
{return (x < y ? x : y);}

#endif /*STEREOZOOM2_H_*/
