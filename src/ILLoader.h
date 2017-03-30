#ifndef ILLOADER_H_
#define ILLOADER_H_

#include "Image.h"


// TODO: This class should be a singleton (due to the ilInit stuff)
class ILLoader: public Loader
{
public:
	ILLoader()
	{
		ilInit();
		ilEnable(IL_ORIGIN_SET);
	}
	void loadFromFileToImage(const char * filename, Image * result) const override;
};


#endif /*ILLOADER_H_*/
