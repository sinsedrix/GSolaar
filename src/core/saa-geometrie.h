/****************************** saa-geometrie.h ******************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Calculs de géométrie affine sur les sommets                             */
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
#ifndef _SAA_GEOMETRIE_H_
#define _SAA_GEOMETRIE_H_

#include "saa-utils.h"

// Precision des calculs
#define EPSILON_1        1E-10 // double
#define EPSILON_2        1E-8  // double*double
#define EPSILON_3        1E-6  // double*double*double

// Structure definissant un sommet (ou un vecteur)
typedef struct SaaSommet {
  double x, y, z;
} SaaSommet;

typedef struct SaaPlan {
	double a, b, c, d;
} SaaPlan;

/* Operations sur les sommets ou vecteurs */
////////////////////////////////////////////
// Cree un nouveau sommet
SaaSommet * saa_sommet_creer(double x, double y, double z);
SaaSommet * saa_sommet_copier(const SaaSommet * s);

// Distance d'un sommet a l'origine (ou norme d'un vecteur)
double saa_sommet_distance(const SaaSommet * s);

// Produit scalaire entre 2 vecteurs
double saa_sommet_ps(const SaaSommet * v1, const SaaSommet * v2);

// Soustraction de 2 sommets (vecteur)
// v est deja alloue et on y stocke le resultat
void saa_sommet_difference(const SaaSommet * s1, const SaaSommet * s2, SaaSommet * v);

// Addition de 2 vecteurs
// v est deja alloue et on y stocke le resultat
void saa_sommet_ajout(const SaaSommet * s1, const SaaSommet * s2, SaaSommet * v);

// Produit vectoriel entre 2 vecteurs
// pv est deja alloue et on y stocke le resultat
void saa_sommet_pv(const SaaSommet * s1, const SaaSommet * s2, SaaSommet * pv);

// Angle entre deux vecteurs appliques a un sommet
double saa_sommet_calculer_angle(const SaaSommet * s, const SaaSommet * v1, const SaaSommet * v2);

// Cherche si deux sommets sont assimilables
// return : 0 si les sommets sont différents
int saa_sommet_comparer(const SaaSommet * s1, const SaaSommet * s2);

// Calcule l'intersection I entre les segments coplanaires [AB] et [CD]
// return : 0 si pas d'intersection
int saa_sommet_intersection_segments(const SaaSommet * A, const SaaSommet * B, const SaaSommet * C, const SaaSommet * D, SaaSommet * I);

/* Operations sur les sommets ou vecteurs */
////////////////////////////////////////////
// Cree un nouveau plan
SaaPlan * saa_plan_creer(double a, double b, double c, double d);

// Calcule si un sommet est sur un segment
int saa_sommet_est_sur_segment(const SaaSommet * s, const SaaSommet * A, const SaaSommet * B);

// Calcule si un sommet est dans le plan
int saa_plan_cote_sommet(const SaaPlan * p, const SaaSommet * s);

// Calcule l'intersection d'un segment avec le plan
int saa_plan_intersection_segment(const SaaPlan * p, const SaaSommet * A, const SaaSommet * B, SaaSommet * I);

// Vecteur normal normalisé
void saa_plan_vecteur_normal(const SaaPlan * p, SaaSommet * n);

// Projection orthogonale sur un plan
void saa_plan_projeter_ortho(const SaaPlan * p, const SaaSommet * S, SaaSommet * Sp);

// modulo 2pi sur des flottants
double saa_mod2pi(double angle);

#endif //_SAA_GEOMETRIE_H_
