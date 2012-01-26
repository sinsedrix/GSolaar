/***************************** saa-solidhandler.c ****************************/
/*                                                                           */
/*   Widget gerant le traitement d'un SaaVisuel                              */
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

#include "saa-solidhandler.h"
#include "saa-core.h"

#include "saa-i18n.h"

enum {
  SOLID_CHANGED_SIGNAL,
  LAST_SIGNAL
};

static void saa_solid_handler_decoupage_activate(SaaSolidHandler * hdl);
static void saa_solid_handler_evidage_activate(SaaSolidHandler * hdl);
static void saa_solid_handler_fusion_activate(SaaSolidHandler * hdl);


static guint solid_handler_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (SaaSolidHandler, saa_solid_handler, GTK_TYPE_TABLE)

static void
saa_solid_handler_class_init (SaaSolidHandlerClass *klass) {
	solid_handler_signals[SOLID_CHANGED_SIGNAL] =
	    g_signal_new ("solid-changed",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaSolidHandlerClass, solid_changed),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

}

static void
saa_solid_handler_init (SaaSolidHandler * hdl) {
	GtkWidget * vbuttonbox;
	GtkWidget * but_decoupage, * but_evidage, * but_fusion;
	GtkWidget * log_scroll, * log_view;
	GtkTextBuffer * log_buffer;

	gtk_table_resize(GTK_TABLE(hdl), 2, 1);

	vbuttonbox = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox);
	gtk_table_attach_defaults (GTK_TABLE (hdl), vbuttonbox, 0, 1, 0, 1);
	gtk_container_set_border_width (GTK_CONTAINER (vbuttonbox), 5);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox), GTK_BUTTONBOX_SPREAD);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox), 5);

	but_decoupage = gtk_button_new_with_mnemonic (_("1 - Cutting"));
	gtk_widget_show (but_decoupage);
	gtk_container_add (GTK_CONTAINER (vbuttonbox), but_decoupage);
	GTK_WIDGET_SET_FLAGS (but_decoupage, GTK_CAN_DEFAULT);

	but_evidage = gtk_button_new_with_mnemonic (_("2 - Empty-fication"));
	gtk_widget_show (but_evidage);
	gtk_container_add (GTK_CONTAINER (vbuttonbox), but_evidage);
	GTK_WIDGET_SET_FLAGS (but_evidage, GTK_CAN_DEFAULT);

	but_fusion = gtk_button_new_with_mnemonic (_("3 - Fusion"));
	gtk_widget_show (but_fusion);
	gtk_container_add (GTK_CONTAINER (vbuttonbox), but_fusion);
	GTK_WIDGET_SET_FLAGS (but_fusion, GTK_CAN_DEFAULT);

	log_scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(log_scroll);
	gtk_container_set_border_width (GTK_CONTAINER (log_scroll), 5);
	gtk_table_attach_defaults (GTK_TABLE (hdl), log_scroll, 0, 1, 1, 2);

	log_buffer = gtk_text_buffer_new(NULL);
	log_view = gtk_text_view_new_with_buffer(log_buffer);
	gtk_widget_show(log_view);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(log_view), FALSE);
	gtk_container_add(GTK_CONTAINER(log_scroll), log_view);

	g_signal_connect_swapped ((gpointer) but_decoupage, "clicked",
			G_CALLBACK (saa_solid_handler_decoupage_activate),
			hdl);
	g_signal_connect_swapped ((gpointer) but_evidage, "clicked",
			G_CALLBACK (saa_solid_handler_evidage_activate),
			hdl);
	g_signal_connect_swapped ((gpointer) but_fusion, "clicked",
			G_CALLBACK (saa_solid_handler_fusion_activate),
			hdl);
}

GtkWidget*  saa_solid_handler_new_with_visuel(SaaVisuel * visuel) {
	GObject * obj = g_object_new (SAA_TYPE_SOLID_HANDLER, NULL);

	SAA_SOLID_HANDLER(obj)->visuel = visuel;

	return GTK_WIDGET(obj);
}

void saa_solid_handler_decoupage_activate(SaaSolidHandler * hdl) {
	GtkWidget * main_window, * warning;

	if(!saa_visuel_traiter_solide(hdl->visuel, SAA_TRAITEMENT_DECOUPAGE)) {
		g_signal_emit (G_OBJECT (hdl), solid_handler_signals[SOLID_CHANGED_SIGNAL], 0);
	} else {
		main_window = gtk_widget_get_toplevel (GTK_WIDGET(hdl));
		warning = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail while cutting polyhedron %s\n"),
		                                  "");
	}
}

void saa_solid_handler_evidage_activate(SaaSolidHandler * hdl) {
	GtkWidget * main_window, * dialog;

	if(!saa_visuel_traiter_solide(hdl->visuel, SAA_TRAITEMENT_EVIDAGE)) {
		g_signal_emit (G_OBJECT (hdl), solid_handler_signals[SOLID_CHANGED_SIGNAL], 0);
	} else {
		main_window = gtk_widget_get_toplevel (GTK_WIDGET(hdl));
		dialog = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail while emptying polyhedron %s\n"),
		                                  "");
	}
}

void saa_solid_handler_fusion_activate(SaaSolidHandler * hdl) {
	GtkWidget * main_window, * dialog;

	if(!saa_visuel_traiter_solide(hdl->visuel, SAA_TRAITEMENT_FUSION)) {
		g_signal_emit (G_OBJECT (hdl), solid_handler_signals[SOLID_CHANGED_SIGNAL], 0);
	} else {
		main_window = gtk_widget_get_toplevel (GTK_WIDGET(hdl));
		dialog = gtk_message_dialog_new (GTK_WINDOW(main_window),
		                                  GTK_DIALOG_DESTROY_WITH_PARENT,
		                                  GTK_MESSAGE_WARNING,
		                                  GTK_BUTTONS_CLOSE,
		                                  _("Fail while fusing polyhedron %s\n"),
		                                  "");
	}
}
