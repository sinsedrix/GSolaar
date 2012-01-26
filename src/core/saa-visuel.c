/******************************* saa-visuel.c ********************************/
/*                                                                           */
/*   Bibliothèque statique graphique :                                       */
/*   Dessin des polyèdres grâce à OpenGL                                     */
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
#include "saa-visuel.h"
#include "saa-traitement.h"

#include <stdlib.h>
#include <GL/glu.h>

static void saa_visuel_construire_listes(SaaVisuel * visuel);
static void saa_visuel_construire_listes_sommets(SaaVisuel * visuel);
static void saa_visuel_construire_listes_aretes(SaaVisuel * visuel);
static void saa_visuel_construire_listes_ailes(SaaVisuel * visuel);
static void saa_visuel_maj_couleurs(SaaVisuel * visuel);

SaaVisuel * saa_visuel_creer() {
  SaaVisuel * new = (SaaVisuel *)g_malloc(sizeof(SaaVisuel));

  new->centre = saa_sommet_creer(0., 0., 0.);
  new->angle  = saa_sommet_creer(0., 0., 0.);
  new->dual         = GL_FALSE;
  new->eclairage    = GL_TRUE;
  new->transparence = GL_FALSE;
  new->axes			= GL_FALSE;
  new->visuAiles   = SAA_AILE_COULEUR;
  new->visuAretes  = SAA_ARETE_MOLECULE;
  new->visuSommets = SAA_SOMMET_MOLECULE;
  new->tailleSommets = 0.05;
  new->tailleAretes  = 0.02;
  new->solide = saa_solide_creer_kaleido("#3");
  new->source = SAA_NUMERO;
  new->texteSource = g_strdup("3");
  new->couleurAiles = NULL;

  new->couleurSommets[0] = 1;
  new->couleurSommets[1] = 0.5;
  new->couleurSommets[2] = 0;
  new->couleurSommets[3] = 1;

  new->couleurAretes[0] = 0;
  new->couleurAretes[1] = 0.75;
  new->couleurAretes[2] = 0;
  new->couleurAretes[3] = 1;

  new->couleurFond[0] = .5;
  new->couleurFond[1] = .5;
  new->couleurFond[2] = .5;

  saa_visuel_maj_couleurs(new);

  return new;
}

void saa_visuel_detruire(SaaVisuel * visuel) {
	saa_solide_detruire(visuel->solide);
	g_free(visuel->centre);
	g_free(visuel->angle);
	g_free(visuel->couleurAiles);
	g_free(visuel);
}

int saa_visuel_construire(SaaVisuel * visuel, SaaVisuelSource source, const char * texteSource, GLboolean dual) {
	SaaSolide * tmpsol;
	char symbol[4];
//printf("saa_visuel_construire %s %s\n", texteSource, dual?"Dual":"");
	switch(source) {
	case SAA_NUMERO:
		sprintf(symbol, "#%s", texteSource);
		tmpsol = dual ? saa_solide_creer_kaleido_dual(symbol) : saa_solide_creer_kaleido(symbol);
		break;
	case SAA_WYTHOFF:
		tmpsol = dual ? saa_solide_creer_kaleido_dual(texteSource) : saa_solide_creer_kaleido(texteSource);
		break;
	case SAA_FILENAME:
		tmpsol = saa_solide_importer_XML(texteSource);
		break;
	default:
		tmpsol = NULL;
	}

	if(tmpsol) {
		glDeleteLists(visuel->listesSommets, visuel->solide->nbSommets);
		glDeleteLists(visuel->listesAretes, visuel->solide->nbAretes);
		glDeleteLists(visuel->listesAiles, visuel->solide->nbAiles);

		if(visuel->solide)
			saa_solide_detruire(visuel->solide);
		visuel->solide = tmpsol;

		visuel->source = source;
		visuel->dual = dual;

		if(visuel->texteSource)
			g_free(visuel->texteSource);
		visuel->texteSource = g_strdup(texteSource);

		saa_visuel_maj_couleurs(visuel);
		saa_visuel_construire_listes(visuel);
	}
	return tmpsol != NULL;
}

void saa_visuel_dessiner(SaaVisuel * visuel) {
	int i;
	static GLfloat r[] = { 1, 0, 0, 1 };
	static GLfloat g[] = { 0, 1, 0, 1 };
	static GLfloat b[] = { 0, 0, 1, 1 };
	static int anim = 0;
	GLfloat invcolor[4];

	anim = !anim;

	if(!visuel->solide) {
	  return;
	}

	if(visuel->eclairage) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_BLEND);

	glPushMatrix();
		glTranslated(visuel->centre->x, -visuel->centre->y, -15-visuel->centre->z);
		glRotated(-visuel->angle->y, 1., 0., 0.);
		glRotated(-visuel->angle->x, 0., 1., 0.);

		if(visuel->axes) {
			glBegin(GL_LINES);
				glColor3d(1, 0, 0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, r);
				glVertex3d(0,0,0);
				glVertex3d(5,0,0);
				glColor3d(0, 1, 0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, g);
				glVertex3d(0,0,0);
				glVertex3d(0,5,0);
				glColor3d(0, 0, 1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, b);
				glVertex3d(0,0,0);
				glVertex3d(0,0,5);
			glEnd();
		}

		if(visuel->transparence) {
			glEnable(GL_BLEND);
		    glClear(GL_DEPTH_BUFFER_BIT);
		    glDepthMask(GL_FALSE);
		} else {
		    glDisable(GL_BLEND);
		    glDepthMask(GL_TRUE);
		}
		for(i=0; i<visuel->solide->nbAiles; i++) {
			if(anim && visuel->sel_wing && visuel->sel_wing == visuel->solide->lesAiles[i]) {
				invcolor[0] = 1;
				invcolor[1] = 1 - visuel->couleurAiles[4*i+1];
				invcolor[2] = 1 - visuel->couleurAiles[4*i+2];
				invcolor[3] = 1 - visuel->couleurAiles[4*i+3];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, invcolor);
				glColor3fv(invcolor);
			} else {
				glColor3fv(visuel->couleurAiles+4*i);
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, visuel->couleurAiles+4*i);
			}
			glCallList(visuel->listesAiles+i);
		}
		glDepthMask(GL_TRUE);

		for(i=0; i<visuel->solide->nbAretes; i++) {
			if(anim && visuel->sel_edge && visuel->sel_edge == visuel->solide->lesAretes[i]) {
				invcolor[0] = 1;
				invcolor[1] = 1 - visuel->couleurAretes[1];
				invcolor[2] = 1 - visuel->couleurAretes[2];
				invcolor[3] = 1 - visuel->couleurAretes[3];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, invcolor);
				glColor3fv(invcolor);
			} else {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, visuel->couleurAretes);
				glColor3fv(visuel->couleurAretes);
			}
			glCallList(visuel->listesAretes+i);
		}

		for(i=0; i<visuel->solide->nbSommets; i++) {
			if(anim && visuel->sel_vertex && visuel->sel_vertex == visuel->solide->lesSommets[i]) {
				invcolor[0] = 1;
				invcolor[1] = 1 - visuel->couleurSommets[1];
				invcolor[2] = 1 - visuel->couleurSommets[2];
				invcolor[3] = 1 - visuel->couleurSommets[3];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, invcolor);
				glColor3fv(invcolor);
			} else {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, visuel->couleurSommets);
				glColor3fv(visuel->couleurSommets);
			}
			glCallList(visuel->listesSommets+i);
		}


	glPopMatrix();
}

GLuint saa_visuel_selectionner(SaaVisuel * visuel, int x, int y) {
	GLint viewport[4];
	GLfloat aspect;
	GLint hits;
	GLuint selectBuf[512];
	GLfloat z, zmin = 1000;
	GLuint name, namemin = -1;

	int w, h, i;

	glGetIntegerv (GL_VIEWPORT, viewport);
	w = viewport[2];
	h = viewport[3];

	glSelectBuffer(512, selectBuf);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
		glLoadIdentity();
		gluPickMatrix ((GLdouble) x, (GLdouble) (h - y), 2., 2., viewport);

		if(w>h) {
			aspect = (w*1.)/h;
			glFrustum (-aspect, aspect, -1.0, 1.0, 10., 60.);
		} else {
			aspect = (h*1.)/w;
			glFrustum (-1.0, 1.0, -aspect, aspect, 10., 60.);
		}

			glTranslated(visuel->centre->x, -visuel->centre->y, -15-visuel->centre->z);
			glRotated(-visuel->angle->y, 1., 0., 0.);
			glRotated(-visuel->angle->x, 0., 1., 0.);

			for(i=0; i<visuel->solide->nbAretes; i++) {
				glPushName(visuel->listesAretes+i);
				glCallList(visuel->listesAretes+i);
				glPopName();
			}
			for(i=0; i<visuel->solide->nbSommets; i++) {
				glPushName(visuel->listesSommets+i);
				glCallList(visuel->listesSommets+i);
				glPopName();
			}
			for(i=0; i<visuel->solide->nbAiles; i++) {
				glPushName(visuel->listesAiles+i);
				glCallList(visuel->listesAiles+i);
				glPopName();
			}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	hits = glRenderMode(GL_RENDER);

	for(i=0; i<hits; i++) {
		z = selectBuf[4*i+1]*1./0x7fffffff;
		name = selectBuf[4*i+3];
//printf("hit %d : name = %d ; z1 =%f ; z2 = %f\n", i, name, z, z);
		if(z < zmin) {
			zmin = z;
			namemin = name;
		}
	}
//printf("namemin = %d\n", namemin);
	return namemin;
}

void saa_visuel_initialiser(SaaVisuel * visuel, int w, int h) {
	static GLfloat diff[] = {1.0, 1.0, 0.7, 0.6};  // couleur face avant
	static GLfloat spec[] = {0.8, 0.8, 0.8};
	static GLfloat ldir[] = {-1., -1., -1.};
	static GLfloat lpos[] = {9.0, 9.0, 9.0, 1.0};

	glClearColor(visuel->couleurFond[0], visuel->couleurFond[1], visuel->couleurFond[2], 0.);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	glLightfv(GL_LIGHT0,GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0,GL_SPECULAR, spec);
	//glLightf(GL_LIGHT0,GL_SHININESS, 60.);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, ldir);

	//glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE); // Intérieur des face visible
	glEnable(GL_DEPTH_TEST); // Test profondeur

	// Lissage
    //glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_POLYGON_SMOOTH);

    // Taille points et lignes
    glPointSize(2);
    glLineWidth(1.5);

	glEnable(GL_LIGHT0);

	saa_visuel_redimensionner(visuel, w, h);
	saa_visuel_construire_listes(visuel);
}

void saa_visuel_redimensionner(SaaVisuel * visuel, int w, int h) {
	GLfloat aspect;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(w>h) {
		aspect = (w*1.)/h;
		glFrustum (-aspect, aspect, -1.0, 1.0, 10., 60.);
	} else {
		aspect = (h*1.)/w;
		glFrustum (-1.0, 1.0, -aspect, aspect, 10., 60.);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void saa_visuel_affichage_sommets(SaaVisuel * visuel, SaaVisuelSommet visu) {
	visuel->visuSommets = visu;
	saa_visuel_construire_listes_sommets(visuel);
}

void saa_visuel_affichage_aretes(SaaVisuel * visuel, SaaVisuelArete visu) {
	visuel->visuAretes = visu;
	saa_visuel_construire_listes_aretes(visuel);
}

void saa_visuel_affichage_ailes(SaaVisuel * visuel, SaaVisuelAile visu) {
	visuel->visuAiles = visu;
	saa_visuel_construire_listes_ailes(visuel);
}

void saa_visuel_eclairage(SaaVisuel * visuel, GLboolean onoff) {
	visuel->eclairage = onoff;
}

void saa_visuel_transparence(SaaVisuel * visuel, GLboolean onoff) {
	visuel->transparence = onoff;
}

void saa_visuel_axes(SaaVisuel * visuel, GLboolean onoff) {
	visuel->axes = onoff;
}

void saa_visuel_maj_couleurs(SaaVisuel * visuel) {
	int i;

	if(visuel->couleurAiles) g_free(visuel->couleurAiles);
	visuel->couleurAiles = (GLfloat *)g_malloc((4*visuel->solide->nbAiles)*sizeof(GLfloat));

	for(i=0; i<visuel->solide->nbAiles; i++) {
		visuel->couleurAiles[4*i+0] = (visuel->solide->lesNormales[i]->x+1)/2;
		visuel->couleurAiles[4*i+1] = (visuel->solide->lesNormales[i]->y+1)/2;
		visuel->couleurAiles[4*i+2] = (visuel->solide->lesNormales[i]->z+1)/2;
		visuel->couleurAiles[4*i+3] = 0.5;
	}
}

void saa_visuel_construire_listes_sommets(SaaVisuel * visuel) {
	int i, n;
	SaaSommet * s;
	GLUquadricObj * quad;

	if(!visuel->solide) return;

	n = visuel->solide->nbSommets;
	//glDeleteLists(visuel->listesSommets, n);
	visuel->listesSommets = glGenLists(n);

	switch(visuel->visuSommets) {
	case SAA_SOMMET_NON:
		for(i=0; i<n; i++) {
			glNewList(visuel->listesSommets+i, GL_COMPILE);
			glEndList();
		}
		break;
	case SAA_SOMMET_POINT:
		for(i=0; i<n; i++) {
			s = visuel->solide->lesSommets[i];
			glNewList(visuel->listesSommets+i, GL_COMPILE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_POINTS);
					glVertex3f(s->x, s->y, s->z);
				glEnd();
			glEndList();
		}
		break;
	case SAA_SOMMET_MOLECULE:
		quad = gluNewQuadric();
		for(i=0; i<n; i++) {
			s = visuel->solide->lesSommets[i];
			glNewList(visuel->listesSommets+i, GL_COMPILE);
				glShadeModel(GL_SMOOTH);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glPushMatrix();
					glTranslatef(s->x, s->y, s->z);
					gluSphere(quad, visuel->tailleSommets, 16, 16);
				glPopMatrix();
			glEndList();
		}
		break;
	}
}

void saa_visuel_construire_listes_aretes(SaaVisuel * visuel) {
	int i, n;
	SaaArete * ar;
	SaaSommet s1s2;
	GLUquadricObj * quad;
	double d, phi, theta;

	if(!visuel->solide) return;

	n = visuel->solide->nbAretes;
	//glDeleteLists(visuel->listesAretes, n);
	visuel->listesAretes = glGenLists(n);

	switch(visuel->visuAretes) {
	case SAA_ARETE_NON:
		for(i=0; i<n; i++) {
			glNewList(visuel->listesAretes+i, GL_COMPILE);
			glEndList();
		}
		break;
	case SAA_ARETE_LIGNE:
		for(i=0; i<n; i++) {
			ar = visuel->solide->lesAretes[i];
			glNewList(visuel->listesAretes+i, GL_COMPILE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_LINES);
					glVertex3d(ar->s1->x, ar->s1->y, ar->s1->z);
					glVertex3d(ar->s2->x, ar->s2->y, ar->s2->z);
				glEnd();
			glEndList();
		}
		break;
	case SAA_ARETE_MOLECULE:
		quad = gluNewQuadric();
		for(i=0; i<n; i++) {
			ar = visuel->solide->lesAretes[i];
			saa_sommet_difference(ar->s1, ar->s2, &s1s2);
			d = saa_sommet_distance(&s1s2);
			phi = acos(s1s2.z/d) / M_PI * 180;
			theta = atan2(s1s2.y, s1s2.x) / M_PI * 180;
			glNewList(visuel->listesAretes+i, GL_COMPILE);
				glShadeModel(GL_SMOOTH);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glPushMatrix();
					glTranslated((ar->s1->x+ar->s2->x)/2, (ar->s1->y+ar->s2->y)/2, (ar->s1->z+ar->s2->z)/2);
					glRotatef(theta, 0, 0, 1);
					glRotatef(phi, 0, 1, 0);
					glTranslated(0, 0, -d/2);
					gluCylinder(quad, visuel->tailleAretes, visuel->tailleAretes, d, 16, 1);
				glPopMatrix();
			glEndList();
		}
		break;
	}
}

void saa_visuel_construire_listes_ailes(SaaVisuel * visuel) {
	int i, n;
	SaaAile * ai;
	SaaSommet g, * norm;
	SaaArete * ar, * sar;

	if(!visuel->solide) return;

	n = visuel->solide->nbAiles;
	//glDeleteLists(visuel->listesAiles, n);
	visuel->listesAiles = glGenLists(n);

	switch(visuel->visuAiles) {
	case SAA_AILE_NON:
		for(i=0; i<n; i++) {
			glNewList(visuel->listesAiles+i, GL_COMPILE);
			glEndList();
		}
		break;
	case SAA_AILE_TRIANGULATION:
		for(i=0; i<n; i++) {
			ai = visuel->solide->lesAiles[i];
			saa_aile_barycentre(ai, &g);
			glNewList(visuel->listesAiles+i, GL_COMPILE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_TRIANGLE_FAN);
					ar = ai->start;
					glVertex3d(g.x, g.y, g.z);
					do {
					  if(ar->ai1 == ai) {
						glVertex3d(ar->s1->x, ar->s1->y, ar->s1->z);
						sar = ar->succ1;
						if(sar == ai->start)
						  glVertex3d(ar->s2->x, ar->s2->y, ar->s2->z);
					  }
					  else {
						glVertex3d(ar->s2->x, ar->s2->y, ar->s2->z);
						sar = ar->succ2;
						if(sar == ai->start)
						  glVertex3d(ar->s1->x, ar->s1->y, ar->s1->z);
					  }
					  ar = sar;
					} while(ar != ai->start);
				glEnd();
			glEndList();
		}
		break;
	case SAA_AILE_COULEUR:
		for(i=0; i<n; i++) {
			ai = visuel->solide->lesAiles[i];
			norm = visuel->solide->lesNormales[i];
			saa_aile_barycentre(ai, &g);

			glNewList(visuel->listesAiles+i, GL_COMPILE);
				glShadeModel(GL_FLAT);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_TRIANGLE_FAN);
		      		glNormal3dv((GLdouble *)norm);
					glVertex3d(g.x, g.y, g.z);
					ar = ai->start;
					do {
					  if(ar->ai1 == ai) {
						glNormal3dv((GLdouble *)norm);
						glVertex3d(ar->s1->x, ar->s1->y, ar->s1->z);
						sar = ar->succ1;
						if(sar == ai->start) {
							glNormal3dv((GLdouble *)norm);
							glVertex3d(ar->s2->x, ar->s2->y, ar->s2->z);
						}
					  }
					  else {
						glNormal3dv((GLdouble *)norm);
						glVertex3d(ar->s2->x, ar->s2->y, ar->s2->z);
						sar = ar->succ2;
						if(sar == ai->start) {
							glNormal3dv((GLdouble *)norm);
							glVertex3d(ar->s1->x, ar->s1->y, ar->s1->z);
						}
					  }
					  ar = sar;
					} while(ar != ai->start);
				glEnd();
			glEndList();
		}
		break;
	}
}

int saa_visuel_traiter_solide(SaaVisuel * visuel, SaaVisuelTraitement trait) {
	int res = 0;
	switch(trait) {
	case SAA_TRAITEMENT_DECOUPAGE:
		res = saa_solide_decouper(visuel->solide);
		break;
	case SAA_TRAITEMENT_EVIDAGE:
		res = saa_solide_evider(visuel->solide);
		break;
	case SAA_TRAITEMENT_FUSION:
		res = saa_solide_fusionner(visuel->solide);
		break;
	case SAA_TRAITEMENT_SEGMENTATION:
		res = saa_solide_segmenter(visuel->solide);
		break;
	case SAA_TRAITEMENT_DECROISEMENT:
		res = saa_solide_decroiser(visuel->solide);
		break;
	}
	if(!res) {
		saa_visuel_maj_couleurs(visuel);
		saa_visuel_construire_listes(visuel);
	}
	return res;
}

void saa_visuel_couleur_fond(SaaVisuel * visuel, GLfloat r, GLfloat g, GLfloat b) {
	visuel->couleurFond[0] = r;
	visuel->couleurFond[1] = g;
	visuel->couleurFond[2] = b;
	glClearColor(r, g, b, 0.);
}

void saa_visuel_couleur_sommets(SaaVisuel * visuel, GLfloat r, GLfloat g, GLfloat b) {
	visuel->couleurSommets[0] = r;
	visuel->couleurSommets[1] = g;
	visuel->couleurSommets[2] = b;
}

void saa_visuel_couleur_aretes(SaaVisuel * visuel, GLfloat r, GLfloat g, GLfloat b) {
	visuel->couleurAretes[0] = r;
	visuel->couleurAretes[1] = g;
	visuel->couleurAretes[2] = b;
}

void saa_visuel_taille_sommets(SaaVisuel * visuel, GLfloat t) {
	visuel->tailleSommets = t;
	saa_visuel_construire_listes_sommets(visuel);
}

void saa_visuel_taille_aretes(SaaVisuel * visuel, GLfloat t) {
	visuel->tailleAretes = t;
	saa_visuel_construire_listes_aretes(visuel);
}

void saa_visuel_construire_listes(SaaVisuel * visuel) {
	glInitNames();
	saa_visuel_construire_listes_sommets(visuel);
	saa_visuel_construire_listes_aretes(visuel);
	saa_visuel_construire_listes_ailes(visuel);
}
