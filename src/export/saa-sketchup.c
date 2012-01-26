/******************************* saa-patron.c ********************************/
/*                                                                           */
/*   Export des solide en ruby pour sketchup                                 */
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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "saa-sketchup.h"

int saa_solide_exporter_RB(SaaRubyMode mode, const SaaSolide * sol, const char * filename, int echelle) {
	FILE * f = fopen(filename, "w");
	int nums, nums1, nums2, nbs = sol->nbSommets;
	int numar, nbar = sol->nbAretes;
	int numai, nbai = sol->nbAiles;
	SaaSommet * s;
	SaaArete * ar;
	SaaAile * ai;

	if(!f) {
		fprintf(SaaErrorFile, "saa_solide_exporter_RB : Impossible d'ouvrir le fichier %s\n", filename);
		return 1;
	}

	fprintf(f, "require 'sketchup'\n");
	fprintf(f, "\n");
	fprintf(f, "def draw_%s\n", sol->nom);
	fprintf(f, "\tmodel = Sketchup.active_model\n");
	fprintf(f, "\tentities = model.entities\n");
	fprintf(f, "\tedges = []\n");
	fprintf(f, "\tvertices = []\n");
	fprintf(f, "\tmodel.start_operation \"%s\"\n", sol->nom);
	fprintf(f, "\n");
	fprintf(f, "puts \"Cr�ation des sommets\"\n");
	for(nums=0; nums<nbs; nums++) {
		s = sol->lesSommets[nums];
		fprintf(f, "\tvertices[%d] = Geom::Point3d.new(%f, %f, %f)\n", nums, s->x*echelle, s->y*echelle, s->z*echelle);
	}
	fprintf(f, "\n");
	fprintf(f, "puts \"Cr�ation des aretes\"\n");
	for(numar=0; numar<nbar; numar++) {
		ar = sol->lesAretes[numar];
		nums1 = saa_solide_index_sommet(sol, ar->s1);
		nums2 = saa_solide_index_sommet(sol, ar->s2);
		fprintf(f, "\tedges[%d] = entities.add_line(vertices[%d], vertices[%d])\n", numar, nums1, nums2);
	}
	fprintf(f, "\n");
	fprintf(f, "puts \"Cr�ation des ailes\"\n");
	for(numai=0; numai<nbai; numai++) {
		ai = sol->lesAiles[numai];
		ar = ai->start;
		fprintf(f, "\tentities.add_face(");
		do {
			numar = saa_solide_index_arete(sol, ar);
			fprintf(f, "edges[%d]", numar);
			ar = (ar->ai1 == ai) ? ar->succ1 : ar->succ2;
			fprintf(f, "%s", ar != ai->start ? "," : ")");
		} while(ar != ai->start);
		fprintf(f, "\n");
	}
	fprintf(f, "\tmodel.commit_operation\n");
	fprintf(f, "end # def draw_%s\n", sol->nom);
	fprintf(f, "\n");
	fprintf(f, "if (not $gsolaar_submenu_loaded)\n");
	fprintf(f, "	$gsolaar_submenu = UI.menu(\"PlugIns\").add_submenu(\"%s\")\n", PACKAGE_NAME);
	fprintf(f, "	$gsolaar_submenu_loaded = true\n");
	fprintf(f, "end\n");
	fprintf(f, "$gsolaar_submenu.add_item(\"Draw %s\") { draw_%s }\n", sol->nom, sol->nom);

	fclose(f);

	return 0;
}
