// vim:ts=4 sts=0 sw=4

#ifdef WIN32
// copied from winresrc.h (or winuser.h)
// I don't want to include windows.h in Rtk headers...
// If this ever (??!!) change use rkeygen.h

#define RK_BACK		0x08 //VK_BACK
#define RK_TAB		0x09 //VK_TAB
#define RK_RETURN	0x0D //VK_RETURN /*kp*/
#define RK_SHIFT	0x10 //VK_SHIFT  /*lr*/
#define RK_CTRL		0x11 //VK_CONTROL/*lr*/
#define RK_ALT		0x12 //VK_MENU   /*lr*/
#define RK_PAUSE	0x13 //VK_PAUSE
#define RK_ESCAPE	0x1B //VK_ESCAPE
#define RK_SPACE	0x20 //VK_SPACE

#define RK_PGUP		0x21 //VK_PRIOR /*kp*/
#define RK_PGDN		0x22 //VK_NEXT  /*kp*/
#define RK_HOME		0x24 //VK_HOME  /*kp*/
#define RK_END		0x23 //VK_END   /*kp*/
#define RK_LEFT		0x25 //VK_LEFT  /*kp*/
#define RK_RIGHT	0x27 //VK_RIGHT /*kp*/
#define RK_UP		0x26 //VK_UP    /*kp*/
#define RK_DOWN		0x28 //VK_DOWN  /*kp*/
#define RK_SELECT	0x29 //VK_SELECT
#define RK_INSERT	0x2D //VK_INSERT/*kp*/
#define RK_DELETE	0x2E //VK_DELETE/*kp*/

#define RK_LWIN		0x5B //VK_LWIN
#define RK_RWIN		0x5C //VK_RWIN
#define RK_APPS		0x5D //VK_APPS

#define RK_KP0		0x60 //VK_NUMPAD0
#define RK_KP1		0x61 //VK_NUMPAD1
#define RK_KP2		0x62 //VK_NUMPAD2
#define RK_KP3		0x63 //VK_NUMPAD3
#define RK_KP4		0x64 //VK_NUMPAD4
#define RK_KP5		0x65 //VK_NUMPAD5
#define RK_KP6		0x66 //VK_NUMPAD6
#define RK_KP7		0x67 //VK_NUMPAD7
#define RK_KP8		0x68 //VK_NUMPAD8
#define RK_KP9		0x69 //VK_NUMPAD9

#define RK_KPMUL	0x6A //VK_MULTIPLY
#define RK_KPADD	0x6B //VK_ADD
#define RK_KPSUB	0x6D //VK_SUBTRACT
#define RK_KPDEC	0x6E //VK_DECIMAL
#define RK_KPDIV	0x6F //VK_DIVIDE

#define RK_F1		0x70 //VK_F1
#define RK_F2		0x71 //VK_F2
#define RK_F3		0x72 //VK_F3
#define RK_F4		0x73 //VK_F4
#define RK_F5		0x74 //VK_F5
#define RK_F6		0x75 //VK_F6
#define RK_F7		0x76 //VK_F7
#define RK_F8		0x77 //VK_F8
#define RK_F9		0x78 //VK_F9
#define RK_F10		0x79 //VK_F10
#define RK_F11		0x7A //VK_F11
#define RK_F12		0x7B //VK_F12

#define RK_NUMLOCK	0x90 //VK_NUMLOCK
#define RK_CAPSLOCK	0x14 //VK_CAPITAL
#define RK_SCRLOCK	0x91 //VK_SCROLL

#elif defined XLIB //=========================================

#include <X11/keysym.h>

#define RK_BACK		XK_BackSpace
#define RK_TAB		XK_Tab
#define RK_RETURN	XK_Return	/*kp XK_KP_Enter*/
#define RK_SHIFT	XK_Shift_L	/*lr XK_Shift_R*/
#define RK_CTRL		XK_Control_L/*lr XK_Control_R*/
#define RK_ALT		XK_Alt_L	/*lr XK_Alt_R*/
#define RK_PAUSE	XK_Pause
#define RK_ESCAPE	XK_Escape
#define RK_SPACE	XK_space
//#define RK_SPACE	XK_KP_Space

#define RK_PGUP		XK_Page_Up	/*kp XK_KP_Page_Up*/
#define RK_PGDN		XK_Page_Down/*kp XK_KP_Page_Down*/
#define RK_HOME		XK_Home		/*kp XK_KP_Home*/
#define RK_END		XK_End		/*kp XK_KP_End*/
#define RK_LEFT		XK_Left		/*kp XK_KP_Left*/
#define RK_RIGHT	XK_Right	/*kp XK_KP_Right*/
#define RK_UP		XK_Up		/*kp XK_KP_Up*/
#define RK_DOWN		XK_Down		/*kp XK_KP_Down*/
#define RK_SELECT	XK_Select
#define RK_INSERT	XK_Insert	/*kp XK_KP_Insert*/
#define RK_DELETE	XK_Delete	/*kp XK_KP_Delete*/

#define RK_LWIN		XK_Super_L
#define RK_RWIN		XK_Super_R
#define RK_APPS		XK_Menu

#define RK_KP0		XK_KP_0
#define RK_KP1		XK_KP_1
#define RK_KP2		XK_KP_2
#define RK_KP3		XK_KP_3
#define RK_KP4		XK_KP_4
#define RK_KP5		XK_KP_5
#define RK_KP6		XK_KP_6
#define RK_KP7		XK_KP_7
#define RK_KP8		XK_KP_8
#define RK_KP9		XK_KP_9

#define RK_KPMUL	XK_KP_Multiply
#define RK_KPADD	XK_KP_Add
#define RK_KPSUB	XK_KP_Subtract
#define RK_KPDEC	XK_KP_Decimal
#define RK_KPDIV	XK_KP_Divide

#define RK_F1		XK_F1
#define RK_F2		XK_F2
#define RK_F3		XK_F3
#define RK_F4		XK_F4
#define RK_F5		XK_F5
#define RK_F6		XK_F6
#define RK_F7		XK_F7
#define RK_F8		XK_F8
#define RK_F9		XK_F9
#define RK_F10		XK_F10
#define RK_F11		XK_F11
#define RK_F12		XK_F12

#define RK_NUMLOCK	XK_Num_Lock
#define RK_CAPSLOCK	XK_Caps_Lock
#define RK_SCRLOCK	XK_Scroll_Lock

#else
#error No OS interface defined...
#endif

#if 0 // keys not yet defined in Rtk

#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_MBUTTON        0x04
VK_0 - VK_9 = '0-9' (0x30 - 0x39)
VK_A - VK_Z = 'A-Z' (0x41 - 0x5A)

#define XK_Linefeed		0xFF0A	/* Linefeed, LF */
#define XK_Clear		0xFF0B
#define VK_CLEAR          0x0C ???

#define VK_CANCEL       0x03 // ctrl-break
#define XK_Print		0xFF61
#define VK_PRINT          0x2A ???
#define XK_Execute		0xFF62	/* Execute, run, do */
#define VK_EXECUTE        0x2B ???
#define XK_Sys_Req		0xFF15 xxx
#define VK_SNAPSHOT       0x2C // print screen
#define VK_HELP           0x2F ???
#define XK_KP_Separator		0xFFAC	/* separator, often comma */
#define VK_SEPARATOR      0x6C //???
#define XK_KP_Equal		0xFFBD	/* equals */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

#define XK_Shift_Lock	0xFFE6	/* Shift lock */
#define XK_Super_L		0xFFEB	/* Left super */
#define XK_Super_R		0xFFEC	/* Right super */
#define XK_Hyper_L		0xFFED	/* Left hyper */
#define XK_Hyper_R		0xFFEE	/* Right hyper */

#endif
