// vim:ts=4 sts=0 sw=4


#include "RtkAll.h"

#define _RGRAPHICS_
#include "drawman.h"

//#define FRAMES
//#define CLIPTEST

//-----------------------------------------------------------------------------++++
//Helper functions XXX relocate this ???

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

void DrButton(RControl *rc, RGraphics *g, bool b)
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

COLOR ColorAvg(COLOR c1, COLOR c2)
{
	u32 x1,x2,y1,y2,z1,z2;

	x1 = c1 & 0xFF;
	y1 = (c1 >> 8) & 0xFF;
	z1 = (c1 >>16) & 0xFF;

	x2 = c2 & 0xFF;
	y2 = (c2 >> 8) & 0xFF;
	z2 = (c2 >>16) & 0xFF;

	x1 += x2;
	y1 += y2;
	z1 += z2;

	x1 >>= 1;
	y1 >>= 1;
	z1 >>= 1;

	c1 = (z1 << 16) | (y1 << 8) | x1;
	return c1;
}


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

//-----------------------------------------------------------------------------++++
// GDI Graphics wrapper class

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
//	c3DLight	= GetSysColor(COLOR_3DLIGHT);
	c3DFace		= GetSysColor(COLOR_3DFACE);
	c3DDShadow	= GetSysColor(COLOR_3DDKSHADOW);
	c3DShadow	= GetSysColor(COLOR_3DSHADOW);
	c3DLight	= ColorAvg(c3DHilight,c3DFace);

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
	for(RControl *c = rc->GetChild(); c; c = c->next) { //XXX remove from here ???
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
//	c3DLight	= rgb(224,224,160);
	c3DFace		= rgb(224,224,160);
	c3DShadow	= rgb(192,192,64);
	c3DDShadow	= 0;
	c3DLight	= ColorAvg(c3DHilight,c3DFace);

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
	for(RControl *c = rc->GetChild(); c; c = c->next) { //XXX remove from here ???
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

	t.SplitL(u,'=');
	t.SplitL(u,',');
	r = t;
	t.SplitL(u,',');
	g = t;
	t.SplitL(u,',');
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
//	rg.c3DLight		= GetRGB(n,"3DLight");
	rg.c3DFace		= GetRGB(n,"3DFace");
	rg.c3DShadow	= GetRGB(n,"3DShadow");
	rg.c3DDShadow	= GetRGB(n,"3DDkShadow");
	rg.c3DLight		= ColorAvg(rg.c3DHilight,rg.c3DFace);

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


extern RDrawer *rdRtk  [];
extern RDrawer *rdWin4 [];

RDrawer rdnull;
//RDrawer **prd = rdRtk;
RDrawer **prd = rdWin4;

//XXX totally rewrite this with named lists instead
RDrawer *GetDrawer(u32 d)
{
	if(d && d <= RDMAX)
		return prd[d-1];
	return &rdnull;
}

//-----------------------------------------------------------------------------++++
