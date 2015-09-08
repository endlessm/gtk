/*
 * Copyright © 2015 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Benjamin Otte <otte@gnome.org>
 */

#include "config.h"

#include "gtkcssgadgetprivate.h"

#include <math.h>

#include "gtkcssnumbervalueprivate.h"
#include "gtkcssshadowsvalueprivate.h"
#include "gtkcssstyleprivate.h"
#include "gtkcssstylepropertyprivate.h"
#include "gtkrenderbackgroundprivate.h"
#include "gtkrenderborderprivate.h"

typedef struct _GtkCssGadgetPrivate GtkCssGadgetPrivate;
struct _GtkCssGadgetPrivate {
  GtkCssNode *node;
  GtkWidget *owner;
};

enum {
  PROP_0,
  PROP_NODE,
  PROP_OWNER,
  /* add more */
  NUM_PROPERTIES
};

static GParamSpec *properties[NUM_PROPERTIES];

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (GtkCssGadget, gtk_css_gadget, G_TYPE_OBJECT,
                                  G_ADD_PRIVATE (GtkCssGadget))

static void
gtk_css_gadget_real_get_preferred_size (GtkCssGadget   *gadget,
                                        GtkOrientation  orientation,
                                        gint            for_size,
                                        gint           *minimum,
                                        gint           *natural,
                                        gint           *minimum_baseline,
                                        gint           *natural_baseline)
{
  *minimum = 0;
  *natural = 0;

  if (minimum_baseline)
    *minimum_baseline = 0;
  if (natural_baseline)
    *natural_baseline = 0;
}

static void
gtk_css_gadget_real_allocate (GtkCssGadget        *gadget,
                              const GtkAllocation *allocation,
                              int                  baseline,
                              GtkAllocation       *out_clip)
{
  *out_clip = *allocation;
}

static gboolean
gtk_css_gadget_real_draw (GtkCssGadget *gadget,
                          cairo_t      *cr,
                          int           width,
                          int           height)
{
  return FALSE;
}

static void
gtk_css_gadget_get_property (GObject    *object,
                             guint       property_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (GTK_CSS_GADGET (object));

  switch (property_id)
  {
    case PROP_NODE:
      g_value_set_object (value, priv->node);
      break;

    case PROP_OWNER:
      g_value_set_object (value, priv->owner);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
gtk_css_gadget_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  GtkCssGadget *gadget = GTK_CSS_GADGET (object);
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);

  switch (property_id)
  {
    case PROP_NODE:
      priv->node = g_value_dup_object (value);
      if (priv->node == NULL)
        priv->node = gtk_css_node_new ();
      break;

    case PROP_OWNER:
      priv->owner = g_value_get_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
gtk_css_gadget_finalize (GObject *object)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (GTK_CSS_GADGET (object));

  g_clear_object (&priv->node);

  G_OBJECT_CLASS (gtk_css_gadget_parent_class)->finalize (object);
}

static void
gtk_css_gadget_class_init (GtkCssGadgetClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = gtk_css_gadget_get_property;
  object_class->set_property = gtk_css_gadget_set_property;
  object_class->finalize = gtk_css_gadget_finalize;

  klass->get_preferred_size = gtk_css_gadget_real_get_preferred_size;
  klass->allocate = gtk_css_gadget_real_allocate;
  klass->draw = gtk_css_gadget_real_draw;

  properties[PROP_NODE] = g_param_spec_object ("node", "Node",
                                               "CSS node",
                                               GTK_TYPE_CSS_NODE,
                                               G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
                                               G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_OWNER] = g_param_spec_object ("owner", "Owner",
                                                "Widget that created and owns this gadget",
                                                GTK_TYPE_WIDGET,
                                                G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
                                                G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);


  g_object_class_install_properties (object_class, NUM_PROPERTIES, properties);
}

static void
gtk_css_gadget_init (GtkCssGadget *gadget)
{

}

GtkCssNode *
gtk_css_gadget_get_node (GtkCssGadget *gadget)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);

  return priv->node;
}

GtkCssStyle *
gtk_css_gadget_get_style (GtkCssGadget *gadget)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);

  return gtk_css_node_get_style (priv->node);
}

GtkWidget *
gtk_css_gadget_get_owner (GtkCssGadget *gadget)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);

  return priv->owner;
}

void
gtk_css_gadget_add_class (GtkCssGadget *gadget,
                          const char   *name)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);
  GQuark quark;

  quark = g_quark_from_string (name);

  gtk_css_node_add_class (priv->node, quark);
}

void
gtk_css_gadget_remove_class (GtkCssGadget *gadget,
                             const char   *name)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);
  GQuark quark;

  quark = g_quark_try_string (name);
  if (quark == 0)
    return;

  gtk_css_node_remove_class (priv->node, quark);
}

static gint
get_number (GtkCssStyle *style,
            guint        property)
{
  double d = _gtk_css_number_value_get (gtk_css_style_get_value (style, property), 100);

  if (d < 1)
    return ceil (d);
  else
    return floor (d);
}

static void
get_box_margin (GtkCssStyle *style,
                GtkBorder   *margin)
{
  margin->top = get_number (style, GTK_CSS_PROPERTY_MARGIN_TOP);
  margin->left = get_number (style, GTK_CSS_PROPERTY_MARGIN_LEFT);
  margin->bottom = get_number (style, GTK_CSS_PROPERTY_MARGIN_BOTTOM);
  margin->right = get_number (style, GTK_CSS_PROPERTY_MARGIN_RIGHT);
}

static void
get_box_border (GtkCssStyle *style,
                GtkBorder   *border)
{
  border->top = get_number (style, GTK_CSS_PROPERTY_BORDER_TOP_WIDTH);
  border->left = get_number (style, GTK_CSS_PROPERTY_BORDER_LEFT_WIDTH);
  border->bottom = get_number (style, GTK_CSS_PROPERTY_BORDER_BOTTOM_WIDTH);
  border->right = get_number (style, GTK_CSS_PROPERTY_BORDER_RIGHT_WIDTH);
}

static void
get_box_padding (GtkCssStyle *style,
                 GtkBorder   *border)
{
  border->top = get_number (style, GTK_CSS_PROPERTY_PADDING_TOP);
  border->left = get_number (style, GTK_CSS_PROPERTY_PADDING_LEFT);
  border->bottom = get_number (style, GTK_CSS_PROPERTY_PADDING_BOTTOM);
  border->right = get_number (style, GTK_CSS_PROPERTY_PADDING_RIGHT);
}

void
gtk_css_gadget_get_preferred_size (GtkCssGadget   *gadget,
                                   GtkOrientation  orientation,
                                   gint            for_size,
                                   gint           *minimum,
                                   gint           *natural,
                                   gint           *minimum_baseline,
                                   gint           *natural_baseline)
{
  GtkCssStyle *style;
  GtkBorder margin, border, padding;
  int min_size, extra_size, extra_opposite, extra_baseline;

  style = gtk_css_gadget_get_style (gadget);
  get_box_margin (style, &margin);
  get_box_border (style, &border);
  get_box_padding (style, &padding);
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      extra_size = margin.left + margin.right + border.left + border.right + padding.left + padding.right;
      extra_opposite = margin.top + margin.bottom + border.top + border.bottom + padding.top + padding.bottom;
      extra_baseline = margin.left + border.left + padding.left;
      min_size = get_number (style, GTK_CSS_PROPERTY_MIN_WIDTH);
    }
  else
    {
      extra_size = margin.top + margin.bottom + border.top + border.bottom + padding.top + padding.bottom;
      extra_opposite = margin.left + margin.right + border.left + border.right + padding.left + padding.right;
      extra_baseline = margin.top + border.top + padding.top;
      min_size = get_number (style, GTK_CSS_PROPERTY_MIN_HEIGHT);
    }

  if (for_size > -1)
    for_size -= extra_opposite;

  if (minimum_baseline)
    *minimum_baseline = -1;
  if (natural_baseline)
    *natural_baseline = -1;

  GTK_CSS_GADGET_GET_CLASS (gadget)->get_preferred_size (gadget,
                                                         orientation,
                                                         for_size,
                                                         minimum, natural,
                                                         minimum_baseline, natural_baseline);

  g_warn_if_fail (*minimum <= *natural);

  *minimum = MAX (min_size, *minimum);
  *natural = MAX (min_size, *natural);

  *minimum += extra_size;
  *natural += extra_size;

  if (minimum_baseline && *minimum_baseline > -1)
    *minimum_baseline += extra_baseline;
  if (natural_baseline && *natural_baseline > -1)
    *natural_baseline += extra_baseline;
}

void
gtk_css_gadget_allocate (GtkCssGadget        *gadget,
                         const GtkAllocation *allocation,
                         int                  baseline,
                         GtkAllocation       *out_clip)
{
  GtkAllocation content_allocation;
  GtkBorder margin, border, padding, shadow, extents;
  GtkCssStyle *style;

  g_return_if_fail (out_clip != NULL);

  style = gtk_css_gadget_get_style (gadget);
  get_box_margin (style, &margin);
  get_box_border (style, &border);
  get_box_padding (style, &padding);
  extents.top = margin.top + border.top + padding.top;
  extents.right = margin.right + border.right + padding.right;
  extents.bottom = margin.bottom + border.bottom + padding.bottom;
  extents.left = margin.left + border.left + padding.left;

  content_allocation.x = allocation->x + extents.left;
  content_allocation.y = allocation->y + extents.top;
  content_allocation.width = allocation->width - extents.left - extents.right;
  content_allocation.height = allocation->height - extents.top - extents.bottom;
  if (baseline >= 0)
    baseline += extents.top;

  g_assert (content_allocation.width >= 0);
  g_assert (content_allocation.height >= 0);

  GTK_CSS_GADGET_GET_CLASS (gadget)->allocate (gadget, &content_allocation, baseline, out_clip);

  _gtk_css_shadows_value_get_extents (gtk_css_style_get_value (style, GTK_CSS_PROPERTY_BOX_SHADOW), &shadow);
  out_clip->x -= extents.left + shadow.left - margin.left;
  out_clip->y -= extents.top + shadow.top - margin.top;
  out_clip->width += extents.left + extents.right + shadow.left + shadow.right - margin.left - margin.right;
  out_clip->height += extents.top + extents.bottom + shadow.top + shadow.bottom - margin.top - margin.bottom;
}

void
gtk_css_gadget_draw (GtkCssGadget *gadget,
                     cairo_t      *cr,
                     int           width,
                     int           height)
{
  GtkCssGadgetPrivate *priv = gtk_css_gadget_get_instance_private (gadget);
  GtkBorder margin, border, padding;
  gboolean draw_focus;
  GtkCssStyle *style;
  int contents_width, contents_height;

  style = gtk_css_gadget_get_style (gadget);
  get_box_margin (style, &margin);
  get_box_border (style, &border);
  get_box_padding (style, &padding);

  gtk_css_style_render_background (style,
                                   cr,
                                   margin.left,
                                   margin.top,
                                   width - margin.left - margin.right,
                                   height - margin.top - margin.bottom,
                                   gtk_css_node_get_junction_sides (priv->node));
  gtk_css_style_render_border (style,
                               cr,
                               margin.left,
                               margin.top,
                               width - margin.left - margin.right,
                               height - margin.top - margin.bottom,
                               0,
                               gtk_css_node_get_junction_sides (priv->node));

  cairo_translate (cr,
                   margin.left + border.left + padding.left,
                   margin.top + border.top + padding.top);
  contents_width = width - margin.left - margin.right - border.left - border.right - padding.left - padding.right;
  contents_height = height - margin.top - margin.bottom - border.top - border.bottom - padding.top - padding.bottom;

  draw_focus = GTK_CSS_GADGET_GET_CLASS (gadget)->draw (gadget, cr, contents_width, contents_height);

  cairo_translate (cr,
                   - (margin.left + border.left + padding.left),
                   - (margin.top + border.top + padding.top));

  if (draw_focus)
    gtk_css_style_render_outline (style,
                                  cr,
                                  margin.left,
                                  margin.top,
                                  width - margin.left - margin.right,
                                  height - margin.top - margin.bottom);
}

void
gtk_css_node_style_changed_for_widget (GtkCssNode  *node,
                                       GtkCssStyle *old_style,
                                       GtkCssStyle *new_style,
                                       GtkWidget    *widget)
{
  static GtkBitmask *affects_size = NULL;
  GtkBitmask *changes;
  
  changes = _gtk_bitmask_new ();
  changes = gtk_css_style_add_difference (changes, old_style, new_style);

  if (G_UNLIKELY (affects_size == NULL))
    affects_size = _gtk_css_style_property_get_mask_affecting (GTK_CSS_AFFECTS_SIZE | GTK_CSS_AFFECTS_CLIP);

  if (_gtk_bitmask_intersects (changes, affects_size))
    gtk_widget_queue_resize (widget);
  else
    gtk_widget_queue_draw (widget);

  _gtk_bitmask_free (changes);
}

