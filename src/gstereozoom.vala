using Gtk;
public void on_main_window_destroy (Window source) {
	/* When window close signal received */
	Gtk.main_quit ();
}


public void chosen_image(Window source)
{
	return;
}


[GtkTemplate (ui = "/org/gnome/gstereozoom/gstereozoom-image_panel.glade")]
public class image_panel : Frame {
}


Grid get_grid(int width, int height)
{
	var ret = new Grid();
	for (int i = 0; i < width; i++)
		ret.insert_column(0);
	for (int i = 0; i < height; i++)
		ret.insert_row(0);
	return ret;
}


int main (string[] args) {
	Gtk.init (ref args);

	var builder = new Builder ();
	/* Getting the glade file */
	builder.add_from_file ("gstereozoom.glade");
	builder.connect_signals (null);
	var window = builder.get_object ("main_window") as Window;
	// var main_grid = builder.get_object ("images_grid") as Grid;
	var main_grid = get_grid(2, 1);
	window.add(main_grid);
	for (int i = 0; i < 2; i++)
	{
		var panel = new image_panel();
		main_grid.attach(panel, i, 0);
		panel.expand = true;
	}
	window.show_all ();
	Gtk.main ();

	return 0;
}
