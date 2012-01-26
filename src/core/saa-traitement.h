/***************************** saa-traitement.h ******************************/
/*                                                                           */
/*   Biblioth�que statique des traitements :                                 */
/*   Traitement des poly�dres                                                */
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
