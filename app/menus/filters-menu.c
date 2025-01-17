/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gegl.h>
#include <gegl-plugin.h>
#include <gtk/gtk.h>

#include "menus-types.h"

#include "gegl/gimp-gegl-utils.h"

#include "core/gimp.h"
#include "core/gimp-filter-history.h"

#include "widgets/gimpuimanager.h"
#include "widgets/gimpwidgets-utils.h"

#include "filters-menu.h"


/*  public functions  */

void
filters_menu_setup (GimpUIManager *manager,
                    const gchar   *ui_path)
{
  GList *op_classes;
  GList *iter;
  gint   i;

  g_return_if_fail (GIMP_IS_UI_MANAGER (manager));
  g_return_if_fail (ui_path != NULL);

  for (i = 0; i < gimp_filter_history_size (manager->gimp); i++)
    {
      gchar *action_name;

      action_name = g_strdup_printf ("filters-recent-%02d", i + 1);

      gimp_ui_manager_add_ui (manager, "/Filters/Recently Used",
                              action_name, "Filters", TRUE);

      g_free (action_name);
    }

  op_classes = gimp_gegl_get_op_classes ();

  for (iter = op_classes; iter; iter = iter->next)
    {
      GeglOperationClass *opclass = GEGL_OPERATION_CLASS (iter->data);
      gchar              *formatted_op_name;
      gchar              *action_name;

      formatted_op_name = g_strdup (opclass->name);
      gimp_make_valid_action_name (formatted_op_name);
      action_name = g_strdup_printf ("filters-%s", formatted_op_name);
      g_free (formatted_op_name);

      gimp_ui_manager_add_ui (manager, "/Filters/GEGL Operations",
                              action_name, NULL, FALSE);

      g_free (action_name);
    }

  g_list_free (op_classes);
}
