/***************************** saa-soliddisplay.c ****************************/
/*                                                                           */
/*   Widget gerant l'affichage d'un SaaVisuel                                */
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
#include "saa-soliddisplay.h"

#include "saa-i18n.h"

enum {
  SOLID_CHANGED_SIGNAL,
  LAST_SIGNAL
};

static void saa_solid_display_s1(SaaSolidDisplay * dsp, GtkRadioButton * rad);
static void saa_solid_display_s2(SaaSolidDisplay * dsp, GtkRadioButton * rad);
static void saa_solid_display_s3(SaaSolidDisplay * dsp, GtkRadioButton * rad);

static void saa_solid_display_ar1(SaaSolidDisplay * dsp, GtkRadioButton * rad);
static void saa_solid_display_ar2(SaaSolidDisplay * dsp, GtkRadioButton * rad);
static void saa_solid_display_ar3(SaaSolidDisplay * dsp, GtkRadioButton * rad);

static void saa_solid_display_ai1(SaaSolidDisplay * dsp, GtkRadioButton * rad);
static void saa_solid_display_ai2(SaaSolidDisplay * dsp, GtkRadioButton * rad);
static void saa_solid_display_ai3(SaaSolidDisplay * dsp, GtkRadioButton * rad);

static void saa_solid_display_light(SaaSolidDisplay * dsp, GtkToggleButton * ch);
static void saa_solid_display_trans(SaaSolidDisplay * dsp, GtkToggleButton * ch);
static void saa_solid_display_axes (SaaSolidDisplay * dsp, GtkToggleButton * ch);

static void saa_solid_display_bgcolor(SaaSolidDisplay * dsp, GtkColorButton * bt);
static void saa_solid_display_scolor (SaaSolidDisplay * dsp, GtkColorButton * bt);
static void saa_solid_display_arcolor(SaaSolidDisplay * dsp, GtkColorButton * bt);
static void saa_solid_display_ssize  (SaaSolidDisplay * dsp, GtkSpinButton * bt);
static void saa_solid_display_arsize (SaaSolidDisplay * dsp, GtkSpinButton * bt);


static guint solid_display_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (SaaSolidDisplay, saa_solid_display, GTK_TYPE_TABLE)

static void
saa_solid_display_class_init (SaaSolidDisplayClass *klass) {
	solid_display_signals[SOLID_CHANGED_SIGNAL] =
	    g_signal_new ("solid-changed",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaSolidDisplayClass, solid_changed),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

}

static void
saa_solid_display_init (SaaSolidDisplay * dsp) {
	GtkWidget * frs, * frar, * frai, * fro;
	GtkWidget * ts, * tar, * to, * hai;
	GtkWidget * bg_label;
	GtkObject * adjs = gtk_adjustment_new (.02, .02, .5, .001, .01, 0);
	GtkObject * adjar = gtk_adjustment_new (.01, .01, .2, .001, .01, 0);

	gtk_table_resize(GTK_TABLE(dsp), 4, 1);
	gtk_table_set_homogeneous(GTK_TABLE(dsp), TRUE);

	frs = gtk_frame_new(_("Vertices"));
	gtk_widget_show(frs);
	gtk_table_attach_defaults (GTK_TABLE (dsp), frs, 0, 1, 0, 1);

	ts = gtk_table_new(3, 2, TRUE);
	gtk_widget_show(ts);
	gtk_container_set_border_width (GTK_CONTAINER (ts), 5);
	gtk_container_add(GTK_CONTAINER(frs), ts);

	dsp->rads1 = gtk_radio_button_new_with_label(NULL, _("Do not display"));
	gtk_widget_show(dsp->rads1);
	gtk_table_attach_defaults (GTK_TABLE (ts), dsp->rads1, 0, 2, 0, 1);

	dsp->rads2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(dsp->rads1), _("Molecular"));
	gtk_widget_show(dsp->rads2);
	gtk_table_attach_defaults (GTK_TABLE (ts), dsp->rads2, 0, 1, 1, 2);

	dsp->rads3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(dsp->rads2), _("Point"));
	gtk_widget_show(dsp->rads3);
	gtk_table_attach_defaults (GTK_TABLE (ts), dsp->rads3, 0, 1, 2, 3);

	dsp->spins = gtk_spin_button_new(GTK_ADJUSTMENT(adjs), 0.001, 3);
	gtk_widget_show(dsp->spins);
	gtk_table_attach (GTK_TABLE (ts), dsp->spins, 1, 2, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);

	dsp->bts = gtk_color_button_new();
	gtk_widget_show(dsp->bts);
	gtk_table_attach (GTK_TABLE (ts), dsp->bts, 1, 2, 2, 3, GTK_SHRINK, GTK_SHRINK, 0, 0);

	frar = gtk_frame_new(_("Edges"));
	gtk_widget_show(frar);
	gtk_table_attach (GTK_TABLE (dsp), frar, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

	tar = gtk_table_new(3, 2, TRUE);
	gtk_widget_show(tar);
	gtk_container_set_border_width (GTK_CONTAINER (tar), 5);
	gtk_container_add(GTK_CONTAINER(frar), tar);

	dsp->radar1 = gtk_radio_button_new_with_label(NULL, _("Do not display"));
	gtk_widget_show(dsp->radar1);
	gtk_table_attach_defaults (GTK_TABLE (tar), dsp->radar1, 0, 2, 0, 1);

	dsp->radar2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(dsp->radar1), _("Molecular"));
	gtk_widget_show(dsp->radar2);
	gtk_table_attach_defaults (GTK_TABLE (tar), dsp->radar2, 0, 1, 1, 2);

	dsp->radar3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(dsp->radar2), _("Line"));
	gtk_widget_show(dsp->radar3);
	gtk_table_attach_defaults (GTK_TABLE (tar), dsp->radar3, 0, 1, 2, 3);

	dsp->spinar = gtk_spin_button_new(GTK_ADJUSTMENT(adjar), 0.001, 3);
	gtk_widget_show(dsp->spinar);
	gtk_table_attach (GTK_TABLE (tar), dsp->spinar, 1, 2, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);

	dsp->btar = gtk_color_button_new();
	gtk_widget_show(dsp->btar);
	gtk_table_attach (GTK_TABLE (tar), dsp->btar, 1, 2, 2, 3, GTK_SHRINK, GTK_SHRINK, 0, 0);

	frai = gtk_frame_new(_("Wings"));
	gtk_widget_show(frai);
	gtk_table_attach (GTK_TABLE (dsp), frai, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0);

	hai = gtk_vbox_new(TRUE, 5);
	gtk_widget_show(hai);
	gtk_container_set_border_width (GTK_CONTAINER (hai), 5);
	gtk_container_add(GTK_CONTAINER(frai), hai);

	dsp->radai1 = gtk_radio_button_new_with_label(NULL, _("Do not display"));
	gtk_widget_show(dsp->radai1);
	gtk_box_pack_start_defaults(GTK_BOX(hai), dsp->radai1);

	dsp->radai2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(dsp->radai1), _("Triangulation"));
	gtk_widget_show(dsp->radai2);
	gtk_box_pack_start_defaults(GTK_BOX(hai), dsp->radai2);

	dsp->radai3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(dsp->radai2), _("Colour"));
	gtk_widget_show(dsp->radai3);
	gtk_box_pack_start_defaults(GTK_BOX(hai), dsp->radai3);

	fro = gtk_frame_new("Autres");
	gtk_widget_show(fro);
	gtk_table_attach_defaults (GTK_TABLE (dsp), fro, 0, 1, 3, 4);

	to = gtk_table_new(4, 2, TRUE);
	gtk_widget_show(to);
	gtk_container_set_border_width (GTK_CONTAINER (to), 5);
	gtk_container_add(GTK_CONTAINER(fro), to);

	dsp->chlight = gtk_check_button_new_with_label(_("Lighting"));
	gtk_widget_show(dsp->chlight);
	gtk_table_attach (GTK_TABLE (to), dsp->chlight, 0, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	dsp->chtrans = gtk_check_button_new_with_label(_("Transparency"));
	gtk_widget_show(dsp->chtrans);
	gtk_table_attach (GTK_TABLE (to), dsp->chtrans, 0, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

	dsp->chaxes = gtk_check_button_new_with_label(_("Axes"));
	gtk_widget_show(dsp->chaxes);
	gtk_table_attach (GTK_TABLE (to), dsp->chaxes, 0, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);

	bg_label = gtk_label_new(_("Background"));
	gtk_widget_show(bg_label);
	gtk_table_attach_defaults (GTK_TABLE (to), bg_label, 0, 1, 3, 4);

	dsp->btbg = gtk_color_button_new();
	gtk_widget_show(dsp->btbg);
	gtk_table_attach (GTK_TABLE (to), dsp->btbg, 1, 2, 3, 4, GTK_SHRINK, GTK_SHRINK, 0, 0);

	g_signal_connect_swapped ((gpointer) dsp->rads1, "toggled",
			G_CALLBACK (saa_solid_display_s1),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->rads2, "toggled",
			G_CALLBACK (saa_solid_display_s2),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->rads3, "toggled",
			G_CALLBACK (saa_solid_display_s3),
			dsp);

	g_signal_connect_swapped ((gpointer) dsp->radar1, "toggled",
			G_CALLBACK (saa_solid_display_ar1),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->radar2, "toggled",
			G_CALLBACK (saa_solid_display_ar2),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->radar3, "toggled",
			G_CALLBACK (saa_solid_display_ar3),
			dsp);

	g_signal_connect_swapped ((gpointer) dsp->radai1, "toggled",
			G_CALLBACK (saa_solid_display_ai1),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->radai2, "toggled",
			G_CALLBACK (saa_solid_display_ai2),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->radai3, "toggled",
			G_CALLBACK (saa_solid_display_ai3),
			dsp);

	g_signal_connect_swapped ((gpointer) dsp->chlight, "toggled",
			G_CALLBACK (saa_solid_display_light),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->chtrans, "toggled",
			G_CALLBACK (saa_solid_display_trans),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->chaxes, "toggled",
			G_CALLBACK (saa_solid_display_axes),
			dsp);

	g_signal_connect_swapped ((gpointer) dsp->btbg, "color-set",
			G_CALLBACK (saa_solid_display_bgcolor),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->bts, "color-set",
			G_CALLBACK (saa_solid_display_scolor),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->btar, "color-set",
			G_CALLBACK (saa_solid_display_arcolor),
			dsp);

	g_signal_connect_swapped ((gpointer) dsp->spins, "value-changed",
			G_CALLBACK (saa_solid_display_ssize),
			dsp);
	g_signal_connect_swapped ((gpointer) dsp->spinar, "value-changed",
			G_CALLBACK (saa_solid_display_arsize),
			dsp);

}


GtkWidget*
saa_solid_display_new_with_visuel(SaaVisuel * visuel) {
	GObject * obj = g_object_new (SAA_TYPE_SOLID_DISPLAY, NULL);
	GdkColor c;
	double ts, tar;

	SAA_SOLID_DISPLAY(obj)->visuel = visuel;

	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->rads1, saa_solid_display_s1, obj);
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->rads2, saa_solid_display_s2, obj);
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->rads3, saa_solid_display_s3, obj);
	switch(visuel->visuSommets) {
	case SAA_SOMMET_NON:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->rads1), TRUE);
		break;
	case SAA_SOMMET_MOLECULE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->rads2), TRUE);
		break;
	case SAA_SOMMET_POINT:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->rads3), TRUE);
		break;
	}
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->rads1, saa_solid_display_s1, obj);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->rads2, saa_solid_display_s2, obj);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->rads3, saa_solid_display_s3, obj);

	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->radar1, saa_solid_display_ar1, obj);
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->radar2, saa_solid_display_ar2, obj);
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->radar3, saa_solid_display_ar3, obj);
	switch(visuel->visuAretes) {
	case SAA_ARETE_NON:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->radar1), TRUE);
		break;
	case SAA_ARETE_MOLECULE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->radar2), TRUE);
		break;
	case SAA_ARETE_LIGNE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->radar3), TRUE);
		break;
	}
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->radar1, saa_solid_display_ar1, obj);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->radar2, saa_solid_display_ar2, obj);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->radar3, saa_solid_display_ar3, obj);

	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->radai1, saa_solid_display_ai1, obj);
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->radai2, saa_solid_display_ai2, obj);
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->radai3, saa_solid_display_ai3, obj);
	switch(visuel->visuAiles) {
	case SAA_AILE_NON:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->radai1), TRUE);
		break;
	case SAA_AILE_TRIANGULATION:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->radai2), TRUE);
		break;
	case SAA_AILE_COULEUR:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->radai3), TRUE);
		break;
	}
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->radai1, saa_solid_display_ai1, obj);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->radai2, saa_solid_display_ai2, obj);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->radai3, saa_solid_display_ai3, obj);

	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->chlight, saa_solid_display_light, obj);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->chlight), visuel->eclairage);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->chlight, saa_solid_display_light, obj);

	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->chtrans, saa_solid_display_trans, obj);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->chtrans), visuel->transparence);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->chtrans, saa_solid_display_trans, obj);

	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->chaxes, saa_solid_display_axes, obj);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SAA_SOLID_DISPLAY(obj)->chaxes), visuel->axes);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->chaxes, saa_solid_display_axes, obj);

	c.red = visuel->couleurFond[0]*65535;
	c.green = visuel->couleurFond[1]*65535;
	c.blue = visuel->couleurFond[2]*65535;
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->btbg, saa_solid_display_bgcolor, obj);
	gtk_color_button_set_color(GTK_COLOR_BUTTON(SAA_SOLID_DISPLAY(obj)->btbg), &c);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->btbg, saa_solid_display_bgcolor, obj);

	c.red = visuel->couleurAretes[0]*65535;
	c.green = visuel->couleurAretes[1]*65535;
	c.blue = visuel->couleurAretes[2]*65535;
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->btar, saa_solid_display_arcolor, obj);
	gtk_color_button_set_color(GTK_COLOR_BUTTON(SAA_SOLID_DISPLAY(obj)->btar), &c);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->btar, saa_solid_display_arcolor, obj);

	c.red = visuel->couleurSommets[0]*65535;
	c.green = visuel->couleurSommets[1]*65535;
	c.blue = visuel->couleurSommets[2]*65535;
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->bts, saa_solid_display_scolor, obj);
	gtk_color_button_set_color(GTK_COLOR_BUTTON(SAA_SOLID_DISPLAY(obj)->bts), &c);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->bts, saa_solid_display_scolor, obj);

	ts = visuel->tailleSommets;
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->spins, saa_solid_display_ssize, obj);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(SAA_SOLID_DISPLAY(obj)->spins), ts);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->spins, saa_solid_display_ssize, obj);

	tar = visuel->tailleAretes;
	g_signal_handlers_block_by_func(SAA_SOLID_DISPLAY(obj)->spinar, saa_solid_display_arsize, obj);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(SAA_SOLID_DISPLAY(obj)->spinar), tar);
	g_signal_handlers_unblock_by_func(SAA_SOLID_DISPLAY(obj)->spinar, saa_solid_display_arsize, obj);

	return GTK_WIDGET(obj);
}

void saa_solid_display_s1(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_sommets(dsp->visuel, SAA_SOMMET_NON);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_s2(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_sommets(dsp->visuel, SAA_SOMMET_MOLECULE);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_s3(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_sommets(dsp->visuel, SAA_SOMMET_POINT);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_ar1(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_aretes(dsp->visuel, SAA_ARETE_NON);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_ar2(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_aretes(dsp->visuel, SAA_ARETE_MOLECULE);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_ar3(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_aretes(dsp->visuel, SAA_ARETE_LIGNE);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_ai1(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_ailes(dsp->visuel, SAA_AILE_NON);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);

}

void saa_solid_display_ai2(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_ailes(dsp->visuel, SAA_AILE_TRIANGULATION);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);

}

void saa_solid_display_ai3(SaaSolidDisplay * dsp, GtkRadioButton * rad) {
	saa_visuel_affichage_ailes(dsp->visuel, SAA_AILE_COULEUR);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_light(SaaSolidDisplay * dsp, GtkToggleButton * ch) {
	gboolean onoff = gtk_toggle_button_get_active(ch);
	saa_visuel_eclairage(dsp->visuel, onoff);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_trans(SaaSolidDisplay * dsp, GtkToggleButton * ch) {
	gboolean onoff = gtk_toggle_button_get_active(ch);
	saa_visuel_transparence(dsp->visuel, onoff);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_axes (SaaSolidDisplay * dsp, GtkToggleButton * ch) {
	gboolean onoff = gtk_toggle_button_get_active(ch);
	saa_visuel_axes(dsp->visuel, onoff);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_bgcolor(SaaSolidDisplay * dsp, GtkColorButton * bt) {
	GdkColor c;
	gtk_color_button_get_color(bt, &c);
	saa_visuel_couleur_fond(dsp->visuel, c.red/65535., c.green/65535., c.blue/65535.);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_scolor (SaaSolidDisplay * dsp, GtkColorButton * bt) {
	GdkColor c;
	gtk_color_button_get_color(bt, &c);
	saa_visuel_couleur_sommets(dsp->visuel, c.red/65535., c.green/65535., c.blue/65535.);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_arcolor(SaaSolidDisplay * dsp, GtkColorButton * bt) {
	GdkColor c;
	gtk_color_button_get_color(bt, &c);
	saa_visuel_couleur_aretes(dsp->visuel, c.red/65535., c.green/65535., c.blue/65535.);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_ssize  (SaaSolidDisplay * dsp, GtkSpinButton * bt) {
	double s = gtk_spin_button_get_value(bt);

	saa_visuel_taille_sommets(dsp->visuel, s);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}

void saa_solid_display_arsize (SaaSolidDisplay * dsp, GtkSpinButton * bt) {
	double s = gtk_spin_button_get_value(bt);

	saa_visuel_taille_aretes(dsp->visuel, s);
	g_signal_emit (G_OBJECT (dsp), solid_display_signals[SOLID_CHANGED_SIGNAL], 0);
}
