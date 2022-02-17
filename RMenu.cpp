// vim:ts=4 sts=0 sw=4

#include "RMenu.h"

GETDRAWER(RMenuButton,RMENUBUTTON);
void RMenuButton::Init(RContainer *r)
{
	RMenuButton_P::Init(r);
	tree = NULL; pm = NULL;
};

RPopupMenu *RMenuBar::AddPopup(char *c) {
	return new RPopupMenu(new RMenuBarButton(this,c));
}

bool RMenuBar::IsDropped()
{
	for(RMenuBarButton *b = (RMenuBarButton*)child; b; b = (RMenuBarButton*)b->next) {
		if(b->pm && b->pm->hwnd)
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------++++
// BuildMenu

void adv(const char *&p)
{
	while(*p) p++;
	p++;
}

void atrim(const char *&p)
{
	p++;
	while(*p==' ' || *p=='\t') p++; //XXX changed from *p++;
}

void RMenuBar::BM(CConn *rc,RPopupMenu *pm, const char *&p)
{
	char sml=*p;
	RMenuButton *mb=0;
	if(*p == 'a')
		return;

	while(1) {
		if(!*p)
			return;

		if(*p < sml || *p > 'z')
			return;

		if(*p == sml) {
			atrim(p);
			mb = pm->AddItem((char*)p);
			adv(p);
			atrim(p);
			if(*p)
				mb->Connect("Si_Clicked(int)",rc,p);
		
			adv(p);
		}

		if(*p > sml) {
			RPopupMenu *w;
			w = new RPopupMenu(mb->wnd);//aaa,120,10,100,3);
			w->bSelfDelete = false;
			mb->pm = w;
			BM(rc,w,p);
		}
	}
}

void RMenuBar::BuildMenu(CConn *rc,const char *p)
{
	RMenuBarButton *b;

	while(1)
	switch(*p) {
	case 0:
		return;
	case 'a':
		b = new RMenuBarButton(this);//aaa,0,0,100,24);
		atrim(p);
		b->SetText(p);
		b->x_weight = 1;
		adv(p);
		adv(p);
		RPopupMenu *w;
		w = new RPopupMenu(b->wnd);//aaa,120,10,100,3);
		w->bSelfDelete = false;
		b->pm = w;
		BM(rc,w,p);
		break;
	default:
		return;
	}
}

// BuildMenu
//-----------------------------------------------------------------------------++++
// RMenuButton

void RMenuButton::Popup()
{
	if(pm && pm->wnd->hwnd)
		return;

	if(pm) {
		if(parent->align & AL_V) {
			pm->SetPosW(right + 1,top);
		}else{
			pm->SetPosW(left,bottom + 1);
		}

		/*// XXX scap this...
		// set first as hit
		RControl *b = pm->child;
		while(b) {
		//	b->b Hit = !b->prev;
			b = b->next;
		}*/

		//new
		if(pm->child)
			pm->child->Sl_Focus();

#ifdef XLIB
//		pm->width = 100;
//		pm->height = 200;
#endif
		pm->Popup(wnd);
		
/*		pm->Create(wnd);
		pm->SetAlignment(AL_V | AL_FITV | AL_FITH);
		pm->ReCalc();
		pm->ShowW();
		pm->SetFocusW();*/

#ifdef XLIB // and win32
//		if(pm->wnd) {
//			pm->wnd->SetFocusW();
//		}
#endif
	}else{
		// ---- send clicked command ??? what about hover ????
		//wnd->SetFocusW();
	}
}

void RMenuButton::KUp(u32 d)
{
	switch(d) {
	case RK_UP:   if(prev) prev->Sl_Focus(); break;
	case RK_DOWN: if(next) next->Sl_Focus(); break;
	case RK_RIGHT: Popup(); break;
	case RK_LEFT: if(wnd->parentw) wnd->parentw->SetFocusW(); break; //wnd->Close(); break;
	case RK_SPACE:
		Sl_Clicked();
		if(pm) {
			Popup();
		} else {
			// wnd->RTKMsg(RTKMSG_DESTROY_POPUP);
			wnd->UnfocusPopups();
		}
		break;
	}
}

//void RMenuButton::MEnter()
void RMenuButton::MHover()
{
	Popup();
}

void RMenuButton::MButton(u32 b)
{
	if(b & M_CLICK)
		Sl_Clicked(b);

	if((b & 0x1F) == 1) {
		if(b & M_DOWN) {
			Popup();
		}else{
			if(!pm)
				// cannot delete ourself (segfault) so queue it
			//	wnd->RTKMsg(RTKMSG_DESTROY_POPUP); // UnfocusPopups(); --> KillPopup();	
				wnd->UnfocusPopups();
		}
	}
}

void RMenuButton::MExit(bool b)
{
}

void RMenuBarButton::KUp(u32 d)
{
	switch(d) {
	case RK_LEFT:  if(prev) prev->Sl_Focus(); break;
	case RK_RIGHT: if(next) next->Sl_Focus(); break;
	case RK_SPACE:
	case RK_DOWN:  Popup(); break;
	}
}

void RMenuBarButton::MHover() {};

void RMenuBarButton::MEnter(bool b)
{
	Sl_Focus(); //.....
	if(((RMenuBar*)parent)->IsDropped())
		Popup();
}

void RMenuBarButton::MButton(u32 b)
{
	if(b & M_CLICK)
		Sl_Clicked(b);

	if((b & M_DOWN) && (b & 0x1F) == 1) {
		if(pm && !pm->hwnd)
			Popup();
	}
}

// RMenuButton


