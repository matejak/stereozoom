#pragma once

#include <vector>
using std::vector;
#include <stdexcept>
using std::runtime_error;

#include "MessageService.h"
#include "stereozoom.h"
#include "Image.h"
#include "Change.h"


enum sensitivity_subject {OF_ZOOM, OF_SHIFT, OF_OFFSET, SUBJECTS_COUNT};
enum sensitivity_device {BOOST, BY_KEYBOARD, BY_MOUSE, DEVICE_COUNT};


class Sensitivity
{
public:
	Sensitivity(): boosted(false), values{{0}}
	{
		values[OF_ZOOM][BY_KEYBOARD] = 1.2;
		values[OF_ZOOM][BY_MOUSE] = 1.15;
		values[OF_ZOOM][BOOST] = 2;
		values[OF_SHIFT][BY_KEYBOARD] = 2.5;
		values[OF_SHIFT][BY_MOUSE] = 1;
		values[OF_SHIFT][BOOST] = 2.5;
		values[OF_OFFSET][BY_KEYBOARD] = 1.0;
		values[OF_OFFSET][BOOST] = 2.0;
	}

	double get(sensitivity_subject of_what, sensitivity_device device)
	{
		double ret = values[int(of_what)][int(device)];
		if (boosted)
		{
			ret *= values[of_what][BOOST];
		}
		return ret;
	}

	// TODO: init with NaNs
	double values[DEVICE_COUNT][SUBJECTS_COUNT];	

	virtual void setBoostedStatus() {}
	bool boosted;
};


class Crosshair
{
public:
	Crosshair():size(0) {}
	virtual ~Crosshair() {}
	void createNormal(unsigned int size);
	virtual void draw(int x, int y) const = 0;
	void createFocused(unsigned int size)
	{
		size = size;
		prepare();
		drawNormal();
		drawFocused();
		finish();
	}
protected:
	void drawNormal();
	void drawFocused();
	virtual void drawCenteredCircle(double radius, double col_r, double col_g, double col_b) = 0;
	virtual void prepare() = 0;
	virtual void drawCenteredHline(double start, double end, double r, double g, double b) = 0;
	virtual void drawCenteredVline(double start, double end, double r, double g, double b) = 0;
	virtual void finish() {}

	double size;
};


class ImageGrid
{
public:
	ImageGrid(unsigned int group_width, unsigned int group_height, unsigned int view_width, unsigned int view_height):
		images(group_width * group_height, 0), views(group_width * group_height, 0), view_size(XY(view_width, view_height)), grid_shape(XY(group_width, group_height))
	{}
	virtual ~ImageGrid()
	{
		for (int ii = 0; ii < images.size(); ii++)
		{
			if (images[ii])
			{
				delete images[ii];
				images[ii] = 0;
			}
			if (views[ii])
			{
				delete views[ii];
				views[ii] = 0;
			}
		}
	}

	virtual void loadImageWhere(const char * filename, int x, int y, const Loader * loader) = 0;
	unsigned int getImageIndexAtPixelCoord(unsigned int coord_x, unsigned int coord_y)
	{
		return xyToIndex(coord_x / view_size[X], coord_y / view_size[Y]);
	}

	virtual void blit() = 0;

	virtual void createCrosshairs() = 0;
	virtual void drawCrosshairs(unsigned int coord_x, unsigned int coord_y) = 0;

	valarray<unsigned int> getSizeInPixels() const
	{
		return view_size * grid_shape;
	}

	void applyChangeToCurrent(Change * change)
	{
		valarray<unsigned int> current_view_xy = getCurrentViewCoords();
		applyChangeTo(change, views[xyToIndex(current_view_xy[X], current_view_xy[Y])]);
	}

	void applyChangeToOne(Change * change, unsigned int x, unsigned int y)
	{
		applyChangeTo(change, views[xyToIndex(x, y)]);
	}

	void applyChangeToAll(Change * change)
	{
		for (int ii = 0; ii < views.size(); ii++)
			applyChangeTo(change, views[ii]);
	}

	virtual valarray<int> getViewCoordinates() const = 0;

protected:
	virtual valarray<unsigned int> getCurrentViewCoords() const = 0;
	void applyChangeTo(Change * change, ViewWithRectangle * view)
	{
		change->transformView(view);
	}

	Image * getImageAtGridCoord(int x, int y)
	{
		return images[xyToIndex(x, y)];
	}

	valarray<unsigned int> indexToXY(unsigned int index)
	{
		return valarray<unsigned int>(XY<unsigned int>(index % grid_shape[X], index / grid_shape[X]));
	}

	unsigned int xyToIndex(int x, int y)
	{
		return x + grid_shape[X] * y;
	}

	vector<Image *> images;
	vector<ViewWithRectangle *> views;

	valarray<unsigned int> view_size;
	valarray<unsigned int> grid_shape;
};


class GenericUI
{
public:
	GenericUI(Sensitivity * sensitivities):
		message_service(nullptr), sensitivities(sensitivities), stereotuple(nullptr), dont_stop(true), dragging_now(false) {}
	virtual ~GenericUI()
	{
		if (message_service != nullptr)
		{
			delete message_service;
			message_service = nullptr;
		}
		if (stereotuple != nullptr)
		{
			delete stereotuple;
			stereotuple = nullptr;
		}
	}

	virtual void powerOn(unsigned int n_horizontal, unsigned int n_vertical, unsigned int size_width, unsigned int size_height)
	{
		initGfxMode(n_horizontal * size_width, n_vertical * size_height);
		stereotuple = makeImageGrid(n_horizontal, n_vertical, size_width, size_height);
		message_service = makeMessageService();
	}

	virtual SpecializedMessageService * makeMessageService() = 0;
	virtual ImageGrid * makeImageGrid(unsigned int n_horizontal, unsigned int n_vertical, unsigned int size_width, unsigned int size_height) = 0;
	virtual void initGfxMode(unsigned int hres, unsigned int vres) = 0;
	virtual void mainLoop() = 0;

	void loadImageWhere(const char * filename, int x, int y, const Loader * loader)
	{
		stereotuple->loadImageWhere(filename, x, y, loader);
	}
	Sensitivity * sensitivities;

protected:
	SpecializedMessageService * message_service;
	ImageGrid * stereotuple;

	bool dont_stop;
	bool dragging_now;
};
