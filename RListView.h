// vim:ts=4 sts=0 sw=4

#ifndef LISTVIEW_H
#define LISTVIEW_H

class SListNode
{
public:
	SListNode(SListNode *p, char *cc = "ABC");
	~SListNode();
	int Count();
	SListNode *GetNode(int);
	SListNode *GetNode(void*);
	SListNode *GetNode(SListNode*);
	int GetIndex(SListNode *);


	void SetText(const char *c) { strncpy(line,c,80); line[80] = 0; };

	SListNode *prev;
	SListNode *next;
	char line[82];
	void *user;
};

class RTreeNode;
#define RListView_P RControl
class RListView : public RControl
{
protected:
	RListView() {};
	void Init(RContainer *r);
	void Dinit() { RListView_P::Dinit(); if(ln) delete ln; };
public:
	void GetDrawer(u32 d);
	RListView(RContainer *r) { Init(r); };
	~RListView() { DINIT(); };

	SISL(1);
	SLOT(Sl_ScrollY)(int);
	SIGNAL(Si_ScrolledY)(int);
	SIGNAL(Si_AdjustX)(int,int);
	SIGNAL(Si_AdjustY)(int,int);
	void ReCalcI(bool b);

	SIGNAL(Si_Selected)(SListNode*);
	virtual SLOT(Sl_Setup)(char*) {};

	void SetList(SListNode *);
	void Scroll(int,int);

	void MScroll(int b,int d);
	//void MMove();
	void MButton(u32 b);
//	void MScroll(int b,int d);

	void KDown(u32 d);

	void Sort();


	void AddItem(char *, void *v = 0);
	void Select(SListNode *n) { if(ln) Si_Selected(sel = ln->GetNode(n)); };
	void Select(void *v) { if(ln) Si_Selected(sel = ln->GetNode(v)); };
	void Select(int i) { if(ln) Si_Selected(sel = ln->GetNode(i)); };
	void Clear();
	int GetIndex() { if(ln) return ln->GetIndex(sel); else return 0; };



	SListNode *ln,*last;
	SListNode *sel; // selection - for cmp only
	SListNode *dln;
	int ooy;
};

#define RListViewDir_P RListView
class RListViewDir : public RListView
{
protected:
	RListViewDir() {};
public:
	RListViewDir(RContainer *r) { Init(r); };
	~RListViewDir() { DINIT(); };
	
	void Sl_Setup(char*);
};

CLASS_RSPP(ListPath)	
	bool Match(const char *sig, const char *sisl);
	void Call(void *ths, void **ppv);
};

//-----------------------------------------------

#define RListViewPopup_P RListView
class RListViewPopup : public RListView
{
protected:
	RListViewPopup() {};
//	void Init(RContainer *r);
//	void Dinit() { RListViewPopup_P::Dinit(); if(ln) delete ln; };
public:
	RListViewPopup(RContainer *r) { Init(r); };
	~RListViewPopup() { DINIT(); };
	SISLTN();

	void MButton(u32 b);
};

#define RPopupList_P RPopup
class RPopupList : public RPopup
{
protected:
	RPopupList() {};
	void Init(RWindow *rw)
	{ RPopupList_P::Init(rw,0,0,0,0); bSelfDelete = false; x_mgn = y_mgn = spacing = 0; };
public:
	RPopupList(RWindow *rw)
	{
		Init(rw);
		lv = new RListViewPopup(this);
		lv->SetText("ListViewC");
		lv->SetAlignment(AL_STRETCHH|AL_STRETCHV);
		lv->frame = F_NONE;
	};
	~RPopupList() { DINIT(); };
	SISLTN();

	void AddItem(char *c, void *v = 0) { lv->AddItem(c,v); };
	void Select(SListNode *n) { lv->Select(n); };
	void Select(void *v) { lv->Select(v); };
	void Select(int i) { lv->Select(i); };
	void Clear() { lv->Clear(); };
	int GetIndex() { return lv->GetIndex(); };

	 //XXX just override this with nothing
	void GetMinSize(int &x, int &y) { /*pDrawer->GetMinSize(this,x,y);*/ };

//	int mx,my;
	RListView *lv;
};

#define RComboBox_P RButton
class RComboBox : public RButton
{
protected:
	RComboBox() {};
	void Init(RContainer *r);
public:
	void GetDrawer(u32 d);
	RComboBox(RContainer *r) { Init(r); };
	~RComboBox() { DINIT(); };

	void MButton(u32);
	void MScroll(int b,int d);
	void Popup();

	SISL(2);
	SLOT(Sl_Update)(SListNode*);
	SListNode *current;
	SIGNAL(Si_IndexChanged)(int,void*);

	RPopupList *pl;
	void AddItem(const char *c, void *v = 0) { pl->AddItem((char*)c,v); };
	void Select(SListNode *n) { pl->Select(n); };
	void Select(void *v) { pl->Select(v); };
	void Select(int i) { pl->Select(i); };
	void Clear() { pl->Clear(); };
	int GetIndex() { return pl->GetIndex(); };
};

/*	void KUp(u32);
	void MExit(bool);
	void MEnter(bool b) { Sl_Focus(); };
	void MHover();*/


#endif //LISTVIEW_H

