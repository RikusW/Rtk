// vim:ts=4 sts=0 sw=4

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "Rtk.h"

class STextLine
{
public:
	STextLine() { prev = next = NULL;
				  iline = 0;
				/*line = (char*)malloc(300);*/ *line = 0;
				memset(line,0,300); };
	~STextLine() {
		if(next) delete next;
	//	if(line) free(line);
	};

	void DelChar(int x);
	void AddChar(int &,char,bool);

	void Join();
	void Split(int i);
	STextLine *AddLine();
	void SetText(const char *);
	STextLine *GetLine(int);

	STextLine *prev;
	STextLine *next;
	//char *line;
	char line[300];

	void ResetILine();
	int iline;
};

class STextDummy : public STextLine
{
public:
	STextDummy() {};
	~STextDummy() {};

	void DelChar(int x) {};
	void AddChar(int &,char,bool) {};
	void Join() {};
	void Split(int i) {};
	STextLine *AddLine() { return /*NULL*/0; };
	void SetText(const char *) {};
	STextLine *GetLine(int) { return this; };
	void ResetILine() {};
};

#define RTextEdit_P RControl
class RTextEdit : public RControl
{
protected:
	RTextEdit() {};
	void Init(RContainer *r);
	void Dinit() { RTextEdit_P::Dinit(); if(stext) delete stext; };
public:
	void GetDrawer(u32 d);
	RTextEdit(RContainer *r) { Init(r); };
	~RTextEdit() { DINIT(); };

	void Scroll(int x, int y);

	STextLine *stext;

	void SetText(const char *);
	int LineCount();
	

	void MMove();
	void MButton(u32 b);
	void MScroll(int b,int d);

	void KChar(u32 d);
	void KDown(u32 d);
	void KUp(u32 d);

	SISL(2);
	SIGNAL(Si_ScrolledX)(int);
	SIGNAL(Si_ScrolledY)(int);
	SLOT(Sl_ScrollX)(int);
	SLOT(Sl_ScrollY)(int);
	SIGNAL(Si_AdjustX)(int,int);
	SIGNAL(Si_AdjustY)(int,int);

	void ReCalcI(bool b=true);

	int kx,kxx,ky; // cursor
	int oox,ooy;

	void SelReset() { s_bx = s_by = s_ex = s_ey = s_cx = s_cy = 0; };
	int s_bx,s_by, s_ex,s_ey ,s_cx,s_cy;

	int ki;
	bool bIns;
	bool bSquare;
};

//-------------------------------------------------------------------

#define REditBox_P RControl
class REditBox : public RControl
{
protected:
	REditBox() {};
	void Init(RContainer *r) { REditBox_P::Init(r); kx = 0; };
public:
	void GetDrawer(u32 d);
	REditBox(RContainer *r) { Init(r); };
	~REditBox() { DINIT(); };
	SISLTN();

	void MMove();
	void MButton(u32 b);
	void KChar(u32 d);
	void KDown(u32 d);
	void KUp(u32 d);

	void AddChar(int&,char);
	void DelChar(int);

	int kx; // cursor
	bool bIns;
};

#define RHexBox_P REditBox
class RHexBox : public REditBox //Temp hack...
{
protected:
	RHexBox() {};
	void Init(RContainer *r) { RHexBox_P::Init(r); bIns = false; SetText("00 00 00 00"); };
public:	
	RHexBox(RContainer *r) { Init(r); };
	~RHexBox() { DINIT(); };
	SISLTN();

	void KChar(u32 d);
	void KDown(u32 d);
	bool HexCheck(char&);

	void GetBytes(u8*);
	void SetBytes(const u8*);
};

#endif //TEXTEDIT_H

