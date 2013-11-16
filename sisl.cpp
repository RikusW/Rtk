// vim:ts=4 sts=0 sw=4

#include <stdio.h>
#include <string.h>

#include "sisl.h"

//-----------------------------------------------------------------------------
// Signal data allocation

// fixed for now...
SSiSl bufx[2048]; // 32KiB
SSiSl *first = 0;
int sislcnt = 0;

int GetSiSl(SSiSl **s)
{
	if(!first) {
		first = bufx;
		for(int i = 0; i < 2047; i++) {
			bufx[i].next = &bufx[i+1];
		}
		bufx[2047].next = 0;
	}

	SSiSl *r = first;

	if(first->next)
		first = first->next;
	else {
		printf("GetSiSl OUT OF MEM\n");
		return 1; // fail
	}

	r->next = *s;
	*s = r;
	sislcnt++;
	return 0;
}

void FreeSiSl(SSiSl **s, SSiSl *p)
{
	if(!first)
		return; // should never actually happen...

	if(*s == p)
		*s = (*s)->next;
	else
	for(SSiSl *t = *s; t; t = t->next) {
		if(t->next == p) {
			t->next = p->next;
			break;
		}
	}

	//DBG
	p->f = 0;
	p->up = 0;
	p->conn = 0;

	p->next = first;
	first = p;
	sislcnt--;
	return;
}

// Signal data allocation
//-----------------------------------------------------------------------------
//XXX Make this thread safe sometime...

UPTR rtkuptr = 0;
CConn *rtkfptr = 0;

UPTR Rtk_GetUPTR() //Get UPTR value passed via si sl connection
{
	return rtkuptr;
}

void Rtk_SetUPTR(UPTR p)
{
	rtkuptr = p;
}

CConn *Rtk_GetFPTR() //Get the a pointer to the sending class 
{
	return rtkfptr;
}

void Rtk_SetFPTR(CConn *p)
{
	rtkfptr = p;
}

//-----------------------------------------------------------------------------
// Signal parameter processing - class RSpp

// all Si to Sl parameters must be of the same type
// a signal can have more parameters than a slot

inline void skip(cchar **c, char x)
{
	for(;  **c; (*c)++) if(**c == x) { (*c)++; break; }
}

bool SiSl_Match(cchar *sig, cchar *sisl)
{
	cchar *i = sig, *l = sisl;

	skip(&i,'(');
	skip(&l,'(');
	if(strncmp(i,l,strlen(l)-1)) { // compare without )
		printf("SiSl_Match fail = %s\n%s",sig,sisl);
		return false;
	}
	return true;
}

static void RSpp_Call(void *ths, void **ppv)
{
	RSpp *q,*p = (RSpp*)Rtk_GetUPTR(); q = p;
#ifdef RSPP_CHAIN
	for(q = p; q->f == RSpp_Call; q = q->spp);
#endif
	Rtk_SetUPTR(q->up);
	p->Call(ths,ppv);
}

RSpp::~RSpp()
{
#ifdef RSPP_CHAIN
	if(f == RSpp_Call) {
		delete spp;
	}
	spp = 0;
	f = 0;
#endif
}

bool RSpp::SiSl_Match(cchar *sig, cchar *sisl)
{
	return ::SiSl_Match(sig,sisl);
}

bool RSpp::SppMatch(cchar *sig, cchar *sisl)
{
#ifdef RSPP_CHAIN
	if(f == RSpp_Call)
		return spp->Match(sig,sisl);
#endif
	return ::SiSl_Match(sig,sisl);
}

bool RSpp::SppMatch(cchar *sig, cchar *ppsi, cchar *ppsl, cchar *sisl)
{
#ifdef RSPP_CHAIN
	if(f == RSpp_Call)
		return ::SiSl_Match(sig,ppsi) && spp->Match(ppsl,sisl);
#endif
	return ::SiSl_Match(sig,ppsi) && ::SiSl_Match(ppsl,sisl);
}

void RSpp::NextCall(void *ths, void **ppv)
{
#ifdef RSPP_CHAIN
	if(f == RSpp_Call) {
		if(spp) {
			spp->Call(ths,ppv);
		}else{
			printf("RSpp::NextCall spp = NULL!!!\n");
		}
	}else{
		f(ths,ppv);
	}
#else
	f(ths,ppv);
#endif
}

FSiSl RSpp::GetRSpp_Call()
{
	return RSpp_Call;
}

void RSpp::SetFU(FSiSl f, UPTR u)
{
	RSpp *spp = this;
#ifdef RSPP_CHAIN
	while(spp->f == RSpp_Call) {
		spp = spp->spp; // next
	}
#endif
	spp->f = f;
	spp->up= u;
}

// Signal parameter processing - class RSpp
//-----------------------------------------------------------------------------
// Signal parameter processing

inline void cutat(char **c, char x, char y)
{
	for(;  **c; (*c)++) if(**c == x || **c == y) { **c = 0; (*c)++; break; }
}

inline char *getp(char **c)
{
	char *t = *c;
	cutat(c,',',')');
	if(!*t) return 0;	
	return t;
}

inline void putp(char **c, cchar *p)
{
	while(*p) {
		**c = *p++;
		(*c)++;
	};
	**c = ',';
	(*c)++;
}

bool RSpp_Swizzle::Match(cchar *sig, cchar *sisl)
{
	char *p[10], *t;
	char so[200],de[200];
	char *s = so, *d = de;
	cupchar c;
	int i = 0, max = 0;

	// parse params
	strcpy(s,sig);
	skip((cchar**)&s,'(');
	while(i < 10 && (t = getp(&s))) p[i++] = t;
	max = i;
	while(i < 10) p[i++] = (char*)"x";
	
	// create swizzled string
	*d++ = '(';
	for(c = sw, i = 0; *c != 0xff && i < 15; c++, i++) {
		if(!*c || *c > max)
			return false;
		putp(&d,p[*c]);
	}
	if(*(d-1) == ',') d--;
	*d++ = ')';
	*d++ = 0;

	printf("Swizzle connect:\n,%s\n%s\n%s\n",sig,de,sisl);
	
	return SppMatch(de,sisl);
}

void RSpp_Swizzle::Call(void *ths,void **ppv)
{
	cupchar p = sw;
	void *buf[12];
	void **bb = buf;
	
	while(*p != 0xff) {
		*bb++ = ppv[*p++];
	}
	
	NextCall(ths,buf);
}

bool RSpp_Bool::Match(cchar *sig, cchar *sisl)
{
	//TODO detect first bool param and store to use in call
	return SiSl_Match(sig,"(bool)") && SppMatch(sig,sisl);
}

void RSpp_Bool::Call(void *ths,void **ppv)
{
	//XXX modifying the original ppv data is NOT allowed...
	bool *b = (bool*)&ppv[0];
	//bool *b = (bool*)(&ppv[param]);
	if(action & SPPB_INV) {
		*b = !*b;
	}
	switch(action) {
		case SPPB_PASST: if( *b) break; return;
		case SPPB_PASSF: if(!*b) break; return;
		case SPPB_TRUE: *b = true; break;
		case SPPB_FALSE: *b = false; break;
	}
	
	NextCall(ths,ppv);
}

bool RSpp_Uptr::Match(cchar *sig, cchar *sisl)
{
	for(cchar *p = string; p && *p; p++) if(*p == ',')
		return false;
	return SiSl_Match(string,sisl);
}

void RSpp_Uptr::Call(void *ths,void **ppv)
{
	void *buf[1];
	buf[0] = Rtk_GetUPTR();
	
	NextCall(ths,buf);
}

//-----------------------------------------------------------------------------
// class CConn

SiSlData sd_CConn = { 0, "CConn", 0, 0, 0 };

SiSlData *CConn::SiSl_GetSD()
{
	return &sd_CConn;
}

void CConn::SendSignal(SSiSl *s, void **ppv)
{
	for(; s; s = s->next) {
		if(!s->conn) { printf("ss NULL conn!!!\n"); continue; } //DBG
		if(!s->f) { printf("ss NULL f!!!\n"); continue; } //DBG

		Rtk_SetUPTR(s->up);
		Rtk_SetFPTR(this);

		s->f((void*)s->conn,ppv);
	}
}

//-----------------------------------------------------------------------------

void CConn::SiSl_Init()
{
	for(SiSlData *sd = SiSl_GetSD(); sd->up; sd = sd->up) {
		for(int i = 0; i < sd->sicnt; i++) {
			this->*sd->si[i] = 0;
		}
	}
}

FSiSl CConn::SiSl_GetF(cchar *sisl)
{
	for(SiSlData *sd = SiSl_GetSD(); sd->up; sd = sd->up) {
		if(sd->sil)
		for(SiSlLink *sil = sd->sil; sil->name; sil++) {
			if(!strcmp(sil->name, sisl)) {
				return sil->f;
			}
		}
	}
	return NULL;
}

bool CConn::SiSl_IDisconnect(FSiSl si, CConn *cnn, FSiSl sisl)
{
	bool ret = false;
	for(SiSlData *sd = SiSl_GetSD(); sd->up; sd = sd->up) {
		for(int i = 0; i < sd->sicnt; i++) {
			if(SiSl_Disconnect(&(this->*sd->si[i]), sd->sil[i].f, si, cnn, sisl)) {
				ret = true;
			}
		}
	}
	return ret;
}

bool CConn::SiSl_Disconnect(SSiSl **conn, FSiSl f, FSiSl si, CConn *cnn, FSiSl sisl)
{
	if(si && si!=f) return false;

	SSiSl *s = *conn,*t;

	while(s) {
		t = s->next;
		if((!cnn || s->conn==cnn) && (!sisl || s->f==sisl)) {
			this->SiSl_DisconnectFrom(s->conn);
			s->conn->SiSl_DisconnectFrom(this); //XXX

			// RSpp delete class
			if(s->f == RSpp_Call && s->up) {
				delete ((RSpp*)s->up);
			}
			FreeSiSl(conn,s);
		}
		s = t;
	}
	return true;
}

bool CConn::SiSl_Connect(SSiSl **conn, cchar *sig, CConn *cnn, cchar *sisl, UPTR u, RSpp *spp)
{
	FSiSl f = cnn->SiSl_GetF(sisl);
	if(f) {
	if(!spp) {
		if(SiSl_Match(sig,sisl)) {
			this->SiSl_ConnectTo(cnn);
			cnn->SiSl_ConnectTo(this);
			GetSiSl(conn);
			(*conn)->f = f;
			(*conn)->up = u;
			(*conn)->conn = cnn;
			return true;
		}
	}else{ //RSpp
		if(spp->Match(sig,sisl)) { 
			this->SiSl_ConnectTo(cnn);
			cnn->SiSl_ConnectTo(this);
			GetSiSl(conn);
			spp->SetFU(f,u);
			(*conn)->f = RSpp_Call;
			(*conn)->up = (UPTR)spp;
			(*conn)->conn = cnn;
			return true;
		}
	}}
	return false;
}

//-----------------------------------------------------------------------------
// Connection Reference Counting

void CConn::SiSl_ConnectTo(CConn *cnn)
{
	if(((uptr)conn) > 1) //xconn //XXX SEGF ?
	for(SSiSl *s = conn; s; s = s->next) {
		if(s->conn==cnn) { // already added ?
			s->refcnt++;
			return;
		}
	}
	if(((uptr)conn) > 1) { //xconn  (int) causes SEGFAULT
		if(GetSiSl(&conn))
			return;
	}else{
		conn = 0;
		if(GetSiSl(&conn)) {
			conn = (SSiSl*)1;
			return;
		}
	}
	conn->refcnt = 1;
	conn->conn = cnn;
}

void CConn::SiSl_DisconnectFrom(CConn *cnn)
{
	SSiSl *s = conn,*t;
	if(s == (SSiSl*)1) //xconn
		s = 0;

	while(s) {
		t = s->next;
		if(s->conn==cnn) {

			s->refcnt--;
//#ifdef PARANOID
			if(s->refcnt > 0xFFFF0000) { //DBG
				printf("--------Negative refcnt !!!!!------- = %i\n",(int)s->refcnt);
				s->refcnt=0;
			}
//#endif			
			if(!s->refcnt) {
				FreeSiSl(&conn,s);
				if(!conn) //xconn
					conn = (SSiSl*)1;
			}
		}
		s = t;
	}
}

//-----------------------------------------------------------------------------
// public members

bool CConn::Inherits(const char *c)
{
	SiSlTN *t;
	GetTypeName(&t);

	if(!t) {
		printf("big error TN null\n");
		return false;
	}

	do {
		if(!strcmp(t->name,c)) {
			return true;
		}
		t = t->up;
	}while(t);

	return false;
}

SiSlTN tn_CConn = { 0, "CConn", 0, 0 };
const char *CConn::GetTypeName(SiSlTN **tn)
{
	if(tn) {
		*tn = &tn_CConn;
	}
	return tn_CConn.name;
}

#include <stdarg.h>

void CConn::Call(CConn *cnn, cchar *sisl, UPTR u, ...)
{
	FSiSl f;
	int i,m;
	void *v;
	va_list l;
	void *pv[10];

	if(!sisl || !(f = cnn->SiSl_GetF(sisl))) {
		return;
	}
	for(i = 0, m = 0; sisl[i] && i < 80; i++) { // parameter count
		if(sisl[i] == '(') {
			m = 0;
			if(sisl[++i] == ')') {
				break;
			}else{
				m++;
			}
		}
		if(sisl[i] == ',') {
			m++;
		}
		if(m >= 10) {
			m = 10;
			break;
		}
	}
	va_start(l,u);
	for(i = 0; i < 10; pv[i++] = 0);
	for(i=0; i < m; i++) {
		v = va_arg(l, void*); //TODO type checking ???
		pv[i] = v;
		//printf("sisl Call %i %x\n",i,(int)v);
	}
	va_end(l);

	Rtk_SetFPTR(this);
	Rtk_SetUPTR(u);
	f(cnn, pv);
}

bool CConn::Connect(cchar *si, CConn *cnn, cchar *sisl, UPTR p, RSpp *spp)
{
	for(SiSlData *sd = SiSl_GetSD(); sd->up; sd = sd->up) {
		for(int i = 0; i < sd->sicnt; i++) {
			if(!strcmp(sd->sil[i].name, si)) {
			//printf("%x %s\n%x %s\n\n",si,si,sd->sil[i].name,sd->sil[i].name);
			if(SiSl_Connect(&(this->*sd->si[i]), si, cnn, sisl, p, spp)) {
				return true;
			}}
		}
	}
	if(spp) {
		delete spp;
	}
	return false; 
}

bool CConn::Disconnect(cchar *si, CConn *cnn, cchar *sisl)
{
	FSiSl i,l;
	if(si) i=SiSl_GetF(si); else i=NULL;
	if(cnn && sisl) l=cnn->SiSl_GetF(sisl); else l=NULL;
	return SiSl_IDisconnect(i,cnn,l);
}

bool CConn::ConnectSlot(cchar *sisl, CConn *cnn, cchar *si,UPTR p)
{
	if(!cnn) return false;
	return cnn->Connect(si,this,sisl,p);
}

bool CConn::DisconnectSlot(cchar *sisl, CConn *cnn, cchar *si)
{
	if(((uptr)conn) <= 1) //xconn //XXX (int) -> (u32) XXX  this was the SEGFAULT bug....
		return true;

	bool ret = false;
	FSiSl siID,sislID;
	if(sisl) sislID=SiSl_GetF(sisl); else sislID=NULL;

	for(SSiSl *t,*s = conn; s; s = t) {
		// use a temp or ELSE... it will change under your feet
		// courtesy of SiSl_IDisconnect
		t = s->next;
		if((!cnn || s->conn==cnn)) {
			if(!s->conn) { printf("disc cn NULL\n"); continue; }

			if(si) siID = s->conn->SiSl_GetF(si); else siID=NULL;
			// watch out for this line (don't like to be called from dtor)
			if(s->conn->SiSl_IDisconnect(siID,this,sislID)) {
				ret = true;
			}
		}
	}

	return ret;
}

bool CConn::DisconnectAll()
{
	bool ret=true;
	ret = SiSl_IDisconnect(0,0,0) && ret; //XXX SEGFAULT seems to have fixed it somewhat
	ret = DisconnectSlot(0,0,0) && ret; //XXX SEGFAULT   turned these 2 around--- seems not to matter anymore
	return ret;
}

void CConn::ListMe()
{
#ifdef XLIB
	for(SiSlData *sd = SiSl_GetSD(); sd->up; sd = sd->up) {
		printf("- %s\n",sd->name);
	}
	for(SiSlData *sd = SiSl_GetSD(); sd->up; sd = sd->up) {
		printf("\n%s\n",sd->name);
		if(sd->sil)
		for(SiSlLink *sil = sd->sil; sil->name; sil++) {
			printf("%s\n",sil->name);
		}
	}
#endif
}

// public members
//-----------------------------------------------------------------------------

