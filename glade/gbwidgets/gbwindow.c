
/*  Gtk+ User Interface Builder
 *  Copyright (C) 1998  Damon Chaplin
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

#include <string.h>
#include <gtk/gtkspinbutton.h>
#include <gtk/gtktogglebutton.h>
#include <gtk/gtkwindow.h>
#include "../gb.h"

/* This file includes some common routines for standard window properties
   such as title, type, position, default width & height, shrink, grow,
   auto_shrink, wmname & wmclass. Other widgets can call these functions just
   by supplying the property names. Note that for most properties we use the
   widget's own property names as keys when storing values in the objects
   datalist, but for default width & height we use the windows own names since
   we need to access the values in the on_toggle_default_xx() callbacks. */

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/window.xpm"

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;

static gchar *Title = "GtkWindow::title";
static gchar *Type = "GtkWindow::type";
static gchar *Position = "GtkWindow::window_position";
static gchar *Modal = "GtkWindow::modal";
static gchar *DefaultWidth = "GtkWindow::default_width";
static gchar *DefaultHeight = "GtkWindow::default_height";
static gchar *WMName = "GtkWindow::wmclass_name";
static gchar *WMClass = "GtkWindow::wmclass_class";
static gchar *Resizable = "GtkWindow::resizable";
static gchar *DestroyWithParent = "GtkWindow::destroy_with_parent";
static gchar *Icon = "GtkWindow::icon";

/* These are deprecated. We don't show them any more. */
static gchar *Shrink = "GtkWindow::allow_shrink";
static gchar *Grow = "GtkWindow::allow_grow";


static const gchar *GbTypeChoices[] =
{"Top Level", "Popup", NULL};
static const gint GbTypeValues[] =
{
  GTK_WINDOW_TOPLEVEL,
  GTK_WINDOW_POPUP
};
static const gchar *GbTypeSymbols[] =
{
  "GTK_WINDOW_TOPLEVEL",
  "GTK_WINDOW_POPUP"
};

static const gchar *GbPositionChoices[] =
{"None", "Center", "Mouse", NULL};
static const gint GbPositionValues[] =
{
  GTK_WIN_POS_NONE,
  GTK_WIN_POS_CENTER,
  GTK_WIN_POS_MOUSE
};
static const gchar *GbPositionSymbols[] =
{
  "GTK_WIN_POS_NONE",
  "GTK_WIN_POS_CENTER",
  "GTK_WIN_POS_MOUSE"
};


static void on_toggle_default_width (GtkWidget * widget, gpointer value);
static void on_toggle_default_height (GtkWidget * widget, gpointer value);

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the function in the GbWidget struct at the end of this file.
 ******/

/*
 * Creates a new GtkWidget of class GtkWindow, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */
GtkWidget *
gb_window_new (GbWidgetNewData * data)
{
  GtkWidget *new_widget, *placeholder;

  new_widget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (new_widget), data->name);
  gtk_window_set_policy (GTK_WINDOW (new_widget), FALSE, TRUE, FALSE);
  gtk_signal_connect (GTK_OBJECT (new_widget), "delete_event",
		      GTK_SIGNAL_FUNC (editor_close_window), NULL);

  /* We need to size the placeholder or the window is very small. */
  if (data->action != GB_LOADING)
    {
      placeholder = editor_new_placeholder ();
      gtk_widget_set_usize (placeholder, 400, 300);
      gtk_container_add (GTK_CONTAINER (new_widget), placeholder);
    }

  return new_widget;
}



/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_window_create_properties (GtkWidget * widget, GbWidgetCreateArgData * data)
{
  gb_window_create_standard_properties (widget, data,
					Title, Type, Position, Modal,
					DefaultWidth, DefaultHeight,
					Shrink, Grow, NULL,
					WMName, WMClass,
					Resizable, DestroyWithParent, Icon);
}


void
gb_window_create_standard_properties (GtkWidget * widget,
				      GbWidgetCreateArgData * data,
				      gchar *title_p,
				      gchar *type_p,
				      gchar *position_p,
				      gchar *modal_p,
				      gchar *default_width_p,
				      gchar *default_height_p,
				      gchar *shrink_p,
				      gchar *grow_p,
				      gchar *auto_shrink_p,
				      gchar *wmname_p,
				      gchar *wmclass_p,
				      gchar *resizable_p,
				      gchar *destroy_with_parent_p,
				      gchar *icon_p)
{
  if (title_p)
    property_add_string (title_p, _("Title:"), _("The title of the window"));

  if (type_p)
    property_add_choice (type_p, _("Type:"), _("The type of the window"),
			 GbTypeChoices);

  if (position_p)
    property_add_choice (position_p, _("Position:"),
			 _("The initial position of the window"),
			 GbPositionChoices);

  if (modal_p)
    property_add_bool (modal_p, _("Modal:"), _("If the window is modal"));

  if (default_width_p && default_height_p)
    {
      property_add_optional_int_range (default_width_p,
				       _("Default Width:"),
				       _("The default width of the window"),
				       0, 10000, 1, 10, 1,
				       on_toggle_default_width);
      property_add_optional_int_range (default_height_p,
				       _("Default Height:"),
				       _("The default height of the window"),
				       0, 10000, 1, 10, 1,
				       on_toggle_default_height);
    }

  if (resizable_p)
    property_add_bool (resizable_p, _("Resizable:"),
		       _("If the window can be resized"));

#if 0
  /* These are deprecated. */
  if (shrink_p && grow_p)
    {
      property_add_bool (shrink_p, _("Shrink:"),
			 _("If the window can be shrunk"));
      property_add_bool (grow_p, _("Grow:"),
			 _("If the window can be enlarged"));
    }
#endif

#if 0
  /* These aren't necessary, and have been used incorrectly for ages. */
  if (wmname_p)
    property_add_string (wmname_p, _("WM Name:"),
			 _("The name to pass to the window manager"));
  if (wmclass_p)
    property_add_string (wmclass_p, _("WM Class:"),
			 _("The class name to pass to the window manager"));
#endif

  if (destroy_with_parent_p)
    property_add_bool (destroy_with_parent_p, _("Auto-Destroy:"),
		       _("If the window is destroyed when its transient parent is destroyed"));

  if (icon_p)
    property_add_filename (icon_p, _("Icon:"),
			   _("The icon for this window"));
}


static void
on_toggle_default_width (GtkWidget * widget, gpointer value)
{
  GtkWidget *property_widget;
  gboolean value_set;
  gint width, height;

  property_widget = property_get_widget ();
  if (property_widget == NULL)
    return;

  value_set = GTK_TOGGLE_BUTTON (widget)->active ? TRUE : FALSE;
  gtk_widget_set_sensitive (GTK_WIDGET (value), value_set);

  if (value_set)
    {
      width = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (value));
      gtk_object_set_data (GTK_OBJECT (property_widget), DefaultWidth,
			   GINT_TO_POINTER (width));
    }
  else
    {
      width = -1;
      gtk_object_set_data (GTK_OBJECT (property_widget), DefaultWidth,
			   NULL);
    }

  height = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (property_widget),
						 DefaultHeight));
  gtk_window_set_default_size (GTK_WINDOW (property_widget),
			       width, height != 0 ? height : -1);
}


static void
on_toggle_default_height (GtkWidget * widget, gpointer value)
{
  GtkWidget *property_widget;
  gboolean value_set;
  gint width, height;

  property_widget = property_get_widget ();
  if (property_widget == NULL)
    return;

  value_set = GTK_TOGGLE_BUTTON (widget)->active ? TRUE : FALSE;
  gtk_widget_set_sensitive (GTK_WIDGET (value), value_set);

  if (value_set)
    {
      height = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (value));
      gtk_object_set_data (GTK_OBJECT (property_widget), DefaultHeight,
			   GINT_TO_POINTER (height));
    }
  else
    {
      height = -1;
      gtk_object_set_data (GTK_OBJECT (property_widget), DefaultHeight,
			   NULL);
    }

   width = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (property_widget),
						 DefaultWidth));
   gtk_window_set_default_size (GTK_WINDOW (property_widget),
				width != 0 ? width : -1, height);
}


/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_window_get_properties (GtkWidget * widget, GbWidgetGetArgData * data)
{
  gb_window_get_standard_properties (widget, data,
				     Title, Type, Position, Modal,
				     DefaultWidth, DefaultHeight,
				     Shrink, Grow, NULL,
				     WMName, WMClass,
				     Resizable, DestroyWithParent, Icon);
}


void
gb_window_get_standard_properties (GtkWidget * widget,
				   GbWidgetGetArgData * data,
				   gchar *title_p,
				   gchar *type_p,
				   gchar *position_p,
				   gchar *modal_p,
				   gchar *default_width_p,
				   gchar *default_height_p,
				   gchar *shrink_p,
				   gchar *grow_p,
				   gchar *auto_shrink_p,
				   gchar *wmname_p,
				   gchar *wmclass_p,
				   gchar *resizable_p,
				   gchar *destroy_with_parent_p,
				   gchar *icon_p)
{
  gint type, position, default_width, default_height;

  if (title_p)
    gb_widget_output_translatable_string (data, title_p,
					  GTK_WINDOW (widget)->title);

  /* The type is stored in the object datalist as we can't change it after the
     window is realized. It will default to 0, which is OK. */
  if (type_p)
    {
      type = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
						   type_p));
      gb_widget_output_choice (data, type_p, type, GbTypeSymbols[type]);
    }

  /* The position is stored in the object datalist so that it doesn't affect
     the displaying of the window within Glade. It will default to 0, i.e.
     GTK_WIN_POS_NONE, which is OK. */
  if (position_p)
    {
      position = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
						       position_p));
      gb_widget_output_choice (data, position_p, position,
			       GbPositionSymbols[position]);
    }

  if (modal_p)
    {
      gb_widget_output_bool (data, modal_p,
			     gtk_object_get_data (GTK_OBJECT (widget), modal_p)
			     != NULL ? TRUE : FALSE);
    }

  if (default_width_p && default_height_p)
    {
      default_width = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
							    DefaultWidth));
      gb_widget_output_optional_int (data, default_width_p, default_width,
				     default_width != 0 ? TRUE : FALSE);

      default_height = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
							     DefaultHeight));
      gb_widget_output_optional_int (data, default_height_p, default_height,
				     default_height != 0 ? TRUE : FALSE);
    }

#if 0
  /* These are deprecated. */
  if (grow_p && shrink_p)
    {
      gb_widget_output_bool (data, grow_p,
			     GTK_WINDOW (widget)->allow_grow);
      gb_widget_output_bool (data, shrink_p,
			     GTK_WINDOW (widget)->allow_shrink);
    }
#endif

  /* These are stored in the object hash since we can't set them after the
     window is realized. */
#if 0
  /* These aren't necessary, and have been used incorrectly for ages. */
  if (wmname_p)
    gb_widget_output_string (data, wmname_p,
			     gtk_object_get_data (GTK_OBJECT (widget),
						  wmname_p));
  if (wmclass_p)
    gb_widget_output_string (data, wmclass_p,
			     gtk_object_get_data (GTK_OBJECT (widget),
						  wmclass_p));
#endif

  if (resizable_p)
    gb_widget_output_bool (data, resizable_p,
			   gtk_window_get_resizable (GTK_WINDOW (widget)));

  if (destroy_with_parent_p)
    gb_widget_output_bool (data, destroy_with_parent_p,
			   gtk_window_get_destroy_with_parent (GTK_WINDOW (widget)));

  if (icon_p)
    gb_widget_output_pixmap_filename (data, icon_p,
				      gtk_object_get_data (GTK_OBJECT (widget),
							   icon_p));
}



/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_window_set_properties (GtkWidget * widget, GbWidgetSetArgData * data)
{
  gb_window_set_standard_properties (widget, data,
				     Title, Type, Position, Modal,
				     DefaultWidth, DefaultHeight,
				     Shrink, Grow, NULL,
				     WMName, WMClass,
				     Resizable, DestroyWithParent, Icon);
}


void
gb_window_set_standard_properties (GtkWidget * widget,
				   GbWidgetSetArgData * data,
				   gchar *title_p,
				   gchar *type_p,
				   gchar *position_p,
				   gchar *modal_p,
				   gchar *default_width_p,
				   gchar *default_height_p,
				   gchar *shrink_p,
				   gchar *grow_p,
				   gchar *auto_shrink_p,
				   gchar *wmname_p,
				   gchar *wmclass_p,
				   gchar *resizable_p,
				   gchar *destroy_with_parent_p,
				   gchar *icon_p)
{
  gchar *title, *type, *position;
  gint default_width, default_height, i;
  gboolean modal, apply_default_width, apply_default_height;
  gboolean resizable, destroy_with_parent;
#if 0
  gchar *wmname, *wmclass;
#endif

  if (title_p)
    {
      title = gb_widget_input_string (data, title_p);
      if (data->apply)
	gtk_window_set_title (GTK_WINDOW (widget), title);
    }

  if (type_p)
    {
      type = gb_widget_input_choice (data, type_p);
      if (data->apply)
	{
	  for (i = 0; i < sizeof (GbTypeValues) / sizeof (GbTypeValues[0]);
	       i++)
	    {
	      if (!strcmp (type, GbTypeChoices[i])
		  || !strcmp (type, GbTypeSymbols[i]))
		{
		  gtk_object_set_data (GTK_OBJECT (widget), type_p,
				       GINT_TO_POINTER (i));
		  break;
		}
	    }
	}
    }

  if (position_p)
    {
      position = gb_widget_input_choice (data, position_p);
      if (data->apply)
	{
	  for (i = 0;
	       i < sizeof (GbPositionValues) / sizeof (GbPositionValues[0]);
	       i++)
	    {
	      if (!strcmp (position, GbPositionChoices[i])
		  || !strcmp (position, GbPositionSymbols[i]))
		{
		  gtk_object_set_data (GTK_OBJECT (widget), position_p,
				       GINT_TO_POINTER (i));
		  break;
		}
	    }
	}
    }

  if (modal_p)
    {
      modal = gb_widget_input_bool (data, modal_p);
      if (data->apply)
	{
	  gtk_object_set_data (GTK_OBJECT (widget), modal_p,
			       modal ? "TRUE" : NULL);
	}
    }

  if (default_width_p && default_height_p)
    {
      default_width = gb_widget_input_int (data, default_width_p);
      apply_default_width = data->apply;
      if (apply_default_width)
	{
	  gtk_object_set_data (GTK_OBJECT (widget), DefaultWidth,
			       GINT_TO_POINTER (default_width));
	}

      default_height = gb_widget_input_int (data, default_height_p);
      apply_default_height = data->apply;
      if (apply_default_height)
	{
	  gtk_object_set_data (GTK_OBJECT (widget), DefaultHeight,
			       GINT_TO_POINTER (default_height));
	}

      if (apply_default_width || apply_default_height)
	{
	  if (!apply_default_width)
	    default_width = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
								  DefaultWidth));
	  if (!apply_default_height)
	    default_height = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
								   DefaultHeight));
	  gtk_window_set_default_size (GTK_WINDOW (widget),
				       default_width ? default_width : -1,
				       default_height ? default_height : -1);
	}
    }

#if 0
  /* These are deprecated. */
  if (shrink_p && grow_p)
    {
      shrink = gb_widget_input_bool (data, shrink_p);
      if (data->apply)
	set_policy = TRUE;
      else
	shrink = GTK_WINDOW (widget)->allow_shrink;

      grow = gb_widget_input_bool (data, grow_p);
      if (data->apply)
	set_policy = TRUE;
      else
	grow = GTK_WINDOW (widget)->allow_grow;

      if (set_policy)
	gtk_window_set_policy (GTK_WINDOW (widget), shrink, grow, FALSE);
    }
#endif

#if 0
  /* These aren't necessary, and have been used incorrectly for ages. */
  if (wmname_p)
    {
      wmname = gb_widget_input_string (data, wmname_p);
      if (wmname && wmname[0] == '\0')
	wmname = NULL;
      if (data->apply)
	{
	  gtk_object_set_data_full (GTK_OBJECT (widget), wmname_p,
				    g_strdup (wmname), wmname ? g_free : NULL);
	}
    }

  if (wmclass_p)
    {
      wmclass = gb_widget_input_string (data, wmclass_p);
      if (wmclass && wmclass[0] == '\0')
	wmclass = NULL;
      if (data->apply)
	{
	  gtk_object_set_data_full (GTK_OBJECT (widget), wmclass_p,
				    g_strdup (wmclass),
				    wmclass ? g_free : NULL);
	}
    }
#endif

  if (resizable_p)
    {
      resizable = gb_widget_input_bool (data, resizable_p);
      if (data->apply)
	gtk_window_set_resizable (GTK_WINDOW (widget), resizable);
    }

  if (destroy_with_parent_p)
    {
      destroy_with_parent = gb_widget_input_bool (data, destroy_with_parent_p);
      if (data->apply)
	gtk_window_set_destroy_with_parent (GTK_WINDOW (widget),
					    destroy_with_parent);
    }

  if (icon_p)
    {
      char *filename = gb_widget_input_pixmap_filename (data, icon_p);
      if (data->apply)
	{
	  char *old_filename;

	  if (filename && filename[0] == '\0')
	    filename = NULL;

	  /* Remove the old pixmap from the project. */
	  old_filename = gtk_object_get_data (GTK_OBJECT (widget), icon_p);
	  glade_project_remove_pixmap (data->project, old_filename);

	  gtk_object_set_data_full (GTK_OBJECT (widget), icon_p,
				    g_strdup (filename),
				    filename ? g_free : NULL);

	  glade_project_add_pixmap (data->project, filename);

	  if (filename)
	    {
	      GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
	      gtk_window_set_icon (GTK_WINDOW (widget), pixbuf);
	      if (pixbuf)
		gdk_pixbuf_unref (pixbuf);
	    }
	}
      if (data->action == GB_LOADING)
	g_free (filename);
    }
}



/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a GtkWindow, with signals pointing to
 * other functions in this file.
 */
/*
   static void
   gb_window_create_popup_menu(GtkWidget *widget, GbWidgetCreateMenuData *data)
   {

   }
 */



/*
 * Writes the source code needed to create this widget.
 * You have to output everything necessary to create the widget here, though
 * there are some convenience functions to help.
 */
static void
gb_window_write_source (GtkWidget * widget, GbWidgetWriteSourceData * data)
{
  gint type;

  if (data->create_widget)
    {
      type = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget), Type));
      source_add (data, "  %s = gtk_window_new (%s);\n", data->wname,
		  GbTypeSymbols[type]);
    }

  gb_widget_write_standard_source (widget, data);

  /* The type is already set above, so we pass NULL to skip it. */
  gb_window_write_standard_source (widget, data,
				   Title, NULL, Position, Modal,
				   DefaultWidth, DefaultHeight,
				   Shrink, Grow, NULL,
				   WMName, WMClass,
				   Resizable, DestroyWithParent, Icon);
}


void
gb_window_write_standard_source (GtkWidget * widget,
				 GbWidgetWriteSourceData * data,
				 gchar *title_p,
				 gchar *type_p,
				 gchar *position_p,
				 gchar *modal_p,
				 gchar *default_width_p,
				 gchar *default_height_p,
				 gchar *shrink_p,
				 gchar *grow_p,
				 gchar *auto_shrink_p,
				 gchar *wmname_p,
				 gchar *wmclass_p,
				 gchar *resizable_p,
				 gchar *destroy_with_parent_p,
				 gchar *icon_p)
{
  gint type, position, default_width, default_height;
#if 0
  gchar *wmname, *wmclass;
#endif

  if (title_p)
    {
      if (GTK_WINDOW (widget)->title
	  && strlen (GTK_WINDOW (widget)->title) > 0)
	source_add (data, "  gtk_window_set_title (GTK_WINDOW (%s), %s);\n",
		    data->wname,
		    source_make_string (GTK_WINDOW (widget)->title,
					data->use_gettext));
    }

  if (type_p)
    {
      type = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
						   type_p));
      if (type != GTK_WINDOW_TOPLEVEL)
	{
	  /* Note: there is no gtk_window_set_type () */
	  source_add (data, "  GTK_WINDOW (%s)->type = %s;\n",
		      data->wname, GbTypeSymbols[type]);
	}
    }

  if (position_p)
    {
      position = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
						       position_p));
      if (GbPositionValues[position] != GTK_WIN_POS_NONE)
	{
	  source_add (data,
		      "  gtk_window_set_position (GTK_WINDOW (%s), %s);\n",
		      data->wname, GbPositionSymbols[position]);
	}
    }

  if (modal_p)
    {
      if (gtk_object_get_data (GTK_OBJECT (widget), modal_p))
	{
	  source_add (data,
		      "  gtk_window_set_modal (GTK_WINDOW (%s), TRUE);\n",
		      data->wname);
	}
    }

  if (default_width_p && default_height_p)
    {
      default_width = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
							    DefaultWidth));
      default_height = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
							     DefaultHeight));
      if (default_width || default_height )
	{
	  source_add (data,
		      "  gtk_window_set_default_size (GTK_WINDOW (%s), %i, %i);\n",
		      data->wname,
		      default_width ? default_width : -1,
		      default_height ? default_height : -1);
	}
    }

#if 0
  if (shrink_p && grow_p)
    {
      if (GTK_WINDOW (widget)->allow_grow != TRUE
	  || GTK_WINDOW (widget)->allow_shrink != FALSE)
	source_add (data,
		    "  gtk_window_set_policy (GTK_WINDOW (%s), %s, %s, %s);\n",
		    data->wname,
		    GTK_WINDOW (widget)->allow_shrink ? "TRUE" : "FALSE",
		    GTK_WINDOW (widget)->allow_grow ? "TRUE" : "FALSE",
		    "FALSE");
    }
#endif

#if 0
  /* These aren't necessary, and have been used incorrectly for ages. */
  if (wmname_p && wmclass_p)
    {
      wmname = gtk_object_get_data (GTK_OBJECT (widget), wmname_p);
      wmclass = gtk_object_get_data (GTK_OBJECT (widget), wmclass_p);
      if (wmname || wmclass)
	{
	  source_add (data,
		      "  gtk_window_set_wmclass (GTK_WINDOW (%s), %s,",
		      data->wname,
		      wmname ? source_make_string (wmname, FALSE) : "\"\"");
	  source_add (data, " %s);\n",
		      wmclass ? source_make_string (wmclass, FALSE) : "\"\"");
	}
    }
#endif

  if (resizable_p)
    {
      if (!gtk_window_get_resizable (GTK_WINDOW (widget)))
	source_add (data,
		    "  gtk_window_set_resizable (GTK_WINDOW (%s), FALSE);\n",
		    data->wname);
    }

  if (destroy_with_parent_p)
    {
      if (gtk_window_get_destroy_with_parent (GTK_WINDOW (widget)))
	source_add (data,
	"  gtk_window_set_destroy_with_parent (GTK_WINDOW (%s), TRUE);\n",
		    data->wname);
    }

  if (icon_p)
    {
      gchar *filename = gtk_object_get_data (GTK_OBJECT (widget), icon_p);
      if (filename && *filename)
	{
	  char *pixbuf_name = g_strdup_printf ("%s_icon_pixbuf", data->wname);

	  source_add_decl (data, "  GdkPixbuf *%s;\n", pixbuf_name);
	  source_create_pixbuf (data, pixbuf_name, filename);
	  source_add (data,
		      "  if (%s)\n"
		      "    {\n"
		      "      gtk_window_set_icon (GTK_WINDOW (%s), %s);\n"
		      "      gdk_pixbuf_unref (%s);\n"
		      "    }\n",
		      pixbuf_name,
		      data->wname, pixbuf_name,
		      pixbuf_name);

	  g_free (pixbuf_name);
	}
    }
}


/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */
GbWidget *
gb_window_init ()
{
  /* Initialise the GTK type */
  volatile GtkType type;
  type = gtk_window_get_type ();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct (&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = window_xpm;
  gbwidget.tooltip = _("Window");

  /* Fill in any functions that this GbWidget has */
  gbwidget.gb_widget_new = gb_window_new;
  gbwidget.gb_widget_create_properties = gb_window_create_properties;
  gbwidget.gb_widget_get_properties = gb_window_get_properties;
  gbwidget.gb_widget_set_properties = gb_window_set_properties;
  gbwidget.gb_widget_write_source = gb_window_write_source;
/*
   gbwidget.gb_widget_create_popup_menu = gb_window_create_popup_menu;
 */

  return &gbwidget;
}
