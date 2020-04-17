// vim:ts=4 sts=0 sw=4

#include "Rtk.h"
#include "RTreeView.h"

//---------------------------------------------------------------------------------------

#include "RFile.h"

void BuildPathX(RTreeNode *t, char *c)
{
	if(t->parent) {
		BuildPathX(t->parent, c);
	}

	strcat(c, t->line);
	strcat(c, "/");
}

void ListDirX(RTreeNode *t, const char *n, int in)
{
	int i;
	RDir r;
//	printf("%s%s---\n",in ? "---":">>>", n);
	for(const char *cc = r.GetFirst(&i,n); cc; cc = r.GetNext(&i)) {
		if(i & RD_DDOT || !(i & RD_DIR)) {
			continue;
		}
		RTreeNode *n = new RTreeNodeDir(t, cc);
		n->bExpand = false;
		
		if(in) {
			char buf[256];
			*buf = 0;
			BuildPathX(n,buf);
		/*	strncpy(buf,(char*)n,255);
			strcat(buf,cc);
			strcat(buf,"/");*/
			ListDirX(n,buf, 0);
			n->Sort();
		}
	}
}

void InitX(RTreeNode *n)
{
	char buf[500];
	*buf=0;
	BuildPathX(n,buf);
	ListDirX(n,buf,1);
	n->Sort();
	((RTreeNodeDir*)n)->bInit = true;
}

RTreeNode *RTreeNodeDir::GetChild()
{
	if(child) {
		if(!bInit) {
			bInit = true;
			for(RTreeNode *n = child; n; n = n->next) InitX(n);
		}
	}else{
		InitX(this);
	}
//	printf("------\n");
	return child;
}

bool RSpp_TreePath::Match(const char *sig, const char *sisl)
{
	return RSpp::SppMatch(sig,"(RTreeNode*)","(char*)",sisl);
};

void RSpp_TreePath::Call(void *ths, void **ppv)
{
	char buf[500]; *buf=0;
	BuildPathX((RTreeNode*)*ppv,buf);
	void *pv = (void*)buf;
	NextCall(ths,&pv);
}

//-----------------------------------------------------------------------------

GETDRAWER(RTreeView,RTREEVIEW);
void RTreeView::Init(RContainer *r)
{
	RTreeView_P::Init(r);
	sel = dtn = tn = NULL;
	ooy = 0;
};

void RTreeView::ReCalcI(bool b)
{
	Si_AdjustX(0,0);
	Si_AdjustY(tn->Count(),height / 16);
}

void RTreeView::Sl_ScrollY(int y)
{
	if(ooy != y) {
		Scroll(0,y - ooy);
	}
}

void RTreeView::Scroll(int x, int y) // see RTextEdit
{
	int i;
	RTreeNode *t;
	if(y > 0) {
		for(i = 0; i < y; i++) {
			if(dtn != (t = dtn->GetNext()))
				dtn = t;
			else
				break;
		}
	}else if(y < 0)	{
		for(i = 0; i > y; i--) {
			if(dtn != (t = dtn->GetPrev()))
				dtn = t;
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

void RTreeView::MScroll(int b,int d)
{
	Scroll(0,d);
}
/*
void RTreeView::MMove()
{
}*/

void RTreeView::MButton(u32 b)
{
	RTreeNode *s = dtn->GetNode(GetMY() / 16); //XXX sz
	if(b & M_DOWN)
	switch(b & M_MASK) {
	case 1:
		if(!(b & M_DBL))
		if(GetMX() > GetNodeOffset(s)) {
			sel = s;
			Si_Selected(sel);
			break;
		}
	case 3:
		s->GetChild();
		s->bExpand =! s->bExpand;
		if(!s->bExpand && s->IsChild(sel,true)) {
			sel = s;
			Si_Selected(sel);
		}
		ReCalcI(0);
		break;
	default:
		return;
	}
	Draw();
}

void RTreeView::Expand(RTreeNode *n)
{
	if(n) {
		n->bExpand = true;
		for(n = n->child; n; n = n->next) {
			n->GetChild(); //late loading
			Expand(n);
		}
	}
}

void RTreeView::KDown(u32 d)
{
	int i;

	if(!sel)
		return;

	switch(d) {
	case RK_UP:
		if(sel == dtn) {
			sel = sel->GetPrev();
			Si_Selected(sel);
			Scroll(0,-1);
			return;
		}
		sel = sel->GetPrev();
		Si_Selected(sel);
		break;
	case RK_DOWN:
		sel = sel->GetNext();
		Si_Selected(sel);
		if(dtn->Count(sel) > height / 16) {
			Scroll(0,1);
			return;
		}
		break;
	case RK_LEFT:
		if(sel->bExpand && sel->child) {
			sel->bExpand = false;
			ReCalcI(0);
		}else{
			if(sel->parent) {
				sel = sel->parent;
				Si_Selected(sel);
			}
		}
		i = sel->GetNumber() - dtn->GetNumber();
		if(i < 0) {
			Scroll(0, i);
		}
		break;
	case RK_RIGHT:
		if(sel->GetChild()) {
			if(sel->bExpand) {
				sel = sel->GetChild();
				Si_Selected(sel);
			}else{
				sel->bExpand = true;
				ReCalcI(0);
			}
		}
		break;
	case RK_KPMUL:
		Expand(sel);
		ReCalcI(0);
		break;
	default:
		return;
	}
	Draw();
}


//---------------------------------------------------------------------------------------

