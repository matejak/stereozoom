#ifndef IMAGE_H_
#define IMAGE_H_

#include "Rectangle.h"

#include <allegro.h>
#include <IL/il.h>
#include <string>


// TODO: supplyData gets RGB data, saves them to its own structure.
class Image
{
public:
	Image() {}
	virtual ~Image() {}
	void supplyData(unsigned char * rgbData, unsigned int width, unsigned int height, const char * filename);

	virtual bool isEmpty() const = 0;

	unsigned int width;
	unsigned int height;
	std::string filename;
protected:
	virtual void mkImageDataStructure(unsigned char * rgbData) = 0;
};


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
protected:
	virtual void mkImageDataStructure(unsigned char * rgbdata) override;
	void clean();
	BITMAP * bitmap;
};


class Loader
{
public:
	virtual void loadFromFileToImage(const char * filename, Image * result) const = 0;
};


#endif /*IMAGE_H_*/
