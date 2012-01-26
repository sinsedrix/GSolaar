/******************************* saa-visuel.h ********************************/
/*                                                                           */
/*   Biblioth�que statique graphique :                                       */
/*   Dessin des poly�dres gr�ce � OpenGL                                     */
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
#include <GL/gl.h>

#ifndef _SAA_GRAPHIC_
#define _SAA_GRAPHIC_

typedef enum SaaVisuelSource {
	SAA_NUMERO,
	SAA_WYTHOFF,
	SAA_FILENAME
} SaaVisuelSource;

typedef enum SaaVisuelSommet {
	SAA_SOMMET_NON,
	SAA_SOMMET_MOLECULE,
	SAA_SOMMET_POINT
} SaaVisuelSommet;

typedef enum SaaVisuelArete {
	SAA_ARETE_NON,
	SAA_ARETE_MOLECULE,
	SAA_ARETE_LIGNE
} SaaVisuelArete;

typedef enum SaaVisuelAile {
	SAA_AILE_NON,
	SAA_AILE_TRIANGULATION,
	SAA_AILE_COULEUR
} SaaVisuelAile;

typedef enum SaaVisuelTraitement {
	SAA_TRAITEMENT_DECOUPAGE,
	SAA_TRAITEMENT_EVIDAGE,
	SAA_TRAITEMENT_FUSION,
	SAA_TRAITEMENT_SEGMENTATION,
	SAA_TRAITEMENT_DECROISEMENT
} SaaVisuelTraitement;

typedef struct SaaVisuel {
	SaaSolide		*	solide;
	SaaSommet		*	centre;
	SaaSommet		*	angle;
	SaaVisuelSource		source;
	char *				texteSource;
	GLboolean			dual;         //
	GLboolean			eclairage;    //
	GLboolean			transparence; //
	GLboolean			axes;
	SaaVisuelSommet		visuSommets;
	SaaVisuelArete		visuAretes;
	SaaVisuelAile		visuAiles;
	GLfloat				tailleSommets;
	GLfloat				tailleAretes;
	GLuint				listesSommets;
	GLuint				listesAretes;
	GLuint				listesAiles;
	GLfloat				couleurSommets[4];
	GLfloat				couleurAretes[4];
	GLfloat			*	couleurAiles;
	GLfloat				couleurFond[3];
	const SaaSommet	*	sel_vertex;
	const SaaArete	*	sel_edge;
	const SaaAile	*	sel_wing;
} SaaVisuel;

SaaVisuel * saa_visuel_creer();
void saa_visuel_detruire();
int saa_visuel_construire(SaaVisuel * visuel, SaaVisuelSource source, const char * texteSource, GLboolean dual);
void saa_visuel_dessiner(SaaVisuel * visuel);
void saa_visuel_initialiser(SaaVisuel * visuel, int w, int h);
void saa_visuel_redimensionner(SaaVisuel * visuel, int w, int h);
GLuint saa_visuel_selectionner(SaaVisuel * visuel, int x, int y);

int saa_visuel_traiter_solide(SaaVisuel * visuel, SaaVisuelTraitement trait);

void saa_visuel_affichage_sommets(SaaVisuel * visuel, SaaVisuelSommet visu);
void saa_visuel_affichage_aretes(SaaVisuel * visuel, SaaVisuelArete visu);
void saa_visuel_affichage_ailes(SaaVisuel * visuel, SaaVisuelAile visu);
void saa_visuel_eclairage(SaaVisuel * visuel, GLboolean onoff);
void saa_visuel_transparence(SaaVisuel * visuel, GLboolean onoff);
void saa_visuel_axes(SaaVisuel * visuel, GLboolean onoff);
void saa_visuel_couleur_fond(SaaVisuel * visuel, GLfloat r, GLfloat g, GLfloat b);
void saa_visuel_couleur_sommets(SaaVisuel * visuel, GLfloat r, GLfloat g, GLfloat b);
void saa_visuel_couleur_aretes(SaaVisuel * visuel, GLfloat r, GLfloat g, GLfloat b);
void saa_visuel_taille_sommets(SaaVisuel * visuel, GLfloat t);
void saa_visuel_taille_aretes(SaaVisuel * visuel, GLfloat t);

#endif //_SAA_GRAPHIC_
