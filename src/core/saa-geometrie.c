/****************************** saa-geometrie.c ******************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Calculs de géométrie affine sur les sommets                             */
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
#include "saa-geometrie.h"
#include <stdlib.h>
#include <math.h>

/*================================= SOMMETS =================================*/
// Cree un nouveau sommet
SaaSommet * saa_sommet_creer(double x, double y, double z) {
  SaaSommet * new = (SaaSommet *)g_malloc(sizeof(SaaSommet));
  g_return_val_if_fail(new, NULL);
  new->x = x;
  new->y = y;
  new->z = z;
  return new;
}

SaaSommet * saa_sommet_copier(const SaaSommet * s) {
	SaaSommet * new = (SaaSommet *)g_malloc(sizeof(SaaSommet));
	g_return_val_if_fail(new, NULL);
	new->x = s->x;
	new->y = s->y;
	new->z = s->z;
	return new;
}

// Distance d'un sommet a l'origine
double saa_sommet_distance(const SaaSommet * s) {
  return sqrt(s->x*s->x + s->y*s->y + s->z*s->z);
}

// Produit scalaire entre 2 vecteurs
double saa_sommet_ps(const SaaSommet * v1, const SaaSommet * v2) {
  return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

// Vecteur entre 2 sommets
void saa_sommet_difference(const SaaSommet * s1, const SaaSommet * s2, SaaSommet * v) {
  v->x = s2->x - s1->x;
  v->y = s2->y - s1->y;
  v->z = s2->z - s1->z;
}

// Addition de 2 vecteurs
void saa_sommet_ajout(const SaaSommet * s1, const SaaSommet * s2, SaaSommet * v) {
  v->x = s2->x + s1->x;
  v->y = s2->y + s1->y;
  v->z = s2->z + s1->z;
}

// Produit vectoriel entre 2 vecteurs
void saa_sommet_pv(const SaaSommet * v1, const SaaSommet * v2, SaaSommet * pv) {
  pv->x = v1->y*v2->z - v1->z*v2->y;
  pv->y = v1->z*v2->x - v1->x*v2->z;
  pv->z = v1->x*v2->y - v1->y*v2->x;
}

// Compare si deux sommets sont assimilables
int saa_sommet_comparer(const SaaSommet * s1, const SaaSommet * s2) {
	SaaSommet v;

	saa_sommet_difference(s1, s2, &v);
	return saa_sommet_distance(&v) < EPSILON_2 ? 0 :
			(s1->x  < s2->x) ? 1 :
			(s1->x == s2->x && s1->y  < s2->y) ? 1 :
			(s1->x == s2->x && s1->y == s2->y && s1->z < s2->z) ? 1 : -1;
}

// Angle entre deux vecteurs appliques a un sommet s
double saa_sommet_calculer_angle(const SaaSommet * s, const SaaSommet * v1, const SaaSommet * v2) {
  double dd, cosa, sina, d, sign;
  SaaSommet pv, orig = {0,0,0};
  dd = saa_sommet_distance(v1)*saa_sommet_distance(v2);
  cosa = saa_sommet_ps(v1, v2)/dd;
  saa_sommet_pv(v1, v2, &pv);
  d = saa_sommet_ps(&orig, &pv);
  sign = (saa_sommet_ps(&pv, &pv) + d > 0) ? 1 : -1;
  sina = sign*saa_sommet_distance(&pv)/dd;
  return atan2(sina, cosa);
}

int saa_sommet_coplanarite(const SaaSommet * A, const SaaSommet * B, const SaaSommet * C, const SaaSommet * D) {
	SaaSommet AB, AC, AD;
	double pm;
	saa_sommet_difference(A, B, &AB);
	saa_sommet_difference(A, C, &AC);
	saa_sommet_difference(A, D, &AD);
	pm = AB.x * (AC.y * AD.z - AC.z * AD.y)
	   - AB.y * (AC.x * AD.z - AC.z * AD.x)
	   + AB.z * (AC.x * AD.y - AC.y * AD.x);

	return pm < EPSILON_3;
}

int saa_sommet_intersection_segments(const SaaSommet * A, const SaaSommet * B, const SaaSommet * C, const SaaSommet * D, SaaSommet * I) {
	int enIntersection = 0;
	SaaSommet AB, CD, AC, i;
	double det, k1 = -1, k2 = -1;

	// A = C ou A = D => I = A
	if((enIntersection = !saa_sommet_comparer(A, C) || !saa_sommet_comparer(A, D))) {
		I->x = A->x;
		I->y = A->y;
		I->z = A->z;
	// B = C ou B = D => I = B
	} else if((enIntersection = !saa_sommet_comparer(B, C) || !saa_sommet_comparer(B, D))) {
		I->x = B->x;
		I->y = B->y;
		I->z = B->z;
	} else {
		saa_sommet_difference(A, B, &AB);
		saa_sommet_difference(C, D, &CD);
		saa_sommet_difference(A, C, &AC);

		if((det = AB.x * CD.y - AB.y * CD.x) != 0) {
			k1 = (AC.x * CD.y - AC.y * CD.x)/det;
			k2 = (AC.x * AB.y - AC.y * AB.x)/det;
		} else if((det = AB.x * CD.z - AB.z * CD.x) != 0) {
			k1 = (AC.x * CD.z - AC.z * CD.x)/det;
			k2 = (AC.x * AB.z - AC.z * AB.x)/det;
		} else if((det = AB.y * CD.z - AB.z * CD.y) != 0) {
			k1 = (AC.y * CD.z - AC.z * CD.y)/det;
			k2 = (AC.y * AB.z - AC.z * AB.y)/det;
		} else {
			//fprintf(SaaErrorFile, "saa_sommet_intersection_droites : segments paralleles\n");
		}

		I->x = A->x + k1 * AB.x;
		I->y = A->y + k1 * AB.y;
		I->z = A->z + k1 * AB.z;
		i.x = C->x + k2 * CD.x;
		i.y = C->y + k2 * CD.y;
		i.z = C->z + k2 * CD.z;
		enIntersection = k1 > - EPSILON_2 && k1 < 1 + EPSILON_2 &&
						k2 > - EPSILON_2 && k2 < 1 + EPSILON_2 &&
						!saa_sommet_comparer(I, &i);
//printf("inter %s  : k1 = %f ; k2 = %f ; %f %f %f\n", enIntersection?"oui":"non", k1, k2, I->x, I->y, I->z);
	}

	return enIntersection;
}

int saa_sommet_est_sur_segment(const SaaSommet * s, const SaaSommet * A, const SaaSommet * B) {
	SaaSommet AB, As;
	double kx, ky, kz, kd, dAB, dAs;

	saa_sommet_difference(A, B, &AB);
	saa_sommet_difference(A, s, &As);
	dAB = saa_sommet_distance(&AB);
	dAs = saa_sommet_distance(&As);
	kx = (fabs(AB.x) < EPSILON_1 && fabs(As.x) < EPSILON_1) ? 1 : As.x / AB.x;
	ky = (fabs(AB.y) < EPSILON_1 && fabs(As.y) < EPSILON_1) ? 1 : As.y / AB.y;
	kz = (fabs(AB.z) < EPSILON_1 && fabs(As.z) < EPSILON_1) ? 1 : As.z / AB.z;
	kd = dAs / dAB;

//printf("k(x,y,z) = %f %f %f\n", kx, ky, kz);
	// As = k AB  et 0 < k < 1
	return  (fabs(kx - ky) < EPSILON_3 || kx == 1 || ky == 1) &&
			(fabs(kx - kz) < EPSILON_3 || kx == 1 || kz == 1) &&
			(fabs(ky - kz) < EPSILON_3 || ky == 1 || kz == 1) &&
			(fabs(kx - kd) < EPSILON_3 || kx == 1) &&
			(fabs(ky - kd) < EPSILON_3 || ky == 1) &&
			(fabs(kz - kd) < EPSILON_3 || kz == 1) &&
			kx > - EPSILON_1 && kx < 1 + EPSILON_1 &&
			ky > - EPSILON_1 && ky < 1 + EPSILON_1 &&
			kz > - EPSILON_1 && kz < 1 + EPSILON_1 &&
			kd > - EPSILON_1 && kd < 1 + EPSILON_1;
}

// Cree un nouveau plan
SaaPlan * saa_plan_creer(double a, double b, double c, double d) {
	  SaaPlan * new = (SaaPlan *)g_malloc(sizeof(SaaPlan));
	  g_return_val_if_fail(new, NULL);
	  new->a = a;
	  new->b = b;
	  new->c = c;
	  new->d = d;
	  return new;
}

// Calcule si un sommet est dans le plan
int saa_plan_cote_sommet(const SaaPlan * p, const SaaSommet * S) {
	SaaSommet Sp, S0, S0S, S0Sp;
	SaaPlan p0;
	double d, k = 0, nk = 0;
	int result;

	p0.a = p->a;
	p0.b = p->b;
	p0.c = p->c;
	p0.d = 0;

	saa_plan_projeter_ortho(p, S, &Sp);
	saa_plan_projeter_ortho(&p0, S, &S0);

	saa_sommet_difference(&S0, &Sp, &S0Sp);
	saa_sommet_difference(&S0, S, &S0S);

	d = saa_sommet_distance(&S0Sp);

	if(fabs(S0Sp.x) > EPSILON_1) { k += S0S.x / S0Sp.x; nk++; }
	if(fabs(S0Sp.y) > EPSILON_1) { k += S0S.y / S0Sp.y; nk++; }
	if(fabs(S0Sp.z) > EPSILON_1) { k += S0S.z / S0Sp.z; nk++; }
	if(nk)  k /= nk;

	if(k < - EPSILON_2) {
		result = -1;
	} else if(k < 1 - EPSILON_2) {
		result = 1;
	} else if(fabs(k - 1) < EPSILON_2) {
		result = 0;
	} else { // k > 1 + EPSILON_2
		result = 2;
	}

	// TODO Enlever le code mort
	//eq = p->a*S->x + p->b*S->y + p->c*S->z + p->d;
	//eq = fabs(eq) < EPSILON_2 ? 0 : eq;
	//if(fabs(eq)<EPSILON_1 && result || !result && eq) printf("saa_plan_cote_sommet : k = %f ; d = %f ; eq = %f ; result = %d\n", k, d, eq, result);

	return result;
}

// Calcule l'intersection d'un segment avec le plan
int saa_plan_intersection_segment(const SaaPlan * p, const SaaSommet * A, const SaaSommet * B, SaaSommet * I) {
	double a = p->a, b = p->b, c = p->c, d = p->d, k = -1, div;
	SaaSommet AB;

	saa_sommet_difference(A, B, &AB);
	div = a * AB.x + b * AB.y + c * AB.z;
	if(fabs(div) > EPSILON_1) {
		k = -(a * A->x + b * A->y + c * A->z + d)/div;
		I->x = A->x + k * AB.x;
		I->y = A->y + k * AB.y;
		I->z = A->z + k * AB.z;
	}
//printf("k = %f\n", k);
	return k > - EPSILON_3 && k < 1 + EPSILON_3;
}

// Vecteur normal normalisé
void saa_plan_vecteur_normal(const SaaPlan * p, SaaSommet * n) {
	double dn = sqrt(p->a*p->a + p->b*p->b + p->c*p->c);
	n->x = p->a / dn;
	n->y = p->b / dn;
	n->z = p->c / dn;
}

// Projection orthogonale sur un plan
void saa_plan_projeter_ortho(const SaaPlan * p, const SaaSommet * S, SaaSommet * Sp) {
	double a = p->a, b = p->b, c = p->c, d = p->d, l = a*a + b*b + c*c;
	double eq = a*S->x + b*S->y + c*S->z +d;

	if(fabs(eq) < EPSILON_1) {
		Sp->x = S->x;
		Sp->y = S->y;
		Sp->z = S->z;
	} else {
		Sp->x = ((b*b + c*c) * S->x         - a*b * S->y         - a*c * S->z - d*a)/l;
		Sp->y = (      - a*b * S->x + (a*a + c*c) * S->y         - b*c * S->z - d*b)/l;
		Sp->z = (      - a*c * S->x         - b*c * S->y + (a*a + b*b) * S->z - d*c)/l;
	}
}

// modulo 2*PI radians
double saa_mod2pi(double angle) {
	  while(angle > 2*M_PI - EPSILON_1) angle-=2*M_PI;
	  while(angle < - EPSILON_1)        angle+=2*M_PI;
  return angle;
}
