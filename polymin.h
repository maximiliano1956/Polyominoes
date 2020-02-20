#include "config.h"

#define YES		1
#define	NO		0
#define OKAY		0
#define ERR		1
#define NOSAVED		2
#define	END		-1
#define PIENO		'@'
#define VUOTO		'.'

#define SFONDO		"lightseagreen"

#define BUTWIDTH	240	
#define BUTHEIGHT	120

#define NUMPIX	87

#define BPIX	4

#define MW	75
#define MH	100

#define N_LPIX	26
#define S_LPIX	12

#define N_DIRPIX	"./pixmaps/normal/"
#define S_DIRPIX	"./pixmaps/small/"

#define DIRSOL		"./solutions/"
#define DIRSNAP		""

#define NOMESET		"NOME SET:"
#define NOMEFIELD	"NOME FIELD:"
#define NRFIELD		"NUMERO RIGHE FIELD:"
#define NCFIELD		"NUMERO COLONNE FIELD:"

#define SOLUZ		"SOLUZIONE"

#define ICONNAME	"iconic.xpm"

//#define	TIMEOUT		100.0
#define	TIMEOUT		10.0
#define	DUTYNC		0.75
#define DUTYC		0.25

#define CONTINOUS	0
#define ONSOL		1
#define ONTIMEOUT	2

#define LIMBO	0
#define START	1
#define SOSP	2

#define	AUTO	0
#define	SEMI	1

#define INSERIMENTO	0
#define RIMOZIONE	1

#define LARGE		0
#define SMALL		1

#define NLOOP		100

#define ASCII		0
#define PIXMAP		1
#define JPG		2

#define TEXT		0
#define GRAPHIC		1

#define LENRIGA		128

#define TIMRIM		1000

#define INIVIS		1
#define INISNAP		1

#define LENFREC		256
#define INCNAME		32

struct	Pezzo
	{
	int nr;				/* numero righe */
	int nc;				/* numero colonne */
	int classe;			/* numero classe pezzo */
	char **pezzo;			/* definizione pezzo */
	struct Pezzo *next;		/* prossima def. pezzo della classe */
	};

struct	Poly_def
	{
	int Poly_num;			/* numero pezzo */
	int inserito;			/* flag pezzo inserito */
	struct Pezzo *testa;		/* pointer definizione pezzo */
	struct Poly_def *next;		/* prossimo pezzo */
	};

struct	Poly_field
	{
	char *Field_name;		/* nome field */
	int nr;				/* numero righe */
	int nc;				/* numero colonne */
	char **field;			/* definizione field */
	struct Poly_field *next;	/* prossimo field */
	};
	
struct	Poly_set
	{
	char *Poly_name;		/* nome set */
	int n_pez;			/* numero pezzi del set */
	struct Poly_def *testa_def;	/* pointer definizione pezzi */
	struct Poly_field *testa_field;	/* pointer definizione fields */
	struct Poly_set *next;		/* prossimo set */
	};

struct Stack
	{
	int Nr;				/* numero riga pezzo inserito */
	int Nc;				/* numero colonna pezzo inserito */
	struct Poly_def *pun_def;	/* pointer struttura definizione pezzo */
	struct Pezzo *pun_pez;		/* pointer struttura pezzo */
	int nr_pezzo;			/* numero pezzo */
	int nr_classe;			/* numero classe */
	};

struct Sol
	{
	char **field;			/* definizione field */
	struct Sol *next;		/* prossima soluzione */
	struct Sol *prev;		/* precedente soluzione */
	};

struct Solutions
	{
	char *Poly_name;                /* nome set */
	char *Field_name;               /* nome field */
	int nr;                         /* numero righe */
	int nc;                         /* numero colonne */
	struct Sol *testa;		/* pointer prima struttura soluzione */
	struct Sol *coda;		/* pointer ultima struttura soluzione */
	};


