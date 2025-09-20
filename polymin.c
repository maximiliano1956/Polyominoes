#ifdef __CYGWIN__
typedef void *locale_t;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/Frame.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/List.h>
#include <Xm/SelectioB.h>
#include <Xm/Scale.h>
#include <Xm/ArrowBG.h>
#include <Xm/FileSB.h>
#include <Xm/Text.h>
#include <Xm/Protocols.h>

#include <X11/Xmu/Editres.h>

#include <xpm.h>

#include "polymin.h"
#include "dlx.h"

#if USE_JPG
#include <jpeglib.h>
#endif

void RunThread(void);
int Decode(void);
int Dec_par(FILE *,char *,char **,int *,int *,int ***);
void CreateClass(void);
void MakeClasses(struct Pezzo *);
int TesPez(struct Pezzo *,struct Poly_field *,char,char);
int InsPez(struct Poly_def *,struct Pezzo *,int *,int *,struct Poly_field *,struct Stack *,int *,int,int);
int RemPez(struct Poly_def **,struct Pezzo **,struct Poly_field *,struct Stack *,int *,int *,int *,int *,int *);
int InitSearch(char *,char *,struct Poly_def **,struct Poly_field **,struct Stack **,int *,char **,int *);
void EndSearch(struct Poly_field *,struct Stack *,char *);
int GenIsle(struct Poly_field *,int,char *);
int Isle(struct Poly_field *,char *,int *);
int conta_mark(struct Poly_field *,int);
int GetCongr(struct Poly_def *);
int CaricaSol(struct Poly_field *);
int Ugu(int **,int **);
int compare(char,char,int **,int **);
int Inserimento(void);
int Rimozione(void);
int Init(char *);
int Init1(char *,char *);
void GenDim(char **);
void InitGra(int argc,char **);
void DrawField(Drawable,int,int,int **);
void Gestimer(XtPointer);
void GesTime(void);
void ReadPixmap(void);
void call_exp(Widget,XtPointer,XtPointer);
void file_cb(Widget,XtPointer,XtPointer);
void sel_cb(Widget,XtPointer,XtPointer);
void option_cb(Widget,XtPointer,XtPointer);
void option_cb1(Widget,XtPointer,XtPointer);
void option_cb2(Widget,XtPointer,XtPointer);
void option_cb3(Widget,XtPointer,XtPointer);
void option_cb4(Widget,XtPointer,XtPointer);
void option_cb5(Widget,XtPointer,XtPointer);
void option_cb6(Widget,XtPointer,XtPointer);
void list_set_cb(Widget,XtPointer,XtPointer);
void list_field_cb(Widget,XtPointer,XtPointer);
void Gest_ok_cb(Widget,XtPointer,XtPointer);
void up_cb(Widget,XtPointer,XtPointer);
void left_cb(Widget,XtPointer,XtPointer);
void right_cb(Widget,XtPointer,XtPointer);
void down_cb(Widget,XtPointer,XtPointer);
void help_cb(Widget,XtPointer,XtPointer);
void vis_cb(Widget,XtPointer,XtPointer);
void snap_cb(Widget,XtPointer,XtPointer);
void shsol_cb(Widget,XtPointer,XtPointer);
void GestAbout(void);
void GestQuit(void);
void GenLists(void);
void ok_pushed(Widget,XtPointer,XtPointer);
void cancel_pushed(Widget,XtPointer,XtPointer);
Widget PostDialog(Widget,int,char *,int);
int CheckSet(char *);
void GestIntest(void);
void ViewSol(int);
void ViewEnd(void);
void GenStrTime(void);
int WriteSol(int);
int AppendSol(int,int);
void IntestSol(FILE *);
void WSol(FILE *,int,int **,char *);
void GetNameSol(char *,int,int);
void GetNameSnap(char *);
int GetLato(int);
int ReadSol(void);
void SaveSols(void);
void GeSaveSols(XtPointer);
void GeDestroy(XtPointer);
void ReadSols(void);
void GeReadSols(XtPointer);
void VSol(struct Sol *,int);
void RSnap(void);
void GeReadSnap(XtPointer);
int ReadSnap(char *);
void SSnap(void);
void GeSaveSnap(XtPointer);
int SaveSnap(void);
void GetDefaults(char *);
void LoadDflts(void);
int LoadDfltsFile(FILE *);
void SavePixmap(char *);
#if USE_JPG
void SaveJpeg(char *);
#endif
void snap_ok_cb(Widget,XtPointer,XtPointer);
void snap_cancel_cb(Widget,XtPointer,XtPointer);
struct Poly_def *GetAddDefPezzo(int);
struct Pezzo *GetAddPezzo(struct Poly_def *,int);
void Create_options(void);
void Create_scales(void);
void Create_com(void);
void Create_arrows(void);
void Refresh_options(void);
void Refresh_scales(void);
void unsensitive(void);
void sensitive(void);

void ge_start(Widget,XtPointer,XtPointer);
void ge_stop(Widget,XtPointer,XtPointer);
void ge_reset(Widget,XtPointer,XtPointer);

void *GenMatDlx(void *);
void InsPz(struct Poly_def *p_def,struct Pezzo *p_pez,int Nr,int Nc,struct Poly_field *p_field);
void RemPz(struct Pezzo *p_pez,int Nr,int Nc,struct Poly_field *p_field);

u_int64_t size;
char *Pass;
unsigned int *Sparse;
int coupez;
XtIntervalId tim;
int click_snap;
int click_vis;
int vised;
int option_dlx;

struct Poly_set *Begin;
struct Solutions solutions;
struct Sol *solview;
int nsol;
int nsolview;
int numpez;

struct Poly_def *p_tpez;
struct Poly_field *p_field;
struct Stack *stack;
int ind_stk;
char *pila_isle;
int numrow;
int numcol;
int nr_max;
int nc_max;
int npez_max;
struct Poly_def *pdef_corr;
struct Pezzo *pez_corr;
XmStringTable set_list;
XmStringTable *field_list;
int Nset;
char *Nfield;
int click;
char StrTime[32];
int lpix;
int bx;
int by;

int congr;
int stato;
int mode;
int ckder;
int waitsol;
int tipovis;
int fase;
int w_dra;
int h_dra;
int width;
int height;
int planes;
int sel_set;
char name_sel_set[128];
int sel_field;
char name_sel_field[128];
char namesnap[128];
int sz;
int save;
int autosnap;
int fmt_save;
int first;
int previs;
int presnap;
char dataname[32];
float timeout;
float duty;
char *record;
int lenrec;
int lenrec_ori;
int nr_pez;
int nr_clas;
int autosnp;
int sfondo;

Widget toplevel;
Widget message_area;
Widget intest_area;
Widget time_area;
Widget frame;
Widget frame0;
Widget frame1;
Widget frame2;
Widget frame20;
Widget frame3;
Widget frame30;
Widget frame40;
Widget frame50;
Widget pushb_start;
Widget pushb_stop;
Widget pushb_reset;
Widget drawing_a;
Widget main_w;
Widget form_w;
Widget form_w1;
Widget form_w2;
Widget sw;
Widget rc;
Widget rc1;
Widget menubar;
Widget option_menu;
Widget option_menu1;
Widget option_menu2;
Widget option_menu3;
Widget option_menu4;
Widget option_menu5;
Widget option_menu6;
Widget list_set;
Widget list_field;
Widget dial;
Widget dial_snap;
Widget scale_vis;
Widget scale_snp;
Widget arrow1;
Widget arrow2;
Widget arrow3;
Widget arrow4;
Widget showsol;
Widget file_menu;
Widget selec_menu;
Widget help_menu;
Display *dsp;
Screen *scr;
XtAppContext app;
Pixmap pixmap;
Pixmap pix[NUMPIX];
Pixmap pol_icon;
GC gc;
Display *dpy;
Window win;
Atom WM_DELETE_WINDOW;

char *pixmap_names[NUMPIX]=
	{
	"poly1.xpm",				/*   0 */
	"poly1-1.xpm",				/*   1 */
	"poly1-2.xpm",				/*   2 */
	"poly1-3.xpm",				/*   3 */
	"poly2.xpm",				/*   4 */
	"poly2-2.xpm",				/*   5 */
	"poly2-3.xpm",				/*   6 */
	"poly2-6.xpm",				/*   7 */
	"poly3.xpm",				/*   8 */
	"poly3-2.xpm",				/*   9 */
	"poly4.xpm",				/*  10 */
	"poly4-2.xpm",				/*  11 */
	"poly4-3.xpm",				/*  12 */
	"poly4-6.xpm",				/*  13 */
	"poly5.xpm",				/*  14 */
	"poly5-3.xpm",				/*  15 */
	"poly6.xpm",				/*  16 */
	"poly6-2.xpm",				/*  17 */
	"poly7.xpm",				/*  18 */
	"poly7-3.xpm",				/*  19 */
	"poly8.xpm",				/*  20 */
	"poly10.xpm",				/*  21 */
	"poly10-2.xpm",				/*  22 */
	"poly10-3.xpm",				/*  23 */
	"poly10-7.xpm",				/*  24 */
	"fpoly1.xpm",				/*  25 */
	"fpoly1-1.xpm",				/*  26 */
	"fpoly1-2.xpm",				/*  27 */
	"fpoly1-3.xpm",				/*  28 */
	"fpoly2.xpm",				/*  29 */
	"fpoly2-2.xpm",				/*  30 */
	"fpoly2-3.xpm",				/*  31 */
	"fpoly2-6.xpm",				/*  32 */
	"fpoly3.xpm",				/*  33 */
	"fpoly3-2.xpm",				/*  34 */
	"fpoly4.xpm",				/*  35 */
	"fpoly4-2.xpm",				/*  36 */
	"fpoly4-3.xpm",				/*  37 */
	"fpoly4-6.xpm",				/*  38 */
	"fpoly5.xpm",				/*  39 */
	"fpoly5-3.xpm",				/*  40 */
	"fpoly6.xpm",				/*  41 */
	"fpoly6-2.xpm",				/*  42 */
	"fpoly7.xpm",				/*  43 */
	"fpoly7-3.xpm",				/*  44 */
	"fpoly8.xpm",				/*  45 */
	"fpoly10.xpm",				/*  46 */
	"fpoly10-2.xpm",			/*  47 */
	"fpoly10-3.xpm",			/*  48 */
	"fpoly10-7.xpm",			/*  49 */
	"epoly1.xpm",				/*  50 */
	"epoly1-1.xpm",				/*  51 */
	"epoly1-2.xpm",				/*  52 */
	"epoly1-3.xpm",				/*  53 */
	"epoly2.xpm",				/*  54 */
	"epoly2-2.xpm",				/*  55 */
	"epoly2-3.xpm",				/*  56 */
	"epoly2-6.xpm",				/*  57 */
	"epoly3.xpm",				/*  58 */
	"epoly3-2.xpm",				/*  59 */
	"epoly4.xpm",				/*  60 */
	"epoly4-2.xpm",				/*  61 */
	"epoly4-3.xpm",				/*  62 */
	"epoly4-6.xpm",				/*  63 */
	"epoly5.xpm",				/*  64 */
	"epoly5-3.xpm",				/*  65 */
	"epoly6.xpm",				/*  66 */
	"epoly6-2.xpm",				/*  67 */
	"epoly7.xpm",				/*  68 */
	"epoly7-3.xpm",				/*  69 */
	"epoly8.xpm",				/*  70 */
	"epoly10.xpm",				/*  71 */
	"epoly10-2.xpm",			/*  72 */
	"epoly10-3.xpm",			/*  73 */
	"epoly10-7.xpm",			/*  74 */
	"poly10-8.xpm",				/*  75 */
	"fpoly10-8.xpm",			/*  76 */
	"epoly10-8.xpm",			/*  77 */
	"poly1-4.xpm",				/*  78 */
	"fpoly1-4.xpm",				/*  79 */
	"epoly1-4.xpm",				/*  80 */
	"poly2-7.xpm",				/*  81 */
	"fpoly2-7.xpm",				/*  82 */
	"epoly2-7.xpm",				/*  83 */
	"poly4-7.xpm",				/*  84 */
	"fpoly4-7.xpm",				/*  85 */
	"epoly4-7.xpm",				/*  86 */
	};

int main(int argc,char *argv[])
{
int noop;

sz=SMALL;
option_dlx=NO;

for (noop=1;noop<argc;noop++)
	{
	if (!strcmp(argv[noop],"-L")) sz=LARGE;
	if (!strcmp(argv[noop],"-DLX")) option_dlx=YES;
	}

if (Init(argv[0])) exit(1);

if (Init1(name_sel_set,name_sel_field)) exit(1);

InitGra(argc,argv);

exit(0);
}


/********************************/
/*				*/
/*	    Decode		*/
/*				*/
/********************************/

/* OUTPUT:	OKAY				*/
/*		ERR		errore		*/

int Decode()
{
FILE *pfile;
int nr;
int nc;
int **mat;
char tipo;
unsigned char fl_ini;
int esito;
int num_pez;
struct Poly_set *point_set=NULL;
struct Poly_set *pun_set=NULL;
struct Poly_def **pun_def=NULL;
struct Poly_field **pun_field=NULL;
char *name;

Begin=NULL;

/* Apre file dati */

if ((pfile=fopen(dataname,"r"))==NULL)
	{
	printf("\nData file (%s) not found!\n\n",dataname);
	return(ERR);
	}

fl_ini=YES;

while (1)
	{
	esito=Dec_par(pfile,&tipo,&name,&nr,&nc,&mat);

	if (esito==ERR)
		{
		printf("\nData file \"%s\" has a syntax error!\n\n",dataname);
		fclose(pfile);
		return(ERR);
		}
	if (fl_ini==YES && tipo!='S')
		{
		printf("\nParagraph S_ is missing!\n\n");
		fclose(pfile);
		return(ERR);
		}
	if (tipo=='S')
		{
		if ((point_set=malloc(sizeof(struct Poly_set)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			fclose(pfile);
			return(ERR);
			}

		point_set->testa_def=NULL;	/* init pointer definizione pezzi */
		point_set->testa_field=NULL;	/* init pointer definizione fields */
		point_set->next=NULL;		/* init prossimo set */

		point_set->Poly_name=name;

		point_set->n_pez=0;		/* init numero pezzi in set */
	
		pun_def=&point_set->testa_def;
		pun_field=&point_set->testa_field;

		if (fl_ini==YES)
			{
			Begin=point_set;
			fl_ini=NO;
			}
		else pun_set->next=point_set;

		pun_set=point_set;
		}
	if (tipo=='P')
		{
		if ((*pun_def=malloc(sizeof(struct Poly_def)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			fclose(pfile);
			return(ERR);
			}
		if (((*pun_def)->testa=malloc(sizeof(struct Pezzo)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			fclose(pfile);
			return(ERR);
			}
		sscanf(name,"%d",&num_pez);
		(*pun_def)->Poly_num=num_pez;
		(*pun_def)->testa->nr=nr;
		(*pun_def)->testa->nc=nc;
 		(*pun_def)->testa->pezzo=mat;
 		(*pun_def)->testa->next=NULL;
		(*pun_def)->next=NULL;
		pun_def=&(*pun_def)->next;
		point_set->n_pez++;
		}
	if (tipo=='F')
		{
		if ((*pun_field=malloc(sizeof(struct Poly_field)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			fclose(pfile);
			return(ERR);
			}
		(*pun_field)->Field_name=name;
		(*pun_field)->nr=nr;
		(*pun_field)->nc=nc;
 		(*pun_field)->field=mat;
		(*pun_field)->next=NULL;
		pun_field=&(*pun_field)->next;
		}
	if (esito==END)
		{
		fclose(pfile);
		return(OKAY);
		}
	}
return(OKAY);
}



/********************************/
/*				*/
/*          Dec_par		*/
/*				*/
/********************************/

/* Decodifica paragrafo */

/* INPUT:	pf		file descriptor		*/
/*		tip_par		tipo paragrafo:		*/
/*				S			*/
/*				P			*/
/*				F			*/
/*		name_par	pointer nome paragrafo	*/
/*		nc		numero colonne		*/
/*		nr		numero righe		*/
/*		dati		matrice dati		*/
/* OUTPUT:	OKAY					*/
/*		ERR		sintassi errata		*/
/*		END		fine file		*/

int Dec_par(FILE *pf,char *tip_par,char **name_par,int *nr,int *nc,int ***dati)
{
char riga[256];
unsigned char i;
unsigned char j;
char a[256][256];
char ch1;
char ch2;
int posiz;
char *esito;

*nr=0;
*nc=0;
*dati=NULL;

while (fgets(riga,sizeof(riga),pf))
	{
	i=0;
	while ((ch1=riga[i++])==' ');
	if (ch1=='\0') continue;
	ch2=riga[i++];
	if (ch1=='/' && ch2=='/') continue;	/* test commento */
	if ((ch1=='\n' && ch2=='\0') ||
		(ch1=='\r' && ch2=='\n')) continue;
	if (ch1!='[')
		return(ERR);
	ch1=riga[i++];
	if (ch1!='_')
		return(ERR);

	if ((*name_par=malloc(strlen(&riga[i])+1))==NULL)
		{
		printf("\nNot enough memory!\n\n");
		return(ERR);
		}

	j=0;
	while ((ch1=riga[i++])!=']' && ch1!= '\r' && ch1!='\n' && ch1!='\0')
		(*name_par)[j++]=ch1;
	(*name_par)[j]='\0';
	*tip_par=ch2;
	posiz=ftell(pf);
	while ((esito=fgets(riga,sizeof(riga),pf)))
		{
		i=0;
		while ((ch1=riga[i++])==' ');
		if (ch1=='\0')
			{
			posiz=ftell(pf);
			continue;
			}
		ch2=riga[i++];
		if (ch1=='/' && ch2=='/')	/* test commento */
			{
			posiz=ftell(pf);
			continue;
			}
		if ((ch1=='\n' && ch2=='\0') ||
			(ch1=='\r' && ch2=='\n'))
			{
			posiz=ftell(pf);
			continue;
			}
		if (ch1=='.' || ch1=='@')
			{
			i--;
			i--;
			j=0;
			while ((ch1=riga[i++])!='\n' && ch1!='\r' && ch1!='\0')
				{
				if (ch1=='@') a[*nr][j++]=1;
				else if (ch1=='.') a[*nr][j++]=0;
					else if (ch1==' ') continue;
						else
							{
							printf("\nSyntax error!\n");
							return(ERR);
							}
				}
			posiz=ftell(pf);
			if (*nc==0) *nc=j;
			else
				if (*nc!=j)
					{
					printf("\nNot a rectangle!\n\n");
					return(ERR);
					}
			(*nr)++;
			}
		else	break;
		}
	if (*nr && *nc)
		{
		if ((*dati=malloc((*nr)*sizeof(int **)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			return(ERR);
			}
		for (i=0;i<*nr;i++)
			{
			if (((*dati)[i]=malloc((*nc)*sizeof(int)))==NULL)
				{
				printf("\nNot enough memory!\n\n");
				return(ERR);
				}
			for (j=0;j<*nc;j++)
				(*dati)[i][j]=a[i][j];
			}
		}
	if (esito)
		{
		fseek(pf,posiz,0);
		return(OKAY);
		}
	else return(END);
	}
return(END);
}


/********************************/
/*				*/
/*	    CreateClass		*/
/*				*/
/********************************/

/* Genera le classi per i pezzi dei vari set */


void CreateClass(void)
{

struct Poly_set *pun_set;
struct Poly_def *pun_def;
struct Pezzo	*pun_pezzo;

pun_set=Begin;

while (pun_set)
	{
	pun_def=pun_set->testa_def;
	while (pun_def)
		{
		pun_pezzo=pun_def->testa;
		MakeClasses(pun_pezzo);
		pun_def=pun_def->next;
		}
	pun_set=pun_set->next;
	}

}

/********************************/
/*				*/
/*	    MakeClasses		*/
/*				*/
/********************************/

/* Genera le varie classi del pezzo */

/* INPUT:	testa		pointer pezzo */

void MakeClasses(struct Pezzo *testa)
{
int max_dim;
int **mat;
int **mat1;
int nr;
int nc=0;
int trasf;
int ugu;
struct Pezzo *pun_pez;
struct Pezzo *punt;
struct Pezzo *ulti_pun=NULL;

if ((pun_pez=malloc(sizeof(struct Pezzo)))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	exit(1);
	}

max_dim = (testa->nr > testa->nc) ? testa->nr : testa->nc;

if ((mat=malloc(max_dim*sizeof(int **)))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	exit(1);
	}

for (nr=0;nr<max_dim;nr++)
	if ((mat[nr]=malloc(max_dim*sizeof(int)))==NULL)
		{
		printf("\nNot enough memory!\n\n");
		exit(1);
		}

pun_pez->pezzo=mat;

for (trasf=0;trasf<7;trasf++)
	{
	switch (trasf)
		{
		case 0:		pun_pez->nr=testa->nc;
				pun_pez->nc=testa->nr;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[nc][testa->nr-nr-1]=testa->pezzo[nr][nc];
				break;
		case 1:		pun_pez->nr=testa->nr;
				pun_pez->nc=testa->nc;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[testa->nr-nr-1][testa->nc-nc-1]=testa->pezzo[nr][nc];
				break;
		case 2:		pun_pez->nr=testa->nc;
				pun_pez->nc=testa->nr;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[testa->nc-nc-1][nr]=testa->pezzo[nr][nc];
				break;
		case 3:		pun_pez->nr=testa->nr;
				pun_pez->nc=testa->nc;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[nr][testa->nc-nc-1]=testa->pezzo[nr][nc];
				break;
		case 4:		pun_pez->nr=testa->nc;
				pun_pez->nc=testa->nr;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[testa->nc-nc-1][testa->nr-nr-1]=testa->pezzo[nr][nc];
				break;
		case 5:		pun_pez->nr=testa->nr;
				pun_pez->nc=testa->nc;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[testa->nr-nr-1][nc]=testa->pezzo[nr][nc];
				break;
		case 6:		pun_pez->nr=testa->nc;
				pun_pez->nc=testa->nr;
				for (nr=0;nr<testa->nr;nr++)
					for (nc=0;nc<testa->nc;nc++)
						pun_pez->pezzo[nc][nr]=testa->pezzo[nr][nc];
				break;
		}

	ugu=0;

	punt=testa;

	while (punt)
		{
		if (punt->nr==pun_pez->nr && punt->nc==pun_pez->nc)
			{
			for (nr=0;nr<punt->nr;nr++)
				for (nc=0;nc<punt->nc;nc++)
					if (punt->pezzo[nr][nc]!=pun_pez->pezzo[nr][nc])
						{
						nr=punt->nr-1;
						break;
						}
			if (nr==punt->nr && nc==punt->nc)
				{
				ugu=1;
				break;
				}
			}
		ulti_pun=punt;
		punt=punt->next;
		}
	if (ugu==0)
		{
		if ((ulti_pun->next=malloc(sizeof(struct Pezzo)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			exit(1);
			}
		
		if ((mat1=malloc(pun_pez->nr*sizeof(int **)))==NULL)
			{
			printf("\nNot enough memory!\n\n");
			exit(1);
			}
		
		for (nr=0;nr<pun_pez->nr;nr++)
			if ((mat1[nr]=malloc((pun_pez->nc)*sizeof(int)))==NULL)
				{
				printf("\nNot enough memory!\n\n");
				exit(1);
				}

		ulti_pun->next->nr=pun_pez->nr;
		ulti_pun->next->nc=pun_pez->nc;
		ulti_pun->next->pezzo=mat1;
		ulti_pun->next->next=NULL;

		for (nr=0;nr<pun_pez->nr;nr++)
			for (nc=0;nc<pun_pez->nc;nc++)
				ulti_pun->next->pezzo[nr][nc]=pun_pez->pezzo[nr][nc];

		}
	}

free(pun_pez);
for (nr=0;nr<max_dim;nr++) free(mat[nr]);
free(mat);
}


/********************************/
/*				*/
/*	    TesPez		*/
/*				*/
/********************************/

/* Test pezzo inseribile */

/* INPUT:	pun_pez		pointer struttura pezzo		*/
/*        	pun_field	pointer struttura field		*/
/*		Nr		numero riga			*/
/*		Nc		numero colonna			*/
/* OUTPUT:	YES		pezzo inseribile		*/
/* OUTPUT:	NO		pezzo non inseribile		*/

int TesPez(struct Pezzo *pun_pez,struct Poly_field *pun_field,char Nr,char Nc)
{
int nr;
int nc;

int nrz=0;

for (nc=0;nc<pun_pez->nc;nc++)
	if (pun_pez->pezzo[0][nc])
		{
		nrz=nc;
		break;
		}

if (Nc-nrz<0) return(NO);

if (pun_pez->nr+Nr>pun_field->nr || pun_pez->nc+Nc-nrz>pun_field->nc) return(NO);

for (nr=0;nr<pun_pez->nr;nr++)
	for (nc=0;nc<pun_pez->nc;nc++)
		if (pun_pez->pezzo[nr][nc] && pun_field->field[Nr+nr][Nc+nc-nrz])
			return(NO);
return(YES);
}


/********************************/
/*				*/
/*	    InsPez		*/
/*				*/
/********************************/

/* Inserimento pezzo in field */

/* INPUT:	p_def		pointer struttura pezzo */
/*		p_pez		pointer pezzo da inserire */
/*		Nr		numero riga di inserimento */
/*		Nc		numero colonna di inserimento */
/*		p_field		pointer struttura field */
/*		stk		pointer stack */
/*		ind_stk		pointer indice stack */
/*		nr_pezzo	numero pezzo */
/*		nr_classe	numero classe */

/* OUTPUT:	YES		trovati nuovi nr,nc */


int InsPez(struct Poly_def *p_def,struct Pezzo *p_pez,int *Nr,int *Nc,struct Poly_field *p_field,
		struct Stack *stk,int *ind_stk,int nr_pezzo,int nr_classe)
{
int nr;
int nc;

int nrz=0;

for (nc=0;nc<p_pez->nc;nc++)
	if (p_pez->pezzo[0][nc])
		{
		nrz=nc;
		break;
		}

for (nr=0;nr<p_pez->nr;nr++)
	for (nc=0;nc<p_pez->nc;nc++)
		if (p_pez->pezzo[nr][nc])
			p_field->field[*Nr+nr][*Nc+nc-nrz]=p_def->Poly_num+2;
p_def->inserito=YES;

stk[*ind_stk].Nr=*Nr;
stk[*ind_stk].Nc=*Nc;
stk[*ind_stk].pun_def=p_def;
stk[*ind_stk].pun_pez=p_pez;
stk[*ind_stk].nr_pezzo=nr_pezzo;
stk[*ind_stk].nr_classe=nr_classe;
(*ind_stk)++;

for (nr=0;nr<p_field->nr;nr++)
	for (nc=0;nc<p_field->nc;nc++)
		if (p_field->field[nr][nc]==0)
			{
			*Nr=nr;
			*Nc=nc;
			return(YES);
			}
return(NO);
}

/********************************/
/*				*/
/*	    RemPez		*/
/*				*/
/********************************/


/* Rimozione pezzo da field */

/* INPUT:	p_def		pointer struttura pezzo */
/*		p_pez		pointer pezzo da rimuovere */
/*		p_field		pointer struttura field */
/*		ind_stk		pointer indice stack */
/*		nr_pezzo	pointer numero pezzo */
/*		nr_classe	pointer numero pezzo */

/* OUTPUT:	ERR		Stack vuoto */


int RemPez(struct Poly_def **p_def,struct Pezzo **p_pez,struct Poly_field *p_field,
			struct Stack *stk,int *ind_stk,int *Nr,int *Nc,int *nr_pezzo,int *nr_classe)
{
int nr;
int nc;

int nrz=0;

if (*ind_stk==0) return(ERR);

(*ind_stk)--;

*Nr=stk[*ind_stk].Nr;
*Nc=stk[*ind_stk].Nc;
*p_def=stk[*ind_stk].pun_def;
*nr_pezzo=stk[*ind_stk].nr_pezzo;
*p_pez=stk[*ind_stk].pun_pez;
*nr_classe=stk[*ind_stk].nr_classe;

stk[*ind_stk].Nr=0;
stk[*ind_stk].Nc=0;
stk[*ind_stk].pun_def=NULL;
stk[*ind_stk].pun_pez=NULL;
stk[*ind_stk].nr_pezzo=0;
stk[*ind_stk].nr_classe=0;

for (nc=0;nc<(*p_pez)->nc;nc++)
	if ((*p_pez)->pezzo[0][nc])
		{
		nrz=nc;
		break;
		}

for (nr=0;nr<(*p_pez)->nr;nr++)
	for (nc=0;nc<(*p_pez)->nc;nc++)
		if ((*p_pez)->pezzo[nr][nc])
			p_field->field[*Nr+nr][*Nc+nc-nrz]=0;
(*p_def)->inserito=NO;

return(OKAY);
}



/********************************/
/*				*/
/*	    InitSearch		*/
/*				*/
/********************************/

/* Init ricerca */

/* INPUT:	s_nm		nome set					*/
/*		f_nm		nome field					*/
/*		*frst		pointer testa struttura relativa al set		*/
/*		*fld		pointer struttura field trovata			*/
/*		stack		pointer testa vettore stack			*/
/*		ind_stk		pointer indice stack				*/
/*		pila		pointer stack per ricerca, isle			*/
/*		npez		nr. pezzi del set				*/
/* OUTPUT:	OKAY								*/
/*		ERR		not found!					*/

int InitSearch(char *s_nm,char *f_nm,struct Poly_def **frst,struct Poly_field **fld,
			struct Stack **stack,int *ind_stk,char **pila,int *npez)
{
struct Poly_set *pset;
struct Poly_field *pfield;
struct Poly_def *pdef;
int nr;
int nc;
int nset;
int nfield;

nset=0;

pset=Begin;

while (pset)
	{
	if (!strcmp(pset->Poly_name,s_nm))		/* trovato set */
		{
		*npez=pset->n_pez;
		sel_set=nset;
		*frst=pset->testa_def;
		pfield=pset->testa_field;
		nfield=0;
		while (pfield)
			{
			if (!strlen(f_nm) || !strcmp(pfield->Field_name,f_nm))	/* trovato field */
										/* se f_nm e' vuoto fa'
										   il match col primo */
				{
				strcpy(f_nm,pfield->Field_name);
				sel_field=nfield;
				if ((*fld=malloc(sizeof(struct Poly_field)))==NULL)
					{
					printf("\nNot enough memory!\n\n");
					return(ERR);
					}
				(*fld)->nr=pfield->nr;
				(*fld)->nc=pfield->nc;
				(*fld)->next=NULL;
				if (((*fld)->Field_name=
					malloc(strlen(pfield->Field_name)+1))==NULL)
					{
					printf("\nNot enough memory!\n\n");
					return(ERR);
					}
				strcpy((*fld)->Field_name,pfield->Field_name);
				if (((*fld)->field=
					malloc(((*fld)->nr)*sizeof(int **)))==NULL)
					{
					printf("\nNot enough memory!\n\n");
					return(ERR);
					}
				for (nr=0;nr<(*fld)->nr;nr++)
					{
					if (((*fld)->field[nr]=
							malloc(((*fld)->nc)*sizeof(int)))==NULL)
						{
						printf("\nNot enough memory!\n\n");
						return(ERR);
						}
					for (nc=0;nc<(*fld)->nc;nc++)
						(*fld)->field[nr][nc]=
							pfield->field[nr][nc];
					}
				if ((*stack=malloc(pset->n_pez*sizeof(struct Stack)))==NULL)
					{
					printf("\nNot enough memory!\n\n");
					return(ERR);
					}
				*ind_stk=0;
				if ((*pila=malloc((*fld)->nr*(*fld)->nc))==NULL)
					{
					printf("\nNot enough memory!\n\n");
					return(ERR);
					}
				pdef=*frst;
				while (pdef)
					{
					pdef->inserito=NO;
					pdef=pdef->next;
					}
				solutions.Poly_name=s_nm;
				solutions.Field_name=f_nm;
				solutions.nr=pfield->nr;
				solutions.nc=pfield->nc;
				solutions.testa=NULL;
				solutions.coda=NULL;
				solview=NULL;
				return(OKAY);
				}
			pfield=pfield->next;
			nfield++;
			}
		printf("\nfield non trovato!\n");
		return(ERR);
		}
	pset=pset->next;
	nset++;
	}
printf("\nset non trovato\n");
return(ERR);
}



/********************************/
/*				*/
/*	    EndSearch		*/
/*				*/
/********************************/

/* Dealloca ram allocata da InitSearch */

/* INPUT:	fld		pointer struttura field				*/
/*		stack		pointer testa vettore stack			*/
/*		pila		pointer pila per ricerca isle			*/

void EndSearch(struct Poly_field *fld,struct Stack *stack,char *pila)
{
int nr;

struct Sol *sol;
struct Sol *sol1;

free(fld->Field_name);
for (nr=0;nr<fld->nr;nr++) free(fld->field[nr]);
free(fld->field);
free(fld);
free(stack);
free(pila);

sol=solutions.testa;

while (sol)
	{
	for (nr=0;nr<solutions.nr;nr++) free(sol->field[nr]);
	free(sol->field);
	sol1=sol->next;
	free(sol);
	sol=sol1;
	}
}





/********************************/
/*				*/
/*	    GenIsle		*/
/*				*/
/********************************/

/* INPUT:	p_field		pointer struttura field	*/
/*		mark		valore marker		*/
/*		pila		pointer stack da usare	*/
/* OUTPUT:	END		se field pieno		*/

int GenIsle(struct Poly_field *p_field,int mark,char *pila)
{
int nr;
int nc;
int ind_pila;
int dir;

nr=0;
nc=0;
ind_pila=0;
dir=0;

while (p_field->field[nr][nc])
	{
	nc++;
	if (nc==p_field->nc)
		{
		nc=0;
		nr++;
		if (nr==p_field->nr) return(END);
		}
	}

p_field->field[nr][nc]=mark;

while (1)
	{
	while (dir<4)
		switch (dir)
			{
			case 0:
				if (nc==p_field->nc-1)
					{
					dir++;
					break;
					}
				if (p_field->field[nr][nc+1])
					{
					dir++;
					break;
					}
				nc++;
				p_field->field[nr][nc]=mark;
				pila[ind_pila]=dir;
				ind_pila++;
				break;
			case 1:	
				if (nr==p_field->nr-1)
					{
					dir++;
					break;
					}
				if (p_field->field[nr+1][nc])
					{
					dir++;
					break;
					}
				nr++;
				p_field->field[nr][nc]=mark;
				pila[ind_pila]=dir;
				ind_pila++;
				dir=0;
				break;
			case 2:
				if (nc==0)
					{
					dir++;
					break;
					}
				if (p_field->field[nr][nc-1])
					{
					dir++;
					break;
					}
				nc--;
				p_field->field[nr][nc]=mark;
				pila[ind_pila]=dir;
				ind_pila++;
				dir=0;
				break;
			case 3:
				if (nr==0)
					{
					dir++;
					break;
					}
				if (p_field->field[nr-1][nc])
					{
					dir++;
					break;
					}
				nr--;
				p_field->field[nr][nc]=mark;
				pila[ind_pila]=dir;
				ind_pila++;
				dir=0;
				break;
			}

	if (!ind_pila) return(OKAY);
	ind_pila--;
	dir=pila[ind_pila];

	switch (dir)
		{
		case 0:	nc--;
			break;
		case 1:	nr--;
			break;
		case 2:	nc++;
			break;
		case 3:	nr++;
			break;
		}
	dir++;
	}
}



/********************************/
/*				*/
/*	    Isle		*/
/*				*/
/********************************/

/* INPUT:	p_field		pointer struttura field	*/
/*		pila		pointer stack da usare	*/
/*		congr		valore di congruenza	*/
/* OUTPUT:	OKAY/ERR	se field OKAY		*/

int Isle(struct Poly_field *p_field,char *pila,int *congr)
{
int mark;
int cnt_mark;
int esito;
int nr;
int nc;

if (*congr==-1) return(OKAY);

esito=OKAY;

mark=MARK;

while (GenIsle(p_field,mark,pila)!=END)
	{
	cnt_mark=conta_mark(p_field,mark);
	if (cnt_mark%*congr)
		{
		esito=ERR;
		break;
		}
	mark--;
	}

for (nc=0;nc<p_field->nc;nc++)
	for (nr=0;nr<p_field->nr;nr++)
		if (p_field->field[nr][nc]<=MARK) p_field->field[nr][nc]=0;
return(esito);
}


/********************************/
/*				*/
/*	    conta_mark		*/
/*				*/
/********************************/

/* Conta i mark in field */

/* INPUT:	p_field		pointer struttura field	*/
/*		mark		valore di mark		*/
/* OUTPUT:	nr. mark trovati			*/

int conta_mark(struct Poly_field *p_field,int mark)
{
int nr;
int nc;
int cnt_mark;

cnt_mark=0;

for (nc=0;nc<p_field->nc;nc++)
	for (nr=0;nr<p_field->nr;nr++)
		if (p_field->field[nr][nc]==mark) cnt_mark++;
return(cnt_mark);
}


/********************************/
/*				*/
/*	    GetCongr		*/
/*				*/
/********************************/

/* INPUT:	p_tpez		pointer struttura definizione pezzi	*/
/* OUTPUT:	valore di congruenza 					*/

int GetCongr(struct Poly_def *p_tpez)
{
int nr;
int nc;
int cnt;
int congr;

congr=-1;

while (p_tpez)
	{
	cnt=0;
	for (nc=0;nc<p_tpez->testa->nc;nc++)
		for (nr=0;nr<p_tpez->testa->nr;nr++)
			if (p_tpez->testa->pezzo[nr][nc]) cnt++;
	if (congr==-1) congr=cnt;
	else
		if (congr!=cnt) return(-1);
	p_tpez=p_tpez->next;
	}
return(congr);
}


/********************************/
/*				*/
/*	    CaricaSol		*/
/*				*/
/********************************/

/* Salva soluzione	*/

/* INPUT:	p_field		pointer struttura field		*/
/* OUTPUT:	-1		se soluzione non derivata 	*/
/*		1,2,..		nr. soluzione da cui deriva	*/

int CaricaSol(struct Poly_field *p_field)
{
int nr;
int nc;
int Nsol;
struct Sol *sol1;
struct Sol **sol2;
Widget widget;

sol1=solutions.testa;
sol2=&solutions.coda;

if (*sol2) sol2=&(*sol2)->next;

Nsol=1;

while (sol1)
	{
	if (ckder==YES && Ugu(p_field->field,sol1->field)) return(Nsol);
	sol1=sol1->next;
	Nsol++;
	}

if ((*sol2=malloc(sizeof(struct Sol)))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	exit(1);
	}

if (((*sol2)->field=malloc((solutions.nr)*sizeof(int **)))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	exit(1);
	}

for (nr=0;nr<solutions.nr;nr++)
	if (((*sol2)->field[nr]=malloc((solutions.nc)*sizeof(int)))==NULL)
		{
		printf("\nNot enough memory!\n\n");
		exit(1);
		}

for (nr=0;nr<solutions.nr;nr++)
	for (nc=0;nc<solutions.nc;nc++)
		(*sol2)->field[nr][nc]=p_field->field[nr][nc];

(*sol2)->next=NULL;

if (nsol==0)	(*sol2)->prev=NULL;
else		(*sol2)->prev=solutions.coda;

solutions.coda=*sol2;

if (nsol==0) solutions.testa=solutions.coda;

if (nsol==0)
	{
	nsolview=1;
	solview=solutions.testa;
	if (stato==LIMBO)
		if ((widget=XtNameToWidget(file_menu,"button_1"))) XtSetSensitive(widget,True);
	}

nsol++;

return(-1);
}


/********************************/
/*				*/
/*	    Ugu			*/
/*				*/
/********************************/

/* INPUT:	field1 field2		field da confrontare	*/
/* OUTPUT:	YES			se sono uguali		*/

int Ugu(int **field1,int **field2)
{
int nr;
int nc;
int n=0;
int **mat;

if ((mat=malloc((solutions.nr)*sizeof(int **)))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	exit(1);
	}
for (nr=0;nr<solutions.nr;nr++)
	if ((mat[nr]=malloc((solutions.nc)*sizeof(int)))==NULL)
		{
		printf("\nNot enough memory!\n\n");
		exit(1);
		}

for (nr=0;nr<solutions.nr;nr++)
	for (nc=0;nc<solutions.nc;nc++)
		mat[solutions.nr-nr-1][solutions.nc-nc-1]=field1[nr][nc];
if (compare(solutions.nr,solutions.nc,mat,field2)) return(YES);

if (solutions.nr==solutions.nc)
	{
	n=solutions.nr;
	for (nr=0;nr<n;nr++)
		for (nc=0;nc<n;nc++)
			mat[nc][n-nr-1]=field1[nr][nc];
	if (compare(n,n,mat,field2)) return(YES);
	for (nr=0;nr<n;nr++)
		for (nc=0;nc<n;nc++)
			mat[n-nc-1][nr]=field1[nr][nc];
	if (compare(n,n,mat,field2)) return(YES);
	}

for (nr=0;nr<solutions.nr;nr++)
	for (nc=0;nc<solutions.nc;nc++)
		mat[solutions.nr-nr-1][nc]=field1[nr][nc];
if (compare(solutions.nr,solutions.nc,mat,field2)) return(YES);

for (nr=0;nr<solutions.nr;nr++)
	for (nc=0;nc<solutions.nc;nc++)
		mat[nr][solutions.nc-nc-1]=field1[nr][nc];
if (compare(solutions.nr,solutions.nc,mat,field2)) return(YES);

if (solutions.nr==solutions.nc)
	{
	for (nr=0;nr<n;nr++)
		for (nc=0;nc<n;nc++)
			mat[nc][nr]=field1[nr][nc];
	if (compare(n,n,mat,field2)) return(YES);
	for (nr=0;nr<n;nr++)
		for (nc=0;nc<n;nc++)
			mat[n-nc-1][n-nr-1]=field1[nr][nc];
	if (compare(n,n,mat,field2)) return(YES);
	}

for (nr=0;nr<solutions.nr;nr++) free(mat[nr]);
free(mat);
return(NO);
}



/********************************/
/*				*/
/*	    compare		*/
/*				*/
/********************************/

/* INPUT:	Nr		numero righe	*/
/*		Nc		numero colonne	*/
/*		mat1		prima matrice	*/
/*       	mat2		seconda matrice */
/* OUTPUT:	YES		se sono uguali	*/

int compare(char Nr,char Nc,int **mat1,int **mat2)
{
int nr;
int nc;

for (nr=0;nr<Nr;nr++)
	for (nc=0;nc<Nc;nc++)
		if (mat1[nr][nc]!=mat2[nr][nc]) return(NO);
return(YES);
}




/********************************/
/*				*/
/*	    Inserimento		*/
/*				*/
/********************************/

/* Eventuale inserimento pezzo */

/* OUTPUT:	0		pezzo non inserito			*/
/*		1		pezzo inserito				*/
/*		2		pezzo inserito e soluzione trovata	*/

int Inserimento()
{
if (!pdef_corr->inserito)
	while (pez_corr)
	{
		if (TesPez(pez_corr,p_field,numrow,numcol))
			{
			if (InsPez(pdef_corr,pez_corr,&numrow,&numcol,p_field,stack,&ind_stk,nr_pez,nr_clas)==NO) return(2);
			if (Isle(p_field,pila_isle,&congr)==ERR)
				{
				RemPez(&pdef_corr,&pez_corr,p_field,stack,&ind_stk,&numrow,&numcol,&nr_pez,&nr_clas);
				pez_corr=pez_corr->next;
				nr_clas++;
				}
			else	return(1);
			}
		else
			{
			pez_corr=pez_corr->next;
			nr_clas++;
			}
	}
return(0);
}


/********************************/
/*				*/
/*	    Rimozione		*/
/*				*/
/********************************/

/* Eventuale rimozione pezzo */

/* OUTPUT:	<>0	se fine ricerca */

int Rimozione()
{
if (RemPez(&pdef_corr,&pez_corr,p_field,stack,&ind_stk,&numrow,&numcol,&nr_pez,&nr_clas)) return(1);

pez_corr=pez_corr->next;
nr_clas++;

return(0);
}


/********************************/
/*				*/
/*	    Init		*/
/*				*/
/********************************/

/* INPUT:	nome			nome base		*/
/* OUTPUT:	<>0			errore			*/

int Init(char *nome)
{
GetDefaults(nome);				/* get eventuali defaults */

/* Carica dati */

printf("\r\nDecoding file %s ....\r\n",dataname);

if (Decode()) return(ERR);

printf("\nDone.\r\n");

printf("\nNow creating class...\r\n");

CreateClass();

printf("\nDone.\r\n");

GenDim(&record);			/* Genera nc_max,nr_max e record */

GenLists();				/* Genera liste sets e fields */

return(OKAY);
}

/********************************/
/*				*/
/*	    Init1		*/
/*				*/
/********************************/

/* INPUT:	nome_set		nome del set di pezzi	*/
/*		nome_field		nome del field		*/
/* OUTPUT:	<>0			errore			*/

int Init1(char *nome_set,char *nome_field)
{
if (InitSearch(nome_set,nome_field,&p_tpez,&p_field,&stack,&ind_stk,&pila_isle,&numpez)==ERR) return(ERR);

congr=GetCongr(p_tpez);

nsol=0;
nsolview=0;
click=0;

pdef_corr=p_tpez;
pez_corr=p_tpez->testa;

nr_pez=0;
nr_clas=0;

numrow=0;
numcol=0;

while (p_field->field[numrow][numcol])
	{
	numcol++;
	if (numcol==p_field->nc)
		{
		numcol=0;
		numrow++;
		if (numrow==p_field->nr) return(ERR);
		}
	}

fase=INSERIMENTO;

return(OKAY);
}





/********************************/
/*				*/
/*	    GenDim		*/
/*				*/
/********************************/

/* Genera nc_max,nr_max (massime dimensioni field) e carica rec */

void GenDim(char **rec)
{
struct Poly_set *pset;
struct Poly_field *pfield;

nr_max=0;
nc_max=0;

npez_max=0;

pset=Begin;

while (pset)
	{
	if (pset->n_pez>npez_max) npez_max=pset->n_pez;

	pfield=pset->testa_field;
	while (pfield)
		{
		if (pfield->nr>nr_max) nr_max=pfield->nr;
		if (pfield->nc>nc_max) nc_max=pfield->nc;
		pfield=pfield->next;
		}
	pset=pset->next;
	}

lenrec=0;

if (nr_max*nc_max*sizeof(int)>lenrec) lenrec=nr_max*nc_max*sizeof(int);

if (npez_max*sizeof(struct Stack)>lenrec) lenrec=npez_max*sizeof(struct Stack);

lenrec=(lenrec/1024+1)*1024;

if ((*rec=malloc(lenrec))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	exit(1);
	}
}



/********************************/
/*				*/
/*	    GenLists		*/
/*				*/
/********************************/

/* Genera liste nomi set e nomi fields di ogni set */

void GenLists(void)
{
int nset;
int nfield;
struct Poly_set *pset;
struct Poly_field *pfield;
int fatto;

nset=0;

pset=Begin;

while (pset)
	{
	nset++;
	pset=pset->next;
	}

Nset=nset;

Nfield=malloc(nset*sizeof(char *));

set_list=(XmStringTable)XtMalloc(Nset*sizeof(XmString *));

for (nset=0;nset<Nset;nset++) set_list[nset]=NULL;

field_list=(XmStringTable *)XtMalloc(nset*sizeof(XmString *));

fatto=NO;

nset=0;

pset=Begin;

while (pset)
	{
	nfield=0;
	pfield=pset->testa_field;
	while (pfield)
		{
		nfield++;
		pfield=pfield->next;
		}
	Nfield[nset]=nfield;
	if (!fatto)
		if (nfield)
			{
			strcpy(name_sel_set,pset->Poly_name);
			strcpy(name_sel_field,pset->testa_field->Field_name);
			fatto=YES;
			}
	field_list[nset]=(XmStringTable)XtMalloc(nfield*sizeof(XmString *));
	nset++;
	pset=pset->next;
	}

for (nset=0;nset<Nset;nset++)
	for (nfield=0;nfield<Nfield[nset];nfield++)
		field_list[nset][nfield]=NULL;


nset=0;

pset=Begin;

while (pset)
	{
	set_list[nset]=XmStringCreateLocalized(pset->Poly_name);
	nfield=0;
	pfield=pset->testa_field;
	while (pfield)
		{
		field_list[nset][nfield]=XmStringCreateLocalized(pfield->Field_name);
		nfield++;
		pfield=pfield->next;
		}
	nset++;
	pset=pset->next;
	}
}


/********************************/
/*				*/
/*	    InitGra		*/
/*				*/
/********************************/

/* Init grafica */

void InitGra(int argc,char *argv[])
{
XmString file;
XmString help;
XmString quit;
XmString quit_acc;
XmString about;
XmString about_acc;
XmString selec;
XmString selset;
XmString selfie;
XmString ReadS;
XmString reads_acc;
XmString WriteS;
XmString writes_acc;
XmString ReSnap;
XmString resnap_acc;
XmString WrSnap;
XmString wrsnap_acc;
Widget widget;
XColor col;
XColor col1;
char *fallback[]={
			"Polymins.*.background:gray76",
		NULL};
	
XInitThreads();

XtSetLanguageProc(NULL,NULL,NULL);

toplevel=XtVaAppInitialize (&app,"Polymins",NULL,0,&argc,argv,fallback,
							NULL);

XtAddEventHandler(toplevel,(EventMask)0,True,_XEditResCheckMessages,NULL);

dsp=XtDisplay(toplevel);
scr=XtScreen(toplevel);

width=WidthOfScreen(scr);
height=HeightOfScreen(scr);
planes=PlanesOfScreen(scr);

if (XAllocNamedColor(dsp,DefaultColormap(dsp,DefaultScreen(dsp)),SFONDO,&col,&col1)) sfondo=col.pixel;
else	sfondo=BlackPixelOfScreen(scr);

if ((XpmReadFileToPixmap(dsp,RootWindowOfScreen(scr),ICONNAME,&pol_icon,NULL,NULL))==XpmSuccess)
			XtVaSetValues(toplevel,	XmNiconPixmap,	pol_icon,
						NULL);


w_dra=nc_max*lpix+2+2*BPIX+2*MW;
h_dra=nr_max*lpix+2+2*BPIX+2*MH;
if (w_dra<(width-BUTWIDTH-20))   w_dra=width-BUTWIDTH-20;
if (h_dra<(height-BUTHEIGHT-20)) h_dra=height-BUTHEIGHT-20;

XtVaSetValues(toplevel,	XmNwidth,		width-12,
                     	XmNheight,		height-28,
                     	XmNgeometry,		"+0+0",
			XmNdeleteResponse,	XmDO_NOTHING,
                     	NULL);


main_w=XtVaCreateManagedWidget("main_window",	xmMainWindowWidgetClass,toplevel,
						NULL);

WM_DELETE_WINDOW=XmInternAtom(dsp,"WM_DELETE_WINDOW",False);
XmAddWMProtocolCallback(toplevel,WM_DELETE_WINDOW,(XtCallbackProc)GestQuit,NULL);

frame=XtVaCreateManagedWidget("frame",xmFrameWidgetClass,main_w,NULL);

form_w=XtVaCreateManagedWidget("form",xmFormWidgetClass,frame,NULL);

file=XmStringCreateLocalized("File");
help=XmStringCreateLocalized("Help");
about=XmStringCreateLocalized("About");
about_acc=XmStringCreateLocalized("Ctrl-A");
quit=XmStringCreateLocalized("Quit");
quit_acc=XmStringCreateLocalized("Ctrl-C");
ReadS=XmStringCreateLocalized("Read solutions");
reads_acc=XmStringCreateLocalized("Ctrl-R");
WriteS=XmStringCreateLocalized("Write solutions");
writes_acc=XmStringCreateLocalized("Ctrl-W");
ReSnap=XmStringCreateLocalized("Read snapshot");
resnap_acc=XmStringCreateLocalized("Ctrl-V");
WrSnap=XmStringCreateLocalized("Write snapshot");
wrsnap_acc=XmStringCreateLocalized("Ctrl-Z");
selec=XmStringCreateLocalized("Selections");
selset=XmStringCreateLocalized("Select set");
selfie=XmStringCreateLocalized("Select field");

menubar=XmVaCreateSimpleMenuBar(main_w,"menubar",
					XmVaCASCADEBUTTON,	file,	'F',
					XmVaCASCADEBUTTON,	selec,	'S',
					XmVaCASCADEBUTTON,	help,	'H',
					NULL);


file_menu=XmVaCreateSimplePulldownMenu(menubar,"file_menu",0,file_cb,
					XmVaPUSHBUTTON,	ReadS,	'R',	"Crtl<Key>r",	reads_acc,
				        XmVaPUSHBUTTON,	WriteS,	'S',	"Crtl<Key>s",	writes_acc,
				        XmVaSEPARATOR,
				        XmVaPUSHBUTTON,	ReSnap,	'V',	"Crtl<Key>v",	resnap_acc,
				        XmVaPUSHBUTTON,	WrSnap,	'Z',	"Crtl<Key>z",	wrsnap_acc,
				        XmVaSEPARATOR,
				        XmVaPUSHBUTTON,	about,	'A',	"Crtl<Key>a",	about_acc,
				        XmVaSEPARATOR,
				        XmVaPUSHBUTTON,	quit,	'Q',	"Ctrl<Key>c",	quit_acc,
				        NULL);

if ((widget=XtNameToWidget(file_menu,"button_1"))) XtSetSensitive(widget,False);
if (option_dlx==YES)
	if ((widget=XtNameToWidget(file_menu,"button_2"))) XtSetSensitive(widget,False);
if ((widget=XtNameToWidget(file_menu,"button_3"))) XtSetSensitive(widget,False);

selec_menu=XmVaCreateSimplePulldownMenu(menubar,"selec_menu",1,sel_cb,
						XmVaPUSHBUTTON,	selset,	'E',	NULL,		NULL,
						XmVaSEPARATOR,
						XmVaPUSHBUTTON,	selfie,	'F',	NULL,		NULL,
						NULL);

help_menu=XmVaCreateSimplePulldownMenu(menubar,"help_menu",2,help_cb,
						XmVaPUSHBUTTON,	help,	'H',	NULL,	NULL,
						NULL);

if ((widget=XtNameToWidget(menubar,"button_2")))
			XtVaSetValues(menubar,XmNmenuHelpWidget,	widget,
					      NULL);

XtManageChild(menubar);

XmStringFree(file);
XmStringFree(about);
XmStringFree(about_acc);
XmStringFree(quit);
XmStringFree(quit_acc);
XmStringFree(help);
XmStringFree(selec);
XmStringFree(selset);
XmStringFree(selfie);
XmStringFree(ReadS);
XmStringFree(reads_acc);
XmStringFree(WriteS);
XmStringFree(writes_acc);
XmStringFree(ReSnap);
XmStringFree(resnap_acc);
XmStringFree(WrSnap);
XmStringFree(wrsnap_acc);


frame0=XtVaCreateManagedWidget("frame1",xmFrameWidgetClass,form_w,
						     XmNshadowType,		XmSHADOW_IN,
						     XmNleftAttachment,		XmATTACH_FORM,
						     XmNrightAttachment,	XmATTACH_FORM,
						     XmNtopAttachment,		XmATTACH_WIDGET,
						     XmNtopWidget,		menubar,
						     NULL);

intest_area=XtVaCreateManagedWidget ("intest area",xmLabelWidgetClass,frame0,NULL);

GestIntest();

frame3=XtVaCreateManagedWidget("frame3",xmFrameWidgetClass,form_w,
					  XmNshadowType,		XmSHADOW_IN,
					  XmNleftAttachment,		XmATTACH_FORM,
					  XmNtopAttachment,		XmATTACH_WIDGET,
					  XmNtopWidget,			intest_area,
					  XmNwidth,			width-BUTWIDTH-20,
					  XmNheight,			height-BUTHEIGHT-20,
					  NULL);

sw=XtVaCreateManagedWidget("scrolled_win",xmScrolledWindowWidgetClass,	frame3,
					  XmNscrollingPolicy,		XmAUTOMATIC,
					  XmNscrollBarDisplayPolicy,	XmAS_NEEDED,
					  NULL);

frame2=XtVaCreateManagedWidget("frame2",xmFrameWidgetClass,form_w,
				XmNshadowType,		XmSHADOW_IN,
				XmNrightAttachment,	XmATTACH_FORM,
				XmNtopAttachment,	XmATTACH_WIDGET,
				XmNtopWidget,		intest_area,
				NULL);

rc=XtVaCreateManagedWidget("rc",xmRowColumnWidgetClass,	frame2,
				XmNnumColumns,		1,
				XmNrightAttachment,	XmATTACH_WIDGET,
				XmNrightWidget,		sw,
				NULL);

frame20=XtVaCreateManagedWidget("frame20",xmFrameWidgetClass,rc,
				XmNshadowType,		XmSHADOW_IN,
				XmNrightAttachment,	XmATTACH_FORM,
				XmNtopAttachment,	XmATTACH_WIDGET,
				XmNtopWidget,		intest_area,
				NULL);

time_area=XtVaCreateManagedWidget ("time area",xmLabelWidgetClass,	frame20,
					XtVaTypedArg,XmNlabelString,XmRString," ",2,
				        NULL);

frame30=XtVaCreateManagedWidget("frame30",xmFrameWidgetClass,rc,
				XmNshadowType,		XmSHADOW_IN,
				XmNrightAttachment,	XmATTACH_FORM,
				XmNtopAttachment,	XmATTACH_WIDGET,
				XmNtopWidget,		frame20,
				NULL);

rc1=XtVaCreateManagedWidget("rc",xmRowColumnWidgetClass,	frame30,
			 	 XmNnumColumns,			1,
				 XmNrightAttachment, 		XmATTACH_WIDGET,
				 XmNrightWidget,		sw,
				 NULL);

frame40=XtVaCreateManagedWidget("frame40",xmFrameWidgetClass,rc,
				XmNshadowType,		XmSHADOW_IN,
				XmNrightAttachment,	XmATTACH_FORM,
				XmNtopAttachment,	XmATTACH_WIDGET,
				XmNtopWidget,		frame30,
				NULL);

form_w1=XtVaCreateManagedWidget("form1",	xmFormWidgetClass,	frame40,
						XmNfractionBase,	4,
						NULL);

frame50=XtVaCreateManagedWidget("frame50",xmFrameWidgetClass,rc,
				XmNshadowType,		XmSHADOW_IN,
				XmNrightAttachment,	XmATTACH_FORM,
				XmNtopAttachment,	XmATTACH_WIDGET,
				XmNtopWidget,		frame40,
				NULL);

form_w2=XtVaCreateManagedWidget("form_w2",	xmFormWidgetClass,	frame50,
						XmNfractionBase,	9,
				 		NULL);

Create_options();
Create_scales();
Create_com();
Create_arrows();


frame1=XtVaCreateManagedWidget("frame1",xmFrameWidgetClass,form_w,
						     XmNshadowType,		XmSHADOW_IN,
					  	     XmNtopAttachment,		XmATTACH_WIDGET,
					  	     XmNtopWidget,		sw,
					  	     XmNrightAttachment,	XmATTACH_FORM,
					  	     XmNleftAttachment,		XmATTACH_FORM,
						     XmNbottomAttachment,	XmATTACH_FORM,
						     NULL);

message_area=XtVaCreateManagedWidget ("message area",xmLabelWidgetClass,frame1,
						     XtVaTypedArg,	XmNlabelString,	XmRString,	"",1,
						     NULL);

drawing_a=XtVaCreateManagedWidget ("drawing_a",xmDrawingAreaWidgetClass,sw,
					     XmNwidth,w_dra,	
					     XmNheight,h_dra,
					     NULL);

XtAddCallback(drawing_a,XmNexposeCallback,call_exp,NULL);

gc=XCreateGC(dsp,RootWindowOfScreen(scr),0,NULL);

pixmap=XCreatePixmap(dsp,RootWindowOfScreen(scr),w_dra,h_dra,DefaultDepthOfScreen(scr));

ReadPixmap();

XtRealizeWidget(toplevel);
XtAppMainLoop(app);
}








/********************************/
/*				*/
/*	    DrawField		*/
/*				*/
/********************************/

/* Visualizza il field */

/* INPUT:	d		drawable		*/
/*		Nr		nr. righe field		*/
/*		Nc;		nr. colonne field	*/

void DrawField(Drawable d,int Nr,int Nc,int **field)
{
int nr;
int nc;
int x;
int y;
int lpiu2;
int lpiu6;
int lpiu8;

lpiu2=lpix+2;
lpiu6=lpiu2+BPIX;
lpiu8=lpiu6+2;

bx=(width-BUTWIDTH-20-GetLato(Nc))/2;
by=(height-BUTHEIGHT-20-GetLato(Nr))/2;

if (bx<=0 || by<=0)
	{
	bx=(w_dra-GetLato(Nc))/2;
	by=(h_dra-GetLato(Nr))/2;
	}
x=bx;
y=by;

for (nr=0;nr<Nr;nr++)
	for (nc=0;nc<Nc;nc++)
		{
		if (nr==0 && nc==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[53],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[52],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[51],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[50],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[80],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[28],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[27],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[26],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[25],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[79],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[3],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[2],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[1],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[0],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[78],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			}
		if (nr==0 && nc==Nc-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[59],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[58],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[34],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[33],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[9],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[8],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				}
			}
		if (nr==Nr-1 && nc==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[67],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[66],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[42],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[41],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[17],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[16],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				}
			}
		if (nr==Nr-1 && nc==Nc-1)
			{
			if (field[nr][nc]==0)
				{
				XCopyArea(dpy,pix[70],d,gc,0,0,lpiu6,lpiu6,x,y);
				continue;
				}
			if (field[nr][nc]==1)
				{
				XCopyArea(dpy,pix[45],d,gc,0,0,lpiu6,lpiu6,x,y);
				continue;
				}
			if (field[nr][nc]>1)
				{
				XCopyArea(dpy,pix[20],d,gc,0,0,lpiu6,lpiu6,x,y);
				continue;
				}
			}
		if (nr==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[57],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[55],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[56],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[54],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[83],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[32],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[30],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[31],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[29],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[82],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[7],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[5],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[6],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[4],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[81],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			}
		if (nr==Nr-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[69],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[68],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[44],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[43],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[19],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[18],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				}
			}
		if (nc==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[63],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[62],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[61],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[60],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[86],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[38],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[37],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[36],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[35],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[85],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[13],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[12],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[11],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[10],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[84],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			}
		if (nc==Nc-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[65],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[64],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[40],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[39],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[15],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[14],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				}
			}
		if (nr!=Nr-1 && nc!=Nc-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[74],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[72],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[73],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[71],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[77],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[49],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[47],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[48],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[46],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[76],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[24],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[22],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[23],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[21],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[75],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			}
		}
}


/********************************/
/*				*/
/*	    GetLato		*/
/*				*/
/********************************/

/* Get lato field in pixel */

/* INPUT:	nel		numero elementi del lato */
/* OUTPUT:	lato		size lato in pixel */

int GetLato(int nel)
{
return(nel*(lpix+2)+2+2*BPIX);
}


/********************************/
/*				*/
/*	    Gestimer		*/
/*				*/
/********************************/

/* Gestione timer */

void Gestimer(XtPointer call_data)
{
int esito;
int click_sec;
//int click_vis;
//int click_snap;
int der;
//XtIntervalId tim;
//int vised;

struct timeval inizio;
struct timeval corrente;

int limite;

vised=NO;

click++;

click_sec=1000/timeout;
click_vis=previs*1000/timeout;
click_snap=presnap*60*1000/timeout;

if (click%click_sec==0) GesTime();

if (autosnap && click%click_snap==0 && autosnp==NO)
	{
	SSnap();
	return;
	}

if (stato!=START || autosnp==YES) return;

tim=XtAppAddTimeOut(app,timeout,(XtTimerCallbackProc)Gestimer,NULL);

gettimeofday(&inizio,NULL);

limite=timeout*duty+inizio.tv_sec*1000+inizio.tv_usec/1000;
	
if (option_dlx==YES)
	return;
	
do
	{
	esito=0;
	while (esito==0 && pdef_corr)
		switch ((esito=Inserimento()))
			{
			case	0:	pdef_corr=pdef_corr->next;
					nr_pez++;
					if (pdef_corr)
						{
						pez_corr=pdef_corr->testa;
						nr_clas=0;
						}
					break;
			case	1:	pdef_corr=p_tpez;
					nr_pez=0;
					pez_corr=p_tpez->testa;
					nr_clas=0;
					break;
			case	2:	pdef_corr=p_tpez;
					nr_pez=0;
					pez_corr=p_tpez->testa;
					nr_clas=0;
					DrawField(win,p_field->nr,p_field->nc,p_field->field);
					der=CaricaSol(p_field);
					ViewSol(der);
					if (der==-1 && save)
						{
						if (AppendSol(TEXT,0) || AppendSol(GRAPHIC,nsol))
							PostDialog(toplevel,XmDIALOG_ERROR,"Write error!",0);
						}
					break;
			}
	if (esito==2 && waitsol)
		{
		stato=SOSP;
		GestIntest();
		XtRemoveTimeOut(tim);
		XtSetSensitive(pushb_start,True);
		XtSetSensitive(pushb_stop,False);
		XtSetSensitive(pushb_reset,True);
		break;
		}
	if (esito==1)
		{
		if (tipovis==CONTINOUS)
			DrawField(win,p_field->nr,p_field->nc,p_field->field);
		if (tipovis==ONTIMEOUT && vised==NO && click%click_vis==0)
			{
			DrawField(win,p_field->nr,p_field->nc,p_field->field);
			vised=YES;
			}
		if (mode==SEMI && stato==START)
			{
			stato=SOSP;
			GestIntest();
			XtRemoveTimeOut(tim);
			XtSetSensitive(pushb_start,True);
			XtSetSensitive(pushb_stop,False);
			XtSetSensitive(pushb_reset,True);
			break;
			}
		}
	if (!pdef_corr)
		{
		if (Rimozione())
			{
			stato=LIMBO;
			GestIntest();
			ViewEnd();
			XtRemoveTimeOut(tim);
			if (save==NO && nsol) SaveSols();
			else sensitive();
			break;
			}
		if (tipovis==CONTINOUS)
			DrawField(win,p_field->nr,p_field->nc,p_field->field);
		if (tipovis==ONTIMEOUT && vised==NO && click%click_vis==0)
			{
			DrawField(win,p_field->nr,p_field->nc,p_field->field);
			vised=YES;
			}
		if (mode==SEMI && stato==START)
			{
			stato=SOSP;
			GestIntest();
			XtRemoveTimeOut(tim);
			XtSetSensitive(pushb_start,True);
			XtSetSensitive(pushb_stop,False);
			XtSetSensitive(pushb_reset,True);
			break;
			}
		}
	gettimeofday(&corrente,NULL);
	}
	while (corrente.tv_sec*1000+corrente.tv_usec/1000<limite);
}


/********************************/
/*				*/
/*	    ReadPixmap		*/
/*				*/
/********************************/

/* Lettura pixmap */

void ReadPixmap(void)
{
int npix;
char name_pix[64];

printf("\nNow reading pixmaps...\r\n");

for (npix=0;npix<NUMPIX;npix++)
	{
	if (sz==LARGE) strcpy(name_pix,N_DIRPIX);
	else	       strcpy(name_pix,S_DIRPIX);
	strcat(name_pix,pixmap_names[npix]);
	if ((XpmReadFileToPixmap(dsp,RootWindowOfScreen(scr),
		name_pix,&pix[npix],NULL,NULL))!=XpmSuccess)
			{
			printf("\nCan't load pixmap!\n");
			exit(1);
			}
	}
printf("\nDone.\r\n");
}


/********************************/
/*				*/
/*	    call_exp		*/
/*				*/
/********************************/

/* Call-back drawing_a */

void call_exp(Widget widget,XtPointer client_data,XtPointer call_data)
{
XmDrawingAreaCallbackStruct *cbs;
XEvent *event;

cbs=(XmDrawingAreaCallbackStruct *)call_data;
event=cbs->event;
dpy=event->xany.display;
win=event->xany.window;

if (cbs->reason == XmCR_EXPOSE)
	{
	if (first==0)
		{
		first=1;
		XSetForeground(dpy,gc,sfondo);
		}
	XFillRectangle(dpy,win,gc,0,0,w_dra,h_dra);
	DrawField(win,p_field->nr,p_field->nc,p_field->field);
	}
}


/********************************/
/*				*/
/*	    file_cb		*/
/*				*/
/********************************/

/* Call-back file */

void file_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	ReadSols();
			break;
	case	1:	SaveSols();
			break;
	case	2:	if (option_dlx==NO)
				RSnap();
			break;
	case	3:	if (option_dlx==NO)
				SSnap();
			break;
	case	4:	GestAbout();
			break;
	case	5:	GestQuit();
			break;
	}
}


/********************************/
/*				*/
/*	    sel_cb		*/
/*				*/
/********************************/

/* Call-back selection */

void sel_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
long int item;
XmString t;

item=(long int)client_data;

switch (item)
	{
	case	0:	list_set=XmCreateSelectionDialog(main_w,"sel_set",NULL,0);
			t=XmStringCreateLocalized("Available Sets");
			XtVaSetValues(list_set, XmNlistItemCount,	Nset,
				     		XmNlistItems,		set_list,
						XmNlistLabelString,	t,
						XmNmustMatch,		True,
						NULL);
			XtSetSensitive(XmSelectionBoxGetChild(list_set,XmDIALOG_HELP_BUTTON),False);
			XmTextSetString(XmSelectionBoxGetChild(list_set,XmDIALOG_TEXT),name_sel_set);
			XtAddCallback(list_set,XmNokCallback,list_set_cb,NULL);
			XtAddCallback(list_set,XmNcancelCallback,list_set_cb,NULL);
			XtAddCallback(list_set,XmNnoMatchCallback,list_set_cb,NULL);
			XmStringFree(t);
			XtManageChild(list_set);
			unsensitive();
			break;
	case	1:	list_field=XmCreateSelectionDialog(main_w,"sel_field",NULL,0);
			t=XmStringCreateLocalized("Available Fields");
			XtVaSetValues(list_field,	XmNlistItemCount,	Nfield[sel_set],
				     			XmNlistItems,		field_list[sel_set],
							XmNlistLabelString,	t,
							XmNmustMatch,		True,
							NULL);
			XtSetSensitive(XmSelectionBoxGetChild(list_field,XmDIALOG_HELP_BUTTON),False);
			XmTextSetString(XmSelectionBoxGetChild(list_field,XmDIALOG_TEXT),name_sel_field);
			XtAddCallback(list_field,XmNokCallback,list_field_cb,NULL);
			XtAddCallback(list_field,XmNcancelCallback,list_field_cb,NULL);
			XtAddCallback(list_field,XmNnoMatchCallback,list_field_cb,NULL);
			XmStringFree(t);
			XtManageChild(list_field);
			unsensitive();
			break;
	}
}


/********************************/
/*				*/
/*	    help_cb		*/
/*				*/
/********************************/

/* Call-back help */

void help_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
}



/********************************/
/*				*/
/*	    vis_cb		*/
/*				*/
/********************************/

/* Call-back scale visualizzazione */

void vis_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
XmScaleCallbackStruct *cbs;

cbs=(XmScaleCallbackStruct *)call_data;

previs=cbs->value;
}


/********************************/
/*				*/
/*	    snap_cb		*/
/*				*/
/********************************/

/* Call-back scale snapshot */

void snap_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
XmScaleCallbackStruct *cbs;

cbs=(XmScaleCallbackStruct *)call_data;

presnap=cbs->value;
}

/********************************/
/*				*/
/*	    shsol_cb		*/
/*				*/
/********************************/

/* Gestione ViewSol */

void shsol_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
if (solutions.testa==NULL)
	{
	PostDialog(toplevel,XmDIALOG_WARNING,"There's no solutions to view!",0);
	return;
	}

solview=solutions.testa;
nsolview=1;

VSol(solview,nsolview);

XtSetSensitive(showsol,False);

if (nsol==1)
	{
	XtSetSensitive(arrow1,False);
	XtSetSensitive(arrow2,False);
	XtSetSensitive(arrow3,False);
	XtSetSensitive(arrow4,False);
	}
if (nsol>1)
	{
	XtSetSensitive(arrow1,True);
	XtSetSensitive(arrow2,False);
	XtSetSensitive(arrow3,True);
	XtSetSensitive(arrow4,False);
	}
}


/********************************/
/*				*/
/*	    up_cb		*/
/*				*/
/********************************/

/* Gestione arrow UP */

void up_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
if (solutions.coda==NULL)
	{
	XtSetSensitive(arrow1,False);
	PostDialog(toplevel,XmDIALOG_WARNING,"There's no solutions to view!",0);
	return;
	}

XtSetSensitive(arrow1,False);
XtSetSensitive(arrow2,True);
XtSetSensitive(arrow3,False);
XtSetSensitive(arrow4,True);

solview=solutions.coda;
nsolview=nsol;

VSol(solview,nsolview);
}


/********************************/
/*				*/
/*	    left_cb		*/
/*				*/
/********************************/

/* Gestione arrow LEFT */

void left_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
XtSetSensitive(arrow1,True);
XtSetSensitive(arrow3,True);

solview=solview->prev;
nsolview--;
VSol(solview,nsolview);

if (solview->prev==NULL)
	{
	XtSetSensitive(arrow2,False);
	XtSetSensitive(arrow4,False);
	}
}


/********************************/
/*				*/
/*	    right_cb		*/
/*				*/
/********************************/

/* Gestione arrow RIGHT */

void right_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
XtSetSensitive(arrow2,True);
XtSetSensitive(arrow4,True);

solview=solview->next;
nsolview++;
VSol(solview,nsolview);

if (solview->next==NULL)
	{
	XtSetSensitive(arrow1,False);
	XtSetSensitive(arrow3,False);
	}
}


/********************************/
/*				*/
/*	    down_cb		*/
/*				*/
/********************************/

/* Gestione arrow DOWN */

void down_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
shsol_cb(widget,client_data,call_data);
}



/********************************/
/*				*/
/*	    VSol		*/
/*				*/
/********************************/

/* Show soluzione */

/* INPUT:	psol		pointer soluzione	*/
/*		ns		nr. soluzione		*/

void VSol(struct Sol *psol,int ns)
{
int nr;
int nc;
char str[128];
char num[16];

for (nr=0;nr<solutions.nr;nr++)
	for (nc=0;nc<solutions.nc;nc++)
		p_field->field[nr][nc]=psol->field[nr][nc];

DrawField(win,p_field->nr,p_field->nc,p_field->field);

strcpy(str,"Solution nr. ");
sprintf(num,"%5d",ns);
strcat(str,num);

XtVaSetValues(message_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	str,strlen(str)+1,
				NULL);
}



/********************************/
/*				*/
/*	    GestAbout		*/
/*				*/
/********************************/

/* Gestione About */

void GestAbout(void)
{
PostDialog(toplevel,XmDIALOG_MESSAGE,"This program was written by Lelli Massimo on August 1997",0);
}



/********************************/
/*				*/
/*	    GestQuit		*/
/*				*/
/********************************/

/* Gestione Quit */

void GestQuit(void)
{
Widget dialog;
Arg arg[1];
XmString t;

if (stato!=LIMBO) return;

t=XmStringCreateLocalized("Really quit?");

XtSetArg(arg[0],XmNmessageString,t);

dialog=XmCreateQuestionDialog(main_w,"quit",arg,1);

unsensitive();

XtUnmanageChild(XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON));

XtAddCallback(dialog,XmNokCallback,ok_pushed,NULL);
XtAddCallback(dialog,XmNcancelCallback,cancel_pushed,NULL);

XtManageChild(dialog);
}


/********************************/
/*				*/
/*	    ok_pushed		*/
/*				*/
/********************************/

/* Gestione okay in Quit */

void ok_pushed(Widget widget,XtPointer client_data,XtPointer call_data)
{
printf("\nBye!\r\n");
exit(0);
}


/********************************/
/*				*/
/*	    cancel_pushed	*/
/*				*/
/********************************/

/* Gestione cancel in Quit */

void cancel_pushed(Widget widget,XtPointer client_data,XtPointer call_data)
{
sensitive();
}


/********************************/
/*				*/
/*	    ge_start		*/
/*				*/
/********************************/

/* Gestione start */

void ge_start(Widget widget,XtPointer client_data,XtPointer call_data)
{
Widget wdg;
int nr;
int nc;
int stato_old;

autosnp=NO;

if (stato==LIMBO || stato==SOSP)
	{
	if (stato==LIMBO)
		{
		EndSearch(p_field,stack,pila_isle);
		if (Init1(name_sel_set,name_sel_field)) exit(1);

		XtVaSetValues(message_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	" ",2,
						NULL);
		for (nr=0;nr<p_field->nr;nr++)
			for (nc=0;nc<p_field->nc;nc++)
				if (p_field->field[nr][nc]<0 || p_field->field[nr][nc]>1)
					p_field->field[nr][nc]=0;
		DrawField(win,p_field->nr,p_field->nc,p_field->field);
		}
	if ((wdg=XtNameToWidget(file_menu,"button_0"))) XtSetSensitive(wdg,False);
	if ((wdg=XtNameToWidget(file_menu,"button_1"))) XtSetSensitive(wdg,False);
	if ((wdg=XtNameToWidget(file_menu,"button_2"))) XtSetSensitive(wdg,False);
	if (option_dlx==NO)
		if ((wdg=XtNameToWidget(file_menu,"button_3"))) XtSetSensitive(wdg,True);
	if ((wdg=XtNameToWidget(file_menu,"button_5"))) XtSetSensitive(wdg,False);
	XtSetSensitive(selec_menu,False);
	XtSetSensitive(pushb_stop,True);
	XtSetSensitive(pushb_start,False);
	XtSetSensitive(pushb_reset,False);
	XtSetSensitive(showsol,False);
	XtSetSensitive(arrow1,False);
	XtSetSensitive(arrow2,False);
	XtSetSensitive(arrow3,False);
	XtSetSensitive(arrow4,False);
	XtSetSensitive(XmOptionButtonGadget(option_menu1),False);
	stato_old=stato;
	stato=START;
	GestIntest();
	Gestimer(NULL);
	if (option_dlx==YES)
		if (stato_old==LIMBO)
			RunThread();
	}
}


/********************************/
/*				*/
/*	    ge_stop		*/
/*				*/
/********************************/

/* Gestione stop */

void ge_stop(Widget widget,XtPointer client_data,XtPointer call_data)
{
if (stato==START)
	{
	stato=SOSP;
	GestIntest();
	XtSetSensitive(pushb_start,True);
	XtSetSensitive(pushb_stop,False);
	XtSetSensitive(pushb_reset,True);
	}
}


/********************************/
/*				*/
/*	    ge_reset		*/
/*				*/
/********************************/

/* Gestione reset */

void ge_reset(Widget widget,XtPointer client_data,XtPointer call_data)
{
Widget wdg;
int nr;
int nc;

if (stato==SOSP)
	{
	stato=LIMBO;
	GestIntest();
	for (nr=0;nr<p_field->nr;nr++)
		for (nc=0;nc<p_field->nc;nc++)
			if (p_field->field[nr][nc]<0 || p_field->field[nr][nc]>1)
				p_field->field[nr][nc]=0;
	DrawField(win,p_field->nr,p_field->nc,p_field->field);
	XtSetSensitive(pushb_reset,False);
	if ((wdg=XtNameToWidget(file_menu,"button_0"))) XtSetSensitive(wdg,True);
	if ((wdg=XtNameToWidget(file_menu,"button_1")) && nsol>0) XtSetSensitive(wdg,True);
	if (option_dlx==NO)
		if ((wdg=XtNameToWidget(file_menu,"button_2"))) XtSetSensitive(wdg,True);
	if ((wdg=XtNameToWidget(file_menu,"button_3"))) XtSetSensitive(wdg,False);
	if ((wdg=XtNameToWidget(file_menu,"button_5"))) XtSetSensitive(wdg,True);
	XtSetSensitive(XmOptionButtonGadget(option_menu1),True);
	XtSetSensitive(selec_menu,True);
	if (nsol>0)
		{
		XtSetSensitive(showsol,True);
		XtSetSensitive(arrow1,False);
		XtSetSensitive(arrow2,False);
		XtSetSensitive(arrow3,False);
		XtSetSensitive(arrow4,False);
		}
	}
}

/********************************/
/*				*/
/*	    option_cb		*/
/*				*/
/********************************/

/* Callback run mode */

void option_cb(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	mode=AUTO;
			break;
	case	1:	mode=SEMI;
			break;
	}
}


/********************************/
/*				*/
/*	    option_cb1		*/
/*				*/
/********************************/

/* Callback check mode */

void option_cb1(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	ckder=YES;
			break;
	case	1:	ckder=NO;
			break;
	}
}


/********************************/
/*				*/
/*	    option_cb2		*/
/*				*/
/********************************/

/* Callback tipo visualizzazione */

void option_cb2(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	if (option_dlx==NO)
				{
				tipovis=CONTINOUS;
				duty=DUTYC;
				}
			else
				Refresh_options();
			break;
	case	1:	tipovis=ONSOL;
			duty=DUTYNC;
			break;
	case	2:	tipovis=ONTIMEOUT;
			duty=DUTYNC;
			break;
	}
}


/********************************/
/*				*/
/*	    option_cb3		*/
/*				*/
/********************************/

/* Callback wait mode */

void option_cb3(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	waitsol=YES;
			break;
	case	1:	waitsol=NO;
			break;
	}
}



/********************************/
/*				*/
/*	    option_cb4		*/
/*				*/
/********************************/

/* Callback save on solution */

void option_cb4(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	save=YES;
			break;
	case	1:	save=NO;
			break;
	}
}


/********************************/
/*				*/
/*	    option_cb5		*/
/*				*/
/********************************/

/* Callback mode autosnap */

void option_cb5(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	autosnap=YES;
			break;
	case	1:	autosnap=NO;
			break;
	}
}


/********************************/
/*				*/
/*	    option_cb6		*/
/*				*/
/********************************/

/* Callback Pixmap save */

void option_cb6(Widget menu_item,XtPointer client_data,XtPointer call_data)
{
long int item;

item=(long int)client_data;

switch (item)
	{
	case	0:	fmt_save=ASCII;
			break;
	case	1:	fmt_save=PIXMAP;
			break;
	case	2:	if (option_dlx==NO)
				fmt_save=JPG;
			else
				Refresh_options();
			break;
	}
}


/********************************/
/*				*/
/*	    PostDialog		*/
/*				*/
/********************************/

/* Out messaggio */

/* INPUT:	parent		widget padre		*/
/*		dialog_type	tipo di dialog widget	*/
/*		msg		messaggio		*/
/*		flag		se <>0 toglie button OK */

Widget PostDialog(Widget parent,int dialog_type,char *msg,int flag)
{
Widget dialog;
XmString text;

if (!flag) unsensitive();

dialog=XmCreateMessageDialog (parent,"dialog",NULL,0);

text=XmStringCreateLocalized (msg);

XtVaSetValues (dialog,XmNdialogType,dialog_type,XmNmessageString,text,NULL);

XmStringFree (text);

if (flag) XtUnmanageChild(XmMessageBoxGetChild(dialog,XmDIALOG_OK_BUTTON));

XtUnmanageChild(XmMessageBoxGetChild(dialog,XmDIALOG_CANCEL_BUTTON));
XtUnmanageChild(XmMessageBoxGetChild (dialog, XmDIALOG_HELP_BUTTON));

if (!flag)	XtAddCallback(dialog,XmNokCallback,Gest_ok_cb,NULL);

XtManageChild(dialog);

return(dialog);
}


/********************************/
/*				*/
/*	    Gest_ok_cb		*/
/*				*/
/********************************/


void Gest_ok_cb(Widget widget,XtPointer client_data,XtPointer  call_data)
{
sensitive();
XtDestroyWidget(widget);
if (autosnp==YES)
	{
	autosnp=NO;
	XtAppAddTimeOut(app,timeout,(XtTimerCallbackProc)Gestimer,NULL);
	}
}




/********************************/
/*				*/
/*	    list_set_cb		*/
/*				*/
/********************************/

/* Callback selezione set */


void list_set_cb(Widget widget,XtPointer client_data,XtPointer  call_data)
{
char msg[256];
char *prompt=NULL;
char *value;
int dialog_type=0;

XmSelectionBoxCallbackStruct *cbs=(XmSelectionBoxCallbackStruct *)call_data; 

switch (cbs->reason)
	{
        case XmCR_OK:		break;
        case XmCR_NO_MATCH:	prompt="Not a valid selection: ";
            			dialog_type=XmDIALOG_ERROR;
            			break;
	case XmCR_CANCEL:	sensitive();
				return;
        default:		prompt="Unknown selection: ";
            			dialog_type=XmDIALOG_ERROR;
				break;
    	}

XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&value);

if (cbs->reason!=XmCR_OK)
	{
	sprintf(msg,"%s%s",prompt,value);
	PostDialog(XtParent(XtParent(widget)),dialog_type,msg,0);
	}
else	
	if (CheckSet(value))
		{
		prompt="No field in the set ";
            	dialog_type=XmDIALOG_ERROR;
		sprintf(msg,"%s%s",prompt,value);
		PostDialog(XtParent(XtParent(widget)),dialog_type,msg,0);
		}
	else
		{
		strcpy(name_sel_set,value);
		strcpy(name_sel_field,"");
		EndSearch(p_field,stack,pila_isle);
		if (Init1(name_sel_set,name_sel_field)) exit(1);
		XFillRectangle(dpy,win,gc,0,0,w_dra,h_dra);
		DrawField(win,p_field->nr,p_field->nc,p_field->field);
		GestIntest();
		sensitive();
		}
XtFree(value);
}

/********************************/
/*				*/
/*	    list_field_cb	*/
/*				*/
/********************************/

/* Callback selezione field */


void list_field_cb(Widget widget,XtPointer client_data,XtPointer  call_data)
{
char msg[256];
char *prompt=NULL;
char *value;
int dialog_type=0;

XmSelectionBoxCallbackStruct *cbs=(XmSelectionBoxCallbackStruct *)call_data; 

switch (cbs->reason)
	{
        case XmCR_OK:		break;
        case XmCR_NO_MATCH:	prompt="Not a valid selection: ";
            			dialog_type=XmDIALOG_ERROR;
            			break;
	case XmCR_CANCEL:	sensitive();
				return;
        default:		prompt="Unknown selection: ";
            			dialog_type=XmDIALOG_ERROR;
				break;
    	}

XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &value);
sprintf (msg, "%s%s", prompt, value);

if (cbs->reason!=XmCR_OK)
	{
	sprintf (msg,"%s%s",prompt,value);
	PostDialog(XtParent(XtParent(widget)),dialog_type,msg,0);
	}
else	
	{
	strcpy(name_sel_field,value);
	EndSearch(p_field,stack,pila_isle);
	if (Init1(name_sel_set,name_sel_field)) exit(1);
	XFillRectangle(dpy,win,gc,0,0,w_dra,h_dra);
	DrawField(win,p_field->nr,p_field->nc,p_field->field);
	GestIntest();
	sensitive();
	}
XtFree(value);
}

/********************************/
/*				*/
/*	    CheckSet		*/
/*				*/
/********************************/

/* Check set set ha almeno un field */

/* OUTPUT:	<>0	no field!	*/

int CheckSet(char *setname)
{
int nset;
struct Poly_set *pset;

nset=0;
pset=Begin;

while (pset)
	{
	if (!strcmp(pset->Poly_name,setname)) return((Nfield[nset]==0) ? 1:0);
	nset++;
	pset=pset->next;
	}
return(1);
}


/********************************/
/*				*/
/*	    GestIntest		*/
/*				*/
/********************************/

/* Gestione intestazione */

void GestIntest(void)
{
char str[128];
char str1[16];

switch (stato)
	{
	case LIMBO:	strcpy(str1,"RESET");
			break;
	case START:	strcpy(str1,"START");
			break;
	case SOSP:	strcpy(str1,"STOP ");
			break;
	}

strcpy(str,"Status: ");
strcat(str,str1);
strcat(str,"             Set name: ");
strcat(str,name_sel_set);
strcat(str,"         Field name: ");
strcat(str,name_sel_field);

XtVaSetValues(intest_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	str,strlen(str)+1,
				NULL);
}



/********************************/
/*				*/
/*	    ViewSol		*/
/*				*/
/********************************/

/* INPUT:	der		-1		se soluzione non derivata (nsol) 	*/
/*				0,1,		soluzione derivata dal numero 0,1,..	*/

void ViewSol(int der)
{
char str[128];
char num[16];

if (der!=-1)
	{
	strcpy(str,"Found a solution! It's derived from solution nr. ");
	sprintf(num,"%5d",der);
	}
else
	{
	strcpy(str,"Found solution nr. ");
	sprintf(num,"%5d",nsol);
	}
strcat(str,num);
XtVaSetValues(message_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	str,strlen(str)+1,
				NULL);
}



/********************************/
/*				*/
/*	    ViewEnd		*/
/*				*/
/********************************/

/* Visualizzazione END */

void ViewEnd(void)
{
char str[128];
char num[16];

strcpy(str,"Search finished!      Nr. solutions ");

if (ckder) strcat(str,"not derived");

strcat(str," founded = ");

sprintf(num,"%d",nsol);
strcat(str,num);

XtVaSetValues(message_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	str,strlen(str)+1,
				NULL);
}


/********************************/
/*				*/
/*	    GenStrTime		*/
/*				*/
/********************************/

/* Genera da click la stringa StrTime */

void GenStrTime(void)
{
int sec;
int min;
int hour;
int day;
int acc;
char num[16];

acc=click*timeout/1000;

sec=acc%60;
acc=acc/60;

min=acc%60;
acc=acc/60;

//hour=acc%60;
//acc=acc/60;

//day=acc%24;

hour=acc%24;
day=acc/24;

strcpy(StrTime,"        Time: ");
if (day<10) strcat(StrTime,"0");
sprintf(num,"%d",day);
strcat(StrTime,num);
strcat(StrTime,":");
if (hour<10) strcat(StrTime,"0");
sprintf(num,"%d",hour);
strcat(StrTime,num);
strcat(StrTime,":");
if (min<10) strcat(StrTime,"0");
sprintf(num,"%d",min);
strcat(StrTime,num);
strcat(StrTime,":");
if (sec<10) strcat(StrTime,"0");
sprintf(num,"%d",sec);
strcat(StrTime,num);

}

/********************************/
/*				*/
/*	    GesTime		*/
/*				*/
/********************************/

/* Gestione visualizzazione tempo */

void GesTime(void)
{
GenStrTime();

XtVaSetValues(time_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	StrTime,strlen(StrTime)+1,
				NULL);
}



/********************************/
/*				*/
/*	    WriteSol		*/
/*				*/
/********************************/

/* Write the solutions to disk */

/* INPUT:	ftype		formato file (TEXT/GRAPHIC)		*/
/* OUTPUT:	OKAY/ERR/NOSAVED					*/

int WriteSol(int ftype)
{
FILE *fp;
struct Sol *sol;
char namesol[128];
int ns;
int nr;
int nc;

if (nsol==0)
	{
	PostDialog(toplevel,XmDIALOG_WARNING,"There's no solutions to save!",0);
	return(NOSAVED);
	}

if (ftype==GRAPHIC)
	{
	if (fmt_save==ASCII) return(OKAY);
	ns=1;
	sol=solutions.testa;
	while (sol)
		{
		for (nr=0;nr<solutions.nr;nr++)
			for (nc=0;nc<solutions.nc;nc++)
				p_field->field[nr][nc]=sol->field[nr][nc];

		if (AppendSol(ftype,ns)) return(ERR);
		sol=sol->next;
		ns++;
		}
	for (nr=0;nr<solutions.nr;nr++)
		for (nc=0;nc<solutions.nc;nc++)
			if (p_field->field[nr][nc]>1) p_field->field[nr][nc]=0;
	return(OKAY);
	}

if (ftype==TEXT)
	{
	GetNameSol(namesol,ftype,0);

	if ((fp=fopen(namesol,"w+"))==NULL) return(ERR);

	IntestSol(fp);

	sol=solutions.testa;

	ns=1;

	while (sol)
		{
		WSol(fp,ns,sol->field,NULL);
		ns++;
		sol=sol->next;
		}
	fclose(fp);
	return(OKAY);
	}

return(OKAY);
}



/********************************/
/*				*/
/*	    SaveSols		*/
/*				*/
/********************************/

/* Salva soluzioni sia in ASCII che in PIXMAP */

void SaveSols(void)
{
unsensitive();
dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Writing solutions in progress... please wait!",1);
XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeSaveSols,&dial);
}

/********************************/
/*				*/
/*	    GeSaveSols		*/
/*				*/
/********************************/

/* Chiamata con timer dalla routine precedente */

void GeSaveSols(XtPointer call_data)
{
Widget dialog;
int esito1;
int esito2=0;

dialog=*(Widget *)call_data;

esito1=WriteSol(TEXT);

if (esito1==OKAY) esito2=WriteSol(GRAPHIC);

XtDestroyWidget(dialog);

if (esito1==NOSAVED)
	{
	sensitive();
	return;
	}

if (esito1==ERR || esito2==ERR) PostDialog(toplevel,XmDIALOG_ERROR,"Write error!",0);
else
	{
	dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Done successfully!",1);
	XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeDestroy,&dial);
	sensitive();
	}
}



/********************************/
/*				*/
/*	    GeDestroy		*/
/*				*/
/********************************/

/* Destroy dialog "Done successfully" */

void GeDestroy(XtPointer call_data)
{
Widget dialog;
dialog=*(Widget *)call_data;
XtDestroyWidget(dialog);
if (autosnp==YES)
	{
	autosnp=NO;
	XtAppAddTimeOut(app,timeout,(XtTimerCallbackProc)Gestimer,NULL);
	}
}


/********************************/
/*				*/
/*	    ReadSols		*/
/*				*/
/********************************/

/* Read soluzioni */

void ReadSols(void)
{
unsensitive();
dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Reading solutions in progress... please wait!",1);
XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeReadSols,&dial);
}

/********************************/
/*				*/
/*	    GeReadSols		*/
/*				*/
/********************************/

/* Chiamata con timer dalla routine precedente */

void GeReadSols(XtPointer call_data)
{
Widget dialog;
int esito;

dialog=*(Widget *)call_data;

esito=ReadSol();

XtDestroyWidget(dialog);

if (esito) PostDialog(toplevel,XmDIALOG_ERROR,"Read error!",0);
else
	{
	dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Done successfully!",1);
	XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeDestroy,&dial);
	sensitive();
	}
}



/********************************/
/*				*/
/*	    AppendSol		*/
/*				*/
/********************************/

/* Write the solutions to disk in append */

/* INPUT:	ftype		formato file (TEXT/GRAPHIC)	*/
/*		ns		numero soluzione (1,2,..	*/
/* OUTPUT:	OKAY/ERR					*/

int AppendSol(int ftype,int ns)
{
char namesol[128];
FILE *fp=NULL;


if (ftype==GRAPHIC)
	{
	switch (fmt_save)
		{
		case ASCII:	break;
		case PIXMAP:	GetNameSol(namesol,ftype,ns);
				SavePixmap(namesol);
				break;
#if USE_JPG
		case JPG:	GetNameSol(namesol,ftype,ns);
				SaveJpeg(namesol);
				break;
#endif
		}
	return(OKAY);
	}

GetNameSol(namesol,ftype,ns);

if (ftype==TEXT)
	{
	if (nsol==1)
		{
		if ((fp=fopen(namesol,"w+"))==NULL) return(ERR);
		IntestSol(fp);
		}
	else
		if ((fp=fopen(namesol,"a"))==NULL) return(ERR);
	}

WSol(fp,nsol,p_field->field,StrTime);

fclose(fp);
return(OKAY);
}




/********************************/
/*				*/
/*	    SavePixmap		*/
/*				*/
/********************************/

/* Save soluzione in formato pixmap */

/* INPUT:	namesol		nome pixmap da salvare */

void SavePixmap(char *namesol)
{
XImage *xim;

DrawField(pixmap,p_field->nr,p_field->nc,p_field->field);
xim=XGetImage(dsp,pixmap,bx,by,GetLato(p_field->nc),GetLato(p_field->nr),0xffffffff,XYPixmap);
XpmWriteFileFromImage(dsp,namesol,xim,NULL,NULL);
XDestroyImage(xim);
}

#if USE_JPG


/********************************/
/*				*/
/*	    SaveJpeg		*/
/*				*/
/********************************/

/* Save soluzione in formato jpeg */

/* INPUT:	namesol		nome jpeg da salvare */

void SaveJpeg(char *namesol)
{
FILE *fp;
XImage *xim;
struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;
JSAMPROW rowsample[1];
char *image;
int dep;
int prel;
int nr;
int nc;
int appo;

if ((fp=fopen(namesol,"wb+"))==NULL)
	{
	PostDialog(toplevel,XmDIALOG_ERROR,"Can't write Jpeg file!",0);
	return;
	}

DrawField(pixmap,p_field->nr,p_field->nc,p_field->field);
xim=XGetImage(dsp,pixmap,bx,by,GetLato(p_field->nc),GetLato(p_field->nr),0xffffffff,ZPixmap);

if ((image=malloc(xim->height*xim->width*3))==NULL)
	{
	printf("\nNot enough memory!\n\n");
	XDestroyImage(xim);
	fclose(fp);
	exit(1);
	}

dep=0;
prel=0;

for (nr=0;nr<xim->width;nr++)
	for (nc=0;nc<xim->height;nc++)
		{
		appo=xim->data[prel];
		image[dep+1]=xim->data[prel+1];
		image[dep]=xim->data[prel+2];
		image[dep+2]=appo;
		dep+=3;
		prel+=3;
		if (nc==xim->height-1) prel+=xim->bytes_per_line-xim->width*3;
		}

cinfo.err=jpeg_std_error(&jerr);
jpeg_create_compress(&cinfo);

jpeg_stdio_dest(&cinfo,fp);

cinfo.image_width=xim->width; 
cinfo.image_height=xim->height;
cinfo.input_components=3;
cinfo.in_color_space=JCS_RGB;

jpeg_set_defaults(&cinfo);

jpeg_set_quality(&cinfo,100,TRUE);

jpeg_start_compress(&cinfo,TRUE);

rowsample[0]=(JSAMPROW)&xim->data[cinfo.next_scanline*3*xim->width];

while (cinfo.next_scanline<cinfo.image_height)
	{
	rowsample[0]=(JSAMPROW)&image[cinfo.next_scanline*3*xim->width];
    	jpeg_write_scanlines(&cinfo,rowsample,1);
	}

jpeg_finish_compress(&cinfo);
jpeg_destroy_compress(&cinfo);

XDestroyImage(xim);

free(image);
fclose(fp);

}

#endif


/********************************/
/*				*/
/*	    IntestSol		*/
/*				*/
/********************************/

/* INPUT:	fp	descrittore file */

/* Genera intestazione file delle soluzioni */

void IntestSol(FILE *fp)
{
fprintf(fp,NOMESET);
fprintf(fp,"\n");
fprintf(fp,"%s\n\n",name_sel_set);
fprintf(fp,NOMEFIELD);
fprintf(fp,"\n");
fprintf(fp,"%s\n\n",name_sel_field);
fprintf(fp,NRFIELD);
fprintf(fp,"\n");
fprintf(fp,"%d\n\n",solutions.nr);
fprintf(fp,NCFIELD);
fprintf(fp,"\n");
fprintf(fp,"%d\n\n\n\n",solutions.nc);
}



/********************************/
/*				*/
/*	    GetNameSol		*/
/*				*/
/********************************/

/* Genera nome file soluzione */

/* INPUT:	namesol		pointer nome file soluzione 	*/
/*       	ftype		formato file (TEXT/GRAPHIC)	*/
/*		ns		nr. soluzione (1,2,..)		*/

void GetNameSol(char *namesol,int ftype,int ns)
{
char num[16];

strcpy(namesol,DIRSOL);
strcat(namesol,name_sel_set);
strcat(namesol,"-[");
strcat(namesol,name_sel_field);
strcat(namesol,"]");
	
if (ckder==YES)
	strcat(namesol,"_nosymm");
	
switch (ftype)
	{
	case TEXT:	strcat(namesol,".txt");
			break;
	case GRAPHIC:	strcat(namesol,"-");
			sprintf(num,"%d",ns);
			strcat(namesol,num);

			switch (fmt_save)
				{
				case PIXMAP:	strcat(namesol,".xpm");
						break;
				case JPG:	strcat(namesol,".jpg");
						break;
				}
			break;

	}
}


/********************************/
/*				*/
/*	    WSol		*/
/*				*/
/********************************/

/* Write di una soluzione */

/* INPUT:	fp		descrittore file */
/*		numsol		numero soluzione */
/*		field		soluzione	 */
/*		ptime		pointer time	 */

void WSol(FILE *fp,int numsol,int **field,char *ptime)
{
int nr;
int nc;
int count;

if (ptime)	fprintf(fp,"SOLUZIONE NR. %d     %s\n\n",numsol,ptime);
else		fprintf(fp,"SOLUZIONE NR. %d\n\n",numsol);

for (nr=0;nr<solutions.nr;nr++)
	for (count=0;count<1;count++)
		{
		for (nc=0;nc<solutions.nc;nc++)
			{
			switch (field[nr][nc])
				{
				case	0:	fprintf(fp,"   .");
						break;
				case	1:	fprintf(fp,"   @");
						break;
				default:	fprintf(fp,"%4d",field[nr][nc]-2);
						break;
				}
			}
		fprintf(fp,"\n");
		}
fprintf(fp,"\n\n\n");
}


/********************************/
/*				*/
/*	    ReadSol		*/
/*				*/
/********************************/


/* Get soluzioni da file */

int ReadSol(void)
{
FILE *fp;
char namesol[128];
char riga[LENRIGA];
int nr;
int nc;
int appo;
int ind;
int flag;

flag=0;

GetNameSol(namesol,TEXT,0);

if ((fp=fopen(namesol,"r"))==NULL) return(ERR);

while (flag!=4)
	{
	if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);

	if (!strcmp(riga,NOMESET"\n"))
		{
		flag++;
		continue;
		}
	if (!strcmp(riga,NOMEFIELD"\n"))
		{
		flag++;
		continue;
		}
	if (!strcmp(riga,NRFIELD"\n"))
		{
		flag++;
		continue;
		}
	if (!strcmp(riga,NCFIELD"\n"))
		{
		flag++;
		continue;
		}
	}

if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);

EndSearch(p_field,stack,pila_isle);

if (Init1(name_sel_set,name_sel_field)) exit(1);

while (1)
	{
	strcpy(riga,"");

	while (strlen(riga)==0 || (strlen(riga)==1 && riga[0]=='\n') || !strncmp(riga,SOLUZ,strlen(SOLUZ)))
		if (fgets(riga,LENRIGA,fp)==NULL)
			{
			for (nr=0;nr<solutions.nr;nr++)
				for (nc=0;nc<solutions.nc;nc++)
					if (p_field->field[nr][nc]>1) p_field->field[nr][nc]=0;
			return(OKAY);
			}
		
	for (nr=0;nr<solutions.nr;nr++)
		{
		ind=0;
		for (nc=0;nc<solutions.nc;nc++)
			{
			if (riga[ind+3]=='.')
				{
				p_field->field[nr][nc]=0;
				ind+=4;
				continue;
				}
			if (riga[ind+3]=='@')
				{
				p_field->field[nr][nc]=1;
				ind+=4;
				continue;
				}
			sscanf(&riga[ind],"%d",&appo);
			p_field->field[nr][nc]=appo+2;
			ind+=4;
			}
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		}
	CaricaSol(p_field);
	}
}

/********************************/
/*				*/
/*	    RSnap		*/
/*				*/
/********************************/

/* Read snapshot */

void RSnap(void)
{
XmString snap;
char namesnap[128];

GetNameSnap(namesnap);

snap=XmStringCreateLocalized("*.snp");

dial_snap=XmCreateFileSelectionDialog(toplevel,"filesb",NULL,0);

XmFileSelectionDoSearch(dial_snap,snap);

XtSetSensitive(XmFileSelectionBoxGetChild(dial_snap,XmDIALOG_HELP_BUTTON),False);
XmTextSetString(XmFileSelectionBoxGetChild(dial_snap,XmDIALOG_TEXT),namesnap);

XtAddCallback(dial_snap,XmNokCallback,snap_ok_cb,&dial_snap);
XtAddCallback(dial_snap,XmNcancelCallback,snap_cancel_cb,&dial_snap);

XtManageChild(dial_snap);

XmStringFree(snap);

unsensitive();
}


/********************************/
/*				*/
/*	    unsensitive		*/
/*				*/
/********************************/

/* Rende insensitivi molti widgets */

void unsensitive(void)
{
Widget widget;

if ((widget=XtNameToWidget(file_menu,"button_0"))) XtSetSensitive(widget,False);
if ((widget=XtNameToWidget(file_menu,"button_1"))) XtSetSensitive(widget,False);
if ((widget=XtNameToWidget(file_menu,"button_2"))) XtSetSensitive(widget,False);
if ((widget=XtNameToWidget(file_menu,"button_3"))) XtSetSensitive(widget,False);
if ((widget=XtNameToWidget(file_menu,"button_4"))) XtSetSensitive(widget,False);
if ((widget=XtNameToWidget(file_menu,"button_5"))) XtSetSensitive(widget,False);
XtSetSensitive(selec_menu,False);
XtSetSensitive(showsol,False);
XtSetSensitive(arrow1,False);
XtSetSensitive(arrow2,False);
XtSetSensitive(arrow3,False);
XtSetSensitive(arrow4,False);
XtSetSensitive(pushb_start,False);
XtSetSensitive(pushb_stop,False);
XtSetSensitive(pushb_reset,False);
XtSetSensitive(XmOptionButtonGadget(option_menu),False);
XtSetSensitive(XmOptionButtonGadget(option_menu1),False);
XtSetSensitive(XmOptionButtonGadget(option_menu2),False);
XtSetSensitive(XmOptionButtonGadget(option_menu3),False);
XtSetSensitive(XmOptionButtonGadget(option_menu4),False);
XtSetSensitive(XmOptionButtonGadget(option_menu5),False);
XtSetSensitive(XmOptionButtonGadget(option_menu6),False);
XtSetSensitive(scale_vis,False);
XtSetSensitive(scale_snp,False);
}

/********************************/
/*				*/
/*	    snap_cancel_cb	*/
/*				*/
/********************************/

/* Gestione cancel in read snapshot */

void snap_cancel_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
Widget dialog;

dialog=*(Widget *)client_data;

XtDestroyWidget(dialog);
sensitive();
}


/********************************/
/*				*/
/*	    sensitive		*/
/*				*/
/********************************/

/* Rende molti widgets sensitive */

void sensitive(void)
{
Widget widget;

if (nsol>0 && stato==LIMBO)
	{
	if ((widget=XtNameToWidget(file_menu,"button_1"))) XtSetSensitive(widget,True);
	XtSetSensitive(showsol,True);
	}
if (stato!=LIMBO && option_dlx==NO)
	if ((widget=XtNameToWidget(file_menu,"button_3"))) XtSetSensitive(widget,True);
if (stato==LIMBO)
	{
	if (option_dlx==NO)
		if ((widget=XtNameToWidget(file_menu,"button_2"))) XtSetSensitive(widget,True);
	if ((widget=XtNameToWidget(file_menu,"button_0"))) XtSetSensitive(widget,True);
	if ((widget=XtNameToWidget(file_menu,"button_4"))) XtSetSensitive(widget,True);
	if ((widget=XtNameToWidget(file_menu,"button_5"))) XtSetSensitive(widget,True);
	XtSetSensitive(selec_menu,True);
	}

switch (stato)
	{
	case LIMBO:	XtSetSensitive(pushb_start,True);
			XtSetSensitive(pushb_stop,False);
			XtSetSensitive(pushb_reset,False);
			break;
	case START:	XtSetSensitive(pushb_start,False);
			XtSetSensitive(pushb_stop,True);
			XtSetSensitive(pushb_reset,False);
			break;
	case SOSP:	XtSetSensitive(pushb_start,True);
			XtSetSensitive(pushb_stop,False);
			XtSetSensitive(pushb_reset,True);
			break;
	}

if (option_dlx==NO)
	XtSetSensitive(XmOptionButtonGadget(option_menu),True);
if (stato==LIMBO) XtSetSensitive(XmOptionButtonGadget(option_menu1),True);
XtSetSensitive(XmOptionButtonGadget(option_menu2),True);
XtSetSensitive(XmOptionButtonGadget(option_menu3),True);
XtSetSensitive(XmOptionButtonGadget(option_menu4),True);
XtSetSensitive(XmOptionButtonGadget(option_menu5),True);
XtSetSensitive(XmOptionButtonGadget(option_menu6),True);
XtSetSensitive(scale_vis,True);

if (option_dlx==NO)
	XtSetSensitive(scale_snp,True);

if (stato==LIMBO && nsol>0)
	XtSetSensitive(showsol,True);
}



/********************************/
/*				*/
/*	    snap_ok_cb		*/
/*				*/
/********************************/

/* Read snapshot */

void snap_ok_cb(Widget widget,XtPointer client_data,XtPointer call_data)
{
Widget dialog;
char *filename;
XmFileSelectionBoxCallbackStruct *cbs;

dialog=*(Widget *)client_data;

XtDestroyWidget(dialog);

cbs=(XmFileSelectionBoxCallbackStruct *)call_data;

if (!XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&filename))
	{
	sensitive();
	return;
	}

if (filename[0]=='\0')
	{
	XtFree(filename);
	sensitive();
	return;
	}

strcpy(namesnap,filename);

XtFree(filename);

dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Reading snapshot in progress... please wait!",1);
XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeReadSnap,&dial);
}

/********************************/
/*				*/
/*	    GeReadSnap		*/
/*				*/
/********************************/

/* Chiamata con timer dalla routine precedente */

void GeReadSnap(XtPointer call_data)
{
Widget dialog;
int esito;

dialog=*(Widget *)call_data;

esito=ReadSnap(namesnap);

XtDestroyWidget(dialog);

if (esito) PostDialog(toplevel,XmDIALOG_ERROR,"Read error!",0);
else
	{
	dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Done successfully!",1);
	XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeDestroy,&dial);
	}
sensitive();
}


/********************************/
/*				*/
/*	    ReadSnap		*/
/*				*/
/********************************/

/* Effettiva lettura snapshot */

/* INPUT:	namesnap	nome file snapshot	*/
/* OUTPUT:	OKAY/ERR				*/

int ReadSnap(char *namesnap)
{
FILE *fp;
int nr;
int nc;
int ind;
int ns;
int nstack;
int nsol1;
struct Poly_def *ppez;
int nbyr;


if ((fp=fopen(namesnap,"rb"))==NULL) return(ERR);

EndSearch(p_field,stack,pila_isle);

nbyr=fread(record,256,1,fp);
if (nbyr==0) return(ERR);

ind=0;

strcpy(name_sel_set,&record[ind]);
ind+=INCNAME;

strcpy(name_sel_field,&record[ind]);
ind+=INCNAME;

if (Init1(name_sel_set,name_sel_field)) exit(1);

nsol1=*(int *)&record[ind];
ind+=sizeof(int);

ind_stk=*(int *)&record[ind];
ind+=sizeof(int);

nr_pez=*(int *)&record[ind];
ind+=sizeof(int);

nr_clas=*(int *)&record[ind];
ind+=sizeof(int);

pdef_corr=*(struct Poly_def **)&record[ind];
ind+=sizeof(struct Poly_def *);

pez_corr=*(struct Pezzo **)&record[ind];
ind+=sizeof(struct Pezzo *);

numrow=*(int *)&record[ind];
ind+=sizeof(int);

numcol=*(int *)&record[ind];
ind+=sizeof(int);

click=*(int *)&record[ind];
ind+=sizeof(int);

if (*(int *)&record[ind]==AUTO || option_dlx==NO)
	mode=*(int *)&record[ind];
ind+=sizeof(int);

ckder=*(int *)&record[ind];
ind+=sizeof(int);

waitsol=*(int *)&record[ind];
ind+=sizeof(int);

if (*(int *)&record[ind]==ONTIMEOUT || *(int *)&record[ind]==ONSOL || option_dlx==NO)
	tipovis=*(int *)&record[ind];
ind+=sizeof(int);

duty=*(float *)&record[ind];
ind+=sizeof(float);

save=*(int *)&record[ind];
ind+=sizeof(int);

if (option_dlx==NO)
	autosnap=*(int *)&record[ind];
ind+=sizeof(int);

if (*(int *)&record[ind]==ASCII || *(int *)&record[ind]==PIXMAP || option_dlx==NO)
	fmt_save=*(int *)&record[ind];
ind+=sizeof(int);

previs=*(int *)&record[ind];
ind+=sizeof(int);

if (option_dlx==NO)
	presnap=*(int *)&record[ind];
ind+=sizeof(int);

lenrec_ori=lenrec;

lenrec=*(int *)&record[ind];
ind+=sizeof(int);

if (lenrec > lenrec_ori)
	{
	printf("\nInconsistent snapshot!\n");
	exit(1);
	}

if (pdef_corr)
	{
	pdef_corr=GetAddDefPezzo(nr_pez);
	if (pdef_corr==NULL)
		{
		printf("\nInconsistent snapshot!\n");
		exit(1);
		}
	if (pez_corr)
		{
		pez_corr=GetAddPezzo(pdef_corr,nr_clas);
		if (pez_corr==NULL)
			{
			printf("\nInconsistent snapshot!\n");
			exit(1);
			}
		}
	}

for (ns=0;ns<nsol1;ns++)
	{
	nbyr=fread(record,lenrec,1,fp);
	if (nbyr==0) return(ERR);

	ind=0;

	for (nr=0;nr<solutions.nr;nr++)
		for (nc=0;nc<solutions.nc;nc++)
			p_field->field[nr][nc]=record[ind++];
	CaricaSol(p_field);
	}

nbyr=fread(record,lenrec,1,fp);
if (nbyr==0) return(ERR);

ind=0;

for (nstack=0;nstack<ind_stk;nstack++)
	{
	stack[nstack].Nr=*(int *)&record[ind];
	ind+=sizeof(int);

	stack[nstack].Nc=*(int *)&record[ind];
	ind+=sizeof(int);

	stack[nstack].nr_pezzo=*(int *)&record[ind];
	ind+=sizeof(int);

	stack[nstack].nr_classe=*(int *)&record[ind];
	ind+=sizeof(int);

	stack[nstack].pun_def=*(struct Poly_def **)&record[ind];
	ind+=sizeof(struct Poly_def *);

	stack[nstack].pun_pez=*(struct Pezzo **)&record[ind];
	ind+=sizeof(struct Pezzo *);
	}

for (nstack=0;nstack<ind_stk;nstack++)
	{
	if (stack[nstack].pun_def)
		{
		stack[nstack].pun_def=GetAddDefPezzo(stack[nstack].nr_pezzo);
		if (stack[nstack].pun_pez) stack[nstack].pun_pez=
			GetAddPezzo(stack[nstack].pun_def,stack[nstack].nr_classe);
		}
	}


nbyr=fread(record,lenrec,1,fp);
if (nbyr==0) return(ERR);

ppez=p_tpez;

ind=0;

while (ppez)
	{
	ppez->inserito=*(int *)&record[ind];
	ind+=sizeof(int);
	ppez=ppez->next;
	}


nbyr=fread(record,lenrec,1,fp);
if (nbyr==0) return(ERR);

ind=0;

for (nr=0;nr<p_field->nr;nr++)
	for (nc=0;nc<p_field->nc;nc++)
		p_field->field[nr][nc]=record[ind++];

fclose(fp);


stato=SOSP;
GestIntest();
XFillRectangle(dpy,win,gc,0,0,w_dra,h_dra);
DrawField(win,p_field->nr,p_field->nc,p_field->field);
GesTime();

XtVaSetValues(message_area,	XtVaTypedArg,	XmNlabelString,	XmRString,	" ",2,
				NULL);

Refresh_options();
Refresh_scales();

lenrec=lenrec_ori;

return(OKAY);
}


/********************************/
/*				*/
/*	    SSnap		*/
/*				*/
/********************************/

/* Salva snapshot */

void SSnap(void)
{
autosnp=YES;
dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Writing snapshot in progress... please wait!",1);
XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeSaveSnap,&dial);
}

/********************************/
/*				*/
/*	    GeSaveSnap		*/
/*				*/
/********************************/

/* Chiamata con timer dalla routine precedente */

void GeSaveSnap(XtPointer call_data)
{
Widget dialog;
int esito;

dialog=*(Widget *)call_data;

esito=SaveSnap();

XtDestroyWidget(dialog);

if (esito) PostDialog(toplevel,XmDIALOG_ERROR,"Write error!",0);
else
	{
	dial=PostDialog(toplevel,XmDIALOG_MESSAGE,"Done successfully!",1);
	XtAppAddTimeOut(app,TIMRIM,(XtTimerCallbackProc)GeDestroy,&dial);
	}
}


/********************************/
/*				*/
/*	    SaveSnap		*/
/*				*/
/********************************/

/* Effettiva scrittura snapshot */

/* OUTPUT:	OKAY/ERR		*/

int SaveSnap(void)
{
FILE *fp;
char namesnap[128];
int nr;
int nc;
int ind;
int nstack;
struct Sol *psol;
struct Poly_def *ppez;

GetNameSnap(namesnap);

if ((fp=fopen(namesnap,"wb+"))==NULL) return(ERR);

memset(record,0,LENFREC);

ind=0;

strcpy(&record[ind],name_sel_set);
ind+=INCNAME;

strcpy(&record[ind],name_sel_field);
ind+=INCNAME;

*(int *)&record[ind]=nsol;
ind+=sizeof(int);

*(int *)&record[ind]=ind_stk;
ind+=sizeof(int);

*(int *)&record[ind]=nr_pez;
ind+=sizeof(int);

*(int *)&record[ind]=nr_clas;
ind+=sizeof(int);

*(struct Poly_def **)&record[ind]=pdef_corr;
ind+=sizeof(struct Poly_def *);

*(struct Pezzo **)&record[ind]=pez_corr;
ind+=sizeof(struct Pezzo *);

*(int *)&record[ind]=numrow;
ind+=sizeof(int);

*(int *)&record[ind]=numcol;
ind+=sizeof(int);

*(int *)&record[ind]=click;
ind+=sizeof(int);

*(int *)&record[ind]=mode;
ind+=sizeof(int);

*(int *)&record[ind]=ckder;
ind+=sizeof(int);

*(int *)&record[ind]=waitsol;
ind+=sizeof(int);

*(int *)&record[ind]=tipovis;
ind+=sizeof(int);

*(float *)&record[ind]=duty;
ind+=sizeof(float);

*(int *)&record[ind]=save;
ind+=sizeof(int);

*(int *)&record[ind]=autosnap;
ind+=sizeof(int);

*(int *)&record[ind]=fmt_save;
ind+=sizeof(int);

*(int *)&record[ind]=previs;
ind+=sizeof(int);

*(int *)&record[ind]=presnap;
ind+=sizeof(int);

*(int *)&record[ind]=lenrec;
ind+=sizeof(int);

fwrite(record,LENFREC,1,fp);

fclose(fp);

if ((fp=fopen(namesnap,"ab+"))==NULL) return(ERR);

psol=solutions.testa;

while (psol)
	{
	memset(record,0,lenrec);

	ind=0;

	for (nr=0;nr<solutions.nr;nr++)
		for (nc=0;nc<solutions.nc;nc++)
			record[ind++]=psol->field[nr][nc];
	
	fwrite(record,lenrec,1,fp);

	psol=psol->next;
	}

memset(record,0,lenrec);

ind=0;

for (nstack=0;nstack<ind_stk;nstack++)
	{
	*(int *)&record[ind]=stack[nstack].Nr;
	ind+=sizeof(int);

	*(int *)&record[ind]=stack[nstack].Nc;
	ind+=sizeof(int);

	*(int *)&record[ind]=stack[nstack].nr_pezzo;
	ind+=sizeof(int);

	*(int *)&record[ind]=stack[nstack].nr_classe;
	ind+=sizeof(int);

	*(struct Poly_def **)&record[ind]=stack[nstack].pun_def;
	ind+=sizeof(struct Poly_def *);

	*(struct Pezzo **)&record[ind]=stack[nstack].pun_pez;
	ind+=sizeof(struct Pezzo *);
	}
	
fwrite(record,lenrec,1,fp);


memset(record,0,lenrec);

ppez=p_tpez;

ind=0;

while (ppez)
	{
	*(int *)&record[ind]=ppez->inserito;
	ind+=sizeof(int);
	ppez=ppez->next;
	}

fwrite(record,lenrec,1,fp);


memset(record,0,lenrec);

ind=0;

for (nr=0;nr<p_field->nr;nr++)
	for (nc=0;nc<p_field->nc;nc++)
		record[ind++]=p_field->field[nr][nc];
	
fwrite(record,lenrec,1,fp);

fclose(fp);

return(OKAY);
}



/********************************/
/*				*/
/*	    GetAddDefPezzo	*/
/*				*/
/********************************/

/* Ottiene il pointer alla definizione del pezzo		*/

/* INPUT:	npez		nr. pezzo			*/
/* OUTPUT:	pdef		pointer definizione pezzo	*/

struct Poly_def *GetAddDefPezzo(int npez)
{
int pcount;
struct Poly_def *pdef;

pcount=0;
pdef=p_tpez;

for (pcount=0;pcount<npez;pcount++)
	pdef=pdef->next;

return(pdef);
}


/********************************/
/*				*/
/*	    GetAddPezzo		*/
/*				*/
/********************************/

/* Ottiene il pointer al pezzo					*/

/* INPUT:	nclas		nr. classe			*/
/* 		pdef		pointer definizione pezzo	*/
/* OUTPUT:	ppez		pointer pezzo			*/

struct Pezzo *GetAddPezzo(struct Poly_def *pdef,int nclas)
{
int pcount;
struct Pezzo *ppez;

pcount=0;
ppez=pdef->testa;

for (pcount=0;pcount<nclas;pcount++)
	ppez=ppez->next;

return(ppez);
}


/********************************/
/*				*/
/*	    GetNameSnap		*/
/*				*/
/********************************/

/* Genera nome file snapshot */

/* INPUT:	namesnap	pointer nome file snapshot 	*/

void GetNameSnap(char *namesnap)
{
strcpy(namesnap,DIRSNAP);
strcat(namesnap,name_sel_set);
strcat(namesnap,"-[");
strcat(namesnap,name_sel_field);
strcat(namesnap,"]");
strcat(namesnap,".snp");
}




/********************************/
/*				*/
/*	    Getdefaults		*/
/*				*/
/********************************/

/* INPUT:	nome		nome base	*/

/* Get eventuali defaults */

void GetDefaults(char *nome)
{
FILE *fp;
char confname[128];

stato=LIMBO;
click=0;
lpix=(sz==LARGE) ? N_LPIX:S_LPIX;

LoadDflts();

strcpy(confname,nome);
strcat(confname,".ini");

if ((fp=fopen(confname,"r")))
	{
	if (LoadDfltsFile(fp)) LoadDflts();
	fclose(fp);
	}
}


/********************************/
/*				*/
/*	    LoadDflts		*/
/*				*/
/********************************/

/* Carica defaults hard-coded */

void LoadDflts(void)
{
mode=AUTO;
ckder=YES;
waitsol=NO;
tipovis=ONSOL;
save=YES;
autosnap=NO;
fmt_save=ASCII;
previs=INIVIS;
presnap=INISNAP;
timeout=TIMEOUT;
duty=DUTYNC;
strcpy(dataname,"polymin.dat");
}

/********************************/
/*				*/
/*	    LoadDfltsFile	*/
/*				*/
/********************************/

/* Carica defaults form file */

int LoadDfltsFile(FILE *fp)
{
char riga[LENRIGA];

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"mode\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&mode);
		if (mode!=AUTO && mode!=SEMI) mode=AUTO;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"ckder\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&ckder);
		if (ckder!=YES && ckder!=NO) ckder=YES;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"waitsol\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&waitsol);
		if (waitsol!=YES && waitsol!=NO) waitsol=NO;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"tipovis\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&tipovis);
		if (tipovis!=CONTINOUS && tipovis!=ONSOL && tipovis!=ONTIMEOUT) tipovis=ONSOL;
		if (tipovis==CONTINOUS) duty=DUTYC;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"save\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&save);
		if (save!=YES && save!=NO) save=YES;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"autosnap\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&autosnap);
		if (autosnap!=YES && autosnap!=NO) autosnap=NO;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"fmt_save\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&fmt_save);
		if (fmt_save!=ASCII && fmt_save!=PIXMAP && fmt_save!=JPG) fmt_save=ASCII;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"previs\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&previs);
		if (previs<1 || previs>60) previs=INIVIS;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"presnap\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%d",&presnap);
		if (presnap<1 || presnap>60) presnap=INISNAP;
		break;
		}

fseek(fp,0,0);

while (fgets(riga,LENRIGA,fp))
	if (!strcmp(riga,"dataname\n"))
		{
		if (fgets(riga,LENRIGA,fp)==NULL) return(ERR);
		sscanf(riga,"%s",dataname);
		if (!strstr(dataname,".dat")) strcat(dataname,".dat");
		break;
		}

return(OKAY);
}



/********************************/
/*				*/
/*	    Create_options	*/
/*				*/
/********************************/

/* Crea widget opzioni */

void Create_options(void)
{
XmString runmode;
XmString autom;
XmString step;
XmString ckmode;
XmString ckon;
XmString ckoff;
XmString vmode;
XmString cont;
XmString onsol;
XmString ontime;
XmString smode;
XmString waiton;
XmString waitoff;
XmString smode1;
XmString saveon;
XmString saveoff;
XmString snap;
XmString snapon;
XmString snapoff;
XmString fmtsave;
XmString fmtasc;
XmString fmtpix;
XmString fmtjpg;

int def=0;

runmode=XmStringCreateLocalized("Run mode:   ");
autom=XmStringCreateLocalized("Auto        ");
step=XmStringCreateLocalized("Step        ");

switch (mode)
	{
	case	AUTO:	def=0;
			break;
	case	SEMI:	def=1;
			break;
	}

option_menu=XmVaCreateSimpleOptionMenu(rc1,"option_menu",runmode,'R',def,option_cb,
						XmVaPUSHBUTTON,	autom,	'A',	NULL,	NULL,
						XmVaPUSHBUTTON,	step,	'S',	NULL,	NULL,
						NULL);

XtManageChild(option_menu);

if (option_dlx==YES)
	XtSetSensitive(XmOptionButtonGadget(option_menu),False);

XmStringFree(runmode);
XmStringFree(autom);
XmStringFree(step);

ckmode=XmStringCreateLocalized("Check mode: ");
ckon=XmStringCreateLocalized("Check on    ");
ckoff=XmStringCreateLocalized("Check off   ");
 

def=(ckder) ? 0:1;

option_menu1=XmVaCreateSimpleOptionMenu(rc1,"option_menu1",ckmode,'C',def,option_cb1,
						XmVaPUSHBUTTON,	ckon,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	ckoff,	NULL,	NULL,	NULL,
						NULL);

XtManageChild(option_menu1);

XmStringFree(ckmode);
XmStringFree(ckon);
XmStringFree(ckoff);

vmode=XmStringCreateLocalized("View mode:  ");
cont=XmStringCreateLocalized("Continous   ");
onsol=XmStringCreateLocalized("On solution ");
ontime=XmStringCreateLocalized("On timeout  ");

switch (tipovis)
	{
	case	CONTINOUS:	def=0;
				break;
	case	ONSOL:		def=1;
				break;
	case	ONTIMEOUT:	def=2;
				break;
	}

option_menu2=XmVaCreateSimpleOptionMenu(rc1,"option_menu2",vmode,'V',def,option_cb2,
						XmVaPUSHBUTTON,	cont,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	onsol,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	ontime,	NULL,	NULL,	NULL,
						NULL);

XtManageChild(option_menu2);

XmStringFree(vmode);
XmStringFree(cont);
XmStringFree(onsol);
XmStringFree(ontime);

smode=XmStringCreateLocalized("On solution:");
waiton=XmStringCreateLocalized("Wait        ");
waitoff=XmStringCreateLocalized("No Wait     ");

def=(waitsol) ? 0:1;

option_menu3=XmVaCreateSimpleOptionMenu(rc1,"option_menu3",smode,'O',def,option_cb3,
						XmVaPUSHBUTTON,	waiton,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	waitoff,NULL,	NULL,	NULL,
						NULL);

XtManageChild(option_menu3);

XmStringFree(smode);
XmStringFree(waiton);
XmStringFree(waitoff);

smode1=XmStringCreateLocalized("On solution:");
saveon=XmStringCreateLocalized("Save to disk");
saveoff=XmStringCreateLocalized("No save     ");

def=(save) ? 0:1;

option_menu4=XmVaCreateSimpleOptionMenu(rc1,"option_menu4",smode1,'T',def,option_cb4,
						XmVaPUSHBUTTON,	saveon,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	saveoff,NULL,	NULL,	NULL,
						NULL);

XtManageChild(option_menu4);

XmStringFree(smode1);
XmStringFree(saveon);
XmStringFree(saveoff);

snap=XmStringCreateLocalized("AutoSnap:   ");
snapon=XmStringCreateLocalized("On          ");
snapoff=XmStringCreateLocalized("Off         ");

def=(autosnap) ? 0:1;

option_menu5=XmVaCreateSimpleOptionMenu(rc1,"option_menu5",snap,'A',def,option_cb5,
						XmVaPUSHBUTTON,	snapon,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	snapoff,NULL,	NULL,	NULL,
						NULL);

XtManageChild(option_menu5);

if (option_dlx==YES)
	XtSetSensitive(XmOptionButtonGadget(option_menu5),False);

XmStringFree(snap);
XmStringFree(snapon);
XmStringFree(snapoff);

fmtsave=XmStringCreateLocalized("Save format:");
fmtasc=XmStringCreateLocalized("ASCII only  ");
fmtpix=XmStringCreateLocalized("PIXMAP      ");
fmtjpg=XmStringCreateLocalized("JPEG        ");

switch (fmt_save)
	{
	case	ASCII:		def=0;
				break;
	case	PIXMAP:		def=1;
				break;
	case	JPG:		def=2;
				break;
	}

#if USE_JPG
if (planes==24)
	option_menu6=XmVaCreateSimpleOptionMenu(rc1,"option_menu6",fmtsave,'P',def,option_cb6,
							XmVaPUSHBUTTON,	fmtasc,	NULL,	NULL,	NULL,
							XmVaPUSHBUTTON,	fmtpix, NULL,	NULL,	NULL,
							XmVaPUSHBUTTON,	fmtjpg, NULL,	NULL,	NULL,
							NULL);
	else
	option_menu6=XmVaCreateSimpleOptionMenu(rc1,"option_menu6",fmtsave,'P',def,option_cb6,
							XmVaPUSHBUTTON,	fmtasc,	NULL,	NULL,	NULL,
							XmVaPUSHBUTTON,	fmtpix, NULL,	NULL,	NULL,
							NULL);
#else
option_menu6=XmVaCreateSimpleOptionMenu(rc1,"option_menu6",fmtsave,'P',def,option_cb6,
						XmVaPUSHBUTTON,	fmtasc,	NULL,	NULL,	NULL,
						XmVaPUSHBUTTON,	fmtpix, NULL,	NULL,	NULL,
						NULL);
#endif

XtManageChild(option_menu6);

XmStringFree(fmtsave);
XmStringFree(fmtasc);
XmStringFree(fmtpix);
XmStringFree(fmtjpg);
}


/********************************/
/*				*/
/*	    Create_scales	*/
/*				*/
/********************************/

/* Crea widget scales */

void Create_scales(void)
{
scale_vis=XtVaCreateManagedWidget("v_timer",	xmScaleWidgetClass,	form_w2,
						XtVaTypedArg,	XmNtitleString,	XmRString,	"View",	5,
						XmNmaximum,			60,
						XmNminimum,			1,
						XmNvalue,			previs,
						XmNshowValue,			True,
						XmNorientation,			XmVERTICAL,
						XmNtopAttachment,		XmATTACH_POSITION,
						XmNtopPosition,			0,
						XmNleftAttachment,		XmATTACH_POSITION,
						XmNleftPosition,		0,
						NULL);

XtAddCallback(scale_vis,XmNvalueChangedCallback,vis_cb,NULL);

scale_snp=XtVaCreateManagedWidget("s_timer",	xmScaleWidgetClass,	form_w2,
						XtVaTypedArg,	XmNtitleString,	XmRString,	"Snap",	5,
						XmNmaximum,			60,
						XmNminimum,			1,
						XmNvalue,			presnap,
						XmNshowValue,			True,
						XmNorientation,			XmVERTICAL,
						XmNtopAttachment,		XmATTACH_POSITION,
						XmNtopPosition,			0,
						XmNleftAttachment,		XmATTACH_POSITION,
						XmNleftPosition,		3,
						NULL);
if (option_dlx==YES)
	XtSetSensitive(scale_snp,False);
	
XtAddCallback(scale_snp,XmNvalueChangedCallback,snap_cb,NULL);
}


/********************************/
/*				*/
/*	    Create_com		*/
/*				*/
/********************************/

/* Crea widget start/stop/reset */

void Create_com(void)
{
pushb_start=XtVaCreateManagedWidget("start",	xmPushButtonWidgetClass,	form_w1,
						XmNtopAttachment,		XmATTACH_POSITION,
						XmNtopPosition,			0,
						XmNleftAttachment,		XmATTACH_POSITION,
						XmNleftPosition,		0,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		2,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		2,
						NULL);

XtAddCallback(pushb_start,XmNactivateCallback,ge_start,NULL);

pushb_stop=XtVaCreateManagedWidget("stop ",	xmPushButtonWidgetClass,	form_w1,
						XmNtopAttachment,		XmATTACH_POSITION,
						XmNtopPosition,			0,
						XmNleftAttachment,		XmATTACH_POSITION,
						XmNleftPosition,		2,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		2,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		4,
						NULL);

XtSetSensitive(pushb_stop,False);
XtAddCallback(pushb_stop,XmNactivateCallback,ge_stop,NULL);

pushb_reset=XtVaCreateManagedWidget("reset",	xmPushButtonWidgetClass,	form_w1,
						XmNtopAttachment,		XmATTACH_POSITION,
						XmNtopPosition,			2,
						XmNleftAttachment,		XmATTACH_POSITION,
						XmNleftPosition,		1,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		4,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		3,
						NULL);
XtSetSensitive(pushb_reset,False);
XtAddCallback(pushb_reset,XmNactivateCallback,ge_reset,NULL);
}


/********************************/
/*				*/
/*	    Create_arrows	*/
/*				*/
/********************************/

/* Crea widget arrows */

void Create_arrows(void)
{
arrow1=XtVaCreateManagedWidget ("arrow1",	xmArrowButtonGadgetClass,	form_w2,
						XmNtopAttachment,		XmATTACH_POSITION,
        					XmNtopPosition,      		0,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		2,
						XmNleftAttachment,		XmATTACH_POSITION,
        					XmNleftPosition,     		7,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		8,
        					XmNarrowDirection,   		XmARROW_UP,
        					NULL);

XtSetSensitive(arrow1,False);
XtAddCallback(arrow1,XmNarmCallback,up_cb,NULL);


arrow2=XtVaCreateManagedWidget ("arrow2",	xmArrowButtonGadgetClass, 	form_w2,
						XmNtopAttachment,		XmATTACH_POSITION,
        					XmNtopPosition,      		2,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		4,
						XmNleftAttachment,		XmATTACH_POSITION,
        					XmNleftPosition,     		6,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		7,
        					XmNarrowDirection,   		XmARROW_LEFT,
        					NULL);

XtSetSensitive(arrow2,False);
XtAddCallback(arrow2,XmNarmCallback,left_cb,NULL);


arrow3=XtVaCreateManagedWidget ("arrow3",	xmArrowButtonGadgetClass,	form_w2,
						XmNtopAttachment,		XmATTACH_POSITION,
        					XmNtopPosition,      		2,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		4,
						XmNleftAttachment,		XmATTACH_POSITION,
        					XmNleftPosition,     		8,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		9,
        					XmNarrowDirection,   		XmARROW_RIGHT,
        					NULL);

XtSetSensitive(arrow3,False);
XtAddCallback(arrow3,XmNarmCallback,right_cb,NULL);


arrow4=XtVaCreateManagedWidget ("arrow4",	xmArrowButtonGadgetClass,	form_w2,
						XmNtopAttachment,		XmATTACH_POSITION,
        					XmNtopPosition,      		4,
						XmNbottomAttachment,		XmATTACH_POSITION,
        					XmNbottomPosition,   		6,
						XmNleftAttachment,		XmATTACH_POSITION,
        					XmNleftPosition,     		7,
						XmNrightAttachment,		XmATTACH_POSITION,
        					XmNrightPosition,    		8,
        					XmNarrowDirection,   		XmARROW_DOWN,
        					NULL);

XtSetSensitive(arrow4,False);
XtAddCallback(arrow4,XmNarmCallback,down_cb,NULL);

showsol=XtVaCreateManagedWidget("ViewSol",	xmPushButtonWidgetClass,	form_w2,
						XmNtopAttachment,		XmATTACH_POSITION,
						XmNtopPosition,			6,
						XmNleftAttachment,		XmATTACH_POSITION,
						XmNleftPosition,		6,
						NULL);

XtSetSensitive(showsol,False);
XtAddCallback(showsol,XmNactivateCallback,shsol_cb,NULL);
}


/********************************/
/*				*/
/*	    Refresh_options	*/
/*				*/
/********************************/

/* Rinfresca widgets options */

void Refresh_options(void)
{
XmString str=NULL;

switch (mode)
	{
	case	AUTO:	str=XmStringCreateLocalized("Auto        ");
			break;
	case	SEMI:	str=XmStringCreateLocalized("Step        ");
			break;
	}

XtVaSetValues(XmOptionButtonGadget(option_menu),XmNlabelString,	str,
						NULL);

XmStringFree(str);

switch (ckder)
	{
	case	NO:	str=XmStringCreateLocalized("Check off   ");
			break;
	case	YES:	str=XmStringCreateLocalized("Check on    ");
			break;
	}
 
XtVaSetValues(XmOptionButtonGadget(option_menu1),XmNlabelString,str,
						 NULL);

XmStringFree(str);

switch (tipovis)
	{
	case	CONTINOUS:	str=XmStringCreateLocalized("Continous   ");
				break;
	case	ONSOL:		str=XmStringCreateLocalized("On solution ");
				break;
	case	ONTIMEOUT:	str=XmStringCreateLocalized("On timeout  ");
				break;
	}

XtVaSetValues(XmOptionButtonGadget(option_menu2),XmNlabelString,str,
						 NULL);

XmStringFree(str);

switch (waitsol)
	{
	case	NO:	str=XmStringCreateLocalized("No Wait     ");
			break;
	case	YES:	str=XmStringCreateLocalized("Wait        ");
			break;
	}

XtVaSetValues(XmOptionButtonGadget(option_menu3),XmNlabelString,str,
						 NULL);
XmStringFree(str);

switch (save)
	{
	case	NO:	str=XmStringCreateLocalized("No Save     ");
			break;
	case	YES:	str=XmStringCreateLocalized("Save to disk");
			break;
	}

XtVaSetValues(XmOptionButtonGadget(option_menu4),XmNlabelString,str,
						 NULL);
XmStringFree(str);

switch (autosnap)
	{
	case	NO:	str=XmStringCreateLocalized("Off         ");
			break;
	case	YES:	str=XmStringCreateLocalized("On          ");
			break;
	}

XtVaSetValues(XmOptionButtonGadget(option_menu5),XmNlabelString,str,
						 NULL);

XmStringFree(str);

switch (fmt_save)
	{
	case	ASCII:		str=XmStringCreateLocalized("ASCII only  ");
				break;
	case	PIXMAP:		str=XmStringCreateLocalized("PIXMAP      ");
				break;
	case	JPG:		str=XmStringCreateLocalized("JPEG        ");
				break;
	}

XtVaSetValues(XmOptionButtonGadget(option_menu6),XmNlabelString,str,
						 NULL);

XmStringFree(str);
}


/********************************/
/*				*/
/*	    Refresh_scales	*/
/*				*/
/********************************/

/* Rinfresca widgets scales */

void Refresh_scales(void)
{
XtVaSetValues(scale_vis,	XmNvalue,	previs,
				NULL);
XtVaSetValues(scale_snp,	XmNvalue,	presnap,
				NULL);
}


void vis(int rows_size, int *rows)
{
int i,j,k;
int ind;
char **fld;
int nump;

nump=0;

fld=malloc((p_field->nr)*sizeof(int **));

for (i=0;i<p_field->nr;i++)
		fld[i]=malloc((p_field->nc)*sizeof(int));

for (j=0;j<p_field->nr;j++)
      	for (k=0;k<p_field->nc;k++)
		fld[j][k]=0;

ind=0;

for (i=0;i<rows_size;i++)
       	{
       	ind = (numpez+coupez)*rows[i];

       	for (j=0;j<numpez;j++)
               	if (Pass[ind++]==1)
			nump=j+2;
       	for (j=0;j<p_field->nr;j++)
               	for (k=0;k<p_field->nc;k++)
                       	{
                       	if (p_field->field[j][k]==1)
                               	fld[j][k]=1;
                       	else
                               	{
                               	if (Pass[ind++]==1)
                                       	fld[j][k] = nump;
                               	}
			}
       	}

for (j=0;j<p_field->nr;j++)
       	for (k=0;k<p_field->nc;k++)
               	p_field->field[j][k]=fld[j][k];

free(fld);

XLockDisplay(dsp);
DrawField(win,p_field->nr,p_field->nc,p_field->field);
XUnlockDisplay(dsp);
vised=YES;
}

int visualize(int rows_size, int *rows)
{
if (tipovis==ONTIMEOUT && vised==NO && click%click_vis==0)
	vis(rows_size,rows);

return 0;
}

int callback1(int rows_size, int *rows, void *data)
{
int i,j,k;
int ind;
char **soluz;
int nump;
int der;

nump=0;

soluz=malloc((p_field->nr)*sizeof(int **));

for (i=0;i<p_field->nr;i++)
	soluz[i]=malloc((p_field->nc)*sizeof(int));

*((int*)data) += 1;

printf("Soluzione nr. %d\n",*(int *)data);

ind=0;

for (i=0;i<rows_size;i++)
        {
        ind = (numpez+coupez)*rows[i];

        for (j=0;j<numpez;j++)
                if (Pass[ind++]==1)
			nump=j+2;
        for (j=0;j<p_field->nr;j++)
                for (k=0;k<p_field->nc;k++)
                        {
                        if (p_field->field[j][k]==1)
                                soluz[j][k]=1;
                        else
                                {
                                if (Pass[ind++]==1)
                                        soluz[j][k] = nump;
                                }
			}
        }

for (j=0;j<p_field->nr;j++)
	{
        for (k=0;k<p_field->nc;k++)
                printf("%3d",soluz[j][k]);
        printf("\n");
        }
printf("\n");

for (j=0;j<p_field->nr;j++)
        for (k=0;k<p_field->nc;k++)
                p_field->field[j][k]=soluz[j][k];

free(soluz);

XLockDisplay(dsp);
DrawField(win,p_field->nr,p_field->nc,p_field->field);
der=CaricaSol(p_field);
ViewSol(der);
if (der==-1 && save)
	{
	if (AppendSol(TEXT,0) || AppendSol(GRAPHIC,nsol))
	PostDialog(toplevel,XmDIALOG_ERROR,"Write error!",0);
	}
if (waitsol)
	{
        stato=SOSP;
        GestIntest();
        XtRemoveTimeOut(tim);
        XtSetSensitive(pushb_start,True);
        XtSetSensitive(pushb_stop,False);
        XtSetSensitive(pushb_reset,True);
        }
XUnlockDisplay(dsp);

return 0;
}

/********************************/
/*				*/
/*	    GenMatDlx		*/
/*				*/
/********************************/

/* INPUT:	*/
/* OUTPUT:	*/
/*		*/

void *GenMatDlx(void *arg)
{
struct Poly_def *pdef;
struct Pezzo *ppez;
char *MatDlx;

int lencol;
int nr;
int nc;
int nr1;
int nc1;
int Nr;
int Nc;
int size;
int nclas;
int ncol;
int ind;
int ind1;
int i;
int j;
int k;
int uni;
struct dlx_Node *header;
int solutions;
int click_copia;
int nrz=0;

click_copia=click;

size=0;

pdef=p_tpez;
nclas=0;

Nr=p_field->nr;
Nc=p_field->nc;

lencol=p_field->nr*p_field->nc+numpez;

MatDlx=malloc(0);

ncol=0;

while (pdef)
	{
	ppez=pdef->testa;
	while (ppez)
		{
		for (nr=0;nr<Nr;nr++)
			for (nc=0;nc<Nc;nc++)
				{
				if (stato==LIMBO)
					pthread_exit(NULL);
				if (TesPez(ppez,p_field,nr,nc)==YES)
					{
					if (tipovis==ONTIMEOUT && vised==NO && click-click_copia>=click_vis)
						{
						click_copia=click;
						InsPz(pdef,ppez,nr,nc,p_field);
						XLockDisplay(dsp);
						DrawField(win,p_field->nr,p_field->nc,p_field->field);
						XUnlockDisplay(dsp);
						RemPz(ppez,nr,nc,p_field);
						vised=YES;
						}
						
					if ((MatDlx=realloc(MatDlx,size+lencol))==NULL)
						{
						printf("\nNot enough memory!\n\n");
						return(0);
						}
					memset(&MatDlx[size],0,lencol);
		
					MatDlx[size+nclas]=1;
		
					for (nc1=0;nc1<ppez->nc;nc1++)
        					if (ppez->pezzo[0][nc1])
                					{
                					nrz=nc1;
                					break;
                					}
					for (nr1=0;nr1<ppez->nr;nr1++)
        					for (nc1=0;nc1<ppez->nc;nc1++)
                					if (ppez->pezzo[nr1][nc1])
                        					MatDlx[size+numpez+(nr+nr1)*Nc+(nc+nc1-nrz)]=1;
		
					size+=lencol;
					ncol++;
					}
				}
		ppez=ppez->next;
		}
	nclas++;
	pdef=pdef->next;
	}
/*
ind=0;
for (nc=0;nc<ncol;nc++)
	{
	for (nr=0;nr<numpez;nr++)
		printf("%1d",MatDlx[ind++]);
		printf("\n");
	for (nr=0;nr<p_field->nr;nr++)
		{
		for (nc1=0;nc1<p_field->nc;nc1++)
			printf("%1d",MatDlx[ind++]);
		printf("\n");
		}
	printf("\n");
	}

ind=0;
for (nc=0;nc<ncol;nc++)
	{
	for (nr=0;nr<numpez+p_field->nr*p_field->nc;nr++)
		printf("%1d",MatDlx[ind++]);
	printf("\n");
	}
*/

coupez=0;
for (j=0;j<p_field->nr;j++)
        for (k=0;k<p_field->nc;k++)
                if (p_field->field[j][k]==0)
                        coupez++;

Pass=malloc(ncol*(numpez+coupez));

ind=0;
ind1=0;
for (i=0;i<ncol;i++)
        {
        for (j=0;j<numpez;j++)
                Pass[ind++]=MatDlx[ind1++];
        for (j=0;j<p_field->nr;j++)
                for (k=0;k<p_field->nc;k++)
			{
                        if (p_field->field[j][k]==0)
                                Pass[ind++]=MatDlx[ind1];
                        ind1++;
			}
        }

free(MatDlx);

uni=0;
for (ind=0;ind<ncol*(numpez+coupez);ind++)
	if (Pass[ind]==1)
		uni++;

Sparse=malloc(uni*2*sizeof(int));

ind=0;
ind1=0;
for (i=0;i<ncol;i++)
	for (j=0;j<numpez+coupez;j++)
		if (Pass[ind1++]==1)
			{
			Sparse[ind]=i;
			Sparse[ind+1]=j;
			ind+=2;
			}

solutions=0;

header=dlx_alloc_sparse(Sparse,ncol,numpez+coupez,uni);
dlx_solve(header, callback1, &solutions);
dlx_free(header);

free(Pass);
free(Sparse);

XLockDisplay(dsp);
stato=LIMBO;
GestIntest();
ViewEnd();
XtRemoveTimeOut(tim);
if (save==NO && nsol) SaveSols();
else sensitive();
XUnlockDisplay(dsp);

return(0);
}


void RunThread(void)
{
pthread_t th;
pthread_attr_t tattr;
struct sched_param param;

/* initialized with default attributes */
pthread_attr_init(&tattr);

/* safe to get existing scheduling param */
pthread_attr_getschedparam(&tattr, &param);

/* set the priority; others are unchanged */
param.sched_priority = 7;

/* setting the new scheduling param */
pthread_attr_setschedparam (&tattr, &param);

pthread_create(&th, &tattr, &GenMatDlx, NULL);
}

void checkEsecuz(void)
{
if (autosnap && click%click_snap==0 && autosnp==NO)
        SSnap();

while (stato!=START || autosnp==YES)
	{
	sleep(1);
	if (stato==LIMBO)
		pthread_exit(NULL);
	}
}


void InsPz(struct Poly_def *p_def,struct Pezzo *p_pez,int Nr,int Nc,struct Poly_field *p_field)
{
int nr;
int nc;

int nrz=0;

for (nc=0;nc<p_pez->nc;nc++)
	if (p_pez->pezzo[0][nc])
		{
		nrz=nc;
		break;
		}

for (nr=0;nr<p_pez->nr;nr++)
	for (nc=0;nc<p_pez->nc;nc++)
		if (p_pez->pezzo[nr][nc])
			p_field->field[Nr+nr][Nc+nc-nrz]=p_def->Poly_num+2;
}

void RemPz(struct Pezzo *p_pez,int Nr,int Nc,struct Poly_field *p_field)
{
int nr;
int nc;

int nrz=0;
for (nc=0;nc<p_pez->nc;nc++)
	if (p_pez->pezzo[0][nc])
		{
		nrz=nc;
		break;
		}

for (nr=0;nr<p_pez->nr;nr++)
	for (nc=0;nc<p_pez->nc;nc++)
		if (p_pez->pezzo[nr][nc])
			p_field->field[Nr+nr][Nc+nc-nrz]=0;
}
