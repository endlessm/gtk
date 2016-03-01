/*
 * Copyright © 2016 Endless Mobile Inc.
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
 * Authors: Matthew Watson <mattdangerw@gmail.com>
 */

#ifndef __GTK_ANIMATION_HELPER_PRIVATE_H__
#define __GTK_ANIMATION_HELPER_PRIVATE_H__

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GtkAnimationHelper GtkAnimationHelper;

struct _GtkAnimationHelper
{
  gboolean is_running;
  guint start_time;
  guint end_time;
  double progress;
};

void                 gtk_animation_helper_start        (GtkAnimationHelper *helper,
                                                        guint duration);

void                 gtk_animation_helper_finish       (GtkAnimationHelper *helper);

void                 gtk_animation_helper_next_frame   (GtkAnimationHelper *helper,
                                                        guint frame_time);

gboolean             gtk_animation_helper_is_running   (GtkAnimationHelper *helper);

gdouble              gtk_animation_helper_get_progress (GtkAnimationHelper *helper);

G_END_DECLS

#endif /* __GTK_ANIMATION_HELPER_PRIVATE_H__ */
