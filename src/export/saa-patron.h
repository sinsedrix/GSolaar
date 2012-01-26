/******************************* saa-patron.h ********************************/
/*                                                                           */
/*   Biblioth�que statique de base :                                         */
/*   Export des patrons                                                      */
/*                                                                           */
/*   Ecrit par : C�dric COULIOU                                              */
/*                                                                           */
/*   GSolaar Copyright (c) (2003-2004) C�dric COULIOU                        */
/*   Contact : sinsedrix@users.sourceforge.net                               */
/*                                                                           */
/* Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et   */
/* respectant les principes de diffusion des logiciels libres. Vous pouvez   */
/* utiliser, modifier et/ou redistribuer ce programme sous les conditions    */
/* de la licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA    */
/* sur le site "http://www.cecill.info".                                     */
/*                                                                           */
/* En contrepartie de l'accessibilit� au code source et des droits de copie, */
/* de modification et de redistribution accord�s par cette licence, il n'est */
/* offert aux utilisateurs qu'une garantie limit�e.  Pour les m�mes raisons, */
/* seule une responsabilit� restreinte p�se sur l'auteur du programme,  le   */
/* titulaire des droits patrimoniaux et les conc�dants successifs.           */
/*                                                                           */
/* A cet �gard  l'attention de l'utilisateur est attir�e sur les risques     */
/* associ�s au chargement,  � l'utilisation,  � la modification et/ou au     */
/* d�veloppement et � la reproduction du logiciel par l'utilisateur �tant    */
/* donn� sa sp�cificit� de logiciel libre, qui peut le rendre complexe �     */
/* manipuler et qui le r�serve donc � des d�veloppeurs et des professionnels */
/* avertis poss�dant  des  connaissances  informatiques approfondies.  Les   */
/* utilisateurs sont donc invit�s � charger  et  tester  l'ad�quation  du    */
/* logiciel � leurs besoins dans des conditions permettant d'assurer la      */
/* s�curit� de leurs syst�mes et ou de leurs donn�es et, plus g�n�ralement,  */
/* � l'utiliser et l'exploiter dans les m�mes conditions de s�curit�.        */
/*                                                                           */
/* Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez    */
/* pris connaissance de la licence CeCILL, et que vous en avez accept� les   */
/* termes.                                                                   */
/*                                                                           */
/*****************************************************************************/
#include "saa-solide.h"

#ifndef _SAA_PATRON_
#define _SAA_PATRON_

typedef enum SaaPatronMode {
	SAA_PATRON_AILES_SEPAREES,
	SAA_PATRON_AILES_COLLEES
} SaaPatronMode;

typedef enum SaaLangMode {
	SAA_LANG_AUCUNE,
	SAA_LANG_NORMAL,
	SAA_LANG_AILE_MAX,
	SAA_LANG_AILE_MIN,
	SAA_LANG_TOUTES
} SaaLangMode;

typedef enum SaaTexteMode {
	SAA_TEXT_AUCUN,
	SAA_TEXT_DESSOUS,
	SAA_TEXT_DESSUS
} SaaTexteMode;

typedef struct SaaBBox {
	float	x, y, w, h;
} SaaBBox;

typedef struct SaaPoint {
	float x, y;
} SaaPoint;

typedef struct SaaPArete {
	SaaPoint	position;
	float		angle;
	float		longueur;
	int			languette;
	char	  *	nom;
} SaaPArete;

typedef struct SaaPAile {
	SaaPoint	position;
	float		angle;
	SaaListe  *	aretes;
	char	  *	nom;
} SaaPAile;

typedef struct SaaPPage {
	SaaPoint	position;
	float		angle;
	SaaListe  *	ailes;
} SaaPPage;

typedef struct SaaPatron {
	SaaPatronMode	mode;
	SaaListe  	  *	pages;
	char		  * nom;
	float			pageH, pageW;
	float			bordure;
	float			languette;
	SaaLangMode		langMode;
	SaaTexteMode	texteMode;
} SaaPatron;

void		saa_point_transformer(SaaPoint * p, float angle, const SaaPoint * vecteur);
void		saa_bbox_transformer(SaaBBox * bbox, float angle, const SaaPoint * vecteur);

SaaPArete * saa_parete_creer(const SaaPoint * position, float angle, float longueur, int languette, const char * nom);
void		saa_parete_detruire(SaaPArete * par);
void		saa_parete_bbox(const SaaPArete * par, SaaBBox * bbox);

SaaPAile *  saa_paile_creer(SaaListe * aretes, const char * nom);
void		saa_paile_detruire(SaaPAile * pai);
void		saa_paile_bbox(const SaaPAile * pai, SaaBBox * bbox);
void		saa_paile_bbox_liste(const SaaListe * ailes, SaaBBox * bbox);

SaaPPage *  saa_ppage_creer();
void		saa_ppage_detruire(SaaPPage * pp);
void		saa_ppage_bbox(const SaaPPage * pp, SaaBBox * bbox);

SaaPatron * saa_patron_creer(SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, const SaaSolide * sol, float echelle, float pageW, float pageH, float bordure, float languette);
void 		saa_patron_detruire(SaaPatron * patron);

int			saa_patron_2_svg10(const SaaPatron * patron, const char * filename);
int			saa_patron_2_svg12(const SaaPatron * patron, const char * filename);
int			saa_patron_2_ps(const SaaPatron * patron, const char * filename);

// Generation d'un fichier PostScript
// mode Ailes Independantes
int saa_solide_exporter_PS(const SaaSolide * sol, const char * filename, SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, int echelle);

// Generation d'un fichier PostScript
// mode Ailes Collees
int saa_solide_exporter_SVG10(const SaaSolide * sol, const char * filename, SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, int echelle);
int saa_solide_exporter_SVG12(const SaaSolide * sol, const char * filename, SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, int echelle);

#endif //_SAA_PATRON_
