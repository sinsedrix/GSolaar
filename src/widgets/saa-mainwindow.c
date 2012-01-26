/***************************** saa-mainwindow.c ******************************/
/*                                                                           */
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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>

#include "saa-core.h"

#include "saa-export.h"

#include "saa-drawingarea.h"
#include "saa-treeview.h"
#include "saa-solidmanager.h"
#include "saa-soliddisplay.h"
#include "saa-solidhandler.h"
#include "saa-mainwindow.h"
#include "saa-exportdialog.h"

#include "saa-i18n.h"

static void saa_main_window_charger_xml_clicked		(SaaMainWindow * w);
static void saa_main_window_exporter_activate		(SaaMainWindow * w);
static void saa_main_window_exporter_rb_activate	(SaaMainWindow * w);
static void saa_main_window_sauver_xml_activate		(SaaMainWindow * w);
static void saa_main_window_quitter_activate		(SaaMainWindow * w);
static void saa_main_window_a_propos_activate		(SaaMainWindow * w);

G_DEFINE_TYPE (SaaMainWindow, saa_main_window, GTK_TYPE_WINDOW)

static void
saa_main_window_class_init (SaaMainWindowClass *klass) {
}

static void
saa_main_window_init (SaaMainWindow * w) {
	GtkWidget * vbox_win;

	// Barre de menu ----------------------------------------------------------
	GtkWidget *menubar;
	GtkWidget *image;

	GtkWidget *mit_fichier;
	GtkWidget *mit_fichier_menu;
		GtkWidget *it_charger_xml;
		GtkWidget *it_enregistrer_xml;

		GtkWidget *mit_exporter;
		GtkWidget *mit_exporter_menu;
			GtkWidget *it_exporter;
			GtkWidget *it_exporter_rb;

		GtkWidget *it_sep_fichier;
		GtkWidget *it_quitter;

	GtkWidget *mit_aide;
	GtkWidget *mit_aide_menu;
		GtkWidget *it_a_propos;


	GtkWidget *hbox_win;
	GtkWidget *vbox_prop;

	// Onglets ----------------------------------------------------------------
	GtkWidget *notebook;

	GtkWidget *label_page1;
	GtkWidget *page1;

	GtkWidget *f_uniform;
	GtkWidget *lbl_uniform;

	GtkWidget *f_traitement;
	GtkWidget *lbl_traitement;

	GtkWidget *label_page2;
	GtkWidget *page2;

	GtkWidget *sw;

	GtkWidget *label_page3;
	GtkWidget *page3;

	// Barre d'état -----------------------------------------------------------
	GtkWidget *hbox_status;
	GtkWidget *statusbar;
	GtkWidget *progressbar;

	GtkAccelGroup *accel_group;

	int i;
	const gchar* const * dirs = g_get_system_data_dirs();
	gchar * path;

	// TODO init_files();
	saa_init_files();

	g_set_application_name (PACKAGE_NAME);

	w->icon = NULL;
	for(i=0; dirs[i] && !w->icon; i++) {
		path = g_build_filename(dirs[i], "images", "gsolaar-logo.png", NULL);
		if(g_file_test(path, G_FILE_TEST_EXISTS)) {
			w->icon = gdk_pixbuf_new_from_file(path, NULL);
			gtk_window_set_default_icon (w->icon);
		}
		g_free(path);
	}
	w->visuel = saa_visuel_creer();

	accel_group = gtk_accel_group_new ();

	gtk_window_set_title (GTK_WINDOW (w), "GSolaar");

	vbox_win = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_win);
	gtk_container_add (GTK_CONTAINER (w), vbox_win);

	menubar = gtk_menu_bar_new ();
	gtk_widget_show (menubar);
	gtk_box_pack_start (GTK_BOX (vbox_win), menubar, FALSE, FALSE, 0);

	mit_fichier = gtk_menu_item_new_with_mnemonic(_("_File"));
	gtk_widget_show (mit_fichier);
	gtk_container_add (GTK_CONTAINER (menubar), mit_fichier);

	mit_fichier_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mit_fichier), mit_fichier_menu);

	// Chargement -------------------------------------------------------------
	it_charger_xml = gtk_image_menu_item_new_from_stock (GTK_STOCK_OPEN, accel_group);
	gtk_widget_show (it_charger_xml);
	gtk_container_add (GTK_CONTAINER (mit_fichier_menu), it_charger_xml);

	// Sauvegarde -------------------------------------------------------------
	it_enregistrer_xml = gtk_image_menu_item_new_from_stock (GTK_STOCK_SAVE, accel_group);
	gtk_widget_show (it_enregistrer_xml);
	gtk_container_add (GTK_CONTAINER (mit_fichier_menu), it_enregistrer_xml);

	// Patron -----------------------------------------------------------------
	mit_exporter = gtk_image_menu_item_new_with_mnemonic(_("_Export"));
	gtk_widget_show(mit_exporter);
	gtk_container_add (GTK_CONTAINER (mit_fichier_menu), mit_exporter);

	image = gtk_image_new_from_stock (GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
	gtk_widget_show (image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mit_exporter), image);

	mit_exporter_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mit_exporter), mit_exporter_menu);

	it_exporter = gtk_image_menu_item_new_with_label (_("Pattern ..."));
	gtk_widget_show (it_exporter);
	gtk_container_add (GTK_CONTAINER (mit_exporter_menu), it_exporter);

	it_exporter_rb = gtk_image_menu_item_new_with_label (_("Ruby script for SketchUp ..."));
	gtk_widget_show (it_exporter_rb);
	gtk_container_add (GTK_CONTAINER (mit_exporter_menu), it_exporter_rb);

	//-------------------------------------------------------------------------
	it_sep_fichier = gtk_menu_item_new ();
	gtk_widget_show (it_sep_fichier);
	gtk_container_add (GTK_CONTAINER (mit_fichier_menu), it_sep_fichier);
	gtk_widget_set_sensitive (it_sep_fichier, FALSE);

	it_quitter = gtk_image_menu_item_new_from_stock (GTK_STOCK_QUIT, accel_group);
	gtk_widget_show (it_quitter);
	gtk_container_add (GTK_CONTAINER (mit_fichier_menu), it_quitter);

	mit_aide = gtk_menu_item_new_with_mnemonic ("_?");
	gtk_widget_show (mit_aide);
	gtk_container_add (GTK_CONTAINER (menubar), mit_aide);

	mit_aide_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mit_aide), mit_aide_menu);

	it_a_propos = gtk_image_menu_item_new_from_stock ( GTK_STOCK_ABOUT, accel_group);
	gtk_widget_show (it_a_propos);
	gtk_container_add (GTK_CONTAINER (mit_aide_menu), it_a_propos);

	//-------------------------------------------------------------------------
	hbox_win = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_win);
	gtk_box_pack_start (GTK_BOX (vbox_win), hbox_win, TRUE, TRUE, 0);

	vbox_prop = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_prop);
	gtk_box_pack_start (GTK_BOX (hbox_win), vbox_prop, TRUE, TRUE, 0);
	gtk_widget_set_size_request (vbox_prop, 500, 400);

	// Drawing Area
	w->area = saa_drawing_area_new_with_visuel (w->visuel);
	gtk_widget_show (w->area);
	gtk_box_pack_start (GTK_BOX (vbox_prop), w->area, TRUE, TRUE, 0);
	gtk_widget_set_size_request (w->area, 600, 400);

	notebook = gtk_notebook_new();
	gtk_widget_show(notebook);
	gtk_container_set_border_width (GTK_CONTAINER (notebook), 5);
	gtk_box_pack_start (GTK_BOX (hbox_win), notebook, FALSE, FALSE, 0);

	label_page1 = gtk_label_new (_("Polyhedron"));
	gtk_widget_show (label_page1);

	page1 = gtk_vbox_new (FALSE, 5);
	gtk_widget_show (page1);
	gtk_container_set_border_width (GTK_CONTAINER (page1), 5);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page1, label_page1);

	f_uniform = gtk_frame_new (NULL);
	gtk_widget_show (f_uniform);
	gtk_box_pack_start (GTK_BOX (page1), f_uniform, FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (f_uniform), 5);
	gtk_frame_set_label_align (GTK_FRAME (f_uniform), 0.5, 0.5);

	lbl_uniform = gtk_label_new (_("Uniform polyhedra"));
	gtk_widget_show (lbl_uniform);
	gtk_frame_set_label_widget (GTK_FRAME (f_uniform), lbl_uniform);
	gtk_label_set_justify (GTK_LABEL (lbl_uniform), GTK_JUSTIFY_LEFT);

	// Solid manager
	w->manager = saa_solid_manager_new_with_visuel(w->visuel);
	gtk_widget_show(w->manager);
	gtk_container_add (GTK_CONTAINER (f_uniform), w->manager);

	f_traitement = gtk_frame_new (NULL);
	gtk_widget_show (f_traitement);
	gtk_box_pack_start (GTK_BOX (page1), f_traitement, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (f_traitement), 5);
	gtk_frame_set_label_align (GTK_FRAME (f_traitement), 0.5, 0.5);

	lbl_traitement = gtk_label_new (_("Treatment"));
	gtk_widget_show (lbl_traitement);
	gtk_frame_set_label_widget (GTK_FRAME (f_traitement), lbl_traitement);
	gtk_label_set_justify (GTK_LABEL (lbl_traitement), GTK_JUSTIFY_LEFT);

	// Solid Handler
	w->handler = saa_solid_handler_new_with_visuel(w->visuel);
	gtk_widget_show(w->handler);
	gtk_container_add (GTK_CONTAINER (f_traitement), w->handler);

	label_page2 = gtk_label_new (_("Navigation"));
	gtk_widget_show (label_page2);

	page2 = gtk_vbox_new (FALSE, 5);
	gtk_widget_show (page2);
	gtk_container_set_border_width (GTK_CONTAINER (page2), 5);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page2, label_page2);

    sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show(sw);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (page2), sw, TRUE, TRUE, 0);

    // Treeview navigator
	w->explorer = saa_tree_view_new_with_visuel(w->visuel);
	gtk_widget_show(w->explorer);
	gtk_container_add (GTK_CONTAINER (sw), w->explorer);

	// TODO a deplacer dans window realize
	saa_tree_view_update(SAA_TREE_VIEW(w->explorer));

	label_page3 = gtk_label_new (_("Display"));
	gtk_widget_show (label_page3);

	page3 = gtk_vbox_new (FALSE, 5);
	gtk_widget_show (page3);
	gtk_container_set_border_width (GTK_CONTAINER (page3), 5);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page3, label_page3);

	// Solid display
	w->display = saa_solid_display_new_with_visuel(w->visuel);
	gtk_widget_show (w->display);
	gtk_box_pack_start(GTK_BOX(page3), w->display, TRUE, TRUE, 0);


	hbox_status = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_status);
	gtk_box_pack_start (GTK_BOX (vbox_win), hbox_status, FALSE, FALSE, 0);

	statusbar = gtk_statusbar_new ();
	gtk_widget_show (statusbar);
	gtk_box_pack_start (GTK_BOX (hbox_status), statusbar, TRUE, TRUE, 0);

	progressbar = gtk_progress_bar_new ();
	gtk_widget_show (progressbar);
	gtk_box_pack_start (GTK_BOX (hbox_status), progressbar, FALSE, FALSE, 0);

	g_signal_connect_swapped ((gpointer)w->manager, "solid-changed",
			G_CALLBACK (saa_tree_view_update),
			w->explorer);
	g_signal_connect_swapped ((gpointer)w->manager, "solid-changed",
			G_CALLBACK (gtk_widget_queue_draw),
			w->area);

	g_signal_connect_swapped ((gpointer)w->display, "solid-changed",
			G_CALLBACK (gtk_widget_queue_draw),
			w->area);

	g_signal_connect_swapped ((gpointer)w->handler, "solid-changed",
			G_CALLBACK (saa_tree_view_update),
			w->explorer);
	g_signal_connect_swapped ((gpointer)w->handler, "solid-changed",
			G_CALLBACK (gtk_widget_queue_draw),
			w->area);

	g_signal_connect_swapped ((gpointer)w->area, "vertex-selected",
			G_CALLBACK (saa_tree_view_select_vertex),
			w->explorer);
	g_signal_connect_swapped ((gpointer)w->area, "edge-selected",
			G_CALLBACK (saa_tree_view_select_edge),
			w->explorer);
	g_signal_connect_swapped ((gpointer)w->area, "wing-selected",
			G_CALLBACK (saa_tree_view_select_wing),
			w->explorer);

	g_signal_connect_swapped ((gpointer)w->explorer, "vertex-selected",
			G_CALLBACK (saa_drawing_area_select_vertex),
			w->area);
	g_signal_connect_swapped ((gpointer)w->explorer, "edge-selected",
			G_CALLBACK (saa_drawing_area_select_edge),
			w->area);
	g_signal_connect_swapped ((gpointer)w->explorer, "wing-selected",
			G_CALLBACK (saa_drawing_area_select_wing),
			w->area);

	g_signal_connect_swapped((gpointer)it_charger_xml, "activate",
			G_CALLBACK (saa_main_window_charger_xml_clicked),
			w);
	g_signal_connect_swapped((gpointer)it_enregistrer_xml, "activate",
			G_CALLBACK (saa_main_window_sauver_xml_activate),
			w);
	g_signal_connect_swapped((gpointer)it_exporter, "activate",
			G_CALLBACK (saa_main_window_exporter_activate),
			w);
	g_signal_connect_swapped((gpointer)it_exporter_rb, "activate",
			G_CALLBACK (saa_main_window_exporter_rb_activate),
			w);

	g_signal_connect_swapped ((gpointer)it_quitter, "activate",
			G_CALLBACK (saa_main_window_quitter_activate),
			w);
	g_signal_connect (G_OBJECT(w), "destroy",
			G_CALLBACK (saa_main_window_quitter_activate),
			NULL);

	g_signal_connect_swapped ((gpointer)it_a_propos, "activate",
			G_CALLBACK (saa_main_window_a_propos_activate),
			w);

	gtk_window_add_accel_group (GTK_WINDOW (w), accel_group);
}

GtkWidget*
saa_main_window_new(const char * filename) {
	GObject * obj = g_object_new (SAA_TYPE_MAIN_WINDOW, NULL);
	GtkWidget * warning;

	if(g_strcmp0(filename, "")) {
		if(!saa_visuel_construire(SAA_MAIN_WINDOW(obj)->visuel, SAA_FILENAME, filename, 0)) {
			warning = gtk_message_dialog_new (GTK_WINDOW(obj),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_WARNING,
					GTK_BUTTONS_CLOSE,
					_("Unable to load the file %s"),
					filename);
			gtk_dialog_run(GTK_DIALOG(warning));
			gtk_widget_destroy (warning);
		} else {
			saa_tree_view_update(SAA_TREE_VIEW(SAA_MAIN_WINDOW(obj)->explorer));
			saa_solid_manager_set_filename(SAA_SOLID_MANAGER(SAA_MAIN_WINDOW(obj)->manager), filename);
			gtk_widget_queue_draw(SAA_MAIN_WINDOW(obj)->area);
		}
	}

	return GTK_WIDGET(obj);
}

void saa_main_window_charger_xml_clicked		(SaaMainWindow * w) {
	GtkWidget * warning;
	const gchar * path = g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS);
	GtkWidget * dialog = gtk_file_chooser_dialog_new(_("Load a solid"), GTK_WINDOW(w),
												GTK_FILE_CHOOSER_ACTION_OPEN,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
												NULL);

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
	    char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	    if(!saa_visuel_construire(w->visuel, SAA_FILENAME, filename, 0)) {
	    	warning = gtk_message_dialog_new (GTK_WINDOW(w),
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_WARNING,
                    GTK_BUTTONS_CLOSE,
                    _("Unable to load the file %s"),
                    filename);
			gtk_dialog_run(GTK_DIALOG(warning));
			gtk_widget_destroy (warning);
	    } else {
	    	saa_tree_view_update(SAA_TREE_VIEW(w->explorer));
	    	saa_solid_manager_set_filename(SAA_SOLID_MANAGER(w->manager), filename);
	    	gtk_widget_queue_draw(w->area);
	    }
	    g_free (filename);
	  }
	gtk_widget_destroy (dialog);
}

void saa_main_window_exporter_activate		(SaaMainWindow * w) {
	GtkWidget * dialog = saa_export_dialog_new_with_visuel(w->visuel);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
	    char * filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (SAA_EXPORT_DIALOG(dialog)->chooser));
	    int index = gtk_combo_box_get_active (GTK_COMBO_BOX(SAA_EXPORT_DIALOG(dialog)->format));
	    int echelle = gtk_range_get_value(GTK_RANGE(SAA_EXPORT_DIALOG(dialog)->echelle));
	    SaaTexteMode textMode = gtk_combo_box_get_active (GTK_COMBO_BOX(SAA_EXPORT_DIALOG(dialog)->texte));
	    SaaLangMode langMode =  gtk_combo_box_get_active (GTK_COMBO_BOX(SAA_EXPORT_DIALOG(dialog)->languette));
	    switch(index) {
	    	case 0:
	    		saa_solide_exporter_PS(w->visuel->solide, filename, SAA_PATRON_AILES_SEPAREES, langMode, textMode, echelle);
	    		break;
	    	case 1:
	    		saa_solide_exporter_SVG10(w->visuel->solide, filename, SAA_PATRON_AILES_SEPAREES, langMode, textMode, echelle);
	    		break;
	    	case 2:
	    		saa_solide_exporter_SVG12(w->visuel->solide, filename, SAA_PATRON_AILES_SEPAREES, langMode, textMode, echelle);
	    		break;
	    }
	    g_free (filename);
	}
	gtk_widget_destroy (dialog);
}

void saa_main_window_exporter_rb_activate	(SaaMainWindow * w) {
	const gchar * path = g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS);
	gchar * name = g_strdup_printf("%s%s", w->visuel->solide->nom, ".rb");
	GtkWidget * dialog = gtk_file_chooser_dialog_new(_("Generate a Ruby script"), GTK_WINDOW(w),
												GTK_FILE_CHOOSER_ACTION_SAVE,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
												NULL);

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), name);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
	    char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	    saa_solide_exporter_RB(SAA_RUBY_FACES, w->visuel->solide, filename, 200);
	    g_free (filename);
	}
	g_free(name);
	gtk_widget_destroy (dialog);

}

void saa_main_window_sauver_xml_activate		(SaaMainWindow * w) {
	//GtkWidget * warning;
	const gchar * path = g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS);
	gchar * name = g_strdup_printf("%s%s", w->visuel->solide->nom, ".saa");
	GtkWidget * dialog = gtk_file_chooser_dialog_new(_("Save a solid"), GTK_WINDOW(w),
												GTK_FILE_CHOOSER_ACTION_SAVE,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
												NULL);

	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), name);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
	    char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	    saa_solide_exporter_XML(w->visuel->solide, filename);
	    g_free (filename);
	}
	g_free(name);
	gtk_widget_destroy (dialog);
}

void saa_main_window_quitter_activate			(SaaMainWindow * w) {
	saa_visuel_detruire(w->visuel);

	if(SaaLogFile)   { fflush(SaaLogFile); fclose(SaaLogFile); }
	if(SaaErrorFile) { fflush(SaaErrorFile); fclose(SaaErrorFile); }

	gtk_main_quit();
}

void saa_main_window_a_propos_activate		(SaaMainWindow * w) {
	GtkWidget * about = gtk_about_dialog_new();
	gchar * authors[] = {"Cedric COULIOU", NULL};

	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(about), PACKAGE_NAME);
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about), PACKAGE_VERSION);

	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about), "http://gsolaar.sourceforge.net/");
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about), (const gchar **)authors);
	if(w->icon)
		gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), w->icon);

	gtk_dialog_run(GTK_DIALOG(about));
	gtk_widget_destroy(about);
}
