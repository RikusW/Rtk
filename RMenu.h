// vim:ts=4 sts=0 sw=4

#ifndef MENU_H
#define MENU_H

#include "Rtk.h"

class RPopupMenu;
class RTreeNode;
#define RMenuButton_P RButton
class RMenuButton : public RButton
{
protected:
	RMenuButton() {};
	void Init(RContainer *r);
public:
	void GetDrawer(u32 d);
	RMenuButton(RContainer *r) { Init(r); };
	~RMenuButton() { DINIT(); };
	SISLTN();


	void KUp(u32);
	void MButton(u32);
	void MExit(bool);
	void MEnter(bool b) { Sl_Focus(); };
	void MHover();

	void Popup();

	RPopupMenu *pm;
	RTreeNode *tree;
};



//#define RMenuBar_P RControl
//class RMenuBar : public RControl
#define RMenuBar_P RHVBox
class RMenuBar : public RHVBox
{
protected:
	RMenuBar() {};
	void Init(RContainer *r)
	{ RMenuBar_P::Init(r);
		SetAlignment(AL_H | AL_FITH | AL_FITV | AL_STRETCHH);
		x_weight = 10;
		//frame = F_UP;
		x_mgn = 0;
		y_mgn = 0;
		spacing = 0;
		SetText("");
	};
public:
	RMenuBar(RContainer *r) { Init(r); };
	~RMenuBar() { DINIT(); };
	SISLTN();

	void BuildMenu(CConn *,const char *);
	void BM(CConn *,RPopupMenu *pm, const char *&);
	
	RPopupMenu *AddPopup(char *c);

	bool IsDropped();
};

#define RMenuBarButton_P RMenuButton
class RMenuBarButton : public RMenuButton
{
protected:
	RMenuBarButton() {};
public:
	RMenuBarButton(RMenuBar *r)
		{ RMenuBarButton_P::Init(r); x_weight = 1; align = AL_STRETCHH; };

	RMenuBarButton(RMenuBar *r,char *c)
		{ RMenuBarButton_P::Init(r); x_weight = 1; SetText(c); align = AL_STRETCHH; };

	~RMenuBarButton() { DINIT(); };
	SISLTN();

	void KUp(u32);
	void MButton(u32);
	void MHover();
	void MEnter(bool);
};

#define RPopupMenu_P RPopup
class RPopupMenu : public RPopup
{
protected:
	RPopupMenu() {};
	void Init(RWindow *rw)
	{ RPopupMenu_P::Init(rw,0,0,0,0); bSelfDelete = false; x_mgn = y_mgn = spacing = 0; };
public:
	RPopupMenu(RWindow *rw) { Init(rw); };
	RPopupMenu(RMenuButton *mb)
	{
		Init(mb->wnd);
		mb->pm = this;
	};
	~RPopupMenu() { DINIT(); };
	SISLTN();

	RMenuButton *AddItem(char *c) {
		RMenuButton *b;
		b = new RMenuButton(this);
		b->SetText(c);// b->x_weight = 1;
		return b;
	};
	RMenuButton *AddItemC(char *c,CConn *cn,char *sl) {
		RMenuButton *mb = AddItem(c);
		mb->Connect("Si_Clicked(int)",cn,sl);
		return mb;
	};
};

#endif //MENU_H

