#pragma once

#include <allegro.h>

#include "Image.h"
#include "Image_group.h"


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


class AllegroCrosshair: public Crosshair
{
public:
	AllegroCrosshair(BITMAP ** destination):
		Crosshair(), dest_bitmap_ptr(destination), our_crosshair(nullptr) {}
	virtual ~AllegroCrosshair()
	{
		if (our_crosshair != nullptr)
		{
			destroy_bitmap(our_crosshair);
		}
		our_crosshair = nullptr;
	}
	virtual void draw(int x, int y) const override
	{
		if (our_crosshair == nullptr)
			return;
		if (* dest_bitmap_ptr == nullptr) 
			throw runtime_error("Attempted to draw a non-existing crosshair.");
		draw_sprite(* dest_bitmap_ptr, our_crosshair, x - our_crosshair->w / 2.0, y - our_crosshair->h / 2.0);
	}
private:
	void prepare(unsigned int size) override;
	void drawCenteredHline(double start, double end, double r, double g, double b) override;
	void drawCenteredVline(double start, double end, double r, double g, double b) override;
	void drawCenteredCircle(double radius, double col_r, double col_g, double col_b) override
	{
		circle(our_crosshair, our_crosshair->w / 2, our_crosshair->h / 2, radius, makecol(255 * col_r, 255 * col_g, 255 * col_b));
	}
	BITMAP ** dest_bitmap_ptr;
	BITMAP * our_crosshair;
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


class AllegroImageGrid: public ImageGrid
{
public:
	AllegroImageGrid(BITMAP * buffer, unsigned int group_width, unsigned int group_height, unsigned int view_width, unsigned int view_height):
		ImageGrid(group_width, group_height, view_width, view_height), buffer(buffer), normal_crosshair(& this->buffer), focused_crosshair(& this->buffer)
	{}

	virtual void createCrosshairs() override
	{
		int crosshair_dimension = int(60 * sqrt(view_size.min() / 300.0));
		normal_crosshair.createNormal(crosshair_dimension);
		focused_crosshair.createFocused(crosshair_dimension);
	}

	virtual void drawCrosshairs(unsigned int coord_x, unsigned int coord_y) override;

	virtual void loadImageWhere(const char * filename, int x, int y, const Loader * loader) override;
	virtual void blit() override;

	valarray<int> getViewCoordinates() const override;

private:
	virtual valarray<unsigned int> getCurrentViewCoords() const override
	{
		return XY<unsigned int>(mouse_x, mouse_y) / view_size;
	}
	BITMAP * buffer;
	AllegroCrosshair normal_crosshair;
	AllegroCrosshair focused_crosshair;
};


class AllegroUI : public GenericUI
{
public:
	AllegroUI(Sensitivity * sensitivities):
		GenericUI(sensitivities), screen_buffer(0)
	{}
	~AllegroUI() 
	{
		clean();
	}

	virtual void initGfxMode(unsigned int hres, unsigned int vres) override;

	virtual SpecializedMessageService * makeMessageService()
	{
		return new AllegroMessageService(& screen_buffer);
	}

	virtual ImageGrid * makeImageGrid(unsigned int n_horizontal, unsigned int n_vertical, unsigned int size_width, unsigned int size_height)
	{
		auto ret = new AllegroImageGrid(screen_buffer, n_horizontal, n_vertical, size_width, size_height);
		ret->createCrosshairs();
		return ret;
	}

	void mainLoop() override
	{
		while(dont_stop)
		{
			draw();
			message_service->purgeOldMessages();
			rest(50);
			
			sensitivities->setBoostedStatus();
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
		stereotuple->blit();
		stereotuple->drawCrosshairs(mouse_x, mouse_y);
		message_service->displayMessages();
		blit(screen_buffer, screen, 0, 0, 0, 0, screen_buffer->w, screen_buffer->h);
	}
private:

	BITMAP * screen_buffer;
};
