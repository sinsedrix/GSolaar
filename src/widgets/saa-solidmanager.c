/***************************** saa-solidmanager.c ****************************/
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
#include "saa-solidmanager.h"

#include "saa-i18n.h"

static void saa_solid_manager_radio_numero_toggled(SaaSolidManager * mgr, GtkToggleButton * togglebutton);
static void saa_solid_manager_numero_changed(SaaSolidManager * mgr, GtkButton * button);
static void saa_solid_manager_radio_wythoff_toggled(SaaSolidManager * mgr, GtkToggleButton *togglebutton);
static void saa_solid_manager_wythoff_changed(SaaSolidManager * mgr, GtkButton * button);
static void saa_solid_manager_dual_toggled(SaaSolidManager * mgr, GtkToggleButton * togglebutton);
static void saa_solid_manager_radio_filename_toggled(SaaSolidManager * mgr, GtkButton * button);
static void saa_solid_manager_filename_changed(SaaSolidManager * mgr, GtkButton * button);

enum {
  SOLID_CHANGED_SIGNAL,
  LAST_SIGNAL
};

static guint solid_manager_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (SaaSolidManager, saa_solid_manager, GTK_TYPE_TABLE)

static void
saa_solid_manager_class_init (SaaSolidManagerClass *klass) {
	solid_manager_signals[SOLID_CHANGED_SIGNAL] =
	    g_signal_new ("solid-changed",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaSolidManagerClass, solid_changed),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

}

static void
saa_solid_manager_init (SaaSolidManager * mgr) {
	GtkWidget * but_numero, * but_wythoff, * but_filename;
	GtkWidget * image1, * image2, * image3;
	GtkObject * sbut_numero_adj;
	const gchar * filename_path;

	gtk_table_resize(GTK_TABLE(mgr), 3, 5);
	gtk_table_set_homogeneous(GTK_TABLE(mgr), FALSE);

	gtk_table_set_row_spacings (GTK_TABLE (mgr), 5);
	gtk_table_set_col_spacings (GTK_TABLE (mgr), 5);
	gtk_container_set_border_width (GTK_CONTAINER (mgr), 5);

	// Numero
	mgr->radio_numero = gtk_radio_button_new_with_label_from_widget (NULL, _("Number:"));
	gtk_widget_set_name(mgr->radio_numero, "numero-rad");
	gtk_widget_show (mgr->radio_numero);
	gtk_table_attach (GTK_TABLE (mgr), mgr->radio_numero, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	sbut_numero_adj = gtk_adjustment_new (1, 1, 80, 1, 10, 0);
	mgr->spin_numero = gtk_spin_button_new (GTK_ADJUSTMENT (sbut_numero_adj), 1, 0);
	gtk_widget_set_name(mgr->spin_numero, "numero-ent");
	gtk_widget_show (mgr->spin_numero);
	gtk_table_attach (GTK_TABLE (mgr), mgr->spin_numero, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
	gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (mgr->spin_numero), TRUE);
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (mgr->spin_numero), TRUE);

	but_numero = gtk_button_new ();
	gtk_widget_set_name(but_numero, "numero-btn");
	gtk_widget_show (but_numero);
	gtk_table_attach (GTK_TABLE (mgr), but_numero, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	image1 = gtk_image_new_from_stock ("gtk-refresh", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image1);
	gtk_container_add (GTK_CONTAINER (but_numero), image1);

	// Wythoff
	mgr->radio_wythoff = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (mgr->radio_numero), _("Wythoff symbol:"));
	gtk_widget_set_name(mgr->radio_wythoff, "wythoff-rad");
	gtk_widget_show (mgr->radio_wythoff);
	gtk_table_attach (GTK_TABLE (mgr), mgr->radio_wythoff, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

	mgr->entry_wythoff = gtk_entry_new ();
	gtk_widget_set_name(mgr->entry_wythoff, "wythoff-ent");
	gtk_widget_show (mgr->entry_wythoff);
	gtk_table_attach (GTK_TABLE (mgr), mgr->entry_wythoff, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
	gtk_entry_set_has_frame (GTK_ENTRY (mgr->entry_wythoff), FALSE);

	but_wythoff = gtk_button_new ();
	gtk_widget_set_name(but_wythoff, "wythoff-btn");
	gtk_widget_show (but_wythoff);
	gtk_table_attach (GTK_TABLE (mgr), but_wythoff, 2, 3, 1, 2, GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

	image2 = gtk_image_new_from_stock ("gtk-refresh", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image2);
	gtk_container_add (GTK_CONTAINER (but_wythoff), image2);

	// Dual
	mgr->but_dual = gtk_toggle_button_new_with_mnemonic (_("Dual"));
	gtk_widget_set_name(mgr->but_dual, "numero-wythoff-dual-btn");
	gtk_widget_show (mgr->but_dual);
	gtk_table_attach (GTK_TABLE (mgr), mgr->but_dual, 3, 4, 0, 2, GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

	// Filename
	mgr->radio_filename = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (mgr->radio_wythoff), _("File:"));
	gtk_widget_set_name(mgr->radio_filename, "filename-rad");
	gtk_widget_show (mgr->radio_filename);
	gtk_table_attach (GTK_TABLE (mgr), mgr->radio_filename, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0);

	mgr->but_filename = gtk_file_chooser_button_new (_("Load a solid"), GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_widget_set_name(mgr->but_filename, "filename-but");
	gtk_widget_show (mgr->but_filename);
	gtk_table_attach (GTK_TABLE (mgr), mgr->but_filename, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
	filename_path = g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS);
	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(mgr->but_filename), filename_path);


	but_filename = gtk_button_new ();
	gtk_widget_set_name(but_filename, "filename-btn");
	gtk_widget_show (but_filename);
	gtk_table_attach (GTK_TABLE (mgr), but_filename, 2, 3, 2, 3, GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

	image3 = gtk_image_new_from_stock ("gtk-refresh", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image3);
	gtk_container_add (GTK_CONTAINER (but_filename), image3);

	// Nom
	mgr->label_nom = gtk_label_new ("");
	gtk_widget_set_name(mgr->label_nom, "numero-wythoff-filename-lbl");
	gtk_widget_show (mgr->label_nom);
	gtk_table_attach (GTK_TABLE (mgr), mgr->label_nom, 0, 4, 4, 5, GTK_FILL, GTK_FILL, 0, 0);
	gtk_label_set_justify (GTK_LABEL (mgr->label_nom), GTK_JUSTIFY_CENTER);

	g_signal_connect_swapped ((gpointer) mgr->radio_numero, "toggled",
			G_CALLBACK (saa_solid_manager_radio_numero_toggled),
			mgr);
	g_signal_connect_swapped ((gpointer) mgr->radio_wythoff, "toggled",
			G_CALLBACK (saa_solid_manager_radio_wythoff_toggled),
			mgr);
	g_signal_connect_swapped ((gpointer) mgr->radio_filename, "toggled",
			G_CALLBACK (saa_solid_manager_radio_filename_toggled),
			mgr);

	g_signal_connect_swapped ((gpointer) but_numero, "clicked",
			G_CALLBACK (saa_solid_manager_numero_changed),
			mgr);

	g_signal_connect_swapped ((gpointer) but_wythoff, "clicked",
			G_CALLBACK (saa_solid_manager_wythoff_changed),
			mgr);

	g_signal_connect_swapped ((gpointer) but_filename, "clicked",
			G_CALLBACK (saa_solid_manager_filename_changed),
			mgr);

	g_signal_connect_swapped ((gpointer) mgr->but_dual, "toggled",
			G_CALLBACK (saa_solid_manager_dual_toggled),
			mgr);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mgr->radio_wythoff), TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mgr->radio_numero), TRUE);
}

GtkWidget*
saa_solid_manager_new_with_visuel(SaaVisuel * visuel) {
	GObject * obj = g_object_new (SAA_TYPE_SOLID_MANAGER, NULL);

	SAA_SOLID_MANAGER(obj)->visuel = visuel;

	if(visuel->solide->p && visuel->solide->p->polyform) {
		gtk_entry_set_text(GTK_ENTRY(SAA_SOLID_MANAGER(obj)->entry_wythoff), visuel->solide->p->polyform);
	}
	if(visuel->solide->nom) {
		gtk_label_set_text(GTK_LABEL(SAA_SOLID_MANAGER(obj)->label_nom), visuel->solide->nom);
	}
	if(visuel->solide->p) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(SAA_SOLID_MANAGER(obj)->spin_numero), visuel->solide->p->index+1);
	}

	switch(visuel->source) {
	case SAA_NUMERO:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_MANAGER(obj)->radio_numero), TRUE);
		break;
	case SAA_WYTHOFF:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_MANAGER(obj)->radio_wythoff), TRUE);
		break;
	case SAA_FILENAME:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_MANAGER(obj)->radio_filename), TRUE);
		break;
	}

	g_signal_handlers_block_by_func(SAA_SOLID_MANAGER(obj)->but_dual, saa_solid_manager_dual_toggled, obj);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_MANAGER(obj)->but_dual), visuel->dual);
	g_signal_handlers_unblock_by_func(SAA_SOLID_MANAGER(obj)->but_dual, saa_solid_manager_dual_toggled, obj);

	return GTK_WIDGET(obj);
}

void activate_family_cbk(GtkWidget * w, gpointer data) {
	gchar * family = (gchar *)data;
	const gchar * name = gtk_widget_get_name(w);
	gchar ** subs = g_strsplit(name, "-", 5);
	gint i;
	gboolean active = FALSE;
	for(i=0; subs[i] != NULL && !active; i++) {
		active |= g_ascii_strcasecmp("rad", subs[i]) == 0 || g_ascii_strcasecmp(family, subs[i]) == 0;
	}
	gtk_widget_set_sensitive(w, active);
}

void
saa_solid_manager_radio_numero_toggled(SaaSolidManager * mgr, GtkToggleButton * togglebutton) {
	gtk_container_foreach(GTK_CONTAINER(mgr), activate_family_cbk, "numero");
}

void
saa_solid_manager_radio_wythoff_toggled(SaaSolidManager * mgr, GtkToggleButton *togglebutton) {
	gtk_container_foreach(GTK_CONTAINER(mgr), activate_family_cbk, "wythoff");
}

void saa_solid_manager_radio_filename_toggled(SaaSolidManager * mgr, GtkButton * button) {
	gtk_container_foreach(GTK_CONTAINER(mgr), activate_family_cbk, "filename");
}

void
saa_solid_manager_numero_changed (SaaSolidManager * mgr, GtkButton * button) {
	const gchar * numero = gtk_entry_get_text(GTK_ENTRY(mgr->spin_numero));
	GLboolean dual = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(mgr->but_dual));
	GtkWidget * dialog, * main_window;

	if(!saa_visuel_construire(mgr->visuel, SAA_NUMERO, numero, dual)) {
		main_window = gtk_widget_get_toplevel (GTK_WIDGET(mgr));
		dialog = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail to create polyhedra %s\n(likely non-orientable)"),
		                                  numero);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy (dialog);
	} else {
		if(mgr->visuel->solide->p && mgr->visuel->solide->p->polyform)
			gtk_entry_set_text(GTK_ENTRY(mgr->entry_wythoff), mgr->visuel->solide->p->polyform);

		if(mgr->visuel->solide->nom)
			gtk_label_set_text(GTK_LABEL(mgr->label_nom), mgr->visuel->solide->nom);

		g_signal_emit (G_OBJECT (mgr), solid_manager_signals[SOLID_CHANGED_SIGNAL], 0);
	}
}

void
saa_solid_manager_wythoff_changed(SaaSolidManager * mgr, GtkButton * button) {
	const gchar * symbol = gtk_entry_get_text(GTK_ENTRY(mgr->entry_wythoff));
	GLboolean dual = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(mgr->but_dual));
	GtkWidget * dialog, * main_window;

	if(!saa_visuel_construire(mgr->visuel, SAA_WYTHOFF, symbol, dual)) {
		main_window = gtk_widget_get_toplevel (GTK_WIDGET(mgr));
		dialog = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail to create polyhedra %s\n(likely non-orientable)"),
		                                  symbol);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy (dialog);
	} else {
		if(mgr->visuel->solide->p)
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(mgr->spin_numero), mgr->visuel->solide->p->index+1);

		if(mgr->visuel->solide->nom)
			gtk_label_set_text(GTK_LABEL(mgr->label_nom), mgr->visuel->solide->nom);

		g_signal_emit (G_OBJECT (mgr), solid_manager_signals[SOLID_CHANGED_SIGNAL], 0);
	}
}

void
saa_solid_manager_dual_toggled(SaaSolidManager * mgr, GtkToggleButton * togglebutton) {
	GLboolean dual = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(mgr->but_dual));
	gboolean numero_selected = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(mgr->radio_numero));
	gboolean wythoff_selected = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(mgr->radio_wythoff));
	GtkWidget * dialog, * main_window;
	const gchar * txt = NULL;
	SaaVisuelSource source;

	if(numero_selected) {
		source = SAA_NUMERO;
		txt = gtk_entry_get_text(GTK_ENTRY(mgr->spin_numero));
	} else if(wythoff_selected) {
		source = SAA_WYTHOFF;
		txt = gtk_entry_get_text(GTK_ENTRY(mgr->entry_wythoff));
	}

	if(txt && !saa_visuel_construire(mgr->visuel, source, txt, dual)) {
		main_window = gtk_widget_get_toplevel (GTK_WIDGET(mgr));
		dialog = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail to create polyhedra %s\n(likely non-orientable)"),
		                                  txt);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy (dialog);
	} else {
		if(mgr->visuel->solide->nom)
			gtk_label_set_text(GTK_LABEL(mgr->label_nom), mgr->visuel->solide->nom);

		g_signal_emit (G_OBJECT (mgr), solid_manager_signals[SOLID_CHANGED_SIGNAL], 0);
	}
}

void saa_solid_manager_filename_changed(SaaSolidManager * mgr, GtkButton * button) {
	const gchar * filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(mgr->but_filename));
	GtkWidget * warning, * main_window = gtk_widget_get_toplevel (GTK_WIDGET(mgr));

	if(g_file_test(filename, G_FILE_TEST_IS_DIR) || !g_file_test(filename, G_FILE_TEST_EXISTS)) {
		warning = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_ERROR,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Please select a file"));
		gtk_dialog_run(GTK_DIALOG(warning));
		gtk_widget_destroy (warning);
	} else if(!saa_visuel_construire(mgr->visuel, SAA_FILENAME, filename, GL_FALSE)) {
		warning = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail to load the file %s"),
		                                  filename);
		gtk_dialog_run(GTK_DIALOG(warning));
		gtk_widget_destroy (warning);
	} else {
		if(mgr->visuel->solide->nom)
			gtk_label_set_text(GTK_LABEL(mgr->label_nom), mgr->visuel->solide->nom);

		g_signal_emit (G_OBJECT (mgr), solid_manager_signals[SOLID_CHANGED_SIGNAL], 0);
	}
}

void saa_solid_manager_set_filename(SaaSolidManager * mgr, const char * filename) {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mgr->radio_filename), TRUE);
	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(mgr->but_filename), filename);
}
