/***************************** saa-traitement.h ******************************/
/*                                                                           */
/*   Bibliothèque statique des traitements :                                 */
/*   Traitement des polyèdres                                                */
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
#include "saa-solide.h"

#ifndef _SAA_TRAITEMENT_
#define _SAA_TRAITEMENT_

// Decoupe d'une arete par un sommet
// valeur renvoyee : addresse de la nouvelle arete
SaaArete * saa_solide_couper_arete(SaaSolide * sol, SaaArete * ar, const SaaSommet * s);

// Coupe une aile par un plan
// valeur renvoyee : addresse de la nouvelle aile
SaaAile * saa_solide_couper_aile(SaaSolide * sol, SaaAile * ai, const SaaPlan * p);

// Fusion de deux demi-aretes
// pour deux demi-aretes ayant deux sommets identiques
void saa_solide_fusionner_demi_aretes(SaaSolide * sol, SaaArete * ar1, SaaArete * ar2);

// Fusion de deux aretes telescopees
// pour deux demi-aretes ayant deux sommets identiques
SaaArete * saa_solide_fusionner_aretes(SaaSolide * sol, SaaArete * ar1, SaaArete * ar2);

// Fusion de deux ailes ayant une arete commune
// pour deux ailes coplanaires
void saa_solide_fusionner_ailes(SaaSolide * sol, SaaArete * ar);

// Segmentation selon les aretes en intersection
int saa_solide_segmenter(SaaSolide * sol);
// Decroisement des ailes du solide
int saa_solide_decroiser(SaaSolide * sol);

// Decoupe des ailes du solide
int saa_solide_decouper(SaaSolide * sol);

// Recuperation du contour du solide
int saa_solide_evider(SaaSolide * sol);

// Fusion des ailes coplanaires
int saa_solide_fusionner(SaaSolide * sol);

#endif //_SAA_TRAITEMENT_
