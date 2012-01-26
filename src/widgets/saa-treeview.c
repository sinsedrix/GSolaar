/******************************* saa-treeview.c ******************************/
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

#include "saa-core.h"
#include "saa-treeview.h"

#include "saa-i18n.h"

static void saa_tree_view_row_activated(SaaTreeView * tv, GtkTreePath * path, GtkTreeViewColumn * column, gpointer user_data);

enum {
	IDENT_COLUMN,
	VALUE_COLUMN,
	N_COLUMNS
};

enum {
  VERTEX_SELECTED_SIGNAL,
  EDGE_SELECTED_SIGNAL,
  WING_SELECTED_SIGNAL,
  LAST_SIGNAL
};

static guint tree_view_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (SaaTreeView, saa_tree_view, GTK_TYPE_TREE_VIEW)

static void
saa_tree_view_class_init (SaaTreeViewClass *klass) {
	tree_view_signals[VERTEX_SELECTED_SIGNAL] =
	    g_signal_new ("vertex-selected",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaTreeViewClass, vertex_selected),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);

	tree_view_signals[EDGE_SELECTED_SIGNAL] =
	    g_signal_new ("edge-selected",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaTreeViewClass, edge_selected),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);

	tree_view_signals[WING_SELECTED_SIGNAL] =
	    g_signal_new ("wing-selected",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaTreeViewClass, wing_selected),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);
}

static void
saa_tree_view_init (SaaTreeView *treeview) {
	GtkCellRenderer * cell;
	GtkTreeSelection * select = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));

	gtk_tree_selection_set_mode (select, GTK_SELECTION_BROWSE);

	cell = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
								    -1, "",
								    cell, "text",
								    IDENT_COLUMN,
								    NULL);

	cell = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
								    -1, "",
								    cell, "text",
								    VALUE_COLUMN,
								    NULL);

	g_signal_connect_swapped((gpointer)treeview, "row-activated",
			G_CALLBACK (saa_tree_view_row_activated), treeview);
}


GtkWidget*
saa_tree_view_new_with_visuel (SaaVisuel * visuel) {
	GObject * obj = g_object_new (SAA_TYPE_TREE_VIEW, NULL);

	SAA_TREE_VIEW(obj)->visuel = visuel;
	SAA_TREE_VIEW(obj)->store = gtk_tree_store_new (N_COLUMNS,
												G_TYPE_STRING,
												G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(obj), GTK_TREE_MODEL(SAA_TREE_VIEW(obj)->store));

	return GTK_WIDGET(obj);
}

void saa_tree_view_update(SaaTreeView * treeview) {
	GtkTreeStore * store = treeview->store;
	SaaSolide * sol = treeview->visuel->solide;
	SaaSommet * s;
	SaaArete * ar;
	SaaAile * ai;
	GtkTreeIter iterg, iters, iterar, iterai;
	int i;
	gchar * ident, * value, * tmpvalue;

	gtk_tree_store_clear(store);

	// Ajout des sommets
	gtk_tree_store_append(store, &iterg, NULL);
	gtk_tree_store_set (store, &iterg, IDENT_COLUMN, _("Vertices"), -1);
	for(i=0; i<sol->nbSommets; i++) {
		s = sol->lesSommets[i];
		ident = g_strdup_printf("s%d", i+1);
		value = g_strdup_printf("(%+f\t%+f\t%+f)", s->x, s->y, s->z);
		gtk_tree_store_append(store, &iters, &iterg);
		gtk_tree_store_set (store, &iters, IDENT_COLUMN, ident, VALUE_COLUMN, value, -1);
	}

	// Ajout des aretes
	gtk_tree_store_append(store, &iterg, NULL);
	gtk_tree_store_set (store, &iterg, IDENT_COLUMN, _("Edges"), -1);
	for(i=0; i<sol->nbAretes; i++) {
		ar = sol->lesAretes[i];
		ident = g_strdup_printf("ar%d", i+1);
		value = g_strdup_printf("s%d - s%d\tai%d - ai%d\tar%d - ar%d",
				saa_solide_index_sommet(sol, ar->s1)+1, saa_solide_index_sommet(sol, ar->s2)+1,
				saa_solide_index_aile(sol, ar->ai1)+1, saa_solide_index_aile(sol, ar->ai2)+1,
				saa_solide_index_arete(sol, ar->succ1)+1, saa_solide_index_arete(sol, ar->succ2)+1);
		gtk_tree_store_append(store, &iterar, &iterg);
		gtk_tree_store_set(store, &iterar, IDENT_COLUMN, ident, VALUE_COLUMN, value, -1);
	}

	gtk_tree_store_append(store, &iterg, NULL);
	gtk_tree_store_set (store, &iterg, IDENT_COLUMN, _("Wings"), -1);
	for(i=0; i<sol->nbAiles; i++) {
		ai = sol->lesAiles[i];
		ident = g_strdup_printf("ai%d", i+1);
		ar = ai->start;
		tmpvalue = g_strdup("");
		do {
			value = g_strdup_printf("%s ar%d", tmpvalue, saa_solide_index_arete(sol, ar)+1);
			g_free(tmpvalue);
			tmpvalue = value;
			ar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
		} while(ar != ai->start);
		gtk_tree_store_append(store, &iterai, &iterg);
		gtk_tree_store_set(store, &iterai, IDENT_COLUMN, ident, VALUE_COLUMN, value, -1);
	}
}

void saa_tree_view_select_vertex(SaaTreeView * treeview, SaaSommet * s) {
	GtkTreeSelection * sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	int i = saa_solide_index_sommet(treeview->visuel->solide, s);
	GtkTreePath * path = gtk_tree_path_new_from_indices(0, i, -1);

	gtk_tree_view_expand_to_path (GTK_TREE_VIEW(treeview), path);
	gtk_tree_selection_select_path(sel, path);
	gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(treeview), path, NULL, TRUE, 0.5, 0);
	gtk_tree_path_free (path);
}

void saa_tree_view_select_edge(SaaTreeView * treeview, SaaArete * ar) {
	GtkTreeSelection * sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	int i = saa_solide_index_arete(treeview->visuel->solide, ar);
	GtkTreePath * path = gtk_tree_path_new_from_indices(1, i, -1);

	gtk_tree_view_expand_to_path (GTK_TREE_VIEW(treeview), path);
	gtk_tree_selection_select_path(sel, path);
	gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(treeview), path, NULL, TRUE, 0.5, 0);
	gtk_tree_path_free (path);
}

void saa_tree_view_select_wing(SaaTreeView * treeview, SaaAile * ai) {
	GtkTreeSelection * sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	int i = saa_solide_index_aile(treeview->visuel->solide, ai);
	GtkTreePath * path = gtk_tree_path_new_from_indices(2, i, -1);

	gtk_tree_view_expand_to_path (GTK_TREE_VIEW(treeview), path);
	gtk_tree_selection_select_path(sel, path);
	gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(treeview), path, NULL, TRUE, 0.5, 0);
	gtk_tree_path_free (path);
}

void saa_tree_view_row_activated(SaaTreeView * tv, GtkTreePath * path, GtkTreeViewColumn * column, gpointer user_data) {
	gchar * spath = gtk_tree_path_to_string(path);
	gchar ** splited = g_strsplit(spath, ":", -1);
	int ind1, ind2;

	g_return_if_fail(splited[1]);

	ind1 = atoi(splited[0]);
	ind2 = atoi(splited[1]);
	g_free(spath);
	g_strfreev(splited);

	//g_debug("row_activated %d %d", ind1, ind2);

	switch(ind1) {
	case 0:
		g_signal_emit (G_OBJECT (tv), tree_view_signals[VERTEX_SELECTED_SIGNAL], 0, tv->visuel->solide->lesSommets[ind2]);
		break;
	case 1:
		g_signal_emit (G_OBJECT (tv), tree_view_signals[EDGE_SELECTED_SIGNAL], 0, tv->visuel->solide->lesAretes[ind2]);
		break;
	case 2:
		g_signal_emit (G_OBJECT (tv), tree_view_signals[WING_SELECTED_SIGNAL], 0, tv->visuel->solide->lesAiles[ind2]);
		break;
	}
}
