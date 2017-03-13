#ifndef CHANGE_H_
#define CHANGE_H_

#include "Rectangle.h"

class Change
{
public:
	virtual void transformView(ViewWithRectangle * subject) const = 0;
};


class ChangeDrag: public Change
{
public:
	ChangeDrag(double shift_x, double shift_y):
		shift(XY<double>(shift_x, shift_y)) {}
	virtual void transformView(ViewWithRectangle * subject) const;
private:
	valarray<double> shift;
};


class ChangeOffset: public Change
{
public:
	ChangeOffset(double shift_x, double shift_y):
		shift(XY<double>(shift_x, shift_y)) {}
	virtual void transformView(ViewWithRectangle * subject) const;
private:
	valarray<double> shift;
};


class ChangeZoom: public Change
{
public:
	ChangeZoom(double change):
		zoom_change(change) {}
	virtual void transformView(ViewWithRectangle * subject) const = 0;
protected:
	double zoom_change;
};


class ChangeZoomViewCentered: public ChangeZoom
{
public:
	ChangeZoomViewCentered(double change):
		ChangeZoom(change) {}
	virtual void transformView(ViewWithRectangle * subject) const;
};


class ChangeZoomViewGeneral: public ChangeZoom
{
public:
	ChangeZoomViewGeneral(double change, valarray<int> position_in_view):
		ChangeZoom(change), position_in_view(XY<double>(position_in_view[X], position_in_view[Y])) {}
	virtual void transformView(ViewWithRectangle * subject) const;
protected:
	valarray<double> position_in_view;
};


class ChangeSetView: public Change
{
public:
	virtual void transformView(ViewWithRectangle * subject) const = 0;
private:
};


class ChangeSetViewToFit: public ChangeSetView
{
public:
	virtual void transformView(ViewWithRectangle * subject) const;
private:
};


class ChangeSetViewToZoom: public ChangeSetView
{
public:
	ChangeSetViewToZoom(double new_zoom):
		new_zoom(new_zoom) {}
	virtual void transformView(ViewWithRectangle * subject) const;
private:
	double new_zoom;
};


class ChangeSetViewToSeeEverything: public ChangeSetView
{
public:
	virtual void transformView(ViewWithRectangle * subject) const;
};


class ChangeSetViewToOriginNoZoom: public ChangeSetView
{
public:
	virtual void transformView(ViewWithRectangle * subject) const;
private:
};


#endif /* CHANGE_H_ */
