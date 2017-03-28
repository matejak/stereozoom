
#include "AllegroImageGroup.h"
#include "AllegroImage.h"


void AllegroImageGrid::blit(BITMAP * buffer)
{
	int vertical_offset, horizontal_offset;
	for (int ii = 0; ii < images.size(); ii++)
	{
		valarray<unsigned int> image_xy = indexToXY(ii);
		horizontal_offset = image_xy[X] * view_size[X];
		vertical_offset = image_xy[Y] * view_size[Y];

		AllegroImage * image = (AllegroImage *)images[ii];
		BlitData coords = views[ii]->getBlitData();
		stretch_blit(image->bitmap, buffer, 
				coords.from_start[X], coords.from_start[Y],
				coords.getFromSize()[X], coords.getFromSize()[Y],
				coords.to_start[X] + horizontal_offset, coords.to_start[Y] + vertical_offset,
				coords.getToSize()[X], coords.getToSize()[Y]
			    );
	}
}


void AllegroImageGrid::draw_crosshairs(BITMAP * buffer, unsigned int coord_x, unsigned int coord_y)
{
	valarray <unsigned int> base_coordinate = XY<unsigned int>(coord_x % view_size[X], coord_y % view_size[Y]);
	for (int ii = 0; ii < images.size(); ii++)
	{
		valarray<unsigned int> base_coordinate_multiplier = indexToXY(ii);
		valarray<unsigned int> destination = base_coordinate_multiplier * view_size + base_coordinate;
		if (destination[X] == coord_x && destination[Y] == coord_y)
			focused_crosshair.draw(buffer, destination[X], destination[Y]);
		else
			normal_crosshair.draw(buffer, destination[X], destination[Y]);
	}
}


void AllegroImageGrid::LoadImageWhere(const char * filename, int x, int y, const Loader * loader)
{
	int image_index = xyToIndex(x, y);
	AllegroImage * image = (AllegroImage *)images[image_index];
	ViewWithRectangle * view = views[image_index];
	if (image != nullptr)
	{
		delete image;
	}
	if (view != nullptr)
	{
		delete view;
	}
	image = new AllegroImage();
	images[image_index] = image;
	loader->loadFromFileToImage(filename, image);

	view = new ViewWithRectangle(view_size[X], view_size[Y], image->width, image->height);
	views[image_index] = view;

	ChangeSetViewToOriginNoZoom change;
	// change.transformView(view);
}


valarray<int> AllegroImageGrid::getViewCoordinates() const
{
	valarray<int> local_mouse_position = XY<int>(mouse_x, mouse_y) % XY<int, unsigned int>(view_size);
	return local_mouse_position;
}
