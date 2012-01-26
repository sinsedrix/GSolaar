/***************************** saa-solidhandler.h ****************************/
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

#ifndef __SAA_SOLID_HANDLER_H__
#define __SAA_SOLID_HANDLER_H__

#include <gtk/gtk.h>
#include "saa-visuel.h"

#define SAA_TYPE_SOLID_HANDLER          	 (saa_solid_handler_get_type ())
#define SAA_SOLID_HANDLER(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), SAA_TYPE_SOLID_HANDLER, SaaSolidHandler))
#define SAA_SOLID_HANDLER_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), SAA_TYPE_SOLID_HANDLER, SaaSolidHandlerClass))
#define SAA_IS_SOLID_HANDLER(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SAA_TYPE_SOLID_HANDLER))
#define SAA_IS_SOLID_HANDLER_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), SAA_TYPE_SOLID_HANDLER))
#define SAA_SOLID_HANDLER_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), SAA_TYPE_SOLID_HANDLER, SaaSolidHandlerClass))


typedef struct SaaSolidHandler {
	GtkTable			parent;

	SaaVisuel		*	visuel;

} SaaSolidHandler;

typedef struct SaaSolidHandlerClass {
	GtkTableClass		parent_class;

	void (* solid_changed) (SaaSolidHandler *hdl);

} SaaSolidHandlerClass;

GType   saa_solid_handler_get_type		 (void);

GtkWidget*  saa_solid_handler_new_with_visuel(SaaVisuel * visuel);

#endif //__SAA_SOLID_HANDLER_H__
