/******************************* saa-treeview.h ******************************/
/*                                                                           */
/*   Widget Treeview affichant un SaaVisuel                                  */
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
