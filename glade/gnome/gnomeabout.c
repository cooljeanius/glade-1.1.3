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

#include <gnome.h>
#include "../gb.h"

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/gnome-about.xpm"

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;

static gchar *Logo = "GnomeAbout::logo";
static gchar *Copyright = "GnomeAbout::copyright";
static gchar *Comments = "GnomeAbout::comments";

static gchar *Authors = "GnomeAbout::authors";
static gchar *Documenters = "GnomeAbout::documenters";
static gchar *TranslatorCredits = "GnomeAbout::translator_credits";

static gchar *WMName = "GnomeAbout|GtkWindow::wmclass_name";
static gchar *WMClass = "GnomeAbout|GtkWindow::wmclass_class";
static gchar *DestroyWithParent = "GnomeAbout|GtkWindow::destroy_with_parent";
static gchar *Icon = "GnomeAbout|GtkWindow::icon";

#define GLADE_TRANSLATORS_STRING "translator_credits"

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the funtion in the GbWidget struct at the end of this file.
 ******/

/* Pinched from gnome-about.c */
#define GNOME_RESPONSE_CREDITS 1

static void
gb_gnome_about_response_cb (GtkDialog *dialog, gint response, gpointer data)
{
  /* We only let the response signal continue if it is going to show the
     Credits sub-dialog. Otherwise we stop the signal. If the window close
     button is pressed, our standard delete-event handler will hide the
     dialog. But we must ensure the dialog isn't destroyed when it is closed.
  */
  if (response != GNOME_RESPONSE_CREDITS) {
    g_signal_stop_emission_by_name (dialog, "response");
  }
}


/*
 * Creates a new GtkWidget of class GnomeAbout, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */
static GtkWidget*
gb_gnome_about_new (GbWidgetNewData *data)
{
  GtkWidget *new_widget;
  gchar *project_name;
  const gchar *authors[] = { NULL };

  project_name = glade_project_get_name (data->project);
  new_widget = gnome_about_new (project_name ? project_name : "",
				"x.x", NULL, NULL, authors, authors, NULL, NULL);

  /* Make the About dialog modal by default. */
  /* We can't do this in GNOME 2, as the Credits sub-dialog won't work. */
  /*gtk_object_set_data (GTK_OBJECT (new_widget), Modal, "TRUE");*/

  /* We connect a close signal handler which always returns TRUE so that
     the built-in close functionality is skipped. */
  gtk_signal_connect (GTK_OBJECT (new_widget), "response",
		      GTK_SIGNAL_FUNC (gb_gnome_about_response_cb), NULL);

  /* Now we connect our normal delete_event handler. */
  gtk_signal_connect (GTK_OBJECT (new_widget), "delete_event",
		      GTK_SIGNAL_FUNC (editor_close_window), NULL);

  return new_widget;
}


/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_gnome_about_create_properties (GtkWidget * widget, GbWidgetCreateArgData * data)
{
  gb_window_create_standard_properties (widget, data,
					NULL, NULL, NULL, NULL,
					NULL, NULL, NULL, NULL, NULL,
					WMName, WMClass,
					NULL, DestroyWithParent, Icon);

  property_add_filename (Logo, _("Logo:"), _("The pixmap to use as the logo"));
  property_add_text (Copyright, _("Copyright:"), _("The copyright notice"), 2);
  property_add_text (Comments, _("Comments:"), _("Additional information, such as a description of the package and its home page on the web"), 2);

  property_add_text (Authors, _("Authors:"), _("The authors of the package, one on each line"), 2);
  property_add_text (Documenters, _("Documenters:"), _("The documenters of the package, one on each line"), 2);
  property_add_text (TranslatorCredits, _("Translators:"), _("The translators of the package. This should normally be left empty so that translators can add their names in the po files"), 2);
}


/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_gnome_about_get_properties (GtkWidget *widget, GbWidgetGetArgData * data)
{
  char *translator_credits, *copyright, *comments;

  gb_window_get_standard_properties (widget, data,
				     NULL, NULL, NULL, NULL,
				     NULL, NULL, NULL, NULL, NULL,
				     WMName, WMClass,
				     NULL, DestroyWithParent, Icon);

  g_object_get (G_OBJECT (widget),
		"copyright", &copyright,
		"comments", &comments,
		"translator-credits", &translator_credits,
		NULL);

  gb_widget_output_pixmap_filename (data, Logo,
				    gtk_object_get_data (GTK_OBJECT (widget),
							 Logo));
  gb_widget_output_translatable_text (data, Copyright, copyright);
  gb_widget_output_translatable_text (data, Comments, comments);

  gb_widget_output_text (data, Authors,
			 gtk_object_get_data (GTK_OBJECT (widget), Authors));
  gb_widget_output_text (data, Documenters,
			 gtk_object_get_data (GTK_OBJECT (widget),
					      Documenters));

  /* If we are saving, and translator_credits isn't set, we save the string
     "translator_credits" and mark it as translatable. Translators can then
     replace this with their own names in the po files. If it is set, we
     output it as it is and don't mark it as translatable. */
  if (data->action == GB_SAVING
      && (!translator_credits || *translator_credits == '\0'))
    {
    gb_widget_output_translatable_text (data, TranslatorCredits,
					GLADE_TRANSLATORS_STRING);
    }
  else
    {
      gb_widget_output_text (data, TranslatorCredits, translator_credits);
    }

  g_free (copyright);
  g_free (comments);
  g_free (translator_credits);
}

static GValueArray *
carray_from_text (GbWidgetSetArgData *data, GtkWidget *widget, const char *key)
{
  GValueArray *array = NULL;
  char *str;

  str = gb_widget_input_text (data, key);
  if (data->apply)
    {
      char **strv;
      int i;

      gtk_object_set_data_full (GTK_OBJECT (widget), key, g_strdup (str),
				str ? g_free : NULL);

      strv = g_strsplit (str, "\n", 0);
      array = g_value_array_new (0);

      for (i = 0; strv[i]; i++) {
	GValue value = { 0 };
	g_value_init (&value, G_TYPE_STRING);
	g_value_set_string (&value, strv[i]);
	array = g_value_array_append (array, &value);
      }

      g_strfreev (strv);
    }
  if (data->action == GB_APPLYING)
    g_free (str);

  return array;
}

/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_gnome_about_set_properties (GtkWidget * widget, GbWidgetSetArgData * data)
{
  GValueArray *authors, *documenters;
  gchar *copyright, *comments, *translators;
  gchar *filename, *old_filename;
  GdkPixbuf *logo = NULL;
  GObject *object;
  gboolean redraw = FALSE;

  object = G_OBJECT (widget);

  gb_window_set_standard_properties (widget, data,
				     NULL, NULL, NULL, NULL,
				     NULL, NULL, NULL, NULL, NULL,
				     WMName, WMClass,
				     NULL, DestroyWithParent, Icon);

  filename = gb_widget_input_pixmap_filename (data, Logo);
  if (data->apply)
    {
      if (filename && filename[0] == '\0')
	filename = NULL;

      old_filename = gtk_object_get_data (GTK_OBJECT (widget), Logo);
      glade_project_remove_pixmap (data->project, old_filename);
      gtk_object_set_data_full (GTK_OBJECT (widget), Logo,
				g_strdup (filename), filename ? g_free : NULL);
      glade_project_add_pixmap (data->project, filename);
      logo = filename ? gdk_pixbuf_new_from_file (filename, NULL) : NULL;
      g_object_set (object, "logo", logo, NULL);
      if (logo)
	g_object_unref (logo);
    }
  if (data->action == GB_LOADING)
    g_free (filename);

  copyright = gb_widget_input_text (data, Copyright);
  if (data->apply)
    {
      g_object_set (object, "copyright", copyright, NULL);
      redraw = TRUE;
    }
  if (data->action == GB_APPLYING)
    g_free (copyright);

  comments = gb_widget_input_text (data, Comments);  
  if (data->apply)
    {
      g_object_set (object, "comments", comments, NULL);
      redraw = TRUE;
    }
  if (data->action == GB_APPLYING)
    g_free (comments);


  translators = gb_widget_input_text (data, TranslatorCredits);
  if (data->apply)
    {
      if (!strcmp (translators, GLADE_TRANSLATORS_STRING))
	g_object_set (object, "translator-credits", "", NULL);
      else
	g_object_set (object, "translator-credits", translators, NULL);
    }
  if (data->action == GB_APPLYING)
    g_free (translators);

  authors = carray_from_text (data, widget, Authors);
  if (data->apply)
    g_object_set (object, "authors", authors, NULL);
  if (authors)
    g_value_array_free (authors);

  documenters = carray_from_text (data, widget, Documenters);
  if (data->apply)
    g_object_set (object, "documenters", documenters, NULL);
  if (documenters)
    g_value_array_free (documenters);

  /* If the window is resized the selection rectangles mess it up, so we
     queue a redraw. */
  if (redraw)
    gtk_widget_queue_draw (widget);
}


/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a GnomeAbout, with signals pointing to
 * other functions in this file.
 */
/*
static void
gb_gnome_about_create_popup_menu (GtkWidget * widget, GbWidgetCreateMenuData * data)
{

}
*/



/*
 * Writes the source code needed to create this widget.
 * You have to output everything necessary to create the widget here, though
 * there are some convenience functions to help.
 */
static void
gb_gnome_about_write_source (GtkWidget * widget, GbWidgetWriteSourceData * data)
{
  if (data->create_widget)
    {
      gchar *project_name = glade_project_get_name (data->project);
      char *copyright, *comments, *translators;
      char *authors, **authorsv, *documenters, **documentersv;
      gchar *logo_filename;
      char *pixbuf_name = NULL;
      gint i;

      g_object_get (G_OBJECT (widget),
		    "copyright", &copyright,
		    "comments", &comments,
		    "translator-credits", &translators,
		    NULL);

      authors = gtk_object_get_data (GTK_OBJECT (widget), Authors);
      documenters = gtk_object_get_data (GTK_OBJECT (widget), Documenters);

      /* Output the array of authors. */
      if (authors && *authors)
	{
	  source_add_decl (data, "  const gchar *authors[] = {\n");
	  authorsv = g_strsplit (authors, "\n", 0);
	  for (i = 0; authorsv[i]; i++)
	    {
	      if (authorsv[i][0])
		source_add_decl (data, "    %s,\n",
				 source_make_string (authorsv[i], FALSE));
	    }
	  source_add_decl (data, "    NULL\n  };\n");
	  g_strfreev (authorsv);
	}
      else
	{
	  source_add_decl (data, "  const gchar *authors[] = { NULL };\n");
	}

      /* Output the array of documenters. */
      if (documenters && *documenters)
	{
	  source_add_decl (data, "  const gchar *documenters[] = {\n");
	  documentersv = g_strsplit (documenters, "\n", 0);
	  for (i = 0; documentersv[i]; i++)
	    {
	      if (documentersv[i][0])
		source_add_decl (data, "    %s,\n",
				 source_make_string (documentersv[i], FALSE));
	    }
	  source_add_decl (data, "    NULL\n  };\n");
	  g_strfreev (documentersv);
	}
      else
	{
	  source_add_decl (data, "  const gchar *documenters[] = { NULL };\n");
	}

      /* If translators is set, then we use that specific string without
	 translation. If it is not set or empty, then if gettext support is
	 enable we output code to call gettext to translate the special
	 "translator_credits" string, and use that if it is translated. */
      if (translators && translators[0])
	{
	  source_add_decl (data, "  gchar *translators = %s;\n",
			   source_make_string (translators, FALSE));
	}
      else if (data->use_gettext)
	{
	  source_add_decl (data,
			   "  /* TRANSLATORS: Replace this string with your names, one name per line. */\n"
			   "  gchar *translators = _(\"%s\");\n",
			   GLADE_TRANSLATORS_STRING);

	  source_add (data,
		      "  if (!strcmp (translators, \"%s\"))\n"
		      "    translators = NULL;\n",
		      GLADE_TRANSLATORS_STRING);
	}
      else
	{
	  source_add_decl (data, "  gchar *translators = NULL;\n");
	}

      logo_filename = gtk_object_get_data (GTK_OBJECT (widget), Logo);
      if (logo_filename && logo_filename[0])
	{
	  pixbuf_name = g_strdup_printf ("%s_logo_pixbuf", data->wname);

	  source_add_decl (data, "  GdkPixbuf *%s;\n", pixbuf_name);
	  source_create_pixbuf (data, pixbuf_name, logo_filename);
	}

      /* Now output the call to gnome_about_new(). We do it in pieces since
	 source_make_string() returns a pointer to a single static buffer. */
      source_add (data,
		  "  %s = gnome_about_new (%s, VERSION,\n",
		  data->wname,
		  source_make_string (project_name ? project_name : "",
				      FALSE));
      source_add (data,
		  "                        %s,\n",
		  source_make_string (copyright ? copyright : "",
				      data->use_gettext));
      source_add (data,
		  "                        %s,\n",
		  source_make_string (comments ? comments : "",
				      data->use_gettext));
      source_add (data,
		  "                        authors,\n"
		  "                        documenters,\n"
		  "                        translators,\n");

      source_add (data,
		  "                        %s);\n",
		  pixbuf_name ? pixbuf_name : "NULL");

      g_free (pixbuf_name);
      g_free (copyright);
      g_free (comments);
      g_free (translators);
    }

  gb_widget_write_standard_source (widget, data);

  gb_window_write_standard_source (widget, data,
				   NULL, NULL, NULL, NULL,
				   NULL, NULL, NULL, NULL, NULL,
				   WMName, WMClass,
				   NULL, DestroyWithParent, Icon);
}


void
gb_gnome_about_destroy (GtkWidget * widget,
			GbWidgetDestroyData * data)
{
  gchar *filename;

  filename = gtk_object_get_data (GTK_OBJECT (widget), Logo);
  glade_project_remove_pixmap (data->project, filename);
}


/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */
GbWidget*
gb_gnome_about_init ()
{
  /* Initialise the GTK type */
  volatile GtkType type;
  type = gnome_about_get_type();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct(&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = gnome_about_xpm;
  gbwidget.tooltip = _("Gnome About Dialog");

  /* Fill in any functions that this GbWidget has */
  gbwidget.gb_widget_new		= gb_gnome_about_new;
  gbwidget.gb_widget_create_properties	= gb_gnome_about_create_properties;
  gbwidget.gb_widget_get_properties	= gb_gnome_about_get_properties;
  gbwidget.gb_widget_set_properties	= gb_gnome_about_set_properties;
  gbwidget.gb_widget_write_source	= gb_gnome_about_write_source;
  gbwidget.gb_widget_destroy		= gb_gnome_about_destroy;
/*
  gbwidget.gb_widget_create_popup_menu	= gb_gnome_about_create_popup_menu;
*/

  return &gbwidget;
}

