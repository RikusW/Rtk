// vim:ts=4 sts=0 sw=4

#include "RTextEdit.h"
#include "RMenu.h"
#include "Rtk-base/RString.h"

//-----------------------------------------------------------------------------

GETDRAWER(RTextEdit,RTEXTEDIT);
void RTextEdit::Init(RContainer *r)
{
	RTextEdit_P::Init(r);

	oox = ooy = 0;
	kx = kxx = ky = 0;
	bSquare = false;

	s_bx = s_ex = s_cx = 0;
	s_by = s_ey = s_cy = 0;

	stext = new STextLine();
}

void RTextEdit::MMove()
{
	int tx = GetMX() / 8; //XXX sz
	int ty = GetMY() / 13; //XXX sz
	if(wnd->MDrag) {
#ifdef DRAG1
		bSquare = wnd->bCtrl;
#endif
		if((ty > s_cy) || ((ty == s_cy) && (tx > s_cx))) {
			s_bx = s_cx;
			s_by = s_cy;
			s_ex = tx;
			s_ey = ty;
		}else{
			s_bx = tx;
			s_by = ty;
			s_ex = s_cx;
			s_ey = s_cy;
		}
		Draw();
	}
}

void RTextEdit::MButton(u32 b)
{
	int tx = GetMX() / 8; //XXX sz
	int ty = GetMY() / 13; //XXX sz

	if(b & M_DOWN)
	switch(b & M_MASK) {
	case 1: {
#ifndef DRAG1
		bSquare = wnd->bCtrl;
#endif
		kx = kxx = tx; ky = ty;
		int x = (int)strlen(stext->GetLine(ky)->line); //XXX
		if(kx > x)
			kx = x;

		s_bx = s_ex = s_cx = tx;
		s_by = s_ey = s_cy = ty;
		Draw();
		break;
	}
	case 2: {
		//stext->GetLine(ty)->Split(tx);
		break;
	}
	case 3: {
		//stext->GetLine(ty)->Join();
		RPopupMenu *w;
		w = new RPopupMenu(wnd);//aaa,cx+left,cy+top,100,200);
		w->SetPosW(GetMX()+left,GetMY()+top);
		w->bSelfDelete = true;
		w->AddItem((char*)"Context Menu"); // -0-
		w->AddItem((char*)"Cut");//->tree = textpopup;
		w->AddItem((char*)"Copy");
		w->AddItem((char*)"Paste");
		w->AddItem((char*)"Exit"); // -4-

		w->Popup(wnd);
		
/*		w->Create(wnd);
		w->SetAlignment(AL_V | AL_FITV | AL_FITH);
		w->ReCalc();
		w->ShowW();
		w->SetFocusW();*/
		break;
	}
	}
}

void RTextEdit::MScroll(int b,int d)
{
	Scroll(0,d);
}

void RTextEdit::Sl_ScrollX(int x)
{
	if(x) {
		Si_AdjustX(0,0);
		parent->ReCalc(true);
		parent->Draw(true);
	}
}

void RTextEdit::Sl_ScrollY(int y)
{
	if(ooy != y) {
		Scroll(0,y - ooy);
	}
}

int RTextEdit::LineCount()
{
	int i=0;
	STextLine *l;
	if(!stext)
		return 0;
	for(l = stext; l->prev; l = l->prev, i++);
	for(l = stext; l->next; l = l->next, i++);
	return ++i;
}

void RTextEdit::ReCalcI(bool b)
{
	Si_AdjustY(LineCount() - 1, height / 13); //XXX 13 must be from Drawer !!!
	Si_AdjustX(0,0);
}

void RTextEdit::Scroll(int x, int y)
{
	int i;
	if(y > 0) {
		for(i = 0; i < y; i++) {
			if(stext->next)
				stext = stext->next;
			else
				break;
		}
	}else if(y < 0)	{
		for(i = 0; i > y; i--) {
			if(stext->prev)
				stext = stext->prev;
			else
				break;
		}
	}else{
		return;
	}
	ky -= i;
	ooy += i;
	Si_ScrolledY(ooy);
	Draw();
}


void RTextEdit::SetText(const char *txt)
{
	int i;
	char buf[400];

	SelReset();

	if(!txt)
		return;

	if(stext)
		delete stext;

	stext = new STextLine();
//	((RTextLine*)child)->stext = stext;
	STextLine *l = stext;

	while(*txt) {
		for(i=0; i<290 && *txt; i++) {
			if(*txt == '\n') {
				buf[i] = 0;
				l->SetText(buf);
			//	l->se = 5; l->ss = 2;
				l = l->AddLine();
				txt++;
				goto xxx;
			}
			buf[i] = *txt++;
		}
		buf[i] = 0;
		l->SetText(buf);
xxx: {}
	}

//zz	SetLines((RTextLine*)child);

	Si_AdjustX(0,0);
	Si_AdjustY(LineCount() - 1, height / 13); //XXX 13 must be from Drawer !!!
	return;
}

void RTextEdit::KChar(u32 d)
{
	STextLine *st = stext->GetLine(ky);

	if(!bFocus || /*!text ||*/ !st) {
		return;
	}

	switch(d)
	{
	// bs		lf			esc			tab				cr		del in X11
	case 0x08: case 0x0A: case 0x1B: /*case 0x09:*/ case 0x0D: case 0x7F:
		return;
	case 0x09:
		st->AddChar(kx,' ',bIns);
		st->AddChar(kx,' ',bIns);
		st->AddChar(kx,' ',bIns);
		d = ' ';
		break;
	}

	SelReset(); //XXX SelDelete ???
	st->AddChar(kx,(char)d,bIns);
	if(kx > 90)
		kx = 90;
	kxx = kx;
//	SetKi(ki > 90 ? 90 : ki); //XXX
	Draw();
}

void RTextEdit::KDown(u32 d)
{
	STextLine *st = stext->GetLine(ky);

	switch(d)
	{
	case RK_BACK:
		if(kx > 0) {
			st->DelChar(--kx); kxx = kx;
		}else{
			if(ky == 0) {
				Scroll(0,-1);
			}
			if(ky > 0) {
				st = stext->GetLine(--ky);
				kx = kxx = strlen(st->line);
				st->Join();
				Si_AdjustY(LineCount() - 1, -1);
			}
		}
		break;

	case RK_DELETE:
		if(st->line[kx]) {
			st->DelChar(kx);
		}else{
			st->Join();
			Si_AdjustY(LineCount() - 1, -1);
		}
		break;

	case RK_INSERT:
		bIns =! bIns;
		break;

	case RK_LEFT:
		if(--kx<0) {
			if(--ky<0) {
				kx = ky = 0;
			}else{
				kx = st->prev ? strlen(st->prev->line) : 0;
			}
		}
		kxx = kx;
		break;
	case RK_RIGHT:
		kx++;
		if(kx > (int)strlen(st->line)) {

			ky++; kx = 0; //XXX ???? ky too big ????
			//kx = strlen(st->line);
		}
		kxx = kx;
		break;

	case RK_HOME:
		kx = kxx = 0;
		break;
	case RK_END:
		kx = kxx = strlen(st->line);
		break;

	case RK_PGUP:
		Scroll(0,-10);
		break;
	case RK_PGDN:
		Scroll(0,10);
		break;

	case RK_UP:
		if(ky > 0) {
			ky--;
		}else{
			Scroll(0,-1);
		}
		{
		int x = (int)strlen(stext->GetLine(ky)->line); //XXX
		if(kxx > x)
			kx = x;
		else
			kx = kxx;
		}
		break;

	case RK_RETURN:
		st->Split(kx);
		kx = kxx = 0;
		Si_AdjustY(LineCount() - 1, -1);
		//fallthrough
	case RK_DOWN:
		if(!st->next)
			break;
		if(ky < height / 13 - 1) { //XXX
			ky++;
		}else{
			Scroll(0,1);
		}
		{
		int x = (int)strlen(stext->GetLine(ky)->line); //XXX
		if(kxx > x)
			kx = x;
		else
			kx = kxx;
		}
		break;

#ifdef DRAG1
	case RK_CTRL:
		if(wnd->MDrag)
			bSquare = true;
		break;
#endif
	default: return;
	}
	Draw();
}

void RTextEdit::KUp(u32 d)
{
	switch(d)
	{
#ifdef DRAG1
	case RK_CTRL:
		if(wnd->MDrag)
			bSquare = false;
		break;
#endif
	case RK_APPS:
		MButton(3 | M_DOWN);
	default: return;
	}
	Draw();
}

//-----------------------------------------------------------------------------
// STextLine

STextDummy td;

STextLine *STextLine::GetLine(int o) //XXX!!!
{
	int i = 0;
	STextLine *s = this;

	if(o < 0)
		return &td;

	while(s) {
		if(i >= o)
			break;
		s = s->next;
		i++;
	}
	if(!s)
		return &td;

	return s;
}

void STextLine::ResetILine()
{
	// reset all the line numbers following this one
	STextLine *t;
	int i = iline;
	for(t = next; t; t = t->next) {
		t->iline = ++i;
	}
}

void STextLine::Join()
{
	if(next) {
		strcat(line,next->line);

		STextLine *n = next;

		next = next->next;
		if(next)
			next->prev = this;

		n->next = NULL;
		delete n;

		ResetILine();
	}
}

void STextLine::Split(int i)
{
	int ln;
	ln = line ? strlen(line) : 0;

	STextLine *l = new STextLine();
	if(i >= ln) {
		*l->line = 0;
	}else{
		//int ll = strlen(line + i);
		//l->line = malloc(300);//ll+4); //XXX
		strcpy(l->line,line + i);
		*(line + i) = 0;
	}

	l->prev = this;
	l->next = next;

	if(next)
		next->prev = l;
	next = l;

	ResetILine();
};

STextLine *STextLine::AddLine()//const char *c)
{
	STextLine *l = new STextLine();
//	if(c && strlen(c) < 300) //XXX
//		strcpy(l->line,c);

	l->prev = this;
	l->next = next;

	if(next) {
		next->prev = l;
	}
	next = l;

	ResetILine();

	return l;
}

void STextLine::SetText(const char *c)
{
	if(c && strlen(c) < 300) //XXX
		strcpy(line,c);
}

void STextLine::AddChar(int &ki, char c, bool bIns)
{
	if(line[ki]) {
		if(bIns) {
			int m = strlen(line) + 1; // ins
			if(m > 299) //XXX
				return;
			for(line[m--] = 0; m > ki; m--) {
				line[m] = line[m-1];
			}
			line[ki++] = c;
		}else{
			line[ki++] = c; // ovr
		}
	}else{
		line[ki++] = c;
		line[ki] = 0;
	}
}

void STextLine::DelChar(int x)
{
	strcpy(line+x,line+x+1);
}

//-----------------------------------------------------------------------------

GETDRAWER(REditBox,REDITBOX);

void REditBox::MMove()
{
/*	int tx = GetMX() / 8; //XXX sz
	if(wnd->MDrag) {
		if(tx > s_cx) {
			s_bx = s_cx;
			s_ex = tx;
		}else{
			s_bx = tx;
			s_ex = s_cx;
		}
		Draw();
	}*/
}

void REditBox::MButton(u32 b)
{
	if(b & M_DOWN)
	if((b & M_MASK) == 1) {
		kx = GetMX() / 8 - 1;
		int x = strlen(text);
		if(kx < 0)
			kx = 0;
		if(kx > x)
			kx = x;
		Draw();
	}
}

void REditBox::KChar(u32 d)
{
//	printf("char = %x\n",d);
	switch(d)
	{
	// bs		lf			esc			tab			cr	del in X11
	case 0x08: case 0x0A: case 0x1B: case 0x09: case 0x0D: case 0x7F:
		return;
	}
	AddChar(kx,(char)d);
	Draw();
}

void REditBox::KDown(u32 d)
{
	switch(d)
	{
	case RK_BACK:   if(kx > 0) { DelChar(--kx); } break;
	case RK_DELETE:	if(text[kx]) { DelChar(kx);	} break;
	case RK_LEFT:   if(--kx<0) { kx = 0; } break;
	case RK_RIGHT: kx++;
		if(kx > (int)strlen(text)) {
			kx = (int)strlen(text);
		} break;
	default: return;
	}
	Draw();
}

void REditBox::KUp(u32 d)
{
	switch(d)
	{
	case RK_INSERT:	bIns =! bIns; break;
	case RK_HOME:	kx = 0;	break;
	case RK_END:	kx = strlen(text); break;
	default: return;
	}
	Draw();
}

void REditBox::AddChar(int &ki, char c)
{
	if(text[ki]) {
		if(bIns) {
			int m = strlen(text) + 1; // ins
			if(m > 98)
				return;
			for(text[m--] = 0; m > ki; m--) {
				text[m] = text[m-1];
			}
			text[ki++] = c;
		}else{
			text[ki++] = c; // ovr
		}
	}else{
		text[ki++] = c;
		text[ki] = 0;
	}
}

void REditBox::DelChar(int x)
{
	strcpy(text+x,text+x+1);
}

//-----------------------------------------------------------------------------

bool RHexBox::HexCheck(char &c)
{
	if(c >= '0' && c <= '9') {
		return true;
	}
	if(c >= 'A' && c <= 'F') {
		return true;
	}
	if(c >= 'a' && c <= 'f') {
		c &= 0xDF; //toupper
		return true;
	}
	return false;
}

void RHexBox::KChar(u32 d)
{
	char c = d;
	switch(c)
	{
	// bs		lf			esc			tab			cr	del in X11
	case 0x08: case 0x0A: case 0x1B: case 0x09: case 0x0D: case 0x7F:
		return;
	}

	switch(kx) {
	case 2:
	case 5:
	case 8:
		text[kx++] = ' ';
		//fallthrough
	case 0:
	case 1:
	case 3:
	case 4:
	case 6:
	case 7:
	case 9:
c9:		if(HexCheck(c)) {
			text[kx++] = c;
		}
		break;
	case 10:
		if(HexCheck(c)) {
			text[kx] = c;
			kx = 0;
		}
		break;
	default:
		kx = 0;
		goto c9;
	}
	Si_SetText(text);
	Draw();
}

void RHexBox::KDown(u32 d)
{
	if(d == RK_BACK || d == RK_DELETE) {
		return;
	}
	RHexBox_P::KDown(d);
}

void RHexBox::GetBytes(u8 *p)
{
	RString s,t;
	s = text;
	t.SplitL(s,' ');
	p[0] = t.FromHex();
	t.SplitL(s,' ');
	p[1] = t.FromHex();
	t.SplitL(s,' ');
	p[2] = t.FromHex();
	t.SplitL(s,' ');
	p[3] = t.FromHex();
}

void RHexBox::SetBytes(const u8 *p)
{
	RString s="",t;
//	s.p = text; //XXX
//	s.l = 0;
	t.ToHexN(p[0],2);
//	printf("nibble1 %s %02X\n",(const char *)t,p[0]);
	s += t;
	s += " ";
	t.ToHexN(p[1],2);
//	printf("nibble1 %s\n",(const char *)t);
	s += t;
	s += " ";
	t.ToHexN(p[2],2);
//	printf("nibble1 %s\n",(const char *)t);
	s += t;
	s += " ";
	t.ToHexN(p[3],2);
//	printf("nibble1 %s\n",(const char *)t);
	s += t;


//	printf("ntext %s\n",(const char *)s);
//	s.p[s.l] = 0; //XXX !!! fixme

	strcpy(text,s);

	Draw();
}

//-----------------------------------------------------------------------------

