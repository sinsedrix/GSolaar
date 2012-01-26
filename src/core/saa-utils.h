/******************************* saa-utils.h *********************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Utilitaires                                                             */
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
#ifndef _SAA_UTILS_H_
#define _SAA_UTILS_H_

#include <stdio.h>
#include <glib.h>

// Quelques macro
#ifndef MAX
#define MAX(a,b)    ((a>b) ? (a):(b))
#endif
#ifndef MIN
#define MIN(a,b)    ((a<b) ? (a):(b))
#endif
#ifndef XOR
#define XOR(a,b)    ((a)&&!(b) || !(a)&&(b))
#endif

typedef struct SaaListeElt {
	void * donnee;
	struct SaaListeElt * suivant;
} SaaListeElt;

typedef struct SaaListe {
	SaaListeElt * premier;
} SaaListe;

// Modèle de fonction pour saa_liste_pour_chaque
// return : booleen pour continuer
typedef int (* SaaPourChaqueFunc) (void * e, void * donnee);

// Modèle de fonction pour saa_liste_trier
// return : 0 si eA = eB, >0 si eA < eB, <0 si eA > eB
typedef int (* SaaCompareFunc) (void * eA, void * eB);

SaaListe * saa_liste_creer();
void       saa_liste_detruire(SaaListe * l);
int        saa_liste_taille(const SaaListe * l);
void       saa_liste_ajouter(SaaListe * l, void * e);
void	   saa_liste_ajouter_unique(SaaListe * l, void * e, SaaCompareFunc f);
void       saa_liste_enlever(SaaListe * l, void * e);
void *     saa_liste_element(const SaaListe * l, int i);
int        saa_liste_index(const SaaListe * l, void * e);
void       saa_liste_pour_chaque(SaaListe * l, SaaPourChaqueFunc f, void * donnee);
void       saa_liste_trier(SaaListe * l, SaaCompareFunc f);

// Programme pipelette
extern FILE * SaaErrorFile;
extern FILE * SaaLogFile;

void saa_init_files();

// Fait des jolis noms avec des majuscules (titre)
void saa_casse_chameau(char * s);

// Fait un nom sobre sans espace (nom de fichier)
void saa_sans_espace(char * s);

// Récupération du répertoire locale
const gchar * saa_locale_directory (void);

#endif //_SAAUTILS_H_
