/********************************* solaar.c **********************************/
/*                                                                           */
/*   Programme en ligne de commande :                                        */
/*   Traitement des polyèdres et génération de scripts et postscripts        */
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
#include "saa-core.h"
#include "saa-export.h"

int main(int argc, char ** argv) {
	char symbol[16];
	char prefix[8], ext[5];
	int num;
	int i;
	char * filename;
	SaaSolide * sol;

	int echelle = 230;

	// modes
	int numero = 0;
	int scriptI = 0;
	int xmlI = 0;
	int wythoff = 0;
	// export
	int scriptO = 0;
	int xmlO = 0;
	int psAS = 0;
	int psAC = 0;
	int svgAS = 0;
	int svgAC = 0;

	int dual = 0;

	int segment = 0;
	int decrois = 0;
	int decoup = 0;
	int contour = 0;
	int fusion = 0;

	if (argc < 3) {
		printf("usages : %s N[mnop][sx][b][c][def][2] NumeroPolyedre\n", argv[0]);
		printf("         %s W[mnop][sx][b][c][def][2] 'Wythoff formula'\n", argv[0]);
		printf("         %s X[mnop][sx][b][c][def]    Polyedre.xml\n", argv[0]);
		return -1;
	}

	for (i = 0; argv[1][i]; i++)
		switch (argv[1][i]) {
		case 'N':
			numero = 1;
			break;
		case 'S':
			scriptI = 1;
			break;
		case 'X':
			xmlI = 1;
			break;
		case 'W':
			wythoff = 1;
			break;

		case '2':
			dual = 1;
			break;

		case 'm':
			psAS = 1;
			break;
		case 'n':
			psAC = 1;
			break;
		case 'o':
			svgAS = 1;
			break;
		case 'p':
			svgAC = 1;
			break;

		case 'b':
			segment = 1;
			break;
		case 'c':
			decrois = 1;
			break;

		case 'd':
			decoup = 1;
			break;
		case 'e':
			contour = 1;
			break;
		case 'f':
			fusion = 1;
			break;

		case 's':
			scriptO = 1;
			break;
		case 'x':
			xmlO = 1;
			break;

		default:
			fprintf(stderr, "%c : Argument inconnu\n", argv[1][i]);
		}

	saa_init_files();

	// Generation
	if (numero) {
		num = atoi(argv[2]);
		if (num > 0 && num <= 80) {
			sprintf(symbol, "#%d", num);
			sol = dual ? saa_solide_creer_kaleido_dual(symbol)
					: saa_solide_creer_kaleido(symbol);
			sprintf(prefix, "%02d%s", num, dual ? "d" : "");
		} else {
			sol = NULL;
		}
	} else if (scriptI) {
		sol = saa_solide_importer_TXT(argv[2]);
	} else if (xmlI) {
		sol = saa_solide_importer_XML(argv[2]);
	} else if (wythoff) {
		strncpy(symbol, argv[2] + 1, strlen(argv[2]) - 2);
		sol = dual ? saa_solide_creer_kaleido_dual(symbol)
				: saa_solide_creer_kaleido(symbol);
		sprintf(prefix, "%s", dual ? "d" : "");
	}

	// Traitements
	if (sol) {
		if (segment)
			saa_solide_segmenter(sol);
		if (decrois)
			saa_solide_decroiser(sol);
		if (decoup)
			saa_solide_decouper(sol);
		if (contour)
			saa_solide_evider(sol);
		if (fusion)
			saa_solide_fusionner(sol);


		filename = (char *) malloc(strlen(prefix) + strlen(sol->nom) + 3);

		if (psAS || psAC) {
			strcpy(ext, "ps");
			sprintf(filename, "%s-%s.%s", prefix, sol->nom, ext);
			if(psAS) saa_solide_exporter_PS(sol, filename, SAA_PATRON_AILES_SEPAREES, SAA_LANG_NORMAL, SAA_TEXT_DESSOUS, echelle);
			if(psAC) saa_solide_exporter_PS(sol, filename, SAA_PATRON_AILES_COLLEES, SAA_LANG_NORMAL, SAA_TEXT_DESSOUS, echelle);
		}

		if (svgAS || svgAC) {
			strcpy(ext, "svg");
			sprintf(filename, "%s-%s.%s", prefix, sol->nom, ext);
			if(svgAS) saa_solide_exporter_SVG10(sol, filename, SAA_PATRON_AILES_SEPAREES, SAA_LANG_NORMAL, SAA_TEXT_DESSOUS, echelle);
			if(svgAC) saa_solide_exporter_SVG10(sol, filename, SAA_PATRON_AILES_COLLEES, SAA_LANG_NORMAL, SAA_TEXT_DESSOUS, echelle);
		}
		if (scriptO) {
			strcpy(ext, "txt");
			sprintf(filename, "%s-%s.%s", prefix, sol->nom, ext);
			saa_solide_exporter_TXT(sol, filename);
		}
		if (xmlO) {
			strcpy(ext, "xml");
			sprintf(filename, "%s-%s.%s", prefix, sol->nom, ext);
			saa_solide_exporter_XML(sol, filename);
		}
		saa_solide_detruire(sol);
	} else
		fprintf(stderr, "> Echec de la creation/du chargement du polyedre\n");

	return 0;
}
