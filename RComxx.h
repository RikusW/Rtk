// vim:ts=4 sts=0 sw=4

#include "rtypes.h" 

#define COM1 1
#define COM2 2
#define COM3 3
#define COM4 4

//-----------------------------------------------
// RCom Linux

#ifndef _WIN32

#include <unistd.h>
#include <sys/ioctl.h>

// see /usr/include/asm/termios.h
//set
#define RCOM_DTR TIOCM_DTR
#define RCOM_RTS TIOCM_RTS

//get
#define RCOM_DSR TIOCM_DSR
#define RCOM_CTS TIOCM_CTS

#define RCOM_RI TIOCM_RI
#define RCOM_CD TIOCM_CD

// used for set/clear break
#define RCOM_BREAK 0x100000

class RCom
{
public:
	RCom() { hCom = 0; };
	~RCom() { Close(); };

	int Open(const char *p);
	int Open(int i);
	int Close();

	int SetBaudRate(const char *p);
	int Read(char *buf, int cnt)  { return read(hCom,buf,cnt);  } //XXX
	int Write(char *buf, int cnt) { return write(hCom,buf,cnt); } //XXX

	int SetLines(int f);
	int ClrLines(int f);
	int GetLines();

	int SetDtr();
	int ClrDtr();
	int SetRts();
	int ClrRts();
	int SetBreak();
	int ClrBreak();
	
	int hCom;
};

#endif

// RCom Linux
//-----------------------------------------------
// RCom Windows

#ifdef _WIN32 

//set
#define RCOM_DTR 1
#define RCOM_RTS 2

//get
#define RCOM_DSR MS_DSR_ON
#define RCOM_CTS MS_CTS_ON

#define RCOM_RI MS_RING_ON
#define RCOM_CD MS_RLSD_ON

// used for set/clear break
#define RCOM_BREAK 0x100000

class RCom
{
public:
	RCom() { hCom = 0; };
	~RCom() { Close(); };

	int Open(const char *p);
	int Open(int i);
	int Close();
	
	int SetBaudRate(const char *p);
	int Read(char *buf, int cnt);
	int Write(char *buf, int cnt);

	int SetLines(int f);
	int ClrLines(int f);
	int GetLines();

	int SetDtr();
	int ClrDtr();
	int SetRts();
	int ClrRts();
	int SetBreak();
	int ClrBreak();
	
	void *hCom;
};

#endif

// RCom Windows
//-----------------------------------------------

