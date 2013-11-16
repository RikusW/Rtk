// vim:ts=4 sts=0 sw=4

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "Rtk.h"
#include "RTreeNode.h"

class RTreeNodeDir : public RTreeNode
{
public:
	RTreeNodeDir(RTreeNode *p, char *cc = "ABC") : RTreeNode(p,cc) { bInit = false; };
	~RTreeNodeDir() {};

	RTreeNode *GetChild();
	bool bInit;
};

CLASS_RSPP(TreePath)	
	bool Match(const char *sig, const char *sisl);
	void Call(void *ths, void **ppv);
};

#define RTreeView_P RControl
class RTreeView : public RControl
{
protected:
	RTreeView() {};
	void Init(RContainer *r);
	void Dinit() { RTreeView_P::Dinit(); if(tn) delete tn; };
public:
	void GetDrawer(u32 d);
	RTreeView(RContainer *r) { Init(r); };
	~RTreeView() { DINIT(); };

	void SetTree(RTreeNode *t) { dtn = tn = t; }; //XXX leak!!!
//	void SetTree(RTreeNode *t) { if(tn) delete tn; dtn = tn = t; };
	void Scroll(int,int);

	SISL(1);
	SLOT(Sl_ScrollY)(int);
	SIGNAL(Si_ScrolledY)(int);
	SIGNAL(Si_AdjustX)(int,int);
	SIGNAL(Si_AdjustY)(int,int);
	void ReCalcI(bool b);

	SIGNAL(Si_Selected)(RTreeNode*);
	//OT(Sl_Select(RTreeNode*)); //???
	virtual SLOT(Sl_Setup)(char*) {};

	void MScroll(int b,int d);
//	void MMove();
	void MButton(u32 b);
//	void MScroll(int b,int d);

	void KDown(u32 d);

	void Expand(RTreeNode *t);
	int GetNodeOffset(RTreeNode *n) {
		if(!n)
			return 0;
		return 10 * n->GetDepth();
	}

	RTreeNode *tn;
	RTreeNode *sel; // selection - for cmp only
	RTreeNode *dtn;
	int ooy;
};

#endif //TREEVIEW_H

