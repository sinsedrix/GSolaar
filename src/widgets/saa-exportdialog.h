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

#ifndef _SAA_EXPORT_DIALOG_H_
#define _SAA_EXPORT_DIALOG_H_

#include <gtk/gtk.h>
#include "saa-visuel.h"

#define SAA_TYPE_EXPORT_DIALOG          	 (saa_export_dialog_get_type ())
#define SAA_EXPORT_DIALOG(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), SAA_TYPE_EXPORT_DIALOG, SaaExportDialog))
#define SAA_EXPORT_DIALOG_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), SAA_TYPE_EXPORT_DIALOG, SaaExportDialogClass))
#define SAA_IS_EXPORT_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SAA_TYPE_EXPORT_DIALOG))
#define SAA_IS_EXPORT_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), SAA_TYPE_EXPORT_DIALOG))
#define SAA_EXPORT_DIALOG_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), SAA_TYPE_EXPORT_DIALOG, SaaExportDialogClass))

typedef struct SaaExportDialog {
	GtkDialog		parent;

	const SaaVisuel	*	visuel;
	GtkWidget		*	echelle;
	GtkWidget		*	format;
	GtkWidget		*	texte;
	GtkWidget		*	languette;
	GtkWidget		*	chooser;

} SaaExportDialog;

typedef struct SaaExportDialogClass {
	GtkDialogClass	parent_class;

} SaaExportDialogClass;

GType   saa_export_dialog_get_type		 (void);

GtkWidget*  saa_export_dialog_new_with_visuel(const SaaVisuel * visuel);

#endif /* _SAA_EXPORT_DIALOG_H_ */
