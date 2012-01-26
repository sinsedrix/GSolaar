/********************************* gsolaar.c *********************************/
/*                                                                           */
/*   Programme avec interface graphique (gtk+gtkglext)                       */
/*   Traitement des polyèdres et génération de scripts et postscripts        */
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

#include "saa-export.h"
#include "saa-widgets.h"
#include <gtk/gtkgl.h>

#include "saa-i18n.h"

//#ifdef G_OS_WIN32
//#	include <windows.h>
//#endif

int main (int argc, char *argv[]) {
	GtkWidget * window, * warning;
	char locale[3];
	const char * lang = pango_language_to_string (gtk_get_default_language ());

	locale[0] = lang[0];
	locale[1] = lang[1];
	locale[2] = '\0';

	setlocale(LC_ALL, "");
	char * lng = setlocale(LC_MESSAGES, "");
	g_debug("setlocale(LC_MESSAGES, \"%s\") = %s", locale, lng);

	lng = setlocale(LC_NUMERIC, "C");
	g_debug("setlocale(LC_NUMERIC, \"C\") = %s", lng);

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, saa_locale_directory());
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain (PACKAGE);
	printf("%s : %s ; %s\n", PACKAGE, saa_locale_directory(), locale);
#endif

	//gtk_disable_setlocale();
	gtk_init (&argc, &argv);
	//printf("setlocale(LC_ALL, NULL) :\n%s\n", setlocale(LC_ALL, NULL));
	gtk_gl_init (&argc, &argv);

	switch(argc) {
	case 1:
	  window = saa_main_window_new("");
	  break;
	case 2:
	  window = saa_main_window_new(argv[1]);
	  break;
	default:
	  window = saa_main_window_new("");
	  warning = gtk_message_dialog_new (GTK_WINDOW(window),
              GTK_DIALOG_DESTROY_WITH_PARENT,
              GTK_MESSAGE_WARNING,
              GTK_BUTTONS_CLOSE,
              "usage : gsolaar [solid.saa]\n");
	  gtk_dialog_run(GTK_DIALOG(warning));
	  gtk_widget_destroy(warning);
	}
	gtk_widget_show (window);

	gtk_main();

	gtk_widget_destroy(window);

	return 0;
}
