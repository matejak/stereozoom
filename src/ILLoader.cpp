#include "ILLoader.h"


void * get_loader()
{
	auto ret = new ILLoader();
	return (void *)ret;
}


void delete_loader(void * loader)
{
	auto todelete = (ILLoader *)loader;
	delete todelete;
}


void ILLoader::loadFromFileToImage(const char * filename, Image * result) const
{
	// IL overhead
	ILuint loaded_image;
	ilGenImages(1, & loaded_image);
	ilBindImage(loaded_image);
	ilLoadImage(filename);

	// We read parameters and data
	unsigned int width = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int to_allocate = width * height * 3;
	unsigned char * image_data = (unsigned char *) new unsigned char [to_allocate];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, (void *)image_data );

	// We create the result
	result->supplyData(image_data, width, height, filename);
	delete [] image_data;
	ilDeleteImages(1, & loaded_image);
	// The image itself takes care of deleting the data
}
