/****************************** saa-drawingarea.c ****************************/
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
#include "saa-drawingarea.h"

#include "saa-i18n.h"

static void		saa_drawing_area_realize		(SaaDrawingArea  *area);
static gboolean saa_drawing_area_expose			(SaaDrawingArea  *area, GdkEventExpose	*event, gpointer user_data);
static gboolean	saa_drawing_area_configure		(SaaDrawingArea  *area, GdkEventConfigure *event);
static gboolean saa_drawing_area_button_press	(SaaDrawingArea  *area, GdkEventButton	*event);
static gboolean saa_drawing_area_button_release	(SaaDrawingArea  *area, GdkEventButton	*event);
static gboolean saa_drawing_area_motion_notify	(SaaDrawingArea  *area, GdkEventMotion  *event);
static gboolean saa_drawing_area_scroll         (SaaDrawingArea  *area, GdkEventScroll *event);
static gboolean saa_drawing_area_animate		(SaaDrawingArea  *area);

enum {
  VERTEX_SELECTED_SIGNAL,
  EDGE_SELECTED_SIGNAL,
  WING_SELECTED_SIGNAL,
  LAST_SIGNAL
};

static guint drawing_area_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (SaaDrawingArea, saa_drawing_area, GTK_TYPE_DRAWING_AREA)

static void
saa_drawing_area_class_init (SaaDrawingAreaClass *klass) {
	drawing_area_signals[VERTEX_SELECTED_SIGNAL] =
	    g_signal_new ("vertex-selected",
	                  G_TYPE_FROM_CLASS (klass),
	                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                  G_STRUCT_OFFSET (SaaDrawingAreaClass, vertex_selected),
	                  NULL, NULL,
	                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);

	drawing_area_signals[EDGE_SELECTED_SIGNAL] =
		    g_signal_new ("edge-selected",
		                  G_TYPE_FROM_CLASS (klass),
		                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		                  G_STRUCT_OFFSET (SaaDrawingAreaClass, edge_selected),
		                  NULL, NULL,
		                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);

	drawing_area_signals[WING_SELECTED_SIGNAL] =
		    g_signal_new ("wing-selected",
		                  G_TYPE_FROM_CLASS (klass),
		                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		                  G_STRUCT_OFFSET (SaaDrawingAreaClass, wing_selected),
		                  NULL, NULL,
		                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);

}

static void
saa_drawing_area_init (SaaDrawingArea *area) {
//g_debug("saa_drawing_area_init");
	GdkGLConfig * glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE);

	if(!glconfig) {
		g_critical (_("OpenGL configuration impossible.\n"));
		return;
	}

	GTK_DRAWING_AREA(area)->draw_data = NULL;
	area->motion = FALSE;

	gtk_widget_set_gl_capability (GTK_WIDGET(area), glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE);
	gtk_widget_add_events(GTK_WIDGET(area), GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

	g_signal_connect ((gpointer) area, "expose_event",
			G_CALLBACK (saa_drawing_area_expose),
			NULL);
	g_signal_connect ((gpointer) area, "configure_event",
			G_CALLBACK (saa_drawing_area_configure),
			NULL);
	g_signal_connect ((gpointer) area, "realize",
			G_CALLBACK (saa_drawing_area_realize),
			NULL);
	g_signal_connect ((gpointer) area, "button_press_event",
			G_CALLBACK (saa_drawing_area_button_press),
			NULL);
	g_signal_connect ((gpointer) area, "button-release-event",
				G_CALLBACK (saa_drawing_area_button_release),
				NULL);
	g_signal_connect ((gpointer) area, "motion_notify_event",
			G_CALLBACK (saa_drawing_area_motion_notify),
			NULL);
	g_signal_connect((gpointer) area, "scroll-event",
			G_CALLBACK (saa_drawing_area_scroll),
			NULL);
}


GtkWidget*
saa_drawing_area_new_with_visuel (SaaVisuel * visuel) {
	GObject * obj = g_object_new (SAA_TYPE_DRAWING_AREA, NULL);

	SAA_DRAWING_AREA(obj)->visuel = visuel;

	return GTK_WIDGET(obj);
}

gboolean
saa_drawing_area_expose(SaaDrawingArea  *area, GdkEventExpose  *event, gpointer         user_data) {
//g_debug("saa_drawing_area_expose");
	GdkGLContext * glcontext = gtk_widget_get_gl_context (GTK_WIDGET(area));
	GdkGLDrawable * gldrawable = gtk_widget_get_gl_drawable (GTK_WIDGET(area));
	SaaVisuel * visuel = area->visuel;

	if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
		return FALSE;

	saa_visuel_dessiner(visuel);
	gdk_gl_drawable_swap_buffers (gldrawable);
	gdk_gl_drawable_gl_end (gldrawable);

	return TRUE;
}

void
saa_drawing_area_realize(SaaDrawingArea  *area) {
//g_debug("saa_drawing_area_realize");
	GdkGLContext *glcontext = gtk_widget_get_gl_context (GTK_WIDGET(area));
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (GTK_WIDGET(area));
	SaaVisuel * visuel = area->visuel;

	if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
		return;

	saa_visuel_initialiser(visuel, GTK_WIDGET(area)->allocation.width, GTK_WIDGET(area)->allocation.height);
	gdk_gl_drawable_gl_end (gldrawable);
}


gboolean
saa_drawing_area_configure(SaaDrawingArea  *area, GdkEventConfigure *event) {
//g_debug("saa_drawing_area_configure");
	GdkGLContext *glcontext = gtk_widget_get_gl_context (GTK_WIDGET(area));
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (GTK_WIDGET(area));
	SaaVisuel * visuel = area->visuel;

	if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
		return FALSE;

	saa_visuel_redimensionner(visuel, event->width, event->height);
	gdk_gl_drawable_gl_end (gldrawable);

	return 0;
}

gboolean
saa_drawing_area_button_press(SaaDrawingArea  *area, GdkEventButton  *event) {
	GdkModifierType state;
	int x, y;
	gdk_window_get_pointer (event->window, &x, &y, &state);
//g_debug("press(x,y) =(%d ; %d)", x, y);
	area->xold = x;
	area->yold = y;
	return FALSE;
}

gboolean
saa_drawing_area_button_release	(SaaDrawingArea  *area, GdkEventButton	*event) {
	GdkModifierType state;

	int x, y;
	GLuint sel;

	gdk_window_get_pointer (event->window, &x, &y, &state);

	if(!area->motion) {
		sel = saa_visuel_selectionner(area->visuel, x, y);
//g_debug("release(x,y) =(%d ; %d) : sel = %d", x, y, sel);
		if(area->visuel->listesSommets <= sel && sel < area->visuel->listesSommets+area->visuel->solide->nbSommets) {
			saa_drawing_area_select_vertex(area, area->visuel->solide->lesSommets[sel-area->visuel->listesSommets]);
			g_signal_emit (G_OBJECT (area), drawing_area_signals[VERTEX_SELECTED_SIGNAL], 0, area->visuel->sel_vertex);
		} else if(area->visuel->listesAretes <= sel && sel < area->visuel->listesAretes+area->visuel->solide->nbAretes) {
			saa_drawing_area_select_edge(area, area->visuel->solide->lesAretes[sel-area->visuel->listesAretes]);
			g_signal_emit (G_OBJECT (area), drawing_area_signals[EDGE_SELECTED_SIGNAL], 0, area->visuel->sel_edge);
		} else if(area->visuel->listesAiles <= sel && sel < area->visuel->listesAiles+area->visuel->solide->nbAiles) {
			saa_drawing_area_select_wing(area, area->visuel->solide->lesAiles[sel-area->visuel->listesAiles]);
			g_signal_emit (G_OBJECT (area), drawing_area_signals[WING_SELECTED_SIGNAL], 0, area->visuel->sel_wing);
		} else {
			g_source_remove(area->id);
			area->visuel->sel_vertex = NULL;
			area->visuel->sel_edge = NULL;
			area->visuel->sel_wing = NULL;
			area->animation = FALSE;
		}
	} else {
		area->motion = FALSE;
	}
	return FALSE;
}

gboolean
saa_drawing_area_motion_notify(SaaDrawingArea  *area, GdkEventMotion  *event) {
  int x, y;
  GdkModifierType state;
  SaaVisuel * visuel = area->visuel;

	gdk_window_get_pointer (event->window, &x, &y, &state);

	if(x != area->xold || y != area->yold) {
		if(state & GDK_BUTTON1_MASK) {
//g_debug("motion1(x,y) =(%d ; %d)", x, y);
			visuel->angle->x -= x - area->xold;
			visuel->angle->y -= y - area->yold;
			if(visuel->angle->x >= 360)
				visuel->angle->x -= 360;
			if(visuel->angle->x < 0)
				visuel->angle->x += 360;
			if(visuel->angle->y >= 360)
				visuel->angle->y -= 360;
			if(visuel->angle->y < 0)
				visuel->angle->y += 360;
			area->motion = TRUE;
			gtk_widget_queue_draw(GTK_WIDGET(area));
		} else if(state & GDK_BUTTON3_MASK) {
//g_debug("motion3(x,y) =(%d ; %d)", x, y);
			area->visuel->angle->x -= (x - area->xold)*0.1;
			area->visuel->angle->y -= (y - area->yold)*0.1;
			area->motion = TRUE;
			gtk_widget_queue_draw(GTK_WIDGET(area));
		}
		area->xold = x;
		area->yold = y;
	}
	return FALSE;
}

gboolean saa_drawing_area_scroll(SaaDrawingArea  *area, GdkEventScroll *event) {
	SaaVisuel * visuel = area->visuel;

	if(event->direction == GDK_SCROLL_UP)
		visuel->centre->z -= .3;
	if(event->direction == GDK_SCROLL_DOWN)
		visuel->centre->z += .3;
	gtk_widget_queue_draw(GTK_WIDGET(area));

	return FALSE;
}

gboolean saa_drawing_area_animate	(SaaDrawingArea  *area) {
	gtk_widget_queue_draw(GTK_WIDGET(area));
	return area->animation;
}

void saa_drawing_area_select_vertex		(SaaDrawingArea  *area, const SaaSommet * s) {
	//g_debug("saa_drawing_area_select_vertex");
	g_source_remove(area->id);
	area->visuel->sel_vertex = s;
	area->visuel->sel_edge = NULL;
	area->visuel->sel_wing = NULL;
	area->id = g_timeout_add(500, (GSourceFunc)saa_drawing_area_animate, area);
	area->animation = TRUE;
}

void saa_drawing_area_select_edge		(SaaDrawingArea  *area, const SaaArete * ar) {
	//g_debug("saa_drawing_area_select_edge");
	g_source_remove(area->id);
	area->visuel->sel_vertex = NULL;
	area->visuel->sel_edge = ar;
	area->visuel->sel_wing = NULL;
	area->id = g_timeout_add(500, (GSourceFunc)saa_drawing_area_animate, area);
	area->animation = TRUE;
}

void saa_drawing_area_select_wing		(SaaDrawingArea  *area, const SaaAile * ai) {
	//g_debug("saa_drawing_area_select_wing");
	g_source_remove(area->id);
	area->visuel->sel_vertex = NULL;
	area->visuel->sel_edge = NULL;
	area->visuel->sel_wing = ai;
	area->id = g_timeout_add(500, (GSourceFunc)saa_drawing_area_animate, area);
	area->animation = TRUE;
}
