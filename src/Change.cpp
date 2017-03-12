#include <allegro.h>

#include "Change.h"


void ChangeOffset::transformView(ViewWithRectangle * subject) const
{
	subject->changePosition(shift);
}


void ChangeZoomViewCentered::transformView(ViewWithRectangle * subject) const
{
	subject->changeZoom(zoom_change, subject->getViewCenteredPivot());
}


void ChangeSetViewToZoom::transformView(ViewWithRectangle * subject) const
{
	subject->setZoom(new_zoom, subject->getViewCenteredPivot());
}


void ChangeSetViewToSeeEverything::transformView(ViewWithRectangle * subject) const
{
	subject->setToJustSeeEverything();
}


void ChangeSetViewToOriginNoZoom::transformView(ViewWithRectangle * subject) const
{
	subject->setZoom(1, XY<double>());
	subject->setPosition(XY<double>(0, 0));
}
