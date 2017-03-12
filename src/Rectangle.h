#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <cmath>
#include "stereozoom2.h"
#include <allegro.h>


enum zoom_focus { MOUSE_POSITION, IMAGE_FOCUS, VIEW_FOCUS };


class BlitData
{
public:
	BlitData(): from_start(2, 0), from_end(2, 0), to_start(2, 0), to_end(2, 0) {}

	valarray<unsigned int> from_start;
	valarray<unsigned int> from_end;
	valarray<unsigned int> getFromSize() const
	{
		return from_end - from_start;
	}
	valarray<unsigned int> to_start;
	valarray<unsigned int> to_end;
	valarray<unsigned int> getToSize() const
	{
		return to_end - to_start;
	}
	void printSelf()
	{
		printf("Blitting from source (%u, %u) - (%u, %u) to dest (%u, %u) - (%u, %u)\n",
				from_start[X], from_start[Y], from_end[X], from_end[Y],
				to_start[X], to_start[Y], to_end[X], to_end[Y],
				0);
	}
};


class ViewWithRectangle
{
public:
	ViewWithRectangle(int view_width, int view_height, int bitmap_width, int bitmap_height):
       		bitmap_size(XY<double>(bitmap_width, bitmap_height)), view_size(XY<double>(view_width, view_height)), upper_left_corner(XY<double>()), zoom(1) {}
	void loadedImage(valarray<unsigned int> bitmap_size)
	{
		this->bitmap_size[X] = bitmap_size[X];
		this->bitmap_size[Y] = bitmap_size[Y];
	}
	valarray<double> getBitmapCenteredPivot() const
	{
		valarray<double> bmp_center = bitmap_size / 2.0;
		return bmp_center;
	}
	valarray<double> getViewCenteredPivot() const
	{
		return upper_left_corner + current_size() / 2.0;
	}

	BlitData getBlitData() const
	{
		BlitData ret = BlitData();
		ret.from_start = XY<unsigned int, double>(drawFromLowerBound() + 0.5);
		ret.from_end = XY<unsigned int, double>(drawFromUpperBound() + 0.5);
		ret.to_start = XY<unsigned int, double>(drawToLowerBound() + 0.5);
		ret.to_end = XY<unsigned int, double>(drawToUpperBound() + 0.5);
		return ret;
	}

	bool rectangleIntersectsCanvas() const
	{
		bool result = false;
		if (true
			&& upper_left_corner[X] < bitmap_size[X] 
			&& lower_right_corner()[X] > 0
			&& upper_left_corner[Y] < bitmap_size[Y]
			&& lower_right_corner()[Y] > 0)
		{
			result = true;
		}
		return result;
	}

	void changeZoom(double zoom_change, valarray<double> pivot)
	{
		double new_zoom = zoom * zoom_change;
		setZoom(new_zoom, pivot);
	}

	void changePosition(valarray<double> pos_change)
	{
		valarray<double> new_position = upper_left_corner + pos_change;
		setPosition(new_position);
	}

	void setZoom(double new_zoom, valarray<double> pivot)
	{
		zoom = new_zoom;
		double zoom_ratio = new_zoom / zoom;
		valarray<double> new_size = current_size() * zoom_ratio;
		upper_left_corner = pivot - new_size / 2.0;
	}

	void setPosition(valarray<double> pos_change)
	{
		upper_left_corner = pos_change;
	}

	void setToJustSeeEverything()
	{
		double highest_zoom_possible = (view_size / bitmap_size).min();
		setZoom(highest_zoom_possible, getBitmapCenteredPivot());
	}
private:
	valarray<double> bitmap_size;
	valarray<double> view_size;

	valarray<double> upper_left_corner;

	double zoom;

	valarray<double> current_size() const
	{
		return lower_right_corner() - upper_left_corner;
	}

	valarray<double> lower_right_corner() const
	{
		return upper_left_corner + view_size / zoom;
	}

	valarray<double> drawFromLowerBound() const
	{
		valarray<double> result = upper_left_corner;
		result[X] = MAX(0.0, result[X]);
		result[Y] = MAX(0.0, result[Y]);
		return result;
	}

	valarray<double> drawFromUpperBound() const
	{
		valarray<double> result = lower_right_corner();
		result[X] = MIN(bitmap_size[X], result[X]);
		result[Y] = MIN(bitmap_size[Y], result[Y]);
		return result;
	}

	valarray<double> drawFromWidth() const
	{
		return drawFromUpperBound() - drawFromLowerBound();;
	}

	valarray<double> drawToLowerBound() const
	{
		valarray<double> lower_offset = drawFromLowerBound() - upper_left_corner;
		valarray<double> offset_ratio = lower_offset / current_size();
		return offset_ratio * view_size;
	}

	valarray<double> drawToUpperBound() const
	{
		valarray<double> upper_offset = lower_right_corner() - drawFromUpperBound();
		valarray<double> offset_ratio = upper_offset / current_size();
		return (1.0 - offset_ratio) * view_size;
	}

	valarray<double> drawToWidth() const
	{
		return drawToUpperBound() - drawToLowerBound();;
	}
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
