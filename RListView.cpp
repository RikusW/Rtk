// vim:ts=4 sts=0 sw=4

#include "Rtk.h"
#include "RListView.h"

//---------------------------------------------------------------------------------------

SListNode::SListNode(SListNode *p, const char *cc)
{
	*line = 0;
	next = NULL;

	if((prev = p)) {
		prev->next = this;
	}
	SetText(cc);
};

SListNode::~SListNode()
{
	if(next) delete next;
};

int SListNode::Count()
{
	int i;
	SListNode *s = this;
	for(i = 0; s; i++, s = s->next);
	return i;
}

SListNode *SListNode::GetNode(int n)
{
	SListNode *s = this;
	for(int i = 0; s && (i < n); i++, s = s->next);
	if(s) {
		return s;
	}
	return this; //???
}

SListNode *SListNode::GetNode(void *v)
{
	SListNode *s = this;
	for(; s && s->user != v; s = s->next);
	if(s) {
		return s;
	}
	return this; //???
}

SListNode *SListNode::GetNode(SListNode *n)
{
	SListNode *s = this;
	for(; s && s != n; s = s->next);
	if(s) {
		return s;
	}
	return this; //???
}

int SListNode::GetIndex(SListNode *n)
{
	int i;
	SListNode *s = this;
	for(i = 0; s && s != n; s = s->next, i++);
	if(s) {
		return i;
	}
	return 0;
}

//-----------------------------------------------------------------------------

GETDRAWER(RListView,RLISTVIEW);
void RListView::Init(RContainer *r)
{
	RListView_P::Init(r);
	dln = sel = ln = last = NULL;
	ooy = 0;
};

void RListView::SetList(SListNode *l)
{
	if(ln) {
		delete ln;
		ln = last = NULL;
	}
	dln = ln = l;
}

void RListView::ReCalcI(bool b)
{
	Si_AdjustX(0,0);
	Si_AdjustY(ln->Count() - 1,height / 16);
}

void RListView::Sl_ScrollY(int y)
{
	if(ooy != y) {
		Scroll(0,y - ooy);
	}
}

void RListView::Scroll(int x, int y) // see RTextEdit
{
	int i;
	if(y > 0) {
		for(i = 0; i < y; i++) {
			if(dln->next)
				dln = dln->next;
			else
				break;
		}
	}else if(y < 0)	{
		for(i = 0; i > y; i--) {
			if(dln->prev)
				dln = dln->prev;
			else
				break;
		}
	}else{
		return;
	}
	ooy += i;
	Si_ScrolledY(ooy);
	Draw();
}

void RListView::MScroll(int b,int d)
{
	Scroll(0,d);
}
/*
void RListView::MMove()
{
}*/

void RListView::MButton(u32 b)
{
	SListNode *s = dln->GetNode(GetMY() / 16); //XXX sz
	if(b & M_DOWN)
	switch(b & M_MASK) {
	case 1:
		if(!(b & M_DBL)) {
			sel = s;
			Si_Selected(sel);
			break;
		}
	default:
		return;
	}
	Draw();
}


void RListView::KDown(u32 d)
{
	if(!sel)
		return;

	switch(d) {
	case RK_UP:
	case RK_LEFT:
		if(sel->prev) {
			sel = sel->prev;
			Si_Selected(sel);
		}
		break;
	case RK_DOWN:
	case RK_RIGHT:
		if(sel->next) {
			sel = sel->next;
			Si_Selected(sel);
		}
		break;
	default:
		return;
	}
	Draw();
}

void RListView::Sort()
{
	SListNode *n,*s,*t,*l;

	l = ln->next;
	s = ln;
	ln->next = 0;
	while(l) {
		n = l;
		l = l->next;
		for(t = s;; t = t->next) {
			if(strcmp(n->line, t->line) < 0) {
				n->prev = t->prev;
				n->next = t;
				t->prev = n;
				if(n->prev) {
					n->prev->next = n;
				}else{
					s = n;
				}
				break;
			}
			if(!t->next) {
				t->next = n;
				n->prev = t;
				n->next = 0;
				break;
			}
			
		}
	}

	dln = ln = s;
}

void RListView::AddItem(char *c, void *v)
{
	last = new SListNode(last,c);
	last->user = v;
	if(!ln) {
		sel = dln = ln = last;
		Si_Selected(sel); //XXX good thing or bad thing ? ---> make configurable XXX
	}
}
/*
void RListView::Select(SListNode*)
{
}

void RListView::Select(void*)
{
}

void RListView::Select(u32)
{
}
*/
void RListView::Clear()
{
	if(ln) {
		delete ln;
	}
	ln = dln = last = sel = NULL;
//	Draw(); ???
}

//---------------------------------------------------------------------------------------

#include "Rtk-base/RFile.h"
#include "RTreeView.h"

void BuildPathLX(RTreeNode *t, char *c)
{
	if(t->parent) {
		BuildPathLX(t->parent, c);
	}

	strcat(c, t->line);
	strcat(c, "/");
}

void RListViewDir::Sl_Setup(char*n)
{
	int i;
	RDir r;
	SListNode *l=0;

	SetText(n);
//	printf("LV Setup %s\n",n);
	for(const char *cc = r.GetFirst(&i,n); cc; cc = r.GetNext(&i)) {
		l = new SListNode(l,(char*)cc);
	}
	if(l) for(; l->prev; l = l->prev);
	SetList(l);
	if(l) Sort();
	sel = ln; //XXX bugfix
	ReCalcI(false);
	Draw();	
}

bool RSpp_ListPath::Match(const char *sig, const char *sisl)
{
	return RSpp::SppMatch(sig,"(SListNode*)","(char*)",sisl);
};

void RSpp_ListPath::Call(void *ths, void **ppv)
{
	RDir r;
	char buf[500]; *buf=0;
	RListView *v = (RListView*)Rtk_GetFPTR();
	SListNode *l = (SListNode*)*ppv;
	if(v) {
		strcpy(buf,v->text);
	}
	if(!strcmp(l->line,".")) {
		return;
	}
	if(!strcmp(l->line,"..")) {
		int i = strlen(buf) - 1;
		if(i) for(buf[i--] = 0; i > 0 && buf[i] != '/'; buf[i--] = 0);
	}else{
		strcat(buf,l->line);
		strcat(buf,"/");
	}
	//printf("new dir->%s<-\n",buf); //XXX debug

	if(!r.GetFirst(0,buf)) {
		return;
	}
	void *pv = (void*)buf;
	NextCall(ths,&pv);
}




//---------------------------------------------------------------------------------------

void RListViewPopup::MButton(u32 b)
{
	RListViewPopup_P::MButton(b);
	if((b & 0x1F) == 1) {
		if(!(b & M_DOWN)) {
			wnd->UnfocusPopups();
		}
	}
}

//-------------------------------------

GETDRAWER(RComboBox,RCOMBOBOX);

void RComboBox::Init(RContainer *r)
{
	RComboBox_P::Init(r);
	pl = new RPopupList(wnd);
	pl->lv->Connect("Si_Selected(SListNode*)",this,"Sl_Update(SListNode*)");
	current = NULL;
}

void RComboBox::MButton(u32 b)
{
	RComboBox_P::MButton(b);
//	if(b & M_CLICK)
//		Sl_Clicked(b);

	if((b & 0x1F) == 1) {
		if(b & M_DOWN) {
			Popup();
		}
	}
}

void RComboBox::MScroll(int b,int d)
{
	if(!current) {
		Sl_Update(pl->lv->ln); //XXX ???
		return;
	}
	if(d > 0) {
		if(current->next) {
			Sl_Update(current->next);
		}
	}else{
		if(current->prev) {
			Sl_Update(current->prev);
		}
	}
	pl->lv->sel = current; //XXX oh well, it works...
}

void RComboBox::Popup()
{
	if(pl) {
	if(pl->wnd->hwnd) {
		wnd->UnfocusPopups();
	}else{
		pl->x_min = width; //XXX hack...
		pl->y_min = 200; //XXX hack
		pl->SetPosW(left,bottom + 1);
//		pl->SetSize(width,300);
		pl->Popup(wnd);
		pl->ReCalc(true);
//		pl->SetSizeW(width,300);
	}}
}

void RComboBox::Sl_Update(SListNode *n)
{
	int i = 0;
	current = n;
	Sl_SetText(n->line);
	for(SListNode *p = n; p->prev && i < 10000; i++, p = p->prev);
	Si_IndexChanged(i,n->user);
}

//---------------------------------------------------------------------------------------

