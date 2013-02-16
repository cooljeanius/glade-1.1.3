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

#include <config.h>

#ifdef USE_GNOME
#include <gnome.h>
#include "../glade_gnome.h"
#else
#include <gtk/gtkbutton.h>
#include <gtk/gtkimage.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkmain.h>
#include <gtk/gtkmenu.h>
#include <gtk/gtkmenuitem.h>
#include <gtk/gtkradiobutton.h>
#include <gtk/gtkspinbutton.h>
#include <gtk/gtktoolbar.h>
#include <gtk/gtkvbox.h>
#endif

#include "../gb.h"

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/toolbar.xpm"

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;

static const gchar *Size = "GtkToolbar::size";
static const gchar *Orientation = "GtkToolbar::orientation";
static const gchar *Style = "GtkToolbar::toolbar_style";
static const gchar *Tooltips = "GtkToolbar::tooltips";

/* For children of a toolbar */
static const gchar *GbNewToolbarGroup = "GtkToolbarChild::new_group";


static const gchar *GbOrientationChoices[] =
{"Horizontal", "Vertical", NULL};
static const gint GbOrientationValues[] =
{
  GTK_ORIENTATION_HORIZONTAL,
  GTK_ORIENTATION_VERTICAL
};
static const gchar *GbOrientationSymbols[] =
{
  "GTK_ORIENTATION_HORIZONTAL",
  "GTK_ORIENTATION_VERTICAL"
};

static const gchar *GbStyleChoices[] =
{"Icons", "Text", "Both", "Both Horizontal", NULL};
static const gint GbStyleValues[] =
{
  GTK_TOOLBAR_ICONS,
  GTK_TOOLBAR_TEXT,
  GTK_TOOLBAR_BOTH,
  GTK_TOOLBAR_BOTH_HORIZ
};
static const gchar *GbStyleSymbols[] =
{
  "GTK_TOOLBAR_ICONS",
  "GTK_TOOLBAR_TEXT",
  "GTK_TOOLBAR_BOTH",
  "GTK_TOOLBAR_BOTH_HORIZ"
};


static void show_toolbar_dialog (GbWidgetNewData * data);
static void on_toolbar_dialog_ok (GtkWidget * widget,
				  GbWidgetNewData * data);
static void on_toolbar_dialog_destroy (GtkWidget * widget,
				       GbWidgetNewData * data);

static void update_toolbar_size (GtkWidget * widget, gint size);

static void gb_toolbar_insert_before (GtkWidget * menuitem,
				      GtkWidget * child);
static void gb_toolbar_insert_after (GtkWidget * menuitem,
				     GtkWidget * child);
static void gb_toolbar_insert (GtkWidget * child,
			       gint offset);

static void gb_toolbar_set_new_toolbar_group	(GtkWidget * toolbar,
						 GtkWidget * widget,
						 gboolean new_group);
static void gb_toolbar_convert_toolbar_button (GtkWidget * menuitem,
					       GtkWidget * widget);

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the function in the GbWidget struct at the end of this file.
 ******/

/*
 * Creates a new GtkWidget of class GtkToolbar, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */
GtkWidget*
gb_toolbar_new(GbWidgetNewData *data)
{
  GtkWidget *new_widget;

  if (data->action == GB_LOADING)
    {
      new_widget = gtk_toolbar_new ();
      return new_widget;
    }
  else
    {
      show_toolbar_dialog (data);
      return NULL;
    }
}


void
gb_toolbar_add_child (GtkWidget *widget, GtkWidget *child, GbWidgetSetArgData *data)
{
  gchar *class_name;
  gboolean new_group;

  /* Tell the load functions to use the child properties array. */
  data->loading_type = GB_CHILD_PROPERTIES;
  new_group = load_bool (data, "new_group");
  data->loading_type = GB_STANDARD_PROPERTIES;

  if (new_group)
    gtk_toolbar_append_space (GTK_TOOLBAR (widget));

  /* See if this is a special toolbar button. */
  if (data->widget_info)
    class_name = data->widget_info->class;

  if (class_name && (!strcmp (class_name, "button")
		     || !strcmp (class_name, "toggle")
		     || !strcmp (class_name, "radio")))
    gb_toolbar_insert_toolbar_child (GTK_TOOLBAR (widget), child, -1);
  else
    gtk_toolbar_append_widget (GTK_TOOLBAR (widget), child, NULL, NULL);
}


static void
show_toolbar_dialog (GbWidgetNewData * data)
{
  GtkWidget *dialog, *vbox, *hbox, *label, *spinbutton;
  GtkObject *adjustment;

  dialog = glade_util_create_dialog (_("New toolbar"), data->parent,
				     GTK_SIGNAL_FUNC (on_toolbar_dialog_ok),
				     data, &vbox);
  gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
		      GTK_SIGNAL_FUNC (on_toolbar_dialog_destroy), data);

  hbox = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
  gtk_widget_show (hbox);

  label = gtk_label_new (_("Number of items:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 5);
  gtk_widget_show (label);

  adjustment = gtk_adjustment_new (3, 1, 100, 1, 10, 10);
  spinbutton = glade_util_spin_button_new (GTK_OBJECT (dialog), "items",
					   GTK_ADJUSTMENT (adjustment), 1, 0);
  gtk_box_pack_start (GTK_BOX (hbox), spinbutton, TRUE, TRUE, 5);
  gtk_widget_set_usize (spinbutton, 50, -1);
  gtk_widget_grab_focus (spinbutton);
  gtk_widget_show (spinbutton);

  gtk_widget_show (dialog);
  gtk_grab_add (dialog);
}


static void
on_toolbar_dialog_ok (GtkWidget * widget, GbWidgetNewData * data)
{
  GtkWidget *new_widget, *spinbutton, *window, *placeholder;
  gint items, i;

  window = gtk_widget_get_toplevel (widget);

  /* Only call callback if placeholder/fixed widget is still there */
  if (gb_widget_can_finish_new (data))
    {
      spinbutton = gtk_object_get_data (GTK_OBJECT (window), "items");
      g_return_if_fail (spinbutton != NULL);
      items = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinbutton));

      new_widget = gtk_toolbar_new ();
      for (i = 0; i < items; i++)
	{
	  placeholder = editor_new_placeholder ();
	  gtk_toolbar_append_widget (GTK_TOOLBAR (new_widget), placeholder,
				     NULL, NULL);
	}
      gb_widget_initialize (new_widget, data);
      (*data->callback) (new_widget, data);
    }
  gtk_widget_destroy (window);
}


static void
on_toolbar_dialog_destroy (GtkWidget * widget,
			   GbWidgetNewData * data)
{
  gb_widget_free_new_data (data);
  gtk_grab_remove (widget);
}


/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_toolbar_create_properties (GtkWidget * widget, GbWidgetCreateArgData * data)
{
  property_add_int_range (Size, _("Size:"),
			  _("The number of items in the toolbar"),
			  0, 1000, 1, 10, 1);
  property_add_choice (Orientation, _("Orientation:"),
		       _("The toolbar orientation"),
		       GbOrientationChoices);
  property_add_choice (Style, _("Style:"),
		       _("The toolbar style"),
		       GbStyleChoices);
  property_add_bool (Tooltips, _("Tooltips:"), _("If tooltips are enabled"));
}



/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_toolbar_get_properties (GtkWidget * widget, GbWidgetGetArgData * data)
{
  gint i;

  if (data->action != GB_SAVING)
    gb_widget_output_int (data, Size, GTK_TOOLBAR (widget)->num_children);

  for (i = 0; i < sizeof (GbOrientationValues)
	 / sizeof (GbOrientationValues[0]); i++)
    {
      if (GbOrientationValues[i] == GTK_TOOLBAR (widget)->orientation)
	gb_widget_output_choice (data, Orientation, i, GbOrientationSymbols[i]);
    }

  for (i = 0; i < sizeof (GbStyleValues) / sizeof (GbStyleValues[0]); i++)
    {
      if (GbStyleValues[i] == GTK_TOOLBAR (widget)->style)
	gb_widget_output_choice (data, Style, i, GbStyleSymbols[i]);
    }

  gb_widget_output_bool (data, Tooltips,
		    GTK_TOOLTIPS (GTK_TOOLBAR (widget)->tooltips)->enabled);
}



/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_toolbar_set_properties (GtkWidget * widget, GbWidgetSetArgData * data)
{
  gchar *orientation, *style;
  gint i, size;
  gboolean tooltips;

  size = gb_widget_input_int (data, Size);
  if (data->apply)
    update_toolbar_size (widget, size);

  orientation = gb_widget_input_choice (data, Orientation);
  if (data->apply)
    {
      for (i = 0; i < sizeof (GbOrientationValues)
	   / sizeof (GbOrientationValues[0]); i++)
	{
	  if (!strcmp (orientation, GbOrientationChoices[i])
	      || !strcmp (orientation, GbOrientationSymbols[i]))
	    {
	      gtk_toolbar_set_orientation (GTK_TOOLBAR (widget),
					   GbOrientationValues[i]);
	      break;
	    }
	}
    }

  style = gb_widget_input_choice (data, Style);
  if (data->apply)
    {
      for (i = 0; i < sizeof (GbStyleValues) / sizeof (GbStyleValues[0]); i++
	)
	{
	  if (!strcmp (style, GbStyleChoices[i])
	      || !strcmp (style, GbStyleSymbols[i]))
	    {
	      /* This avoids any problems with redrawing the selection. */
	      if (data->action == GB_APPLYING)
		editor_clear_selection (NULL);
	      gtk_toolbar_set_style (GTK_TOOLBAR (widget), GbStyleValues[i]);
	      /*editor_refresh_widget (widget);*/
	      break;
	    }
	}
    }

  tooltips = gb_widget_input_bool (data, Tooltips);
  if (data->apply)
    gtk_toolbar_set_tooltips (GTK_TOOLBAR (widget), tooltips);
}


/* This updates the toolbar size to the given value, adding buttons or
   deleting items as necessary. */
static void
update_toolbar_size (GtkWidget * widget, gint size)
{
  GtkWidget *placeholder;
  gint current_size = GTK_TOOLBAR (widget)->num_children;

  if (current_size < size)
    {
      /* FIXME: This avoids any problems with redrawing the selection. */
      editor_clear_selection (NULL);

      while (current_size < size)
	{
	  placeholder = editor_new_placeholder ();
	  gtk_toolbar_append_widget (GTK_TOOLBAR (widget), placeholder,
				     NULL, NULL);
	  current_size++;
	}
    }
  else if (current_size > size)
    {
      GList *item = g_list_nth (GTK_TOOLBAR (widget)->children, size);
      while (item)
	{
	  GtkToolbarChild *child = (GtkToolbarChild*) item->data;
	  item = item->next;
	  if (child->type == GTK_TOOLBAR_CHILD_SPACE)
	    {
	      /* Note that we are always removing the element at size. */
	      gtk_toolbar_remove_space (GTK_TOOLBAR (widget), size);
	    }
	  else
	    {
	      gtk_container_remove (GTK_CONTAINER (widget), child->widget);
	    }
	}
    }
}


/*
 * Creates the child packing properties for children of this widget.
 */
static void
gb_toolbar_create_child_properties (GtkWidget * widget,
				    GbWidgetCreateChildArgData * data)
{
  property_add_bool (GbNewToolbarGroup, _("New Group:"),
		     _("Set True to start a new group of toolbar items"));
}


static void
gb_toolbar_get_child_properties (GtkWidget *widget, GtkWidget *child,
				 GbWidgetGetArgData *data)
{
  GtkToolbarChild *tchild;
  gint pos = 0;

  tchild = glade_util_find_toolbar_child (widget, child, &pos, NULL);
  if (pos == 0)
    {
      if (data->action == GB_SHOWING)
	{
	  gb_widget_output_bool (data, GbNewToolbarGroup, FALSE);
	  property_set_sensitive (GbNewToolbarGroup, FALSE);
	}
    }
  else
    {
      gboolean new_group = gb_toolbar_get_new_toolbar_group (widget, child);
      if (data->action == GB_SHOWING)
	{
	  property_set_sensitive (GbNewToolbarGroup, TRUE);
	  gb_widget_output_bool (data, GbNewToolbarGroup, new_group);
	}
      else if (new_group && data->action == GB_SAVING)
	{
	  save_start_tag (data, "packing");
	  gb_widget_output_bool (data, GbNewToolbarGroup, new_group);
	  save_end_tag (data, "packing");
	}
    }
}


static void
gb_toolbar_set_child_properties (GtkWidget *widget, GtkWidget *child,
				 GbWidgetSetArgData *data)
{
  gboolean new_group = gb_widget_input_bool (data, GbNewToolbarGroup);
  if (data->apply)
    gb_toolbar_set_new_toolbar_group (widget, child, new_group);
}

/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a GtkToolbar, with signals pointing to
 * other functions in this file.
 */
static void
gb_toolbar_create_popup_menu(GtkWidget *widget, GbWidgetCreateMenuData *data)
{
  GtkWidget *menuitem;
  GList *child_element;
  GtkToolbarChild *child;

  if (data->child == NULL)
    return;

  /* Find the toolbar child corresponding to the button the mouse is over. */
  child = glade_util_find_toolbar_child (widget, data->child, NULL, &child_element);

  /* Commands for inserting new items. */
  if (child)
    {
      menuitem = gtk_menu_item_new_with_label (_("Insert Item Before"));
      gtk_widget_show (menuitem);
      gtk_container_add (GTK_CONTAINER (data->menu), menuitem);
      gtk_signal_connect (GTK_OBJECT (menuitem), "activate",
			  GTK_SIGNAL_FUNC (gb_toolbar_insert_before),
			  data->child);

      menuitem = gtk_menu_item_new_with_label (_("Insert Item After"));
      gtk_widget_show (menuitem);
      gtk_container_add (GTK_CONTAINER (data->menu), menuitem);
      gtk_signal_connect (GTK_OBJECT (menuitem), "activate",
			  GTK_SIGNAL_FUNC (gb_toolbar_insert_after),
			  data->child);
    }
}


static void
gb_toolbar_insert_before (GtkWidget * menuitem, GtkWidget * child)
{
  gb_toolbar_insert (child, -1);
}


static void
gb_toolbar_insert_after (GtkWidget * menuitem, GtkWidget * child)
{
  gb_toolbar_insert (child, 1);
}


static void
gb_toolbar_insert (GtkWidget * child, gint offset)
{
  GtkWidget *toolbar = child->parent, *placeholder;
  GtkToolbarChild *tbchild;
  gint pos;

  tbchild = glade_util_find_toolbar_child (toolbar, child, &pos, NULL);
  g_return_if_fail (tbchild != NULL);

  if (offset > 0)
    pos++;

  placeholder = editor_new_placeholder ();
  gtk_toolbar_insert_widget (GTK_TOOLBAR (toolbar), placeholder,
			     NULL, NULL, pos);
}





/*************************************************************************
 * Toolbar helper functions.
 *************************************************************************/

/* This returns TRUE if the widget is a special toolbar button, i.e. it
   has an icon and label. */
gboolean
gb_toolbar_is_toolbar_button (GtkWidget *widget)
{
  gchar *child_name = gb_widget_get_child_name (widget);
  return (child_name && !strcmp (child_name, GladeChildToolbarButton));
}


gboolean
gb_toolbar_get_new_toolbar_group	(GtkWidget	       *toolbar,
				 GtkWidget	       *widget)
{
  GtkToolbarChild *tbchild, *prev_tbchild;
  GList *elem;

  tbchild = glade_util_find_toolbar_child (toolbar, widget, NULL, &elem);
  g_return_val_if_fail (tbchild != NULL, FALSE);

  if (elem->prev == NULL)
    return FALSE;

  prev_tbchild = (GtkToolbarChild*) elem->prev->data;
  if (prev_tbchild->type == GTK_TOOLBAR_CHILD_SPACE)
    return TRUE;

  return FALSE;
}


/* This sets whether or not a toolbar child starts a new group on the toolbar,
   i.e. there is a space before it. */
static void
gb_toolbar_set_new_toolbar_group (GtkWidget	       *toolbar,
				 GtkWidget	       *widget,
				 gboolean		new_group)
{
  GtkToolbarChild *tbchild, *prev_tbchild;
  gint pos;
  GList *elem;

  tbchild = glade_util_find_toolbar_child (toolbar, widget, &pos, &elem);
  g_return_if_fail (tbchild != NULL);

  if (elem->prev == NULL)
    return;

  prev_tbchild = (GtkToolbarChild*) elem->prev->data;

  if (new_group)
    {
      if (prev_tbchild->type != GTK_TOOLBAR_CHILD_SPACE)
	{
	  gtk_toolbar_insert_space (GTK_TOOLBAR (toolbar), pos);
	}
    }
  else
    {
      if (prev_tbchild->type == GTK_TOOLBAR_CHILD_SPACE)
	{
	  gtk_toolbar_remove_space (GTK_TOOLBAR (toolbar), pos - 1);
	}
    }
}


void
gb_toolbar_create_toolbar_button_popup_menu (GtkWidget	           *widget,
					     GbWidgetCreateMenuData *data)
{
  GtkWidget *menuitem;

  if (gb_toolbar_is_toolbar_button (widget))
    {
      menuitem = gtk_menu_item_new_with_label (_("Convert to Ordinary Button"));
      gtk_widget_show (menuitem);
      gtk_container_add (GTK_CONTAINER (data->menu), menuitem);
      gtk_signal_connect (GTK_OBJECT (menuitem), "activate",
			  GTK_SIGNAL_FUNC (gb_toolbar_convert_toolbar_button),
			  widget);
    }
}


static void
gb_toolbar_convert_toolbar_button (GtkWidget * menuitem,
				   GtkWidget * widget)
{
  GtkWidget *button;
  GtkToolbarChild *child;

  g_return_if_fail (widget->parent != NULL);
  child = glade_util_find_toolbar_child (widget->parent, widget, NULL, NULL);
  g_return_if_fail (child != NULL);

  /* We set the parent to NULL when calling gb_widget_new, otherwise it
     creates a new toolbar button! */
  if (child->type == GTK_TOOLBAR_CHILD_BUTTON)
    button = gb_widget_new ("GtkButton", NULL);
  else if (child->type == GTK_TOOLBAR_CHILD_TOGGLEBUTTON)
    button = gb_widget_new ("GtkToggleButton", NULL);
  else if (child->type == GTK_TOOLBAR_CHILD_RADIOBUTTON)
    button = gb_widget_new ("GtkRadioButton", NULL);
  else
    {
      g_warning ("Invalid toolbar button");
      return;
    }

  /* Set properties widget to NULL, in case the widget or parent is replaced */
  property_set_widget (NULL);

  gb_widget_replace_child (widget->parent, widget, button);
}


GtkWidget *
gb_toolbar_new_toolbar_button (GbWidgetNewData *data,
			       GtkToolbarChildType type)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (data->parent);
  GtkWidget *new_widget, *box, *icon, *label;
  GbWidget * pixmap_gbwidget;

  /* If we are loading, we check that a toolbar button is needed. */
  MSG ("In gb_widget_new_toolbar_button");
  if (data->action == GB_LOADING)
    {
      gchar *class_name;

      if (data->loading_data->widget_info)
	class_name = data->loading_data->widget_info->class;

      if (!class_name)
	return NULL;

      if (strcmp (class_name, "button")
	  && strcmp (class_name, "toggle")
	  && strcmp (class_name, "radio"))
	return NULL;
    }

  if (type == GTK_TOOLBAR_CHILD_BUTTON)
    {
      new_widget = gtk_button_new ();
    }
  else if (type == GTK_TOOLBAR_CHILD_TOGGLEBUTTON)
    {
      new_widget = gtk_toggle_button_new ();
      gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (new_widget), FALSE);
    }
  else
    {
      new_widget = gtk_radio_button_new (NULL);
      gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (new_widget), FALSE);
    }

  /* Remember it is a special widget. */
  gb_widget_set_child_name (new_widget, GladeChildToolbarButton);

  GTK_WIDGET_UNSET_FLAGS (new_widget, GTK_CAN_FOCUS);
  if (toolbar->style == GTK_TOOLBAR_BOTH_HORIZ)
    box = gtk_hbox_new (FALSE, 0);
  else
    box = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (new_widget), box);
  gtk_widget_show (box);

  label = gtk_label_new (data->name);
  gtk_box_pack_end (GTK_BOX (box), label, FALSE, FALSE, 0);
  if (toolbar->style != GTK_TOOLBAR_ICONS)
    gtk_widget_show (label);

  pixmap_gbwidget = gb_widget_lookup_class ("GtkImage");
  if (pixmap_gbwidget)
    {
      icon = gtk_image_new_from_pixmap (pixmap_gbwidget->gdkpixmap,
					pixmap_gbwidget->mask);
      gtk_box_pack_end (GTK_BOX (box), icon, FALSE, FALSE, 0);
      if (toolbar->style != GTK_TOOLBAR_TEXT)
	gtk_widget_show (icon);
    }
  else
    g_warning ("Couldn't find GtkPixmap data");

  return new_widget;
}


/* SPECIAL CODE: This inserts a proper toolbar button (with an icon and label)
   into a toolbar at the given position. We need it since GtkToolbar creates
   and inserts the widget all in one go, which isn't good enough for us,
   as we want to set the events mask to add signal handlers to the buttons.
   This also sets the radio button group, if it is a radio button. */
void
gb_toolbar_insert_toolbar_child (GtkToolbar *toolbar,
				 GtkWidget *new_child,
				 gint position)
{
  GtkToolbarChild *child;
  GtkWidget *icon, *label;
  GtkToolbarChildType type;

  MSGIN;

  /* Determine if the new child is a proper toolbar button, i.e. it has
     a vbox with a pixmap and a label in it. If it isn't we just add it as
     a standard widget. */
  if (!gb_toolbar_is_toolbar_button (new_child))
    {
      gtk_toolbar_insert_widget (toolbar, new_child, NULL, NULL, position);
      return;
    }

  if (!gb_button_toolbar_find_child_widgets (new_child, &label, &icon))
    return;
  if (GTK_IS_RADIO_BUTTON (new_child))
    type = GTK_TOOLBAR_CHILD_RADIOBUTTON;
  else if (GTK_IS_TOGGLE_BUTTON (new_child))
    type = GTK_TOOLBAR_CHILD_TOGGLEBUTTON;
  else
    type = GTK_TOOLBAR_CHILD_BUTTON;

  child = g_new (GtkToolbarChild, 1);
  child->widget = new_child;
  child->icon = icon;
  child->label = label;
  child->type = type;

  /* This is mainly copied from gtktoolbar.c */
  toolbar->children = g_list_insert (toolbar->children, child, position);
  toolbar->num_children++;

  gtk_widget_set_parent (child->widget, GTK_WIDGET (toolbar));
}


/*************************************************************************
 * END Toolbar helper functions.
 *************************************************************************/


/*
 * Writes the source code needed to create this widget.
 * You have to output everything necessary to create the widget here, though
 * there are some convenience functions to help.
 */
static void
gb_toolbar_write_source (GtkWidget * widget, GbWidgetWriteSourceData * data)
{
  const gchar *orientation = GbOrientationSymbols[0];
  const gchar *style = GbStyleSymbols[0];
  gint i;

  for (i = 0;
       i < sizeof (GbOrientationValues) / sizeof (GbOrientationValues[0]); i++)
    {
      if (GbOrientationValues[i] == GTK_TOOLBAR (widget)->orientation)
	orientation = GbOrientationSymbols[i];
    }

  for (i = 0; i < sizeof (GbStyleValues) / sizeof (GbStyleValues[0]); i++)
    {
      if (GbStyleValues[i] == GTK_TOOLBAR (widget)->style)
	style = GbStyleSymbols[i];
    }

  if (data->create_widget)
    {
      source_add (data, "  %s = gtk_toolbar_new ();\n", data->wname);
    }

  gb_widget_write_standard_source (widget, data);

  /* We have to set the style explicitly for now, as the default style depends
     on the user's theme settings. Ideally GTK+ should have a
     GTK_TOOLBAR_DEFAULT style. */
  source_add (data,
	      "  gtk_toolbar_set_style (GTK_TOOLBAR (%s), %s);\n",
	      data->wname, style);

  if (GTK_TOOLBAR (widget)->orientation != GTK_ORIENTATION_HORIZONTAL)
    source_add (data,
		"  gtk_toolbar_set_orientation (GTK_TOOLBAR (%s), %s);\n",
		data->wname, orientation);

  if (!GTK_TOOLTIPS (GTK_TOOLBAR (widget)->tooltips)->enabled)
    source_add (data,
		"  gtk_toolbar_set_tooltips (GTK_TOOLBAR (%s), FALSE);\n",
		data->wname);
}


/* Outputs source to add a child widget to a hbox/vbox. */
static void
gb_toolbar_write_add_child_source (GtkWidget * parent,
				   const gchar *parent_name,
				   GtkWidget *child,
				   GbWidgetWriteSourceData * data)
{
  /* For toolbar buttons, the widgets are added to the toolbar when
     created, so we skip this. For standard widgets, we add them here. */
  if (!gb_toolbar_is_toolbar_button (child))
    {
      if (gb_toolbar_get_new_toolbar_group (parent, child))
	source_add (data,
		    "  gtk_toolbar_append_space (GTK_TOOLBAR (%s));\n\n",
		    parent_name);

      source_add (data,
		  "  gtk_toolbar_append_widget (GTK_TOOLBAR (%s), %s, %s, NULL);\n",
		  parent_name, data->wname,
		  data->widget_data->tooltip ? source_make_string (data->widget_data->tooltip, data->use_gettext) : "NULL");
    }
}


/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */
GbWidget *
gb_toolbar_init ()
{
  /* Initialise the GTK type */
  volatile GtkType type;
  type = gtk_toolbar_get_type ();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct (&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = toolbar_xpm;
  gbwidget.tooltip = _("Toolbar");

  /* Fill in any functions that this GbWidget has */
   gbwidget.gb_widget_new               = gb_toolbar_new;
  gbwidget.gb_widget_add_child = gb_toolbar_add_child;
  gbwidget.gb_widget_create_properties = gb_toolbar_create_properties;
  gbwidget.gb_widget_get_properties = gb_toolbar_get_properties;
  gbwidget.gb_widget_set_properties = gb_toolbar_set_properties;
  gbwidget.gb_widget_create_child_properties = gb_toolbar_create_child_properties;
  gbwidget.gb_widget_get_child_properties = gb_toolbar_get_child_properties;
  gbwidget.gb_widget_set_child_properties = gb_toolbar_set_child_properties;
  gbwidget.gb_widget_create_popup_menu = gb_toolbar_create_popup_menu;
  gbwidget.gb_widget_write_source = gb_toolbar_write_source;
  gbwidget.gb_widget_write_add_child_source = gb_toolbar_write_add_child_source;

  return &gbwidget;
}
