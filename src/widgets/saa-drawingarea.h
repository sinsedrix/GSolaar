/***************************** saa-drawingarea.c *****************************/
/*                                                                           */
/*   Widget GtkGLArea affichant un SaaVisuel                                 */
/*                                                                           */
/*   Ecrit par : C�dric COULIOU                                              */
/*                                                                           */
/*   GSolaar Copyright (c) (2003-2008) C�dric COULIOU                        */
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

#ifndef __SAA_DRAWING_AREA_H__
#define __SAA_DRAWING_AREA_H__

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include "saa-visuel.h"

#define SAA_TYPE_DRAWING_AREA          		(saa_drawing_area_get_type ())
#define SAA_DRAWING_AREA(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), SAA_TYPE_DRAWING_AREA, SaaDrawingArea))
#define SAA_DRAWING_AREA_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), SAA_TYPE_DRAWING_AREA, SaaDrawingAreaClass))
#define SAA_IS_DRAWING_AREA(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SAA_TYPE_DRAWING_AREA))
#define SAA_IS_DRAWING_AREA_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), SAA_TYPE_DRAWING_AREA))
#define SAA_DRAWING_AREA_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), SAA_TYPE_DRAWING_AREA, SaaDrawingAreaClass))

typedef struct SaaDrawingArea {
	GtkDrawingArea		parent;

	SaaVisuel		*	visuel;

	int					xold, yold, motion, animation;
	guint				id;
} SaaDrawingArea;

typedef struct SaaDrawingAreaClass {
	GtkDrawingAreaClass	parent_class;

	void (* vertex_selected) (SaaDrawingArea *area, SaaSommet * s);
	void (* edge_selected)  (SaaDrawingArea *area, SaaArete * ar);
	void (* wing_selected)   (SaaDrawingArea *area, SaaAile * ai);

} SaaDrawingAreaClass;

GType   saa_drawing_area_get_type		 (void);

GtkWidget*  saa_drawing_area_new_with_visuel(SaaVisuel * visuel);

void saa_drawing_area_select_vertex		(SaaDrawingArea  *area, const SaaSommet * s);
void saa_drawing_area_select_edge		(SaaDrawingArea  *area, const SaaArete * ar);
void saa_drawing_area_select_wing		(SaaDrawingArea  *area, const SaaAile * ai);

#endif /* __SAA_DRAWING_AREA_H__ */
