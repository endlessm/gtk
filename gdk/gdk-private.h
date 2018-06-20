#ifndef __GDK__PRIVATE_H__
#define __GDK__PRIVATE_H__

#include <gdk/gdk.h>
#include "gdk/gdkinternals.h"

#define GDK_PRIVATE_CALL(symbol)        (gdk__private__ ()->symbol)

GdkDisplay *    gdk_display_open_default        (void);

gboolean        gdk_device_grab_info            (GdkDisplay  *display,
                                                 GdkDevice   *device,
                                                 GdkWindow  **grab_window,
                                                 gboolean    *owner_events);

void            gdk_add_option_entries          (GOptionGroup *group);

void            gdk_pre_parse                   (void);

GdkGLFlags      gdk_gl_get_flags                (void);
void            gdk_gl_set_flags                (GdkGLFlags flags);

void            gdk_window_freeze_toplevel_updates      (GdkWindow *window);
void            gdk_window_thaw_toplevel_updates        (GdkWindow *window);

GdkRenderingMode gdk_display_get_rendering_mode (GdkDisplay       *display);
void             gdk_display_set_rendering_mode (GdkDisplay       *display,
                                                 GdkRenderingMode  mode);

gboolean         gdk_display_get_debug_updates (GdkDisplay *display);
void             gdk_display_set_debug_updates (GdkDisplay *display,
                                                gboolean    debug_updates);

void            gdk_window_move_to_rect         (GdkWindow          *window,
                                                 const GdkRectangle *rect,
                                                 GdkGravity          rect_anchor,
                                                 GdkGravity          window_anchor,
                                                 GdkAnchorHints      anchor_hints,
                                                 gint                rect_anchor_dx,
                                                 gint                rect_anchor_dy);

const gchar *   gdk_get_startup_notification_id (void);


typedef struct {
  /* add all private functions here, initialize them in gdk-private.c */
  gboolean (* gdk_device_grab_info) (GdkDisplay  *display,
                                     GdkDevice   *device,
                                     GdkWindow  **grab_window,
                                     gboolean    *owner_events);

  GdkDisplay *(* gdk_display_open_default) (void);

  void (* gdk_add_option_entries) (GOptionGroup *group);
  void (* gdk_pre_parse) (void);

  GdkGLFlags (* gdk_gl_get_flags) (void);
  void       (* gdk_gl_set_flags) (GdkGLFlags flags);

  void (* gdk_window_freeze_toplevel_updates) (GdkWindow *window);
  void (* gdk_window_thaw_toplevel_updates) (GdkWindow *window);

  GdkRenderingMode (* gdk_display_get_rendering_mode) (GdkDisplay       *display);
  void             (* gdk_display_set_rendering_mode) (GdkDisplay       *display,
                                                       GdkRenderingMode  mode);

  gboolean         (* gdk_display_get_debug_updates) (GdkDisplay *display);
  void             (* gdk_display_set_debug_updates) (GdkDisplay *display,
                                                      gboolean    debug_updates);

  void (* gdk_window_move_to_rect) (GdkWindow          *window,
                                    const GdkRectangle *rect,
                                    GdkGravity          rect_anchor,
                                    GdkGravity          window_anchor,
                                    GdkAnchorHints      anchor_hints,
                                    gint                rect_anchor_dx,
                                    gint                rect_anchor_dy);

  const gchar * (* gdk_get_startup_notification_id) (void);
} GdkPrivateVTable;

GDK_AVAILABLE_IN_ALL
GdkPrivateVTable *      gdk__private__  (void);

#endif /* __GDK__PRIVATE_H__ */
