#ifndef PAIR_H_
#define PAIR_H_

#include <vector>
using std::vector;
#include <string>
using std::string;

#include "stereozoom2.h"
#include "Image.h"

/// class Image_group represents the group of examined images
/**
 * It takes care mainly about user input
 */
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

#endif /*PAIR_H_*/
