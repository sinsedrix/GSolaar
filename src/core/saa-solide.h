/******************************* saa-solide.h ********************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Modélisation arêtes ailees des polyèdres                                */
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
#ifndef _SAA_SOLIDE_
#define _SAA_SOLIDE_

#include "kaleido.h"
#include "saa-geometrie.h"

#define PRECISION		18

#define SOLIDE_DIR		"./solides/"

struct SaaAile;
// Structure definissant un couple de demi-aretes
typedef struct SaaArete {
  struct SaaArete  * succ1, * succ2; // Aretes suivantes pour chaque aile
  struct SaaAile   * ai1,   * ai2;   // Les ailes
  SaaSommet        * s1,    * s2;    // Les sommets
} SaaArete;

// Structure definissant une aile
typedef struct SaaAile {
  SaaArete        * start;                  // Arete de depart
  //struct SaaAile ** sousAiles;              // Ailes découpées
  int               nbSousAiles;
} SaaAile;

// Structure d'un solide selon un modele aretes-ailees
typedef struct SaaSolide {
  char        * nom;
  SaaAile    ** lesAiles;
  SaaArete   ** lesAretes;
  SaaSommet  ** lesSommets;
  int           nbAiles;
  int           nbAretes;
  int           nbSommets;
  SaaSommet  ** lesNormales; // Tableau des vecteurs normaux aux ailes
  Polyhedron  * p;
} SaaSolide;

/* Operations sur les solides aretes ailees */
//////////////////////////////////////////////
// Alloue de la memoire pour un nouveau solide
SaaSolide * saa_solide_creer(const char * nom);

// Detruit un solide en liberant de espace memoire
void saa_solide_detruire(SaaSolide * s);

// Cree un solide grace au module kaleido
// symbol : numero precede d'un # ou bien symbole de Wythoff
SaaSolide * saa_solide_creer_kaleido(const char * symbol);

// Cree le dual d'un solide de kaleido
// symbol : numero precede d'un # ou bien symbole de Wythoff
SaaSolide * saa_solide_creer_kaleido_dual(const char * symbol);

// Cree le compose de 2 solides
SaaSolide * saa_solide_creer_compose(const SaaSolide * sol1, const SaaSolide * sol2);

// Cree un solide a partir d'un fichier de script
// filename : nom du fichier script
SaaSolide * saa_solide_importer_TXT(const char * filename);

// Cree un solide a partir d'un fichier XML
// filename : nom du fichier XML
SaaSolide * saa_solide_importer_XML(const char * filename);

// Generation d'un fichier script
void saa_solide_exporter_TXT(const SaaSolide * s, const char * filename);

// Generation d'un fichier XML
void saa_solide_exporter_XML(const SaaSolide * s, const char * filename);

// Affichage d'un solide sur un flux qq
void saa_solide_ecrire_txt(const SaaSolide * sol, FILE * f);
void saa_solide_ecrire_aile_txt(const SaaSolide * sol, FILE * f, const SaaAile * ai);
void saa_solide_ecrire_arete_txt(const SaaSolide * sol, FILE * f, const SaaArete * ar);
void saa_solide_ecrire_sommet_txt(const SaaSolide * sol, FILE * f, const SaaSommet * s);

// Affichage d'un solide au format XML sur un flux qq
void saa_solide_ecrire_xml(const SaaSolide * sol, FILE * f);
void saa_solide_ecrire_aile_xml(const SaaSolide * sol, FILE * f, const SaaAile * ai);
void saa_solide_ecrire_arete_xml(const SaaSolide * sol, FILE * f, const SaaArete * ar);
void saa_solide_ecrire_sommet_xml(const SaaSolide * sol, FILE * f, const SaaSommet * s);

// Ajout d'un sommet au solide
// condition : il n'y est pas deja et il n'est pas trop proche d'un autre sommet
// valeur renvoyee : addresse de ce sommet dans le solide
SaaSommet * saa_solide_ajouter_sommet(SaaSolide * sol, const SaaSommet * s);

// Ajout d'une arete au solide
// condition : elle n'y est pas deja referencee
// valeur renvoyee : addresse de cette arete dans le solide
SaaArete * saa_solide_ajouter_arete(SaaSolide * sol, SaaArete * ar);

// Ajout d'une aile au solide
// condition : elle n'y est pas deja referencee
// valeur renvoyee : addresse de cette aile dans le solide
SaaAile * saa_solide_ajouter_aile(SaaSolide * sol, SaaAile * ai);

// Suppression d'un sommet dans un solide
// note : on met le dernier sommet a sa place dans le tableau
void saa_solide_supprimer_sommet(SaaSolide * sol, SaaSommet * s);

// Suppression d'une arete dans un solide
// note : on met la derniere arete a sa place dans le tableau
void saa_solide_supprimer_arete(SaaSolide * sol, SaaArete * ar);

// Suppression d'une aile dans un solide
// note : on met la derniere aile a sa place dans le tableau
void saa_solide_supprimer_aile(SaaSolide * sol, SaaAile * ai);

void saa_solide_barycentre(const SaaSolide * sol, SaaSommet * g);

// Suppression des sommets en double ou utilses par aucune arete
void saa_solide_nettoyer_sommets(SaaSolide * sol);

// Suppression des aretes utilisees par aucune aile
void saa_solide_nettoyer_aretes(SaaSolide * sol);

// Suppression des ailes en double (ou +)
void saa_solide_nettoyer_ailes_doubles(SaaSolide * sol);
// Suppression des ailes isolees
void saa_solide_nettoyer_ailes_isolees(SaaSolide * sol);

// Numero d'un sommet dans un solide
// valeur renvoyee : entre 0 et sol->nbSommets
// si le sommet est trouve, -1 sinon
int saa_solide_index_sommet(const SaaSolide * sol, const SaaSommet * s);

// Numero d'une arete dans un solide
// valeur renvoyee : entre 0 et sol->nbAretes
// si le sommet est trouve, -1 sinon
int saa_solide_index_arete(const SaaSolide * sol, const SaaArete * ar);

// Numero d'une aile dans un solide
// valeur renvoyee : entre 0 et sol->nbAretes
// si le sommet est trouve, -1 sinon
int saa_solide_index_aile(const SaaSolide * sol, const SaaAile * ai);

/* Operations sur les aretes */
///////////////////////////////
// Angle entre 2 aretes pour une aile commune
double saa_arete_calculer_angle(const SaaArete * ar1, const SaaArete * ar2);

// Teste si deux aretes ont les memes points
int saa_arete_comparer(const SaaArete * ar1, const SaaArete * ar2);

/* Operations sur les ailes */
//////////////////////////////
// Calcule le barycentre d'une aile
// g est deja alloue et on y stocke le resultat
void saa_aile_barycentre(const SaaAile * ai, SaaSommet * g);

// Calcule le vecteur normal a une aile
// n est deja alloue et on y stocke le resultat
void saa_aile_vecteur_normal(const SaaAile * ai, SaaSommet * n);

// Calcule le plan contenant l'aile
void saa_aile_plan(const SaaAile * ai, SaaPlan * pai);

// Indique le cote d'un sommet par rapport a une aile
// le calcul est fait grace a l'equation du plan de ai
int saa_aile_cote_sommet(const SaaAile * ai, const SaaSommet * s);

// Calcule le sommet d'intersection entre le plan de l'aile et un segment
//  valeur de retour : cote des deux points par rapport a l'aile
int saa_aile_intersection_segment(const SaaAile * ai, const SaaSommet * A, const SaaSommet * B, SaaSommet * I);

// Calcule si 2 ailes sont en intersection
// retourne : le nombre de sommets en intersection
int saa_aile_intersection(const SaaAile * ai1, const SaaAile * ai2);

// Test de point interieur a une aile
int saa_aile_contient_sommet(const SaaAile * ai, const SaaSommet * s);

// Teste si une aile est croisee (ou degeneree)
int saa_aile_est_croisee(const SaaAile * ai);

// Teste si deux ailes ont la meme succession de points
int saa_aile_comparer(const SaaAile * ai1, const SaaAile * ai2);

// Test de point interieur
// par decoupe de l'aile en triangles
int saa_aile_contient_sommet(const SaaAile * ai, const SaaSommet * s);

// Nombre d'aretes dans l'aile
int saa_aile_nb_aretes(const SaaAile * ai);

#endif //_SAA_SOLIDE_
