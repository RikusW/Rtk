// vim:ts=4 sts=0 sw=4

#include <X11/Xlib.h>
#include <stdio.h>

const char *xstr[] =
{
"X0--------------",
"X1--------------",
"KeyPress        ",
"KeyRelease      ",
"ButtonPress     ",
"ButtonRelease   ",
"MotionNotify    ",
"EnterNotify     ",
"LeaveNotify     ",
"FocusIn         ",
"FocusOut        ",
"KeymapNotify    ",
"Expose          ",
"GraphicsExpose  ",
"NoExpose        ",
"VisibilityNotify",
"CreateNotify    ",
"DestroyNotify   ",
"UnmapNotify     ",
"MapNotify       ",
"MapRequest      ",
"ReparentNotify  ",
"ConfigureNotify ",
"ConfigureRequest",
"GravityNotify   ",
"ResizeRequest   ",
"CirculateNotify ",
"CirculateRequest",
"PropertyNotify  ",
"SelectionClear  ",
"SelectionRequest",
"SelectionNotify ",
"ColormapNotify  ",
"ClientMessage   ",
"MappingNotify   ",
"LASTEvent       "
};

extern Display *rdisplay;

static void ShowFocus(Window w)
{
	int rr;
	Window ww;

	XGetInputFocus(rdisplay, &ww, &rr);
	if(w != ww)
		printf(" dd> 0x%x revert = %x\n",(uint)ww,rr);
	else
		printf("\n");
}

static void PrintExtra(XEvent *xev)
{
	switch(xev->type) {
	case ButtonPress:
	case ButtonRelease:
		printf(" %i\n",xev->xbutton.button);
		break;
	case FocusIn:
	case FocusOut:
		ShowFocus(xev->xfocus.window);
		break;
	default:
		printf("\n");
	}
}

void PrintXev(XEvent *xev, bool b)
{
	const char *cc;
	if(xev->xany.send_event) {
		cc = b ? "++>" : "+x>";
	}else{
		cc = b ? "-->" : "-x>";
	}

	if(xev->type == MotionNotify)
		return;
	if(((uint)xev->type) < 35)
		printf("%s 0x%x %s 0x%x", cc,	(uint)xev->xany.window,xstr[xev->type],(uint)xev->xany.serial);
	else
		printf("%s 0x%x %i 0x%x", cc,	(uint)xev->xany.window,xev->type,(uint)xev->xany.serial);

	PrintExtra(xev);
}



/*
#define KeyPress		2
#define KeyRelease		3
#define ButtonPress		4
#define ButtonRelease		5
#define MotionNotify		6
#define EnterNotify		7
#define LeaveNotify		8
#define FocusIn			9
#define FocusOut		10
#define KeymapNotify		11
#define Expose			12
#define GraphicsExpose		13
#define NoExpose		14
#define VisibilityNotify	15
#define CreateNotify		16
#define DestroyNotify		17
#define UnmapNotify		18
#define MapNotify		19
#define MapRequest		20
#define ReparentNotify		21
#define ConfigureNotify		22
#define ConfigureRequest	23
#define GravityNotify		24
#define ResizeRequest		25
#define CirculateNotify		26
#define CirculateRequest	27
#define PropertyNotify		28
#define SelectionClear		29
#define SelectionRequest	30
#define SelectionNotify		31
#define ColormapNotify		32
#define ClientMessage		33
#define MappingNotify		34
#define LASTEvent		35
*/


/*	switch(x->type) {
	case KeyPress: break;
	case KeyRelease: break;
	case ButtonPress: break;
	case ButtonRelease: break;
	case MotionNotify: break;
	case EnterNotify: break;
	case LeaveNotify: break;
	case FocusIn: break;
	case FocusOut: break;
	case KeymapNotify: break;
	case Expose: break;
	case GraphicsExpose: break;
	case NoExpose: break;
	case VisibilityNotify: break;
	case CreateNotify: break;
	case DestroyNotify: break;
	case UnmapNotify: break;
	case MapNotify: break;
	case MapRequest: break;
	case ReparentNotify: break;
	case ConfigureNotify: break;
	case ConfigureRequest: break;
	case GravityNotify: break;
	case ResizeRequest: break;
	case CirculateNotify: break;
	case CirculateRequest: break;
	case PropertyNotify: break;
	case SelectionClear: break;
	case SelectionRequest: break;
	case SelectionNotify: break;
	case ColormapNotify: break;
	case ClientMessage: break;
	case MappingNotify: break;
	case LASTEvent: break;
	}*/

