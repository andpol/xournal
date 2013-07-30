/*
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libgnomecanvas/libgnomecanvas.h>
#include <glib/gstdio.h>
#include <string.h>

#include "xo-bookmark.h"

void add_bookmark_listtore_entry(Item * bookmark, int page_num, Layer * layer) {
  GtkListStore * list_store = GTK_LIST_STORE(GET_COMPONENT("bookmark_liststore"));

  // TODO: insert into order
  gtk_list_store_insert_with_values(list_store, NULL, -1, 0, bookmark->text, 1, page_num + 1, 2, layer, 3, bookmark, -1);
}

GnomeCanvasItem * create_bookmark_canvas_item(Item * bookmark, GnomeCanvasGroup * group) {
  return gnome_canvas_item_new(group,
      gnome_canvas_line_get_type(),
      "points", bookmark->path,
      "width-units", 10.0,
      "fill-color-rgba", 0x000000FF,
      "cap-style", GDK_CAP_ROUND,
      "join-style", GDK_JOIN_ROUND,
      NULL);
}

Item * create_bookmark(const gchar* title, double page_pos) {
  Page * page = ui.cur_page;
  Item * bmrk = g_new(struct Item, 1);

  bmrk->type = ITEM_BOOKMARK;
  bmrk->text = g_strdup(title);

  bmrk->bbox.left =   page->width - 100;
  bmrk->bbox.right =  page->width;
  bmrk->bbox.top =    page_pos - 5.0; // 5.0 = half of line width
  bmrk->bbox.bottom = page_pos + 5.0;

  bmrk->path = gnome_canvas_points_new(2);
  gdouble pts_array[] = {
    page->width - 100, page_pos,
    page->width      , page_pos,
  };
  memcpy(bmrk->path->coords, pts_array, sizeof(pts_array));

  bmrk->canvas_item = create_bookmark_canvas_item(bmrk, ui.cur_layer->group);

  // Add the bookmark to the liststore for displaying in the sidebar tree
  add_bookmark_listtore_entry(bmrk, g_list_index(journal.pages, page), ui.cur_layer);

  // Append the bookmark the the page's top layer
  ui.cur_layer->items = g_list_append(ui.cur_layer->items, bmrk);

  return bmrk;
}

void delete_selected_bookmark() {
  GtkListStore * list_store = GTK_LIST_STORE(GET_COMPONENT("bookmark_liststore"));
  GtkTreeView *tree = GTK_TREE_VIEW(GET_COMPONENT("bookmark_tree"));


  GtkTreeSelection *selection = gtk_tree_view_get_selection(tree);
  GtkTreeModel *tree_model;
  GtkTreeIter tree_iter;

  if (!gtk_tree_selection_get_selected(selection, &tree_model, &tree_iter)) {
    // No selection
    return;
  }

  {
    gint page_num;
    Item * bookmark;
    Layer * layer;
    gtk_tree_model_get(tree_model, &tree_iter, 1, &page_num, 2, &layer, 3, &bookmark, -1);

    // Remove the layer's bookmark 'Item'
    layer->items = g_list_remove(layer->items, bookmark);

    // Remove the UI element and free allocated memory
    gtk_object_destroy(GTK_OBJECT(bookmark->canvas_item));
    free_bookmark_resources(bookmark);
    g_free(bookmark);
  }

  { // Remove the list store entry
    gtk_list_store_remove(list_store, &tree_iter);
  }
}

/*
 * Frees all the memory allocated to a bookmark's properties
 * NOTE: Does NOT free the bookmark Item itself or destroy the UI gnome canvas item it references
 */
void free_bookmark_resources(Item * bookmark) {
    g_free(bookmark->text);
    gnome_canvas_points_free(bookmark->path);
}

/*
 * Clear all the bookmarks from the sidebar
 * NOTE: Does NOT free the bookmark Items or destroy the referenced gnome canvas items
 */
void clear_bookmarks() {
  GtkListStore * list_store = GTK_LIST_STORE(GET_COMPONENT("bookmark_liststore"));
  gtk_list_store_clear(list_store);
}


/*
 * Returns an iterator pointing to the matching GtkListStore entry for the
 * specified bookmark Item, or NULL if none found
 */
gboolean get_bookmark_list_store_entry(Item * bookmark, GtkListStore ** out_listStore, GtkTreeIter * outIter) {
  GtkTreeModel * list_store = GTK_TREE_MODEL(GET_COMPONENT("bookmark_liststore"));
  *out_listStore = GTK_LIST_STORE(list_store);
  GtkTreeIter it;

  if (bookmark == NULL ||
      bookmark->type != ITEM_BOOKMARK ||
      !gtk_tree_model_get_iter_first(list_store, &it)) {
    return FALSE;
  }

  do {
    Item * bkmrk;
    gtk_tree_model_get(list_store, &it, 3, &bkmrk, -1);
    if (bkmrk == bookmark) {
      // Found the matching bookmark, copy the iterator for the list store entry
      *outIter = it;
      return TRUE;
    }
  } while (gtk_tree_model_iter_next(list_store, &it));

  return FALSE;
}

