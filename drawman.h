// vim:ts=4 sts=0 sw=4

#ifndef DRAWMAN_H
#define DRAWMAN_H

//-----------------------------------------------------------------------------++++
// drawman.h

#ifdef _RGRAPHICS_

#ifdef XLIB
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
	COLOR c3DFace,c3DDShadow,c3DShadow,c3DLight,c3DHilight;
	COLOR cBTNText,cHilight,cHilightText,cWindow,cText;
};

#endif // _RGRAPHICS_

//-----------------------------------------------------------------------------++++
/*
class RDrawer
{
public:
	RDrawer() { };
	virtual ~RDrawer() {};
	virtual void Draw(RControl *rc, RGraphics *g);
	virtual void Draw(RControl *rc, bool bAll);
	virtual void GetMinSize(RControl *rc, int &x, int &y);
};*/

#define RDRAWER(name) class name : public RDrawer { public: name () { }; \
	void Draw(RControl *rc, RGraphics *g); \
	void GetMinSize(RControl *rc, int &x, int &y); };
#define RDRAWERX(name) class name : public RDrawer { public: name () { }; \
	void Draw(RControl *rc, RGraphics *g); \
	void GetMinSize(RControl *rc, int &x, int &y); 
#define xx } // vim syntax color fix
#undef xx

// to be moved to appropriate headers (rtk.h, etc) ???
RDRAWER(RdControl)
RDRAWER(RdTBox)
RDRAWER(RdWindow)
RDRAWER(RdTabControl)

RDRAWER(RdButton)
RDRAWER(RdComboBox)
RDRAWER(RdLabel)
RDRAWER(RdMenuButton)
RDRAWER(RdCheckBox)
RDRAWER(RdRadioButton)
RDRAWER(RdToggleButton)
RDRAWER(RdTextEdit)
RDRAWERX(RdTreeView)
void DrawNode(RTreeView *tv,int x,int &y,RTreeNode *n, RGraphics *g); };
RDRAWERX(RdListView)
void DrawNode(RListView *lv,int x,int y,SListNode *n, RGraphics *g); };
RDRAWER(RdToolTip)
RDRAWER(RdEditBox)
RDRAWER(RdRange)
RDRAWER(RdScrollBar)
RDRAWER(RdProgressBar)
RDRAWER(RdStatusBar)

//-----------------------------------------------------------------------------++++

#endif //DRAWMAN_H

