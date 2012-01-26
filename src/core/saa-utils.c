/******************************* saa-utils.c *********************************/
/*                                                                           */
/*   Bibliothèque statique de base :                                         */
/*   Utilitaires                                                             */
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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "saa-utils.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <glib.h>

#ifdef G_OS_WIN32
#	include <windows.h>
#	include <io.h>
#endif

static gchar *exe = NULL;

#ifdef G_OS_WIN32

static HMODULE libsaabase_dll = NULL;

/* Minimal DllMain that just stores the handle to this DLL */

BOOL WINAPI			/* Avoid silly "no previous prototype" gcc warning */
DllMain (HINSTANCE hinstDLL,
	 DWORD     fdwReason,
	 LPVOID    lpvReserved);

BOOL WINAPI
DllMain (HINSTANCE hinstDLL,
	 DWORD     fdwReason,
	 LPVOID    lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      libsaabase_dll = hinstDLL;
      break;
  }

  return TRUE;
}

#endif

int defaultCompareFunc (void * eA, void * eB) {
	return eA == eB;
}

SaaListe * saa_liste_creer() {
	SaaListe * nouv = (SaaListe *)g_malloc(sizeof(SaaListe));
	g_return_val_if_fail(nouv, NULL);
	nouv->premier = NULL;
	return nouv;
}

void saa_liste_detruire(SaaListe * l) {
	SaaListeElt * elt, * nextelt;
	for(elt=l->premier; elt; elt = nextelt) {
		nextelt = elt->suivant;
		g_free(elt);
	}
	g_free(l);
}

SaaListeElt * saa_element_creer(void * e) {
	SaaListeElt * nouv = (SaaListeElt *)g_malloc(sizeof(SaaListeElt));
	g_return_val_if_fail(nouv, NULL);
	nouv->donnee = e;
	nouv->suivant = NULL;
	return nouv;
}

int saa_liste_taille(const SaaListe * l) {
	int t;
	SaaListeElt * elt;
	for(elt=l->premier, t=0; elt; elt=elt->suivant, t++);
	return t;
}

void saa_liste_ajouter(SaaListe * l, void * e) {
	SaaListeElt * elt;
	int ajout = 0;

	if(!l->premier)
		l->premier = saa_element_creer(e);
	else
	for(elt=l->premier; elt && !ajout; elt=elt->suivant) {
		if(!elt->suivant) {
			ajout = 1;
			elt->suivant = saa_element_creer(e);
		}
	}
}

void saa_liste_enlever(SaaListe * l, void * e) {
	SaaListeElt * elt;
	int enlev = 0;

	if(l->premier && l->premier->donnee == e)
		l->premier = l->premier->suivant;
	else
	for(elt=l->premier; elt && !enlev; elt=elt->suivant) {
		if(elt->suivant && elt->suivant->donnee == e) {
			elt->suivant = elt->suivant->suivant;
			enlev = 1;
		}
	}
}

void saa_liste_ajouter_unique(SaaListe * l, void * e, SaaCompareFunc f) {
	int ajout = 0, present = 0;
	SaaListeElt * elt;

	if(!f) f = defaultCompareFunc;

	if(!l->premier)
		l->premier = saa_element_creer(e);
	else
	for(elt=l->premier; elt && !ajout && !present; elt=elt->suivant) {
		present = !f(elt->donnee, e);

		if(!elt->suivant && !present) {
			ajout = 1;
			elt->suivant = saa_element_creer(e);
		}
	}
}


void * saa_liste_element(const SaaListe * l, int i) {
	int t;
	SaaListeElt * elt;
	void * result = NULL;
	for(elt=l->premier, t=0; elt && t<=i; elt=elt->suivant, t++)
		result = t == i ? elt->donnee : NULL;
	return result;
}

int saa_liste_index(const SaaListe * l, void * e) {
	int t;
	SaaListeElt * elt;
	int result = -1;
	for(elt=l->premier, t=0; elt && result<0; elt=elt->suivant, t++)
		result = elt->donnee == e ? t : -1;
	return result;
}

void saa_liste_pour_chaque(SaaListe * l, SaaPourChaqueFunc  f, void * e) {
	SaaListeElt * elt;
	int loop = 1;
	for(elt=l->premier; elt && loop; elt=elt->suivant)
		loop = f(elt->donnee, e);
}

void saa_liste_trier(SaaListe * l, SaaCompareFunc f) {
	SaaListeElt * elt1, * elt2;
	void * tmp;
	for(elt1=l->premier; elt1; elt1=elt1->suivant)
		for(elt2=elt1->suivant; elt2; elt2=elt2->suivant)
			if(f(elt1->donnee, elt2->donnee) < 0) {
				tmp = elt1->donnee;
				elt1->donnee = elt2->donnee;
				elt2->donnee = tmp;
			}
}

FILE * SaaErrorFile = NULL;
FILE * SaaLogFile = NULL;

void saa_init_files() {
	if(SaaErrorFile == NULL) SaaErrorFile = stderr;
	if(SaaLogFile == NULL) SaaLogFile = stdout;
}

// Fait des jolis noms avec des majuscules (titre)
void saa_casse_chameau(char * s) {
  int indic = 1;
  do
    if(*s == ' ')
      indic = 1;
    else {
      if(indic)
        *s = toupper(*s);
      indic = 0;
    }
  while(*s++);
}

// Fait un nom sobre sans espace (nom de fichier)
void saa_sans_espace(char * s) {
  int i = 0, d = 0;
  do {
    if(s[i+d] == ' ') d++;
    s[i] = s[i+d];
  } while(s[++i]);
}

// Recherche des répertoires pompée sur gimp (merci gimp)
static gchar * _saa_reloc_find_prefix (const gchar *default_prefix) {
        gchar *dir1, *dir2;

        if (exe == NULL) {
                /* BinReloc not initialized. */
                if (default_prefix != NULL)
                        return g_strdup (default_prefix);
                else
                        return NULL;
        }

        dir1 = g_path_get_dirname (exe);
        dir2 = g_path_get_dirname (dir1);
        g_free (dir1);

        return dir2;
}

static gchar * _saa_reloc_find_data_dir (const gchar *default_data_dir) {
        gchar *prefix, *dir;

        prefix = _saa_reloc_find_prefix (NULL);
        if (prefix == NULL) {
                /* BinReloc not initialized. */
                if (default_data_dir != NULL)
                        return g_strdup (default_data_dir);
                else
                        return NULL;
        }

        dir = g_build_filename (prefix, "share",
                                PACKAGE,
                                VERSION,
                                NULL);
        g_free (prefix);

        return dir;
}

static gchar * _saa_reloc_find_locale_dir (const gchar * default_locale_dir) {
        gchar *data_dir, *dir;

        data_dir = _saa_reloc_find_data_dir (NULL);
        if (data_dir == NULL) {
                /* BinReloc not initialized. */
                if (default_locale_dir != NULL)
                        return g_strdup (default_locale_dir);
                else
                        return NULL;
        }

        dir = g_build_filename (data_dir, "locale", NULL);
        g_free (data_dir);

        return dir;
}

static const gchar * saa_toplevel_directory (void) {
  static gchar *toplevel = NULL;

  if (toplevel)
    return toplevel;

#ifdef G_OS_WIN32
  {
    /* Figure it out from the location of this DLL */
    gchar *filename;
    gchar *sep1, *sep2;

    wchar_t w_filename[MAX_PATH];

    if (GetModuleFileNameW (libsaabase_dll, w_filename, G_N_ELEMENTS (w_filename)) == 0)
      g_error ("GetModuleFilenameW failed");

    filename = g_utf16_to_utf8 (w_filename, -1, NULL, NULL, NULL);
    if (filename == NULL)
      g_error ("Converting module filename to UTF-8 failed");

    /* If the DLL file name is of the format
     * <foobar>\bin\*.dll, use <foobar>.
     * Otherwise, use the directory where the DLL is.
     */

    sep1 = strrchr (filename, '\\');
    *sep1 = '\0';

    sep2 = strrchr (filename, '\\');
    if (sep2 != NULL) {
        if (g_ascii_strcasecmp (sep2 + 1, "bin") == 0) {
            *sep2 = '\0';
        }
    }

    toplevel = filename;
  }
#else
  toplevel = _saa_reloc_find_prefix (PREFIX);
#endif
  return toplevel;
}

static void saa_path_runtime_fix (gchar **path) {
#if defined (G_OS_WIN32) && defined (PREFIX)
  gchar *p;

  /* Yes, I do mean forward slashes below */
  if (strncmp (*path, PREFIX "/", strlen (PREFIX "/")) == 0) {
      /* This is a compile-time entry. Replace the path with the
       * real one on this machine.
       */
      p = *path;
      *path = g_strconcat (saa_toplevel_directory (),
                           "\\",
                           *path + strlen (PREFIX "/"),
                           NULL);
      g_free (p);
  }
  /* Replace forward slashes with backslashes, just for
   * completeness */
  p = *path;
  while ((p = strchr (p, '/')) != NULL) {
      *p = '\\';
      p++;
  }
#elif defined (G_OS_WIN32)
  /* without defineing PREFIX do something useful too */
  gchar *p = *path;
  if (!g_path_is_absolute (p)) {
      *path = g_build_filename (saa_toplevel_directory (), *path, NULL);
      g_free (p);
  }
#else
  gchar *p;

  if (strncmp (*path, PREFIX G_DIR_SEPARATOR_S,
               strlen (PREFIX G_DIR_SEPARATOR_S)) == 0)
    {
      /* This is a compile-time entry. Replace the path with the
       * real one on this machine.
       */
      p = *path;
      *path = g_build_filename (saa_toplevel_directory (),
                                *path + strlen (PREFIX G_DIR_SEPARATOR_S),
                                NULL);
      g_free (p);
    }
#endif
}

static gchar * saa_env_get_dir (const gchar * saa_env_name, const gchar *env_dir) {
  const gchar *env = g_getenv (saa_env_name);

  if(env) {
      if (! g_path_is_absolute (env))
        g_error ("%s environment variable should be an absolute path.",
                 saa_env_name);
      return g_strdup (env);
  } else {
      gchar *retval = g_strdup (env_dir);

      saa_path_runtime_fix (&retval);
      return retval;
  }
}

const gchar * saa_locale_directory (void) {
  static gchar * saa_locale_dir = NULL;

  if (! saa_locale_dir) {
      gchar * tmp = _saa_reloc_find_locale_dir (LOCALEDIR);

      saa_locale_dir = saa_env_get_dir ("SAA_LOCALEDIR", tmp);
      g_free (tmp);
#ifdef G_OS_WIN32
      tmp = g_win32_locale_filename_from_utf8 (saa_locale_dir);
      g_free (saa_locale_dir);
      saa_locale_dir = tmp;
#endif
  }
  return saa_locale_dir;
}
