#pragma once

#include "Image_group.h"


class AllegroImageGrid: public ImageGrid
{
public:
	AllegroImageGrid(unsigned int group_width, unsigned int group_height, unsigned int view_width, unsigned int view_height):
		ImageGrid(group_width, group_height, view_width, view_height) 
	{}

	void gfxModeOn()
	{
		int crosshair_dimension = int(60 * sqrt(view_size.min() / 300.0));
		normal_crosshair.createNormal(crosshair_dimension);
		focused_crosshair.createFocused(crosshair_dimension);
	}

	virtual void LoadImageWhere(const char * filename, int x, int y, const Loader * loader);
	virtual void blit(BITMAP * buffer);

	void draw_crosshairs(BITMAP * buffer, unsigned int coord_x, unsigned int coord_y);

	valarray<int> getViewCoordinates() const override;

private:
	virtual valarray<unsigned int> getCurrentViewCoords() const
	{
		return XY<unsigned int>(mouse_x, mouse_y) / view_size;
	}
	Crosshair normal_crosshair;
	Crosshair focused_crosshair;
};
