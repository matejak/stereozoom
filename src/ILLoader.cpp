#define LOADER
#include "module_interface.h"
#include "ILLoader.h"


void * PUBLIC(get_loader)()
{
	auto ret = new ILLoader();
	return reinterpret_cast<void *>(ret);
}


void PUBLIC(delete_loader)(void * loader)
{
	auto todelete = reinterpret_cast<ILLoader *>(loader);
	delete todelete;
}


void PUBLIC(init)()
{
	ilInit();
}


void PUBLIC(exit)()
{
	return;
}


void ILLoader::loadFromFileToImage(const char * filename, Image * result) const
{
	ilEnable(IL_ORIGIN_SET);
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
