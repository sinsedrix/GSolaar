/******************************* saa-treeview.h ******************************/
/*                                                                           */
/*   Widget Treeview affichant un SaaVisuel                                  */
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

#ifndef __SAA_TREE_VIEW_H__
#define __SAA_TREE_VIEW_H__

#include <gtk/gtk.h>
#include "saa-visuel.h"

#define SAA_TYPE_TREE_VIEW          	 (saa_tree_view_get_type ())
#define SAA_TREE_VIEW(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), SAA_TYPE_TREE_VIEW, SaaTreeView))
#define SAA_TREE_VIEW_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), SAA_TYPE_AREA, SaaTreeViewClass))
#define SAA_IS_TREE_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SAA_TYPE_TREE_VIEW))
#define SAA_IS_TREE_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), SAA_TYPE_TREE_VIEW))
#define SAA_TREE_VIEW_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), SAA_TYPE_TREE_VIEW, SaaTreeViewClass))

typedef struct SaaTreeView {
	GtkTreeView		parent;

	SaaVisuel	  * visuel;
	GtkTreeStore  * store;
} SaaTreeView;

typedef struct SaaTreeViewClass {
	GtkTreeViewClass		parent_class;

	void (* vertex_selected) (SaaTreeView *treeview, SaaSommet * s);
	void (* edge_selected) (SaaTreeView *treeview, SaaArete * ar);
	void (* wing_selected) (SaaTreeView *treeview, SaaAile * ai);

} SaaTreeViewClass;


GType		saa_tree_view_get_type			(void);

GtkWidget*  saa_tree_view_new_with_visuel	(SaaVisuel * visuel);
void 		saa_tree_view_update			(SaaTreeView * treeview);
void		saa_tree_view_select_vertex		(SaaTreeView * treeview, SaaSommet * s);
void		saa_tree_view_select_edge		(SaaTreeView * treeview, SaaArete * ar);
void		saa_tree_view_select_wing		(SaaTreeView * treeview, SaaAile * ai);

#endif // __SAA_TREE_VIEW_H__
