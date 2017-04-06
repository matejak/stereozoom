#pragma once

#include <cmath>
#include "stereozoom.h"


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
		return bitmap_size / 2.0;
	}
	valarray<double> getViewCenteredPivot() const
	{
		return viewToBitmapCoord(view_size / 2.0);
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
		valarray<double> new_position = upper_left_corner + pos_change / zoom;
		setPosition(new_position);
	}

	void changeOffset(valarray<double> offset_change)
	{
		valarray<double> new_position = upper_left_corner + offset_change;
		setPosition(new_position);
	}

	void setZoom(double new_zoom, valarray<double> pivot)
	{
		if (verbose)
			printf("Set new zoom %.2g -> %.2g with pivot (%.2g, %.2g)\n", zoom, new_zoom, pivot[X], pivot[Y]);
		double zoom_ratio = new_zoom / zoom;
		zoom = new_zoom;
		valarray<double> old_pivot_positional_vector = pivot - upper_left_corner;
		upper_left_corner = pivot - old_pivot_positional_vector / zoom_ratio;
	}

	void setPosition(valarray<double> pos_change)
	{
		upper_left_corner = pos_change;
	}

	void centerPositionOverBitmap()
	{
		setPosition((bitmap_size - current_size()) / 2.0);
	}

	void setToJustSeeEverything()
	{
		centerPositionOverBitmap();
		double highest_zoom_possible = (view_size / bitmap_size).min();
		setZoom(highest_zoom_possible, getBitmapCenteredPivot());
	}

	valarray<double> viewToBitmapCoord(valarray<double> view_coord) const;
	valarray<double> bitmapToViewCoord(valarray<double> bitmap_coord) const;
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
