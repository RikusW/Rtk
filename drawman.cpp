// vim:ts=4 sts=0 sw=4


#include "RtkAll.h"

#ifdef WIN32
#define NO_STRICT
#include <windows.h>
#endif

#ifdef XLIB
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#endif

#define _RGRAPHICS_
#include "drawman.h"

//#define FRAMES
//#define CLIPTEST

//-----------------------------------------------------------------------------++++
// GDI Graphics wrapper class

void DrawFrame(RControl *rc, RGraphics *g);
RGraphics rg;

void RDrawer::Draw(RControl *rc, bool bAll)
{
	rg.StartDraw(rc,bAll);
}

void RDrawer::Draw(RControl *rc, RGraphics *g)
{
	g->FillRect(g->rgb(255,0,255),rc->left,rc->top,rc->width,rc->height);
}

void RDrawer::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

#ifdef WIN32

/*RGraphics::RGraphics(){}*/

int RGraphics::Start(RWND wnd)
{
	if(!(hwnd = wnd))
		return 1;

	if(!(dc = GetDC(hwnd)))
		return 1;

	hp,hb,hf = NULL;

	hf = (HFONT)SelectObject(dc,GetStockObject(ANSI_FIXED_FONT));

	w = 1;
	bg = 0;
	st = PS_SOLID;
	fg = 0xFFFFFF;

	return 0;
}

void RGraphics::InitColor()
{
	c3DHilight	= GetSysColor(COLOR_3DHILIGHT);
	c3DLight	= GetSysColor(COLOR_3DLIGHT);
	c3DFace		= GetSysColor(COLOR_3DFACE);
	c3DDShadow	= GetSysColor(COLOR_3DDKSHADOW);
	c3DShadow	= GetSysColor(COLOR_3DSHADOW);

	c3DText		= GetSysColor(COLOR_BTNTEXT);
	cGrayText	= GetSysColor(COLOR_GRAYTEXT);
	cScrollbar	= GetSysColor(COLOR_SCROLLBAR);

	cWindow		= GetSysColor(COLOR_WINDOW);
	cWinText	= GetSysColor(COLOR_WINDOWTEXT);
	cSelect		= GetSysColor(COLOR_HIGHLIGHT);
	cSelectText	= GetSysColor(COLOR_HIGHLIGHTTEXT);
}

void RGraphics::Stop()
{
	if(hf) SelectObject(dc,hf); hf = NULL;
	ReleaseDC(hwnd,dc);

/*	if(hp)
		if(!
		DeleteObject(SelectObject(dc,hp)))
			MessageBeep(0);
		//	MessageBox(0,"free STOP pen fail","ERROR",MB_OK);
	hp = NULL;
	if(hb)
		if(!
			DeleteObject(SelectObject(dc,hb)))
				MessageBeep(0);
		//	MessageBox(0,"free STOP brush fail","ERROR",MB_OK);
	hb = NULL;*/
}

void RGraphics::SetFg(COLOR c, int wi, int style)
{
	wi = w;
	fg = c;
	st = style;
	return;
/*	HPEN p = (HPEN)SelectObject(dc,CreatePen(style, w, c));
	if(hp)
		if(!DeleteObject(p))
			MessageBox(0,"free pen fail","ERROR",MB_OK);
	else
		hp = p;
//	SetBkColor(dc,c);*/
}

void RGraphics::SetBg(COLOR c)
{
	bg = c;
	return;
/*	HBRUSH b = (HBRUSH)SelectObject(dc,CreateSolidBrush(c));
	if(hb)
		if(!DeleteObject(b))
			MessageBox(0,"free brush fail","ERROR",MB_OK);
	else
		hb = b;*/
}

void RGraphics::SetBgHollow()
{
/*	return;
	HBRUSH b = (HBRUSH)SelectObject(dc,GetStockObject(HOLLOW_BRUSH));
	if(hb)
		if(!DeleteObject(b))
			MessageBox(0,"free brush fail","ERROR",MB_OK);
	else
		hb = b;*/
}

void RGraphics::SetTextColor(COLOR c, COLOR b)
{
	::SetTextColor(dc,c);
	::SetBkColor(dc,b);
}

void RGraphics::SetBk(COLOR c)
{
	SetBkColor(dc,c);
}

void RGraphics::Pixel(int x, int y)
{
	SetPixelV(dc,x,y,fg);
}

void RGraphics::Pixel(int x, int y, COLOR c)
{
	SetFg(c); // make same as Xlib
	SetPixelV(dc,x,y,c);
}

void RGraphics::Line(int x, int y, int a, int b)
{
	MoveTo(x,y);
	LineTo(a,b);
}

void RGraphics::Line(int x, int y, int a, int b, int c ,int d)
{
	MoveTo(x,y);
	LineTo(a,b);
	LineTo(c,d);
}

void RGraphics::MoveTo(int x, int y)
{
	::MoveToEx(dc,x,y,0);
}

void RGraphics::LineTo(int x, int y)
{
	HPEN p = CreatePen(st, w, fg);
	HPEN op = (HPEN)SelectObject(dc,p);
	::LineTo(dc,x,y);
	SetPixelV(dc,x,y,fg); //Bugfix...
	SelectObject(dc,op);
	DeleteObject(p);
}

void RGraphics::Rectangle(int l, int t, int r, int b)
{
	::MoveToEx(dc,l,t,0);
	::LineTo(dc,r,t);
	::LineTo(dc,r,b);
	::LineTo(dc,l,b);
	::LineTo(dc,l,t);
}

void RGraphics::FillRect(COLOR c,int x, int y, int w, int h)
{
	HBRUSH b = CreateSolidBrush(c);
	HBRUSH ob = (HBRUSH)SelectObject(dc,b);
	PatBlt(dc,x,y,w,h,PATCOPY);
	SelectObject(dc,ob);
	DeleteObject(b);
}

void RGraphics::FillRectI(int x, int y, int w, int h)
{
	HBRUSH b = CreateSolidBrush(bg);
	HBRUSH ob = (HBRUSH)SelectObject(dc,b);
	PatBlt(dc,x,y,w,h,DSTINVERT);
	SelectObject(dc,ob);
	DeleteObject(b);
}

void RGraphics::Ellipse(int x, int y, int ww, int h)
{
	HPEN p = CreatePen(st, w, fg);
	HPEN op = (HPEN)SelectObject(dc,p);

	HBRUSH b = (HBRUSH)SelectObject(dc,GetStockObject(HOLLOW_BRUSH));
	::Ellipse(dc,x,y,ww,h);
	SelectObject(dc,b);

	SelectObject(dc,op);
	DeleteObject(p);
}

void RGraphics::TextOut(int x, int y, const char *c, int len)
{
	::SetBkMode(dc,TRANSPARENT);
	::TextOut(dc,x,y,c,len);
}

COLOR RGraphics::rgb(BYTE r, BYTE g, BYTE b)
{
	return ((b << 16) & 0xFF0000) | ((g << 8) & 0xFF00) | r;
}


void RGraphics::DrawControl(RControl *rc, bool bAll)
{
	if(rc->bHidden)
		return;

	// setup the control clipping rectangle
	Rect r;
	r.left = rc->left;	r.right = rc->right;//+1;
	r.top = rc->top;	r.bottom = rc->bottom;//+1;
	if(rc->parent)
		rc->parent->ClipRect(r);
	
	if(r.left > r.right || r.top > r.bottom)
		return;
	
	// check this +1 closely...
	HRGN rgn = CreateRectRgn(r.left,r.top,r.right+1,r.bottom+1);
	SelectClipRgn(dc,rgn);

	// clip the children and then draw the container
	if(rc->pDrawer) {
		for(RControl *c = rc->GetChild(); c; c = c->next) {
			if((!c->bHidden) && c->pDrawer) {
				ExcludeClipRect(dc,c->left,c->top,c->right+1,c->bottom+1);
			}
		}

		rc->pDrawer->Draw(rc,this);
	}
	for(RControl *c = rc->GetChild(); c; c = c->next) {
		if(c->frame)
			DrawFrame(c,this);
	}
	SelectClipRgn(dc,NULL);
	if(rgn) DeleteObject(rgn);

	// draw all children only if needed
	if(bAll) {
		for(RControl *c = rc->GetChild(); c; c = c->next) {
			DrawControl(c,bAll);
		}
	}
}

void RGraphics::StartDraw(RControl *rc, bool bAll)
{
	if(Start(rc->wnd->hwnd))
		return;
	DrawControl(rc,bAll);
	Stop();
}

#endif
// GDI Graphics wrapper class
//-----------------------------------------------------------------------------++++
// Xlib Graphics wrapper class
#ifdef XLIB

Font font; //XXX bad


int RGraphics::Start(Display *d, RWND w)
{
	display = d;
	win = w;

	//-----------------------
	// Get the GC

	XGCValues values;
	unsigned long vm = 0;
	static char dd[] = {1, 3};
	int ddlen = sizeof(dd);

	if(!win) {
		printf("null Window....\n");
		return 1;
	}

	gc = XCreateGC(display, win, vm, &values);

//	XFontStruct *font_info;
	const char *fontname = "8x13";
//	char *fontname =
//	"-misc-fixed-medium-r-normal--13-100-100-100-c-70-iso8859-1";
//	"-misc-fixed-bold-r-normal--13-100-100-100-c-70-iso8859-1";



//	if((font_info=XLoadQueryFont(display,fontname)) == NULL) { segfaults... ?!!
	if((font = XLoadFont(display,fontname)) == 0) {
		printf("stderr, basicwin: cannot open 8x13 font\n");
		exit(-1);
	}
	XSetFont(display, gc, font);
//	font_info = XQueryFont(display,font);   //*/



	//XSetSubwindowMode(display, gc, IncludeInferiors); no no...

	XSetForeground(display, gc, 0);
	XSetBackground(display, gc, 0xFFFF);
	XSetLineAttributes(display, gc, 1, LineSolid, CapButt, JoinMiter);
	XSetDashes(display, gc, 0, dd, ddlen);

	return 0;
}

void RGraphics::InitColor()
{
	c3DHilight	= rgb(240,240,208);
	c3DLight	= rgb(224,224,160);
	c3DFace		= rgb(224,224,160);
	c3DShadow	= rgb(192,192,64);
	c3DDShadow	= 0;

	c3DText		= 0;
	cGrayText	= rgb(192,192,64);
	cScrollbar	= rgb(240,240,208);

	cWindow		= rgb(240,244,200);
	cWinText	= 0;
	cSelect		= rgb(128,128,0);
	cSelectText	= rgb(255,255,255);
}

void RGraphics::Stop()
{
	XFreeGC(display,gc);
	XUnloadFont(display,font);
}

void RGraphics::SetFg(COLOR c, int wi, int style)
{
	XSetForeground(display,gc,c);
	XSetLineAttributes(display, gc, wi, style, CapProjecting, JoinMiter);
	return;
}

void RGraphics::SetBg(COLOR c)
{
	XSetBackground(display,gc,c);
}

void RGraphics::SetBgHollow()
{
}

void RGraphics::SetTextColor(COLOR c, COLOR b)
{
	XSetForeground(display,gc,c);
	XSetBackground(display,gc,b);
}

void RGraphics::SetBk(COLOR c)
{
	XSetBackground(display,gc,c);
}

void RGraphics::Pixel(int x, int y)
{
	XDrawPoint(display,win,gc,x,y);
}

void RGraphics::Pixel(int x, int y, COLOR c)
{
	SetFg(c);	
	XDrawPoint(display,win,gc,x,y);
}

void RGraphics::Line(int x, int y, int a, int b)
{
	XDrawLine(display,win,gc,x,y,a,b);
}

void RGraphics::Line(int x, int y, int a, int b, int c ,int d)
{
	XDrawLine(display,win,gc,x,y,a,b);
	XDrawLine(display,win,gc,a,b,c,d);
}

void RGraphics::MoveTo(int x, int y)
{
	cx = x; cy = y;
}

void RGraphics::LineTo(int x, int y)
{
	XDrawLine(display,win,gc,cx,cy,x,y);
	cx = x; cy = y;
}

void RGraphics::Rectangle(int l, int t, int r, int b)
{
	MoveTo(l,t);
	LineTo(r,t);
	LineTo(r,b);
	LineTo(l,b);
	LineTo(l,t);
}

void RGraphics::FillRect(COLOR c,int x, int y, int w, int h)
{
	XSetForeground(display,gc,c);//rgb(200,200,0));
	XFillRectangle(display, win, gc, x, y, w, h);
}

void RGraphics::FillRectI(int x, int y, int w, int h)
{
	XSetForeground(display,gc,0xFFFF);
	XSetFunction(display, gc, GXxor);
	XFillRectangle(display, win, gc, x, y, w, h);
	XSetFunction(display, gc, GXcopy);
}

void RGraphics::Ellipse(int x, int y, int w, int h)
{
	w -= x;
	h -= y;
	XDrawArc(display, win, gc, x, y, w, h, 0, 23040);
}

void RGraphics::TextOut(int x, int y, const char *c, int len)
{
	y+= 10;
//	XSetForeground(display,gc,rgb(0,0,0));
	XDrawString(display, win, gc, x, y, c, len);
}

COLOR RGraphics::rgb(BYTE r, BYTE g, BYTE b)
{
	// 16bpp 565 rgb
	//return ((((r) << 8) & 0xF800) | (((g) << 3) & 0x07E0) | (((b) >> 3) & 0x001F));
	// 24bpp 888 rgb
	return ((((r) << 16) & 0xFF0000) | (((g) << 8) & 0xFF00) | ((b) & 0xFF));
}

void RGraphics::DrawControl(RControl *rc, bool bAll)
{
	if(rc->bHidden)
		return;

	// setup the control clipping rectangle
	Rect rr;
	rr.left = rc->left;	rr.right = rc->right;//+1;
	rr.top = rc->top;	rr.bottom = rc->bottom;//+1;
	if(rc->parent)
		rc->parent->ClipRect(rr);

	if(rr.left > rr.right || rr.top > rr.bottom)
		return;

	Region rgn = XCreateRegion();
	XRectangle r;
	r.x = rr.left;
	r.y = rr.top;
	r.width = rr.right - rr.left + 1;
	r.height= rr.bottom - rr.top + 1;
	XUnionRectWithRegion(&r,rgn,rgn);

	// clip the children and then draw the container
	if(rc->pDrawer) {
		if(rc->GetChild()) {
			Region rx = XCreateRegion();
			for(RControl *c = rc->GetChild(); c; c = c->next) {
				if((!c->bHidden) && c->pDrawer) {
					r.x = c->left;
					r.y = c->top;
					r.width = c->right - c->left + 1;
					r.height= c->bottom - c->top + 1;
					XUnionRectWithRegion(&r,rx,rx);
				}
			}
			XSubtractRegion(rgn,rx,rgn);
			XSetRegion(display,gc,rgn);
			XDestroyRegion(rgn);
			XDestroyRegion(rx);
		}else{
			XSetRegion(display,gc,rgn);
			XDestroyRegion(rgn);
		}

		rc->pDrawer->Draw(rc,this);
	}
	for(RControl *c = rc->GetChild(); c; c = c->next) {
		if(c->frame != F_NONE)
			DrawFrame(c,this);
	}

	// draw all children only if needed
	if(bAll) {
		for(RControl *c = rc->GetChild(); c; c = c->next) {
			DrawControl(c,bAll);
		}
	}
}

extern Display *rdisplay;

void RGraphics::StartDraw(RControl *rc, bool bAll)
{
	if(Start(rdisplay, rc->wnd->hwnd))
		return;
	DrawControl(rc,bAll);
	Stop();
}

#endif
// Xlib Graphics wrapper class
//-----------------------------------------------------------------------------++++
// Load colors
//Added 201311 -- integrate fully later...

COLOR GetRGB(RConfigNode *n, const char *s)
{
	RString t,u;
	RConfigNode *m;
	u8 r,g,b;

	m = n->GetNode(s);
	if(!m) {
//		printf("Failed to find color -> %s\n",s);
		return 0;
	}

	u = m->line;
//	printf("GetRGB line -> %s\n",m->line);

	t.Split(u,'=');
	t.Split(u,',');
	r = t;
	t.Split(u,',');
	g = t;
	t.Split(u,',');
	b = t;

//	printf("GetRGB -> %i %i %i\n",r,g,b);

	return rg.rgb(r,g,b);
}

void SetupColors(RConfigNode *n)
{
	if(!n) {
		return;
	}

	rg.c3DHilight	= GetRGB(n,"3DHilight");
	rg.c3DLight		= GetRGB(n,"3DLight");
	rg.c3DFace		= GetRGB(n,"3DFace");
	rg.c3DShadow	= GetRGB(n,"3DShadow");
	rg.c3DDShadow	= GetRGB(n,"3DDkShadow");

	rg.c3DText		= GetRGB(n,"3DText");
	rg.cGrayText	= GetRGB(n,"GrayText");
	rg.cScrollbar	= GetRGB(n,"Scrollbar");

	rg.cWindow		= GetRGB(n,"Window");
	rg.cWinText		= GetRGB(n,"WindowText");
	rg.cSelect		= GetRGB(n,"Selected");
	rg.cSelectText	= GetRGB(n,"SelectedText");
	
	//TtText ??
	//TtWindow??

	//later ??
	//Menu -
	//MenuText -

	//MenuSelected ??
	//MenuBar
	//AppWorkspace --MDI background
	//HotTrackingColor?? sometimes == selected ?? not used ???


/*
	rg.c3DFace		= rgb(224,224,160);
	rg.c3DShadow	= rgb(192,192,64);
	rg.c3DDShadow	= 0;
	rg.c3DLight		= rgb(224,224,160);
	rg.c3DHilight	= rgb(240,240,208);

	rg.c3DText		= 0;
	rg.cSelect		= rgb(128,128,0); //Selected
	rg.cWindow		= rgb(240,244,200);
	rg.cSelectText	= rgb(255,255,255);
	rg.cWinText		= 0;
*/
}

//-----------------------------------------------------------------------------++++
// Drawing Manager

void DrButton(RControl *rc, RGraphics *g, int left, int top, int right, int bottom, bool b)
{
	COLOR cTopO,cTopI,cBotO,cBotI;

	if(b)
	{
		cTopI = g->c3DDShadow;
		cTopO = g->c3DShadow;
		cBotI = g->c3DLight; //201311 g->rgb(214,214,156); //g->c3DLight;
		cBotO = g->c3DHilight;
	}else{
		cTopI = g->c3DHilight;
		cTopO = g->c3DLight; //201311 g->rgb(214,214,156); //g->c3DLight;
		cBotI = g->c3DShadow;
		cBotO = g->c3DDShadow;
	}

	g->SetFg(cTopO); g->Line(left  ,bottom  ,left   ,top     ,right  ,top  );
	g->SetFg(cTopI); g->Line(left+1,bottom-1,left+1 ,top+1   ,right-1,top+1);
	g->SetFg(cBotI); g->Line(left+2,bottom-1,right-1,bottom-1,right-1,top+2);
	g->SetFg(cBotO); g->Line(left+1,bottom  ,right  ,bottom  ,right  ,top+1);
}

inline void DrButton(RControl *rc, RGraphics *g, bool b)
{
	DrButton(rc,g,rc->left,rc->top,rc->right,rc->bottom,b);
}

void DrawFrame(RControl *rc, RGraphics *g)
{
	switch(rc->frame) {
	case F_UP:
		DrButton(rc,g,rc->left-2,rc->top-2,rc->right+2,rc->bottom+2,false);
		break;
	case F_DOWN:
		DrButton(rc,g,rc->left-2,rc->top-2,rc->right+2,rc->bottom+2,true);
		break;
//	case F_GROUP: XXX
//		DrButton(rc,g,rc->left-2,rc->top-2,rc->right+2,rc->bottom+2);
//		break;
	}
}

void DrFocus(RControl *rc, RGraphics *g, int left, int top, int right, int bottom) //GGG
{
	if(rc->bFocus) {
		g->SetBk(g->c3DFace); //GGGG
		g->SetFg(g->c3DText, 1, PS_DOT); //GGG
		g->Rectangle(left,top,right,bottom);
	}
}

//-----------------------------------------------------------------------------++++
// RDrawer

void RdControl::Draw(RControl *rc, RGraphics *g)
{
	g->FillRect(g->c3DFace,rc->left,rc->top,rc->width,rc->height);

#ifdef FRAMES
	g->SetFg(g->rgb(255,0,255)); //ff
	for(RControl *c = rc->GetChild(); c; c = c->next) {
		g->Rectangle(c->left-1,c->top-1,c->right+1,c->bottom+1);
	}
#endif
}

void RdControl::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdTBox::Draw(RControl *rc, RGraphics *g)
{
	RTBox *r = (RTBox*)rc;

	//ct g->FillRect(g->rgb(0,200,200),r->left-3,r->top-3,r->width+6,r->height+6);
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);

#ifdef FRAMES
	g->SetFg(g->rgb(0,255,0)); //ff
	for(RControl *c = r->GetChild(); c; c = c->next) {
		g->Rectangle(c->left-1,c->top-1,c->right+1,c->bottom+1);
	}
	g->SetFg(g->rgb(0,255,255)); //ff
	g->Rectangle(r->left,r->top,r->right,r->bottom);
#endif	
}

void RdTBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdWindow::Draw(RControl *rc, RGraphics *g)
{
	RWindow *r = (RWindow*)rc;
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
//	g->SetFg(0);
//	g->Rectangle(r->left,r->top,r->right,r->bottom);
	
#ifdef FRAMES
	g->SetFg(g->rgb(255,255,0)); //ff
	for(RControl *c = r->GetChild(); c; c = c->next) {
		g->Rectangle(c->left-1,c->top-1,c->right+1,c->bottom+1);
	}
#endif
}

void RdWindow::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdTabControl::Draw(RControl *rc, RGraphics *g)
{
//#ifdef CLIPTEST
//	g->FillRect(g->rgb(0xDD,0xAA,0xDD),rc->left,rc->top,rc->width,rc->height);
//#endif
//	DrButton(rc,g,true);
}

void RdTabControl::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdButton::Draw(RControl *rc, RGraphics *g)
{
	RButton *r = (RButton*)rc;
#ifdef CLIPTEST
	g->FillRect(g->rgb(200,0,200),r->left-3,r->top-3,r->width+6,r->height+6);
#else	
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
#endif	
	DrFocus(r,g,r->left+5,r->top+5,r->right-5,r->bottom-4);

	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(255,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+6,r->text,strlen(r->text));
	
	DrButton(r,g,r->bPushed);
}

void RdButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdComboBox::Draw(RControl *rc, RGraphics *g)
{
	RComboBox *r = (RComboBox*)rc;
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
//	DrFocus(r,g,r->left+5,r->top+5,r->right-5,r->bottom-4);

	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(255,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+6,r->text,strlen(r->text));

	g->SetTextColor(g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->right-16,r->top+6,"V",1);

	DrButton(r,g,0); //2013 r->bPushed);
}

void RdComboBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdLabel::Draw(RControl *rc, RGraphics *g)
{
	RLabel *r = (RLabel*)rc;

	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+2,r->text,strlen(r->text));
}

void RdLabel::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 16;
}

//-----------------

void RdMenuButton::Draw(RControl *rc, RGraphics *g)
{
	RMenuButton *r =(RMenuButton*)rc;

	if(r->IsHit()) {
		g->FillRect(g->cSelect,r->left,r->top,r->width,r->height);
		g->SetTextColor(g->cSelectText,g->cSelect);
	}else{
		g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
		g->SetTextColor(g->cWinText,g->c3DFace);
	}
	if(r->bDisabled) {
		g->SetTextColor(g->cGrayText,g->c3DFace);
	}
//	g->SetTextColor(g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+6,r->text,strlen(r->text));

//	DrFocus(r,g,r->left+5,r->top+5,r->right-5,r->bottom-4);
	//-------------

	if(r->pm) {
#ifndef XLIB //TODO
		Pie(g->dc,r->right - 20,r->top,r->right,r->bottom, //GGG
			r->right - 20,r->top,r->right - 20,r->bottom);
#else
		int h;//r->bottom - r->top; h++;
		h = r->top + 10;
		g->SetFg(g->rgb(128,128,128),10,PS_SOLID);
		g->Line(r->right - 15,h,r->right-5,h);
#endif
	}
}

void RdMenuButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdCheckBox::Draw(RControl *rc, RGraphics *g)
{
	RCheckBox *r = (RCheckBox*)rc;
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);

	DrButton(r,g,r->left + 6,r->top + 6,r->left + r->height - 6,
				r->bottom - 5, 1); //r->bChecked);

	if(r->bChecked) {
		g->FillRect(g->cWinText,r->left+10,r->top+10,r->height-19,r->height-19);
	}

	int left = r->left + r->height + 2;
	DrFocus(r,g,left+3,r->top+3,r->right-3,r->bottom-2);

	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(255,0,0),g->c3DFace);
	g->TextOut(left+6,r->top+6,r->text,strlen(r->text));
}

void RdCheckBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRadioButton::Draw(RControl *rc, RGraphics *g)
{
	RRadioButton *r = (RRadioButton*)rc;
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);

	int	bleft = r->left + 6,
		bright = r->left + r->height - 6,
		btop = r->top + 6,
		bbottom = r->bottom - 6;

	// The circle
	g->SetBgHollow();
	g->SetFg(g->c3DDShadow);
	g->Ellipse(bleft+1,btop+1,bright,bbottom);
	g->SetFg(g->c3DShadow);
	g->Ellipse(bleft,btop,bright-1,bbottom-1);

	// The dot
	if(r->bChecked) {
		g->SetFg(g->cWinText);
		g->SetBg(g->cWinText);
		g->Ellipse(bleft+4,btop+4,bright-4,bbottom-4);
	}

	int left = r->left + r->height;
	DrFocus(r,g,left+3,r->top+3,r->right-3,r->bottom-2);

	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(255,0,0),g->c3DFace);
	g->TextOut(left+6,r->top+6,r->text,strlen(r->text));
}

void RdRadioButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdToggleButton::Draw(RControl *rc, RGraphics *g)
{
	RToggleButton *r = (RToggleButton*)rc;
#ifdef CLIPTEST
	g->FillRect(g->rgb(20,20,20),r->left-3,r->top-3,r->width+6,r->height+6);
#else	
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
#endif	
	DrFocus(r,g,r->left+5,r->top+5,r->right-5,r->bottom-4);

	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(255,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+6,r->text,strlen(r->text));
	
	DrButton(r,g,r->bChecked || r->bPushed);
}

void RdToggleButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 80;
	y = 24;
}

//-----------------

void RdTextEdit::Draw(RControl *rc, RGraphics *g)
{
	RTextEdit *te = (RTextEdit*)rc;

	g->SetFg(0);
#ifdef CLIPTEST
	g->FillRect(g->rgb(200,0,0),rc->left-3,rc->top-3,rc->width+6,rc->height+6);
#else
	g->FillRect(g->cWindow,rc->left,rc->top,rc->width,rc->height);
#endif

	//////----------------------------------------
	// draw text
	//int yy = ((te->height-4)-(te->y margin << 1)) / 13;
	int yy = (te->height-4) / 13;

	STextLine *st = te->stext;

	g->SetTextColor(g->cWinText,g->cWindow);

	Rect r;
	r.top = te->top+2; r.bottom = r.top + 13;
	r.left = te->left+2; r.right = te->right-2;

	for(int i = 0; st;) {
		g->TextOut(r.left,r.top,st->line,strlen(st->line));
//-		ExtTextOut(dc,r.left,r.top,ETO_CLIPPED,&r,
//-					st->line,strlen(st->line),NULL);

		//cursor XXX
		if(i == te->ky && te->bFocus) {
			g->Line(r.left + 8*te->kx,r.top,r.left + 8*te->kx,r.bottom);
		}

		//selection drawing
		if(st->iline >= te->s_by && st->iline <= te->s_ey) {
			int	L,R,max = r.left + 8 * strlen(st->line);

			//square sel
			if(te->bSquare) {
				L = r.left + 8 * te->s_bx;
				if(L > max) L = max;
				R = r.left + 8 * te->s_ex;
				if(R > max) R = max;
			}else{
				L = r.left;
				R = max;
				if(st->iline == te->s_by) {
					L = r.left + 8 * te->s_bx;
					if(L > max) L = max;
				}
				if(st->iline == te->s_ey) {
					R = r.left + 8 * te->s_ex;
					if(R > max) R = max;
				}
			}
			g->FillRectI(L, r.top, R - L, r.bottom - r.top + 1);
#ifdef XLIB
			g->SetTextColor(g->cWinText,g->c3DFace);
#endif
//-			PatBlt(dc, L, r.top, R - L, r.bottom - r.top + 1, DSTINVERT);
		}

		st = st->next;
		r.top += 13; r.bottom += 13;
		if(i++ >= yy)
			goto jend;
	}

	// ending dash
	g->Line(r.left + 2, r.bottom - 7, r.left + 20, r.bottom - 7);
	g->Line(r.left + 2, r.bottom - 6, r.left + 20, r.bottom - 6);
jend:	{}
}

void RdTextEdit::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdTreeView::DrawNode(RTreeView *tv,int x,int &y,RTreeNode *n, RGraphics *g)
{
	if(!n) return;
	int i,s = n->GetDepth();
	int stack[40];
	for(i = 0; i < 30; stack[i++] = tv->top-5);


	g->SetFg(0);
	g->SetTextColor(g->rgb(0,0,0),g->cWindow);
	while(n) {
		//TODO  draw only when node is visible
/*		if(y > tv->bottom) {	// beyond bottom ?
			do {
				s--;
				g->Line(x+s*10+5,stack[s]+5,x+s*10+5,y+8);
			}while(s);
			return;
		}*/

		if(!n->next) { //draw vertical lines
			s--;
			g->Line(x+s*10+5,stack[s]+5,x+s*10+5,y+6);
			s++;
		}

		if(tv->sel == n) {
			g->SetTextColor(g->rgb(115,115,220),g->rgb(255,200,0));
			g->TextOut(4+x+s*10,y,n->line,strlen(n->line));
			g->SetTextColor(g->rgb(0,0,0),g->cWindow);
		}else{
			g->TextOut(4+x+s*10,y,n->line,strlen(n->line));
		}

		if(n->child) {
			g->Line(x+s*10-10,y+6,x+s*10,y+6);
		}else{
			g->Line(x+s*10-4 ,y+6,x+s*10,y+6);
		}

		if(n->child && n->bExpand) {//down
			n = n->child;
			stack[s++] = y+8;
		}else{
			while(!n->next) {		//up
				s--;
				n = n->parent;
				if(!n) return; //root node
			}
			n = n->next;
		}
		y += 16;
	}
}

void RdTreeView::Draw(RControl *rc, RGraphics *g)
{
	RTreeView *r = (RTreeView*)rc;

	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
	g->SetBg(g->cWindow);
	g->SetBk(g->cWindow);

	int y = r->top + 2;
	DrawNode(r,r->left+2,y,r->dtn,g);
}

void RdTreeView::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdToolTip::Draw(RControl *rc, RGraphics *g)
{
	RToolTip *r = (RToolTip*)rc;

	g->FillRect(g->cWindow,0,0,r->width,r->height);

	g->SetTextColor(g->cWinText,g->cWindow);
	g->TextOut(r->left+8,r->top+4,r->ttext,strlen(r->ttext));

	g->SetFg(g->cWinText);
	g->Rectangle(0,0,r->right,r->bottom);
}

void RdToolTip::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdEditBox::Draw(RControl *rc, RGraphics *g)
{
	REditBox *r = (REditBox*)rc;
	g->FillRect(g->cWindow,r->left,r->top,r->width,r->height);

	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(80,80,80):g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+6,r->text,strlen(r->text));

	//dbg g->Line(0,r->top+6,100,r->top+6);
	
	if(r->bFocus) {
		g->SetFg(g->rgb(0,0,0));
		g->Line(r->left + 8*r->kx+8,r->top+3,r->left + 8*r->kx+8,r->bottom-3); //sz font
	}

	DrButton(r,g,true);
}

void RdEditBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRange::Draw(RControl *rc, RGraphics *g)
{
	RRange *r = (RRange*)rc;
	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);

	// The Line
	int top = r->top+(r->height>>1);
	g->SetFg(g->c3DShadow);
	g->Pixel(r->left,top+1);
	g->Line(r->left,top,r->right,top);
	g->SetFg(g->c3DHilight);
	g->Line(r->left+1,top+1,r->right,top+1);
	g->Pixel(r->right,top);

	// the RButton
	int left = r->current - 5 + r->left;
	int right= r->current + 5 + r->left;
	g->FillRect(r->IsHit() ? g->cScrollbar : g->c3DFace,left+2,r->top+2,right-left-1,r->height-2);
	DrButton(r,g,left,r->top,right,r->bottom,0);
}

void RdRange::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 24;
	y = 24;
}

//-----------------


void DrSbButton(RControl *rc, RGraphics *g, int left, int top, int right, int bottom, bool b)
{
	DrButton(rc,g,left,top,right,bottom,b);
//	g->FillRect(g->rgb(0,200,200),left+2,top+2,right-left-3,bottom-top-3);
	g->FillRect(g->c3DFace,left+2,top+2,right-left-3,bottom-top-3);
}

void RdScrollBar::Draw(RControl *rc, RGraphics *g)
{
	RScrollBar *r = (RScrollBar*)rc;
//	g->FillRect(g->rgb(200,0,200),r->left,r->top,r->width,r->height); //Debug

	if(r->align & AL_V) {
		DrSbButton(r,g,r->left,r->top,r->right,r->top+r->width-1,0);
		DrSbButton(r,g,r->left,r->bottom-r->width+1,r->right,r->bottom,0);

		int cx,cy,q; // arrows
		g->SetFg(g->cWinText);
		cx = r->left + (r->width/2);
		cy = r->top + (r->width/2);
		q = r->width / 5;
		g->Line(cx-q,cy+q,cx,cy-q,cx+q,cy+q);
		cy = r->bottom - (r->width/2);
		g->Line(cx-q,cy-q,cx,cy+q,cx+q,cy-q);


		// the RButton
		int l = (r->height - 1) - r->width * 2 - r->pmax;
		int top = r->ppos + r->top + r->width;
		int bottom = top + l;
		g->FillRect(r->IsHit() ? g->cScrollbar : g->c3DFace,r->left+2,top+2,r->width-4,bottom-top-1);
		DrButton(r,g,r->left,top,r->right,bottom,0);

		//background
		g->FillRect(g->cScrollbar,r->left,r->top+r->width,r->width,top-(r->top+r->width));
		g->FillRect(g->cScrollbar,r->left,bottom+1,r->width,(r->bottom-r->width)-bottom);


	}else{
		//buttons on side
		DrSbButton(r,g,r->left,r->top,r->left+r->height-1,r->bottom,0);
		DrSbButton(r,g,r->right-r->height+1,r->top,r->right,r->bottom,0);

		int cx,cy,q; //arrows
		g->SetFg(g->cWinText);
		cx = r->left + (r->height/2);
		cy = r->top + (r->height/2);
		q = r->height / 5;
		g->Line(cx+q,cy-q,cx-q,cy,cx+q,cy+q);
		cx = r->right - (r->height/2);
		g->Line(cx-q,cy-q,cx+q,cy,cx-q,cy+q);

		// the RButton
		int l = (r->width - 1) - r->height * 2 - r->pmax;
		int left = r->ppos + r->left + r->height;
		int right= left + l;
		g->FillRect(r->IsHit() ? g->cScrollbar : g->c3DFace,left+2,r->top+2,right-left-1,r->height-2);
		DrButton(r,g,left,r->top,right,r->bottom,0);

		//background
		g->FillRect(g->cScrollbar,r->left+r->height,r->top,left-(r->left+r->height),r->height);
		g->FillRect(g->cScrollbar,right+1,r->top,(r->right-r->height)-right,r->height);
	}
}

void RdScrollBar::GetMinSize(RControl *rc, int &x, int &y)
{
	if(rc->align & AL_V) {
		x = 16;
		y = 50;
	}else{
		x = 50;
		y = 16;
	}
}

//-----------------
//
#include <stdio.h>
#ifdef WIN32
#define snprintf _snprintf
#endif

void RdProgressBar::Draw(RControl *rc, RGraphics *g)
{
	RProgressBar *r = (RProgressBar*)rc;
	if(r->align & AL_V) {
		int h = r->height * r->ppos / r->pmax;
		g->FillRect(g->cSelect,r->left,r->top,r->width,h);
		g->FillRect(g->c3DLight,r->left,r->top + h,r->width,r->height - h);
	}else{
		int w = r->width * r->ppos / r->pmax;
		g->FillRect(g->cSelect,r->left,r->top,w,r->height);
		g->FillRect(g->c3DLight,r->left + w,r->top,r->width - w,r->height);
	}

	char c[12];
	snprintf(c,11,"%i%%",r->ppos * 100 / r->pmax);
	g->SetTextColor(g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+r->width/2,r->top+3,c,strlen(c));
}

void RdProgressBar::GetMinSize(RControl *rc, int &x, int &y)
{
	if(rc->align & AL_V) {
		x = 16;
		y = 50;
	}else{
		x = 50;
		y = 16;
	}
}

//-----------------

void RdStatusBar::Draw(RControl *rc, RGraphics *g)
{
	RStatusBar *r = (RStatusBar*)rc;

	g->FillRect(g->c3DLight,r->left,r->top,r->width,r->height);
	g->SetTextColor(r->IsHit() ? g->rgb(0,180,0):g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+4,r->text,strlen(r->text));
}

void RdStatusBar::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 50;
	y = 20;
}

//-----------------

void RdListView::DrawNode(RListView *lv,int x,int y,SListNode *n, RGraphics *g)
{
	if(!n) {
		return;
	}
	if(lv->sel == n) {
		g->SetTextColor(g->rgb(115,115,220),g->rgb(255,200,0));
		g->TextOut(x,y,n->line,strlen(n->line));
		g->SetBk(g->cWindow);
	}else{
		g->SetTextColor(g->rgb(0,0,0),g->cWindow);
		g->TextOut(x,y,n->line,strlen(n->line));
	}
}

void RdListView::Draw(RControl *rc, RGraphics *g)
{
	RListView *r = (RListView*)rc;

	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
	g->SetBg(g->cWindow);
	g->SetBk(g->cWindow);

	int i = 0, y;
	for(SListNode *n = r->dln; n; n = n->next, i++) {
		y = r->top+2 + i*16;
		if(y > r->bottom) {
			break;
		}
		DrawNode(r,r->left+2,y,n,g);
	}
}

void RdListView::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 150; //XXX RComboBox min size... !!! XXX
	y = 200;
}


// Drawing Manager
//-----------------------------------------------------------------------------++++

/*
#define RBUTTON			1
#define RMENUBUTTON		2
#define RCONTROL		3
#define RRANGE			4
#define RCHECKBOX		5
#define RRADIOBUTTON	6
#define RTOGGLEBUTTON	7
#define RTEXTEDIT		8
#define RTABCONTROL		9
#define RTREEVIEW		10
#define RWINDOW			11
#define RTOOLTIP		12
#define RTBOX			13
#define REDITBOX		14
#define RCOMBOBOX		20
*/


RdControl		rdControl;
RdTBox			rdTBox;
RdWindow		rdWindow;
RdTabControl	rdTabControl;

RdButton		rdButton;
RdMenuButton	rdMenuButton;
RdRange			rdRange;
RdCheckBox		rdCheckBox;
RdRadioButton	rdRadioButton;
RdToggleButton	rdToggleButton;
RdTextEdit		rdTextEdit;
RdTreeView		rdTreeView;
RdToolTip		rdToolTip;
RdEditBox		rdEditBox;
RdScrollBar		rdScrollBar;
RdProgressBar	rdProgressBar;
RdStatusBar		rdStatusBar;
RdLabel			rdLabel;
RdListView		rdListView;
RdComboBox		rdComboBox;


RDrawer *rd [] =
{	
	&rdButton,
	&rdMenuButton,
	&rdControl,
	&rdRange,
	&rdCheckBox,
	&rdRadioButton,
	&rdToggleButton,
	&rdTextEdit,
	&rdTabControl,
	&rdTreeView,
	&rdWindow,
	&rdToolTip,
	&rdTBox,
	&rdEditBox,
	&rdScrollBar,
	&rdProgressBar,
	&rdStatusBar,
	&rdLabel,
	&rdListView,
	&rdComboBox,
};

RDrawer rdnull;

RDrawer *GetDrawer(u32 d)
{
	if(d && d <= RDMAX)
		return rd[d-1];
	return &rdnull;
}

//-----------------------------------------------------------------------------++++
