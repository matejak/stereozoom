#ifndef PAIR_H_
#define PAIR_H_

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


class Message
{
public:
	Message(const char * text):text(text) {}
	std::string text;

};


class MessageRecord
{
public:
	MessageRecord(const char * text, double duration);
	double time_to_expire;
	time_point<steady_clock> time_inserted;

	double getRemainingSeconds()
	{
		double ret = (duration_cast<milliseconds>(time_inserted.time_since_epoch() - steady_clock::now().time_since_epoch())).count() / 1000.0 + time_to_expire;
		return ret;
	}

	std::string getMessageText() const
	{
		return message->text;
	}

	Message * message;
};



struct APtrComp
{
	bool operator () (const MessageRecord * lhs, const MessageRecord * rhs) const
	{
		return lhs->time_inserted < rhs->time_inserted;
	}
};


class MessageService
{
public:
	virtual ~MessageService();
	void addMessage(const char * msg, double time_to_live);
	void cleanOldMessages();
	virtual void displayMessages() const = 0;
	void purgeOldMessages();
protected:
	set<MessageRecord *, APtrComp> messages;
};


class AllegroMessageService: public MessageService
{
public:
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
	~Crosshair()
	{
		if (bitmap) {
			destroy_bitmap(bitmap); }
		bitmap = 0;
	}
	void createNormal(unsigned int size);
	void draw(BITMAP * dest_bitmap, int x, int y) const
	{
		if (bitmap == NULL) 
			throw runtime_error("Attempted to draw a non-existing crosshair.");
		draw_sprite(dest_bitmap, bitmap, x - bitmap->w / 2.0, y - bitmap->h / 2.0);
	}
	void createFocused(unsigned int size)
	{
		createNormal(size);
		circle(bitmap, bitmap->w / 2, bitmap->h / 2, size / 6, makecol(255,255,0));
		circle(bitmap, bitmap->w / 2, bitmap->h / 2, size / 15, makecol(0, 0, 0));
	}
protected:
	BITMAP * bitmap;
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


class AllegroImageGrid: public ImageGrid
{
public:
	AllegroImageGrid(unsigned int group_width, unsigned int group_height, unsigned int view_width, unsigned int view_height):
		ImageGrid(group_width, group_height, view_width, view_height) 
	{}

	void gfxModeOn()
	{
		int crosshair_dimension = int(view_size.min() / 5.0);
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


class AllegroUI
{
public:
	AllegroUI(AllegroImageGrid & stereotuple):
		sensitivities(), screen_buffer(0), stereotuple(stereotuple), dont_stop(true), dragging_now(false), message_service(& screen_buffer) {}
	~AllegroUI() 
	{
		clean();
	}

	void createBuffer()
	{
		set_color_depth(desktop_color_depth());
		valarray<unsigned int> window_size_in_pixels = stereotuple.getSizeInPixels();

		int setmode_code = set_gfx_mode(GFX_AUTODETECT_WINDOWED, window_size_in_pixels[X], window_size_in_pixels[Y], 0, 0);

		if (1)
			printf("Graphics mode: %dx%d, result: %d\n", window_size_in_pixels[X], window_size_in_pixels[Y], setmode_code);

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
			message_service.purgeOldMessages();
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
		message_service.displayMessages();
		blit(screen_buffer, screen, 0, 0, 0, 0, screen_buffer->w, screen_buffer->h);
	}
private:
	void setSensitivities();

	AllegroSensitivity sensitivities;
	BITMAP * screen_buffer;

	AllegroImageGrid & stereotuple;
	bool dont_stop;

	bool dragging_now;

	AllegroMessageService message_service;

};


/// class Image_group represents the group of examined images
/*
 * It takes care mainly about user input
class Image_group
{
public:
	Image_group();
	/// A copy constructor, that is necessary
	Image_group(const Image_group & rhs);
	~Image_group();
	/// Function mainly fills variables with meaningful values
	int Init(int img_dims[2], int max_coords[2]);
	/// Adds another image to the stack
	int Add_image(string filename, int coords[2] );
	/// Just starts the main loop
	int Start();
	/// Displays some text in the display area for an amount of time - tenth of seconds
	void Display_notice(const char * text, int time_decsecs);
	/// Timer callback function, just decreases the value of clock variable if it is less than zero
	static void Timer_callback()
	{
		if (Duration > 0) Duration--;
	}
	END_OF_STATIC_FUNCTION(Timer_callback)	//stuff required by Allegro

	/// Function specifying behavior when we press the mouse button. Useful for dragging stuff
	static void button_down(int flag)
	{
		if (flag & (MOUSE_FLAG_LEFT_DOWN) )
		{	//we are interested only in left mouse button pressed down
			int a, b;
			get_mouse_mickeys(& a, & b);	// poll the mouse with something
		}//endif MOUSE_FLAG_LEFT_DOWN
		else if (flag & (MOUSE_FLAG_RIGHT_DOWN) )
		{
			//maybe we could display color values of the current pixel here
		}//endif MOUSE_FLAG_RIGHT_DOWN
	}
	END_OF_STATIC_FUNCTION(button_down)	//stuff required by Allegro
private:
	/// Draws some crosshair shape to the crosshair bitmap that is created if necessary
	void Make_crosshair();
	/// Clears the leftover space
	void Destroy_crosshair()
	{ if (Crosshair) destroy_bitmap(Crosshair); Crosshair = 0; }
	/// Displays help message
	void Print_help();
	/// Displays current zoom value
	void Print_zoom();
	/// Displays current offset value
	void Print_offset();
	/// Main loop
	int Loop();
	/// Process keypresses
	void ProcessUserInput(int & loop);

	double Zoom_boost;	///< Zoom multiplier; when thing have to speed up
	double Move_boost;	///< Move multiplier; when thing have to speed up
	double Offset_boost;///< Offset multiplier; when thing have to speed up

	double Zoom_sens;	///< Zoom multiplier; during normal conditions
	double Move_sens;	///< Move multiplier; during normal conditions
	double Offset_sens;	///< Offset multiplier; during normal conditions

	int Current_image;	///< The number of the image that is active, -1 if none

	int Img_dim[2];	///< Dimension of the image

	vector<Image> Images;	///< Images (projections)
	int Max_coords[2];	///< The highest image coordinates

	BITMAP * Notice; ///< Some notice displayed to the user
	static volatile int Duration; ///< How transparent the notice is - 0..255

	BITMAP * Crosshair; ///< The crosshair mouse cursor
};
*/


#endif /*PAIR_H_*/
