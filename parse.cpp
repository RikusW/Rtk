// vim:ts=4 sts=0 sw=4

#ifdef WIN32
#include <io.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
//#include <sys/times.h>


//char spc[30];
int FileTot=0;
int LineTot=0;

/*//debug
//
int nnew=0;
int nfree=0;
int npnew=0;
int npfree=0;
//
//end debug*/



//-------------------------------------------------------------------------------------------------
// CfgFile



// used for loading the config.in and .config files.
class CfgFile
{
public:

    CfgFile();
    ~CfgFile();

    int Parse(int CheckEof=1);
	int ParseClass();

	void ParseParams(char *p);
	//int DelChar(char *);
	//int WordCmp(const char *);

    int ReadLine();
    int Open(const char *strFile);
    int Close();
    int strncmp(const char *,const char *,int);


    char FName[100];
    int LineNum;
    void *pmem;
    char *cmem;
    char *LineStart;
    char *LineCurrent;
    char *LineNext;
    char *EndMem;
};



CfgFile::CfgFile()
{
//    FName[0]=0;
    LineNum=0;
    pmem=0;
}

CfgFile::~CfgFile()
{
    FileTot++;
    LineTot+=LineNum;
}

int CfgFile::Close()
{
    if(pmem) free(pmem); pmem=0;
    return 1;
}

int CfgFile::ReadLine()
{
    // trim left
    while(LineNext < EndMem)
    {
	if(*LineNext != ' ' && *LineNext != '\x09') break;
	LineNext++;
    }
    
    LineStart = LineNext;

    // get the line
next:
    while(LineNext < EndMem)
    {
		if(*LineNext == 0xd && *(LineNext+1) == 0xa) {
			*LineNext=0; goto rOK;
		}
		if(*LineNext == 0xa) goto rOK;
	//	if(*LineNext == ';') goto rOK; buggy - should be ignored in comment lines
		if(*LineNext == '#') *LineNext = 0;
		LineNext++;
    }
    if(LineNext > EndMem) return 0; // reading after eof...
    
rOK:
    *LineNext++ = 0;
    LineNum++;

    if(*(LineNext-2) == '\\')
    {
		*(LineNext-1) = ' ';
		*(LineNext-2) = ' ';
		goto next;
    }

   // trim right
   char *p=LineNext;
   while(--p>=LineStart)
    {
	if(*p == ' ' || *p == '\x09')
	    *p=0;
	else
	    break;
    }

    LineCurrent = LineStart;
    return 1;
}

int CfgFile::strncmp(const char *s1,const char *s2,int cnt)
{
    for(int i=0; i<cnt; i++)
    {
	if(s1[i] != s2[i]) return 1;
    }
    LineCurrent += cnt;
    return 0;
}

int CfgFile::Open(const char *strFile)
{
    int fd;
    struct stat st;

#ifdef WIN32
    fd = _open(strFile,_O_RDONLY | _O_BINARY);
#else
    fd = open(strFile,O_RDONLY);
#endif
    if(fd == -1)
    {
		printf("//Couldn't open <%s>.\n",strFile);
		return 0;
    }

    if(fstat(fd,&st) == -1)
    {
		printf("//Couldn't get file stats <%s>.\n",strFile);
		goto endclose;
    }

    pmem = (void*)malloc(st.st_size+1);
    if(!pmem)
    {
		printf("//Couldn't malloc memory <%s>.\n",strFile);
		goto endclose;
    }
    cmem = (char*)pmem;
    cmem[st.st_size]=0;
    LineNext=cmem;
    EndMem=&cmem[st.st_size];
    
    int x;
#ifdef WIN32
    if((x = _read(fd,pmem,st.st_size)) != st.st_size)
#else
    if((x = read(fd,pmem,st.st_size)) != st.st_size)
#endif
    {
		printf("//File read error <%s>.\n",strFile);
		goto endfree;
    }//*/

	// _read translate 0x0D 0x0A to 0x0A.....
    EndMem=&cmem[x]; // XXX hackkkkkk........

    strcpy(FName,strFile);
    close(fd);
    return 1;

endfree:
    free(pmem); pmem=0;
endclose:
    close(fd);
    return 0;
} 

#define cError(msg)\
{\
LineNext--; LineCurrent=LineStart;\
while(LineCurrent<LineNext) { if(!*LineCurrent) *LineCurrent=' '; LineCurrent++; }\
LineNext++;\
printf("//%s\n<%s> %i: %s\n",msg,FName,LineNum,LineStart);\
}

#define TRIM while(LineCurrent<LineNext && (*LineCurrent==' ' || *LineCurrent=='\x09')) LineCurrent++;
#define NEXTW(c) c=LineCurrent; while(LineCurrent<LineNext && (*LineCurrent!=' ' && *LineCurrent!='\x09' && *LineCurrent!='\0')) LineCurrent++; *LineCurrent++=0;
#define SCAN(c)               while(LineCurrent<LineNext && *LineCurrent!=c) LineCurrent++; *LineCurrent++=0;
#define SCANR(c) LineCurrent; while(LineCurrent<LineNext && *LineCurrent!=c) LineCurrent++; *LineCurrent++=0;
#define SCANX(c) { char *lc = LineCurrent; while(lc<LineNext && *lc!=c) lc++; *--lc=0; }
//#define SCANT(c) LineCurrent; while(LineCurrent<LineNext && *LineCurrent!=c) LineCurrent++; *LineCurrent++=0;
#define TERM

void DelChar(char *p)
{
	char *f = p+1;
	do *p++=*f++; while(*f); *p=0;
}

void Trim(char *p)
{
	while(*p==' ' || *p=='\t') DelChar(p);
}

void Trim1(char **pp)
{
	char *p=*pp;

	//remove all spaces except for one
	while(*p==' ' || *p=='\t')
	{
		if(*(p+1)==' ' || *(p+1)=='\t') {
			DelChar(p);
			*p=' ';
		}else
			break;
	}
	if(**pp==' ') (*pp)++;
}

int WordCmp(char **p,const char *s2)
{
	int l = strlen(s2);
	char *s1=*p;

	while(*s2) {
		if(*s1++ != *s2++) return 0;
    }

    *p += l;
    return 1;
}


void CfgFile::ParseParams(char *p)
{
	Trim(p);
	while(1)
	{
		Trim1(&p);
		if(WordCmp(&p,"const")) continue;
		if(WordCmp(&p,"void")) continue;
		if(WordCmp(&p,"bool")) continue;
		if(WordCmp(&p,"unsigned")) continue;
		if(WordCmp(&p,"signed")) continue;
		if(WordCmp(&p,"short")) continue;
		if(WordCmp(&p,"long")) continue;
		if(WordCmp(&p,"float")) continue;
		if(WordCmp(&p,"int")) continue;
		if(WordCmp(&p,"char")) continue;
		if(WordCmp(&p,"s8")) continue;
		if(WordCmp(&p,"u8")) continue;
		if(WordCmp(&p,"s16")) continue;
		if(WordCmp(&p,"u16")) continue;
		if(WordCmp(&p,"s32")) continue;
		if(WordCmp(&p,"u32")) continue;
		if(WordCmp(&p,"RTreeNode")) continue; //???
		if(WordCmp(&p,"SListNode")) continue; //???
		if(*p=='*')
		{
			if(*(p-1)==' ') DelChar(p-1); else p++;
			continue;
		}else
		{
			if(*(p-1)==' ') p--;
			Trim(p);

			if(*p==',') { p++; continue; }
			if(!*p) return;

			// delete all unrecognized words
			while(*p && *p!=',') DelChar(p);
			if(*p==',') { p++; Trim(p); }
			if(*p) continue;

			return;
		}
	}
}

//---------------------------------------------------------
// Output

int iSi,iSl;
char *pSi[1000],*pSl[1000],*pSiParams[1000],*pSlParams[1000];

void ShowSiSl()
{
	int i;

	if(!iSi && !iSl) return;

	printf("/*---Signals---\n");
	for(i=0; i<iSi; i++)
	{
		printf("%s(%s)\n",pSi[i],pSiParams[i]);
	}
	printf("//---Slots-----\n");
	for(i=0; i<iSl; i++)
	{
		printf("%s(%s)\n",pSl[i],pSlParams[i]);
	}
	printf("//-----------*/\n");
}

void CopyStrSD(char *t, char *f, int skip, char d)
{
	*t=0;
	int i=0;
	do {
		if(*f==d) { i++; f++; }
		if(!*f) { *t=0; break; }

		if(i > skip) { *t=0; break; } else
		 if(i==skip) { *t++ = *f; }
	}while(*f++);
}

inline void PrintStubs(int cnt, char **pS, char **pSp, char *c, char *buf)
{
	int i,j;
	for(i=0; i<cnt; i++) {
		printf("void %s_%s", c, pS[i]);
		for(j=0; j<100; j++) {
			CopyStrSD(buf,pSp[i],j,',');
			if(!*buf) break;
			printf("_%c",*buf);
		}
		printf("(void *ths, void **ppv)\t{ ((%s*)ths)->%s(", c, pS[i]);
		for(j=0; j<100; j++) {
			CopyStrSD(buf,pSp[i],j,',');
			if(!*buf) break; else if(j) printf(",");
			printf("(%s)(uptr)ppv[%i]",buf,j); //x86x64
		}
		printf("); }\n");
	}
}

void PrintSil(int cnt, char **pS, char **pSp, char *c, char *buf)
{
	int i,j;
	for(i=0; i<cnt; i++) {
		printf("\t{ \"%s(%s)\", ", pS[i],pSp[i]);

		printf("%s_%s", c, pS[i]);
		for(j=0; j<100; j++) {
			CopyStrSD(buf,pSp[i],j,',');
			if(!*buf) break;
			printf("_%c",*buf);
		}
		printf(" },\n");
	}
}

// c - current class
// p - parent class
/*
void PrintInherit(char *c,char*p)
{
	printf("\
bool %s::Inherits(const char *c)\n\
{\n\tif(!strcmp(\"%s\",c)) {\n\
\t\treturn true;\n\t}\n\
\treturn %s::Inherits(c);\n\
}\n\n",c,c,p);
}*/

void PrintGTN(char *c,char*p,int si, int sl) //si / sl count
{
	printf("SiSlTN tn_%s = { 0, \"%s\", %i, %i };\n",c,c,si,sl);
	printf("const char *%s::GetTypeName(SiSlTN **tn)\n{\n",c);
	printf("\tif(tn) {\n\t\tif(!tn_%s.up) {\n\t\t\t%s::GetTypeName(&tn_%s.up);\n",c,p,c);
	printf("\t\t}\n\t\t*tn = &tn_%s;\n\t}\n\treturn tn_%s.name;\n}\n\n",c,c);
}
		
void GenCode(char *c,char*p,char*g)
{
	char buf[100];
	int i,j,k=0,level = atoi(g ? g : "0");

	if(!iSi && !iSl) {
		if(level == -1)
		//	PrintInherit(c,p);
			PrintGTN(c,p,0,0);
		return;
	}

	printf("//------------------------------\n");
	printf("//---Start of connector stubs---\n\n");

	PrintStubs(iSi,pSi,pSiParams,c,buf);
	PrintStubs(iSl,pSl,pSlParams,c,buf);

	printf("\n//---End of connector stubs---\n");
	printf("//----------------------------\n");
	printf("//---Start of SiSl Data-------\n\n");

	// si
	if(iSi) {
		printf("CSiSl %s::*si_%s[%i] =\n{\n",c,c,iSi);
		for(i=0; i<iSi; i++)
		{
			printf("\t&%s::CSiSl_%s,\n",c,pSi[i]);
		}
	    printf("};\nstatic union { CSiSl %s::**d; CSiSl CConn::**b; } upp_%s = { si_%s };\n\n",c,c,c);
	}

	// sil
	printf("SiSlLink sil_%s[%i] =\n{\n",c,iSi+iSl+1);
	PrintSil(iSi,pSi,pSiParams,c,buf);
	PrintSil(iSl,pSl,pSlParams,c,buf);
	printf("\t{ 0,0 }\n};\n\n");

	// sd
	if(iSi) {
		printf("SiSlData sd_%s = { 0, \"%s\", %i, upp_%s.b, sil_%s };\n\n",c,c,iSi,c,c);
	}else{
		printf("SiSlData sd_%s = { 0, \"%s\", %i, 0, sil_%s };\n\n",c,c,iSi,c);
	}
	
	printf("SiSlData *%s::SiSl_GetSD()\n{\n",c);
	printf("\tif(!sd_%s.up) {\n\t\tsd_%s.up = %s::SiSl_GetSD();\n\t}\n",c,c,p);
	printf("\treturn &sd_%s;\n}\n\n",c);

	//PrintInherit(c,p);

	PrintGTN(c,p,iSi,iSl);

	printf("//---End of SiSl Data-----------\n");
	printf("//------------------------------\n");
	printf("//----Start of signal stubs-----\n\n");

	for(i=0; i<iSi; i++) {
		printf("void %s::%s(",c,pSi[i]);
		for(j=0; j<100; j++) {
			CopyStrSD(buf,pSiParams[i],j,',');
			if(!*buf) break; else if(j) printf(", ");
			printf("%s %c",buf,'a'+j);
		}
		printf(")\n{\n\tvoid *pv[%i]%s;\n",j?j:1, j?"":" = { 0 }");

		for(k=0; k<j; k++) {
			printf("\tpv[%i]=(void*)(uptr)%c;\n",k,'a'+k); //x86x64
		}
		printf("\tSendSignal(CSiSl_%s,pv);\n}\n\n",pSi[i]);
	}

	printf("//-----End of signal stubs------\n");
	printf("//------------------------------\n");

}

// Output
//---------------------------------------------------------

int CfgFile::ParseClass()
{
	iSi = iSl = 0;

	char *c,*p,*g; // child parent generation
	char *sisl,*params,*t;
	int blevel=0;

	TRIM NEXTW(c) TRIM
	if(strncmp(LineCurrent,":",1)) return 0;

	TRIM
	if(strncmp(LineCurrent,"public",6)) return 0; //XXX public only for now

	TRIM NEXTW(p);

    while(ReadLine())
    {
		if(*LineStart ==  0 ) continue; // empty lines + comments

		t=LineCurrent;
		while(t<LineNext) {
			if(*t=='{')
				blevel++;
			if(*t=='}')
				blevel--;
			t++;
			if(blevel <= 0) {
				ShowSiSl();
				GenCode(c,p,g);
				printf("//done with class %s : %s\n\n",c,p);
				goto end;
			}
		}
		TRIM

		if(!strncmp(LineCurrent,"SISLTN()",8)) {
			g = (char*)"-1";
			printf("//--TN--class %s : %s\n",c,p);
		}else
		if(!strncmp(LineCurrent,"SISL(",5))
		{
		//	TRIM
			NEXTW(g);
			if(*g == ')') g = (char*)" gXg ";
			if(*(g+1) == ')') *(g+1) = 0;
			if(*(g+2) == ')') *(g+2) = 0;
			//if(*g && g[strlen(g)-1]==')') g[strlen(g)-1]=0; //XXX
			printf("//---class %s : %s\n",c,p);
		}else
		if(!strncmp(LineCurrent,"SIGNAL(",7))
		{
			SCANX('{');
			//printf("s %s",LineCurrent);
			sisl = SCANR(')'); SCAN('('); //===
			params = SCANR(')');
			//xprintf("//   %s (%s)",sisl,params);
			ParseParams(params);
			//printf(" --> (%s)\n",params);

			pSi[iSi]=sisl;
			pSiParams[iSi]=params;
			iSi++;
		}else
		if(!strncmp(LineCurrent,"SLOT(",5) || !strncmp(LineCurrent,"virtual SLOT(",13))
		{
			SCANX('{');
			//printf("- %s",LineCurrent);
			sisl = SCANR(')'); TERM SCAN('('); //===
			params = SCANR(')'); TERM
			//xprintf("//   %s (%s)",sisl,params);
			ParseParams(params);
			//printf(" --> (%s)\n",params);

			pSl[iSl]=sisl;
			pSlParams[iSl]=params;
			iSl++;
		}
	}
end:
	return 0;
}


int CfgFile::Parse(int CheckEof)
{
    while(ReadLine())
    {
		if(*LineStart ==  0 ) continue; // empty lines + comments

		TRIM
		if(!strncmp(LineCurrent,"class",5)) ParseClass();
	}
    return 1;
}

/*
 * P1 Is the file to be parsed, will be #included
 * P2 If specified will override the #include of P1,
 *    can be "" in which case nothing is #included
 * P3 If specified will provide a second header #include,
 *    now either P2 or P3 can be "1" which will set it to P1.
 *    P2 Cannot be "" if P3 is specified.
 */

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("#error parse: Specify a input file\n");
		return 1;
	}

	CfgFile f;
	if(!f.Open(argv[1])) {
		printf("#error parse: Open %s fail\n",argv[1]);
		return 1;
	}

	printf("// vim:ts=4 sts=0 sw=4\n");
	printf("//Automatically generated from: %s\n",argv[1]);
	printf("//Don't edit, all changes _will be lost_.\n\n");

	switch(argc) {
	case 2:
		printf("#include \"%s\"\n\n",argv[1]);
	break;
	case 3:
		if(*argv[2]) {
			printf("#include \"%s\"\n\n",argv[2]);
		}
	break;
	case 4:
		printf("#include \"%s\"\n"  ,*argv[2] == '1' ? argv[1] : argv[2]);
		printf("#include \"%s\"\n\n",*argv[3] == '1' ? argv[1] : argv[3]);
	break;
	default:
		printf("#error parse: Too many parameters\n");
		f.Close();
		return 1;
	}

	f.Parse();
	f.Close();
	return 0;
}







/*
int CfgFile::CopyTo(char **f,char **t)
{
	if(*f == *t) return;

	//remove all spaces except for one
	while(**f==' ' || **f=='\t')
		if(*((*f)+1)==' ' || *((*f)+1)=='\t') (*f)++;

	while(*f < LineEnd && **f) *(*t)++=*(*f)++;
	**t=0;
}

*/


