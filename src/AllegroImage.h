#pragma once

#include <allegro.h>

#include "Image.h"


class AllegroImage: public Image
{
public:
	AllegroImage():Image(), bitmap(0) {}
	virtual ~AllegroImage()
	{
		clean();
	}
	virtual bool isEmpty() const override
	{
		return (bitmap == 0);
	}
	BITMAP * bitmap;
protected:
	virtual void mkImageDataStructure(unsigned char * rgbdata) override;
	void clean();
};
