/******************************* saa-utils.h *********************************/
/*                                                                           */
/*   Biblioth�que statique de base :                                         */
/*   Utilitaires                                                             */
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

// Mod�le de fonction pour saa_liste_pour_chaque
// return : booleen pour continuer
typedef int (* SaaPourChaqueFunc) (void * e, void * donnee);

// Mod�le de fonction pour saa_liste_trier
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

// R�cup�ration du r�pertoire locale
const gchar * saa_locale_directory (void);

#endif //_SAAUTILS_H_
