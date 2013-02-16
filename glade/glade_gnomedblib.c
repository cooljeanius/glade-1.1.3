/*  Gtk+ User Interface Builder
 *  Copyright (C) 1998-1999  Damon Chaplin
 *  Copyright (C) 2001  Carlos Perelló Marín <carlos@gnome-db.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "gladeconfig.h"

#ifdef USE_GNOME_DB

#include "glade.h"
#include "gbwidget.h"

/* I've commented this out to avoid warnings. */
/*gchar *libname = "GNOME 1.0";*/

GbWidget *gb_gnome_db_browser_init ();
GbWidget *gb_gnome_db_combo_init ();
GbWidget *gb_gnome_db_connection_selector_init ();
GbWidget *gb_gnome_db_data_source_selector_init ();
GbWidget *gb_gnome_db_dsn_config_druid_init ();
GbWidget *gb_gnome_db_dsnconfig_init ();
GbWidget *gb_gnome_db_error_init ();
GbWidget *gb_gnome_db_errordlg_init ();
GbWidget *gb_gnome_db_form_init ();
GbWidget *gb_gnome_db_grid_init ();
GbWidget *gb_gnome_db_list_init ();
GbWidget *gb_gnome_db_login_init ();
GbWidget *gb_gnome_db_logindlg_init ();
GbWidget *gb_gnome_db_provider_selector_init ();
GbWidget *gb_gnome_db_sql_editor_init ();
GbWidget *gb_gnome_db_table_editor_init ();

static GladeWidgetInitData gnome_db[] = {
        { "GnomeDbLoginDialog", gb_gnome_db_logindlg_init },
	{ "GnomeDbLogin",       gb_gnome_db_login_init },
        { "GnomeDbErrorDialog", gb_gnome_db_errordlg_init }, 
        { "GnomeDbError",       gb_gnome_db_error_init },

        { "GnomeDbDsnConfig",   gb_gnome_db_dsnconfig_init },
	{ "GnomeDbDsnConfigDruid",	gb_gnome_db_dsn_config_druid_init },
	{ "GnomeDbProviderSelector",	gb_gnome_db_provider_selector_init },
	{ "GnomeDbConnectionSelector",	gb_gnome_db_connection_selector_init },

        { "GnomeDbBrowser",     gb_gnome_db_browser_init },
	{ "GnomeDbTableEditor",	gb_gnome_db_table_editor_init },
	{ "GnomeDbForm",	gb_gnome_db_form_init },
	{ "GnomeDbSqlEditor",	gb_gnome_db_sql_editor_init },

        { "GnomeDbGrid",        gb_gnome_db_grid_init },
        { "GnomeDbList",        gb_gnome_db_list_init },
        { "GnomeDbCombo",       gb_gnome_db_combo_init },



	/* These may be added back at some point. */
        /*{ "GnomeDbIconList",    gb_gnome_db_iconlist_init },*/

	{ NULL, NULL }  
};


static GladeWidgetInitData notshown[] =
{
  /* This is useless at present (it needs to have a provider set when
     created, and we can't do that), so we don't show it. */
  { "GnomeDbDataSourceSelector",	gb_gnome_db_data_source_selector_init },

  /* Unfinished. */
  { NULL, NULL }
};


static GladePaletteSectionData sections[] =
{
  { "Gnome _DB", gnome_db },
  { "NotShown", notshown },
  { NULL, NULL }
};


GladePaletteSectionData *get_gnome_db_widgets()
{
	return sections;
}

#endif
