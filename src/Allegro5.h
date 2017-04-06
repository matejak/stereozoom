#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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
	ALLEGRO_BITMAP * bitmap;
protected:
	virtual void mkImageDataStructure(unsigned char * rgbdata) override;
	void clean();
};

class AllegroSensitivity: public Sensitivity
{
public:
	void setBoostedStatus() override
	{
		al_get_keyboard_state(& keyboard);
		bool l_shift_on = al_key_down(& keyboard, ALLEGRO_KEY_LSHIFT);
		bool r_shift_on = al_key_down(& keyboard, ALLEGRO_KEY_RSHIFT);
		bool shift_on = l_shift_on || r_shift_on;
		if (shift_on)
		{
			boosted = true;
		}
		else
		{
			boosted = false;
		}
	}
private:
	ALLEGRO_KEYBOARD_STATE keyboard;
};


class AllegroCrosshair: public Crosshair
{
public:
	AllegroCrosshair(ALLEGRO_BITMAP ** destination):
		Crosshair(), dest_bitmap_ptr(destination), our_crosshair(nullptr) {}
	virtual ~AllegroCrosshair()
	{
		if (our_crosshair != nullptr)
		{
			al_destroy_bitmap(our_crosshair);
		}
		our_crosshair = nullptr;
	}
	virtual void draw(int x, int y) const override
	{
		if (our_crosshair == nullptr)
			return;
		if (* dest_bitmap_ptr == nullptr) 
			throw runtime_error("Attempted to draw a non-existing crosshair.");
		al_draw_bitmap(our_crosshair, x - size / 2.0, y - size / 2.0, 0);
	}
private:
	void prepare() override;
	void finish() override;
	void drawCenteredHline(double start, double end, double r, double g, double b) override;
	void drawCenteredVline(double start, double end, double r, double g, double b) override;
	void drawCenteredCircle(double radius, double col_r, double col_g, double col_b) override
	{
		al_draw_circle(size / 2, size / 2, radius, al_map_rgb(255 * col_r, 255 * col_g, 255 * col_b), 0);
	}
	ALLEGRO_BITMAP ** dest_bitmap_ptr;
	ALLEGRO_BITMAP * our_crosshair;
};


class AllegroMessageService: public SpecializedMessageService
{
public:
	AllegroMessageService()
	{
		font = al_create_builtin_font();
	}
	~AllegroMessageService()
	{
		al_destroy_font(font);
	}
	void displayMessages() const override;
private:
	ALLEGRO_FONT * font;
};


class AllegroImageGrid: public ImageGrid
{
public:
	AllegroImageGrid(ALLEGRO_BITMAP * buffer, unsigned int group_width, unsigned int group_height, unsigned int view_width, unsigned int view_height):
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

	valarray<int> getViewCoordinates(int mouse_x, int mouse_y) const override;

private:
	valarray<unsigned int> getCurrentViewCoords(int mouse_x, int mouse_y) const override
	{
		auto ret = XY<unsigned int>(mouse_x, mouse_y) / view_size;
		return ret;
	}

	ALLEGRO_BITMAP * buffer;
	AllegroCrosshair normal_crosshair;
	AllegroCrosshair focused_crosshair;
};


class AllegroUI : public GenericUI
{
public:
	AllegroUI(Sensitivity * sensitivities):
		GenericUI(sensitivities), screen_buffer(nullptr), display(nullptr), events(nullptr)
	{}
	~AllegroUI() 
	{
		al_destroy_display(display);
		clean();
	}

	virtual void initGfxMode(unsigned int hres, unsigned int vres) override;

	virtual SpecializedMessageService * makeMessageService()
	{
		return new AllegroMessageService();
	}

	virtual ImageGrid * makeImageGrid(unsigned int n_horizontal, unsigned int n_vertical, unsigned int size_width, unsigned int size_height)
	{
		auto ret = new AllegroImageGrid(screen_buffer, n_horizontal, n_vertical, size_width, size_height);
		ret->createCrosshairs();
		return ret;
	}

	void processMouseEvent(ALLEGRO_EVENT & evt);

	void mainLoop() override
	{
		al_set_target_bitmap(screen_buffer);
		events = al_create_event_queue();
		al_register_event_source(events, al_get_keyboard_event_source());
		al_register_event_source(events, al_get_mouse_event_source());

		ALLEGRO_EVENT event;
		while(dont_stop)
		{
			draw();
			al_wait_for_event(events, & event);
			// TODO: Optimize events processing
			// aggregate keypresses and mouse moves

			al_get_mouse_state(& mouse);
			sensitivities->setBoostedStatus();
			message_service->purgeOldMessages();

			processEvent(event);

			al_rest(0.01);
		}
		draw();
		al_destroy_event_queue(events);
	}

	vector<int> processUIControl(vector<int>);
	vector<int> processUserInput(vector<int>);

	void applyMouseDrag(int dx, int dy);
	void applyMouseZoom(int x, int y, int z);
	void processEvent(ALLEGRO_EVENT & event);
	void processMouseDrag(ALLEGRO_EVENT & event);

	void printHelp();

	void clean()
	{
	}

	void draw()
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		stereotuple->blit();

		stereotuple->drawCrosshairs(mouse.x, mouse.y);
		message_service->displayMessages();
		al_flip_display();
	}
private:

	ALLEGRO_DISPLAY * display;
	ALLEGRO_BITMAP * screen_buffer;

	ALLEGRO_EVENT_QUEUE * events;

	ALLEGRO_MOUSE_STATE mouse;

	int dragging;
};
