using Gtk;
public void on_main_window_destroy (Window source) {
	/* When window close signal received */
	Gtk.main_quit ();
}

[GtkTemplate (ui = "/org/gnome/gstereozoom/gstereozoom-image_panel.glade")]
public class image_panel : Frame {
}

int main (string[] args) {
	Gtk.init (ref args);

	var builder = new Builder ();
	/* Getting the glade file */
	builder.add_from_file ("gstereozoom.glade");
	builder.connect_signals (null);
	var window = builder.get_object ("main_window") as Window;
	var main_grid = builder.get_object ("images_grid") as Grid;
	main_grid.attach(new image_panel(), 0, 0);
	main_grid.attach(new image_panel(), 0, 1);
	window.show_all ();
	Gtk.main ();

	return 0;
}
