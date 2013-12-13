
#include "RtkAll.h"

#define _RGRAPHICS_
#include "drawman.h"

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
RDRAWER(RdRtkControl)
RDRAWER(RdRtkTBox)
RDRAWER(RdRtkWindow)
RDRAWER(RdRtkTabControl)

RDRAWER(RdRtkButton)
RDRAWER(RdRtkComboBox)
RDRAWER(RdRtkLabel)
RDRAWER(RdRtkMenuButton)
RDRAWER(RdRtkCheckBox)
RDRAWER(RdRtkRadioButton)
RDRAWER(RdRtkToggleButton)
RDRAWER(RdRtkTextEdit)
RDRAWERX(RdRtkTreeView)
void DrawNode(RTreeView *tv,int x,int &y,RTreeNode *n, RGraphics *g); };
RDRAWERX(RdRtkListView)
void DrawNode(RListView *lv,int x,int y,SListNode *n, RGraphics *g); };
RDRAWER(RdRtkToolTip)
RDRAWER(RdRtkEditBox)
RDRAWER(RdRtkRange)
RDRAWERX(RdRtkScrollBar)
void DrSbButton(RControl *rc, RGraphics *g, int left, int top, int right, int bottom, bool b); };
RDRAWER(RdRtkProgressBar)
RDRAWER(RdRtkStatusBar)

//-----------------------------------------------------------------------------++++
// RDrawer

void RdRtkControl::Draw(RControl *rc, RGraphics *g)
{
	g->FillRect(g->c3DFace,rc->left,rc->top,rc->width,rc->height);

#ifdef FRAMES
	g->SetFg(g->rgb(255,0,255)); //ff
	for(RControl *c = rc->GetChild(); c; c = c->next) {
		g->Rectangle(c->left-1,c->top-1,c->right+1,c->bottom+1);
	}
#endif
}

void RdRtkControl::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdRtkTBox::Draw(RControl *rc, RGraphics *g)
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

void RdRtkTBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdRtkWindow::Draw(RControl *rc, RGraphics *g)
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

void RdRtkWindow::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdRtkTabControl::Draw(RControl *rc, RGraphics *g)
{
//#ifdef CLIPTEST
//	g->FillRect(g->rgb(0xDD,0xAA,0xDD),rc->left,rc->top,rc->width,rc->height);
//#endif
//	DrButton(rc,g,true);
}

void RdRtkTabControl::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdRtkButton::Draw(RControl *rc, RGraphics *g)
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

void RdRtkButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkComboBox::Draw(RControl *rc, RGraphics *g)
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

void RdRtkComboBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkLabel::Draw(RControl *rc, RGraphics *g)
{
	RLabel *r = (RLabel*)rc;

	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
	g->SetTextColor(r->bDisabled ? g->cGrayText : r->IsHit() ? g->rgb(0,180,0):g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+2,r->text,strlen(r->text));
}

void RdRtkLabel::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 16;
}

//-----------------

void RdRtkMenuButton::Draw(RControl *rc, RGraphics *g)
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

void RdRtkMenuButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkCheckBox::Draw(RControl *rc, RGraphics *g)
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

void RdRtkCheckBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkRadioButton::Draw(RControl *rc, RGraphics *g)
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

void RdRtkRadioButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkToggleButton::Draw(RControl *rc, RGraphics *g)
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

void RdRtkToggleButton::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 80;
	y = 24;
}

//-----------------

void RdRtkTextEdit::Draw(RControl *rc, RGraphics *g)
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

void RdRtkTextEdit::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkTreeView::DrawNode(RTreeView *tv,int x,int &y,RTreeNode *n, RGraphics *g)
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

void RdRtkTreeView::Draw(RControl *rc, RGraphics *g)
{
	RTreeView *r = (RTreeView*)rc;

	g->FillRect(g->c3DFace,r->left,r->top,r->width,r->height);
	g->SetBg(g->cWindow);
	g->SetBk(g->cWindow);

	int y = r->top + 2;
	DrawNode(r,r->left+2,y,r->dtn,g);
}

void RdRtkTreeView::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 20;
	y = 20;
}

//-----------------

void RdRtkToolTip::Draw(RControl *rc, RGraphics *g)
{
	RToolTip *r = (RToolTip*)rc;

	g->FillRect(g->cWindow,0,0,r->width,r->height);

	g->SetTextColor(g->cWinText,g->cWindow);
	g->TextOut(r->left+8,r->top+4,r->ttext,strlen(r->ttext));

	g->SetFg(g->cWinText);
	g->Rectangle(0,0,r->right,r->bottom);
}

void RdRtkToolTip::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkEditBox::Draw(RControl *rc, RGraphics *g)
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

void RdRtkEditBox::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 100;
	y = 24;
}

//-----------------

void RdRtkRange::Draw(RControl *rc, RGraphics *g)
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

void RdRtkRange::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 24;
	y = 24;
}

//-----------------


void RdRtkScrollBar::DrSbButton(RControl *rc, RGraphics *g, int left, int top, int right, int bottom, bool b)
{
	DrButton(rc,g,left,top,right,bottom,b);
//	g->FillRect(g->rgb(0,200,200),left+2,top+2,right-left-3,bottom-top-3);
	g->FillRect(g->c3DFace,left+2,top+2,right-left-3,bottom-top-3);
}

void RdRtkScrollBar::Draw(RControl *rc, RGraphics *g)
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

void RdRtkScrollBar::GetMinSize(RControl *rc, int &x, int &y)
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

void RdRtkProgressBar::Draw(RControl *rc, RGraphics *g)
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

void RdRtkProgressBar::GetMinSize(RControl *rc, int &x, int &y)
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

void RdRtkStatusBar::Draw(RControl *rc, RGraphics *g)
{
	RStatusBar *r = (RStatusBar*)rc;

	g->FillRect(g->c3DLight,r->left,r->top,r->width,r->height);
	g->SetTextColor(r->IsHit() ? g->rgb(0,180,0):g->rgb(0,0,0),g->c3DFace);
	g->TextOut(r->left+8,r->top+4,r->text,strlen(r->text));
}

void RdRtkStatusBar::GetMinSize(RControl *rc, int &x, int &y)
{
	x = 50;
	y = 20;
}

//-----------------

void RdRtkListView::DrawNode(RListView *lv,int x,int y,SListNode *n, RGraphics *g)
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

void RdRtkListView::Draw(RControl *rc, RGraphics *g)
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

void RdRtkListView::GetMinSize(RControl *rc, int &x, int &y)
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


RdRtkControl		rdRtkControl;
RdRtkTBox			rdRtkTBox;
RdRtkWindow			rdRtkWindow;
RdRtkTabControl		rdRtkTabControl;

RdRtkButton			rdRtkButton;
RdRtkMenuButton		rdRtkMenuButton;
RdRtkRange			rdRtkRange;
RdRtkCheckBox		rdRtkCheckBox;
RdRtkRadioButton	rdRtkRadioButton;
RdRtkToggleButton	rdRtkToggleButton;
RdRtkTextEdit		rdRtkTextEdit;
RdRtkTreeView		rdRtkTreeView;
RdRtkToolTip		rdRtkToolTip;
RdRtkEditBox		rdRtkEditBox;
RdRtkScrollBar		rdRtkScrollBar;
RdRtkProgressBar	rdRtkProgressBar;
RdRtkStatusBar		rdRtkStatusBar;
RdRtkLabel			rdRtkLabel;
RdRtkListView		rdRtkListView;
RdRtkComboBox		rdRtkComboBox;


RDrawer *rdRtk [] =
{	
	&rdRtkButton,
	&rdRtkMenuButton,
	&rdRtkControl,
	&rdRtkRange,
	&rdRtkCheckBox,
	&rdRtkRadioButton,
	&rdRtkToggleButton,
	&rdRtkTextEdit,
	&rdRtkTabControl,
	&rdRtkTreeView,
	&rdRtkWindow,
	&rdRtkToolTip,
	&rdRtkTBox,
	&rdRtkEditBox,
	&rdRtkScrollBar,
	&rdRtkProgressBar,
	&rdRtkStatusBar,
	&rdRtkLabel,
	&rdRtkListView,
	&rdRtkComboBox,
};

//-----------------------------------------------------------------------------++++


