#ifndef IMAGE_H_
#define IMAGE_H_

#include "Rectangle.h"

#include <allegro.h>
#include <IL/il.h>

///Class Image does everything connected with drawing
class Image
{
public:
	Image();
	/// Copy ctor, absolutely neccessary!
	Image(const Image & rhs);
	~Image()
	{ Destroy(); }
	/// Initializes variables
	int Init(const char * filename, int img_dim[2], int coords[2]);
	/// Let's tidy up after finishing
	void Destroy();
	/// Operator used to simplify syntax when blitting
	operator BITMAP * ();
	/// Sets the highest zoom when the whole picture is visible at the same time
	void Show_whole_picture();
	/// Returns the real coordinates
	int Get_position(int dimension) const
	{ return Img_dim[dimension] * Img_coords[dimension];  }
	/// Returns the logical coordinates
	int Get_coords(int dimension) const
	{ return Img_coords[dimension];  }
	/// Passes change of the image offset to the rectangle
	void Change_offset(int x, int y)
	{
		int offset [2] = {x, y};
		Rect.D_offset(offset);
	}
	/// Passes change of zoom to the rectangle in order to set it according to a given value
	void Set_zoom(double zoom, int focus_x = -1, int focus_y = -1)
	{ Change_zoom(zoom/Rect.Get_zoom() - 1.0, focus_x, focus_y); }
	/// Passes change of the image zoom to the rectangle
	void Change_zoom(double change, double focus_x = -1.0, double focus_y = -1.0)
	{
		double focus_point [2] = {focus_x, focus_y};
		Rect.D_zoom(change, focus_point);
	}
	/// Passes change of the image position to the rectangle
	void Change_pos(double DX, double DY)
	{
		double change [2] = {DX, DY};
		Rect.D_pos(change);
	}
	/// Directly returns the zoom value
	double Get_zoom() const { return Rect.Get_zoom(); }
	/// Passes offset through a int [2] array
	void Get_offset(int * offset) { Rect.Get_offset(offset); }

private:

	int Pic_dim[2];	///< Dimensions of the picture
	int Img_dim[2];	///< Dimensions of the image
	int Img_coords[2];	///< Image coordinates on the output

	BITMAP * Pic;	///< Picture bitmap, the image file
	BITMAP * Img;	///< Image bitmap, the projected image of Picture

	Rectangle Rect; ///< Rect controls carrying out of user's orders
};

#endif /*IMAGE_H_*/
