#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <cmath>
#include "stereozoom2.h"


class ViewWithRectangle;


class Change
{
public:
	XY<double> shift;
	double zoom;
};


class AbsoluteChange: Change
{
public:
	void applyChange(XY<double> pivot, ViewWithRectangle & subject);
private:
};


class RelativeChange: Change
{
public:
	void applyChange(XY<double> pivot, ViewWithRectangle & subject);
private:
};


class Rectangle2
{
public:
	Rectangle2 getCroppedThatFitsInside(XY<double> bounds)
	{
		Rectangle2 cropped;
		cropped.upper_left_corner.x = MAX(0.0, upper_left_corner.x);
		cropped.upper_left_corner.y = MAX(0.0, upper_left_corner.y);
		cropped.lower_right_corner.x = MIN(bounds.x, lower_right_corner.x);
		cropped.lower_right_corner.y = MIN(bounds.x, lower_right_corner.y);
		return cropped;
	}

	XY<double> getViewCenteredPivot() const
	{
		XY<double> ret;
		ret.x = (lower_right_corner.x + upper_left_corner.x ) / 2.0;
		ret.y = (lower_right_corner.y + upper_left_corner.y ) / 2.0;
		return ret;
	}

	XY<double> upper_left_corner;
	XY<double> lower_right_corner;
};


class ViewWithRectangle
{
public:
	XY<double> getBitmapCenteredPivot() const
	{
		XY<double> bmp_center = bitmap_size / 2.0;
		return bmp_center;
	}

	XY<double> getViewCenteredPivot() const
	{
		XY<double> view_middle = viewer.getViewCenteredPivot();
		return view_middle;
	}

	void applyAbsoluteChange(XY<double> pivot, const Change * change)
	{
		zoom = change->zoom;
		shift = change->shift;
	}

	void applyRelativeChange(XY<double> pivot, const Change * change)
	{
		zoom *= change->zoom;
		shift += change->shift;
	}

	XY<double> screenToBitmap(XY<double> coord) const
	{
		const XY<double> ratio = coord / view_size;
		XY<double> their_position = viewer.upper_left_corner;
		their_position += ratio * viewer.lower_right_corner - viewer.upper_left_corner;
		return their_position;
	};

	XY<double> bitmapToScreen(XY<double> coord) const
	{
		XY<double> ratio = coord - viewer.upper_left_corner;
		ratio /= viewer.lower_right_corner - viewer.upper_left_corner;
		return ratio * view_size;
	};
private:
	void applyChange(XY<double> pivot)
	{
		viewer.upper_left_corner = screenToBitmap(pivot) - pivot / zoom - shift;
		viewer.lower_right_corner = viewer.upper_left_corner + (view_size / zoom);
	}

	Rectangle2 viewer;
	XY<double> view_size;
	XY<double> bitmap_size;

	double zoom;
	XY<double> shift;
};


/// A class that has to determine which part of the picture will be projected to the image
/**
 * Basically, there are this kinds of problems:
 * - clipping. Only the interior of the bitmap can be projected.
 * - zooming. The size of the rectangle isn't constant.
 * - moving. The rectangle is supposed to change position quite often.
 */
class Rectangle
{
public:
	/// Finction basicaly fills class variables and sets meaningful defaults
	int Init(int pic_dim[2], int img_dim[2]);

	/// Fills vars with data concerning stretched blitting
	void Set_area(int source [4], int dest[4]);
	/// Used to extract useful info for Image - borders of drawable area at this time
	int Pic_area(int index);
	/// Used to extract useful info for Image - borders of alien area at this time
	int Img_area(int index);

	/// Want to change zoom?
	void D_zoom(double DZ, double focus_point [2]);
	/// Want to move around?
	void D_pos(double change [2]);
	/// Offset one image to align them
	void D_offset(int offset [2]);

	/// Access zoom
	double Get_zoom() const {return Zoom;}
	/// Returns offset by modifying a parameter
	void Get_offset(int * offset) const {offset[0] = Offset[0]; offset[1] = Offset[1];}
private:

	/// Transform function, picture -> image, DO NOT PASS THE SAME VALUE AS pic AND img !!
	void Pic_to_img(double pic[2], double img[2]);
	/// Transform function, image -> picture, DO NOT PASS THE SAME VALUE AS pic AND img !!
	void Img_to_pic(double img[2], double pic[2]);

	/// This function is used to determine whether the changes applied are acceptable or not...
	int Check();


	double Zoom; ///< Current value of the zoom

	double Out[4]; ///< Minimal pixel shift to eliminate rectangle overlapping {X-,Y-,X+,Y+}

	int Offset [2]; ///< Offset of two rectangles

	double Pos [2]; ///< Position of the rectangle
	double Dim [2]; ///< Dimensions of the rectangle

	int Pic_dim [2]; ///< Dimensions of the picture
	int Img_dim [2]; ///< Dimensions of the image
};

#endif /*RECTANGLE_H_*/
