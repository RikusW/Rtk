// vim:ts=4 sts=0 sw=4
// Win32 -- RWindow & RApp

#ifndef WINDOW_H
#define WINDOW_H

#include "Rtk.h"

// RTKMsg magic
#define RTKMSG_MAGIC			0x5AB30000
#define RTKMSG_MAGIC_MASK		0xFFFF0000
// RTKMsg message
#define RTKMSG_MASK				0x0000FFFF
#define RTKMSG_REMOVE_LATER		0x5AB30001
#define RTKMSG_DESTROY_POPUP	0x5AB30002
#define RTKMSG_KILL_POPUP		0x5AB30003
#define RTKMSG_DELETE_PTR		0x5AB30004

// I don't want to include OS headers in the toolkit headers...
#ifdef XLIB
typedef uptr RWND;
#else
typedef void* RWND;
#endif

//#define RWindow_P RControl
//class RWindow : public RControl
#define RWindow_P RHVBox
class RWindow : public RHVBox
{
protected:
	RWindow() {};
	void Init(RWindow *rw,int x,int y,int w,int h);
	void Dinit();// { RWindow_P::Dinit(); if(childw) delete childw; if(nextw) delete nextw; Destroy(); };
	void DestroyChain();
public:
	RWindow(RWindow *rw,int x,int y,int w,int h) { Init(rw,x,y,w,h); };
	~RWindow() { DINIT(); };
	SISLTN();

	bool IsWindow() { return true; };
	void GetDrawer(u32 d);
	void AddWindow(RWindow *);
	void DelWindow(RWindow *);

	void ShowW();
	void HideW();
	void EnableW();
	void DisableW();

	void SetPosW(int,int);
	void SetSizeW(int,int);

	void SetLTRB(int l,int t,int r,int b);
//x	void SetPos F(int,int);
//	void SetPosR(int,int);
//x	void SetSize F(int,int);
	//RWindow *GetParentWR();

	// mouse
	void SetMHit(RControl *c);
	void MRawButton(u32 b);
	void MRawMove(int x, int y);
	void MRawScroll(int b, int d);

	// mouse
	void MMove() {};
	void MScroll(int b,int d) {};
	void MButton(u32 b) {};

	// keyboard
	void KRawUp(u32 d);
	void KRawDown(u32 d);
	void KRawChar(u32 d);
	void KGrab(RControl *c);
	void KUnGrab(RControl *c);
	void KMFree(RControl *c);

	// window
	void SetText(const char *c);
	void SetFocusW();
	void Close();
	virtual void Destroy();

	virtual bool OnClose() { return true; };
	virtual void OnDestroy() {};
	virtual void OnFocus() {};
	virtual void OnUnfocus(RWindow *to) {};

	//void DeleteLater();
	void RTKMsg(u32 id, void *p1=0);
	void RTKMsgH(u32 id, void *p1);

	virtual void UnfocusPopups() { SetFocusW(); };
	virtual bool KillPopup(RWindow *w) { return true; };

//-----------------------------------------------

	virtual u32 RWndProc(void *);

#ifdef XLIB
	virtual void InitWin();
#endif
	virtual void Create(RWindow *w);

//	SISL(3);
//	SLOT(RePos,(int x,int y)); xxx
//	SLOT(ReSize,(int w,int h)); xxx
//	SIGNAL(Si_MRawButton,(int b)); ?????
//	SIGNAL(Si_MRawMove,(int x, int y));
//	SIGNAL(Si_MRawScroll,(int b, int d));

//-----------------------------------------------

	RWindow *parentw,*childw,*nextw,*prevw,*lastw;
	int win_x,win_y;
	RWND hwnd;
	RControl *rcTT;
	RControl *rcMHit;
	RControl *rcMGrab;
	RControl *rcKGrab;
	u16 MDrag;
	u16 MBut;

	// mouse data
	int mx,my,rx,ry,ox,oy;

	struct {
		bool bAlt:1,bCtrl:1,bShift:1;
	};
};

#define RMainWindow_P RWindow
class RMainWindow : public RWindow
{
protected:
	RMainWindow() {};
	void Init(RWindow *rw,int x,int y,int w,int h)
	{	RMainWindow_P::Init(rw,x,y,w,h); bQuit = false; };
	//void Dinit() { /*DestroyChain();*/ RMainWindow_P::Dinit(); };
public:
	RMainWindow(int x,int y,int w,int h) { Init(NULL,x,y,w,h); };
	~RMainWindow() { DINIT(); };
	SISLTN();

	void Run();
	void Create(RWindow *w = NULL);
#ifdef XLIB
	void Destroy() { bQuit = true; };
#endif

	u32 RWndProc(void *);

/*
#ifndef XLIB
	void Destroy() { if(hwnd) { KillTimer(hwnd,1); RMainWindow_P::Destroy(); } }; //win32
#else
	void Destroy() { if(hwnd) { RMainWindow_P::Destroy(); } }; //Xlib
#endif*/

	void OnTimer();
	bool bQuit;
};



//#ifdef WINX

#define RPopup_P RWindow
class RPopup : public RWindow // Win32
{
protected:
	RPopup() {};
	void Init(RWindow *rw,int x,int y,int w,int h);
public:
	RPopup(RWindow *rw,int x=0,int y=0,int w=0,int h=0) { Init(rw,x,y,w,h); bSelfDelete = true; };
	~RPopup() { DINIT(); };
	SISLTN();

	void OnUnfocus(RWindow *to);
	bool KillPopup(RWindow *w);
	void UnfocusPopups() { if(parentw) parentw->UnfocusPopups(); }

	
	void Popup(RWindow *w);
	void Create(RWindow *w);
	void ReCalcI(bool b=true);

	bool bSelfDelete;
};

#define RToolTip_P RPopup
class RToolTip : public RPopup
{
protected:
	RToolTip() {};
	void Init(RWindow *rw,int x,int y,int w,int h);
public:
	void GetDrawer(u32);
	RToolTip(RWindow *rw,int x=0,int y=0,int w=0,int h=0) { Init(rw,x,y,w,h); };
	RToolTip(RWindow *rw,int x,int y,const char *c);
	void ShowTT(int x,int y,const char *c);
	~RToolTip() { DINIT(); };
	SISLTN();

	void Create(RWindow *w);

	bool OnClose();
	void OnUnfocus(RWindow *to);
	void SetText(const char *c) { ttext = c; };

	const char *ttext;
};

//#endif //WINX

#define RApp_P CConn
class RApp : public CConn
{
protected:
	void Dinit() { RApp_P::Dinit(); if(wnd)	delete wnd; wnd = NULL; };
public:
	RApp() { wnd = 0; };
	~RApp() { DINIT(); };
	void Run();
	

	SISL(1);
	SLOT(Sl_Quit)();
	SLOT(Sl_ShowIt)();

	RMainWindow *wnd;
};

#ifdef WIN32
#define RMain() __stdcall WinMain(void *v1,void *v2,char *cmdline,int show)
#endif

#ifdef XLIB
#define RMain() main()
#endif

//XXX circular dependency hack...
inline int RControl::GetMX() { return wnd->mx - left; };
inline int RControl::GetMY() { return wnd->my - top; };
inline int RControl::GetMOX() { return wnd->ox - left; };
inline int RControl::GetMOY() { return wnd->oy - top; };
inline int RControl::GetMRX() { return wnd->rx; };
inline int RControl::GetMRY() { return wnd->ry; };

#endif //WINDOW_H
