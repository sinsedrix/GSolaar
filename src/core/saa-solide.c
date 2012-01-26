/******************************* saa-solide.c ********************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Modélisation arêtes ailees des polyèdres                                */
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
#include "saa-solide.h"

#include <stdio.h>
#include <math.h>
#include <glib.h>

static void saa_xml_start(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names,
                          const gchar **attribute_values, gpointer user_data, GError **error);
static void saa_xml_end(GMarkupParseContext *context, const gchar *element_name,
						gpointer user_data, GError **error);
static void saa_xml_error(GMarkupParseContext *context, GError *error, gpointer user_data);
static void saa_xml_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error);
static void saa_xml_passthrough(GMarkupParseContext *context, const gchar *passthrough_text, gsize text_len, gpointer user_data, GError **error);

static GMarkupParser saa_xml_parser = {saa_xml_start, saa_xml_end, saa_xml_text, saa_xml_passthrough, saa_xml_error};

/*================================= ARETES ==================================*/
/* Operations sur les aretes */
// Angle entre 2 aretes pour une aile commune
double saa_arete_calculer_angle(const SaaArete * ar1, const SaaArete * ar2) {
  double dd, cosa, sina, sign;
  SaaSommet * s1 = NULL, * s2 = NULL, * s3 = NULL, * s4 = NULL;
  SaaSommet v1, v2, pv, p;
  SaaAile * ai;

  if(ar1->ai1 == ar2->ai1) {
    s1 = ar1->s1;
    s2 = ar1->s2;
    s3 = ar2->s1;
    s4 = ar2->s2;
    ai = ar1->ai1;
  }
  else if(ar1->ai1 == ar2->ai2) {
    s1 = ar1->s1;
    s2 = ar1->s2;
    s3 = ar2->s2;
    s4 = ar2->s1;
    ai = ar1->ai1;
  }
  else if(ar1->ai2 == ar2->ai2) {
    s1 = ar1->s2;
    s2 = ar1->s1;
    s3 = ar2->s2;
    s4 = ar2->s1;
    ai = ar1->ai2;
  }
  else if(ar1->ai2 == ar2->ai1) {
    s1 = ar1->s2;
    s2 = ar1->s1;
    s3 = ar2->s1;
    s4 = ar2->s2;
    ai = ar1->ai2;
  }
  saa_sommet_difference(s1, s2, &v1);
  saa_sommet_difference(s3, s4, &v2);
  saa_sommet_pv(&v1, &v2, &pv);
  saa_sommet_ajout(&pv, s1, &p);

  sign = (saa_sommet_ps(&p, &pv) + saa_sommet_ps(s1, &pv) > 0) ? 1 : -1;
  /* TODO enlever le code mort
  saa_aile_plan(ai, &pai);
  c = saa_plan_contient_sommet(&pai, &p);
  sign = c ? c/fabs(c) : 0;
  */
  dd = saa_sommet_distance(&v1)*saa_sommet_distance(&v2);
  cosa = saa_sommet_ps(&v1, &v2)/dd;
  sina = sign*saa_sommet_distance(&pv)/dd;
  return atan2(sina, cosa);
}

// Teste si deux aretes ont les memes points
int saa_arete_comparer(const SaaArete * ar1, const SaaArete * ar2) {
  return
    (ar1->s1 == ar2->s1 && ar1->s2 == ar2->s2) ||
    (ar1->s1 == ar2->s2 && ar1->s2 == ar2->s1);
}

/*================================== AILES ==================================*/
/* Operations sur les ailes */
// Nombre d'aretes dans l'aile
int saa_aile_nb_aretes(const SaaAile * ai) {
	SaaArete * ar;
	int nb = 0;

	ar = ai->start;
	do { ar = ar->ai1 == ai ? ar->succ1 : ar->succ2; nb++; } while(ar != ai->start);
	return nb;
}

// Calcule le barycentre d'une aile
void saa_aile_barycentre(const SaaAile * ai, SaaSommet * g) {
  SaaArete * ar;
  int nbSommets = 0;
  SaaSommet * s;
  g->x = g->y = g->z = 0;
  ar = ai->start;
  do {
    s = (ai == ar->ai1) ? ar->s1 : ar->s2;
    ar = (ai == ar->ai1) ? ar->succ1 : ar->succ2;
    g->x += s->x;
    g->y += s->y;
    g->z += s->z;
    nbSommets++;
  } while(ar != ai->start);
  g->x /= nbSommets;
  g->y /= nbSommets;
  g->z /= nbSommets;
}

// Calcule le vecteur normal a une aile
// - on utilise la somme des produits vectoriels
void saa_aile_vecteur_normal(const SaaAile * ai, SaaSommet * n) {
	SaaSommet v1, v2, pv, spv = {0.,0.,0.};
	SaaArete * ar, * sar;
	ar = ai->start;
	do {
		sar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
		if(ar->ai1 == ai)
			saa_sommet_difference(ar->s1, ar->s2, &v1);
		else
			saa_sommet_difference(ar->s2, ar->s1, &v1);

		if(sar->ai1 == ai)
			saa_sommet_difference(sar->s1, sar->s2, &v2);
		else
			saa_sommet_difference(sar->s2, sar->s1, &v2);

		saa_sommet_pv(&v1, &v2, &pv);
		saa_sommet_ajout(&spv, &pv, &spv);
		ar = sar;
	} while(ar != ai->start);
	n->x = spv.x;
	n->y = spv.y;
	n->z = spv.z;
}

void saa_aile_plan(const SaaAile * ai, SaaPlan * pai) {
	SaaSommet n;
	saa_aile_vecteur_normal(ai, &n);
	pai->a = n.x;
	pai->b = n.y;
	pai->c = n.z;
	pai->d = - saa_sommet_ps(&n, ai->start->s1);
}

// Indique la position d'un sommet par rapport à une aile
// 0 : sur le plan, 1 : entre le plan et l'origine, 2 : apres le plan, -1 : avant l'origine
int saa_aile_cote_sommet(const SaaAile * ai, const SaaSommet * S) {
  SaaSommet n, Sp, SpS, G, Gp, GpG;
  SaaPlan pai;
  double dSpS, dGpG, k = 0, nk = 0;
  int result;

  saa_aile_plan(ai, &pai);
  pai.d = 0;
  saa_plan_vecteur_normal(&pai, &n);

  saa_plan_projeter_ortho(&pai, S, &Sp);
  saa_sommet_difference(&Sp, S, &SpS);
  dSpS = saa_sommet_distance(&SpS);

  saa_aile_barycentre(ai, &G);
  saa_plan_projeter_ortho(&pai, &G, &Gp);
  saa_sommet_difference(&Gp, &G, &GpG);
  dGpG = saa_sommet_distance(&GpG);

  if(fabs(n.x) > EPSILON_1) { k += SpS.x / n.x; nk++; }
  if(fabs(n.y) > EPSILON_1) { k += SpS.y / n.y; nk++; }
  if(fabs(n.z) > EPSILON_1) { k += SpS.z / n.z; nk++; }
  if(nk)  k /= nk;

  if(k < - EPSILON_2) {
	  result = -1;
  } else if(k < dGpG - EPSILON_2) {
	  result = 1;
  } else if(fabs(k - dGpG) < EPSILON_2) {
	  result = 0;
  } else { // k > dGpG + EPSILON_2
	  result = 2;
  }
//printf("saa_aile_cote_sommet %f %f %f --> %d\n", k, dGpG, dSpS, result);
  return result;
}

// Calcule le sommet d'intersection entre le plan de l'aile et un segment
// s est alloue, on y stocke le point d'intersection
//  valeur de retour : cote des deux points par rapport a l'aile
int saa_aile_intersection_segment(const SaaAile * ai, const SaaSommet * A, const SaaSommet * B, SaaSommet * I) {
  SaaPlan pai;

  saa_aile_plan(ai, &pai);
  return saa_plan_intersection_segment(&pai, A, B, I) && saa_aile_contient_sommet(ai, I);
}

// Teste si une face est croisee
//   on utilise la somme des angle qui doit valoir 2*pi
int saa_aile_est_croisee(const SaaAile * ai) {
  SaaArete * ar, * sar;
  double angle = 0;
  ar = ai->start;
  do {
    sar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
    angle += saa_arete_calculer_angle(ar, sar);
    ar = sar;
  } while(sar != ai->start);
  return fabs(fabs(angle) - 2*M_PI) > EPSILON_3;
}

// Compare si eux ailes ont la meme succession de points
int saa_aile_comparer(const SaaAile * ai1, const SaaAile * ai2) {
  int i1, i2;
  int nbs1, nbs2;
  SaaArete * ar;
  SaaSommet * s;
  SaaSommet ** sai1, ** sai2;
  // On compte les nombres de sommets
  nbs1 = 0;
  ar = ai1->start;
  do {
    nbs1++;
    ar = (ar->ai1 == ai1) ? ar->succ1 : ar->succ2;
  } while(ar != ai1->start);
  nbs2 = 0;
  ar = ai2->start;
  do {
    nbs2++;
    ar = (ar->ai1 == ai2) ? ar->succ1 : ar->succ2;
  } while(ar != ai2->start);
  if(nbs1 != nbs2) return -1;

  // On recupere les successions de sommets
  sai1 = (SaaSommet **)g_malloc(nbs1*sizeof(SaaSommet *));
  nbs1 = 0;
  ar = ai1->start;
  do {
    s  = (ar->ai1 == ai1) ? ar->s1 : ar->s2;
    ar = (ar->ai1 == ai1) ? ar->succ1 : ar->succ2;
    sai1[nbs1++] = s;
  } while(ar != ai1->start);

  sai2 = (SaaSommet **)g_malloc(nbs2*sizeof(SaaSommet *));
  nbs2 = 0;
  ar = ai2->start;
  do {
    s  = (ar->ai1 == ai2) ? ar->s1 : ar->s2;
    ar = (ar->ai1 == ai2) ? ar->succ1 : ar->succ2;
    sai2[nbs2++] = s;
  } while(ar != ai2->start);

  // On compare les succession de points
  for(i2=0; i2<nbs2 && sai2[i2] != sai1[0]; i2++);
  if(i2 == nbs2) {
    g_free(sai1);
    g_free(sai2);
    return -2;
  }
  for(i1=0; i1<nbs1 && sai1[i1] == sai2[(i1+i2)%nbs1]; i1++);
  if(i1 != nbs1) {
    g_free(sai1);
    g_free(sai2);
    return -3;
  }
  return 0;
}

// Test de point interieur a une aile par triangulation (strangulation ;o)
int saa_aile_contient_sommet(const SaaAile * ai, const SaaSommet * s) {
	int boolin;
	SaaArete * ar;
	SaaSommet g, si;
	SaaPlan pai;

	saa_aile_barycentre(ai, &g);
	saa_aile_plan(ai, &pai);
//printf("saa_aile_contient_sommet saa_plan_cote_sommet %d\n", saa_plan_cote_sommet(&pai, s));
	boolin = saa_plan_cote_sommet(&pai, s) == 0;

	ar = ai->start;
	do {
		boolin &= !saa_sommet_intersection_segments(s, &g, ar->s1, ar->s2, &si) || saa_sommet_est_sur_segment(s, ar->s1, ar->s2);
		ar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
	} while(ar != ai->start && boolin);
	return boolin;
}

/* Operations sur les solides aretes ailees */
// Calcul du barycentre g d'un solide
// g est alloue, on y stocke le barycentre
void saa_solide_barycentre(const SaaSolide * sol, SaaSommet * g) {
  int i;
  g->x = g->y = g->z = 0;
  for(i=0; i<sol->nbSommets; i++)
    saa_sommet_ajout(sol->lesSommets[i], g, g);
  g->x /= sol->nbSommets;
  g->y /= sol->nbSommets;
  g->z /= sol->nbSommets;
}

// Creation d'un nouveau solide (vide)
SaaSolide * saa_solide_creer(const char * nom) {
  SaaSolide * new = (SaaSolide *)g_malloc(sizeof(SaaSolide));

  new->nom = g_strdup(nom);
  saa_casse_chameau(new->nom);
  saa_sans_espace(new->nom);
  new->lesAiles = NULL;
  new->lesAretes = NULL;
  new->lesSommets = NULL;
  new->nbAiles = 0;
  new->nbAretes = 0;
  new->nbSommets = 0;
  return new;
}

// Detruit un solide en liberant de espace memoire
void saa_solide_detruire(SaaSolide * s) {
  int i;

  g_free(s->nom);
  if(s->lesAiles) {
    for(i=0; i<s->nbAiles; i++)
      g_free(s->lesAiles[i]);
    g_free(s->lesAiles);
  }

  if(s->lesNormales) {
    for(i=0; i<s->nbAiles; i++)
      g_free(s->lesNormales[i]);
    g_free(s->lesNormales);
  }

  if(s->lesAretes) {
    for(i=0; i<s->nbAretes; i++)
      g_free(s->lesAretes[i]);
    g_free(s->lesAretes);
  }
  if(s->lesSommets) {
    for(i=0; i<s->nbSommets; i++)
      g_free(s->lesSommets[i]);
    g_free(s->lesSommets);
  }
  g_free(s);
}

// Affichage d'un solide sur un flux qq
void saa_solide_ecrire_txt(const SaaSolide * sol, FILE * f) {
  int i;
  fprintf(f, "Solide : %s\n", sol->nom);
  // Sommets
  fprintf(f, "Sommets (%d) :\n", sol->nbSommets);
  for(i=0; i<sol->nbSommets; i++)
    saa_solide_ecrire_sommet_txt(sol, f, sol->lesSommets[i]);
  // Aretes
  fprintf(f, "Aretes (%d) :\n", sol->nbAretes);
  for(i=0; i<sol->nbAretes; i++)
    saa_solide_ecrire_arete_txt(sol, f, sol->lesAretes[i]);
  // Ailes
  fprintf(f, "Ailes (%d) :\n", sol->nbAiles);
  for(i=0; i<sol->nbAiles; i++)
    saa_solide_ecrire_aile_txt(sol, f, sol->lesAiles[i]);
}

// Affichage d'un solide au format XML sur un flux qq
void saa_solide_ecrire_xml(const SaaSolide * sol, FILE * f) {
  int i, idx = sol->p ? sol->p->index+1 : rand();
  // En-tete
  fprintf(f, "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");
  fprintf(f, "%s", "<!-- Created with GSolaar (http://source-forge.net/projects/gsolaar/) -->\n");
  // Solaar NS
  fprintf(f, "<saa version=\"1.0\" author=\"sinsedrix@source-forge.net\">\n");
  fprintf(f, "\t<solid id=\"sol%d\" name=\"%s\">\n", idx, sol->nom);
  // Sommets
  fprintf(f, "\t\t<vertices>\n");
  for(i=0; i<sol->nbSommets; i++)
    saa_solide_ecrire_sommet_xml(sol, f, sol->lesSommets[i]);
  fprintf(f, "\t\t</vertices>\n");
  // Aretes
  fprintf(f, "\t\t<edges>\n");
  for(i=0; i<sol->nbAretes; i++)
    saa_solide_ecrire_arete_xml(sol, f, sol->lesAretes[i]);
  fprintf(f, "\t\t</edges>\n");
  // Ailes
  fprintf(f, "\t\t<wings>\n");
  for(i=0; i<sol->nbAiles; i++)
    saa_solide_ecrire_aile_xml(sol, f, sol->lesAiles[i]);
  fprintf(f, "\t\t</wings>\n");
  fprintf(f, "\t</solid>\n");
  fprintf(f, "</saa>\n");
}

// Generation d'un fichier TXT
void saa_solide_exporter_TXT(const SaaSolide * s, const char * filename) {
  FILE * f;

  fprintf(SaaLogFile, "> EXPORT FICHIER TXT\n");

  fprintf(SaaLogFile, "- Ouverture du fichier %s\n", filename);
  f = fopen(filename, "w");
  if (!f) {
	  fprintf(SaaErrorFile, "Erreur a l'ouverture du fichier destination %s\n", filename);
	  return;
  }
  saa_solide_ecrire_txt(s, f);
  fclose(f);
  fprintf(SaaLogFile, "< EXPORT FICHIER TXT\n");
}

// Generation d'un fichier XML
void saa_solide_exporter_XML(const SaaSolide * s, const char * filename) {
	  FILE * f;

	  fprintf(SaaLogFile, "> EXPORT FICHIER XML\n");

	  fprintf(SaaLogFile, "- Ouverture du fichier %s\n", filename);
	  f = fopen(filename, "w");
	  if (!f) {
		  fprintf(SaaErrorFile, "Erreur a l'ouverture du fichier destination %s\n", filename);
		  return;
	  }
	  saa_solide_ecrire_xml(s, f);
	  fclose(f);
	  fprintf(SaaLogFile, "< EXPORT FICHIER XML\n");
}

// Ajout d'un sommet au solide (s'il n'y est pas deja)
SaaSommet * saa_solide_ajouter_sommet(SaaSolide * sol, const SaaSommet * s) {
  int i, traite = 0;
  SaaSommet * sres = NULL;

  // On cherche si ce sommet existe deja sous une autre reference
  for(i=0; i<sol->nbSommets && !traite; i++)
    if(!saa_sommet_comparer(s, sol->lesSommets[i])) {
      sres = sol->lesSommets[i];
      traite = 1;
    }
  // sinon on l'ajoute
  if(!traite) {
    sol->lesSommets = (SaaSommet **)g_realloc(sol->lesSommets, (sol->nbSommets+1)*sizeof(SaaSommet *));
    sres = sol->lesSommets[sol->nbSommets++] = saa_sommet_creer(s->x, s->y, s->z);
  }
  return sres;
}

// Ajout d'une arete au solide (si elle n'y est pas deja)
SaaArete * saa_solide_ajouter_arete(SaaSolide * sol, SaaArete * ar) {
  int i, traite = 0;
  SaaArete * arres = NULL;

  // On cherche si cette arete existe deja sous une autre reference
  for(i=0; i<sol->nbAretes && !traite; i++)
    if(saa_arete_comparer(ar, sol->lesAretes[i])) {
      arres = sol->lesAretes[i];
      traite = 1;
    }
  // sinon on l'ajoute
  if(!traite) {
    sol->lesAretes = (SaaArete **)g_realloc(sol->lesAretes, (sol->nbAretes+1)*sizeof(SaaArete *));

    arres = sol->lesAretes[sol->nbAretes] = ar;
    sol->nbAretes++;
  }
  return arres;
}

// Ajout d'une aile au solide (si elle n'y est pas deja)
SaaAile * saa_solide_ajouter_aile(SaaSolide * sol, SaaAile * ai) {
  int i, traite = 0;
  SaaAile * aires = NULL;

  // On cherche si cette aile existe deja sous une autre reference
  for(i=0; i<sol->nbAiles && !traite; i++)
    if(saa_aile_comparer(ai, sol->lesAiles[i])) { //1E-5
      aires = sol->lesAiles[i];
      traite = 1;
    }
  // sinon on l'ajoute
  if(!traite) {
    sol->lesAiles = (SaaAile **)g_realloc(sol->lesAiles, (sol->nbAiles+1)*sizeof(SaaAile *));

    aires = sol->lesAiles[sol->nbAiles] = ai;
    sol->nbAiles++;
  }
  return aires;
}

// Suppression d'un sommet dans un solide
void saa_solide_supprimer_sommet(SaaSolide * sol, SaaSommet * s) {
  int i, trouvs = 0;
  // on cherche le sommet
  for(i=0; i<sol->nbSommets && !trouvs; i++)
    if(s == sol->lesSommets[i]) {
      trouvs = 1;
      // on met le dernier sommet a sa place dans le tableau
      sol->lesSommets[i] = sol->lesSommets[sol->nbSommets-1];
      // on libere l'espace mem
      g_free(s);
      // on diminue la taille des tableaux
      sol->nbSommets--;
      sol->lesSommets = (SaaSommet **)g_realloc(sol->lesSommets, sol->nbSommets*sizeof(SaaSommet *));

    }
}

// Suppression d'une arete dans un solide
void saa_solide_supprimer_arete(SaaSolide * sol, SaaArete * ar) {
  int i, trouvar = 0;
  // on cherche l'arete
  for(i=0; i<sol->nbAretes && !trouvar; i++)
    if(ar == sol->lesAretes[i]) {
      trouvar = 1;
      // on met la derniere arete a sa place dans le tableau
      sol->lesAretes[i] = sol->lesAretes[sol->nbAretes-1];
      // on liber l'espace mem
      g_free(ar);
      // on diminue la taille des tableaux
      sol->nbAretes--;
      sol->lesAretes = (SaaArete **)g_realloc(sol->lesAretes, sol->nbAretes*sizeof(SaaArete *));

    }
}

// Suppression d'une aile dans un solide
void saa_solide_supprimer_aile(SaaSolide * sol, SaaAile * ai) {
  int i, trouvai = 0;

  // On met à NULL ses occurences dans les aretes
  for(i=0; i<sol->nbAretes; i++) {
	  if(sol->lesAretes[i]->ai1 == ai)
		  sol->lesAretes[i]->ai1 = NULL;
	  if(sol->lesAretes[i]->ai2 == ai)
		  sol->lesAretes[i]->ai2 = NULL;
  }
  // on cherche l'aile
  for(i=0; i<sol->nbAiles && !trouvai; i++)
    if(ai == sol->lesAiles[i]) {
      trouvai = 1;
      // on met la derniere aile a sa place dans le tableau
      sol->lesAiles[i] = sol->lesAiles[sol->nbAiles-1];
      // on met a jour la normale a la face
      sol->lesNormales[i] = sol->lesNormales[sol->nbAiles-1];
      // on libere l'espace mem
      g_free(ai);
      // on diminue la taille des tableaux
      sol->nbAiles--;
      sol->lesAiles = (SaaAile **)g_realloc(sol->lesAiles, sol->nbAiles*sizeof(SaaAile *));

      sol->lesNormales = (SaaSommet **)g_realloc(sol->lesNormales, sol->nbAiles*sizeof(SaaSommet *));

    }
}

// Cree un solide a partir d'un fichier de script
SaaSolide * saa_solide_importer_TXT(const char * filename) {
  int i, is1, is2, iar, iai;
  int nbSommets, nbAretes, nbAiles;
  SaaSolide * new;
  FILE * f;
  char nom[512];
  char bufsx[64], bufsy[64], bufsz[64];
  double sx, sy, sz;
  SaaArete * ar, * sar;
  SaaAile * ai;
  SaaSommet n, * stmp;
  double d;

  if((f=fopen(filename, "r"))== NULL) {
    fprintf(stderr, "Erreur a l'ouverture du script %s\n",  filename);
    exit(1);
  }

  // Nom du solide
  fscanf(f, "Solide : %s\n", nom);
  new = saa_solide_creer(nom);

  // Nombre de sommets
  fscanf(f, "Sommets (%d) :\n", &nbSommets);
  new->lesSommets = (SaaSommet **)g_malloc(nbSommets*sizeof(SaaSommet *));

  new->nbSommets = nbSommets;
  // Coordonnees des sommets
  for(i=0; i<nbSommets; i++) {
    if(fscanf(f, " s%d : ( %s , %s , %s )\n", &is1, bufsx, bufsy, bufsz) != 4) {
      fprintf(stderr, "Erreur de sommet L%d\n", i+3);
      new->nbSommets = i;
      saa_solide_detruire(new);
      return NULL;
    }
    if(feof(f)) {
      fprintf(stderr, "Fin de fichier inattendue L%d\n", i+3);
      new->nbSommets = i;
      saa_solide_detruire(new);
      return NULL;
    }
    sx = strtod(bufsx, NULL);
    sy = strtod(bufsy, NULL);
    sz = strtod(bufsz, NULL);
    new->lesSommets[i] = saa_sommet_creer(sx, sy, sz);
  }

  // Nombre d'aretes
  fscanf(f, "Aretes (%d) :\n", &nbAretes);
  new->lesAretes = (SaaArete **)g_malloc(nbAretes*sizeof(SaaArete *));

  new->nbAretes = nbAretes;
  // Sommets des aretes
  for(i=0; i<nbAretes; i++) {
    if(fscanf(f, " ar%d : s%d -- s%d\n", &iar, &is1, &is2) != 3) {
      fprintf(stderr, "Erreur d'arete L%d\n", nbSommets+i+4);
      new->nbAretes = i;
      saa_solide_detruire(new);
      return NULL;
    }
    if(feof(f)) {
      fprintf(stderr, "Fin de fichier inattendue L%d\n", nbSommets+i+4);
      new->nbAretes = i;
      saa_solide_detruire(new);
      return NULL;
    }
    new->lesAretes[i] = (SaaArete *)g_malloc(sizeof(SaaArete));

    new->lesAretes[i]->s1 = new->lesSommets[is1-1];
    new->lesAretes[i]->s2 = new->lesSommets[is2-1];
    new->lesAretes[i]->ai1 = NULL;
    new->lesAretes[i]->ai2 = NULL;
  }

  // Nombre d'ailes
  fscanf(f, "Ailes (%d) :\n", &nbAiles);
  new->lesAiles = (SaaAile **)g_malloc(nbAiles*sizeof(SaaAile *));

  new->lesNormales = (SaaSommet **)g_malloc(nbAiles*sizeof(SaaSommet *));

  new->nbAiles = nbAiles;
  for(i=0; i<nbAiles; i++) {
    new->lesAiles[i] = (SaaAile *)g_malloc(sizeof(SaaAile));

  }
  // Aretes des ailes
  for(i=0; i<nbAiles; i++) {
    if(fscanf(f, " ai%d : ar%d", &iai, &iar) != 2) {
      fprintf(stderr, "Erreur d'aile L%d\n",  nbAretes+nbSommets+i+5);
      new->nbAiles = i;
      saa_solide_detruire(new);
      return NULL;
    }
    ai = new->lesAiles[i];
    ar = ai->start = new->lesAretes[iar-1];
    while(fscanf(f, " ar%d", &iar) && !feof(f)) {
      sar = new->lesAretes[iar-1];
      if(saa_solide_index_aile(new, ar->ai1) < 0) {
        ar->ai1 = ai;
        ar->succ1 = sar;
      }
      else
      if(saa_solide_index_aile(new, ar->ai2) < 0) {
        ar->ai2 = ai;
        ar->succ2 = sar;
      }
      ar = sar;
    }
    fseek(f, -1, SEEK_CUR);
    sar = ai->start;
    if(saa_solide_index_aile(new, ar->ai1) < 0) {
      ar->ai1 = ai;
      ar->succ1 = sar;
    }
    else
    if(saa_solide_index_aile(new, ar->ai2) < 0) {
      ar->ai2 = ai;
      ar->succ2 = sar;
    }
    if(feof(f)) {
      fprintf(stderr, "Fin de fichier inattendue L%d\n", nbAretes+nbSommets+i+5);
      new->nbAiles = i;
      saa_solide_detruire(new);
      return NULL;
    }
  }
  fclose(f);

  // rectification des sens des aretes
  for(i=0; i<nbAiles; i++) {
    ai = new->lesAiles[i];
    ar = ai->start;
    do {
      if(ar->ai1 == ai) {
        if(ar->s1 == ar->succ1->s1 || ar->s1 == ar->succ1->s2)
          stmp = ar->s1, ar->s1 = ar->s2, ar->s2 = stmp;
        ar = ar->succ1;
      }
      else {
        if(ar->s2 == ar->succ2->s1 || ar->s2 == ar->succ2->s2)
          stmp = ar->s1, ar->s1 = ar->s2,ar->s2 = stmp;
        ar = ar->succ2;
      }
    } while(ar != ai->start);
  }

  // construction des normales
  for(i=0; i<nbAiles; i++) {
    saa_aile_vecteur_normal(new->lesAiles[i], &n);
    d = saa_sommet_distance(&n);
    new->lesNormales[i] = saa_sommet_creer(n.x/d, n.y/d, n.z/d);
  }
  return new;
}

// Cree un solide a partir d'un fichier XML
// filename : nom du fichier XML
SaaSolide * saa_solide_importer_XML(const char * filename) {
	  SaaSolide * new = saa_solide_creer(filename);
	  gchar * xml_data;
	  gboolean result = g_file_get_contents (filename, &xml_data, NULL, NULL);
	  GMarkupParseContext * context = g_markup_parse_context_new (&saa_xml_parser, 0, (gpointer)new, NULL);
	  int i;
	  double d;
	  SaaAile * ai;
	  SaaArete * ar;
	  SaaSommet n, * stmp;

	  fprintf(SaaLogFile, "> CHARGEMENT DU FICHIER %s\n", filename);
	  if (!result) {
		  fprintf(SaaErrorFile, "- Erreur a l'ouverture du fichier %s\n",  filename);
		  saa_solide_detruire(new);
		  new = NULL;
	  } else {
		  fprintf(SaaLogFile, "- Parsage du fichier\n");
		  g_markup_parse_context_parse(context, xml_data, -1, NULL);
		  g_free (xml_data);

		  fprintf(SaaLogFile, "- Rectification du sens des aretes\n");
		  // Rectification des sens des aretes
		  for(i=0; i<new->nbAiles; i++) {
		    ai = new->lesAiles[i];
		    ar = ai->start;
		    do {
		      if(ar->ai1 == ai) {
		        if(ar->s1 == ar->succ1->s1 || ar->s1 == ar->succ1->s2)
		          stmp = ar->s1, ar->s1 = ar->s2, ar->s2 = stmp;
		        ar = ar->succ1;
		      }
		      else {
		        if(ar->s2 == ar->succ2->s1 || ar->s2 == ar->succ2->s2)
		          stmp = ar->s1, ar->s1 = ar->s2,ar->s2 = stmp;
		        ar = ar->succ2;
		      }
		    } while(ar != ai->start);
		  }
	  }

	  fprintf(SaaLogFile, "- Construction des normales\n");
	  // Construction des normales
	  new->lesNormales = (SaaSommet **)g_malloc(new->nbAiles*sizeof(SaaSommet *));

	  for(i=0; i<new->nbAiles; i++) {
	    saa_aile_vecteur_normal(new->lesAiles[i], &n);
	    d = saa_sommet_distance(&n);
	    new->lesNormales[i] = saa_sommet_creer(n.x/d, n.y/d, n.z/d);
	  }
	  new->p = NULL;

	  g_markup_parse_context_free (context);
	  fprintf(SaaLogFile, "< CHARGEMENT TERMINE\n");

	  return new;
}

// Creer un solide grace a kaleido
SaaSolide * saa_solide_creer_kaleido(const char * symbol) {
  int i, j;
  int nums1, nums2, numai, numar, numar1, numar2;
  int trouvar;
  double d;
  SaaArete * ar1, * ar2;
  Polyhedron * p;
  SaaSommet n;
  SaaSolide * new;

  if(!(p = kaleido(g_strdup(symbol), 1, 1, 0, 0))) {
	 return NULL;
  }
  // Le polyedre doit etre oriente pour pouvoir etre un solide
  if(p->onesided) {
    polyfree(p); // destruction du polyhedre de kaleido
    return NULL;
  }

  new = saa_solide_creer(p->name);
  new->p = p;

  // on recopie les sommets et les normales
  new->lesSommets = (SaaSommet **)g_malloc(p->V*sizeof(SaaSommet *));

  for(i=0; i<p->V; i++)
    new->lesSommets[i] = saa_sommet_creer(p->v[i].x, p->v[i].y, p->v[i].z);
  new->nbSommets = p->V;
  // on alloue la place pour les aretes
  new->lesAretes = (SaaArete **)g_malloc(p->E*sizeof(SaaArete *));

  for(i=0; i<p->E; i++) {
    new->lesAretes[i] = (SaaArete *)g_malloc(sizeof(SaaArete));

  }
  // on recopie les normales aux faces
  // on alloue la place pour les ailes
  new->lesAiles = (SaaAile **)g_malloc(p->F*sizeof(SaaAile *));

  new->lesNormales = (SaaSommet **)g_malloc(p->F*sizeof(SaaSommet *));

  for(i=0; i<p->F; i++) {
    n.x = p->f[i].x, n.y = p->f[i].y, n.z = p->f[i].z;
    d = saa_sommet_distance(&n);
    new->lesNormales[i] = saa_sommet_creer(n.x/d, n.y/d, n.z/d);
    new->lesAiles[i] = (SaaAile *)g_malloc(sizeof(SaaAile));

  }
  new->nbAiles = p->F;

  // pour chaque face on ajoute les aretes
  for(numai=0; numai<p->F; numai++)     // F = nbAiles
    for(nums1=0; nums1<p->V; nums1++)   // V = nbSommets
      for(j=0; j<p->M; j++)             // M = nbAdjacents
        if(p->incid[j][nums1] == numai) {
          nums2  = p->adj[j][nums1];
          // on cherche une arete ayant les memes extremites ?
          for(numar=0, trouvar=0; numar<new->nbAretes && !trouvar; numar++)
            if(new->lesAretes[numar]->s2 == new->lesSommets[nums1] &&
               new->lesAretes[numar]->s1 == new->lesSommets[nums2]) {
              trouvar = 1;
              new->lesAretes[numar]->ai2  = new->lesAiles[numai];
              new->lesAiles[numai]->start = new->lesAretes[numar];
              numar--;
            }
          // sinon on prend la premiere dispo
          if(!trouvar) {
            numar = new->nbAretes;
            new->lesAretes[numar]->s1 = new->lesSommets[nums1];
            new->lesAretes[numar]->s2 = new->lesSommets[nums2];
            new->lesAretes[numar]->ai1 = new->lesAiles[numai];
            new->lesAiles[numai]->start = new->lesAretes[numar];
            new->nbAretes++;
          }
        }

  // on reconstruit les successions d'aretes
  // pour chaque arete on cherche ses aretes successeurs
  for(numar1=0; numar1<new->nbAretes; numar1++) {
    ar1 = new->lesAretes[numar1];
    for(numar2=0; numar2<new->nbAretes; numar2++) {
      ar2 = new->lesAretes[numar2];
      // successeur pour l'aile 1
      if(ar1->ai1 == ar2->ai1 && ar1->s2 == ar2->s1) {
        ar1->succ1 = ar2;
        //printf("ar%d - ar%d : ai%d\n", 1+numar1, 1+numar2, 1+idxAile(ar1->ai1, new));
       }
      else if(ar1->ai1 == ar2->ai2 && ar1->s2 == ar2->s2) {
        ar1->succ1 = ar2;
        //printf("ar%d - ar%d : ai%d\n", 1+numar1, 1+numar2, 1+idxAile(ar1->ai1, new));
      }
      // successeur pour l'aile 2
      if(ar1->ai2 == ar2->ai2 && ar1->s1 == ar2->s2) {
        ar1->succ2 = ar2;
        //printf("ar%d - ar%d : ai%d\n", 1+numar1, 1+numar2, 1+idxAile(ar1->ai2, new));
      }
      else if(ar1->ai2 == ar2->ai1 && ar1->s1 == ar2->s1) {
        ar1->succ2 = ar2;
      }
    }
  }
  return new;
}

// Cree le dual d'un solide de kaleido
SaaSolide * saa_solide_creer_kaleido_dual(const char * symbol) {
  int i;
  int numar1, numar2;
  double d;
  SaaArete * ar1, * ar2;
  Polyhedron * p;
  SaaSommet n;
  SaaSolide * new, * sol;

  if(!(p = kaleido(g_strdup(symbol), 1, 1, 0, 0))) {
	  return NULL;
  }
  // Le polyedre doit etre oriente pour pouvoir etre un solide
  if(!(sol = saa_solide_creer_kaleido(symbol))) {
    polyfree(p); // destruction du polyhedre de kaleido
    return NULL;
  }

  new = saa_solide_creer(p->dual_name);
  new->p = p;

  // on recopie les sommets
  new->lesSommets = (SaaSommet **)g_malloc(p->F*sizeof(SaaSommet *));

  for(i=0; i<p->F; i++)
    new->lesSommets[i] = saa_sommet_creer(p->f[i].x, p->f[i].y, p->f[i].z);
  new->nbSommets = p->F;

  // on alloue la place pour les aretes
  new->lesAretes = (SaaArete **)g_malloc(sol->nbAretes*sizeof(SaaArete *));

  for(i=0; i<sol->nbAretes; i++) {
    new->lesAretes[i] = (SaaArete *)g_malloc(sizeof(SaaArete));

  }
  new->nbAretes = sol->nbAretes;
  // on recopie les normales aux faces
  // on alloue la place pour les ailes
  new->lesAiles = (SaaAile **)g_malloc(sol->nbSommets*sizeof(SaaAile *));

  new->lesNormales = (SaaSommet **)g_malloc(sol->nbSommets*sizeof(SaaSommet *));

  for(i=0; i<sol->nbSommets; i++) {
    n.x = sol->lesSommets[i]->x;
    n.y = sol->lesSommets[i]->y;
    n.z = sol->lesSommets[i]->z;
    d = saa_sommet_distance(&n);
    new->lesNormales[i] = saa_sommet_creer(n.x/d, n.y/d, n.z/d);
    new->lesAiles[i] = (SaaAile *)g_malloc(sizeof(SaaAile));

  }
  new->nbAiles = sol->nbSommets;

  // Creation des aretes
  for(i=0; i<new->nbAretes; i++) {
    new->lesAretes[i]->s1  = new->lesSommets[saa_solide_index_aile(sol, sol->lesAretes[i]->ai2)];
    new->lesAretes[i]->s2  = new->lesSommets[saa_solide_index_aile(sol, sol->lesAretes[i]->ai1)];
    new->lesAretes[i]->ai1 = new->lesAiles[saa_solide_index_sommet(sol, sol->lesAretes[i]->s1)];
    new->lesAretes[i]->ai2 = new->lesAiles[saa_solide_index_sommet(sol, sol->lesAretes[i]->s2)];
    new->lesAretes[i]->ai1->start = new->lesAretes[i];
  }

  // On reconstruit les successions d'aretes
  // pour chaque arete on cherche ses aretes successeurs
  for(numar1=0; numar1<new->nbAretes; numar1++) {
    ar1 = new->lesAretes[numar1];
    for(numar2=0; numar2<new->nbAretes; numar2++) {
      ar2 = new->lesAretes[numar2];
      // successeur pour l'aile 1
      if(ar1->ai1 == ar2->ai1 && ar1->s2 == ar2->s1) {
        ar1->succ1 = ar2;
        //printf("ar%d - ar%d : ai%d\n", 1+numar1, 1+numar2, 1+idxAile(ar1->ai1, new));
       }
      else if(ar1->ai1 == ar2->ai2 && ar1->s2 == ar2->s2) {
        ar1->succ1 = ar2;
        //printf("ar%d - ar%d : ai%d\n", 1+numar1, 1+numar2, 1+idxAile(ar1->ai1, new));
      }
      // successeur pour l'aile 2
      if(ar1->ai2 == ar2->ai2 && ar1->s1 == ar2->s2) {
        ar1->succ2 = ar2;
        //printf("ar%d - ar%d : ai%d\n", 1+numar1, 1+numar2, 1+idxAile(ar1->ai2, new));
      }
      else if(ar1->ai2 == ar2->ai1 && ar1->s1 == ar2->s1) {
        ar1->succ2 = ar2;
      }
    }
  }
  return new;
}

// Cree le compose de 2 solides
SaaSolide * saa_solide_creer_compose(const SaaSolide * sol1, const SaaSolide * sol2) {
  SaaSolide * sol = NULL;
  char * nom = (char *)g_malloc(strlen(sol1->nom) + strlen(sol2->nom) + 1);


  strcpy(nom, sol1->nom);
  strcat(nom, sol2->nom);

  sol  = saa_solide_creer(nom);

  // on recopie les ailes et les normales
  sol->lesAiles    = (SaaAile   **)g_malloc((sol1->nbAiles+sol2->nbAiles)*sizeof(SaaAile *));

  sol->lesNormales = (SaaSommet **)g_malloc((sol1->nbAiles+sol2->nbAiles)*sizeof(SaaSommet *));

  memcpy(sol->lesAiles,    sol1->lesAiles,    sol1->nbAiles*sizeof(SaaAile *));
  memcpy(sol->lesAiles+sol1->nbAiles,    sol2->lesAiles,    sol2->nbAiles*sizeof(SaaAile *));
  memcpy(sol->lesNormales, sol1->lesNormales, sol1->nbAiles*sizeof(SaaSommet *));
  memcpy(sol->lesNormales+sol1->nbAretes, sol2->lesNormales, sol2->nbAiles*sizeof(SaaSommet *));
  sol->nbAiles = sol1->nbAiles + sol2->nbAiles;

  // on recopie les aretes
  sol->lesAretes = (SaaArete **)g_malloc((sol1->nbAretes+sol2->nbAretes)*sizeof(SaaArete *));

  memcpy(sol->lesAretes, sol1->lesAretes, sol1->nbAretes*sizeof(SaaArete *));
  memcpy(sol->lesAretes+sol1->nbAretes, sol2->lesAretes, sol2->nbAretes*sizeof(SaaArete *));
  sol->nbAretes = sol1->nbAretes + sol2->nbAretes;

  // on recopie les sommets
  sol->lesSommets = (SaaSommet **)g_malloc((sol1->nbSommets+sol2->nbSommets)*sizeof(SaaSommet *));

  memcpy(sol->lesSommets, sol1->lesSommets, sol1->nbSommets*sizeof(SaaSommet *));
  memcpy(sol->lesSommets+sol1->nbSommets, sol2->lesSommets, sol2->nbSommets*sizeof(SaaSommet *));
  sol->nbSommets = sol1->nbSommets+sol2->nbSommets;

  saa_solide_nettoyer_sommets(sol);

  return sol;
}

// Suppression des sommets utilses par aucune arete
void saa_solide_nettoyer_sommets(SaaSolide * sol) {
  int i, nums, numar, boolutil, nbsuppr;
  SaaSommet * s, * bads;
  SaaArete * ar;
  SaaSommet ** tabsupprs  = (SaaSommet **)g_malloc(sol->nbSommets*sizeof(SaaSommet *));

  // Recuperation de l'adresse des sommets inutiles
  nbsuppr = 0;
  for(nums=0; nums<sol->nbSommets; nums++) {
    s = sol->lesSommets[nums];
    boolutil = 0;
    for(numar=0; numar<sol->nbAretes && !boolutil; numar++) {
      ar = sol->lesAretes[numar];
      if(ar->s1 == s || ar->s2 == s)
        boolutil = 1;
    }
    if(!boolutil)
      tabsupprs[nbsuppr++] = s;
  }
  // Suppression effective des sommets
  for(i=0; i<nbsuppr; i++)
    saa_solide_supprimer_sommet(sol, tabsupprs[i]);

  // Recuperation des sommets en double
  nbsuppr = 0;
  for(nums=0; nums<sol->nbSommets; nums++) {
    bads = sol->lesSommets[nums];
    s = saa_solide_ajouter_sommet(sol, bads);
    if(bads != s) {
      tabsupprs[nbsuppr++] = bads;
      for(numar=0; numar<sol->nbAretes; numar++) {
        ar = sol->lesAretes[numar];
        if(ar->s1 == bads) ar->s1 = s;
        if(ar->s2 == bads) ar->s2 = s;
      }
    }
  }
  // Suppression effective des sommets
  for(i=0; i<nbsuppr; i++)
    saa_solide_supprimer_sommet(sol, tabsupprs[i]);
  g_free(tabsupprs);
}

// Supprime les aretes utilisees par aucune aile
void saa_solide_nettoyer_aretes(SaaSolide * sol) {
  int i, numai, numar, boolutil, nbsuppr;
  SaaArete * ar1, * ar2;
  SaaAile * ai;
  SaaArete  ** tabsupprar = (SaaArete  **)g_malloc(sol->nbAretes*sizeof(SaaArete *));


  // Recuperation de l'adresse des aretes a supprimer
  nbsuppr = 0;
  for(numar=0; numar<sol->nbAretes; numar++) {
    ar1 = sol->lesAretes[numar];
    boolutil = 0;
    for(numai=0; numai<sol->nbAiles && !boolutil; numai++) {
      ai = sol->lesAiles[numai];
      ar2 = ai->start;
      do {
        if(ar2 == ar1)
          boolutil = 1;
        ar2 = (ar2->ai1 == ai) ? ar2->succ1 : ar2->succ2;
      } while(ar2 != ai->start && !boolutil);
    }
    if(!boolutil) tabsupprar[nbsuppr++] = ar1;
  }
  // Suppression effective des aretes
  for(i=0; i<nbsuppr; i++)
    saa_solide_supprimer_arete(sol, tabsupprar[i]);
  g_free(tabsupprar);
}

// Suppression des ailes isolées
void saa_solide_nettoyer_ailes_isolees(SaaSolide * sol) {
	int i, numai, boolutil, nbsuppr;
	SaaArete * ar;
	SaaAile * ai;
	SaaAile ** tabsupprai = (SaaAile **)g_malloc(sol->nbAiles*sizeof(SaaAile *));


	// Recuperation des numero d'ailes
	nbsuppr = 0;
	for(numai=0; numai<sol->nbAiles; numai++) {
	boolutil = 1;
	ai = sol->lesAiles[numai];
	ar = ai->start;
	do {
		boolutil = (ar->ai1 && ar->ai2);
		ar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
	} while(ar != ai->start && boolutil);

	if(!boolutil)
	  tabsupprai[nbsuppr++] = ai;
	}
	// Suppression effective des ailes
	for(i=0; i<nbsuppr; i++)
	saa_solide_supprimer_aile(sol, tabsupprai[i]);

	if(nbsuppr)
	saa_solide_nettoyer_aretes(sol);

	g_free(tabsupprai);
}

// Suppression des ailes en double
void saa_solide_nettoyer_ailes_doubles(SaaSolide * sol) {
  int i, numai1, numai2, boolutil, nbsuppr;
  SaaAile * ai1, * ai2;
  SaaAile ** tabsupprai = (SaaAile **)g_malloc(sol->nbAiles*sizeof(SaaAile *));


  // Recuperation des numero d'ailes
  nbsuppr = 0;
  for(numai1=0; numai1<sol->nbAiles; numai1++) {
    boolutil = 1;
    ai1 = sol->lesAiles[numai1];

    for(numai2=numai1+1; numai2<sol->nbAiles && boolutil; numai2++) {
    	ai2 = sol->lesAiles[numai2];
    	boolutil = saa_aile_comparer(ai1, sol->lesAiles[numai2]);
    }
    //if(!boolutil) printf("ai%d et ai%d memeustra\n", 1+numai1, 1+numai2);
    if(!boolutil)
      tabsupprai[nbsuppr++] = sol->lesAiles[numai1];
  }
  // Suppression effective des ailes
  for(i=0; i<nbsuppr; i++)
    saa_solide_supprimer_aile(sol, tabsupprai[i]);

  if(nbsuppr)
    saa_solide_nettoyer_aretes(sol);
  g_free(tabsupprai);
}

void saa_solide_ecrire_sommet_txt(const SaaSolide * sol, FILE * f, const SaaSommet * s) {
  fprintf(f, "\ts%-3d : ( %*.*f , %*.*f , %*.*f )\n", 1+saa_solide_index_sommet(sol, s),
          2+PRECISION, PRECISION, s->x,
          2+PRECISION, PRECISION, s->y,
          2+PRECISION, PRECISION, s->z);
}

void saa_solide_ecrire_sommet_xml(const SaaSolide * sol, FILE * f, const SaaSommet * s) {
  fprintf(f, "\t\t\t<vertex id=\"s%-d\" x=\"%*.*f\" y=\"%*.*f\" z=\"%*.*f\" />\n", 1+saa_solide_index_sommet(sol, s),
          2+PRECISION, PRECISION, s->x,
          2+PRECISION, PRECISION, s->y,
          2+PRECISION, PRECISION, s->z);
}

void saa_solide_ecrire_arete_txt(const SaaSolide * sol, FILE * f, const SaaArete * ar) {
  fprintf(f, "\tar%-3d : s%-3d -- s%-3d\n", 1+saa_solide_index_arete(sol, ar),
          1+saa_solide_index_sommet(sol, ar->s1),
          1+saa_solide_index_sommet(sol, ar->s2));
}

void saa_solide_ecrire_arete_xml(const SaaSolide * sol, FILE * f, const SaaArete * ar) {
  fprintf(f, "\t\t\t<edge id=\"ar%-d\" v1=\"s%-d\" v2=\"s%-d\" />\n", 1+saa_solide_index_arete(sol, ar),
          1+saa_solide_index_sommet(sol, ar->s1),
          1+saa_solide_index_sommet(sol, ar->s2));
}

void saa_solide_ecrire_aile_txt(const SaaSolide * sol, FILE * f, const SaaAile * ai) {
  SaaArete * ar;
  fprintf(f, "\tai%-3d :", 1+saa_solide_index_aile(sol, ai));
  ar = ai->start;
  do {
    fprintf(f, " ar%-3d", 1+saa_solide_index_arete(sol, ar));
    ar = ai == ar->ai1 ? ar->succ1 : ar->succ2;
  } while(ar != ai->start);
  fprintf(f, "\n");
}

void saa_solide_ecrire_aile_xml(const SaaSolide * sol, FILE * f, const SaaAile * ai) {
  SaaArete * ar;
  fprintf(f, "\t\t\t<wing id=\"ai%-d\" edge-list=\"", 1+saa_solide_index_aile(sol, ai));
  ar = ai->start;
  do {
    fprintf(f, "ar%-d ", 1+saa_solide_index_arete(sol, ar));
    ar = ai == ar->ai1 ? ar->succ1 : ar->succ2;
  } while(ar != ai->start);
  fprintf(f, "\" />\n");
}

// Numero d'un sommet dans un solide
int saa_solide_index_sommet(const SaaSolide * sol, const SaaSommet * s) {
  int i, index = -1;
  for(i=0; i<sol->nbSommets && index<0; i++)
    if(sol->lesSommets[i]==s) index = i;
  return index;
}

// Numero d'une arete dans un solide
int saa_solide_index_arete(const SaaSolide * sol, const SaaArete * ar) {
  int i, index = -1;
  for(i=0; i<sol->nbAretes && index<0; i++)
    if(sol->lesAretes[i]==ar) index = i;
  return index;
}

// Numero d'une aile dans un solide
int saa_solide_index_aile(const SaaSolide * sol, const SaaAile * ai) {
  int i, index = -1;
  for(i=0; i<sol->nbAiles && index<0; i++)
    if(sol->lesAiles[i]==ai) index = i;
  return index;
}

void saa_xml_start(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names,
                          const gchar **attribute_values, gpointer user_data, GError **error) {
	SaaSolide * solide = (SaaSolide *)user_data;
	SaaSommet * s;
	SaaArete * ar, * par;
	SaaAile * ai;
	double x, y, z;
	int i, i1, i2;
	gchar ** arlist;

	if(!g_strcmp0(element_name, "solid")) {
		g_free(solide->nom);
		solide->nom = g_strdup(attribute_values[1]);
//printf("solid = %s\n", attribute_values[1]);
	} else if(!g_strcmp0(element_name, "vertex")) {
		x = g_strtod(attribute_values[1], NULL);
		y = g_strtod(attribute_values[2], NULL);
		z = g_strtod(attribute_values[3], NULL);
//printf("vertex = %f %f %f\n", x, y, z);
		s = saa_sommet_creer(x, y, z);
	    solide->lesSommets = (SaaSommet **)g_realloc(solide->lesSommets, (solide->nbSommets+1)*sizeof(SaaSommet *));
	    solide->lesSommets[solide->nbSommets++] = s;
	} else if(!g_strcmp0(element_name, "edge")) {
		ar = (SaaArete *)g_malloc(sizeof(SaaArete));
	    solide->lesAretes = (SaaArete **)g_realloc(solide->lesAretes, (solide->nbAretes+1)*sizeof(SaaArete *));
	    solide->lesAretes[solide->nbAretes++] = ar;
		ar->ai1 = ar->ai2 = NULL;
		ar->succ1 = ar->succ2 = NULL;
		i1 = g_strtod(attribute_values[1]+1, NULL) - 1;
		i2 = g_strtod(attribute_values[2]+1, NULL) - 1;
//printf("edge = %d %d\n", i1, i2);
		ar->s1 = solide->lesSommets[i1];
		ar->s2 = solide->lesSommets[i2];
	} else if(!g_strcmp0(element_name, "wing")) {
		ai = (SaaAile *)g_malloc(sizeof(SaaAile));
	    solide->lesAiles = (SaaAile **)g_realloc(solide->lesAiles, (solide->nbAiles+1)*sizeof(SaaAile *));
	    solide->lesAiles[solide->nbAiles++] = ai;
//printf("wing = %s\n", attribute_values[1]);
		arlist = g_strsplit(attribute_values[1], " ", -1);
		par = NULL;
		for(i=0; arlist[i] && g_strcmp0(arlist[i], ""); i++) {
			i1 = g_strtod(arlist[i]+2, NULL) - 1;
			ar = solide->lesAretes[i1];
			if(!ar->ai1) ar->ai1 = ai;
			else ar->ai2 = ai;
			if (par) { if(par->ai1 == ai) par->succ1 = ar; else par->succ2 = ar; }
			else { ai->start = ar; }
			par = ar;
		}
		if(par->ai1 == ai) par->succ1 = ai->start; else par->succ2 = ai->start;

	    g_strfreev(arlist);
	}
}

void saa_xml_end(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error) {
	//SaaSolide * solid = (SaaSolide *)user_data;
}


void saa_xml_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error) {
	//SaaSolide * solid = (SaaSolide *)user_data;
}

void saa_xml_passthrough(GMarkupParseContext *context, const gchar *passthrough_text, gsize text_len, gpointer user_data, GError **error) {
	//SaaSolide * solid = (SaaSolide *)user_data;
	fprintf (SaaLogFile, "PASS '%.*s'\n", (int)text_len, passthrough_text);
}

void saa_xml_error(GMarkupParseContext *context, GError *error, gpointer user_data) {
	//SaaSolide * solid = (SaaSolide *)user_data;
	fprintf (SaaErrorFile, "ERROR %s\n", error->message);
}

int saa_aile_intersection(const SaaAile * ai1, const SaaAile * ai2) {
	int intersect, inter1, inter2, exter1, exter2, cs1, cs2, nbinter;
	SaaArete * ar;
	SaaSommet si;
	SaaListe * sommets = saa_liste_creer();

	inter1 = exter1 = 0;
	// On teste pour chaque arete de ai2
	ar = ai2->start;
	do {
		// si elle traverse ai1
		intersect = saa_aile_intersection_segment(ai1, ar->s1, ar->s2, &si);
		if(intersect) saa_liste_ajouter_unique(sommets, saa_sommet_copier(&si), (SaaCompareFunc)saa_sommet_comparer);
		cs1 = saa_aile_cote_sommet(ai1, ar->s1);
		cs2 = saa_aile_cote_sommet(ai1, ar->s2);
		inter1 |= intersect && (cs1 == 1 || cs1 == -1 || cs2 == 1 || cs2 == -1);
		exter1 |= intersect && (cs1 == 2 || cs2 == 2);
		ar = (ar->ai1 == ai2) ? ar->succ1 : ar->succ2;
	} while(ar != ai2->start);

	inter2 = exter2 = 0;
	// On teste pour chaque arete de ai1
	ar = ai1->start;
	if (inter1 && exter1) do {
	  // si elle traverse ai2
		intersect = saa_aile_intersection_segment(ai2, ar->s1, ar->s2, &si);
		if(intersect) saa_liste_ajouter_unique(sommets, saa_sommet_copier(&si), (SaaCompareFunc)saa_sommet_comparer);
		cs1 = saa_aile_cote_sommet(ai2, ar->s1);
		cs2 = saa_aile_cote_sommet(ai2, ar->s2);
		inter2 |= intersect && (cs1 == 1 || cs1 == -1 || cs2 == 1 || cs2 == -1);
		exter2 |= intersect && (cs1 == 2 || cs2 == 2);
		ar = (ar->ai1 == ai1) ? ar->succ1 : ar->succ2;
	} while(ar != ai1->start);

	nbinter = saa_liste_taille(sommets);
	saa_liste_pour_chaque(sommets, (SaaPourChaqueFunc)g_free, NULL);
	saa_liste_detruire(sommets);

	return inter1 && exter1 && inter2 && exter2 && nbinter >= 2 ? nbinter : 0;
}
