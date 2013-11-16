// vim:ts=4 sts=0 sw=4
//Automatically generated from: Rtk.h
//Don't edit, all changes _will be lost_.

#include "Rtk.h"

//---class RControl : CConn
/*---Signals---
Si_MEnter(bool)
Si_MExit(bool)
Si_SetText(char*)
Si_Focus(bool)
//---Slots-----
Sl_SetText(char*)
Sl_Show(bool)
Sl_Show()
Sl_Hide()
Sl_Enable(bool)
Sl_Enable()
Sl_Disable()
Sl_Focus(bool)
Sl_Focus()
Sl_UnFocus()
//-----------*/
//------------------------------
//---Start of connector stubs---

void RControl_Si_MEnter_b(void *ths, void **ppv)	{ ((RControl*)ths)->Si_MEnter((bool)(uptr)ppv[0]); }
void RControl_Si_MExit_b(void *ths, void **ppv)	{ ((RControl*)ths)->Si_MExit((bool)(uptr)ppv[0]); }
void RControl_Si_SetText_c(void *ths, void **ppv)	{ ((RControl*)ths)->Si_SetText((char*)(uptr)ppv[0]); }
void RControl_Si_Focus_b(void *ths, void **ppv)	{ ((RControl*)ths)->Si_Focus((bool)(uptr)ppv[0]); }
void RControl_Sl_SetText_c(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_SetText((char*)(uptr)ppv[0]); }
void RControl_Sl_Show_b(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Show((bool)(uptr)ppv[0]); }
void RControl_Sl_Show(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Show(); }
void RControl_Sl_Hide(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Hide(); }
void RControl_Sl_Enable_b(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Enable((bool)(uptr)ppv[0]); }
void RControl_Sl_Enable(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Enable(); }
void RControl_Sl_Disable(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Disable(); }
void RControl_Sl_Focus_b(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Focus((bool)(uptr)ppv[0]); }
void RControl_Sl_Focus(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_Focus(); }
void RControl_Sl_UnFocus(void *ths, void **ppv)	{ ((RControl*)ths)->Sl_UnFocus(); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RControl::*si_RControl[4] =
{
	&RControl::CSiSl_Si_MEnter,
	&RControl::CSiSl_Si_MExit,
	&RControl::CSiSl_Si_SetText,
	&RControl::CSiSl_Si_Focus,
};
static union { CSiSl RControl::**d; CSiSl CConn::**b; } upp_RControl = { si_RControl };

SiSlLink sil_RControl[15] =
{
	{ "Si_MEnter(bool)", RControl_Si_MEnter_b },
	{ "Si_MExit(bool)", RControl_Si_MExit_b },
	{ "Si_SetText(char*)", RControl_Si_SetText_c },
	{ "Si_Focus(bool)", RControl_Si_Focus_b },
	{ "Sl_SetText(char*)", RControl_Sl_SetText_c },
	{ "Sl_Show(bool)", RControl_Sl_Show_b },
	{ "Sl_Show()", RControl_Sl_Show },
	{ "Sl_Hide()", RControl_Sl_Hide },
	{ "Sl_Enable(bool)", RControl_Sl_Enable_b },
	{ "Sl_Enable()", RControl_Sl_Enable },
	{ "Sl_Disable()", RControl_Sl_Disable },
	{ "Sl_Focus(bool)", RControl_Sl_Focus_b },
	{ "Sl_Focus()", RControl_Sl_Focus },
	{ "Sl_UnFocus()", RControl_Sl_UnFocus },
	{ 0,0 }
};

SiSlData sd_RControl = { 0, "RControl", 4, upp_RControl.b, sil_RControl };

SiSlData *RControl::SiSl_GetSD()
{
	if(!sd_RControl.up) {
		sd_RControl.up = CConn::SiSl_GetSD();
	}
	return &sd_RControl;
}

SiSlTN tn_RControl = { 0, "RControl", 4, 10 };
const char *RControl::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RControl.up) {
			CConn::GetTypeName(&tn_RControl.up);
		}
		*tn = &tn_RControl;
	}
	return tn_RControl.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RControl::Si_MEnter(bool a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_MEnter,pv);
}

void RControl::Si_MExit(bool a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_MExit,pv);
}

void RControl::Si_SetText(char* a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_SetText,pv);
}

void RControl::Si_Focus(bool a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Focus,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RControl : CConn

//--TN--class RContainer : RControl
SiSlTN tn_RContainer = { 0, "RContainer", 0, 0 };
const char *RContainer::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RContainer.up) {
			RControl::GetTypeName(&tn_RContainer.up);
		}
		*tn = &tn_RContainer;
	}
	return tn_RContainer.name;
}

//done with class RContainer : RControl

//--TN--class RHVBox : RContainer
SiSlTN tn_RHVBox = { 0, "RHVBox", 0, 0 };
const char *RHVBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RHVBox.up) {
			RContainer::GetTypeName(&tn_RHVBox.up);
		}
		*tn = &tn_RHVBox;
	}
	return tn_RHVBox.name;
}

//done with class RHVBox : RContainer

//--TN--class RTBox : RContainer
SiSlTN tn_RTBox = { 0, "RTBox", 0, 0 };
const char *RTBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RTBox.up) {
			RContainer::GetTypeName(&tn_RTBox.up);
		}
		*tn = &tn_RTBox;
	}
	return tn_RTBox.name;
}

//done with class RTBox : RContainer

//--TN--class RSBox : RTBox
SiSlTN tn_RSBox = { 0, "RSBox", 0, 0 };
const char *RSBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RSBox.up) {
			RTBox::GetTypeName(&tn_RSBox.up);
		}
		*tn = &tn_RSBox;
	}
	return tn_RSBox.name;
}

//done with class RSBox : RTBox

//---class RScrollView : RHVBox
/*---Signals---
Si_ScrolledX(int)
Si_ScrolledY(int)
Si_AdjustX(int,int)
Si_AdjustY(int,int)
//---Slots-----
Sl_ScrollX(int)
Sl_ScrollY(int)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RScrollView_Si_ScrolledX_i(void *ths, void **ppv)	{ ((RScrollView*)ths)->Si_ScrolledX((int)(uptr)ppv[0]); }
void RScrollView_Si_ScrolledY_i(void *ths, void **ppv)	{ ((RScrollView*)ths)->Si_ScrolledY((int)(uptr)ppv[0]); }
void RScrollView_Si_AdjustX_i_i(void *ths, void **ppv)	{ ((RScrollView*)ths)->Si_AdjustX((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RScrollView_Si_AdjustY_i_i(void *ths, void **ppv)	{ ((RScrollView*)ths)->Si_AdjustY((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RScrollView_Sl_ScrollX_i(void *ths, void **ppv)	{ ((RScrollView*)ths)->Sl_ScrollX((int)(uptr)ppv[0]); }
void RScrollView_Sl_ScrollY_i(void *ths, void **ppv)	{ ((RScrollView*)ths)->Sl_ScrollY((int)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RScrollView::*si_RScrollView[4] =
{
	&RScrollView::CSiSl_Si_ScrolledX,
	&RScrollView::CSiSl_Si_ScrolledY,
	&RScrollView::CSiSl_Si_AdjustX,
	&RScrollView::CSiSl_Si_AdjustY,
};
static union { CSiSl RScrollView::**d; CSiSl CConn::**b; } upp_RScrollView = { si_RScrollView };

SiSlLink sil_RScrollView[7] =
{
	{ "Si_ScrolledX(int)", RScrollView_Si_ScrolledX_i },
	{ "Si_ScrolledY(int)", RScrollView_Si_ScrolledY_i },
	{ "Si_AdjustX(int,int)", RScrollView_Si_AdjustX_i_i },
	{ "Si_AdjustY(int,int)", RScrollView_Si_AdjustY_i_i },
	{ "Sl_ScrollX(int)", RScrollView_Sl_ScrollX_i },
	{ "Sl_ScrollY(int)", RScrollView_Sl_ScrollY_i },
	{ 0,0 }
};

SiSlData sd_RScrollView = { 0, "RScrollView", 4, upp_RScrollView.b, sil_RScrollView };

SiSlData *RScrollView::SiSl_GetSD()
{
	if(!sd_RScrollView.up) {
		sd_RScrollView.up = RHVBox::SiSl_GetSD();
	}
	return &sd_RScrollView;
}

SiSlTN tn_RScrollView = { 0, "RScrollView", 4, 2 };
const char *RScrollView::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RScrollView.up) {
			RHVBox::GetTypeName(&tn_RScrollView.up);
		}
		*tn = &tn_RScrollView;
	}
	return tn_RScrollView.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RScrollView::Si_ScrolledX(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_ScrolledX,pv);
}

void RScrollView::Si_ScrolledY(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_ScrolledY,pv);
}

void RScrollView::Si_AdjustX(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustX,pv);
}

void RScrollView::Si_AdjustY(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustY,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RScrollView : RHVBox

//--TN--class RRadioBox : RTBox
SiSlTN tn_RRadioBox = { 0, "RRadioBox", 0, 0 };
const char *RRadioBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RRadioBox.up) {
			RTBox::GetTypeName(&tn_RRadioBox.up);
		}
		*tn = &tn_RRadioBox;
	}
	return tn_RRadioBox.name;
}

//done with class RRadioBox : RTBox

//--TN--class RTabControl : RContainer
SiSlTN tn_RTabControl = { 0, "RTabControl", 0, 0 };
const char *RTabControl::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RTabControl.up) {
			RContainer::GetTypeName(&tn_RTabControl.up);
		}
		*tn = &tn_RTabControl;
	}
	return tn_RTabControl.name;
}

//done with class RTabControl : RContainer

//---class RButton : RControl
/*---Signals---
Si_Clicked(int)
//---Slots-----
Sl_Clicked(int)
Sl_Clicked()
//-----------*/
//------------------------------
//---Start of connector stubs---

void RButton_Si_Clicked_i(void *ths, void **ppv)	{ ((RButton*)ths)->Si_Clicked((int)(uptr)ppv[0]); }
void RButton_Sl_Clicked_i(void *ths, void **ppv)	{ ((RButton*)ths)->Sl_Clicked((int)(uptr)ppv[0]); }
void RButton_Sl_Clicked(void *ths, void **ppv)	{ ((RButton*)ths)->Sl_Clicked(); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RButton::*si_RButton[1] =
{
	&RButton::CSiSl_Si_Clicked,
};
static union { CSiSl RButton::**d; CSiSl CConn::**b; } upp_RButton = { si_RButton };

SiSlLink sil_RButton[4] =
{
	{ "Si_Clicked(int)", RButton_Si_Clicked_i },
	{ "Sl_Clicked(int)", RButton_Sl_Clicked_i },
	{ "Sl_Clicked()", RButton_Sl_Clicked },
	{ 0,0 }
};

SiSlData sd_RButton = { 0, "RButton", 1, upp_RButton.b, sil_RButton };

SiSlData *RButton::SiSl_GetSD()
{
	if(!sd_RButton.up) {
		sd_RButton.up = RControl::SiSl_GetSD();
	}
	return &sd_RButton;
}

SiSlTN tn_RButton = { 0, "RButton", 1, 2 };
const char *RButton::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RButton.up) {
			RControl::GetTypeName(&tn_RButton.up);
		}
		*tn = &tn_RButton;
	}
	return tn_RButton.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RButton::Si_Clicked(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Clicked,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RButton : RControl

//--TN--class RLabel : RButton
SiSlTN tn_RLabel = { 0, "RLabel", 0, 0 };
const char *RLabel::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RLabel.up) {
			RButton::GetTypeName(&tn_RLabel.up);
		}
		*tn = &tn_RLabel;
	}
	return tn_RLabel.name;
}

//done with class RLabel : RButton

//---class RCheckBox : RControl
/*---Signals---
Si_Checkd(bool)
Si_Checked()
Si_UnChecked()
//---Slots-----
Sl_Check()
Sl_UnCheck()
//-----------*/
//------------------------------
//---Start of connector stubs---

void RCheckBox_Si_Checkd_b(void *ths, void **ppv)	{ ((RCheckBox*)ths)->Si_Checkd((bool)(uptr)ppv[0]); }
void RCheckBox_Si_Checked(void *ths, void **ppv)	{ ((RCheckBox*)ths)->Si_Checked(); }
void RCheckBox_Si_UnChecked(void *ths, void **ppv)	{ ((RCheckBox*)ths)->Si_UnChecked(); }
void RCheckBox_Sl_Check(void *ths, void **ppv)	{ ((RCheckBox*)ths)->Sl_Check(); }
void RCheckBox_Sl_UnCheck(void *ths, void **ppv)	{ ((RCheckBox*)ths)->Sl_UnCheck(); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RCheckBox::*si_RCheckBox[3] =
{
	&RCheckBox::CSiSl_Si_Checkd,
	&RCheckBox::CSiSl_Si_Checked,
	&RCheckBox::CSiSl_Si_UnChecked,
};
static union { CSiSl RCheckBox::**d; CSiSl CConn::**b; } upp_RCheckBox = { si_RCheckBox };

SiSlLink sil_RCheckBox[6] =
{
	{ "Si_Checkd(bool)", RCheckBox_Si_Checkd_b },
	{ "Si_Checked()", RCheckBox_Si_Checked },
	{ "Si_UnChecked()", RCheckBox_Si_UnChecked },
	{ "Sl_Check()", RCheckBox_Sl_Check },
	{ "Sl_UnCheck()", RCheckBox_Sl_UnCheck },
	{ 0,0 }
};

SiSlData sd_RCheckBox = { 0, "RCheckBox", 3, upp_RCheckBox.b, sil_RCheckBox };

SiSlData *RCheckBox::SiSl_GetSD()
{
	if(!sd_RCheckBox.up) {
		sd_RCheckBox.up = RControl::SiSl_GetSD();
	}
	return &sd_RCheckBox;
}

SiSlTN tn_RCheckBox = { 0, "RCheckBox", 3, 2 };
const char *RCheckBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RCheckBox.up) {
			RControl::GetTypeName(&tn_RCheckBox.up);
		}
		*tn = &tn_RCheckBox;
	}
	return tn_RCheckBox.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RCheckBox::Si_Checkd(bool a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Checkd,pv);
}

void RCheckBox::Si_Checked()
{
	void *pv[1] = { 0 };
	SendSignal(CSiSl_Si_Checked,pv);
}

void RCheckBox::Si_UnChecked()
{
	void *pv[1] = { 0 };
	SendSignal(CSiSl_Si_UnChecked,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RCheckBox : RControl

//--TN--class RRadioButton : RCheckBox
SiSlTN tn_RRadioButton = { 0, "RRadioButton", 0, 0 };
const char *RRadioButton::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RRadioButton.up) {
			RCheckBox::GetTypeName(&tn_RRadioButton.up);
		}
		*tn = &tn_RRadioButton;
	}
	return tn_RRadioButton.name;
}

//done with class RRadioButton : RCheckBox

//--TN--class RToggleButton : RCheckBox
SiSlTN tn_RToggleButton = { 0, "RToggleButton", 0, 0 };
const char *RToggleButton::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RToggleButton.up) {
			RCheckBox::GetTypeName(&tn_RToggleButton.up);
		}
		*tn = &tn_RToggleButton;
	}
	return tn_RToggleButton.name;
}

//done with class RToggleButton : RCheckBox

//---class RRange : RControl
/*---Signals---
Si_Moved(int)
//---Slots-----
//-----------*/
//------------------------------
//---Start of connector stubs---

void RRange_Si_Moved_i(void *ths, void **ppv)	{ ((RRange*)ths)->Si_Moved((int)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RRange::*si_RRange[1] =
{
	&RRange::CSiSl_Si_Moved,
};
static union { CSiSl RRange::**d; CSiSl CConn::**b; } upp_RRange = { si_RRange };

SiSlLink sil_RRange[2] =
{
	{ "Si_Moved(int)", RRange_Si_Moved_i },
	{ 0,0 }
};

SiSlData sd_RRange = { 0, "RRange", 1, upp_RRange.b, sil_RRange };

SiSlData *RRange::SiSl_GetSD()
{
	if(!sd_RRange.up) {
		sd_RRange.up = RControl::SiSl_GetSD();
	}
	return &sd_RRange;
}

SiSlTN tn_RRange = { 0, "RRange", 1, 0 };
const char *RRange::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RRange.up) {
			RControl::GetTypeName(&tn_RRange.up);
		}
		*tn = &tn_RRange;
	}
	return tn_RRange.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RRange::Si_Moved(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Moved,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RRange : RControl

//---class RScrollBar : RControl
/*---Signals---
Si_Scrolled(int)
//---Slots-----
Sl_Scroll(int)
Sl_Adjust(int,int)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RScrollBar_Si_Scrolled_i(void *ths, void **ppv)	{ ((RScrollBar*)ths)->Si_Scrolled((int)(uptr)ppv[0]); }
void RScrollBar_Sl_Scroll_i(void *ths, void **ppv)	{ ((RScrollBar*)ths)->Sl_Scroll((int)(uptr)ppv[0]); }
void RScrollBar_Sl_Adjust_i_i(void *ths, void **ppv)	{ ((RScrollBar*)ths)->Sl_Adjust((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RScrollBar::*si_RScrollBar[1] =
{
	&RScrollBar::CSiSl_Si_Scrolled,
};
static union { CSiSl RScrollBar::**d; CSiSl CConn::**b; } upp_RScrollBar = { si_RScrollBar };

SiSlLink sil_RScrollBar[4] =
{
	{ "Si_Scrolled(int)", RScrollBar_Si_Scrolled_i },
	{ "Sl_Scroll(int)", RScrollBar_Sl_Scroll_i },
	{ "Sl_Adjust(int,int)", RScrollBar_Sl_Adjust_i_i },
	{ 0,0 }
};

SiSlData sd_RScrollBar = { 0, "RScrollBar", 1, upp_RScrollBar.b, sil_RScrollBar };

SiSlData *RScrollBar::SiSl_GetSD()
{
	if(!sd_RScrollBar.up) {
		sd_RScrollBar.up = RControl::SiSl_GetSD();
	}
	return &sd_RScrollBar;
}

SiSlTN tn_RScrollBar = { 0, "RScrollBar", 1, 2 };
const char *RScrollBar::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RScrollBar.up) {
			RControl::GetTypeName(&tn_RScrollBar.up);
		}
		*tn = &tn_RScrollBar;
	}
	return tn_RScrollBar.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RScrollBar::Si_Scrolled(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Scrolled,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RScrollBar : RControl

//---class RProgressBar : RControl
/*---Signals---
//---Slots-----
Sl_Move(int)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RProgressBar_Sl_Move_i(void *ths, void **ppv)	{ ((RProgressBar*)ths)->Sl_Move((int)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

SiSlLink sil_RProgressBar[2] =
{
	{ "Sl_Move(int)", RProgressBar_Sl_Move_i },
	{ 0,0 }
};

SiSlData sd_RProgressBar = { 0, "RProgressBar", 0, 0, sil_RProgressBar };

SiSlData *RProgressBar::SiSl_GetSD()
{
	if(!sd_RProgressBar.up) {
		sd_RProgressBar.up = RControl::SiSl_GetSD();
	}
	return &sd_RProgressBar;
}

SiSlTN tn_RProgressBar = { 0, "RProgressBar", 0, 1 };
const char *RProgressBar::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RProgressBar.up) {
			RControl::GetTypeName(&tn_RProgressBar.up);
		}
		*tn = &tn_RProgressBar;
	}
	return tn_RProgressBar.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

//-----End of signal stubs------
//------------------------------
//done with class RProgressBar : RControl

//--TN--class RStatusBar : RControl
SiSlTN tn_RStatusBar = { 0, "RStatusBar", 0, 0 };
const char *RStatusBar::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RStatusBar.up) {
			RControl::GetTypeName(&tn_RStatusBar.up);
		}
		*tn = &tn_RStatusBar;
	}
	return tn_RStatusBar.name;
}

//done with class RStatusBar : RControl

// vim:ts=4 sts=0 sw=4
//Automatically generated from: window.h
//Don't edit, all changes _will be lost_.

//--TN--class RWindow : RHVBox
SiSlTN tn_RWindow = { 0, "RWindow", 0, 0 };
const char *RWindow::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RWindow.up) {
			RHVBox::GetTypeName(&tn_RWindow.up);
		}
		*tn = &tn_RWindow;
	}
	return tn_RWindow.name;
}

//done with class RWindow : RHVBox

//--TN--class RMainWindow : RWindow
SiSlTN tn_RMainWindow = { 0, "RMainWindow", 0, 0 };
const char *RMainWindow::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RMainWindow.up) {
			RWindow::GetTypeName(&tn_RMainWindow.up);
		}
		*tn = &tn_RMainWindow;
	}
	return tn_RMainWindow.name;
}

//done with class RMainWindow : RWindow

//--TN--class RPopup : RWindow
SiSlTN tn_RPopup = { 0, "RPopup", 0, 0 };
const char *RPopup::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RPopup.up) {
			RWindow::GetTypeName(&tn_RPopup.up);
		}
		*tn = &tn_RPopup;
	}
	return tn_RPopup.name;
}

//done with class RPopup : RWindow

//--TN--class RToolTip : RPopup
SiSlTN tn_RToolTip = { 0, "RToolTip", 0, 0 };
const char *RToolTip::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RToolTip.up) {
			RPopup::GetTypeName(&tn_RToolTip.up);
		}
		*tn = &tn_RToolTip;
	}
	return tn_RToolTip.name;
}

//done with class RToolTip : RPopup

//---class RApp : CConn
/*---Signals---
//---Slots-----
Sl_Quit()
Sl_ShowIt()
//-----------*/
//------------------------------
//---Start of connector stubs---

void RApp_Sl_Quit(void *ths, void **ppv)	{ ((RApp*)ths)->Sl_Quit(); }
void RApp_Sl_ShowIt(void *ths, void **ppv)	{ ((RApp*)ths)->Sl_ShowIt(); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

SiSlLink sil_RApp[3] =
{
	{ "Sl_Quit()", RApp_Sl_Quit },
	{ "Sl_ShowIt()", RApp_Sl_ShowIt },
	{ 0,0 }
};

SiSlData sd_RApp = { 0, "RApp", 0, 0, sil_RApp };

SiSlData *RApp::SiSl_GetSD()
{
	if(!sd_RApp.up) {
		sd_RApp.up = CConn::SiSl_GetSD();
	}
	return &sd_RApp;
}

SiSlTN tn_RApp = { 0, "RApp", 0, 2 };
const char *RApp::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RApp.up) {
			CConn::GetTypeName(&tn_RApp.up);
		}
		*tn = &tn_RApp;
	}
	return tn_RApp.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

//-----End of signal stubs------
//------------------------------
//done with class RApp : CConn

// vim:ts=4 sts=0 sw=4
//Automatically generated from: RTextEdit.h
//Don't edit, all changes _will be lost_.

#include "RTextEdit.h"

//done with class STextDummy : STextLine

//---class RTextEdit : RControl
/*---Signals---
Si_ScrolledX(int)
Si_ScrolledY(int)
Si_AdjustX(int,int)
Si_AdjustY(int,int)
//---Slots-----
Sl_ScrollX(int)
Sl_ScrollY(int)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RTextEdit_Si_ScrolledX_i(void *ths, void **ppv)	{ ((RTextEdit*)ths)->Si_ScrolledX((int)(uptr)ppv[0]); }
void RTextEdit_Si_ScrolledY_i(void *ths, void **ppv)	{ ((RTextEdit*)ths)->Si_ScrolledY((int)(uptr)ppv[0]); }
void RTextEdit_Si_AdjustX_i_i(void *ths, void **ppv)	{ ((RTextEdit*)ths)->Si_AdjustX((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RTextEdit_Si_AdjustY_i_i(void *ths, void **ppv)	{ ((RTextEdit*)ths)->Si_AdjustY((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RTextEdit_Sl_ScrollX_i(void *ths, void **ppv)	{ ((RTextEdit*)ths)->Sl_ScrollX((int)(uptr)ppv[0]); }
void RTextEdit_Sl_ScrollY_i(void *ths, void **ppv)	{ ((RTextEdit*)ths)->Sl_ScrollY((int)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RTextEdit::*si_RTextEdit[4] =
{
	&RTextEdit::CSiSl_Si_ScrolledX,
	&RTextEdit::CSiSl_Si_ScrolledY,
	&RTextEdit::CSiSl_Si_AdjustX,
	&RTextEdit::CSiSl_Si_AdjustY,
};
static union { CSiSl RTextEdit::**d; CSiSl CConn::**b; } upp_RTextEdit = { si_RTextEdit };

SiSlLink sil_RTextEdit[7] =
{
	{ "Si_ScrolledX(int)", RTextEdit_Si_ScrolledX_i },
	{ "Si_ScrolledY(int)", RTextEdit_Si_ScrolledY_i },
	{ "Si_AdjustX(int,int)", RTextEdit_Si_AdjustX_i_i },
	{ "Si_AdjustY(int,int)", RTextEdit_Si_AdjustY_i_i },
	{ "Sl_ScrollX(int)", RTextEdit_Sl_ScrollX_i },
	{ "Sl_ScrollY(int)", RTextEdit_Sl_ScrollY_i },
	{ 0,0 }
};

SiSlData sd_RTextEdit = { 0, "RTextEdit", 4, upp_RTextEdit.b, sil_RTextEdit };

SiSlData *RTextEdit::SiSl_GetSD()
{
	if(!sd_RTextEdit.up) {
		sd_RTextEdit.up = RControl::SiSl_GetSD();
	}
	return &sd_RTextEdit;
}

SiSlTN tn_RTextEdit = { 0, "RTextEdit", 4, 2 };
const char *RTextEdit::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RTextEdit.up) {
			RControl::GetTypeName(&tn_RTextEdit.up);
		}
		*tn = &tn_RTextEdit;
	}
	return tn_RTextEdit.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RTextEdit::Si_ScrolledX(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_ScrolledX,pv);
}

void RTextEdit::Si_ScrolledY(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_ScrolledY,pv);
}

void RTextEdit::Si_AdjustX(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustX,pv);
}

void RTextEdit::Si_AdjustY(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustY,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RTextEdit : RControl

//--TN--class REditBox : RControl
SiSlTN tn_REditBox = { 0, "REditBox", 0, 0 };
const char *REditBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_REditBox.up) {
			RControl::GetTypeName(&tn_REditBox.up);
		}
		*tn = &tn_REditBox;
	}
	return tn_REditBox.name;
}

//done with class REditBox : RControl

//--TN--class RHexBox : REditBox
SiSlTN tn_RHexBox = { 0, "RHexBox", 0, 0 };
const char *RHexBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RHexBox.up) {
			REditBox::GetTypeName(&tn_RHexBox.up);
		}
		*tn = &tn_RHexBox;
	}
	return tn_RHexBox.name;
}

//done with class RHexBox : REditBox

// vim:ts=4 sts=0 sw=4
//Automatically generated from: RListView.h
//Don't edit, all changes _will be lost_.

#include "RListView.h"

//---class RListView : RControl
/*---Signals---
Si_ScrolledY(int)
Si_AdjustX(int,int)
Si_AdjustY(int,int)
Si_Selected(SListNode*)
//---Slots-----
Sl_ScrollY(int)
Sl_Setup(char*)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RListView_Si_ScrolledY_i(void *ths, void **ppv)	{ ((RListView*)ths)->Si_ScrolledY((int)(uptr)ppv[0]); }
void RListView_Si_AdjustX_i_i(void *ths, void **ppv)	{ ((RListView*)ths)->Si_AdjustX((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RListView_Si_AdjustY_i_i(void *ths, void **ppv)	{ ((RListView*)ths)->Si_AdjustY((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RListView_Si_Selected_S(void *ths, void **ppv)	{ ((RListView*)ths)->Si_Selected((SListNode*)(uptr)ppv[0]); }
void RListView_Sl_ScrollY_i(void *ths, void **ppv)	{ ((RListView*)ths)->Sl_ScrollY((int)(uptr)ppv[0]); }
void RListView_Sl_Setup_c(void *ths, void **ppv)	{ ((RListView*)ths)->Sl_Setup((char*)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RListView::*si_RListView[4] =
{
	&RListView::CSiSl_Si_ScrolledY,
	&RListView::CSiSl_Si_AdjustX,
	&RListView::CSiSl_Si_AdjustY,
	&RListView::CSiSl_Si_Selected,
};
static union { CSiSl RListView::**d; CSiSl CConn::**b; } upp_RListView = { si_RListView };

SiSlLink sil_RListView[7] =
{
	{ "Si_ScrolledY(int)", RListView_Si_ScrolledY_i },
	{ "Si_AdjustX(int,int)", RListView_Si_AdjustX_i_i },
	{ "Si_AdjustY(int,int)", RListView_Si_AdjustY_i_i },
	{ "Si_Selected(SListNode*)", RListView_Si_Selected_S },
	{ "Sl_ScrollY(int)", RListView_Sl_ScrollY_i },
	{ "Sl_Setup(char*)", RListView_Sl_Setup_c },
	{ 0,0 }
};

SiSlData sd_RListView = { 0, "RListView", 4, upp_RListView.b, sil_RListView };

SiSlData *RListView::SiSl_GetSD()
{
	if(!sd_RListView.up) {
		sd_RListView.up = RControl::SiSl_GetSD();
	}
	return &sd_RListView;
}

SiSlTN tn_RListView = { 0, "RListView", 4, 2 };
const char *RListView::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RListView.up) {
			RControl::GetTypeName(&tn_RListView.up);
		}
		*tn = &tn_RListView;
	}
	return tn_RListView.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RListView::Si_ScrolledY(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_ScrolledY,pv);
}

void RListView::Si_AdjustX(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustX,pv);
}

void RListView::Si_AdjustY(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustY,pv);
}

void RListView::Si_Selected(SListNode* a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Selected,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RListView : RControl

//done with class RListViewDir : RListView

//--TN--class RListViewPopup : RListView
SiSlTN tn_RListViewPopup = { 0, "RListViewPopup", 0, 0 };
const char *RListViewPopup::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RListViewPopup.up) {
			RListView::GetTypeName(&tn_RListViewPopup.up);
		}
		*tn = &tn_RListViewPopup;
	}
	return tn_RListViewPopup.name;
}

//done with class RListViewPopup : RListView

//--TN--class RPopupList : RPopup
SiSlTN tn_RPopupList = { 0, "RPopupList", 0, 0 };
const char *RPopupList::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RPopupList.up) {
			RPopup::GetTypeName(&tn_RPopupList.up);
		}
		*tn = &tn_RPopupList;
	}
	return tn_RPopupList.name;
}

//done with class RPopupList : RPopup

//---class RComboBox : RButton
/*---Signals---
Si_IndexChanged(int,void*)
//---Slots-----
Sl_Update(SListNode*)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RComboBox_Si_IndexChanged_i_v(void *ths, void **ppv)	{ ((RComboBox*)ths)->Si_IndexChanged((int)(uptr)ppv[0],(void*)(uptr)ppv[1]); }
void RComboBox_Sl_Update_S(void *ths, void **ppv)	{ ((RComboBox*)ths)->Sl_Update((SListNode*)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RComboBox::*si_RComboBox[1] =
{
	&RComboBox::CSiSl_Si_IndexChanged,
};
static union { CSiSl RComboBox::**d; CSiSl CConn::**b; } upp_RComboBox = { si_RComboBox };

SiSlLink sil_RComboBox[3] =
{
	{ "Si_IndexChanged(int,void*)", RComboBox_Si_IndexChanged_i_v },
	{ "Sl_Update(SListNode*)", RComboBox_Sl_Update_S },
	{ 0,0 }
};

SiSlData sd_RComboBox = { 0, "RComboBox", 1, upp_RComboBox.b, sil_RComboBox };

SiSlData *RComboBox::SiSl_GetSD()
{
	if(!sd_RComboBox.up) {
		sd_RComboBox.up = RButton::SiSl_GetSD();
	}
	return &sd_RComboBox;
}

SiSlTN tn_RComboBox = { 0, "RComboBox", 1, 1 };
const char *RComboBox::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RComboBox.up) {
			RButton::GetTypeName(&tn_RComboBox.up);
		}
		*tn = &tn_RComboBox;
	}
	return tn_RComboBox.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RComboBox::Si_IndexChanged(int a, void* b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_IndexChanged,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RComboBox : RButton

// vim:ts=4 sts=0 sw=4
//Automatically generated from: RTreeView.h
//Don't edit, all changes _will be lost_.

#include "RTreeView.h"

//done with class RTreeNodeDir : RTreeNode

//---class RTreeView : RControl
/*---Signals---
Si_ScrolledY(int)
Si_AdjustX(int,int)
Si_AdjustY(int,int)
Si_Selected(RTreeNode*)
//---Slots-----
Sl_ScrollY(int)
Sl_Setup(char*)
//-----------*/
//------------------------------
//---Start of connector stubs---

void RTreeView_Si_ScrolledY_i(void *ths, void **ppv)	{ ((RTreeView*)ths)->Si_ScrolledY((int)(uptr)ppv[0]); }
void RTreeView_Si_AdjustX_i_i(void *ths, void **ppv)	{ ((RTreeView*)ths)->Si_AdjustX((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RTreeView_Si_AdjustY_i_i(void *ths, void **ppv)	{ ((RTreeView*)ths)->Si_AdjustY((int)(uptr)ppv[0],(int)(uptr)ppv[1]); }
void RTreeView_Si_Selected_R(void *ths, void **ppv)	{ ((RTreeView*)ths)->Si_Selected((RTreeNode*)(uptr)ppv[0]); }
void RTreeView_Sl_ScrollY_i(void *ths, void **ppv)	{ ((RTreeView*)ths)->Sl_ScrollY((int)(uptr)ppv[0]); }
void RTreeView_Sl_Setup_c(void *ths, void **ppv)	{ ((RTreeView*)ths)->Sl_Setup((char*)(uptr)ppv[0]); }

//---End of connector stubs---
//----------------------------
//---Start of SiSl Data-------

CSiSl RTreeView::*si_RTreeView[4] =
{
	&RTreeView::CSiSl_Si_ScrolledY,
	&RTreeView::CSiSl_Si_AdjustX,
	&RTreeView::CSiSl_Si_AdjustY,
	&RTreeView::CSiSl_Si_Selected,
};
static union { CSiSl RTreeView::**d; CSiSl CConn::**b; } upp_RTreeView = { si_RTreeView };

SiSlLink sil_RTreeView[7] =
{
	{ "Si_ScrolledY(int)", RTreeView_Si_ScrolledY_i },
	{ "Si_AdjustX(int,int)", RTreeView_Si_AdjustX_i_i },
	{ "Si_AdjustY(int,int)", RTreeView_Si_AdjustY_i_i },
	{ "Si_Selected(RTreeNode*)", RTreeView_Si_Selected_R },
	{ "Sl_ScrollY(int)", RTreeView_Sl_ScrollY_i },
	{ "Sl_Setup(char*)", RTreeView_Sl_Setup_c },
	{ 0,0 }
};

SiSlData sd_RTreeView = { 0, "RTreeView", 4, upp_RTreeView.b, sil_RTreeView };

SiSlData *RTreeView::SiSl_GetSD()
{
	if(!sd_RTreeView.up) {
		sd_RTreeView.up = RControl::SiSl_GetSD();
	}
	return &sd_RTreeView;
}

SiSlTN tn_RTreeView = { 0, "RTreeView", 4, 2 };
const char *RTreeView::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RTreeView.up) {
			RControl::GetTypeName(&tn_RTreeView.up);
		}
		*tn = &tn_RTreeView;
	}
	return tn_RTreeView.name;
}

//---End of SiSl Data-----------
//------------------------------
//----Start of signal stubs-----

void RTreeView::Si_ScrolledY(int a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_ScrolledY,pv);
}

void RTreeView::Si_AdjustX(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustX,pv);
}

void RTreeView::Si_AdjustY(int a, int b)
{
	void *pv[2];
	pv[0]=(void*)(uptr)a;
	pv[1]=(void*)(uptr)b;
	SendSignal(CSiSl_Si_AdjustY,pv);
}

void RTreeView::Si_Selected(RTreeNode* a)
{
	void *pv[1];
	pv[0]=(void*)(uptr)a;
	SendSignal(CSiSl_Si_Selected,pv);
}

//-----End of signal stubs------
//------------------------------
//done with class RTreeView : RControl

// vim:ts=4 sts=0 sw=4
//Automatically generated from: RMenu.h
//Don't edit, all changes _will be lost_.

#include "RMenu.h"

//--TN--class RMenuButton : RButton
SiSlTN tn_RMenuButton = { 0, "RMenuButton", 0, 0 };
const char *RMenuButton::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RMenuButton.up) {
			RButton::GetTypeName(&tn_RMenuButton.up);
		}
		*tn = &tn_RMenuButton;
	}
	return tn_RMenuButton.name;
}

//done with class RMenuButton : RButton

//--TN--class RMenuBar : RHVBox
SiSlTN tn_RMenuBar = { 0, "RMenuBar", 0, 0 };
const char *RMenuBar::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RMenuBar.up) {
			RHVBox::GetTypeName(&tn_RMenuBar.up);
		}
		*tn = &tn_RMenuBar;
	}
	return tn_RMenuBar.name;
}

//done with class RMenuBar : RHVBox

//--TN--class RMenuBarButton : RMenuButton
SiSlTN tn_RMenuBarButton = { 0, "RMenuBarButton", 0, 0 };
const char *RMenuBarButton::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RMenuBarButton.up) {
			RMenuButton::GetTypeName(&tn_RMenuBarButton.up);
		}
		*tn = &tn_RMenuBarButton;
	}
	return tn_RMenuBarButton.name;
}

//done with class RMenuBarButton : RMenuButton

//--TN--class RPopupMenu : RPopup
SiSlTN tn_RPopupMenu = { 0, "RPopupMenu", 0, 0 };
const char *RPopupMenu::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		if(!tn_RPopupMenu.up) {
			RPopup::GetTypeName(&tn_RPopupMenu.up);
		}
		*tn = &tn_RPopupMenu;
	}
	return tn_RPopupMenu.name;
}

//done with class RPopupMenu : RPopup

