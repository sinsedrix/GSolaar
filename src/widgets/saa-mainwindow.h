/****************************** saa-mainwindow.h *****************************/
/*                                                                           */
/*   Fen�tre principale de l'application                                     */
/*                                                                           */
/*   Ecrit par : C�dric COULIOU                                              */
/*                                                                           */
/*   GSolaar Copyright (c) (2003-2004) C�dric COULIOU                        */
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
#ifndef __SAA_MAIN_WINDOW_H__
#define __SAA_MAIN_WINDOW_H__

#include <gtk/gtk.h>
#include "saa-visuel.h"

#define SAA_TYPE_MAIN_WINDOW          	   (saa_main_window_get_type ())
#define SAA_MAIN_WINDOW(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), SAA_TYPE_MAIN_WINDOW, SaaMainWindow))
#define SAA_MAIN_WINDOW_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), SAA_TYPE_MAIN_WINDOW, SaaMainWindowClass))
#define SAA_IS_MAIN_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SAA_TYPE_MAIN_WINDOW))
#define SAA_IS_MAIN_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), SAA_TYPE_MAIN_WINDOW))
#define SAA_MAIN_WINDOW_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), SAA_TYPE_MAIN_WINDOW, SaaMainWindowClass))


typedef struct SaaMainWindow {
	GtkWindow			parent;

	SaaVisuel		*	visuel;
	GtkWidget		*	area;
	GtkWidget		*	manager;
	GtkWidget		*	handler;
	GtkWidget		*	explorer;
	GtkWidget		*	display;
	GdkPixbuf		*	icon;
} SaaMainWindow;

typedef struct SaaMainWindowClass {
	GtkWindowClass		parent_class;

} SaaMainWindowClass;

GType   saa_main_window_get_type		 (void);

GtkWidget*  saa_main_window_new(const char * filename);

#endif //__SAA_MAIN_WINDOW_H__
