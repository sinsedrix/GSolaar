/***************************** saa-exportdialog.c ****************************/
/*                                                                           */
/*   Boite de dialogue pour l'export des patrons                             */
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

#include "saa-exportdialog.h"

#include "saa-i18n.h"

static void saa_export_dialog_format_changed(SaaExportDialog * d);

G_DEFINE_TYPE (SaaExportDialog, saa_export_dialog, GTK_TYPE_DIALOG)

static void
saa_export_dialog_class_init (SaaExportDialogClass *klass) {
}

static void
saa_export_dialog_init (SaaExportDialog * d) {
	const gchar * path = g_get_user_special_dir(G_USER_DIRECTORY_PICTURES);
	GtkWidget * vbox = GTK_DIALOG(d)->vbox;
	GtkWidget * table = gtk_table_new(4, 2, FALSE);
	GtkWidget * label_f = gtk_label_new(_("Format:"));
	GtkWidget * label_s = gtk_label_new(_("Scale:"));
	GtkWidget * label_t = gtk_label_new(_("Edge name:"));
	GtkWidget * label_l = gtk_label_new(_("Tongue:"));
	GtkObject * adj_s   = gtk_adjustment_new(200, 100, 500, 1, 10, 0);
	GtkWidget * hsep	= gtk_hseparator_new();

	d->chooser			= gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_SAVE);
	d->format			= gtk_combo_box_new_text();
	d->echelle			= gtk_hscale_new(GTK_ADJUSTMENT(adj_s));
	d->texte			= gtk_combo_box_new_text();
	d->languette		= gtk_combo_box_new_text();

	gtk_window_set_title(GTK_WINDOW(d), _("Generate a pattern"));
	gtk_window_set_default_size(GTK_WINDOW(d), 700, 650);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(d->chooser), path);
	gtk_container_set_border_width(GTK_CONTAINER(d), 10);
	gtk_table_set_col_spacings(GTK_TABLE(table), 10);
	gtk_table_set_row_spacings(GTK_TABLE(table), 10);

	gtk_widget_show(table);
	gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, TRUE, 5);
	gtk_widget_show(hsep);
	gtk_box_pack_start(GTK_BOX(vbox), hsep, FALSE, TRUE, 5);
	gtk_widget_show(d->chooser);
	gtk_box_pack_start_defaults(GTK_BOX(vbox), d->chooser);

	gtk_widget_show(label_f);
	gtk_table_attach_defaults(GTK_TABLE(table), label_f, 0, 1, 0, 1);
	gtk_widget_show(d->format);
	gtk_table_attach_defaults(GTK_TABLE(table), d->format, 1, 2, 0, 1);
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->format), "PostScript (*.ps)");
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->format), "SVG 1.0 (*.svg)");
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->format), "SVG 1.2 (multipage) (*.svg)");

	gtk_widget_show(label_s);
	gtk_table_attach_defaults(GTK_TABLE(table), label_s, 0, 1, 1, 2);
	gtk_widget_show(d->echelle);
	gtk_table_attach_defaults(GTK_TABLE(table), d->echelle, 1, 2, 1, 2);

	gtk_widget_show(label_t);
	gtk_table_attach_defaults(GTK_TABLE(table), label_t, 0, 1, 2, 3);
	gtk_widget_show(d->texte);
	gtk_table_attach_defaults(GTK_TABLE(table), d->texte, 1, 2, 2, 3);
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->texte), _("None"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->texte), _("Upside the edge"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->texte), _("Downside the edge"));

	gtk_widget_show(label_l);
	gtk_table_attach_defaults(GTK_TABLE(table), label_l, 0, 1, 3, 4);
	gtk_widget_show(d->languette);
	gtk_table_attach_defaults(GTK_TABLE(table), d->languette, 1, 2, 3, 4);
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->languette), _("None"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->languette), _("Regular"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->languette), _("On the greatest wing"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->languette), _("On the smallest wing"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(d->languette), _("All"));

	gtk_dialog_add_buttons (GTK_DIALOG (d),
							GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
							GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
							NULL);

	g_signal_connect_swapped ((gpointer)d->format, "changed",
				G_CALLBACK (saa_export_dialog_format_changed),
				d);
}


GtkWidget*  saa_export_dialog_new_with_visuel(const SaaVisuel * visuel) {
	GObject * obj = g_object_new (SAA_TYPE_EXPORT_DIALOG, NULL);

	SAA_EXPORT_DIALOG(obj)->visuel = visuel;
	gtk_combo_box_set_active(GTK_COMBO_BOX(SAA_EXPORT_DIALOG(obj)->format), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(SAA_EXPORT_DIALOG(obj)->texte), 1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(SAA_EXPORT_DIALOG(obj)->languette), 1);

	return GTK_WIDGET(obj);
}

void saa_export_dialog_format_changed(SaaExportDialog * d) {
	gchar * name;
	int index = gtk_combo_box_get_active (GTK_COMBO_BOX(d->format));
	switch(index) {
	case 0:
		name = g_strdup_printf("%s%s", d->visuel->solide->nom, ".ps");
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(d->chooser), name);
		g_free(name);
		break;
	case 1:
	case 2:
		name = g_strdup_printf("%s%s", d->visuel->solide->nom, ".svg");
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(d->chooser), name);
		g_free(name);
		break;
	}
}
