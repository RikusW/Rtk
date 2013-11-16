// run the next line to generate RK_xxxx defines
// gcc -E rkeygen.h | sed -e 's/#/\/\/V/' -e 's/#/\/\//' -e 's/def/#def/' -e 's/.0x/\t0x/' -e 's/\(RK_[A-Z0-9]\{1,4\}\t\)/\1\t/' -e 's/\(VK_[A-Z0-9]\{1,4\} \)\//\1\t\//'

// put the path to winresrc.h or winuser.h here
#include "/mnt/win98/Program Files/DevStudio/vc/include/winresrc.h"

define RK_BACK		VK_BACK #K_BACK
define RK_TAB		VK_TAB #K_TAB
define RK_RETURN	VK_RETURN #K_RETURN   #kp
define RK_SHIFT 	VK_SHIFT #K_SHIFT    #lr
define RK_CTRL		VK_CONTROL #K_CONTROL  #lr
define RK_ALT		VK_MENU #K_MENU     #lr
define RK_PAUSE		VK_PAUSE #K_PAUSE
define RK_ESCAPE	VK_ESCAPE #K_ESCAPE
define RK_SPACE		VK_SPACE #K_SPACE

define RK_PGUP		VK_PRIOR #K_PRIOR   #kp
define RK_PGDN		VK_NEXT #K_NEXT    #kp
define RK_HOME		VK_HOME #K_HOME    #kp
define RK_END		VK_END #K_END     #kp
define RK_LEFT		VK_LEFT #K_LEFT    #kp
define RK_RIGHT		VK_RIGHT #K_RIGHT   #kp
define RK_UP		VK_UP #K_UP      #kp
define RK_DOWN		VK_DOWN #K_DOWN    #kp
define RK_SELECT	VK_SELECT #K_SELECT
define RK_INSERT	VK_INSERT #K_INSERT  #kp
define RK_DELETE	VK_DELETE #K_DELETE  #kp

define RK_LWIN		VK_LWIN #K_LWIN
define RK_RWIN		VK_RWIN #K_RWIN
define RK_APPS		VK_APPS #K_APPS

define RK_KP0		VK_NUMPAD0 #K_NUMPAD0
define RK_KP1		VK_NUMPAD1 #K_NUMPAD1
define RK_KP2		VK_NUMPAD2 #K_NUMPAD2
define RK_KP3		VK_NUMPAD3 #K_NUMPAD3
define RK_KP4		VK_NUMPAD4 #K_NUMPAD4
define RK_KP5		VK_NUMPAD5 #K_NUMPAD5
define RK_KP6		VK_NUMPAD6 #K_NUMPAD6
define RK_KP7		VK_NUMPAD7 #K_NUMPAD7
define RK_KP8		VK_NUMPAD8 #K_NUMPAD8
define RK_KP9		VK_NUMPAD9 #K_NUMPAD9

define RK_KPMUL		VK_MULTIPLY #K_MULTIPLY
define RK_KPADD		VK_ADD #K_ADD
define RK_KPSUB		VK_SUBTRACT #K_SUBTRACT
define RK_KPDEC		VK_DECIMAL #K_DECIMAL
define RK_KPDIV		VK_DIVIDE #K_DIVIDE

define RK_F1		VK_F1 #K_F1
define RK_F2		VK_F2 #K_F2
define RK_F3		VK_F3 #K_F3
define RK_F4		VK_F4 #K_F4
define RK_F5		VK_F5 #K_F5
define RK_F6		VK_F6 #K_F6
define RK_F7		VK_F7 #K_F7
define RK_F8		VK_F8 #K_F8
define RK_F9		VK_F9 #K_F9
define RK_F10		VK_F10 #K_F10
define RK_F11		VK_F11 #K_F11
define RK_F12		VK_F12 #K_F12

define RK_NUMLOCK	VK_NUMLOCK #K_NUMLOCK
define RK_CAPSLOCK	VK_CAPITAL #K_CAPITAL
define RK_SCRLOCK	VK_SCROLL #K_SCROLL




