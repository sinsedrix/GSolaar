/***************************** saa-traitement.c ******************************/
/*                                                                           */
/*   Traitement des polyèdres                                                */
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
#include "saa-traitement.h"

// Coupe une arete par un sommet
SaaArete * saa_solide_couper_arete(SaaSolide * sol, SaaArete * ar, const SaaSommet * s) {
	  int traite = 0;
	  SaaSommet * som;
	  SaaAile * ai1, * ai2;
	  SaaArete * sar, * newar;

	  // raf si le sommet appartient a l'arete
	  if(ar->s1 == som || ar->s2 == som || !saa_sommet_comparer(s, ar->s1) || !saa_sommet_comparer(s, ar->s2)) return ar;

	  newar = (SaaArete *)g_malloc(sizeof(SaaArete));

	  // on ajoute le sommet au solide (on recupere son adresse)
	  som = saa_solide_ajouter_sommet(sol, s);

	  ai1 = ar->ai1;
	  ai2 = ar->ai2;
	  newar->ai1 = ai1;
	  newar->ai2 = ai2;
	  newar->s1 = som;
	  newar->s2 = ar->s2;
	  ar->s2 = som;
	  // on colle les arete suivantes pour la face ai1
	  newar->succ1 = ar->succ1;
	  ar->succ1 = newar;
	  // on colle les arete suivantes pour la face ai2
	  newar->succ2 = ar;
	  // l'arete qui a pour succ ar doit avoir pour succ newar
	  sar = ai2->start;
	  do {
	    if(sar->ai1 == ai2)
	      if(sar->succ1 == ar)
	        sar->succ1 = newar, traite = 1;
	      else
	        sar = sar->succ1;
	    else
	      if(sar->succ2 == ar)
	        sar->succ2 = newar, traite = 1;
	      else
	        sar = sar->succ2;
	  } while(sar != ar && !traite);

	sol->lesAretes = (SaaArete **)g_realloc(sol->lesAretes, (sol->nbAretes+1)*sizeof(SaaArete *));

	sol->lesAretes[sol->nbAretes++] = newar;

	return newar;
}

static SaaSommet * plusProcheDeG(const SaaListe * soms, const SaaSommet * g) {
	  SaaSommet * res, * s = NULL, sg;
	  double      dsresg = 9999, dsg;
	  int i, n = saa_liste_taille(soms);

	  for(i=0; i<n; i++) {
		  s = (SaaSommet *)saa_liste_element(soms, i);
		  saa_sommet_difference(s, g, &sg);
		  dsg = saa_sommet_distance(&sg);
		  if(dsg < dsresg) {
			  res = s;
			  dsresg = dsg;
		  }
	  }
	  return res;
}

// Coupe une aile (non croisee) par un plan
SaaAile * saa_solide_couper_aile(SaaSolide * sol, SaaAile * ai, const SaaPlan * p) {
  int ait, nbsom, i, trouvar, inter, exter;
  int c1, c2;    // cote des sommet de l'arete par rapport au plan
  SaaAile * newai;
  SaaArete * ar, * sar, * newar;
  SaaSommet si, * sdec1, * sdec2, g, * s1, * s2;
  SaaListe * listeSommets = saa_liste_creer();
  SaaPlan pai;

  saa_aile_barycentre(ai, &g);
  saa_aile_plan(ai, &pai);

  // on recherche tous les sommets dans le plan de découpe
  inter = exter = 0;
  ar = ai->start;
  do {
    if(ar->ai1 == ai)
      s1 = ar->s1, s2 = ar->s2;
    else
      s1 = ar->s2, s2 = ar->s1;

    c1 = saa_plan_cote_sommet(p, s1);
    c2 = saa_plan_cote_sommet(p, s2);
    inter |= (c1 == -1 || c1 == 1 || c2 == -1 || c2 == -1);
    exter |= (c1 == 2 || c2 == 2);
    if(c1 == 0 && c2 == 0) {
//printf("saa_solide_couper_aile ar%d : cas0 (ne pas traiter)\n", 1+saa_solide_index_arete(sol, ar));
    } else if(c1 == 0 && c2 != 0) {
//printf("saa_solide_couper_aile ar%d : cas1\n", 1+saa_solide_index_arete(sol, ar));
    	saa_liste_ajouter_unique(listeSommets, s1, (SaaCompareFunc)saa_sommet_comparer);
    } else if(c1 != 0 && c2 == 0) {
//printf("saa_solide_couper_aile ar%d : cas2\n", 1+saa_solide_index_arete(sol, ar));
		saa_liste_ajouter_unique(listeSommets, s2, (SaaCompareFunc)saa_sommet_comparer);
    } else if(saa_plan_intersection_segment(p, s1, s2, &si)) {
//printf("saa_solide_couper_aile ar%d --> cas3\n", 1+saa_solide_index_arete(sol, ar));
    	saa_solide_couper_arete(sol, ar, &si);
		saa_liste_ajouter_unique(listeSommets, saa_solide_ajouter_sommet(sol, &si), (SaaCompareFunc)saa_sommet_comparer);
    }
    ar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
  } while(ar != ai->start);

  nbsom = saa_liste_taille(listeSommets);
  if(!(inter && exter) || nbsom < 2) {
	  saa_liste_detruire(listeSommets);
	  if (inter && exter) fprintf(SaaErrorFile, " - annulation du decoupage\n");
	  return NULL;
  }


  //printf("sommets trouvés :");
  //for(i=0; i<saa_liste_taille(listeSommets); i++)
  //  printf(" s%d", 1+saa_solide_index_sommet(sol, saa_liste_element(listeSommets, i)));
  //printf("\n");

  // on cherche les deux sommets de decoupe sdec1 et sdec2 (les plus proches de g)

  if(nbsom == 2) {
	  sdec1 = saa_liste_element(listeSommets, 0);
	  sdec2 = saa_liste_element(listeSommets, 1);
  } else { // nbsom > 2
	  sdec1 = plusProcheDeG(listeSommets, &g);
	  saa_liste_enlever(listeSommets, sdec1);
	  sdec2 = plusProcheDeG(listeSommets, &g);
	  //saa_liste_trier(listeSommets, (SaaCompareFunc)plusProcheDeG);
	  //sdec1 = saa_liste_element(listeSommets, 0);
	  //sdec2 = saa_liste_element(listeSommets, 1);
  }
  saa_liste_detruire(listeSommets);
//printf("sdec1 = s%d\n", 1+saa_solide_index_sommet(sol, sdec1));
//printf("sdec2 = s%d\n", 1+saa_solide_index_sommet(sol, sdec2));

  // On verifie que les sommetes de découpe n'appartiennent pas a une arete de l'aile
  trouvar = 0;
  for(i=0; i< sol->nbAretes && !trouvar; i++) {
	  ar = sol->lesAretes[i];
	  if(ar->ai1 == ai || ar->ai2 == ai)
		  trouvar = (!saa_sommet_comparer(ar->s1, sdec1) && !saa_sommet_comparer(ar->s2, sdec2)) ||
					  (!saa_sommet_comparer(ar->s2, sdec1) && !saa_sommet_comparer(ar->s1, sdec2));
  }

  if(trouvar) {
//printf("2 - annulation du decoupage\n");
	  return NULL;
  }

  // On commence la decoupe
  newai = (SaaAile *)g_malloc(sizeof(SaaAile));
  newar = (SaaArete *)g_malloc(sizeof(SaaArete));
  nbsom = 0;
  ait = 0;
  newar->ai1 = ai;
  newar->ai2 = newai;
  newai->start = newar;
  ar = ai->start;
  do {
    if(ar->ai1 == ai) {
      sar = ar->succ1;
      if(ar->s2 == sdec1) {
        if(!nbsom) {
          newar->s1 = sdec1;
          ar->succ1 = newar;
          newar->succ2 = sar;
          nbsom = 1;
        } else {
          newar->s2 = sdec1;
          ar->succ1 = newar;
          newar->succ1 = sar;
          nbsom = 2;
        }
      } else if(ar->s2 == sdec2) {
        if(!nbsom) {
          newar->s1 = sdec2;
          ar->succ1 = newar;
          newar->succ2 = sar;
          nbsom = 1;
        } else {
          newar->s2 = sdec2;
          ar->succ1 = newar;
          newar->succ1 = sar;
          nbsom = 2;
        }
      }
    } else { // ar->ai2 == ai
      sar = ar->succ2;
      if(ar->s1 == sdec1) {
        if(!nbsom) {
          newar->s1 = sdec1;
          ar->succ2 = newar;
          newar->succ2 = sar;
          nbsom = 1;
        } else {
          newar->s2 = sdec1;
          ar->succ2 = newar;
          newar->succ1 = sar;
          nbsom = 2;
        }
      } else if(ar->s1 == sdec2) {
        if(!nbsom) {
          newar->s1 = sdec2;
          ar->succ2 = newar;
          newar->succ2 = sar;
          nbsom = 1;
        } else {
          newar->s2 = sdec2;
          ar->succ2 = newar;
          newar->succ1 = sar;
          nbsom = 2;
        }
      }
    }
    if(ait) {
      if(ar->ai1 == ai)
        ar->ai1 = newai;
      else
        ar->ai2 = newai;
    }
    if(nbsom == 1) ait = 1;
    ar = sar;
  } while(ar != ai->start && nbsom != 2);

  // Ajout de la nouvelle arete au solide
  sol->lesAretes = (SaaArete **)g_realloc(sol->lesAretes, (sol->nbAretes+1)*sizeof(SaaArete *));
  sol->lesAretes[sol->nbAretes++] = newar;

  // Ajout de la nouvelle aile au solide
  sol->lesAiles = (SaaAile **)g_realloc(sol->lesAiles, (sol->nbAiles+1)*sizeof(SaaAile *));
  sol->lesAiles[sol->nbAiles++] = newai;

  return newai;
}

// Décroisement des faces croisées
int saa_solide_decroiser(SaaSolide * sol) {
	int nbai = sol->nbAiles, nbs = sol->nbSommets;
	SaaAile * ai;
	int i, j, k, nbsom, decal;
	int trouvar;
	double oldteta, tetatmp, rotmp;
	SaaSommet g;          // le barycentre de l'aile
	SaaSommet v, oldv;    // vecteurs g-s et old g-s
	SaaSommet * som;      // sommet temporaire
	SaaArete * ar, * sar; // arete courante et suivante
	// tableaux pour le tri des sommets
	SaaSommet ** soms  = (SaaSommet **)g_malloc(nbs*nbai*sizeof(SaaSommet *));
	double  * tetas = (double  *)g_malloc(nbs*nbai*sizeof(double));
	double  * ros   = (double  *)g_malloc(nbs*nbai*sizeof(double));

	if(saa_solide_segmenter(sol)) {
		fprintf(SaaErrorFile, "Erreur lors de la segmentation\n");
		return 1;
	}

	fprintf(SaaLogFile, "- decroiser : Recolte et tri des sommets par arete\n");
	for(i=0; i<nbai; i++) {
		ai = sol->lesAiles[i];
		if(!saa_aile_est_croisee(ai)) continue;

		// On commence par remplir les tableaux avec les infos (angle et ro)
		saa_aile_barycentre(ai, &g);
		ar = ai->start;
		som = (ar->ai1 == ai) ? ar->s1 : ar->s2;
		sar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
		saa_sommet_difference(&g, som, &oldv);
		nbsom = 0;
		soms[nbsom] = som;
		oldteta = tetas[nbsom] = 0;
		ros[nbsom] = saa_sommet_distance(&oldv);
		nbsom++;
		ar = sar;
		do {
		  som = (ar->ai1 == ai) ? ar->s1 : ar->s2;
		  sar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
		  saa_sommet_difference(&g, som, &v);
		  soms[nbsom]  = som;
		  tetas[nbsom] = saa_mod2pi(oldteta + saa_sommet_calculer_angle(&g, &oldv, &v));
		  ros[nbsom]   = saa_sommet_distance(&v);
		  oldv = v;
		  oldteta = tetas[nbsom];
		  nbsom++;
		  ar = sar;
		} while(ar != ai->start);

		// On trie le tableau selon les tetas (tri a bulle)
		for(j=0; j<nbsom-1; j++)
		  for(k=j+1; k<nbsom; k++) {
			if(tetas[j] > tetas[k]) {
			  som     = soms[j];  soms[j]  = soms[k];  soms[k]  = som;
			  tetatmp = tetas[j]; tetas[j] = tetas[k]; tetas[k] = tetatmp;
			  rotmp   = ros[j];   ros[j]   = ros[k];   ros[k]   = rotmp;
			}
		  }
	/*
		printf("Aile %d - trie nbsom = %d\n", i+1, nbsom);
		for(j=0; j<nbsom; j++) printf(" %5d", 1+saa_solide_index_sommet(sol, soms[j]));
		printf("\n");
		for(j=0; j<nbsom; j++) printf(" %5.1f", 180*tetas[j]/M_PI);
		printf("\n");
		for(j=0; j<nbsom; j++) printf(" %5.1f", 10*ros[j]);
		printf("\n");
	*/
		//fprintf(SaaLogFile, "- decroiser : Nettoyage des sommets inutiles\n");
		// On nettoie les tableaux (pas de doublons pour un meme angle)
		decal = 0;
		for(j=0; j+decal<nbsom; j++) {
			while(fabs(tetas[j+decal] - tetas[j+decal+1]) < EPSILON_3) {
				// on conserve le point le plus externe
				if(ros[j+decal] > ros[j+decal+1]) {
				  soms[j+decal+1] = soms[j+decal];
				  tetas[j+decal+1] = tetas[j+decal];
				  ros[j+decal+1] = ros[j+decal];
				}
				decal++;
			}
			soms[j] = soms[j+decal];
			tetas[j] = tetas[j+decal];
			ros[j] = ros[j+decal];
		}
		nbsom -= decal;
/*
    printf("trie nbsom = %d\n", nbsom);
    for(j=0; j<nbsom; j++) printf(" %5d", 1+saa_solide_index_sommet(sol, soms[j]));
    printf("\n");
    for(j=0; j<nbsom; j++) printf(" %5.1f", 180*tetas[j]/M_PI);
    printf("\n");
    for(j=0; j<nbsom; j++) printf(" %5.1f", 10*ros[j]);
    printf("\n");
*/
		//fprintf(SaaLogFile, "- decroiser : Recomposition des successions d'aretes\n");
		for(j=0; j<=nbsom; j++) {
			trouvar = 0;
			for(k=0; k<sol->nbAretes && !trouvar; k++) {
				sar = sol->lesAretes[k];
				trouvar = (sar->ai1 == ai && sar->s1 == soms[j%nbsom] && sar->s2 == soms[(j+1)%nbsom]) ||
							(sar->ai2 == ai && sar->s2 == soms[j%nbsom] && sar->s1 == soms[(j+1)%nbsom]);
			}

			if(!trouvar) {
				fprintf(SaaErrorFile, "- decroiser : probleme %d/%d : arete introuvable\n", j, nbsom);
				return 1;
			}

			if(j) {
				if(ar->ai1 == ai)
				  ar->succ1 = sar;
				else
				  ar->succ2 = sar;
			}
			ar = sar;
		}
	}
	fprintf(SaaLogFile, "- decroiser : Nettoyage des aretes\n");
	saa_solide_nettoyer_aretes(sol);

	fprintf(SaaLogFile, "- decroiser : Liberation d'espace memoire\n");
	g_free(soms);
	g_free(tetas);
	g_free(ros);

	return 0;
}

// Decoupe des ailes du solide
int saa_solide_decouper(SaaSolide * s) {
  int nbai = s->nbAiles; //, nbsom = s->nbSommets, nbar = s->nbAretes;
  int i, j, k;
  int numai1, numai2;       // Numero des ailes a parcourir et
  SaaAile * ai1, * ai2;     //   les ailes correspondantes
  SaaPlan pai;

  // Donnees des decoupages
  int nbdecoup;
  //   Listes des plans de découpe pour chaque aile et leur dimension
  SaaAile *** tabdecoupai = (SaaAile ***)g_malloc(nbai*sizeof(SaaAile **));
  int *       dimdecoupai = (int *)g_malloc(nbai*sizeof(int));
  //   Listes des alias pour chaque aile et leur dimension
  SaaAile *** tabaliasai = (SaaAile ***)g_malloc(nbai*sizeof(SaaAile **));
  int *       dimaliasai = (int *)g_malloc(nbai*sizeof(int));

  fprintf(SaaLogFile, "> DECOUPAGE\n");

  for(i=0; i<nbai; i++) {
	tabdecoupai[i] = (SaaAile **)g_malloc(nbai*sizeof(SaaAile *));
	dimdecoupai[i] = 0;

    tabaliasai[i] = (SaaAile **)g_malloc(nbai*sizeof(SaaAile *));
    tabaliasai[i][0] = s->lesAiles[i];
    dimaliasai[i] = 1;
  }

  // Preparation au decoupage
  fprintf(SaaLogFile, "- Recuperation des ailes en intersection\n");
  nbdecoup = 0;
  for(numai1=0; numai1<nbai; numai1++) {
    ai1 = s->lesAiles[numai1];
    for(numai2=numai1+1; numai2<nbai; numai2++) {
      ai2 = s->lesAiles[numai2];
      if(saa_aile_intersection(ai1, ai2)) {
        tabdecoupai[numai1][dimdecoupai[numai1]++] = ai2;
        tabdecoupai[numai2][dimdecoupai[numai2]++] = ai1;
        nbdecoup++;
      }
    }
  }
/*
  // Afficher la tabdecoupai
  for(i=0; i<s->nbAiles; i++) {
    printf("> decoup de ai%d :", i+1);
    for(j=0; j<dimdecoupai[i]; j++) {
      printf(" ai%d", 1+saa_solide_index_aile(s, tabdecoupai[i][j]));
    }
    printf("\n");
  }
*/
	// Segmenter et decroiser le solide
	fprintf(SaaLogFile, "- Decroisement des ailes croisees\n");
	if(saa_solide_decroiser(s)) {
		fprintf(SaaErrorFile, "Erreur lors du decroisement\n");
		return 1;
	}

	// Appliquer des decoupages
	fprintf(SaaLogFile, "- Decoupage effectif des ailes\n");
	for(i=0; i<nbai; i++) { //nbai
	//printf(">>> ai%d :\n", i+1);
		for(j=0; j<dimdecoupai[i]; j++) {
			saa_aile_plan(tabdecoupai[i][j], &pai);
//printf(">> par ai%d\n", 1+saa_solide_index_aile(s, tabdecoupai[i][j]));
			for(k=0; k<dimaliasai[i]; k++) {
//printf("> alias[%d][%d] : ai%d\n", i, k, 1+saa_solide_index_aile(s, tabaliasai[i][k]));
				ai1 = saa_solide_couper_aile(s, tabaliasai[i][k], &pai);
				if(ai1)
					tabaliasai[i][dimaliasai[i]++] = ai1;
//printf(" nouvel alias pour %d : ai%d\n", 1+i, 1+saa_solide_index_aile(s, ai1));

			}
		}
	}

  // Afficher la tabaliasai
/*
  for(i=0; i<nbai; i++) {
    printf("> alias de ai%d :", i+1);
    for(k=0; k<dimaliasai[i]; k++)
      printf(" ai%d", 1+saa_solide_index_aile(s, tabaliasai[i][k]));
    printf("\n");
  }
*/

  // On met a jour les normales aux faces
  fprintf(SaaLogFile, "- Mise a jour des normales au faces\n");
  s->lesNormales = (SaaSommet **)g_realloc(s->lesNormales, s->nbAiles*sizeof(SaaSommet *));

  for(i=0; i<nbai; i++)
    for(j=0; j<dimaliasai[i]; j++)
      s->lesNormales[saa_solide_index_aile(s, tabaliasai[i][j])] = saa_sommet_copier(s->lesNormales[saa_solide_index_aile(s, tabaliasai[i][0])]);

  fprintf(SaaLogFile, "- Liberation d'espace memoire\n");
  g_free(dimdecoupai);
  g_free(dimaliasai);
  for(i=0; i<nbai; i++) { // TODO FIXME
//fprintf(SaaLogFile, "3a - Liberation d'espace memoire %d : %p\n", i, tabdecoupai[i]);
	  //g_free(tabdecoupai[i]);
//fprintf(SaaLogFile, "3b - Liberation d'espace memoire %d : %p\n", i, tabaliasai[i]);
	  //g_free(tabaliasai[i]);
  }
  g_free(tabdecoupai);
  g_free(tabaliasai);
  fprintf(SaaLogFile, "< DECOUPAGE TERMINE - %d decoupages effectues\n", nbdecoup);
  return 0;
}

// Recupere le contour du solide apres decoupe par elimination des ailes cachees
int saa_solide_evider(SaaSolide * sol) {
	int i, j, nbar = sol->nbAretes, nbai = sol->nbAiles, nbs = sol->nbSommets;
	int numai1, numai2;          // numero de l'aile courante
	SaaAile * ai1, * ai2;        // aile courante
	SaaSommet g, gai1;  		 // barycentres
	SaaSommet si;                // sommet d'intersection
	int numar1, numar2;          // numero de l'arete courante
	SaaArete * ar1, * ar2;       // arete courante
	int boolutil;
	int nums;
	SaaSommet * s, v, srho;
	double d, rho, rhomax = 0;

	// Donnees de suppression d'ailes et de fusion d'aretes
	int nbsuppr, nbfusio, nbnet;
	int ** tabfusio = (int **)g_malloc(sol->nbAretes*sizeof(int *));
	SaaAile ** tabsupprai = (SaaAile **)g_malloc(sol->nbAiles*sizeof(SaaAile *));
	SaaArete ** tabfusioar = (SaaArete **)g_malloc(sol->nbAiles*sol->nbAiles*sizeof(SaaArete *));

	fprintf(SaaLogFile, "> EVIDAGE\n");

	fprintf(SaaLogFile, "- Nettoyage des ailes en double ... ");
	nbnet = sol->nbAiles;
	saa_solide_nettoyer_ailes_doubles(sol);
	nbnet -= sol->nbAiles;
	fprintf(SaaLogFile, "%d\n", nbnet);

	for(i=0; i<sol->nbAretes; i++)
		tabfusio[i] = (int *)g_malloc0(sol->nbAretes*sizeof(int));

	saa_solide_barycentre(sol, &g);

	fprintf(SaaLogFile, "- Calcul de la sphere englobante ... ");
	for(nums=0; nums<nbs; nums++) {
		s = sol->lesSommets[nums];
		saa_sommet_difference(&g, s, &v);
		d = saa_sommet_distance(&v);
		rhomax = d > rhomax ? d : rhomax;
	}
	rho = rhomax * 1.1;
	fprintf(SaaLogFile, "%.2f\n", rho);

	fprintf(SaaLogFile, "- Recuperation des ailes a supprimer ... ");
	nbsuppr = 0;
	for(numai1=0; numai1<nbai; numai1++) {
		ai1 = sol->lesAiles[numai1];
		boolutil = 1;
		saa_aile_barycentre(ai1, &gai1);
		d = saa_sommet_distance(&gai1);
		srho.x = gai1.x/d * rho;
		srho.y = gai1.y/d * rho;
		srho.z = gai1.z/d * rho;

		for(numai2=0; numai2<nbai && boolutil; numai2++) {
		  if(numai1 == numai2) continue;
		  ai2 = sol->lesAiles[numai2];

		  if(saa_aile_intersection_segment(ai2, &gai1, &srho, &si)) {
//printf("a - Rayon entre G(ai%d) et P(G)/Sphere eliminé par ai%d\n", 1+numai1, 1+numai2);
			  boolutil = 0;
			  tabsupprai[nbsuppr++] = ai1;
		  }
		}
	}
	fprintf(SaaLogFile, "%d\n", nbsuppr);

	fprintf(SaaLogFile, "- Suppression effective des ailes ... ");
	nbnet = sol->nbAiles;
	for(i=0; i<nbsuppr; i++)
	saa_solide_supprimer_aile(sol, tabsupprai[i]);
	nbnet -= sol->nbAiles;
	fprintf(SaaLogFile, "%d\n", nbnet);

	fprintf(SaaLogFile, "- Nettoyage des aretes inutiles ... ");
	nbnet = sol->nbAretes;
	saa_solide_nettoyer_aretes(sol);
	nbnet -= sol->nbAretes;
	fprintf(SaaLogFile, "%d\n", nbnet);

	fprintf(SaaLogFile, "- Recuperation des demi-aretes a fusionner ... ");
	nbfusio = 0;
	for(numar1=0; numar1<sol->nbAretes; numar1++) {
		ar1 = sol->lesAretes[numar1];
		for(numar2=0; numar2<sol->nbAretes; numar2++) {
			if(numar1 == numar2) continue;
			ar2 = sol->lesAretes[numar2];
			if(((ar1->s1 == ar2->s1 && ar1->s2 == ar2->s2) ||
			    (ar1->s1 == ar2->s2 && ar1->s2 == ar2->s1))) {
				tabfusio[numar1][numar2] = 1;
				nbfusio++;
			}
		}
	}
	fprintf(SaaLogFile, "%d\n", nbfusio);

	// Construction de la table des aretes a fusionner
	fprintf(SaaLogFile, "- Tri des demi-aretes a fusionner\n");
	//tabfusioar = (SaaArete **)g_malloc(2*nbfusio*sizeof(SaaArete *));
	nbfusio = 0;
	for(i=0; i<sol->nbAretes; i++)
	for(j=0; j<sol->nbAretes; j++)
		if(tabfusio[i][j]) {
			tabfusio[j][i] = 0;
			tabfusioar[nbfusio++] = sol->lesAretes[i];
			tabfusioar[nbfusio++] = sol->lesAretes[j];
		}

	// Fusion effective des aretes
	fprintf(SaaLogFile, "- Fusion effective des demi-aretes ... ");
	nbnet = sol->nbAretes;
	for(i=0; i<nbfusio; i+=2)
	saa_solide_fusionner_demi_aretes(sol, tabfusioar[i], tabfusioar[i+1]);
	//printf("%d: ar%d - ar%d\n", i, 1+idxArete(tabfusioar[i], sol), 1+idxArete(tabfusioar[i+1], sol));
	nbnet -= sol->nbAretes;
	fprintf(SaaLogFile, "%d\n", nbnet);

	fprintf(SaaLogFile, "- Nettoyage des ailes isolees ... ");
	nbnet = sol->nbAiles;
	saa_solide_nettoyer_ailes_isolees(sol);
	nbnet -= sol->nbAiles;
	fprintf(SaaLogFile, "%d\n", nbnet);


	fprintf(SaaLogFile, "- Nettoyage des aretes ... ");
	nbnet = sol->nbAretes;
	saa_solide_nettoyer_aretes(sol);
	nbnet -= sol->nbAretes;
	fprintf(SaaLogFile, "%d\n", nbnet);

	fprintf(SaaLogFile, "- Nettoyage des sommets ... ");
	nbnet = sol->nbSommets;
	saa_solide_nettoyer_sommets(sol);
	nbnet -= sol->nbSommets;
	fprintf(SaaLogFile, "%d\n", nbnet);

	fprintf(SaaLogFile, "- Liberation d'espace memoire\n");
	g_free(tabsupprai);
	for(i=0; i<nbar; i++)
	g_free(tabfusio[i]);
	g_free(tabfusio);
	g_free(tabfusioar);
	fprintf(SaaLogFile, "< EVIDAGE TERMINE - %d faces supprimees et %d demi-aretes fusionnees\n", nbsuppr, nbfusio);
	return 0;
}

// Fusionne deux demi-aretes
void saa_solide_fusionner_demi_aretes(SaaSolide * sol, SaaArete * ar1, SaaArete * ar2) {
  int trouvar;
  //int memesens = (ar1->s1 == ar2->s1) && (ar1->s2 == ar2->s2);
  SaaArete * ar, * sar;

  // Teste si les aretes ont des ailes "libres"
  if(!ar1->ai1 && !ar2->ai1) {
    ar1->ai1   = ar2->ai2;
    ar1->succ1 = ar2->succ2;
  }
  else if(!ar1->ai1 && !ar2->ai2) {
    ar1->ai1   = ar2->ai1;
    ar1->succ1 = ar2->succ1;
  }
  else if(!ar1->ai2 && !ar2->ai2) {
    ar1->ai2   = ar2->ai1;
    ar1->succ2 = ar2->succ1;
  }
  else if(!ar1->ai2 && !ar2->ai1) {
    ar1->ai2   = ar2->ai2;
    ar1->succ2 = ar2->succ2;
  }
  else {
    /*
    if(idxAile(ar1->ai1, sol) < 0 ||
       idxAile(ar1->ai2, sol) < 0)
      supprimeAreteSolideAA(sol, ar1);
    if(idxAile(ar2->ai1, sol) < 0 ||
       idxAile(ar2->ai2, sol) < 0)
      supprimeAreteSolideAA(sol, ar2);
    */
    return;
  }

  // Cherche l'arete precedant ar1 dans l'aile ai1
  //printf("pour ai%d : ", 1+idxAile(ar1->ai1, sol));
  trouvar = 0;
  ar = ar1->ai1->start;
  do {
    //printf("ar%d ", 1+idxArete(ar, sol));
    if(ar->ai1 == ar1->ai1) {
      sar = ar->succ1;
      if(sar == ar2) ar->succ1 = ar1, trouvar = 1;
    }
    else {
      sar = ar->succ2;
      if(sar == ar2) ar->succ2 = ar1, trouvar = 1;
    }
    ar = sar;
  } while(ar != ar1->ai1->start && !trouvar);
  if(ar1->ai1->start == ar2)
    ar1->ai1->start = ar1;

  // Cherche l'arete precedant ar1 dans l'aile ai2
  //printf("\npour ai%d : ", 1+idxAile(ar1->ai2, sol));
  trouvar = 0;
  ar = ar1->ai2->start;
  do {
    //printf("ar%d ", 1+idxArete(ar, sol));
    if(ar->ai1 == ar1->ai2) {
      sar = ar->succ1;
      if(sar == ar2) ar->succ1 = ar1, trouvar = 1;
    }
    else {
      sar = ar->succ2;
      if(sar == ar2) ar->succ2 = ar1, trouvar = 1;
    }
    ar = sar;
  } while(ar != ar1->ai2->start && !trouvar);
  if(ar1->ai2->start == ar2)
    ar1->ai2->start = ar1;
  //printf("\n");

  // Suppression de l'arete inutile
  saa_solide_supprimer_arete(sol, ar2);
}

SaaArete * saa_solide_fusionner_aretes(SaaSolide * sol, SaaArete * ar1, SaaArete * ar2) {
  int i;
  SaaAile * ai;
  SaaArete * new;
  SaaArete * ar;

  if(!((ar1->ai1 == ar2->ai1 && ar1->ai2 == ar2->ai2) ||
       (ar1->ai1 == ar2->ai2 && ar1->ai2 == ar2->ai1)))
    return NULL;

  if(saa_solide_index_aile(sol, ar1->ai1) < 0 || saa_solide_index_aile(sol, ar1->ai2) < 0 ||
     saa_solide_index_aile(sol, ar2->ai1) < 0 || saa_solide_index_aile(sol, ar2->ai2) < 0) {
	  fprintf(SaaErrorFile, "allo ?\n");
    return NULL;
  }

  new = (SaaArete *)g_malloc(sizeof(SaaArete));

  new->ai1 = ar1->ai1;
  new->ai2 = ar1->ai2;

  if(ar1->s2 == ar2->s1) {
    //printf("cas1\n");
    new->s1 = ar1->s1;
    new->s2 = ar2->s2;
    new->succ1 = ar2->succ1;
    new->succ2 = ar1->succ2;
  }
  else if(ar1->s1 == ar2->s2) {
    //printf("cas2\n");
    new->s1 = ar2->s1;
    new->s2 = ar1->s2;
    new->succ1 = ar1->succ1;
    new->succ2 = ar2->succ2;
  }
  for(i=0; i<sol->nbAretes; i++) {
    ar = sol->lesAretes[i];
    if(ar->succ1 == ar1 || ar->succ1 == ar2)
      ar->succ1 = new;
    if(ar->succ2 == ar1 || ar->succ2 == ar2)
      ar->succ2 = new;
  }
  for(i=0; i<sol->nbAiles; i++) {
    ai = sol->lesAiles[i];
    if(ai->start == ar1 || ai->start == ar2)
      ai->start = new;
  }

  saa_solide_supprimer_arete(sol, ar1);
  saa_solide_supprimer_arete(sol, ar2);
  sol->lesAretes = (SaaArete **)g_realloc(sol->lesAretes, (sol->nbAretes+1)*sizeof(SaaArete *));

  sol->lesAretes[sol->nbAretes++] = new;

  return new;
}

// Fusion de deux ailes ayant une arete commune
void saa_solide_fusionner_ailes(SaaSolide * sol, SaaArete * ar) {
  int i;
  int trouvar1, trouvar2;
  SaaAile * ai1 = ar->ai1, * ai2 = ar->ai2, * aitmp;
  SaaArete * arc, * sarc;
  SaaArete * ar1 = NULL, * ar2 = NULL;   // aretes precedant ar dans chaque aile
  SaaArete * sar1 = NULL, * sar2 = NULL; // aretes devant leur succeder

  // Recherche dans ai1 de la 1ere arete prec et de la 2eme succ
  trouvar1 = 0;
  arc = ai1->start;
  do {
    sarc = (arc->ai1 == ai1) ? arc->succ1 : arc->succ2;
    if(sarc == ar)
      ar1 = arc, trouvar1++;
    if(arc == ar)
      sar2 = sarc, trouvar1++;
    arc = sarc;
  } while(arc != ai1->start && trouvar1 != 2);

  // Recherche dans ai2 de la 2eme arete prec et de la 1ere succ
  trouvar2 = 0;
  arc = ai2->start;
  do {
    sarc = (arc->ai1 == ai2) ? arc->succ1 : arc->succ2;
    if(sarc == ar)
      ar2 = arc, trouvar2++;
    if(arc == ar)
      sar1 = sarc, trouvar2++;
    arc = sarc;
  } while(arc != ai2->start && trouvar2 != 2);

  //printf("precar1 : ar%d\n", 1+saa_solide_index_arete(sol, ar1));
  //printf("succar1 : ar%d\n", 1+saa_solide_index_arete(sol, sar1));
  //printf("precar2 : ar%d\n", 1+saa_solide_index_arete(sol, ar2));
  //printf("succar2 : ar%d\n", 1+saa_solide_index_arete(sol, sar2));

  // Recomposition de la succession d'aretes
  if(trouvar1 == 2 && trouvar2 == 2) {
    if(ar1->ai1 == ai1)
      ar1->succ1 = sar1;
    else
      ar1->succ2 = sar1;

    if(ar2->ai1 == ai2)
      ar2->succ1 = sar2;
    else
      ar2->succ2 = sar2;

    ai1->start = ar1;
  } else if(trouvar1 == 0) {
    if(ar2->ai1 == ai2)
      ar2->succ1 = sar1;
    else
      ar2->succ2 = sar1;
    ai1->start = ar2;
  } else if(trouvar2 == 0) {
    if(ar1->ai1 == ai1)
      ar1->succ1 = sar2;
    else
      ar1->succ2 = sar2;
    aitmp = ai1, ai1 = ai2, ai2 = aitmp;
    ai1->start = ar1;
  }

  // Remplacement de ai2 par ai1 dans chaque arete
  for(i=0; i<sol->nbAretes; i++) {
    arc = sol->lesAretes[i];
    if(arc->ai1 == ai2)
      arc->ai1 = ai1;
    if(arc->ai2 == ai2)
      arc->ai2 = ai1;
  }

  // Suppression de l'aile inutile
  saa_solide_supprimer_aile(sol, ai2);

  // Suppression des aretes de separation
  //saa_solide_nettoyer_aretes(sol); // car il peut y en avoir pls
}

// Fusion des ailes coplanaires
int saa_solide_fusionner(SaaSolide * sol) {
  int i, numar, numai, nbfusai = 0, nbfusar = 0, fus;
  SaaArete * ar, * sar;
  SaaAile * ai;
  SaaSommet var1, var2; // vecteurs arete
  SaaSommet pvar;       // produit vectoriel entre vecteurs arete
  SaaSommet n1, n2;     // normales aux ailes
  SaaSommet pvn;        // produit vectoriel entre les normales
  SaaArete ** tabfusar = (SaaArete **)g_malloc(sol->nbAretes*sizeof(SaaArete *));


  fprintf(SaaLogFile, "> FUSION\n");

  fprintf(SaaLogFile, "- Recuperation des aretes joignant deux ailes a fusionner ...");
  fflush(SaaLogFile);
  for(numar=0; numar<sol->nbAretes; numar++) {
    ar = sol->lesAretes[numar];
//printf("ar%d : ai%d - ai%d\n", numar, 1+saa_solide_index_aile(sol, ar->ai1), 1+saa_solide_index_aile(sol, ar->ai2));
    saa_aile_vecteur_normal(ar->ai1, &n1);
    saa_aile_vecteur_normal(ar->ai2, &n2);
    saa_sommet_pv(&n1, &n2, &pvn);
    if(saa_sommet_distance(&pvn) < EPSILON_2) {

        tabfusar[nbfusai++] = ar;
    }
  }
  fprintf(SaaLogFile, " %d\n", nbfusai);

  fprintf(SaaLogFile, "- Fusion effective des ailes\n");
  for(i=0; i<nbfusai; i++)
    if(saa_solide_index_arete(sol, tabfusar[i]) >= 0)
      saa_solide_fusionner_ailes(sol, tabfusar[i]);
    //else printf("%d arete inconnue\n");

  fprintf(SaaLogFile, "- Fusion des aretes telescopees\n");
  for(numai=0; numai<sol->nbAiles; numai++) {
    ai = sol->lesAiles[numai];
    ar = ai->start;
    do {
      fus = 0;
      do {
        sar = (ai == ar->ai1) ? ar->succ1 : ar->succ2;
        saa_sommet_difference(ar->s1, ar->s2, &var1);
        saa_sommet_difference(sar->s1, sar->s2, &var2);
        saa_sommet_pv(&var1, &var2, &pvar);
        if(saa_sommet_distance(&pvar) < EPSILON_2)
          if((sar = saa_solide_fusionner_aretes(sol, ar, sar)))
            nbfusar++, fus = 1;
        ar = sar;
      } while(ar != ai->start);
    } while(fus);
  }

  fprintf(SaaLogFile, "- Nettoyage des aretes\n");
  saa_solide_nettoyer_aretes(sol);

  fprintf(SaaLogFile, "- Nettoyage des sommets\n");
  saa_solide_nettoyer_sommets(sol);

  fprintf(SaaLogFile, "- Liberation de l'espace memoire\n");
  g_free(tabfusar);

  fprintf(SaaLogFile, "< FUSION TERMINEE - %d ailes et %d aretes fusionnees\n", nbfusai, nbfusar);
  return 0;
}

int saa_solide_segmenter(SaaSolide * sol) {
	int i, j, k, numar1, numar2, nbar = sol->nbAretes;
	SaaArete * ar1, * ar2, * newar;
	SaaSommet si, * s1, * s2;
	SaaSommet *** tabinterar = (SaaSommet ***)g_malloc(nbar*sizeof(SaaSommet **));
	int * diminterar = (int *)g_malloc(nbar*sizeof(int));
	SaaArete *** tabaliasar = (SaaArete ***)g_malloc(nbar*sizeof(SaaArete **));
	int * dimaliasar = (int *)g_malloc(nbar*sizeof(int));

	for(i=0; i<nbar; i++) {
		tabinterar[i] = (SaaSommet **)g_malloc(nbar*sizeof(SaaSommet *));
		tabaliasar[i] = (SaaArete **)g_malloc(nbar*sizeof(SaaArete *));
		tabaliasar[i][0] = sol->lesAretes[i];
		diminterar[i] = 0;
		dimaliasar[i] = 1;
	}

	fprintf(SaaLogFile, "- segmenter : Recuperation des aretes en intersection\n");
	for(numar1=0; numar1<nbar; numar1++) {
		ar1 = sol->lesAretes[numar1];
		for(numar2=numar1+1; numar2<nbar; numar2++) {
			ar2 = sol->lesAretes[numar2];

			if(saa_sommet_intersection_segments(ar1->s1, ar1->s2, ar2->s1, ar2->s2, &si)) {
				if(saa_sommet_comparer(&si, ar1->s1) && saa_sommet_comparer(&si, ar1->s2)) {
//printf("ar%d coupe ar%d\n", 1+saa_solide_index_arete(sol, ar2), 1+saa_solide_index_arete(sol, ar1));
					tabinterar[numar1][diminterar[numar1]++] = ar2->s1;
					tabinterar[numar1][diminterar[numar1]++] = ar2->s2;
				}

				if(saa_sommet_comparer(&si, ar2->s1) && saa_sommet_comparer(&si, ar2->s2)) {
//printf("ar%d coupe ar%d\n", 1+saa_solide_index_arete(sol, ar1), 1+saa_solide_index_arete(sol, ar2));
					tabinterar[numar2][diminterar[numar2]++] = ar1->s1;
					tabinterar[numar2][diminterar[numar2]++] = ar1->s2;
				}

			}
		}
	}

	fprintf(SaaLogFile, "- segmenter : Decoupage effectif des aretes\n");
	for(i=0; i<nbar; i++) {
		for(j=0; j<diminterar[i]; j+=2) {
			s1 = tabinterar[i][j];
			s2 = tabinterar[i][j+1];
			newar = NULL;
			for(k=0; k<dimaliasar[i] && !newar; k++) {
				ar1 = tabaliasar[i][k];
				if(saa_sommet_intersection_segments(ar1->s1, ar1->s2, s1, s2, &si) &&
						saa_sommet_comparer(&si, s1) && saa_sommet_comparer(&si, s2)) {
//printf("debug : ar%d est coupee par [s%d s%d]\n", 1+saa_solide_index_arete(sol, ar1), 1+saa_solide_index_sommet(sol, s1), 1+saa_solide_index_sommet(sol, s2));
					newar = saa_solide_couper_arete(sol, ar1, &si);
					if(newar != ar1) {
//printf("debug : Creation d'une nouvelle arete pour ar%d : ar%d\n", i+1, 1+saa_solide_index_arete(sol, newar));
						tabaliasar[i][dimaliasar[i]++] = newar;
					} else {
						newar = NULL;
					}
				}
			}
		}
	}

	fprintf(SaaLogFile, "- segmenter : Liberation d'espace memoire\n");
	for(i=0; i<nbar; i++) {
		g_free(tabaliasar[i]);
		g_free(tabinterar[i]);
	}

	g_free(tabaliasar);
	g_free(tabinterar);
	g_free(dimaliasar);
	g_free(diminterar);

	return 0;
}
