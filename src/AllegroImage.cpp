#include "AllegroImage.h"


void AllegroImage::clean()
{
	if (bitmap)
	{
		// TODO: Should not be called before Allegro exits
		destroy_bitmap(bitmap);
		bitmap = 0;
	}
}


void AllegroImage::mkImageDataStructure(unsigned char * rgbdata)
{
	clean();
	bitmap = create_bitmap(width, height);
	int color_pos, color;
	for(int jj = 0; jj < height; jj++)
	{
		for(int ii = 0; ii < width; ii++)
		{
			color_pos = (jj * width + ii) * 3;
			color = makecol(rgbdata[color_pos], rgbdata[color_pos + 1], rgbdata[color_pos + 2]);
			// for some reason, it is better to flip the image upside down
			putpixel(bitmap, ii, height - jj - 1, color);
		}
	}
}
