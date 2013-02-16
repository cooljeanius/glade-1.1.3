/*  Gtk+ User Interface Builder
 *  Copyright (C) 1999  Damon Chaplin
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

#include <config.h>

#include <bonobo.h>
#include "../gb.h"
#include "../tree.h"

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/gnome-app.xpm"

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;


static gchar *Title = "BonoboWindow|GtkWindow::title";
static gchar *Type = "BonoboWindow|GtkWindow::type";
static gchar *Position = "BonoboWindow|GtkWindow::position";
static gchar *Modal = "BonoboWindow|GtkWindow::modal";
static gchar *DefaultWidth = "BonoboWindow|GtkWindow::default_width";
static gchar *DefaultHeight = "BonoboWindow|GtkWindow::default_height";
static gchar *Shrink = "BonoboWindowl|GtkWindow::allow_shrink";
static gchar *Grow = "BonoboWindow|GtkWindow::allow_grow";
static gchar *AutoShrink = "BonoboWindow|GtkWindow::auto_shrink";
static gchar *WMName = "BonoboWindow|GtkWindow::wmclass_name";
static gchar *WMClass = "BonoboWindow|GtkWindow::wmclass_class";

static gchar *Resizable = "BonoboWindow|GtkWindow::resizable";
static gchar *DestroyWithParent = "BonoboWindow|GtkWindow::destroy_with_parent";
static gchar *Icon = "BonoboWindow|GtkWindow::icon";

/* This is kept as object data as we don't want config info written when it is
   used in Glade. */
static gchar *EnableLayoutConfig = "BonoboWindow::enable_layout_config";

/* This is only used within Glade. We don't save or load it, since the
   appbar will be saved on its own if it has been added. */
static gchar *StatusBar = "BonoboWindow::status_bar";

#if 0

/* The indices come from GladeStockMenuItemValues in glade_gnome.c */
static gint FileMenuIndices[] = { 3, 4, 5, 6, -1, 11 };
static GnomeUIInfo FileMenu[] =
{
  GNOMEUIINFO_MENU_NEW_ITEM (N_("_New File"), N_("Create a new file"),
			     NULL, NULL),
  GNOMEUIINFO_MENU_OPEN_ITEM (NULL, NULL),
  GNOMEUIINFO_MENU_SAVE_ITEM (NULL, NULL),
  GNOMEUIINFO_MENU_SAVE_AS_ITEM (NULL, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_EXIT_ITEM (NULL, NULL),
  GNOMEUIINFO_END
};

static gint EditMenuIndices[] = { 14, 15, 16, 18, -1, 24 };
static GnomeUIInfo EditMenu[] =
{
  GNOMEUIINFO_MENU_CUT_ITEM (NULL, NULL),
  GNOMEUIINFO_MENU_COPY_ITEM (NULL, NULL),
  GNOMEUIINFO_MENU_PASTE_ITEM (NULL, NULL),
  GNOMEUIINFO_MENU_CLEAR_ITEM (NULL, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_PROPERTIES_ITEM (NULL, NULL),
  GNOMEUIINFO_END
};

static gint ViewMenuIndices[] = { -1 };
static GnomeUIInfo ViewMenu[] =
{
  GNOMEUIINFO_END
};

static gint SettingsMenuIndices[] = { 29 };
static GnomeUIInfo SettingsMenu[] =
{
  GNOMEUIINFO_MENU_PREFERENCES_ITEM (NULL, NULL),
  GNOMEUIINFO_END
};

static gint HelpMenuIndices[] = { 38 };
static GnomeUIInfo HelpMenu[] =
{
  GNOMEUIINFO_MENU_ABOUT_ITEM (NULL, NULL),
  GNOMEUIINFO_END
};

static gint MainMenuIndices[] = { 2, 13, 26, 28, 37 };
static GnomeUIInfo MainMenu[] =
{
  GNOMEUIINFO_MENU_FILE_TREE (FileMenu),
  GNOMEUIINFO_MENU_EDIT_TREE (EditMenu),
  GNOMEUIINFO_MENU_VIEW_TREE (ViewMenu),
  GNOMEUIINFO_MENU_SETTINGS_TREE (SettingsMenu),
  GNOMEUIINFO_MENU_HELP_TREE (HelpMenu),
  GNOMEUIINFO_END
};


#endif /* 0 */

static void gb_bonobo_window_setup_initial_app (GtkWidget *widget);

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the function in the GbWidget struct at the end of this file.
 ******/

/*
 * Creates a new GtkWidget of class BonoboWindow, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */
static GtkWidget*
gb_bonobo_window_new (GbWidgetNewData *data)
{
  GtkWidget *new_widget;
  gchar *project_name;

  project_name = glade_project_get_name (data->project);
  new_widget = bonobo_window_new (project_name ? project_name : "", project_name);
  gtk_object_set_data (GTK_OBJECT (new_widget), EnableLayoutConfig,
		       GINT_TO_POINTER (TRUE));

  /* Turn off the automatic loading/saving of the configuration, and
     destroy the BonoboDockLayout, so that when items are added they are
     added straight away. If we don't do this, items don't get added to
     the widget tree. */
#if 0
  bonobo_window_enable_layout_config (BONOBO_WINDOW (new_widget), FALSE);
  g_object_unref (G_OBJECT (BONOBO_WINDOW (new_widget)->layout));
  BONOBO_WINDOW (new_widget)->layout = NULL;
#endif

  gtk_signal_connect (GTK_OBJECT (new_widget), "delete_event",
		      GTK_SIGNAL_FUNC (editor_close_window), NULL);

#if 0
  gb_widget_create_from (BONOBO_WINDOW (new_widget)->dock,
			 data->action == GB_CREATING ? "BonoboDock" : NULL);
  gb_widget_set_child_name (BONOBO_WINDOW (new_widget)->dock, GladeChildBonoboWindowDock);
#endif

  if (data->action == GB_CREATING)
    gb_bonobo_window_setup_initial_app (new_widget);

  return new_widget;
}


static void
gb_bonobo_window_setup_initial_app (GtkWidget *widget)
{
  GtkWidget *placeholder;
#ifdef FIXME
  GtkWidget *toolbar;
  GnomeUIInfo *uiinfo;
  gint i;

  /* We create a standard menubar and toolbar which the user can edit or
     simply delete anything they don't want. */
  bonobo_window_create_menus (BONOBO_WINDOW (widget), MainMenu);

  /* We need to get rid of the accelerators, since they aren't used, and
     they would override our default accelerators. */
  gtk_window_remove_accel_group (GTK_WINDOW (widget),
				 BONOBO_WINDOW (widget)->accel_group);

  /* FIXME: GnomeLibs bug workaround. It sets the initial border width
     to -2, which causes us lots of problems. */
#if 0
  gtk_container_set_border_width (GTK_CONTAINER (BONOBO_WINDOW (widget)->menubar),
				  0);
#endif
  gb_widget_create_from (BONOBO_WINDOW (widget)->menubar, "GtkMenubar");
  gb_widget_create_from (BONOBO_WINDOW (widget)->menubar->parent, "BonoboDockItem");

  /* Turn all the menus & menuitems into GbWidgets and add default
     handlers. */
  gb_bonobo_window_setup_menu_items (FileMenu, FileMenuIndices);
  gb_bonobo_window_setup_menu_items (EditMenu, EditMenuIndices);
  gb_bonobo_window_setup_menu_items (ViewMenu, ViewMenuIndices);
  gb_bonobo_window_setup_menu_items (SettingsMenu, SettingsMenuIndices);
  gb_bonobo_window_setup_menu_items (HelpMenu, HelpMenuIndices);
  gb_bonobo_window_setup_menu_items (MainMenu, MainMenuIndices);


  /* Toolbar */
  toolbar = gtk_toolbar_new ();
  bonobo_window_fill_toolbar (GTK_TOOLBAR (toolbar), ToolBar,
			  BONOBO_WINDOW (widget)->accel_group);
  bonobo_window_set_toolbar (BONOBO_WINDOW (widget), GTK_TOOLBAR (toolbar));
  gb_widget_create_from (toolbar, "GtkToolbar");
  gb_widget_create_from (toolbar->parent, "BonoboDockItem");

  for (uiinfo = ToolBar, i = 0;
       uiinfo->type != BONOBO_WINDOW_UI_ENDOFINFO;
       uiinfo++, i++)
    {
      if (uiinfo->widget)
	{
	  gb_widget_create_from (uiinfo->widget, "GtkButton");
	  gb_widget_set_child_name (uiinfo->widget, GladeChildToolbarButton);
	  gtk_object_set_data (GTK_OBJECT (uiinfo->widget), "GbStockIcon",
			       GINT_TO_POINTER (ToolBarIndices[i]));

	  /* Copy the tooltip to our GladeWidgetData struct. */
	  if (uiinfo->hint)
	    {
	      GladeWidgetData *wdata;

	      wdata = gtk_object_get_data (GTK_OBJECT (uiinfo->widget),
					   GB_WIDGET_DATA_KEY);
	      wdata->tooltip = g_strdup (_(uiinfo->hint));
	    }
	}
    }

  /* Statusbar */
  bonobo_window_set_statusbar (BONOBO_WINDOW (widget),
			   gb_widget_new ("BonoboWindowBar", widget));
  gb_widget_set_child_name (BONOBO_WINDOW (widget)->statusbar, GladeChildBonoboWindowAppBar);
#endif /* FIXME */
  /* We need to size the placeholders or the dialog is very small. */
  placeholder = editor_new_placeholder ();
  gtk_widget_set_usize (placeholder, 300, 200);
  bonobo_window_set_contents (BONOBO_WINDOW (widget), placeholder);
}

#ifdef FIXME
static void
gb_bonobo_window_setup_menu_items (GnomeUIInfo *uiinfo, gint *indices)
{
  gint i = 0;
  gchar *name, *src, *dest;

  while (uiinfo->type != BONOBO_WINDOW_UI_ENDOFINFO)
    {
      if (uiinfo->widget)
	{
	  if (indices[i] == -1)
	    {
	      gb_widget_create_from (uiinfo->widget, "separator");
	    }
	  else
	    {
	      name = g_malloc (strlen (uiinfo->label));
	      /* Convert spaces to underscores, and ignore periods (e.g. in
		 "Open...") and underscores (e.g. in "_Open"). */
	      for (src = uiinfo->label, dest = name; *src; src++)
		{
		  if (*src == ' ')
		    *dest++ = '_';
		  else if (*src == '.')
		    continue;
		  else if (*src == '_')
		    continue;
		  else
		    *dest++ = *src;
		}
	      *dest = '\0';

	      gb_widget_create_from (uiinfo->widget, name);

	      gtk_object_set_data (GTK_OBJECT (uiinfo->widget),
				   GladeMenuItemStockIndexKey,
				   GINT_TO_POINTER (indices[i]));

	      /* If the item has a child menu, turn it into a GbWidget.
		 If not, add a default handler to the item. */
	      if (GTK_IS_MENU_ITEM (uiinfo->widget)
		  && GTK_MENU_ITEM (uiinfo->widget)->submenu)
		{
		  gchar *submenu_name;

		  submenu_name = g_strdup_printf ("%s_menu",
						  uiinfo->widget->name);
		  gtk_widget_set_name (GTK_MENU_ITEM (uiinfo->widget)->submenu,
				       submenu_name);
		  g_free (submenu_name);
		  gb_widget_create_from (GTK_MENU_ITEM (uiinfo->widget)->submenu, NULL);
		}
	      else
		{
		  GladeWidgetData *wdata;
		  GladeSignal *signal;
		  gchar *start = "on_", *end = "_activate";

		  signal = g_new (GladeSignal, 1);
		  signal->name = g_strdup ("activate");
		  /* Generate a default handler name. */
		  signal->handler = g_malloc (strlen (uiinfo->widget->name) + 1
					      + strlen (start) + strlen (end));
		  strcpy (signal->handler, start);
		  strcat (signal->handler, uiinfo->widget->name);
		  strcat (signal->handler, end);

		  signal->object = NULL;
		  signal->after = FALSE;
		  signal->data = NULL;
		  signal->last_modification_time = time (NULL);

		  wdata = gtk_object_get_data (GTK_OBJECT (uiinfo->widget),
					       GB_WIDGET_DATA_KEY);
		  wdata->signals = g_list_append (wdata->signals, signal);
		}
	    }
	}
      uiinfo++;
      i++;
    }
}
#endif /* FIXME */

/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_bonobo_window_create_properties (GtkWidget * widget, GbWidgetCreateArgData * data)
{
  gb_window_create_standard_properties (widget, data,
					Title, Type, Position, Modal,
					DefaultWidth, DefaultHeight,
					Shrink, Grow, AutoShrink,
					WMName, WMClass,
					Resizable, DestroyWithParent, Icon);
#ifdef FIXME
  property_add_bool (StatusBar, _("Status Bar:"),
		     _("If the window has a status bar"));
  property_add_bool (EnableLayoutConfig, _("Store Config:"),
		     _("If the layout is saved and restored automatically"));
#endif
}



/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_bonobo_window_get_properties (GtkWidget *widget, GbWidgetGetArgData * data)
{
  gb_window_get_standard_properties (widget, data,
				     Title, Type, Position, Modal,
				     DefaultWidth, DefaultHeight,
				     Shrink, Grow, AutoShrink,
				     WMName, WMClass,
				     Resizable, DestroyWithParent, Icon);
#ifdef FIXME
  if (data->action == GB_SHOWING)
    {
      gb_widget_output_bool (data, StatusBar,
			     BONOBO_WINDOW (widget)->statusbar != NULL
			     ? TRUE : FALSE);
    }

  gb_widget_output_bool (data, EnableLayoutConfig, GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget), EnableLayoutConfig)));
#endif
}



/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_bonobo_window_set_properties (GtkWidget * widget, GbWidgetSetArgData * data)
{
  gboolean statusbar, enable_layout_config;

  gb_window_set_standard_properties (widget, data,
				     Title, Type, Position, Modal,
				     DefaultWidth, DefaultHeight,
				     Shrink, Grow, AutoShrink,
				     WMName, WMClass,
				     Resizable, DestroyWithParent, Icon);
#ifdef FIXME
  if (data->action == GB_APPLYING)
    {
      statusbar = gb_widget_input_bool (data, StatusBar);
      if (data->apply)
	{
	  if (statusbar)
	    {
	      if (!BONOBO_WINDOW (widget)->statusbar)
		{
		  bonobo_window_set_statusbar (BONOBO_WINDOW (widget),
					   gb_widget_new ("BonoboWindowBar",
							  widget));
		  gb_widget_set_child_name (BONOBO_WINDOW (widget)->statusbar,
					    GladeChildBonoboWindowAppBar);
		  tree_add_widget (BONOBO_WINDOW (widget)->statusbar);
		}
	    }
	  else
	    {
	      if (BONOBO_WINDOW (widget)->statusbar)
		{
		  /* This is not very clean, but there's no proper way to
		     remove the statusbar. The statusbar has an hbox inserted
		     above it which is added to the BonoboWindow's vbox, so we
		     remove the hbox. */
		  GtkWidget *hbox = BONOBO_WINDOW (widget)->statusbar->parent;
		  gtk_container_remove (GTK_CONTAINER (hbox->parent), hbox);
		  BONOBO_WINDOW (widget)->statusbar = NULL;
		}
	    }
	}
    }

  enable_layout_config = gb_widget_input_bool (data, EnableLayoutConfig);
  if (data->apply)
    {
      gtk_object_set_data (GTK_OBJECT (widget), EnableLayoutConfig,
			   GINT_TO_POINTER (enable_layout_config));
    }
#endif
}



/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a BonoboWindow, with signals pointing to
 * other functions in this file.
 */
/*
static void
gb_bonobo_window_create_popup_menu (GtkWidget * widget, GbWidgetCreateMenuData * data)
{

}
*/


#ifdef FIXME
/*
 * Writes the source code needed to create this widget.
 * You have to output everything necessary to create the widget here, though
 * there are some convenience functions to help.
 */
static void
gb_bonobo_window_write_source (GtkWidget * widget, GbWidgetWriteSourceData * data)
{
  gchar *appname;

  if (data->create_widget)
    {
      appname = glade_project_get_name (data->project);
      appname = appname ? source_make_string (appname, FALSE) : "\"\"";
      appname = g_strdup (appname);

      /* Note that this assumes that we use the same appname for each BonoboWindow.
	 FIXME: I think this is correct, but I'm not sure.
	 Note also that we don't translate the project name. Maybe we should.*/
      source_add (data, "  %s = bonobo_window_new (%s, %s);\n", data->wname,
		  appname,
		  GTK_WINDOW (widget)->title
		  ? source_make_string (GTK_WINDOW (widget)->title,
					data->use_gettext)
		  : "NULL");
      g_free (appname);
    }

  gb_widget_write_standard_source (widget, data);

  /* The title is already set above, so we pass NULL to skip it. */
  gb_window_write_standard_source (widget, data,
				   NULL, Type, Position, Modal,
				   DefaultWidth, DefaultHeight,
				   Shrink, Grow, AutoShrink,
				   WMName, WMClass,
				   Resizable, DestroyWithParent, Icon);

  if (!GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
					     EnableLayoutConfig)))
    {
      source_add (data,
		  "  bonobo_window_enable_layout_config (BONOBO_WINDOW (%s), FALSE);\n",
		  data->wname);
    }
}


/* Outputs source to add a child menu to a BonoboWindow. */
static void
gb_bonobo_window_write_add_child_source (GtkWidget * parent,
				     const gchar *parent_name,
				     GtkWidget *child,
				     GbWidgetWriteSourceData * data)
{
  gchar *child_name;

  child_name = gb_widget_get_child_name (child);

  if (child_name && (!strcmp (child_name, GladeChildBonoboWindowAppBar)))
    {
      source_add (data,
		  "  bonobo_window_set_statusbar (BONOBO_WINDOW (%s), %s);\n",
		  parent_name, data->wname);
    }
  else
    {
      g_warning ("Adding unknown child to a BonoboWindow: %s", data->wname);
      source_add (data, "  gtk_container_add (GTK_CONTAINER (%s), %s);\n",
		  parent_name, data->wname);
    }
}


static void
gb_bonobo_window_add_child (GtkWidget *widget, GtkWidget * child,
			GbWidgetSetArgData *data)
{
  gchar *child_name = NULL;

  if (data->child_info)
    child_name = data->child_info->internal_child;

  if (child_name && (!strcmp (child_name, GladeChildBonoboWindowAppBar)))
    {
      bonobo_window_set_statusbar (BONOBO_WINDOW (widget), child);
    }
}


static GtkWidget *
gb_bonobo_window_get_child (GtkWidget * widget,
			const gchar * child_name)
{
  if (!strcmp (child_name, GladeChildBonoboWindowDock))
    return BONOBO_WINDOW (widget)->dock;
  else
    return NULL;
}
#endif

/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */
GbWidget*
gb_bonobo_window_init ()
{
  /* Initialise the GTK type */
  volatile GtkType type;
  type = bonobo_window_get_type();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct(&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = gnome_app_xpm;
  gbwidget.tooltip = _("Bonobo Window");

  /* Fill in any functions that this GbWidget has */
  gbwidget.gb_widget_new		= gb_bonobo_window_new;
#ifdef FIXME
  gbwidget.gb_widget_add_child		= gb_bonobo_window_add_child;
  gbwidget.gb_widget_get_child		= gb_bonobo_window_get_child;
#endif
  gbwidget.gb_widget_create_properties	= gb_bonobo_window_create_properties;
  gbwidget.gb_widget_get_properties	= gb_bonobo_window_get_properties;
  gbwidget.gb_widget_set_properties	= gb_bonobo_window_set_properties;
#ifdef FIXME
  gbwidget.gb_widget_write_source	= gb_bonobo_window_write_source;
  gbwidget.gb_widget_write_add_child_source = gb_bonobo_window_write_add_child_source;
#endif
/*
  gbwidget.gb_widget_create_popup_menu	= gb_bonobo_window_create_popup_menu;
*/

  return &gbwidget;
}

