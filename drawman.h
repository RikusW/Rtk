// vim:ts=4 sts=0 sw=4

#ifndef DRAWMAN_H
#define DRAWMAN_H

//-----------------------------------------------------------------------------++++
// drawman.h

#ifdef _RGRAPHICS_

//XXX relocate ???
void DrButton(RControl *rc, RGraphics *g, int left, int top, int right, int bottom, bool b);
void DrButton(RControl *rc, RGraphics *g, bool b);
void DrawFrame(RControl *rc, RGraphics *g);
void DrFocus(RControl *rc, RGraphics *g, int left, int top, int right, int bottom);


#ifdef WIN32
#define NO_STRICT
#include <windows.h>
#endif


#ifdef XLIB

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define PS_SOLID LineSolid
#define PS_DOT   LineOnOffDash
typedef unsigned long COLOR;
typedef unsigned char BYTE;
#else
typedef COLORREF COLOR;
#endif

class RGraphics
{
public:
	RGraphics() { InitColor(); };
	~RGraphics() {};
#ifndef XLIB
	int Start(RWND w);
#else
	int Start(Display *d, RWND w);
#endif
	void Stop();
	void InitColor();

	void SetFg(COLOR c, int w = 1, int style = PS_SOLID);
	void SetBg(COLOR c);
	void SetBk(COLOR c);
	void SetBgHollow();
	void SetTextColor(COLOR c, COLOR b);
	void Pixel(int x, int y);
	void Pixel(int x, int y, COLOR c);
	void Line(int x, int y, int a, int b);
	void Line(int x, int y, int a, int b, int c, int d);
	void MoveTo(int x, int y);
	void LineTo(int x, int y);
	void Rectangle(int l, int t, int r, int b);
	void FillRect(COLOR c,int x, int y, int w, int h);
	void FillRectI(int x, int y, int w, int h);
	void Ellipse(int x, int y, int w, int h);
	void TextOut(int x, int y, const char *, int len);
	COLOR rgb(BYTE r, BYTE g, BYTE b);
	void DrawControl(RControl *rc, bool bAll);
	void StartDraw(RControl *rc, bool bAll);

#ifdef WIN32
	HDC dc;
	HPEN hp;
	HBRUSH hb;
	HFONT hf;
	HWND hwnd; //XXX

	int w,st;
	COLOR fg,bg;
#endif
#ifdef XLIB
	int cx,cy;
	GC gc;
	Window win; //XXX
	Display *display;
#endif
	COLOR c3DFace,c3DDShadow,c3DShadow,c3DLight,c3DHilight,cScrollbar;
	COLOR c3DText,cGrayText,cSelect,cSelectText,cWindow,cWinText;
};

#endif // _RGRAPHICS_

//-----------------------------------------------------------------------------++++

#endif //DRAWMAN_H

