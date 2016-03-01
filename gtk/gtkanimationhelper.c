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

#include "gtkanimationhelperprivate.h"

void
gtk_animation_helper_start (GtkAnimationHelper *helper,
                            guint duration)
{
  helper->is_running = TRUE;
  helper->progress = 0.0;
  /* We will not actually record a start time until our first frame has been
   * recorded. */
  helper->start_time = 0;
  helper->end_time = duration;
}

void
gtk_animation_helper_finish (GtkAnimationHelper *helper)
{
  helper->is_running = FALSE;
}

void
gtk_animation_helper_next_frame (GtkAnimationHelper *helper,
                                 guint frame_time)
{
  if (!helper->is_running)
    return;

  if (helper->start_time == 0)
    {
      helper->start_time += frame_time;
      helper->end_time += frame_time;
    }

  if (frame_time >= helper->end_time)
    {
      helper->is_running = FALSE;
      return;
    }

  helper->progress = (frame_time - helper->start_time) / (double) (helper->end_time - helper->start_time);
}

gboolean
gtk_animation_helper_is_running (GtkAnimationHelper *helper)
{
  return helper->is_running;
}

gdouble
gtk_animation_helper_get_progress (GtkAnimationHelper *helper)
{
  return helper->is_running ? helper->progress : 1.0;
}
