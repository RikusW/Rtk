// vim:ts=4 sts=0 sw=4

#include "Rtk.h"

#ifdef WIN32
#define NO_STRICT
#include <windows.h>
#endif

#ifdef XLIB //----------+++
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/Xos.h>
#include <X11/Xatom.h>

Time RtkXTime = 0;
Display *rdisplay;
RWND wFocus=0;

Atom RTK_MSG = 0;
Atom WMDelWin = 0;
Atom WMTakeFocus = 0;
Atom WMProtocols = 0;

#if 1
#define USECTX
#include <X11/Xresource.h>
#else
#undef USECTX
#endif //1
void ClientMsg(Atom t, Atom a, RWND wnd);
#endif //---------------+++

bool bDropWmSize = false;
RWindow *WtoRW(RWND w);

//============================================================================

GETDRAWER(RWindow,RWINDOW);
void RWindow::Init(RWindow *rw,int x,int y,int w,int h)
{
	wnd = this; hwnd = 0; rcTT = 0;

	RWindow_P::Init((RContainer*)NULL);
	SetSize(w,h);

	rcMHit = NULL;
	rcMGrab = NULL; rcKGrab = NULL;
	bShift = bCtrl = bAlt = false;
	MDrag = MBut = 0;
	parentw = childw = nextw = prevw = lastw = NULL;

	if(rw) {
		win_x = x + rw->win_x; // x is relative to desktop
		win_y = y + rw->win_y;
		rw->AddWindow(this);
	}else{
		win_x = x;
		win_y = y;
	}
};

void RWindow::Dinit()
{
	RWindow_P::Dinit();
	
	if(childw)
		delete childw;
	if(nextw)
		delete nextw;

	childw = 0;
	nextw = 0;

	Destroy();
};

void RWindow::DestroyChain()
{
	if(childw)
		childw->DestroyChain();
	if(nextw)
		nextw->DestroyChain();

	RWindow::Destroy();
}

void RWindow::AddWindow(RWindow *w)
{
/*	if(!wnd) { //XXX
		MessageBox(0,"wnd not set yet.","Error",0); //win32
		return;
	}*/


//	w->next = NULL;
	w->parent = this; //???XXX
/*	if(lastw) {
		lastw->next = w;
		w->prev = lastw;
	//	lastw = w;
	}//else{
	//	childw = lastw = w;
	//}*/


	w->nextw = NULL;
	w->parentw = this;
	if(lastw) {
		lastw->nextw = w;
		w->prevw = lastw;
		lastw = w;
	}else{
		childw = lastw = w;
	}
}

void RWindow::DelWindow(RWindow *w)
{
	RWindow *c = childw;

	while(c) {
		if(c == w) {
			if(c->prevw) {
				c->prevw->nextw = c->nextw;
			}else{
				if(c->parentw == c) break; // only for toplevel window....
				c->parentw->childw = c->nextw;
			}

			if(c->nextw) {
				c->nextw->prevw = c->prevw;
			}else{
				c->parentw->lastw = c->prevw;
			}
			break;
		}
		c = c->nextw;
	}
}

//=============================================================================

#ifdef WIN32

void RWindow::ShowW()
{
	ShowWindow(hwnd,SW_SHOW); //win32
};

void RWindow::HideW()
{
	ShowWindow(hwnd,SW_HIDE); //win32
};

void RWindow::EnableW()
{
	EnableWindow(hwnd,true); //win32
};

void RWindow::DisableW()
{
	EnableWindow(hwnd,false); //win32
};

#endif
#ifdef XLIB

void RWindow::ShowW()
{
	if(hwnd)
		XMapWindow(rdisplay,hwnd);
};

void RWindow::HideW()
{
	if(hwnd)
		XUnmapWindow(rdisplay,hwnd);
};

void RWindow::EnableW()
{
	printf("enable window");
};

void RWindow::DisableW()
{
	printf("disable window");
};

#endif

//=============================================================================

// WIN32 xy is relative to the desktop
// XLIB  xy is relative to the parent window
// RTK   xy is relative to the parent window

void RWindow::SetPosW(int x,int y)
{
	RWindow *w = wnd ? wnd->parentw : NULL;
	if(w) {
		x += w->win_x;
		y += w->win_y;
	}
	win_x = x;
	win_y = y;
	
	if(hwnd)
#ifdef WIN32
		SetWindowPos(hwnd,HWND_TOP,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER); //win32
#endif
#ifdef XLIB
		XMoveWindow(rdisplay,hwnd,x,y);
#endif
}

void RWindow::SetSizeW(int w,int h)
{
#ifdef WIN32
	SetWindowPos(hwnd,NULL,0,0,w,h,SWP_NOMOVE|SWP_NOZORDER); //win32
#endif
#ifdef XLIB
	XResizeWindow(rdisplay,hwnd,w,h);
#endif
}

void RWindow::SetLTRB(int l,int t,int r,int b)
{
	left = 0;
	top = 0;
	right = r - l;
	bottom = b - t;
	width = right + 1;
	height = bottom + 1;

	if(width < 10) { width = 10; right = 9; }
	if(height< 10) { height= 10; bottom = 9; }
}

/*
RWindow *RWindow::GetParentWR()
{
	RWindow *w = wnd;
	while(1) {
		if(w->parentw)
			w = w->parentw;
		else
			break;
	}
	return w;
}*/

//=============================================================================
//mouse

void RWindow::SetMHit(RControl *c)
{
	rcMHit = c;
}

void RWindow::MRawButton(u32 b)
{
	if(b & M_DOWN) {
		MDrag |= 1 << (b & M_MASK);
	}else{
		MDrag &= ~(1 << (b & M_MASK));
	}
	
	// TODO add a listening interface
//	Si_MRawButton(b);

	RControl *g = rcMGrab;
	if(rcMGrab) {
		if(!(b & M_DOWN) && b == MBut) {
			if(rcMGrab == rcMHit) // is pointer inside button ?
				b |= M_CLICK;

			MBut = 0;
			rcMGrab = NULL;
#ifdef WIN32
			ReleaseCapture();
#endif
		}
		g->MRawButton(b);
		return;
	}
	if(b & M_DOWN && !rcMGrab && rcMHit)  {
		MBut = (u16)b & M_MASK;
		rcMGrab = rcMHit;
#ifdef WIN32
		SetCapture(hwnd);
#endif
	}
	
	if(rcMHit) {
		rcMHit->MRawButton(b);
	}
}

void RWindow::MRawMove(int x, int y)
{
	mx = x; rx = mx - ox; ox = mx;
	my = y; ry = my - oy; oy = my;

	if(!rcMHit)
		rcMHit = this;
	rcMHit = rcMHit->MRawHitTest(x,y,true);

//	Si_MRawMove(x,y);
	
	if(rcMGrab) {
		rcMGrab->MRawMove(x,y);
		return;
	}

	if(rcMHit) {
		rcMHit->MRawMove(x,y);
	}
}

void RWindow::MRawScroll(int b, int d)
{
//	Si_MRawScroll(b,d);
	
	if(rcMGrab) {
		rcMGrab->MRawScroll(b,d);
		return;
	}

	if(rcMHit) {
		rcMHit->MRawScroll(b,d);
	}
}

// mouse
//=============================================================================
// keyboard

void RWindow::KRawUp(u32 d)
{
	switch(d) {
	case RK_SHIFT: bShift = false; break;
	case RK_CTRL: bCtrl = false; break;
	case RK_ALT: bAlt = false; break;
	case RK_ESCAPE: Close(); break; //XXX tmp
	}
	//Si_KUp(d);

	if(rcKGrab)
		rcKGrab->KUp(d);
}

void RWindow::KRawDown(u32 d)
{
	switch(d) {
	case RK_SHIFT: bShift = true; break;
	case RK_CTRL: bCtrl = true; break;
	case RK_ALT: bAlt = true; break;
	//case 'Q': align ^= 0x10;				//XXX DEBUG
	case 'R': ReCalc(); Draw(true); break;	//XXX DEBUG
	case RK_TAB: { //TODO implement proper TAB ordering -- move to RDialog
		if(rcKGrab) {
			if(rcKGrab->next) {
				rcKGrab->next->Sl_Focus();
			}else{
				rcKGrab->parent->child->Sl_Focus();
			}

		}else{
			KGrab(child);
		}
		break;
		}
	}
	//Si_KDown(d);

	if(rcKGrab)
		rcKGrab->KDown(d);
}

void RWindow::KRawChar(u32 d) //win32
{
	if(rcKGrab)
		rcKGrab->KChar(d);
}

void RWindow::KGrab(RControl *c)
{
	if(rcKGrab != c) {
		if(rcKGrab)
			rcKGrab->Sl_UnFocus();
		rcKGrab = c;
	}
}

void RWindow::KUnGrab(RControl *c)
{
	if(rcKGrab == c)
		rcKGrab = NULL;
#ifdef XLIB		
	else
		printf("KUnGrab fail...\n");
#endif
}

// used by Sl_Hide and Sl_Disable
void RWindow::KMFree(RControl *c)
{
	if(!c)
		return;

	if(c->IsChildR(rcKGrab)) {
		if(rcKGrab->bFocus) {
			rcKGrab->Sl_UnFocus();
		}else{
			rcKGrab = NULL;
#ifdef XLIB		
			printf("KUnGrabR XXX...\n");
#endif
		}
	}

	if(c->IsChildR(rcMGrab)) {
		MBut = 0;
		rcMGrab = NULL;
#ifdef WIN32
		ReleaseCapture();
#endif
	}

	if(c->IsChildR(rcMHit)) {
		for(RControl *r = rcMHit; r; r = r->parent) {
			r->MRawExit(true);
			if(r != r->parent) { //root ?
				r->parent->MRawEnter(false);
			}else{
				rcMHit = 0;
				break;
			}
			if(r == c) {
				rcMHit = c->parent;
				break;
			}
		}
	}
}

// keyboard
//=============================================================================

void RWindow::SetText(const char *c)
{
	RWindow_P::SetText(c); //??? move up ???
	if(hwnd)
#ifdef WIN32 
		SetWindowText(hwnd,text);
#endif
#ifdef XLIB
		XStoreName(rdisplay,hwnd,text);
#endif
};

void RWindow::SetFocusW()
{
	if(hwnd) {
#ifdef WIN32
		::SetFocus(hwnd);
#endif
#ifdef XLIB
		wFocus = hwnd; // needed ???
		XSetInputFocus(rdisplay,hwnd,RevertToParent,RtkXTime);
#endif
	}
	if(rcKGrab)
		rcKGrab->Sl_Focus(); //TODO set focus to init or <last focus> control here...
	else
	if(child)
		child->Sl_Focus();
}

void RWindow::Close()
{
	if(hwnd)
#ifdef WIN32
		PostMessage(hwnd,WM_CLOSE,0,0);
#endif
#ifdef XLIB
		ClientMsg(WMProtocols, WMDelWin, hwnd);
#endif
}

void RWindow::Destroy()
{
	if(hwnd) {
		OnDestroy();
#ifdef WIN32		
		DestroyWindow(hwnd);
#endif
#ifdef XLIB
#ifndef USECTX		
		TR_RM(this);
#endif		
		XDestroyWindow(rdisplay,hwnd);
#endif
	}
	hwnd = 0;
}

//=============================================================================
#ifdef WIN32 //misc
#define RTK_MSG (WM_USER+14)

void RWindow::RTKMsg(u32 id, void *p1)
{
	PostMessage(wnd->hwnd,RTK_MSG,(WPARAM)id,(LPARAM)p1);
}

#endif
//======================
#ifdef XLIB

void RWindow::RTKMsg(u32 id, void *p1)
{
	printf("RTKMsg %x\n",(uint)id & RTKMSG_MASK);
	
	XClientMessageEvent c;
	c.type = ClientMessage;
	c.serial = 0;
	c.display = rdisplay;
	c.window = hwnd;
	c.message_type = RTK_MSG;
	c.format = 32;
	c.data.l[0] = id;
	c.data.l[1] = (uptr)p1;
	c.data.l[2] = ((uptr)p1) >> 32;
	XSendEvent(rdisplay,hwnd,0,0,(XEvent*)&c);
}

void ClientMsg(Atom t, Atom a, RWND wnd)
{
	printf("clientmessage %i\n",(int)a);
	
	XClientMessageEvent c;
	c.type = ClientMessage;
	c.serial = 0;
	c.display = rdisplay;
	c.window = wnd;
	c.message_type = t;
	c.format = 32;
	c.data.l[0] = a;	
	XSendEvent(rdisplay,wnd,0,0,(XEvent*)&c);
}

#endif //misc
//=============================================================================

void RWindow::RTKMsgH(u32 id, void *p1)
{
	if((id & RTKMSG_MAGIC_MASK) != RTKMSG_MAGIC) {
		//printf("Invalid RTKMsg = %x - %x\n",id,p1);
		return;
	}

	switch(id) {
	case RTKMSG_REMOVE_LATER: {
		RControl *c = (RControl*)p1;
		if(c) {
			if(rcMGrab == c) rcMGrab = NULL;
			if(rcKGrab == c) rcKGrab = NULL;
			c->Remove();
		}
		ReCalc();
		Draw(true);
		return;
	}
	case RTKMSG_DESTROY_POPUP: {
		UnfocusPopups();
		//KillPopup(0);
		return;
	}
	case RTKMSG_KILL_POPUP: {
		KillPopup((RWindow*)p1);
		return;
	}
	case RTKMSG_DELETE_PTR: {
		delete (RControl*)p1;
	}
	}
}

	/*if(a == RT MSG_DELETE_LATER) {
		//printf("mt = %x WMP = %x\n",xev->xclient.message_type, RTK_MSG);
		Destroy();	
		if(parentw) parentw->DelWindow(this);
		parentw = nextw = prevw = NULL; //???
		delete this;
		break;
	}*/
/*case WM_USER+1: // DestroyLater
	{
		RControl *c = (RControl*)lParam;
		if(c) {
			if(rcMGrab == c) rcMGrab = NULL;
			if(rcKGrab == c) rcKGrab = NULL;
//			c->wnd->Destroy();
//			c->wnd->K illPopup(0);

			//wnd->GetParentWR()->SetFocusW(); //XXX ???
			//---VS---
			RWindow *w = wnd;
			while(1) {
				if(w->parentw)
					w = w->parentw;
				else
					break;
			}
			w->Sl_Focus();
			::SetFocus(w->hwnd);
		}
		break;
	}*/

//=============================================================================

void RMainWindow::OnTimer()
{
//---------------------------
#ifndef XLIB
/*
	static int t = 0, tt = 0;
	static RControl *h = NULL, *htt = NULL;

	//----------Common-------------
	t++; tt+=100;
	if(h != rcMHit) {
		t = 0;
		if(h == rcTT && rcMHit == htt) { // moving from the tt back to its control
			h = rcMHit;
			break;
		}else
			h = rcMHit;

		// moving from one control to another
		if(tt < 100000) tt = 0; else goto j_ttx; 
		break;
	}
	//-----------Hover-------------
	if(t > 2) {
		if(rcMHit)
			rcMHit->MHover();
	//	MRawHover();
		t = 0x80000000;
	}
	//-----------ToolTip-----------
	if(tt == 1000) {
		tt = 0;
j_ttx:		if(h && h->tooltip) {
			if(rcTT)
				((RToolTip*)rcTT)->ShowTT(cx+left+14, cy+top, h->tooltip);
			else
				rcTT = new RToolTip(this, cx+left+14, cy+top, h->tooltip);
			tt = 100000;
			htt = h;
		}
	}
	if(tt >= 100000) {
		if(htt == h || rcTT == h) { // when on the control or its tt
			tt = 100000;
		}else{
			if(tt > 100800) tt = 0;
			if(tt == 100100) { // hide the tooltip
				SetFocus(((RToolTip*)rcTT)->parentw->hwnd); //win32
			}
		}
	}
	//-----------ToolTip-----------
	break;*/
#endif //XLIB
//---------------------------
}

//=============================================================================
#ifdef WIN32 // RWndProc

struct WMsg
{
	HWND hwnd;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
};

u32 RMainWindow::RWndProc(void *pv) //win32
{
	WMsg *w = (WMsg*)pv;
	switch(w->msg) {
	case WM_TIMER:
		OnTimer();
		break;
	case WM_DESTROY:
	//	if(!GetParent(hwnd))
			PostQuitMessage(0);
		break;
//	case WM_MOUSEMOVE:
//		rc RawHit = NULL; // maybe put this somewhere else ???
	default:
		return RMainWindow_P::RWndProc(pv);
	}
	return 0;
}

u32 RWindow::RWndProc(void *pv) //win32
{
	WMsg *w = (WMsg*)pv;
	switch(w->msg)
	{
	case WM_MOUSEMOVE:		MRawMove(LOWORD(w->lParam),HIWORD(w->lParam)); break;
	case WM_LBUTTONDBLCLK:	MRawButton(1 | M_DOWN | M_DBL); break;
	case WM_MBUTTONDBLCLK:	MRawButton(2 | M_DOWN | M_DBL); break;
	case WM_RBUTTONDBLCLK:	MRawButton(3 | M_DOWN | M_DBL); break;
	case WM_LBUTTONDOWN:	MRawButton(1 | M_DOWN); break;
	case WM_MBUTTONDOWN:	MRawButton(2 | M_DOWN); break;
	case WM_RBUTTONDOWN:	MRawButton(3 | M_DOWN); break;
	case WM_LBUTTONUP:		MRawButton(1); break;
	case WM_MBUTTONUP:		MRawButton(2); break;
	case WM_RBUTTONUP:		MRawButton(3); break;

	case WM_CHAR:		KRawChar(w->wParam); break;
	case WM_KEYUP:		KRawUp(w->wParam);   break;
	case WM_KEYDOWN:	KRawDown(w->wParam); break;

	case WM_PAINT:
		Draw(true);
		ValidateRect(hwnd,NULL);
		break;
//	case WM_NCPAINT:
//		break;

	case WM_ERASEBKGND:
		return 1;

	case WM_MOVE:
		win_x = LOWORD(w->lParam);
		win_y = HIWORD(w->lParam);
	//	SetPos F(LOWORD(w->lParam),HIWORD(w->lParam));
		break;

	case WM_SIZE: // needs attention.... for looping.....
		if(bDropWmSize)
			break;
		SetSize(LOWORD(w->lParam),HIWORD(w->lParam));
		ReCalc();
		break;

	case RTK_MSG:
		RTKMsgH((u32)w->wParam,(void*)w->lParam);
		break;

	case WM_CLOSE:
		if(OnClose())
			Destroy();
		break;
		
	case WM_SETFOCUS:
		OnFocus();
		break;

	case WM_KILLFOCUS:
		if(w->wParam)
			OnUnfocus(WtoRW((RWND)w->wParam));
		break;

	// this was tested in both win98 and XP not sure about win95...
	//case WM_MOUSEWHEEL: buggy #defines in winuser.h......
	case 0x020A:
		MRawScroll(1,((short) HIWORD(w->wParam)) > 0 ? -1 : 1);
		break;

	default:
	/*	if(msg == rollmsg) { // win95 hack for mouse wheel messages
			MRawScroll(1,((int)wParam) > 0 ? -1 : 1);
		}else//*/
			return DefWindowProc(w->hwnd,w->msg,w->wParam,w->lParam);
	}
	return 0;
}

#endif
//=============================================================================
#ifdef XLIB // RWndProc

#ifdef USECTX
XContext RWindowCTX = 0;
#endif

u32 RMainWindow::RWndProc(void *pv)
{
	return RMainWindow_P::RWndProc(pv);
	//OnTimer(); //XXX...
}

u32 RWindow::RWndProc(void *pv)
{
	XEvent *xev = (XEvent*)pv;
	//static int xx = 0;
	switch (xev->type) {
	case MotionNotify:
		MRawMove(xev->xmotion.x, xev->xmotion.y);
		break;
	case ButtonPress:
		switch(xev->xbutton.button) {
			case 1: MRawButton(1 | M_DOWN); break;
			case 2: MRawButton(2 | M_DOWN); break;
			case 3: MRawButton(3 | M_DOWN); break;
			case 4: MRawScroll(1,-1); break;
			case 5: MRawScroll(1,1); break;
		}
		break;
		//XXX MRawButton(1 | M_DBL); ??? doubleclick ???
	case ButtonRelease:
		switch(xev->xbutton.button) {
			case 1: MRawButton(1); break;
			case 2: MRawButton(2); break;
			case 3: MRawButton(3); break;
		}
		break;
	case EnterNotify:
		//MEnter();
		break;
	case LeaveNotify:
		//MExit();
		break;
		
	case KeyPress: {
			char buf[10];
			KeySym ks;// = XLookupKeysym(&xev->xkey,0);
			int l = XLookupString(&xev->xkey, buf, 6, &ks, 0); buf[l] = 0;
			if(ks >= 'a' && ks <= 'z') //toupper
				ks &= 0xDF;
			KRawDown(ks);

			char *c = XKeysymToString(ks);
			if(!c) c = (char*)"XX";
//			printf("KeySym == %x - %s = %s\n",(uint)ks,c,buf);
			
			if(!l) break;
			if(l == 1) 
				KRawChar(buf[0]);
			else
				printf("DBG: KChar = %i - %s\n",l,buf);
		}
		break;
	case KeyRelease: {
			KeySym ks = XLookupKeysym(&xev->xkey,0);
			KRawUp(ks);
		}
		break;
	case MappingNotify:
		XRefreshKeyboardMapping(&xev->xmapping);
		break;

	case Expose:
		//printf("drawing %i\n",xx++);
		while(XCheckTypedWindowEvent(rdisplay, hwnd, Expose, xev));
		Draw(true);
		break;

	case ConfigureNotify:
		while(XCheckTypedWindowEvent(rdisplay, hwnd, ConfigureNotify, xev));
		win_x = xev->xconfigure.x;
		win_y = xev->xconfigure.y;
		width = xev->xconfigure.width;
		height = xev->xconfigure.height;
		
		// needs attention.... for looping.....
		if(bDropWmSize)
			break;
		SetSize(width,height); //.....
		ReCalc();
		break;

		case FocusIn:
			OnFocus();
			break;

		case FocusOut:
		{
			int i;
			RWND w;
			// get the new focus
			XGetInputFocus(rdisplay,&w,&i);
			if(w!=hwnd) {
				RWindow *rw = WtoRW(w);
				if(!rw) {
					printf("dd> RPopup FocusOut TR_GW fail...\n");
				}
				OnUnfocus(rw);
			}
		}
		break;

	case ClientMessage:
	//-----------------------------------------------------------
	if(xev->xclient.message_type == RTK_MSG) {
		uptr u = xev->xclient.data.l[2];
		u <<= 32;
		u |= xev->xclient.data.l[1];
		RTKMsgH(xev->xclient.data.l[0],(void*)u);
		break;
	}
	//-----------------------------------------------------------
	if(xev->xclient.message_type == WMProtocols) {
		if(((Atom)xev->xclient.data.l[0]) == WMDelWin) { // "WM_CLOSE"
			//printf("mt = %x WMP = %x\n",xev->xclient.message_type, WMProtocols);
			if(OnClose())
				Destroy();
		}/*else
		if(((Atom)xev->xclient.data.l[0]) == WMTakeFocus) {
			printf("========wm_take_focus %x========\n",(uint)hwnd);
			XSetInputFocus(rdisplay,hwnd,RevertToParent,(Time)xev->xclient.data.l[1]);
		}//*/
		break;
	}
	//-----------------------------------------------------------
	} //switch

	return 0;
}

#endif // Xlib RWndProc
//=============================================================================
#ifdef WIN32 // message loop

RWindow *WtoRW(RWND w)
{
	return (RWindow*)GetWindowLong(w,GWL_USERDATA);
}

LRESULT CALLBACK RWndProc(HWND h,UINT m,WPARAM w,LPARAM l) //win32
{
	RWindow *rw = WtoRW(h);
	if(rw == NULL)
	switch(m) {
		case WM_NCCREATE:
		case WM_CREATE:
			{
				//u32 *d = (u32*)l; // == lpCreateParams
				//rw = (RWindow*)*d;
				rw = (RWindow*)*((u32*)l);
				SetWindowLong(h,GWL_USERDATA,(long)rw);
				break;
			}
		default:
			return DefWindowProc(h,m,w,l);
	}
	WMsg ww;
	ww.hwnd=h;
	ww.msg=m;
	ww.wParam=w;
	ww.lParam=l;
	return rw->RWndProc(&ww);
}

void RMainWindow::Run() //win32
{
	MSG msg;

	while(GetMessage(&msg,0,0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#endif
// win32 message loop
//=============================================================================
// Xlib message loop
#ifdef XLIB
/////////////////////////////////////////
// TRanslate RWND to RWindow*
int iTR = 0;
#ifndef USECTX

struct wtr
{
	RWND w;
	RWindow *r;
};

wtr TR[100];

void TR_ADD(RWindow *r)
{
	TR[iTR].w = r->hwnd;
	TR[iTR].r = r;
	iTR++;
}

void TR_RM(RWindow *r)
{
	int i,j,k = iTR;
	for(i=0,j=0; j<k; i++,j++) {
		if(TR[j].r == r) {
			j++;
			iTR--;
			if(j>=k) break;
		}
		TR[i] = TR[j];
	}
}

RWindow *TR_GW(RWND w)
{
	// RWND to RWindow* lookup
	for(int i=0; i<iTR; i++) {
		if(w == TR[i].w) {
			return TR[i].r;
		}
	}
	return 0;
}
#endif
// TRanslate RWND to RWindow*
/////////////////////////////////////////
//=============================================================================

void PrintXev(XEvent *,bool);

RWindow *WtoRW(RWND w)
{
#ifdef USECTX		
	RWindow *rw;
	if(XFindContext(rdisplay,w,RWindowCTX,(XPointer*)&rw) == XCNOENT) {
		printf("WtoRW CTX not found... setting rw = 0\n");
		rw = 0;
	}
#else
	RWindow *rw = TR_GW(w);
#endif
	return rw;
}

void RMainWindow::Run()
{
	XEvent xev;
	RWindow *w;

	while(!bQuit) {
		XNextEvent(rdisplay, &xev);

		// do some filtering to make XEvents more Windows like
		switch(xev.type) {
		case ButtonPress:
			if(xev.xbutton.button <= 3)
				wFocus = xev.xbutton.window;
			RtkXTime = xev.xbutton.time; // used by XSetInputFocus
			break;
		case ButtonRelease:
			RtkXTime = xev.xbutton.time; // used by XSetInputFocus
			break;
		case KeyRelease: // I don't want KeyRelease while repeating
			XCheckTypedWindowEvent(rdisplay, xev.xany.window, KeyPress, &xev); 
			RtkXTime = xev.xkey.time;
			break;
		case KeyPress:
			RtkXTime = xev.xkey.time;
			break;
		case FocusIn:
		case FocusOut:
			switch(xev.xfocus.detail) {
			case NotifyAncestor:
			case NotifyInferior:
			case NotifyNonlinear:
				break;
			default:
				//printf("=======UNWANTED FOCUS MSG=======\n");
				//PrintXev(&xev,0);
				continue; // not interested in the other focus stuff...
			}
			break;
		}

		// Get the RWindow* from the RWND
		w = WtoRW(xev.xany.window);

		if(!w) {
			//printf("TR_GW fail...\n");
			//PrintXev(&xev,0);
			continue;
		}else{
			//PrintXev(&xev,1);
		}

		// RWindow message handler
		w->RWndProc((void*)&xev);

		// Set Focus on ButtonPress
		if(xev.type == ButtonPress && xev.xbutton.button <= 3) {
			int i;
			RWND w;
			XGetInputFocus(rdisplay,&w,&i);
			if(w != wFocus && wFocus) {
				printf("=======FOCUS click=======\n");
				XSetInputFocus(rdisplay,wFocus,RevertToParent,xev.xbutton.time);
			}
		}
	}
	DestroyChain();
    XCloseDisplay(rdisplay);
}

#endif // Xlib message loop
//=============================================================================
// Popup

void RPopup::Init(RWindow *rw,int x,int y,int w,int h)
{
	 RPopup_P::Init(rw,x,y,w,h);
}

void RPopup::OnUnfocus(RWindow *rw)
{
	RTKMsg(RTKMSG_KILL_POPUP,rw);
/*#ifdef XLIB
	KillPopup(rw);
#else
	// User32 send WM_KILLFOCUS before WM_LBUTTONDOWN
	RTKMsg(RTKMSG_KILL_POPUP,rw);
#endif*/
}

bool RPopup::KillPopup(RWindow *w)
{
	if(w == this) return true; // ourself
	if(w && w->parentw == this) return true; // a child menu
	
	Destroy();

	if(parentw)
		parentw->KillPopup(w);

	if(bSelfDelete) {
		if(parentw) parentw->DelWindow(this);
		parentw = nextw = prevw = NULL; //???
		RTKMsg(RTKMSG_DELETE_PTR,this);
//		delete this;
	}

	return true;
}

void RPopup::ReCalcI(bool b)
{
	RPopup_P::ReCalcI(b);
	bDropWmSize = true;
#ifdef WIN32
	RECT r;
	r.left = win_x;	r.right = r.left + width;
	r.top = win_y;	r.bottom = r.top + height;
	AdjustWindowRect(&r,WS_POPUP | WS_SIZEBOX ,false);
	SetWindowPos(hwnd,NULL,0,0,r.right-r.left,r.bottom-r.top,SWP_NOMOVE|SWP_NOZORDER); //win32
#endif
#ifdef XLIB
	XEvent xev;
	XResizeWindow(rdisplay,hwnd,width,height);
	while(XCheckTypedWindowEvent(rdisplay,hwnd,ConfigureNotify,&xev));
#endif
	bDropWmSize = false;
}

void RPopup::Popup(RWindow *w)
{
	Create(w);
//	SetAlignment(AL_V|AL_STRETCHH|AL_STRETCHV);//| AL_FITV | AL_FITH);
	SetAlignment(AL_V | AL_FITV | AL_FITH);
	ReCalc();
	ShowW();
	SetFocusW();
}

// Popup
//=============================================================================
// ToolTip

GETDRAWER(RToolTip,RTOOLTIP);
void RToolTip::Init(RWindow *rw,int x,int y,int w,int h)
{
//f	frametype = FT_NONE;
	RToolTip_P::Init(rw,x,y,w,h);
	ttext = "---111---";
}

RToolTip::RToolTip(RWindow *rw,int x,int y,const char *c)
{
	if(!c || !*c) {
		c = "...Empty...";
	}

	int w = strlen(c) * 9 + 12; //XXX FONT REIMPLEMENT THIS!!!!!
	Init(rw,x,y,w,21);
	ttext = c;

	Create(rw);
}

void RToolTip::ShowTT(int x,int y,const char *c)
{
	if(!c || !*c) {
		c = "...Empty...";
	}

	int w = strlen(c) * 9 + 12; //XXX REIMPLEMENT THIS!!!!!
	ttext = c;

	SetPosW(x,y);
	SetSizeW(w,21);
	ShowW();
}

bool RToolTip::OnClose()
{
	return false;
}

void RToolTip::OnUnfocus(RWindow *rw)
{
	HideW();
}

// ToolTip
//=============================================================================
#ifdef WIN32 // window creation

void RMainWindow::Create(RWindow *w) //win32
{
	RMainWindow_P::Create(w);
//	SetTimer(hwnd,1,100,NULL);
}

void InitCls(HBRUSH hb, LPCTSTR cls)
{
	WNDCLASS wd;
	memset(&wd,0,sizeof(WNDCLASS));
	wd.hIcon=NULL;
	wd.lpszMenuName=NULL;
	wd.hInstance=GetModuleHandle(0);
	wd.hCursor=LoadCursor(NULL,IDC_ARROW);
	wd.style=CS_BYTEALIGNCLIENT|CS_DBLCLKS | CS_HREDRAW|CS_VREDRAW;
	wd.lpfnWndProc=::RWndProc;
	wd.cbClsExtra=4;
	wd.hbrBackground=hb; //TODO winbackcolor
	wd.lpszClassName=cls;
	RegisterClass(&wd);
}

void RWindow::Create(RWindow *w)
{
	if(::IsWindow(hwnd)) return; // already created ?

	InitCls(NULL,"RwTkWindow");

	RECT r;
	r.left = win_x;	r.right = win_x + width;
	r.top = win_y;	r.bottom = win_y + height;
	AdjustWindowRect(&r,WS_OVERLAPPEDWINDOW|WS_VISIBLE,false);

	// M$ have a off by one bug... XXX x+w=r? XXX actually <x+w-1=r>
	hwnd=CreateWindow("RwTkWindow","Test 123",WS_OVERLAPPEDWINDOW,//|WS_VSCROLL,
		r.left,r.top,r.right-r.left,r.bottom-r.top,0,NULL,GetModuleHandle(0),this);

	SetWindowLong(hwnd,GWL_USERDATA,(LONG)this);
	ShowWindow(hwnd,SW_SHOW);

	// win95 wheel hack
//	rollmsg = RegisterWindowMessage("MSWHEEL_ROLLMSG");
	// win98 wheel hack
//	SetWindowLong(hwnd,GWL_STYLE,G tWindowLong(hwnd,GWL_STYLE)|WS_VSCROLL);

}

void RPopup::Create(RWindow *w) //win32
{
	if(::IsWindow(hwnd) || !w) return; // already created ?

	InitCls((HBRUSH)(COLOR_3DFACE+1),"RwTkPopup");

	RECT r;
	r.left = win_x;/* + w->win_x;*/	r.right = r.left + width; //XXX
	r.top = win_y;/* + w->win_y;*/	r.bottom = r.top + height; //XXX
	AdjustWindowRect(&r,WS_POPUP | WS_SIZEBOX ,false);

	bDropWmSize = true; //hwnd is not set jet...
	// M$ have a off by one bug... XXX x+w=r? XXX actually <x+w-1=r>
	hwnd=CreateWindow("RwTkPopup","Test 123",WS_POPUP | WS_DLGFRAME,
		r.left,r.top,r.right-r.left,r.bottom-r.top,w->hwnd,NULL,GetModuleHandle(0),this);
	bDropWmSize = false;

	SetWindowLong(hwnd,GWL_USERDATA,(LONG)this);
	ShowWindow(hwnd,SW_SHOW);
}

void RToolTip::Create(RWindow *w) //win32
{
	if(::IsWindow(hwnd) || !w) return; // already created ?

	InitCls((HBRUSH)GetStockObject(WHITE_BRUSH),"RwTkToolTip");

	RECT r;
	r.left = win_x;/* + w->win_x;*/	r.right = r.left + width; //XXX
	r.top = win_y;/* + w->win_y;*/	r.bottom = r.top + height; //XXX
	AdjustWindowRect(&r,WS_POPUP,false);

	bDropWmSize = true; //hwnd is not set jet...
	// M$ have a off by one bug... XXX x+w=r? XXX actually <x+w-1=r>
	hwnd=CreateWindow("RwTkToolTip","Test 123",WS_POPUP,//WS_DLGFRAME,
		r.left,r.top,r.right-r.left,r.bottom-r.top,w->hwnd,NULL,GetModuleHandle(0),this);
	bDropWmSize = false;

	SetWindowLong(hwnd,GWL_USERDATA,(LONG)this);
	ShowWindow(hwnd,SW_SHOW);
}

#endif
//=============================================================================
#ifdef XLIB

void RWindow::InitWin()
{
	if(!WMProtocols) {
		RTK_MSG = XInternAtom(rdisplay,"RTK_MSG",0);
		WMProtocols = XInternAtom(rdisplay,"WM_PROTOCOLS",0);
		WMDelWin = XInternAtom(rdisplay,"WM_DELETE_WINDOW",0);
		WMTakeFocus = XInternAtom(rdisplay,"WM_TAKE_FOCUS",0);
#ifdef USECTX
		RWindowCTX = XUniqueContext();
#endif
	}

#ifdef USECTX
	XSaveContext(rdisplay,hwnd,RWindowCTX,(XPointer)this);
#else	
	TR_ADD(this);
#endif	

	// Enable "WM_CLOSE" // Enable FOCUS... sort of... ???!!!
	Atom aa[2] = { WMDelWin, WMTakeFocus };
	XSetWMProtocols(rdisplay,hwnd,aa,1);

	XWMHints wmh;
	wmh.flags = InputHint;
	wmh.input = true;
	XSetWMHints(rdisplay,hwnd,&wmh);

	// reduce flicker
	XSetWindowBackgroundPixmap(rdisplay, hwnd, None);
	
	XSelectInput(rdisplay, hwnd, FocusChangeMask | //LeaveWindowMask |// SubstructureNotifyMask |
			ExposureMask | KeyPressMask | KeyReleaseMask |
			ButtonPressMask | ButtonReleaseMask |
			PointerMotionMask | StructureNotifyMask);
}


/*
void ShowFonts(Display *rdisplay,char *c)
{
	int i,j;
	char **pp;
	pp = XListFonts(rdisplay,c,5000,&i);
	printf("Count = %i\n",i);

	for(j=0; j<i; j++)
		printf("+++++>%s<+++++\n",pp[j]);

	XFreeFontNames(pp);

}//*/

void RMainWindow::Create(RWindow *w) //win32
{
	if((rdisplay=XOpenDisplay(NULL)) == NULL) {
		fprintf(stderr,"basicwin: cannot connect to X server %s\n",XDisplayName(NULL));
		exit(-1);
	}

#if 0
	int i,j;
	int o,v,e;
	char **l = XListExtensions(rdisplay,&j);
	for(i = 0; i < j; i++) {
		XQueryExtension(rdisplay, l[i], &o, &v, &e);
		printf("XE - %i %s --- %i %i\n",o,l[i],v,e);
	}
	XFreeExtensionList(l);
#endif
	
	RMainWindow_P::Create(w);
	wFocus = hwnd;


	XSizeHints *xsh = XAllocSizeHints();
	xsh->flags = PMinSize;// | PResizeInc;
	xsh->width_inc = 10;
	xsh->height_inc = 10;
	xsh->min_width = 200;
	xsh->min_height= 200;
	XSetNormalHints(rdisplay,hwnd,xsh);
	XFree(xsh);


	//RTK_MSG = RegisterWindowMessage("RTK_MSG");

//	SetTimer(hwnd,1,100,NULL); //win32...

/*	ShowFonts(rdisplay,"*");
	printf("\n\n\n========\n\n\n");
	ShowFonts(rdisplay,"*fixed*");//*/
}

void RWindow::Create(RWindow *w)
{
	int screen = DefaultScreen(rdisplay);
	hwnd = XCreateSimpleWindow(rdisplay,RootWindow(rdisplay,screen), win_x, win_y, width, height,
				0, BlackPixel(rdisplay,screen), 0xE0E0A0);//TODO winbackcolor//WhitePixel(rdisplay,screen));

	InitWin();

	/*
	XSizeHints size_hints;
	size_hints.flags = PPosition | PSize | PMinSize;
	size_hints.x = x;
	size_hints.y = y;
	size_hints.width = width;
	size_hints.height = height;
	size_hints.min_width = 350;
	size_hints.min_height = 250;

	//XSetStandardProperties(rdisplay, win, window_name, icon_name, icon_pixmap,
	//		       argv, argc, &size_hints);*/

	XMapWindow(rdisplay, hwnd);
}

void RPopup::Create(RWindow *w)
{
	//printf("create popup\n");

	RWND ww = 0;//w->hwnd;
	int screen = DefaultScreen(rdisplay);
	if(!ww) ww = RootWindow(rdisplay,screen);
	hwnd = XCreateSimpleWindow(rdisplay, ww, win_x, win_y, width, height,
				1, BlackPixel(rdisplay,screen), 0xE0E0A0);//TODO winbackcolor// WhitePixel(rdisplay,screen));

	XSetWindowAttributes xswa;
	xswa.save_under = true;
	xswa.override_redirect = true;
	//xswa.background_pixmap = None;
	XChangeWindowAttributes(rdisplay,hwnd,CWOverrideRedirect|CWSaveUnder/*|CWBackPixmap*/,&xswa);
	
	InitWin();
}

void RToolTip::Create(RWindow *w) //win32
{
	RWND ww = 0;//w->hwnd;
	int screen = DefaultScreen(rdisplay);
	if(!ww) ww = RootWindow(rdisplay,screen);
	hwnd = XCreateSimpleWindow(rdisplay, ww, win_x, win_y, width, height,
				1, BlackPixel(rdisplay,screen), WhitePixel(rdisplay,screen));

	XSetWindowAttributes xswa;
	xswa.save_under = true;
	xswa.override_redirect = true;
	XChangeWindowAttributes(rdisplay,hwnd,CWOverrideRedirect|CWSaveUnder,&xswa);
	
	InitWin();
}

#endif //XLIB  // window creation
//=============================================================================
// RApp

extern int sislcnt;
void RApp::Sl_ShowIt() //DBG
{
#ifdef WIN32
	char num[10];
	itoa(sislcnt,num,10);
	MessageBox(wnd->hwnd,num,"sislcnt",0);
#endif
#ifdef XLIB
	printf("sislcnt %i\n",sislcnt);

	SiSlTN *tn;
	for(wnd->GetTypeName(&tn); tn; tn = tn->up) {
		printf("wnd -> %s %i %i\n",tn->name,tn->si,tn->sl);
	}
	for(Rtk_GetFPTR()->GetTypeName(&tn); tn; tn = tn->up) {
		printf("btn -> %s %i %i\n",tn->name,tn->si,tn->sl);
	}

#endif
}

void RApp::Sl_Quit()
{
	if(wnd) {
		wnd->Close();
	}
}

void RApp::Run()
{
	if(wnd) {
		wnd->ReCalc();
		wnd->Draw(true);
		wnd->Run();
	}
}

// RApp
//=============================================================================
// OLD Debug stuff

/*	case RK_DELETE:
		{
			if(rcKGrab) {
				rcKGrab->RemoveLater();
		//		rcKGrab->Remove();
		//		ReCalc();
		//		Draw(true);
		//		rcKGrab = NULL;
			}
			break;
		}*/


//=============================================================================
