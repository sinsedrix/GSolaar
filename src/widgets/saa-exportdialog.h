/***************************** saa-exportdialog.c ****************************/
/*                                                                           */
/*   Boite de dialogue pour l'export des patrons                             */
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
