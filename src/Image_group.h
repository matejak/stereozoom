#pragma once

#include <vector>
using std::vector;
#include <set>
using std::set;
#include <string>
using std::string;
#include <stdexcept>
using std::runtime_error;
#include <chrono>
using namespace std::chrono;

#include "stereozoom2.h"
#include "Image.h"
#include "Change.h"


enum sensitivity_subject {OF_ZOOM, OF_SHIFT, OF_OFFSET, SUBJECTS_COUNT};
enum sensitivity_device {BOOST, BY_KEYBOARD, BY_MOUSE, DEVICE_COUNT};


struct APtrComp
{
	bool operator () (const MessageRecord * lhs, const MessageRecord * rhs) const
	{
		return lhs->time_inserted < rhs->time_inserted;
	}
};


class AllegroMessageService: public SpecializedMessageService
{
public:
	virtual ~AllegroMessageService() {}
	AllegroMessageService(BITMAP ** screen_buffer_ptr):screen_buffer_ptr(screen_buffer_ptr) {}
	void displayMessages() const override;
private:
	BITMAP ** screen_buffer_ptr;
};


class Sensitivity
{
public:
	Sensitivity(): boosted(false), values{{0}} {}
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


class AllegroSensitivity: public Sensitivity
{
public:
	void setBoostedStatus() override
	{
		if (key_shifts & KB_SHIFT_FLAG)
		{
			boosted = true;
		}
		else
		{
			boosted = false;
		}
	}
};


class Crosshair
{
public:
	Crosshair(): bitmap(0) {}
	virtual ~Crosshair() {}
	virtual void createNormal(unsigned int size) = 0;
	virtual void draw(int x, int y) const = 0;
	void createFocused(unsigned int size)
	{
		createNormal(size);
		drawCenteredCircle(size / 6, 255, 255, 0);
		drawCenteredCircle(size / 15, 0, 0, 0);
	}
protected:
	void drawCenteredCircle(double radius, int col_r, int col_g, int col_b);
};


class AllegroCrosshair: public Crosshair
{
public:
	AllegroCrosshair(BITMAP ** destination):
		Crosshair(), dest_bitmap_ptr(destination), our_crosshair(0) {}
	virtual ~AllegroCrosshair()
	{
		if (our_crosshair)
		{
			destroy_bitmap(our_crosshair);
		}
		our_crosshair = 0;
	}
	virtual void createNormal(unsigned int size) override;
	virtual void draw(int x, int y) const override
	{
		if (* dest_bitmap == nullptr) 
			throw runtime_error("Attempted to draw a non-existing crosshair.");
		draw_sprite(* dest_bitmap, our_crosshair, x - our_crosshair->w / 2.0, y - our_crosshair->h / 2.0);
	}
private:
	void drawCenteredCircle(double radius, int col_r, int col_g, int col_b)
	{
		circle(our_crosshair, our_crosshair->w / 2, our_crosshair->h / 2, radius, makecol(col_r, col_g, col_b))
	}
};


class ImageGrid
{
public:
	ImageGrid(unsigned int group_width, unsigned int group_height, unsigned int view_width, unsigned int view_height):
		images(group_width * group_height, 0), views(group_width * group_height, 0), view_size(XY(view_width, view_height)), grid_shape(XY(group_width, group_height))
	{}
	~ImageGrid()
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

	virtual void LoadImageWhere(const char * filename, int x, int y, const Loader * loader) = 0;
	unsigned int getImageIndexAtPixelCoord(unsigned int coord_x, unsigned int coord_y)
	{
		return xyToIndex(coord_x / view_size[X], coord_y / view_size[Y]);
	}
	virtual void blit(BITMAP * buffer) = 0;
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
	GenericUI(Sensitivity * sensitivities, MessageService * message_service):
		sensitivities(sensitivities), stereotuple(stereotuple), dont_stop(true), dragging_now(false), message_service(message_service) {}
protected:
	MessageService * message_service;
	bool dont_stop;
	Sensitivity * sensitivities;
};


class AllegroUI : public GenericUI
{
public:
	AllegroUI(AllegroImageGrid & stereotuple):
		sensitivities(), screen_buffer(0), stereotuple(stereotuple), dont_stop(true), dragging_now(false) {}
	~AllegroUI() 
	{
		clean();
	}

	void createBuffer()
	{
		set_color_depth(desktop_color_depth());
		valarray<unsigned int> window_size_in_pixels = stereotuple.getSizeInPixels();

		int setmode_code = set_gfx_mode(GFX_AUTODETECT_WINDOWED, window_size_in_pixels[X], window_size_in_pixels[Y], 0, 0);

		if (verbose)
			printf("Trying to set resolution: %dx%d, %d bpp\n", window_size_in_pixels[X], window_size_in_pixels[Y], get_color_depth());

		screen_buffer = create_bitmap(window_size_in_pixels[X], window_size_in_pixels[Y]);
		// TODO: Utilize the observer pattern here
		stereotuple.gfxModeOn();
	}

	void mainLoop()
	{
		setSensitivities();
		while(dont_stop)
		{
			draw();
			message_service->purgeOldMessages();
			rest(50);
			
			sensitivities.setBoostedStatus();
			vector<int> keystrokes = pollForKeystrokes();
			keystrokes = processUIControl(keystrokes);
			keystrokes = processUserInput(keystrokes);
			processMouseZoom();
			processMouseDrag();
		}
	}

	vector<int> processUIControl(vector<int>);
	vector<int> processUserInput(vector<int>);
	vector<int> pollForKeystrokes();

	void processMouseDrag();
	void processMouseZoom();

	void printHelp();

	void clean()
	{
		if (screen_buffer)
		{
			destroy_bitmap(screen_buffer);
			screen_buffer = 0;
		}
	}
	void draw()
	{
		clear_to_color(screen_buffer, makecol(0, 0, 0));
		stereotuple.blit(screen_buffer);
		stereotuple.draw_crosshairs(screen_buffer, mouse_x, mouse_y);
		message_service->displayMessages();
		blit(screen_buffer, screen, 0, 0, 0, 0, screen_buffer->w, screen_buffer->h);
	}
private:
	void setSensitivities();

	AllegroSensitivity sensitivities;
	BITMAP * screen_buffer;

	AllegroImageGrid & stereotuple;
	bool dont_stop;

	bool dragging_now;
};
