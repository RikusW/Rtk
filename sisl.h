// vim:ts=4 sts=0 sw=4

#ifndef SISL_H
#define SISL_H

#include "Rtk-base/RTypes.h"

typedef const char cchar;
typedef void * UPTR; // user value
typedef const unsigned char * cupchar;

#ifndef NULL
#define NULL 0
#endif

#define RSPP_CHAIN

class CConn;
typedef void (*FSiSl)(void*,void**);

UPTR Rtk_GetUPTR();
void Rtk_SetUPTR(UPTR);
CConn *Rtk_GetFPTR();
void Rtk_SetFPTR(CConn*);

//-----------------------------------------------------------------------------
// Signal parameter processing

class RSpp
{
protected:
	RSpp() { f = 0; up = 0; };
public:
	virtual ~RSpp();

	bool SiSl_Match(cchar *sig, cchar *sisl);
	bool SppMatch(cchar *sig, cchar *sisl);
	bool SppMatch(cchar *sig, cchar *ppsi, cchar *ppsl, cchar *sisl);
	virtual bool Match(cchar *sig, cchar *sisl) { return false; };
	virtual void Call(void*,void**) {};
	void NextCall(void*,void**);
	FSiSl GetRSpp_Call();
	inline void SetFU(FSiSl f, UPTR u);
#ifdef RSPP_CHAIN
	inline void Link(RSpp *p) { spp = p; f = GetRSpp_Call(); };
#endif

	FSiSl f;
	union {
		UPTR up;
		RSpp *spp;
	};
};

class RSpp_Swizzle : public RSpp
{
protected:
	RSpp_Swizzle() {};
public:
	RSpp_Swizzle(cupchar c) { sw = c; };
	~RSpp_Swizzle() {};

	bool Match(cchar *sig, cchar *sisl);
	void Call(void*,void**);
	cupchar sw;
};

#define SPPB_PASST 1
#define SPPB_PASSF 2
#define SPPB_TRUE  3
#define SPPB_FALSE 4
#define SPPB_INV   8

class RSpp_Bool : public RSpp
{
protected:
	RSpp_Bool() {};
public:
	RSpp_Bool(int i) { action = i; param = 0; };
	~RSpp_Bool() {};

	bool Match(cchar *sig, cchar *sisl);
	void Call(void*,void**);
	short action;
	short param;
};

class RSpp_Uptr : public RSpp
{
protected:
	RSpp_Uptr() {};
public:
	RSpp_Uptr(cchar *c) { string = c; };
	~RSpp_Uptr() {};

	bool Match(cchar *sig, cchar *sisl);
	void Call(void*,void**);
	cchar *string;
};



//-------------------------------------
#ifdef RSPP_CHAIN
#define CLASS_RSPP(name) \
class RSpp_##name : public RSpp \
{ \
public: \
	RSpp_##name() {}; \
	~RSpp_##name() {};

#define CLASS_RSPP_NL(name) \
class RSpp_##name : public RSpp \
{ \
protected:\
	void Link(RSpp *p) {};\
public: \
	RSpp_##name() {}; \
	~RSpp_##name() {};
#else //-------------------------------
#define CLASS_RSPP(name) \
class RSpp_##name : public RSpp \
{ \
public: \
	RSpp_##name() {}; \
	~RSpp_##name() {};
#endif
#define xx }}} // vim colored syntax zap
#undef xx

// Signal parameter processing
//-----------------------------------------------------------------------------

struct SSiSl
{
	SSiSl *next; //ab
	CConn *conn; //ab
	struct {
		union { FSiSl f;  u32 refcnt; }; //a b
		union { UPTR up;  u32 rsvd;   }; //a b
	};
};

typedef SSiSl* CSiSl;
struct SiSlData;
struct SiSlTN;

#define DINIT() if(conn) { Dinit(); }

class CConn
{
protected:
	CConn() { conn=(SSiSl*)1; };
	void Init() { SiSl_Init(); };
	void Dinit() { DisconnectAll(); conn = 0; };
	virtual ~CConn() { DINIT(); };

	// This do double duty as bDInit and a ptr
	// code that use it as a ptr must treat a value of 1 as 0/NULL
	// set it to 1 instead of 0/NULL
	// search for xconn in sisl.cpp to see code that use this
	SSiSl *conn;

	virtual SiSlData *SiSl_GetSD();
	void SendSignal(SSiSl *s, void **ppv);

private:
	void SiSl_Init();
	FSiSl SiSl_GetF(cchar *sisl);
	bool SiSl_IDisconnect(FSiSl si, CConn *cnn, FSiSl sisl);
	bool SiSl_Disconnect(SSiSl **conn, FSiSl f, FSiSl si, CConn *cnn, FSiSl sisl);
	bool SiSl_Connect(SSiSl**, cchar*, CConn*, cchar*, UPTR, RSpp*);

	// connection tracking + refcnt
	void SiSl_ConnectTo(CConn*);
	void SiSl_DisconnectFrom(CConn*);

public:
	bool Inherits(const char *);
	virtual const char *GetTypeName(SiSlTN **tn=0);
	void Call(CConn *cnn, cchar *sisl, UPTR u=0, ...);
	bool Connect(cchar *si, CConn *cnn, cchar *sisl, UPTR u=0, RSpp *spp=0);
	bool Disconnect(cchar *si, CConn *cnn, cchar *sisl);
	bool ConnectSlot(cchar *sisl, CConn *cnn, cchar *si,UPTR p=0);
	bool DisconnectSlot(cchar *sisl, CConn *cnn, cchar *si);
	bool DisconnectAll();
	void ListMe();
};

struct SiSlTN
{
	SiSlTN *up;
	cchar *name;
	int si,sl;
};

struct SiSlLink
{
	cchar *name;
	FSiSl f;
};

struct SiSlData
{
	SiSlData *up;
	cchar *name;
	int sicnt;
	CSiSl CConn::**si;
	SiSlLink *sil;
};

// bool Inherits(const char *)
#define SISL(level)  SiSlData *SiSl_GetSD(); const char *GetTypeName(SiSlTN **tn=0);
#define SISLTN() const char *GetTypeName(SiSlTN **tn=0)
#define SIGNAL(name) CSiSl CSiSl_##name; void name
#define SLOT(name)   void name

#endif //SISL_H
