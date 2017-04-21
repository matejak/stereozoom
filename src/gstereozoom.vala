using Gtk;

public void on_main_window_destroy (Window source) {
	/* When window close signal received */
	Gtk.main_quit ();
}


public void chosen_image(Window source) {
	return;
}


double get_scale_factor(int src_width, int src_height, int dest_width, int dest_height) {
	double scale = double.min(dest_width / (double)src_width, dest_height / (double)src_height);
	return scale;
}


Gdk.Pixbuf scale_pixbuf(Gdk.Pixbuf pixbuf, int dest_width, int dest_height) {
	int pixbuf_height = pixbuf.height;
	int pixbuf_width = pixbuf.width;
	double scale_factor = get_scale_factor(pixbuf_width, pixbuf_height, dest_width, dest_height);
	scale_factor *= 0.95;
	return pixbuf.scale_simple(
		(int)(pixbuf_width * scale_factor), 
		(int)(pixbuf_height * scale_factor),
		Gdk.InterpType.BILINEAR);
}


[GtkTemplate (ui = "/org/gnome/gstereozoom/gstereozoom-image_panel.glade")]
public class image_panel : Frame {
	[GtkChild]
	private Image image;

	private Gdk.Pixbuf bitmap;

	[GtkCallback]
	private void chosen_image(FileChooserButton chooser) {
		var fname = chooser.get_filename();
		this.loadImage(fname);
	}

	[GtkCallback]
	private void frame_size_allocated() {
		if (this.bitmap == null)
			return;
		scaleImageSoItFits();
	}
	
	private void loadImage(string fname) {
		this.bitmap = new Gdk.Pixbuf.from_file(fname);
		this.scaleImageSoItFits();
	}

	private void scaleImageSoItFits() {
		Allocation allocation;
		this.image.get_allocation(out allocation);
		var pixbuf = scale_pixbuf(this.bitmap, allocation.width, allocation.height);
		this.image.set_from_pixbuf(pixbuf);
	}
}


Grid get_grid(int width, int height) {
	var ret = new Grid();
	for (int i = 0; i < width; i++)
		ret.insert_column(0);
	for (int i = 0; i < height; i++)
		ret.insert_row(0);
	ret.set_row_homogeneous(true);
	ret.set_column_homogeneous(true);
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
