// vim:ts=4 sts=0 sw=4
// RTK base classes

#include "Rtk.h"

//--------------------------------------------------------------------------++++
// RControl
// Init stuff

RDrawer *GetDrawer(u32 d); // from drawman.cpp

void RControl::GetDrawer(u32 d)
{
	pDrawer = ::GetDrawer(d ? d : RCONTROL);
}

void RControl::Init(RContainer *r)
{
	RControl_P::Init();

	GetDrawer(0);
	next = prev = parent = NULL;

	//set the default control FrameType
	align = 0;
	frame = F_NONE;
	flags = 0;

	//SetLTRB(0,0,0,0); xxx
	left = top = right = bottom = width = height = 0;

	x_weight = y_weight = 0;

	text = (char*)malloc(100);
	memset(text,0,100);
	tooltip = NULL;
	bPushed = bChecked = bFocus = false;

	bForceMin = 1;
	bDisabled = bHidden = 0;
	if(r) {
		r->AddControl(this);
		if(r->bDisabled)
			bDisabled |= 2;
		if(r->bHidden)
			bHidden |= 2;
	}
}

void RControl::Dinit()
{
	RControl_P::Dinit();
//%	if(next) delete next;
	if(text) free(text);
};

void RContainer::Init(RContainer *r)
{
	child = last = NULL;
	x_min = y_min = 20;
	x_mgn = y_mgn = spacing = 4;
	
	RContainer_P::Init(r);

//	bForceMin = 0;
}

void RContainer::Dinit()
{
	RContainer_P::Dinit();
//%	if(child)
//%		delete child;
//
	RControl *t,*c = child;
	while(c) {
		t = c;
		c = c->next;
		delete t;
	}
}

//---------------------------

GETDRAWER(RTBox,RTBOX);
void RTBox::Init(RContainer *r)
{
	if(!ww) ww = 1;
	if(!hh) hh = 1;
	Alloc();
	
	RTBox_P::Init(r);
}

void RTBox::Dinit()
{
	RTBox_P::Dinit();
	if(mem)
		free(mem);
	mem = 0;
}

void RTBox::Alloc()
{
	int i = ((ww + hh) * 3 + 2) * sizeof(u16);
	i += ww * hh * sizeof(RControl*);
	mem = (char*)malloc(i);
	if(!mem) {
#ifdef XLIB
		printf("RTBox::Alloc FAIL!!!!\n");
#endif
		return;
	}
	memset(mem,0,i);
	pc = (RControl**)mem;
	xmin = (u16*)&pc[ww * hh];
	ymin = xmin + ww;
	xw = ymin + hh;
	yw = xw + ww;
	xpos = yw + hh;
	ypos = xpos + (ww + 1);
}

//---------------------------

GETDRAWER(RTabControl,RTABCONTROL);
void RTabControl::Init(RContainer *r)
{
	RTabControl_P::Init(r);
	rrb = NULL;
	rrb = new RRadioBox(this,8,1); //XXX TODO
	//rrb->GetDrawer(RCONTROL);
	rrb->SetText("Radio");
	rrb->y_mgn = 0;
	rrb->x_mgn = 4;
	rrb->spacing = 0;
	rrb->x_weight = 1;
	rrb->bForceMin = 1;
};

//---------------------------

GETDRAWER(RButton,RBUTTON);
GETDRAWER(RLabel,RLABEL);
GETDRAWER(RCheckBox,RCHECKBOX);
GETDRAWER(RRadioButton,RRADIOBUTTON);
GETDRAWER(RToggleButton,RTOGGLEBUTTON);
GETDRAWER(RRange,RRANGE);
GETDRAWER(RScrollBar,RSCROLLBAR);
GETDRAWER(RProgressBar,RPROGRESSBAR);
GETDRAWER(RStatusBar,RSTATUSBAR);

// Init stuff
//---------------------------
// container Stuff

bool RControl::IsChild(RControl *c)
{
	for(RControl *r = GetChild(); r; r = r->next) {
		if(r == c)
			return true;
	}
	return false;
}

bool RControl::IsChildR(RControl *c)
{
	if(this == c)
		return true;
	for(RControl *r = GetChild(); r; r = r->next) {
		if(r->IsChildR(c))
			return true;
	}
	return false;
}

void RContainer::AddControl(RControl *c)
{
	if(!wnd) { //XXX
#ifndef XLIB
//		MessageBox(0,"wnd not set yet.","Error",0);
#else
		printf("Error wnd not set yet.\n");
#endif
		return;
	}

	c->wnd = wnd;
	c->next = NULL;
	c->parent = this;
	if(last) {
		last->next = c;
		c->prev = last;
		last = c;
	}else{
		child = last = c;
	}
}

void RContainer::RemoveControl(RControl *b, bool del)
{
	if(!IsChild(b))
		return;

	if(!b->prev) {
		child = b->next;
		if(!child)
			last = NULL;
		else
			child->prev = NULL;
	}else{
		b->prev->next = b->next;
		if(!b->next)
			last = b->prev;
		else
			b->next->prev = b->prev;
	}
	b->prev = b->next = b->parent = NULL;
	if(del) {
		delete b;
	}
}

void RContainer::Clear()
{
	child = NULL;
	last = NULL;
}

void RControl::ClipRect(Rect &r)
{
	if(wnd == this)
		return;
	
	if(left > r.left) r.left = left;
	if(top > r.top) r.top = top;
	if(right < r.right) r.right = right;
	if(bottom < r.bottom) r.bottom = bottom;

	if(parent)
		parent->ClipRect(r);
}

// container Stuff
//---------------------------
// control

void RControl::Draw(bool bAll)
{
	pDrawer->Draw(this,bAll);
}

void RControl::SetText(const char *c) {
	if(strlen(c) > 100) { //XXX
		if(text) free(text);
		text = (char*)malloc(strlen(c)+10);
	}
	strcpy(text,c);
	Si_SetText(text);
}

void RControl::Remove()
{
	if(parent) parent->RemoveControl(this);
}

void RControl::RemoveLater()
{
	wnd->RTKMsg(RTKMSG_REMOVE_LATER,this);
}

void RControl::ShowToolTip()
{
	if(tooltip) {
//		new RToolTip(wnd, cx+left+10, cy+top+16, tooltip);
	}
}

// control
//---------------------------
// sizing + positioning

void RControl::SetLTRB(int l,int t,int r,int b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
	vw = width = r - l + 1;
	vh = height = b - t + 1;

	if(bForceMin) {
		int w,h;
		GetMinSize(w,h);
		if(width < w) { width = w; right = left + w - 1; }
		if(height< h) { height= h; bottom = top + h - 1; }
	}
}

void RControl::SetPos(int x, int y)
{
	int dx = x - left, dy = y - top;
	SetPosR(dx,dy);
/*	SetLTRB(x, y, x+width-1, y+height-1);

	for(RControl *c = GetChild(); c; c = c->next) { //t
		c->SetPosR(dx,dy);
	}*/
}

void RControl::SetPosR(int x, int y)
{
	SetLTRB(left+x, top+y, right+x, bottom+y);

	for(RControl *c = GetChild(); c; c = c->next) { //t
		c->SetPosR(x,y);
	}
}

void RControl::SetSize(int x, int y)
{
	SetLTRB(left, top, left+x-1, top+y-1);
}

void RControl::SetSizeR(int x, int y)
{
	SetLTRB(left, top, right+x, bottom+y);
}

void RControl::SetPosSize(int x, int y, int w, int h)
{
	int dx = x - left, dy = y - top;
	SetLTRB(x, y, x+w-1, y+h-1);

	for(RControl *c = GetChild(); c; c = c->next) {
		c->SetPosR(dx,dy);
	}
}

//-------

void RControl::GetSizeF(int &w, int &h)
{
	int wa,ha,wb,hb;
	GetFrameSizes(wa, ha, wb, hb);
	w = x_min + wa + wb;
	h = y_min + ha + hb;
}

void RControl::GetSizeFI(int &w, int &h)
{
	int wa,ha,wb,hb,wm,hm;
	GetFrameSizes(wa, ha, wb, hb);
	GetMinSize(wm,hm);
	w = wm + wa + wb;
	h = hm + ha + hb;
}

void RControl::SetBoxF(int x, int y, int w, int h)
{
	int wa,ha,wb,hb;
	int xx, yy, ww, hh;

	ww = x_min; hh = y_min;
	GetFrameSizes(wa, ha, wb, hb);
	x += wa;
	y += ha;
	w -= (wa + wb);
	h -= (ha + hb);
	
	// center left right stretch
	switch(align & 3) {
	case 0: xx = x + ((w - ww) / 2); break;
	case 1: xx = x; break;
	case 2: xx = x + (w - ww); break;
	case 3:	xx = x; ww = w; break;
	}
	switch((align >> 2) & 3) {
	case 0: yy = y + ((h - hh) / 2); break;
	case 1: yy = y; break;
	case 2: yy = y + (h - hh); break;
	case 3:	yy = y; hh = h; break;
	}

	SetPosSize(xx,yy,ww,hh);
}

void RControl::GetFrameSizes(int &w, int &h, int &ww, int &hh)
{
	// should query the parent if using parent define sizes....
	// using generic frames for now
	// if(parent) parent->GetFrameSizesP(frame,w,h,ww,hh);
	if(frame) { //XXX
		w = 2; h = 2; ww = 2; hh = 2;
	}else{
		w = 0; h = 0; ww = 0; hh = 0;
	}
}

// sizing + positioning
//---------------------------
// Mouse Handling

void RControl::MRawEnter(bool b)
{
/*	if(!b) {
		//printf("MRawEnter F\n");
		return;
	}//else printf("MRawEnter T\n");*/

	if(b) {
		bPushed = HasGrab(); //bMouseDown; // && still down); XXX
		Draw();
	}
	MEnter(b);
	Si_MEnter(b);
}

void RControl::MRawExit(bool b)
{
/*	if(!b) {
		//printf("MRawExit F\n");
		return;
	}//else printf("MRawExit T\n");*/

	if(b) {
		bPushed = 0;
		Draw();
	}
	MExit(b);
	Si_MExit(b);
}

void RControl::MRawButton(u32 b)
{
	if(bDisabled || bHidden)
		return;

	if(b & M_DOWN) {
		if(IsHit()) {
			bPushed = 1;
			//bMouseDown = 1;
			MButton(b);

			if(bFocus) //XXX
				Draw();
			else
				Sl_Focus();

			return;
		}
	}else{
		bPushed = IsHit() && HasGrab();
		//bMouseDown = 0;
		MButton(b);

	/*	if(b & M_CLICK) { //TEMP move to RButton::MButton...
			Sl_Clicked(b);
		}*/
	}
	Draw();
}

/*void RControl::SetCxy(int x, int y)
{
	cx = x - left; //XXX remove cx and cy from class ???
	cy = y - top;
}*/

bool RControl::HitTest(int x, int y)
{
	if((u32)(x - left) < (u32)width && (u32)(y - top) < (u32)height) {
		if(parent) {
			return parent->HitTest(x,y);
		}else{
			return true;
		}
	}
	return false;
}

RControl *RControl::MRawHitTest(int x, int y, bool up)
{
	if(bDisabled || bHidden) {
		if(up) {
			goto tryparent;
		}else{
			return 0;
		}
	}

	if(!HitTest(x,y)) {
		if(up && parent && parent->wnd == wnd) {
tryparent:		
			wnd->SetMHit(parent); //???
			MRawExit(true);
			parent->MRawEnter(false);
			return parent->MRawHitTest(x,y,true);
		}
		return 0;
	}
	
	//MCoordUpdate(); //TMP

	if(!up) {
		wnd->SetMHit(this); //???
		if(parent)
			parent->MRawExit(false);
		MRawEnter(true);
	}

	RControl *rc;
	for(RControl *c = GetLast(); c; c = c->prev) {
			if((rc = c->MRawHitTest(x,y,false)))
				return rc;
	}

	return this;
}

void RControl::MRawMove(int x, int y)
{
	if(!bDisabled && !bHidden)
		MMove();
}

void RControl::MRawScroll(int b,int d)
{
	if(!bDisabled && !bHidden)
		MScroll(b,d);
}

bool RControl::IsHit() { return wnd->rcMHit == this; };
u32 RControl::HasGrab() { return wnd->rcMGrab == this ? wnd->MBut : 0; };
void RControl::MMove() { parent->MMove(); };
void RControl::MButton(u32 b) { parent->MButton(b); };
void RControl::MScroll(int b,int d) { parent->MScroll(b,d); };
//void RControl::MScroll(int b,int d) { SetPosR(0,20*d); };

// Mouse Handling
//---------------------------
// signals & slots

//------------

bool RControl::IsShown()
{
	return !bHidden;
/*	for(RControl *p = this; p; p = p->parent) {
		if(!p->b Show)
			return false;
	}
	return true;*/
}

void RControl::Sl_Show(bool b)
{
	if(b)
		Sl_Show();
	else
		Sl_Hide();
}


void RControl::ShowC()
{
	for(RControl *c = GetChild(); c; c = c->next) {
		c->bHidden &= 1;
		if(!(c->bHidden & 1))
			c->ShowC();
	}
}

void RControl::Sl_Show()
{
	if(!(bHidden & 1))
		return;
	bHidden &= 2;
	if(bHidden & 2) //XXX
		return;
	ShowC();
	Draw(true);
};

void RControl::HideC()
{
	for(RControl *c = GetChild(); c; c = c->next) {
		c->bHidden |= 2;
		if(!(c->bHidden & 1))
			c->HideC();
	}
}

void RControl::Sl_Hide()
{
	if(bHidden & 1)
		return;
	bHidden |= 1;
	if(bHidden & 2)
		return;
	HideC();
	wnd->KMFree(this);
	parent->Draw(true);
};

//------------

bool RControl::IsEnabled() 
{
	return !bDisabled;
/*	for(RControl *p = this; p; p = p->parent) {
		if(p->bDisabled)
			return false;
	}
	return true;*/
}

void RControl::Sl_Enable(bool b)
{
	if(b)
		Sl_Enable();
	else
		Sl_Disable();
}

void RControl::EnableC()
{
	for(RControl *c = GetChild(); c; c = c->next) {
		c->bDisabled &= 1;
		if(!(c->bDisabled & 1))
			c->EnableC();
	}
}

void RControl::Sl_Enable()
{
	if(!(bDisabled & 1))
		return;
	bDisabled &= 2;
	if(bDisabled & 2) //XXX
		return;
	EnableC();
	Draw(true);
};

void RControl::DisableC()
{
	for(RControl *c = GetChild(); c; c = c->next) {
		c->bDisabled |= 2;
		if(!(c->bDisabled & 1))
			c->DisableC();
	}
}

void RControl::Sl_Disable()
{
	if(bDisabled & 1)
		return;
	bDisabled |= 1;
	if(bDisabled & 2)
		return;
	DisableC();
	wnd->KMFree(this);
	Draw(true);
};

//------------

bool RControl::IsFocused()
{
	return bFocus;
}

void RControl::Sl_Focus(bool b)
{
	if(b)
		Sl_Focus();
	else
		Sl_UnFocus();
}

void RControl::Sl_Focus()
{
	if(bFocus || !IsShown())
		return;
	bFocus = true;
	wnd->KGrab(this);
	//Focused(); // zap ???
	Si_Focus(true);
	Draw();
}

void RControl::Sl_UnFocus()
{
	if(!bFocus)
		return;
	bFocus = false;
	wnd->KUnGrab(this);
	//UnFocused(); // zap ???
	Si_Focus(false);
	Draw();
}

// RControl
//--------------------------------------------------------------------------++++
// RContainer
// Moving functionality
#if 0
void R ontainer::MButton(u32 b)
{
//	if(b>0) {
		ox = cx;
		oy = cy;
//	}

	if(cx-3 > 0 && cx+3 < width && cy-3 > 0 && cy+3 < height)
		bMvSz = 1;
	else
		bMvSz = 0;
}

void R ontainer::MMove()
{
	if(!b MouseDown)
		return;

	if(bMvSz) {
		SetPosR(cx-ox,cy-oy);
		/*ox = cx;	oy = cy; ....NOOOOO....*/
	}else{
		SetSizeR(cx-ox,cy-oy);
		ox = cx;
		oy = cy;
	}


	if(parent) parent->Draw(true); else Draw(true);
}

/*
void R ontainer::KDown(u32 d)
{
	if(bFocus) {
		switch(d) {
		case RK_DOWN: SetPosR(0,1); break;
		case RK_UP: SetPosR(0,-1); break;
		case RK_LEFT: SetPosR(-1,0); break;
		case RK_RIGHT: SetPosR(1,0); break;

		case 'H': b Show = false; break;
		case 'S': b Show = true; break;
		default: return;
		}
		if(parent) parent->Draw(true); else Draw(true);
	}
}*/

#endif

// RContainer
//--------------------------------------------------------------------------++++
// Containers

void RHVBox::ReCalcI(bool b)
{
	RControl *c;
	float r=0,a=0,i;
	int xw=0,yw=0,w,h;

	for(c = child; c; c = c->next) {
		xw += c->x_weight;
		yw += c->y_weight;
	}
	if(align & AL_FITH && !x_weight) SetWidth(x_min);
	if(align & AL_FITV && !y_weight) SetHeight(y_min);

	if(align & AL_V) {
		a = top + y_mgn;
		if(height > y_min && yw)
			r = float(height - y_min) / (float)yw;
		else
			r = 0;
		for(c = child; c; c = c->next) {
			c->GetSizeF(w,h); i = h; //XXX
			if(c->y_weight) {
				i += c->y_weight * r;
			}
			c->SetBoxF(left + x_mgn, (int)a, width - (x_mgn * 2), (int)i);
			c->ReCalcI();
			a += i + spacing;
		}
	}else{
		a = left + x_mgn;
		if(width > x_min && xw)
			r = float(width - x_min) / (float)xw;
		else
			r = 0;
		for(c = child; c; c = c->next) {
			c->GetSizeF(w,h); i = w; //XXX
			if(c->x_weight) {
				i += c->x_weight * r;
			}
			c->SetBoxF((int)a, top + y_mgn, (int)i, height - (y_mgn * 2));
			c->ReCalcI();
			a += i + spacing;
		}
	}
}

void RHVBox::GetMinSize(int &x, int &y)
{
	RControl *c;
	int w,h;
	x_min = y_min = 0;

	if(align & AL_V) {
		if((c = child)) while(1) {
			c->GetSizeFI(w,h); //GetMinSize
			x_min = x_min > w ? x_min : w;
			y_min += h;
			if((c = c->next))
				y_min += spacing;
			else
				break;
		}
	}else{
		if((c = child)) while(1) {
			c->GetSizeFI(w,h); //GetMinSize
			y_min = y_min > h ? y_min : h;
			x_min += w;
			if((c = c->next))
				x_min += spacing;
			else
				break;
		}
	}
	x_min += x_mgn * 2;
	y_min += y_mgn * 2;
	x = x_min;
	y = y_min;
}

//-------------------------------------

RControl *RTBox::GetAt(int x, int y)
{
	if(x >= ww || y >= hh) {
#ifdef XLIB
		printf("RTBox::GetAt FAIL... %i %i\n",x,y);
#endif
	}
	return pc[(y*ww)+x];
}

void RTBox::SetAt(RControl *r, int x, int y)
{
	if(!r || x >= ww || y >= hh) {
#ifdef XLIB
		printf("RTBox::SetAt FAIL... %i %i\n",x,y);
#endif
	}
	pc[(y*ww)+x] = r;
}

void RTBox::SetPos(int x, int y, int w, int h)
{
	if(!last || x >= ww || y >= hh) {
#ifdef XLIB
		printf("RTBox::SetPos FAIL... %i %i\n",x,y);
#endif
		return;
	}

	SetAt(last,x,y);
	if(!w) w = 1;
	if(!h) h = 1;
	if(x+w > ww)
		w = ww - x;
	if(y+h > hh)
		h = hh - y;
	last->x_weight = w;
	last->y_weight = h;
	//XXX TODO set ptrs to 1 here...
}

void RTBox::SetNextPos()
{
	int i,j;
	last->x_weight = 1;
	last->y_weight = 1;
	if(align & AL_V) {
		for(i = 0; i < ww; i++)
		for(j = 0; j < hh; j++) {
			if(!GetAt(i,j)) {
				SetAt(last,i,j);
				return;
			}
		}
	}else{
		for(j = 0; j < hh; j++)
		for(i = 0; i < ww; i++) {
			if(!GetAt(i,j)) {
				SetAt(last,i,j);
				return;
			}
		}
	}
}

void RTBox::SetWeight(int x, int xv, int y, int yv)
{
	if(x >= 0 && x < ww)
		xw[x] = xv;
	if(y >= 0 && y < hh)
		yw[y] = yv;
}

void RTBox::ReCalcI(bool b)
{
	float a,c,r;
	int xwe,ywe;
	int i,j;
	
	// x weighting
	for(i = 0, xwe = 0; i < ww; i++) xwe += xw[i];
	if(width > x_min && xwe)
		r = float(width - x_min) / (float)xwe;
	else
		r = 0;
	a = xpos[0] = x_mgn;
	for(i = 0; i < ww; i++) {
		c = xmin[i];
		if(xw[i]) {
			c += xw[i] * r;
		}
		a += c + spacing;
		xpos[i+1] = (u16)a;
	}
	// y weighting
	for(i = 0, ywe = 0; i < hh; i++) ywe += yw[i];
	if(height > y_min && ywe)
		r = float(height - y_min) / (float)ywe;
	else
		r = 0;
	a = ypos[0] = y_mgn;
	for(i = 0; i < hh; i++) {
		c = ymin[i];
		if(yw[i]) {
			c += yw[i] * r;
		}
		a += c + spacing;
		ypos[i+1] = (u16)a;
	}

	// set positions
	for(i = 0; i < ww; i++)
	for(j = 0; j < hh; j++) {
		RControl *r = GetAt(i,j);
		if(((uptr)r) < 2) //XXX
			continue;
		if((r->bHidden & 1) && r->bCollapse) {
			r->SetBoxF(0,0,0,0);
			continue;
		}
		if(i + r->x_weight > ww)
			r->x_weight = ww - i;
		if(j + r->y_weight > hh)
			r->y_weight = hh - j;
		r->SetBoxF(xpos[i] + left, ypos[j] + top, //XXX weight bounds...
			xpos[i+r->x_weight] - xpos[i] - spacing, ypos[j+r->y_weight] - ypos[j] - spacing);
		r->ReCalcI();
	}
}

void RTBox::GetMinSize(int &xx, int &yy)
{
	int i,j,x,y;
	
	for(i = 0; i < ww; i++) xmin[i] = 0;
	for(i = 0; i < hh; i++) ymin[i] = 0;
	
	for(i = 0; i < ww; i++)
	for(j = 0; j < hh; j++) {
		RControl *r = GetAt(i,j);
		if(((uptr)r) < 2 || ((r->bHidden & 1) && r->bCollapse)) //XXX
			continue;
		r->GetMinSize(x,y);
		if(xmin[i] < x)
			xmin[i] = x;
		if(ymin[j] < y)
			ymin[j] = y;
	}
	
	for(i = 0, x_min = x_mgn * 2 - spacing; i < ww; i++) {
		x_min += xmin[i] + spacing;
	}
	for(i = 0, y_min = y_mgn * 2 - spacing; i < hh; i++) {
		y_min += ymin[i] + spacing;
	}
	xx = x_min;
	yy = y_min;
}

//-------------------------------------

/*void RSBox::Init(RContainer *r)
{
	PSBox_P::Init(r);
}*/

void RSBox::Setup(int wx, int wy)
{
	RControl *sb,*sv = last;
	if(!sv)
		return;

	SetPos(0,0);
	sv->SetAlignment(AL_H | AL_STRETCHH | AL_STRETCHV);
	SetAlignment(AL_H | AL_STRETCHH | AL_STRETCHV);
	SetWeight(0,1,0,1);
	SetText("RSBox");
	sv->x_weight = x_weight = wx;
	sv->y_weight = y_weight = wy;
	spacing = 0;
	x_mgn = 0;
	y_mgn = 0;

	//X
	sb = new RScrollBar(this);
	sb->SetAlignment(AL_STRETCHH | AL_BOTTOM);
	sb->SetText("SBX");
	SetPos(0,1);
	sv->Connect("Si_ScrolledX(int)",sb,"Sl_Scroll(int)");
	sb->Connect("Si_Scrolled(int)" ,sv,"Sl_ScrollX(int)");
	sv->Connect("Si_AdjustX(int,int)",sb,"Sl_Adjust(int,int)");

//xx causes SEGFAULT when quitting app....
//XXX EGFAULT signal in reverse....	

	//Y
	sb = new RScrollBar(this);
	sb->SetAlignment(AL_V | AL_STRETCHV | AL_LEFT);
	sb->SetText("SBY");
	SetPos(1,0);
	sv->Connect("Si_ScrolledY(int)",sb,"Sl_Scroll(int)");
	sb->Connect("Si_Scrolled(int)" ,sv,"Sl_ScrollY(int)");
	sv->Connect("Si_AdjustY(int,int)",sb,"Sl_Adjust(int,int)");
}

void RSBox::ReCalcI(bool b)
{
//	RScrollView *r = (RScrollView*)child;
	RControl *r = child;
	RScrollBar *sbx,*sby;
	sbx = (RScrollBar*)r->next,
	sby = (RScrollBar*)sbx->next;

//	int mx,my;
	int xx,xy,yx,yy;
	sbx->GetMinSize(xx,xy);
	sby->GetMinSize(yx,yy);
/*	r->RScrollView_P::GetMinSize(mx,my);

	sbx->bHidden &= ~1;
	if(mx <= width) {
		sbx->bHidden |= 1;
	}

	sby->bHidden &= ~1;
	if(sbx->bHidden) {
		if(my <= height) {
			sby->bHidden |= 1;
			yx = yy = 0;
		}
	}else{
		if(my <= height-xy) {
			sby->bHidden |= 1;
			yx = yy = 0;
		}
	}

	if(!sby->bHidden && mx > width-yx) {
		sbx->bHidden &= ~1;
		if(my <= height-xy) {
			sby->bHidden |= 1;
			yx = yy = 0;
		}
	}else{
		xx = xy = 0;
	}*/

	r->SetBoxF(left,top,width-yx,height-xy);
	sbx->SetBoxF(left,bottom-xy+1,width-yx,xy);
	sby->SetBoxF(right-yx+1,top,yx,height-xy);

	r->ReCalcI(true);
	sbx->ReCalcI(true);
	sby->ReCalcI(true);


//	int x,y;
//	r->GetMinSize(x,y);

	static int aa = 0;
	
	if(!aa) {
//		aa = 1;
		sbx->Sl_Adjust(width,r->vw);
		sby->Sl_Adjust(height,r->vh);
		aa = 0;
	}

//	r->Si_AdjustX(x, width);
//	r->Si_AdjustY(y, height);

}


/*
void RContainer::SetLTRB(int l,int t,int r,int b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
	width = r - l + 1;
	height = b - t + 1;
}*/


//--------------------------------------------------------------------------++++

void RScrollView::Sl_ScrollX(int x)
{
	if(oox == x)
		return;
	Si_ScrolledX(x);
	Scroll(oox - x,0);
	oox = x;
	Draw(true);
}

void RScrollView::Sl_ScrollY(int y)
{
	if(ooy == y)
		return;
	Si_ScrolledY(y);
	Scroll(0,ooy - y);
	ooy = y;
	Draw(true);
}

void RScrollView::ReCalcI(bool b)
{
//	RScrollView_P::ReCalcI(b);
//	Si_AdjustX(width * 2 /*temp*/, width - x_mgn * 2);
//	Si_AdjustY(height* 2 /*temp*/, height- y_mgn * 2);
	
	RScrollView_P::ReCalcI(b);
//	int x,y;
//	GetMinSize(x,y);
//	Si_AdjustX(x, width);
//	Si_AdjustY(y, height);

}

void RScrollView::Scroll(int x, int y)
{
	for(RControl *c = child; c; c = c->next) {
		c->SetPosR(x,y);
	}
}

void RScrollView::MScroll(int b, int d)
{
	if(wnd->bShift) {
		Sl_ScrollX(oox + d*20);
	}else{
		Sl_ScrollY(ooy + d*20);
	}
}

void RScrollView::KDown(u32 d)
{
	/*for(RControl *c = child; c; c = c->next) {
		c->KDown(d);
	}*/

	if(bFocus) {
		switch(d) {
		case RK_UP: Scroll(0,-10); break;
		case RK_DOWN: Scroll(0,10); break;
		case RK_LEFT: Scroll(-10,0); break;
		case RK_RIGHT: Scroll(10,0); break;
		/*case 'H': b Show = false; break;
		case 'S': b Show = true; break;//*/
		default: return;
		}
		Draw(true);
	}
}

//-------------------------------------

void RTabControl::AddControl(RControl *b)
{
	if(rrb) {
		if(rrb->child) {
			b->bHidden = 1; //xxx
		}
		//good or bad policy ???
		b->SetAlignment(AL_STRETCHH | AL_STRETCHV);
		//or should this be left out ?

		RRadioButton *rb = new RRadioButton(rrb,(RRadioButton*)rrb->child);
		rrb->SetNextPos();
		rrb->ReCalc();

		rb->SetText(b->text);
		rb->GetDrawer(RTOGGLEBUTTON); //NB
		rb->Connect("Si_Checkd(bool)",b,"Sl_Show(bool)",NULL);
		//rb->Connect("Si_Checked()",b,"Sl_Show()",NULL);
		//rb->Connect("Si_UnChecked()",b,"Sl_Hide()",NULL);

		b->Connect("Si_SetText(char*)",rb,"Sl_SetText(char*)",NULL);

		//XXX	b->SetFrameType(F_DOWN); ??? F_TAB ???
	}
	RTabControl_P::AddControl(b);
	
}

void RTabControl::ReCalcI(bool b)
{
	RControl *c = child;
	if(!c)
		return;
	c->SetPos(left, top);
	c->SetWidth(width);
	c->ReCalcI();

	int tt = top + c->GetHeight();
	int hh = c->GetHeight();
	for(c = c->next; c; c = c->next) {
		c->SetBoxF(left, tt, width, height - hh);
		c->ReCalcI();
	}
	return;
}

void RTabControl::GetMinSize(int &xx, int &yy)
{
	int x,y;
	x_min = y_min = 0;
	for(RControl *c = child; c; c = c->next) {
		c->GetMinSize(x,y);
		if(x_min < x)
			x_min = x;
		if(y_min < y)
			y_min = y;
	}
	y_min += child ? child->y_min : 0; // tab bar
	xx = x_min;
	yy = y_min;
}

//XXX hack function until I find a better way to do this... if ever :-P
bool RTabControl::ConnectTab(cchar *tab, cchar *si, CConn *cnn, cchar *sisl, UPTR u)
{
	if(!rrb || !rrb->child) {
		return false;
	}

	for(RControl *c = rrb->child; c; c = c->next) {
		if(!strcmp(tab,c->text)) {
			return c->Connect(si,cnn,sisl,u);
		}
	}

	return false;
}

// Containers
//--------------------------------------------------------------------------++++
// Controls
// RButton

void RButton::Sl_Clicked(int b)
{
	Si_Clicked(b);
}

void RButton::KUp(u32 d)
{
	switch(d) {
	case RK_LEFT:
	case RK_UP:   if(prev) prev->Sl_Focus(); break;
	case RK_RIGHT:
	case RK_DOWN: if(next) next->Sl_Focus(); break;
	case RK_SPACE: bPushed = false; Sl_Clicked(1); Draw(); break;
	}
}

void RButton::KDown(u32 d)
{
	if(d == RK_SPACE && !bPushed) {
		bPushed = true;
		Draw();
	}
}

void RButton::MButton(u32 b)
{
	if(b & M_CLICK)
		Sl_Clicked(b);
}

// RButton
//--------------------------------------------------------------------------++++
// CheckBox

void RCheckBox::MButton(u32 b)
{
	if(b & M_CLICK) {
	if(bChecked) {
		Sl_UnCheck();
	}else{
		Sl_Check();
	}}
}

// CheckBox
//--------------------------------------------------------------------------++++
// RRadioButton

void RRadioButton::InitRB(RRadioButton *rb)
{
	if(!rb) {
		rfirst = this;
		rnext = rprev = 0;
		bChecked = true;
	}else{
		RRadioButton *l = rfirst = rb->rfirst;
		while(l->rnext) l = l->rnext;
		l->rnext = this;
		rprev = l;
		rnext = 0;
	}
}

void RRadioButton::Remove()
{
	RRadioButton_P::Remove();
	if(rprev) {
		rprev->rnext = rnext;
		if(rnext)
			rnext->rprev = rprev;
	}else{
		for(RRadioButton *r = rnext; r; r = r->rnext) r->rfirst = rnext;
		if(rnext)
			rnext->rprev = 0;
	}
}

void RRadioButton::MScroll(int b,int d)
{
	for(RRadioButton *c = rfirst; c; c = c->rnext) {
		if(c->bChecked) {
			if(d > 0) {
				if(c->rnext)
					c->rnext->Sl_Check();
			}else{
				if(c->rprev)
					c->rprev->Sl_Check();
			}
			break;
		}
	}
}

void RRadioButton::MButton(u32 b)
{
	if(b & M_CLICK)
	if(!bChecked) {
		Sl_Check();
	}
}

void RRadioButton::KDown(u32 d)
{
	switch(d) {
	case RK_LEFT:
	case RK_UP:   if(rprev) rprev->Sl_Focus(); break;
	case RK_RIGHT:
	case RK_DOWN: if(rnext) rnext->Sl_Focus(); break;
	case RK_SPACE: MButton(M_CLICK | 1); break;
	}
}

void RRadioButton::Sl_Check()
{
	for(RRadioButton *c = rfirst; c; c = c->rnext) {
		if(c == this) {
			continue;
		}
		if(c->bChecked) {
			c->Sl_UnCheck();
		}
	}
	RRadioButton_P::Sl_Check();
}

// RRadioButton
//--------------------------------------------------------------------------++++
// RRange

void RRange::KDown(u32 d)
{
	switch(d) {
	case RK_RIGHT: case RK_DOWN: current++; break;
	case RK_LEFT: case RK_UP: current--; break;
	case RK_PGUP: current -= 16; break;
	case RK_PGDN: current += 16; break;
	case RK_HOME: current = 9; break;
	case RK_END: current = width-10; break;
	default: return;
	}
	if(current < 9) current = 9;
	if(current > width - 10) current = width - 10;
	Si_Moved(current-9);
	Draw();
}

void RRange::MMove()
{
	int r;
	if(HasGrab()) {
		if(align & AL_V) {
			r = height;
			current = GetMY();
		}else{
			r = width;
			current = GetMX();
		}
		if(current < 9) current = 9;
		if(current > r - 10) current = r - 10;
		Si_Moved(current-9);
		Draw();
	}
}

void RRange::MScroll(int b,int d)
{
	int r = align & AL_V ? height : width;
	current += d * 12;
	if(current < 9) current = 9;
	if(current > r - 10) current = r - 10;
	Si_Moved(current-9);
	Draw();
}

void RRange::MButton(u32 b)
{
	if(b & M_DOWN)
		MMove();
}

// RRange
//--------------------------------------------------------------------------++++
// RScrollBar

void RScrollBar::Sl_Scroll(int p)
{
	SetL(p);
}

void RScrollBar::Sl_Adjust(int m, int v)
{
	int lp = lpos;
	bool bh = (bHidden & 1);
	if(v != -1)
		lv = v;
	if(m != -1)
		lmax = m;

	if(m && lmax > lv) { //m > v ???
		if(lpos < 0) lpos = 0;
		if(lpos > lmax) lpos = lmax;
		ppos = (lpos * pmax) / lmax;
		if(bCollapse)
			bHidden &= ~1;
	}else{
		if(bCollapse)
			bHidden |= 1;
	}
	if(lp != lpos) {
//		printf("XXXX adjusting lpos XXXX\n");
		//Si_Scrolled(lpos);
	}
	if(bh != (bHidden & 1) && bCollapse) {
	//	parent->ReCalc();
	}else{
		ReCalcI();
	}
	Draw();
}

void RScrollBar::ReCalcI(bool b)
{
	//int pm = ((xx*)pDrawer)->GetMax(); TODO
	int pm = align & AL_V ? height - width*3 : width - height*3; // temp
	pmax = (int)(pm - ((float)pm) * ((float)lv/(float)(lmax ? lmax : 1))); //TODO...
	if(pmax < 0) pmax = 0;
	if(pmax > pm) pmax = pm;
}

// TODO move into pDrawer...
int RScrollBar::Gett()
{
	int b,l,m;
	if(align & AL_V) {
		m = GetMY();
		l = height;
		b = width;
	}else{
		m = GetMX();
		l = width;
		b = height;
	}
	if(m < b) return -1;
	if(m >= l-b) return 1;
	if(m < ppos+b) return -16;
	if(m >= l-b-(pmax-ppos)) return 16;
	return 0;
}

void RScrollBar::MMove()
{
	if(HasGrab()) {
		int i = Gett();
		if(i) {
			AdjustL(i);
		}else{
			AdjustP(align & AL_V ? GetMRY() : GetMRX());
		}
	}
}

void RScrollBar::MButton(u32 b)
{
	if(b & M_DOWN) {
		int i = Gett();
		if(i) {
			AdjustL(i);
		}
	}
}

void RScrollBar::MScroll(int b,int d)
{
	AdjustP(d * 12); //TODO scroll len
}

void RScrollBar::KDown(u32 d)
{
	switch(d) {
	case RK_RIGHT: case RK_DOWN: AdjustL( 1); break; //TODO linelen
	case RK_LEFT:  case RK_UP:   AdjustL(-1); break;
	case RK_PGDN: AdjustL( 16); break;
	case RK_PGUP: AdjustL(-16); break; //TODO pagelen
	case RK_HOME: SetL(0);    break;
	case RK_END:  SetL(lmax); break;
	default: return;
	}
}

void RScrollBar::AdjustP(int i)
{
	ppos += i;
	if(ppos < 0) ppos = 0;
	if(ppos > pmax) ppos = pmax;
	if(pmax) {
		lpos = (ppos * lmax) / pmax;
	}else{
		lpos = 0;
	}
	Si_Scrolled(lpos);
	Draw();
}

inline void RScrollBar::AdjustL(int i)
{
	SetL(lpos + i);
}

void RScrollBar::SetL(int i)
{
	if(lpos == i)
		return;
	lpos = i;
	if(lpos < 0) lpos = 0;
	if(lpos > lmax) lpos = lmax;
	if(lmax) {
		ppos = (lpos * pmax) / lmax;
	}else{
		ppos = 0;
	}
	Si_Scrolled(lpos);
	Draw();
}

// RScrollBar
//--------------------------------------------------------------------------++++
// RStatusBar
// RStatusBar
//--------------------------------------------------------------------------++++

