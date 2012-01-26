/***************************** saa-drawingarea.c *****************************/
/*                                                                           */
/*   Widget GtkGLArea affichant un SaaVisuel                                 */
/*                                                                           */
/*   Ecrit par : Cédric COULIOU                                              */
/*                                                                           */
/*   GSolaar Copyright (c) (2003-2008) Cédric COULIOU                        */
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
