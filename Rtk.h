// vim:ts=4 sts=0 sw=4

#ifndef RTK_H
#define RTK_H

//----------------------
#undef HAVE_OS
#undef HAVE_ERR

// POSIX ... ???
#ifdef XLIB
#define HAVE_OS
#endif

#ifdef WIN32
#ifdef HAVE_OS
#define HAVE_ERR
#else
#define HAVE_OS
#endif
#endif

/*#ifdef MAC
#ifdef HAVE_OS
#define HAVE_ERR
#else
#define HAVE_OS
#endif
#endif*/

#ifndef HAVE_OS
#error No OS interface defined...
#endif

#ifdef HAVE_ERR
#error Cannot define more than one OS interface...
#endif
//----------------------

#include <string.h> //????
#include <stdlib.h> //????
#ifdef XLIB
#include <stdio.h>
#endif

#include "sisl.h"
#include "rkey.h"

class RControl;
class RWindow;

//-----------------------------------------------------------------------------++++
// Drawing

class RGraphics;
class RDrawer
{
public:
	RDrawer() {};
	virtual ~RDrawer() {};
	virtual void Draw(RControl *rc, RGraphics *g);
	virtual void Draw(RControl *rc, bool bAll);
	virtual void GetMinSize(RControl *rc, int &x, int &y);
};

// to be replaced by something more flexible ???
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
#define RSCROLLBAR		15
#define RPROGRESSBAR	16
#define RSTATUSBAR		17
#define RLABEL			18
#define RLISTVIEW		19
#define RCOMBOBOX		20
#define RDMAX			20

#define GETDRAWER(a,b) void a ::GetDrawer(u32 d)\
{\
	RControl::GetDrawer(d ? d : b );\
}

/*-----------------------------------------------------------------------------++++
Defining class, Init and Dinit stuff
Thanks to problems with <this> in the ctor
and vtable modification in ctor and dtor
I now use the following.

#define class_P parentclass

protected:
class() {}; - not used
void Init()
{
	your code
	class_P::Init(params);
	your code
}
void Dinit()
{
	your code
	class_P::Dinit(); <-- IMPORTANT!!!
	your code
}
public:
class(params) //ctor
{
	your code
	Init(params);
	your code
}
~class() { DINIT(); }; //dtor

*/
//-----------------------------------------------------------------------------++++

// frametype
#define F_NONE		0
#define F_UP		1
#define F_DOWN		2
#define F_GROUP		3

// align
#define AL_CENTER	0x00
#define AL_LEFT		0x01
#define AL_RIGHT	0x02
#define AL_TOP		0x04
#define AL_BOTTOM	0x08
#define AL_STRETCHH	0x03
#define AL_STRETCHV 0x0C
#define AL_H		0x00
#define AL_V		0x10
#define AL_FITH		0x40
#define AL_FITV		0x80

// SetPos SetSize
#define SETPS_X 1
#define SETPS_Y 2

// MButton
#define M_MASK		0xFF
#define M_REP		0x10000000
#define M_DBL		0x40000000
#define M_DOWN		0x80000000
#define M_CLICK		0x20000000

struct Rect
{
	int left,right,top,bottom;
};

//-----------------------------------------------------------------------------++++
// Control class -- the base of all

class RContainer;
#define RControl_P CConn
class RControl : public CConn
{
protected:
	RControl() {};
	void Init(RContainer *r);
	void Dinit();
public:
	virtual void GetDrawer(u32 d);
	RControl(RContainer *r) { Init(r); };
	~RControl() { DINIT(); };

//-----------------------------------------------
// container stuff

	bool IsChild(RControl *c);
	bool IsChildR(RControl *c); // recursive IsChild - used by RWindow::KUnGrabR
	void ClipRect(Rect &r);
	virtual RControl *GetChild() { return 0; };
	virtual RControl *GetLast()  { return 0; };

//-----------------------------------------------
// control

	void Draw(bool bAll = false);
	virtual bool IsWindow() { return false; };
	virtual void SetText(const char *c);
	virtual void Remove();// { if(parent) parent->RemoveControl(this); };
	void RemoveLater();
	char *GetToolTip() { return tooltip; };
	void ShowToolTip();

//-----------------------------------------------
// sizing + positioning

	virtual void SetLTRB(int l,int t,int r,int b); // internal
	//void SetLTWH(int l,int t,int w,int h) { SetLTRB(l,t,l+w-1,t+h-1); }; // internal

	void ReCalc(bool b=true) { int x,y; GetMinSize(x,y); ReCalcI(b); };
	virtual void ReCalcI(bool b=true) {}; //Internal
	virtual void GetMinSize(int &x, int &y)	{ pDrawer->GetMinSize(this,x,y); x_min = x; y_min = y; };

	// R = Relative
	void SetPos(int x, int y);
	void SetPosR(int x, int y);
	void SetSize(int x, int y);
	void SetSizeR(int x, int y);
	void SetPosSize(int x, int y, int w, int h);

	// convenience functions
	int GetPosX()	{ return left; };
	int GetPosY()	{ return top; };
	int GetWidth()	{ return width; };
	int GetHeight()	{ return height; };
	void SetPosX(int x) { SetPos(x,top); };
	void SetPosY(int y) { SetPos(left,y); };
	void SetWidth(int w) { SetSize(w,height); };
	void SetHeight(int h){ SetSize(width,h); };

	void GetSizeF(int &w, int &h);
	void GetSizeFI(int &w, int &h);
	void SetBoxF(int x, int y, int w, int h);
	void GetFrameSizes(int &w, int &h, int &ww, int &hh);

	void SetAlignment(u32 d) { align = (u16)d; }; //????

//-----------------------------------------------
// raw mouse
	void MRawEnter(bool);
	void MRawExit(bool);
	void MRawButton(u32 b);
	bool HitTest(int x, int y);
	RControl *MRawHitTest(int x, int y, bool up);
	void MRawMove(int x, int y);
	void MRawScroll(int b,int d);

//-----------------------------------------------
// mouse
	bool IsHit();
	u32 HasGrab();
	//bool IsPushed() { return IsHit() && HasGrab(); };
	virtual void MEnter(bool b) {};
	virtual void MExit(bool b) {};
	virtual void MHover() {};
	virtual void MMove();
	virtual void MScroll(int b,int d);
	virtual void MButton(u32 b);
	inline int GetMX();
	inline int GetMY();
	inline int GetMOX();
	inline int GetMOY();
	inline int GetMRX();
	inline int GetMRY();

//-----------------------------------------------
// keyboard
	virtual void KUp(u32 d) {};
	virtual void KDown(u32 d) {};
	virtual void KChar(u32 d) {};

//-----------------------------------------------
// signals & slots

	SISL(1);

	SIGNAL(Si_MEnter)(bool);
	SIGNAL(Si_MExit)(bool);

	SIGNAL(Si_SetText)(char *);
	SLOT(Sl_SetText)(char *c) { SetText(c); Draw(); };

	void ShowC();
	void HideC();
	bool IsShown();
	SLOT(Sl_Show)(bool);
	SLOT(Sl_Show)();
	SLOT(Sl_Hide)();


	void EnableC(); //private
	void DisableC(); //private
	bool IsEnabled();
	SLOT(Sl_Enable)(bool);
	SLOT(Sl_Enable)();
	SLOT(Sl_Disable)();
	void Enable() { Sl_Enable(); };
	void Disable() { Sl_Disable(); };

	bool IsFocused();
	SIGNAL(Si_Focus)(bool);
	SLOT(Sl_Focus)(bool);
	SLOT(Sl_Focus)();
	SLOT(Sl_UnFocus)();

//-----------------------------------------------
// variables

public:
	RWindow *wnd;
	RContainer *parent;
	RControl *next,*prev;

	u16 rsvd16;
	//u8 x_margin,y_margin; //Z move to the container
	u8 x_weight,y_weight; //Z ??? stay here ???
	s16 left,right,top,bottom;
	u16 width,height;
	s16 x_min, y_min;

	u16 vw,vh; //view w/h

	char *text;
	char *tooltip;
	RDrawer *pDrawer;

	u16 align;
	// button/control attributes
	union {
		u16 flags;
	struct {
		u8 bPushed:1,bFocus:1,bChecked:1;
		u8 bDisabled:2,bHidden:2;
		u8 bMvSz:1; // moving & sizing temp variable
		u8 bCollapse:1; // MinSize = 0 for hidden control
		u8 bForceMin:1; //force min size
		u8 rsvd:4;
		u8 frame:2;
	}; };

	void *user;
};

// Control class -- the base of all
//-----------------------------------------------------------------------------++++
// Containers

#define RContainer_P RControl
class RContainer : public RControl
{
protected:
	RContainer() {};
	void Init(RContainer *r);
	void Dinit();
public:
	//RContainer(RContainer *r) { Init(r); };
	~RContainer() { DINIT(); };
	SISLTN();
	
	virtual void AddControl(RControl *b);
	virtual void RemoveControl(RControl *b, bool del = true);

	RControl *GetChild() { return child; };
	RControl *GetLast()  { return last; };

	void Clear();

	void GetMinSize(int &x, int &y) { x_min = x = 20; y_min = y = 20; };
//	virtual void SetLTRB(int l,int t,int r,int b); // internal
	
	int x_mgn, y_mgn, spacing;
	RControl *child,*last;
};

#define RHVBox_P RContainer
class RHVBox : public RContainer
{
protected:
	RHVBox() {};
public:
	RHVBox(RContainer *r) { Init(r); };
	~RHVBox() { DINIT(); };
	SISLTN();

	void ReCalcI(bool b=true);
	void GetMinSize(int &x, int &y);
};

#define RTBox_P RContainer
class RTBox : public RContainer
{
protected:
	RTBox() {};
	void Init(RContainer *r);
	void Dinit();
	void Alloc();
public:
	void GetDrawer(u32 d);
	RTBox(RContainer *r, int w, int h) { ww = w; hh = h; Init(r); };
	~RTBox() { DINIT(); };
	SISLTN();

	RControl *GetAt(int,int);
protected:
	void SetAt(RControl *,int,int);
public:
	void SetPos(int x, int y, int w=1, int h=1);
	void SetNextPos();
	void SetWeight(int,int,int,int);

	void ReCalcI(bool b=true);
	void GetMinSize(int &x, int &y);

	//int autopos;
protected:
	int ww,hh;
	u16 *xmin,*ymin,*xw,*yw,*xpos,*ypos;
	RControl **pc;
	void *mem;
};

#define RSBox_P RTBox
class RSBox : public RTBox
{
protected:
	RSBox() {};
	//void Init(RContainer *r);
public:
	RSBox(RContainer *r) { ww = 2; hh = 2; Init(r); };
	~RSBox() { DINIT(); };
	SISLTN();
	
	void ReCalcI(bool b=true);
	void GetMinSize(int &x, int &y) { x = 20; y = 20; };

	void Setup(int,int);
};

//------------------------------------------------

#define RScrollView_P RHVBox
class RScrollView : public RHVBox
{
protected:
	RScrollView() {};
	void Init(RContainer *r) { RScrollView_P::Init(r); oox = ooy = 0; };
public:
	RScrollView(RContainer *r) { Init(r); };
	~RScrollView() { DINIT(); };

	SISL(2);
	SIGNAL(Si_ScrolledX)(int);
	SIGNAL(Si_ScrolledY)(int);
	SLOT(Sl_ScrollX)(int);
	SLOT(Sl_ScrollY)(int);
	SIGNAL(Si_AdjustX)(int,int);
	SIGNAL(Si_AdjustY)(int,int);
	void ReCalcI(bool b=true);

	virtual void Scroll(int x, int y);

	void MScroll(int b,int d);
	void KDown(u32 d);
	void GetMinSize(int &x, int &y) { x = 20, y = 20; };

	int oox,ooy;
};

//------------------------------------------------

#define RRadioBox_P RTBox
class RRadioBox : public RTBox
{
protected:
	RRadioBox() {};
public:
	RRadioBox(RContainer *r, int w, int h) { ww = w; hh = h; Init(r); };
	~RRadioBox() { DINIT(); };
	SISLTN();

	//WARNING could loop if child is not overridden
	void MScroll(int b,int d) { if(child) child->MScroll(b,d); };
};

//------------------------------------------------

#define RTabControl_P RContainer
class RTabControl : public RContainer
{
protected:
	RTabControl() {};
	void Init(RContainer *r);
public:
	void GetDrawer(u32 d);
	RTabControl(RContainer *r) { Init(r); };
	~RTabControl() { DINIT(); };
	SISLTN();

	void AddControl(RControl *b);
	void ReCalcI(bool b=true);
	void GetMinSize(int &x, int &y);
	void MMove() {};

	bool ConnectTab(cchar *tab, cchar *si, CConn *cnn, cchar *sisl, UPTR u=0);

	RRadioBox *rrb;
};

// Containers
//-----------------------------------------------------------------------------++++
// Controls

#define RButton_P RControl
class RButton : public RControl
{
protected:
	RButton() {};
public:
	void GetDrawer(u32 d);
	RButton(RContainer *r) { Init(r); };
	~RButton() { DINIT(); };

	SISL(2);
	SIGNAL(Si_Clicked)(int btn);
	virtual SLOT(Sl_Clicked)(int);
	SLOT(Sl_Clicked)() { Sl_Clicked(1); };

	void KUp(u32);
	void KDown(u32);
	void MButton(u32);
};

#define RLabel_P RButton
class RLabel : public RButton
{
protected:
	RLabel() {};
	//void Init(RContainer *r) { RLabel_P::Init(r); };
public:
	void GetDrawer(u32 d);
	RLabel(RContainer *r) { Init(r); };
	~RLabel() { DINIT(); };
	SISLTN();
};

//------------------------------------------------

#define RCheckBox_P RControl
class RCheckBox : public RControl
{
protected:
	RCheckBox() {};
public:
	void GetDrawer(u32 d);
	RCheckBox(RContainer *r) { Init(r); };
	~RCheckBox() { DINIT(); };

	void MButton(u32 b);

	SISL(2);
	SIGNAL(Si_Checkd)(bool);
	SIGNAL(Si_Checked)();
	SIGNAL(Si_UnChecked)();
	virtual SLOT(Sl_Check)() { bChecked = true; Si_Checked(); Si_Checkd(true); Draw(); };
	virtual SLOT(Sl_UnCheck)() { bChecked = false; Si_UnChecked(); Si_Checkd(false); Draw(); };
};

//------------------------------------------------

#define RRadioButton_P RCheckBox
class RRadioButton : public RCheckBox
{
protected:
	RRadioButton() {};
	void InitRB(RRadioButton *rb);
public:
	void GetDrawer(u32 d);
	RRadioButton(RRadioBox *r) { Init(r); InitRB((RRadioButton*)prev); };
	RRadioButton(RContainer *r, RRadioButton *rb = 0) { Init(r); InitRB(rb); };
	~RRadioButton() { DINIT(); };
	SISLTN();
	void Remove();

	void MScroll(int b,int d);
	void MButton(u32 b);
	void KDown(u32 d);
	void Sl_Check();
	
	RRadioButton *rfirst,*rnext,*rprev;
};

//------------------------------------------------

#define RToggleButton_P RCheckBox
class RToggleButton : public RCheckBox
{
protected:
	RToggleButton() {};
public:
	void GetDrawer(u32 d);
	RToggleButton(RContainer *r) { Init(r); };
	~RToggleButton() { DINIT(); };
	SISLTN();
};

//------------------------------------------------

#define RRange_P RControl
class RRange : public RControl
{
protected:
	RRange() {};
	void Init(RContainer *r) { RRange_P::Init(r); current=9; max=0; };
public:
	void GetDrawer(u32 d);
	RRange(RContainer *r) { Init(r); };
	~RRange() { DINIT(); };

	SISL(2);
	SIGNAL(Si_Moved)(int);

	void KDown(u32);
	void MMove();
	void MScroll(int b,int d);
	void MButton(u32 b);

	int current,max;
};

#define RScrollBar_P RControl
class RScrollBar : public RControl
{
protected:
	RScrollBar() {};
	void Init(RContainer *r) { RScrollBar_P::Init(r); lpos = ppos = 0; pmax = 100; lv = 300; lmax = 600; };
public:
	void GetDrawer(u32 d);
	RScrollBar(RContainer *r) { Init(r); bCollapse = 1; };
	~RScrollBar() { DINIT(); };

	SISL(2);
	SLOT(Sl_Scroll)(int);
	SIGNAL(Si_Scrolled)(int);
	SLOT(Sl_Adjust)(int,int);
	void ReCalcI(bool b=true);

	int Gett();
	void MMove();
	void MButton(u32 b);
	void MScroll(int b,int d);
	void KDown(u32);

//	friend class RdScrollBar;
//20131213 protected:

	void AdjustP(int);
	void AdjustL(int);
	void SetL(int);
	
	int ppos,pmax,lmax,lv;
	int lpos;
};

#define RProgressBar_P RControl
class RProgressBar : public RControl
{
protected:
	RProgressBar() {};
	void Init(RContainer *r) { RProgressBar_P::Init(r); ppos = 0; pmax = 100; frame = F_DOWN; };
public:
	void GetDrawer(u32 d);
	RProgressBar(RContainer *r) { Init(r); };
	~RProgressBar() { DINIT(); };

	SISL(2);
	SLOT(Sl_Move)(int i) { ppos = i; if(ppos < 0) ppos = 0; if(ppos > pmax) ppos = pmax; Draw(); };

	int ppos,pmax;
};

#define RStatusBar_P RControl
class RStatusBar : public RControl
{
protected:
	RStatusBar() {};
	void Init(RContainer *r) { RStatusBar_P::Init(r); frame = F_DOWN; };
public:
	void GetDrawer(u32 d);
	RStatusBar(RContainer *r) { Init(r); };
	~RStatusBar() { DINIT(); };
	SISLTN();
};

// Controls
//-----------------------------------------------------------------------------++++

#include "RWindow.h"

#endif //RTK_H

