/******************************* saa-patron.c ********************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Export des patrons                                                      */
/*                                                                           */
/*   Ecrit par : Cédric COULIOU                                              */
/*                                                                           */
/*   GSolaar Copyright (c) (2003-2004) Cédric COULIOU                        */
/*   Contact : sinsedrix@users.sourceforge.net                               */
/*                                                                           */
/* Ce logiciel est régi par la licence CeCILL soumise au droit français et   */
/* respectant les principes de diffusion des logiciels libres. Vous pouvez   */
/* utiliser, modifier et/ou redistribuer ce programme sous les conditions    */
/* de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA    */
/* sur le site "http://www.cecill.info".                                     */
/*                                                                           */
/* En contrepartie de l'accessibilité au code source et des droits de copie, */
/* de modification et de redistribution accordés par cette licence, il n'est */
/* offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons, */
/* seule une responsabilité restreinte pèse sur l'auteur du programme,  le   */
/* titulaire des droits patrimoniaux et les concédants successifs.           */
/*                                                                           */
/* A cet égard  l'attention de l'utilisateur est attirée sur les risques     */
/* associés au chargement,  à l'utilisation,  à la modification et/ou au     */
/* développement et à la reproduction du logiciel par l'utilisateur étant    */
/* donné sa spécificité de logiciel libre, qui peut le rendre complexe à     */
/* manipuler et qui le réserve donc à des développeurs et des professionnels */
/* avertis possédant  des  connaissances  informatiques approfondies.  Les   */
/* utilisateurs sont donc invités à charger  et  tester  l'adéquation  du    */
/* logiciel à leurs besoins dans des conditions permettant d'assurer la      */
/* sécurité de leurs systèmes et ou de leurs données et, plus généralement,  */
/* à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.        */
/*                                                                           */
/* Le fait que vous puissiez accéder à cet en-tête signifie que vous avez    */
/* pris connaissance de la licence CeCILL, et que vous en avez accepté les   */
/* termes.                                                                   */
/*                                                                           */
/*****************************************************************************/
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "saa-patron.h"

static void saa_patron_arranger_1(SaaPatron * patron, SaaListe * ailes);
static void saa_patron_arranger_2(SaaPatron * patron, SaaListe * ailes);

int saa_point_compare_x(const SaaPoint * pa, const SaaPoint * pb) {
	return	pa->x < pb->x ? 1 : -1;
}
int saa_point_compare_y(const SaaPoint * pa, const SaaPoint * pb) {
	return	pa->y < pb->y ? 1 : -1;
}

int saa_paile_compare_bbox(const SaaPAile * pa, const SaaPAile * pb) {
	SaaBBox bboxa, bboxb;
	float airea, aireb;

	saa_paile_bbox(pa, &bboxa);
	saa_paile_bbox(pb, &bboxb);
	airea = bboxa.w * bboxa.h;
	aireb = bboxb.w * bboxb.h;

	return airea > aireb ? 1 : -1;
}

int saa_paile_compare_nbar_sup(const SaaPAile * pa, const SaaPAile * pb) {
	int nba = saa_liste_taille(pa->aretes);
	int nbb = saa_liste_taille(pb->aretes);

	return nba > nbb ? 1 : -1;
}

int saa_paile_compare_nbar_inf(const SaaPAile * pa, const SaaPAile * pb) {
	int nba = saa_liste_taille(pa->aretes);
	int nbb = saa_liste_taille(pb->aretes);

	return nba < nbb ? 1 : -1;
}

#define TAILLE_PATRON 84

static char * BasePatronPS[] = {
"/titleFont   /TimesNewRoman findfont 20 scalefont def",
"/infosFont   /TimesNewRoman findfont 14 scalefont def",
"/areteFont   /Courier findfont 8 scalefont def",
"",
"% arete : nomarete angle posx posy longueur languette txtmode",
"/arete {",
"    /m exch def",
"    /b exch def",
"    /l exch def",
"    /y exch def",
"    /x exch def",
"    /angle exch def",
"    /n exch def",
"    /LARGEUR LARG_LANGUE l LARG_LANGUE 2 mul sub 0 min 2 div add def",
"    % on dessine l'arete",
"    gsave",
"    x y translate",
"    angle rotate",
"    0 0 moveto",
"    l 0 rlineto",
"    stroke",
"    % on ecrit son nom",
"    m 1 eq {",
"        0 0 moveto",
"        l n stringwidth pop sub 2 div 3 rmoveto",
"        n show",
"    } if",
"    m 2 eq {",
"        0 0 moveto",
"        l n stringwidth pop sub 2 div 4 LARG_LANGUE sub rmoveto",
"        n show",
"    } if",
"    % eventuellement on dessine la facette de recollement",
"    b {",
"        0 0 moveto",
"        LARGEUR LARGEUR neg rlineto",
"        l LARGEUR 2 mul sub 0 rlineto",
"        LARGEUR LARGEUR rlineto",
"        stroke",
"    } if",
"    grestore",
"} bind def",
"",
"% (arete)* nb angle posx posy facette",
"/facette {",
"    /y exch def",
"    /x exch def",
"    /angle exch def",
"    /nb exch def",
"    gsave",
"    areteFont setfont",
"    x y translate",
"    angle rotate",
"    nb { arete } repeat",
"    grestore",
"} bind def",
"",
"% nom infogen infopage patron",
"/patron {",
"    /infopage exch def",
"    /infogen exch def",
"    /nom exch def",
"    % on dessine une belle bordure",
"    BORDURE                      BORDURE                     moveto",
"    LARG_PAGE BORDURE 2 mul sub  0                           rlineto",
"    0                            HAUT_PAGE BORDURE 2 mul sub rlineto",
"    BORDURE 2 mul LARG_PAGE sub  0                           rlineto",
"    0                            BORDURE 2 mul HAUT_PAGE sub rlineto",
"    stroke",
"    % on ecrit le titre",
"    titleFont setfont",
"    LARG_PAGE nom stringwidth pop sub 2 div HAUT_PAGE BORDURE 5 sub sub moveto",
"    nom show",
"    % on ecrit les info de generation",
"    infosFont setfont",
"    BORDURE BORDURE 2 div moveto",
"    infogen show",
"    % on ecrit les info de page",
"    infosFont setfont",
"    LARG_PAGE BORDURE sub infopage stringwidth pop sub BORDURE 2 div moveto",
"    infopage show",
"} bind def",
"",
"% et maintenant les donnees"
};


// Genere un fichier PostScript
// mode Ailes Separees
int saa_solide_exporter_PS(const SaaSolide * sol, const char * filename, SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, int echelle) {
	SaaPatron * pat = saa_patron_creer(modeP, modeL, modeT, sol, echelle, 210, 297, 15, 5);
	int res = saa_patron_2_ps(pat, filename);
	saa_patron_detruire(pat);

	return res;
}

// Generation d'un fichier Scalable Vector Graphics
int saa_solide_exporter_SVG10(const SaaSolide * sol, const char * filename, SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, int echelle) {
	SaaPatron * pat = saa_patron_creer(modeP, modeL, modeT, sol, echelle, 210, 297, 15, 5);
	int res = saa_patron_2_svg10(pat, filename);
	saa_patron_detruire(pat);

	return res;
}

int saa_solide_exporter_SVG12(const SaaSolide * sol, const char * filename, SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, int echelle) {
	SaaPatron * pat = saa_patron_creer(modeP, modeL, modeT, sol, echelle, 210, 297, 15, 5);
	int res = saa_patron_2_svg12(pat, filename);
	saa_patron_detruire(pat);

	return res;
}

void saa_point_transformer(SaaPoint * p, float angle, const SaaPoint * vecteur) {
	float x = p->x, ca = cos(angle);
	float y = p->y, sa = sin(angle);

	p->x = vecteur->x + x*ca - y*sa;
	p->y = vecteur->y + x*sa + y*ca;
}

void saa_bbox_transformer(SaaBBox * bbox, float angle, const SaaPoint * vecteur) {
	SaaListe * points = saa_liste_creer();
	SaaPoint p1, p2, p3, p4;
	float xmin, ymin, xmax, ymax;

	p1.x = p4.x = bbox->x;
	p1.y = p2.y = bbox->y;
	p2.x = p3.x = bbox->x + bbox->w;
	p3.y = p4.y = bbox->y + bbox->h;

	saa_point_transformer(&p1, angle, vecteur);
	saa_point_transformer(&p2, angle, vecteur);
	saa_point_transformer(&p3, angle, vecteur);
	saa_point_transformer(&p4, angle, vecteur);
	saa_liste_ajouter(points, &p1);
	saa_liste_ajouter(points, &p2);
	saa_liste_ajouter(points, &p3);
	saa_liste_ajouter(points, &p4);

	saa_liste_trier(points, (SaaCompareFunc)saa_point_compare_x);
	xmin = ((SaaPoint *)saa_liste_element(points, 0))->x;
	xmax = ((SaaPoint *)saa_liste_element(points, 3))->x;
	saa_liste_trier(points, (SaaCompareFunc)saa_point_compare_y);
	ymin = ((SaaPoint *)saa_liste_element(points, 0))->y;
	ymax = ((SaaPoint *)saa_liste_element(points, 3))->y;

	saa_liste_detruire(points);

	bbox->x = xmin;
	bbox->y = ymin;
	bbox->w = xmax - xmin;
	bbox->h = ymax - ymin;
}

SaaPArete * saa_parete_creer(const SaaPoint * position, float angle, float longueur, int languette, const char * nom) {
	SaaPArete * newar = (SaaPArete *)g_malloc(sizeof(SaaPArete));

	newar->angle = angle;
	newar->languette = languette;
	newar->longueur = longueur;
	newar->nom = g_strdup(nom);
	newar->position.x = position->x;
	newar->position.y = position->y;

	return newar;
}
void saa_parete_detruire(SaaPArete * par) {
	g_free(par->nom);
	g_free(par);
}

void saa_parete_bbox(const SaaPArete * par, SaaBBox * bbox) {
	float x = par->position.x + par->longueur*cos(par->angle);
	float y = par->position.y + par->longueur*sin(par->angle);

	bbox->x = x < par->position.x ? x : par->position.x;
	bbox->y = y < par->position.y ? y : par->position.y;
	bbox->w = fabs(par->longueur*cos(par->angle));
	bbox->h = fabs(par->longueur*sin(par->angle));
}

void saa_parete_points(const SaaPArete * par, SaaPoint * p1, SaaPoint * p2) {
	SaaBBox bbox;
	float angle = saa_mod2pi(par->angle);
	saa_parete_bbox(par, &bbox);
	if((angle >= 0 && angle < M_PI/2) || (angle >= M_PI && angle < 3*M_PI/2)) {
		p1->x = bbox.x;
		p1->y = bbox.y;
		p2->x = bbox.x + bbox.w;
		p2->y = bbox.y + bbox.h;
	} else if((angle >= M_PI/2 && angle < M_PI) || (angle >= 3*M_PI/2 && angle < 2*M_PI)) {
		p1->x = bbox.x + bbox.w;
		p1->y = bbox.y;
		p2->x = bbox.x;
		p2->y = bbox.y + bbox.h;
	}
}

SaaPAile *  saa_paile_creer(SaaListe * aretes, const char * nom) {
	SaaPAile * newai = (SaaPAile *)g_malloc(sizeof(SaaPAile));

	newai->angle = 0;
	newai->aretes = aretes;
	newai->nom = g_strdup(nom);
	newai->position.x = 0;
	newai->position.y = 0;

	return newai;
}

void saa_paile_detruire(SaaPAile * pai) {
	saa_liste_pour_chaque(pai->aretes, (SaaPourChaqueFunc)saa_parete_detruire, NULL);
	g_free(pai->nom);
	g_free(pai);
}

void saa_paile_bbox(const SaaPAile * pai, SaaBBox * bbox) {
	int numar, nbar = saa_liste_taille(pai->aretes);
	float xmin, xmax, ymin, ymax;
	SaaBBox bboxar;
	SaaPArete * par;

	bbox->x = bbox->y = bbox->w = bbox->h = 0;
	for(numar=0; numar<nbar; numar++) {
		par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
		saa_parete_bbox(par, &bboxar);

		if(numar == 0) {
			bbox->x = bboxar.x;
			bbox->y = bboxar.y;
			bbox->w = bboxar.w;
			bbox->h = bboxar.h;
		} else {
			xmin = bboxar.x < bbox->x ? bboxar.x : bbox->x;
			ymin = bboxar.y < bbox->y ? bboxar.y : bbox->y;
			xmax = bboxar.x + bboxar.w > bbox->x + bbox->w ? bboxar.x + bboxar.w : bbox->x + bbox->w;
			ymax = bboxar.y + bboxar.h > bbox->y + bbox->h ? bboxar.y + bboxar.h : bbox->y + bbox->h;

			bbox->x = xmin;
			bbox->y = ymin;
			bbox->w = xmax - xmin;
			bbox->h = ymax - ymin;
		}
	}

	saa_bbox_transformer(bbox, pai->angle, &pai->position);
}

void saa_paile_bbox_liste(const SaaListe * ailes, SaaBBox * bbox) {
	int numai, nbai = saa_liste_taille(ailes);
	float xmin, xmax, ymin, ymax;
	SaaBBox bboxai;
	SaaPAile * pai;

	bbox->x = bbox->y = bbox->w = bbox->h = 0;
	for(numai=0; numai<nbai; numai++) {
		pai = (SaaPAile *)saa_liste_element(ailes, numai);
		saa_paile_bbox(pai, &bboxai);
//printf("bbox ai%d : %f %f %f %f\n", numai, bboxai.x, bboxai.y, bboxai.w, bboxai.h);
		if(numai == 0) {
			bbox->x = bboxai.x;
			bbox->y = bboxai.y;
			bbox->w = bboxai.w;
			bbox->h = bboxai.h;
		} else {
			xmin = bboxai.x < bbox->x ? bboxai.x : bbox->x;
			ymin = bboxai.y < bbox->y ? bboxai.y : bbox->y;
			xmax = bboxai.x + bboxai.w > bbox->x + bbox->w ? bboxai.x + bboxai.w : bbox->x + bbox->w;
			ymax = bboxai.y + bboxai.h > bbox->y + bbox->h ? bboxai.y + bboxai.h : bbox->y + bbox->h;

			bbox->x = xmin;
			bbox->y = ymin;
			bbox->w = xmax - xmin;
			bbox->h = ymax - ymin;
		}
	}
}

SaaPPage * saa_ppage_creer() {
	SaaPPage * newp = (SaaPPage *)g_malloc(sizeof(SaaPPage));

	newp->ailes = saa_liste_creer();
	newp->position.x = 0;
	newp->position.y = 0;
	newp->angle = 0;

	return newp;
}

void saa_ppage_detruire(SaaPPage * pp) {
	saa_liste_detruire(pp->ailes);
	g_free(pp);
}

void saa_ppage_bbox(const SaaPPage * pp, SaaBBox * bbox) {
	saa_paile_bbox_liste(pp->ailes, bbox);
	saa_bbox_transformer(bbox, pp->angle, &pp->position);
}

SaaPatron * saa_patron_creer(SaaPatronMode modeP, SaaLangMode modeL, SaaTexteMode modeT, const SaaSolide * sol, float echelle, float pageW, float pageH, float bordure, float languette) {
	SaaPatron * newpat = (SaaPatron *)g_malloc(sizeof(SaaPatron));
	SaaSommet v;
	SaaAile * ai;
	SaaArete * ar, * sar;
	SaaPAile * pai;
	SaaPArete * par;
	SaaPoint posar;
	SaaListe * listeAretes, * listeAiles;
	int numai;
	char sbuf[16];
	float angle, lar;
	int l, nbar1, nbar2;

	newpat->mode = modeP;
	newpat->langMode = modeL;
	newpat->texteMode = modeT;
	newpat->pageW = pageW * 5; // 5 pixels/mm
	newpat->pageH = pageH * 5; // 5 pixels/mm
	newpat->bordure = bordure * 5;
	newpat->languette = languette * 5;
	newpat->pages = saa_liste_creer();
	newpat->nom = g_strdup(sol->nom);


	listeAiles = saa_liste_creer();

	for(numai=0; numai<sol->nbAiles; numai++) {
		ai = sol->lesAiles[numai];
		listeAretes = saa_liste_creer();
		posar.x = posar.y = 0;
		angle = 0;
		ar = ai->start;
		do {
			saa_sommet_difference(ar->s1, ar->s2, &v);
			lar = saa_sommet_distance(&v)*echelle;
			sprintf(sbuf, "ar%d", 1+saa_solide_index_arete(sol, ar));

			switch(modeL) {
			case SAA_LANG_AUCUNE : l = 0; break;
			case SAA_LANG_NORMAL : l = (ar->ai1 == ai); break;
			case SAA_LANG_AILE_MAX :
				if(ar->ai1 == ai) {
					nbar1 = saa_aile_nb_aretes(ar->ai1);
					nbar2 = saa_aile_nb_aretes(ar->ai2);
				} else {
					nbar1 = saa_aile_nb_aretes(ar->ai2);
					nbar2 = saa_aile_nb_aretes(ar->ai1);
				}
				l = (nbar1 == nbar2) ? (ar->ai1 == ai) : (nbar1 > nbar2);
				//printf("MAX ar%d : %d >? %d : %d\n", 1+saa_solide_index_arete(sol, ar), nbar1, nbar2, l);
				break;
			case SAA_LANG_AILE_MIN :
				if(ar->ai1 == ai) {
					nbar1 = saa_aile_nb_aretes(ar->ai1);
					nbar2 = saa_aile_nb_aretes(ar->ai2);
				} else {
					nbar1 = saa_aile_nb_aretes(ar->ai2);
					nbar2 = saa_aile_nb_aretes(ar->ai1);
				}
				l = (nbar1 == nbar2) ? (ar->ai1 == ai) : (nbar1 < nbar2);
				//printf("MIN ar%d : %d <? %d : %d\n", 1+saa_solide_index_arete(sol, ar), nbar1, nbar2, l);
				break;
			case SAA_LANG_TOUTES : l = 1;
			}
			par = saa_parete_creer(&posar, angle, lar, l, sbuf);
			saa_liste_ajouter(listeAretes, par);

			sar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
			posar.x += lar*cos(angle);
			posar.y += lar*sin(angle);
			angle += saa_arete_calculer_angle(ar, sar);
			ar = sar;
		} while(ar != ai->start);

		sprintf(sbuf, "ai%d", 1+numai);
		pai = saa_paile_creer(listeAretes, sbuf);
		saa_liste_ajouter(listeAiles, pai);
	}

	switch(modeP) {
	case SAA_PATRON_AILES_SEPAREES:
		saa_patron_arranger_1(newpat, listeAiles); break;
	case SAA_PATRON_AILES_COLLEES:
		saa_patron_arranger_2(newpat, listeAiles); break;
	}

	saa_liste_detruire(listeAiles);

	return newpat;
}

void saa_patron_detruire(SaaPatron * patron) {
	saa_liste_pour_chaque(patron->pages, (SaaPourChaqueFunc)saa_ppage_detruire, NULL);
	g_free(patron->nom);
	g_free(patron);
}

void saa_patron_arranger_1(SaaPatron * patron, SaaListe * ailes) {
	SaaPPage * page;
	SaaPAile * pai;
	SaaPoint posai;
	SaaBBox bboxai, bboxliste;
	int numai, nbai;

	saa_liste_trier(ailes, (SaaCompareFunc)saa_paile_compare_nbar_sup);
	nbai = saa_liste_taille(ailes);

	page = saa_ppage_creer();
	posai.x = posai.y = patron->bordure + patron->languette;

	for(numai=0; numai<nbai; numai++) {
		pai = (SaaPAile *)saa_liste_element(ailes, numai);

		saa_paile_bbox_liste(page->ailes, &bboxliste);
		saa_paile_bbox(pai, &bboxai);

		if(posai.x - bboxai.x + bboxai.w > patron->pageW - (patron->bordure+patron->languette)) {
			posai.x = patron->bordure + patron->languette;
			posai.y = bboxliste.y + bboxliste.h + 2*patron->languette;
		}

		if(posai.y - bboxai.y + bboxai.h > patron->pageH - (patron->bordure+patron->languette)) {
			posai.x = posai.y = patron->bordure + patron->languette;
			saa_liste_ajouter(patron->pages, page);
			page = saa_ppage_creer();
		}

		saa_liste_ajouter(page->ailes, pai);
		pai->position.x = posai.x - bboxai.x;
		pai->position.y = posai.y - bboxai.y;
		posai.x += bboxai.w + 2*patron->languette;
	}
	saa_liste_ajouter(patron->pages, page);
}


void saa_patron_arranger_2(SaaPatron * patron, SaaListe * ailes) {
	int numar, nbar;
	int numarc, nbarc, trouvar;
	int numai, nbai;
	SaaPAile * pai, * paic;
	SaaPArete * par, * parc;
	SaaPoint p1, p2, p1c, p2c;
	SaaPPage * page;
	SaaBBox bboxpg;

	saa_liste_trier(ailes, (SaaCompareFunc)saa_paile_compare_nbar_inf);

	page = saa_ppage_creer();
	pai = (SaaPAile *)saa_liste_element(ailes, 0);
	saa_liste_enlever(ailes, pai);
	saa_liste_ajouter(page->ailes, pai);

	while(saa_liste_taille(ailes)) {

		nbar = saa_liste_taille(pai->aretes);
		for(numar=0; numar<nbar; numar++) {
			par = (SaaPArete *)saa_liste_element(pai->aretes, numar);

			trouvar = 0;
			nbai = saa_liste_taille(ailes);
			for(numai=0; numai<nbai && !trouvar; numai++) {
				paic = (SaaPAile *)saa_liste_element(ailes, numai);

				nbarc = saa_liste_taille(paic->aretes);
				for(numarc=0; numarc<nbarc && !trouvar; numarc++) {
					parc = (SaaPArete *)saa_liste_element(paic->aretes, numarc);
					trouvar = !strcmp(parc->nom, par->nom);
				}
			}
			if(trouvar) {
				saa_liste_enlever(ailes, paic);
				saa_liste_ajouter(page->ailes, paic);
				par->languette = parc->languette = 0;
				paic->angle = par->angle - parc->angle + M_PI;
				saa_parete_points(par, &p1, &p2);
				saa_point_transformer(&p1, pai->angle, &pai->position);
				saa_point_transformer(&p2, pai->angle, &pai->position);
				saa_parete_points(parc, &p1c, &p2c);
				saa_point_transformer(&p1c, paic->angle, &paic->position);
				saa_point_transformer(&p2c, paic->angle, &paic->position);
				paic->position.x = p1.x - p1c.x;
				paic->position.y = p1.y - p1c.y;
			}
		}

		saa_ppage_bbox(page, &bboxpg);
		saa_liste_ajouter(patron->pages, page);
		page = saa_ppage_creer();
		if(trouvar)
			pai = paic;
		else if(saa_liste_taille(ailes))
			pai = (SaaPAile *)saa_liste_element(ailes, 0);
	}
}

int saa_patron_2_svg10(const SaaPatron * patron, const char * filename) {
	FILE * f;
	char * filename_page, * base;
	int numpg, nbpg;
	int numai, nbai;
	int numar, nbar;
	SaaPPage * pg;
	SaaPAile * pai;
	SaaPArete * par;
	SaaListe * ailes;
	float angleDeg;
	float lang;

	base = g_strdup(filename);
	base[strlen(filename)-4] = '\0';
	filename_page = (char *)g_malloc(strlen(filename)+4);

	nbpg = saa_liste_taille(patron->pages);
	for(numpg=0; numpg<nbpg; numpg++) {
		pg = (SaaPPage *)saa_liste_element(patron->pages, numpg);
		ailes = pg->ailes;

		sprintf(filename_page, "%s-%d.svg", base, numpg+1);

		f = fopen(filename_page, "w");
		if(!f) {
			fprintf(SaaErrorFile, "saa_patron_2_svg10 : Impossible d'ouvrir le fichier %s\n", filename_page);
			return 1;
		}

		fprintf(f, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
		fprintf(f, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
		fprintf(f, "<svg version=\"1.1\" width=\"%.0fmm\" height=\"%.0fmm\" viewBox=\"0 0 %.0f %.0f\" xmlns=\"http://www.w3.org/2000/svg\">\n", patron->pageW/5, patron->pageH/5, patron->pageW, patron->pageH);
		fprintf(f, "\t<rect x=\"%.0f\" y=\"%.0f\" width=\"%.0f\" height=\"%.0f\" fill=\"none\" stroke=\"black\"/>\n", patron->bordure, patron->bordure, patron->pageW-2*patron->bordure, patron->pageH-2*patron->bordure);
		fprintf(f, "\t<text x=\"%.0f\" y=\"%.0f\" font-family=\"Verdana\" font-size=\"20\" text-anchor=\"middle\">%s</text>\n", patron->pageW/2, patron->bordure-10, patron->nom);
		fprintf(f, "\t<text x=\"%.0f\" y=\"%.0f\" font-family=\"Verdana\" font-size=\"14\" text-anchor=\"start\">Generated with %s - %s</text>\n", patron->bordure, patron->pageH-patron->bordure/2, PACKAGE_NAME, PACKAGE_VERSION);
		fprintf(f, "\t<text x=\"%.0f\" y=\"%.0f\" font-family=\"Verdana\" font-size=\"14\" text-anchor=\"end\">Page %d / %d</text>\n", patron->pageW-patron->bordure, patron->pageH-patron->bordure/2, numpg+1, nbpg);

		nbai = saa_liste_taille(ailes);
		for(numai=0; numai<nbai; numai++) {
			pai = (SaaPAile *)saa_liste_element(ailes, numai);
			angleDeg = pai->angle/M_PI*180;
			fprintf(f, "\t<g id=\"%s\" transform=\"translate(%f, %f) rotate(%f)\">\n", pai->nom, pai->position.x, pai->position.y, angleDeg);
			nbar = saa_liste_taille(pai->aretes);
			// Tracés
			for(numar=0; numar<nbar; numar++) {
				par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
				angleDeg = par->angle/M_PI*180;

				fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);
				fprintf(f, "\t\t\t<path stroke=\"black\" fill=\"none\" d=\"M 0 0 H %f Z\"/>\n", par->longueur);
				fprintf(f, "\t\t</g>\n");
			}
			// Languettes
			switch(patron->langMode) {
			case SAA_LANG_AUCUNE : break;
			case SAA_LANG_AILE_MAX :
			case SAA_LANG_AILE_MIN :
			case SAA_LANG_NORMAL :
				for(numar=0; numar<nbar; numar++) {
					par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
					angleDeg = par->angle/M_PI*180;

					fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);
					if(par->languette) {
						lang = par->longueur < 2*patron->languette ? par->longueur - patron->languette : patron->languette;
						fprintf(f, "\t\t\t<path stroke=\"black\" fill=\"none\" d=\"M %f 0 L %f %f L %f %f L 0 0 Z\"/>\n", par->longueur, par->longueur-lang, -lang, lang, -lang);
					}
					fprintf(f, "\t\t</g>\n");
				}
				break;
			case SAA_LANG_TOUTES :
				for(numar=0; numar<nbar; numar++) {
					par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
					angleDeg = par->angle/M_PI*180;

					fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);
					lang = par->longueur < 2*patron->languette ? par->longueur - patron->languette : patron->languette;
					fprintf(f, "\t\t\t<path stroke=\"black\" fill=\"none\" d=\"M %f 0 L %f %f L %f %f L 0 0 Z\"/>\n", par->longueur, par->longueur-lang, -lang, lang, -lang);
					fprintf(f, "\t\t</g>\n");
				}
				break;
			}

			// Noms
			for(numar=0; numar<nbar; numar++) {
				par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
				angleDeg = par->angle/M_PI*180;

				fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);


				switch(patron->texteMode) {
					case SAA_TEXT_AUCUN: break;
					case SAA_TEXT_DESSOUS:
						fprintf(f, "\t\t\t<text text-anchor=\"middle\" font-size=\"12\" font-family=\"Verdana\" transform=\"translate(%f,%f) rotate(180)\">%s</text>\n", par->longueur/2, 5., par->nom);
						break;
					case SAA_TEXT_DESSUS:
						fprintf(f, "\t\t\t<text text-anchor=\"middle\" font-size=\"12\" font-family=\"Verdana\" transform=\"translate(%f,%f) rotate(180)\">%s</text>\n", par->longueur/2, -17., par->nom);
						break;
				}

				fprintf(f, "\t\t</g>\n");

			}
			fprintf(f, "\t</g>\n");
		}

		fprintf(f, "</svg>\n");
		fclose(f);
	}
	g_free(base);
	g_free(filename_page);

	return 0;
}

int saa_patron_2_svg12(const SaaPatron * patron, const char * filename) {
	FILE * f;
	int numpg, nbpg;
	int numai, nbai;
	int numar, nbar;
	SaaPPage * pg;
	SaaPAile * pai;
	SaaPArete * par;
	SaaListe * ailes;
	float angleDeg;
	float lang;

	nbpg = saa_liste_taille(patron->pages);
	f = fopen(filename, "w");
	if(!f) {
		fprintf(SaaErrorFile, "saa_patron_2_svg12 : Impossible d'ouvrir le fichier %s\n", filename);
		return 1;
	}

	fprintf(f, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fprintf(f, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.2//EN\" \"http://www.w3.org/Graphics/SVG/1.2/DTD/svg12.dtd\">\n");
	fprintf(f, "<svg version=\"1.2\" width=\"%.0fmm\" height=\"%.0fmm\" viewBox=\"0 0 %.0f %.0f\" xmlns=\"http://www.w3.org/2000/svg\">\n", patron->pageW/5, patron->pageH/5, patron->pageW, patron->pageH);

	fprintf(f, "\t<rect x=\"%.0f\" y=\"%.0f\" width=\"%.0f\" height=\"%.0f\" fill=\"none\" stroke=\"black\"/>\n", patron->bordure, patron->bordure, patron->pageW-2*patron->bordure, patron->pageH-2*patron->bordure);
	fprintf(f, "\t<text x=\"%.0f\" y=\"%.0f\" font-family=\"Verdana\" font-size=\"20\" text-anchor=\"middle\">%s</text>\n", patron->pageW/2, patron->bordure-10, patron->nom);
	fprintf(f, "\t<text x=\"%.0f\" y=\"%.0f\" font-family=\"Verdana\" font-size=\"14\" text-anchor=\"start\">Generated with %s - %s</text>\n\n", patron->bordure, patron->pageH-patron->bordure/2, PACKAGE_NAME, PACKAGE_VERSION);
	fprintf(f, "\t<pageSet>\n\n");

	for(numpg=0; numpg<nbpg; numpg++) {
		pg = (SaaPPage *)saa_liste_element(patron->pages, numpg);
		ailes = pg->ailes;
		fprintf(f, "\t<page>\n");
		fprintf(f, "\t<text x=\"%.0f\" y=\"%.0f\" font-family=\"Verdana\" font-size=\"14\" text-anchor=\"end\">Page %d / %d</text>\n", patron->pageW-patron->bordure, patron->pageH-patron->bordure/2, numpg+1, nbpg);

		nbai = saa_liste_taille(ailes);
		for(numai=0; numai<nbai; numai++) {
			pai = (SaaPAile *)saa_liste_element(ailes, numai);
			angleDeg = pai->angle/M_PI*180;
			fprintf(f, "\t<g id=\"%s\" transform=\"translate(%f, %f) rotate(%f)\">\n", pai->nom, pai->position.x, pai->position.y, angleDeg);
			nbar = saa_liste_taille(pai->aretes);
			// Tracés
			for(numar=0; numar<nbar; numar++) {
				par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
				angleDeg = par->angle/M_PI*180;

				fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);
				fprintf(f, "\t\t\t<path stroke=\"black\" fill=\"none\" d=\"M 0 0 H %f Z\"/>\n", par->longueur);
				fprintf(f, "\t\t</g>\n");
			}
			// Languettes
			switch(patron->langMode) {
			case SAA_LANG_AUCUNE : break;
			case SAA_LANG_AILE_MAX :
			case SAA_LANG_AILE_MIN :
			case SAA_LANG_NORMAL :
				for(numar=0; numar<nbar; numar++) {
					par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
					angleDeg = par->angle/M_PI*180;

					fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);
					if(par->languette) {
						lang = par->longueur < 2*patron->languette ? par->longueur - patron->languette : patron->languette;
						fprintf(f, "\t\t\t<path stroke=\"black\" fill=\"none\" d=\"M %f 0 L %f %f L %f %f L 0 0 Z\"/>\n", par->longueur, par->longueur-lang, -lang, lang, -lang);
					}
					fprintf(f, "\t\t</g>\n");
				}
				break;
			case SAA_LANG_TOUTES :
				for(numar=0; numar<nbar; numar++) {
					par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
					angleDeg = par->angle/M_PI*180;

					fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);
					lang = par->longueur < 2*patron->languette ? par->longueur - patron->languette : patron->languette;
					fprintf(f, "\t\t\t<path stroke=\"black\" fill=\"none\" d=\"M %f 0 L %f %f L %f %f L 0 0 Z\"/>\n", par->longueur, par->longueur-lang, -lang, lang, -lang);
					fprintf(f, "\t\t</g>\n");
				}
				break;
			}

			// Noms
			for(numar=0; numar<nbar; numar++) {
				par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
				angleDeg = par->angle/M_PI*180;

				fprintf(f, "\t\t<g transform=\"translate(%f, %f) rotate(%f)\">\n", par->position.x, par->position.y, angleDeg);

				switch(patron->texteMode) {
					case SAA_TEXT_AUCUN: break;
					case SAA_TEXT_DESSOUS:
						fprintf(f, "\t\t\t<text text-anchor=\"middle\" font-size=\"12\" font-family=\"Verdana\" transform=\"translate(%f,%f) rotate(180)\">%s</text>\n", par->longueur/2, 5., par->nom);
						break;
					case SAA_TEXT_DESSUS:
						fprintf(f, "\t\t\t<text text-anchor=\"middle\" font-size=\"12\" font-family=\"Verdana\" transform=\"translate(%f,%f) rotate(180)\">%s</text>\n", par->longueur/2, -17., par->nom);
						break;
				}

				fprintf(f, "\t\t</g>\n");

			}
			fprintf(f, "\t</g>\n");
		}


		fprintf(f, "\t</page>\n\n");
	}
	fprintf(f, "\t</pageSet>\n");

	fprintf(f, "</svg>\n");

	return 0;
}

int saa_patron_2_ps(const SaaPatron * patron, const char * filename) {
	FILE * f;
	int i;
	int numpg, nbpg;
	int numai, nbai;
	int numar, nbar;
	SaaPPage * page;
	SaaPAile * pai;
	SaaPArete * par;
	SaaListe * ailes;
	float angleDeg, ratio = 595/patron->pageW;

	f = fopen(filename, "w");
	if(!f) {
		fprintf(SaaErrorFile, "saa_patron_2_ps : Impossible d'ouvrir le fichier %s\n", filename);
		return 1;
	}

	nbpg = saa_liste_taille(patron->pages);

	fprintf(f, "%%!PS-Adobe-2.0\n");
	fprintf(f, "%sDocumentMedia: A4 595 842 0 () ()\n", "%%");
	fprintf(f, "%sCreator: %s - %s\n", "%%", PACKAGE_NAME, PACKAGE_VERSION);
	fprintf(f, "%sTitle: %s\n", "%%", patron->nom);
	fprintf(f, "%sPages: %d\n", "%%", nbpg);

	fprintf(f, "/LARG_PAGE   %.0f def\n", patron->pageW*ratio);
	fprintf(f, "/HAUT_PAGE   %.0f def\n", patron->pageH*ratio);
	fprintf(f, "/LARG_LANGUE %.0f def\n", patron->languette*ratio);
	fprintf(f, "/BORDURE     %.0f def\n", patron->bordure*ratio);

	for(i=0; i<TAILLE_PATRON; i++) {
		fprintf(f, "%s\n", BasePatronPS[i]);
	}

	for(numpg=0; numpg<nbpg; numpg++) {
		page = (SaaPPage *)saa_liste_element(patron->pages, numpg);
		ailes = page->ailes;
		fprintf(f, "%sPage: %d %d\n", "%%", numpg+1, numpg+1);
		nbai = saa_liste_taille(ailes);
		for(numai=0; numai<nbai; numai++) {
			pai = (SaaPAile *)saa_liste_element(ailes, numai);
			nbar = saa_liste_taille(pai->aretes);
			for(numar=0; numar<nbar; numar++) {
				par = (SaaPArete *)saa_liste_element(pai->aretes, numar);
				angleDeg = par->angle/M_PI*180;
			    fprintf(f, "(%s) %4.4f %4.4f %4.4f %4.4f %s %d\n", par->nom, angleDeg, par->position.x*ratio, par->position.y*ratio, par->longueur*ratio, par->languette ? "true" : "false", patron->texteMode);
			}
			angleDeg = pai->angle/M_PI*180;
			fprintf(f, "%d %4.4f %4.4f %4.4f facette\n", nbar, angleDeg, pai->position.x*ratio, pai->position.y*ratio);
		}
		fprintf(f, "(%s) (Genere par %s - %s) (Page %d/%d) patron\nshowpage\n", patron->nom, PACKAGE_NAME, PACKAGE_VERSION, numpg+1, nbpg);
	}
	fprintf(f, "%sEOF\n", "%%");
	fclose(f);

	return 0;
}
