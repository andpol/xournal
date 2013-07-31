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

// Sort page num column by page num then by page position
gint bookmark_liststore_compare_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data) {
    gint page_num1, page_num2;
    Item *bookmark1, *bookmark2;
    gtk_tree_model_get(model, a, BOOKMARK_COL_PAGENUM, &page_num1, BOOKMARK_COL_ITEM, &bookmark1, -1);
    gtk_tree_model_get(model, b, BOOKMARK_COL_PAGENUM, &page_num2, BOOKMARK_COL_ITEM, &bookmark2, -1);

    if (page_num1 == page_num2) {
      // Order by vertical order on page
      return bookmark1->bbox.top - bookmark2->bbox.top;
    } else {
      // Order by page
      return page_num1 - page_num2;
    }
}
GtkListStore * create_new_bookmark_liststore() {
  // NOTE: the order of each of the column params MUST match the enum definition
  GtkListStore * store = gtk_list_store_new(BOOKMARK_COL_COUNT,
      G_TYPE_STRING,
      G_TYPE_INT,
      G_TYPE_POINTER,
      G_TYPE_POINTER);

  gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store), BOOKMARK_COL_PAGENUM, bookmark_liststore_compare_func, NULL, NULL);
  // Sort by page num column by default
  gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store), BOOKMARK_COL_PAGENUM, GTK_SORT_ASCENDING);

  return store;
}

void add_bookmark_liststore_entry(GtkListStore * list_store, Item * bookmark, const gchar * title, int page_num, Layer * layer) {
  if(bookmark == NULL || bookmark->type != ITEM_BOOKMARK) {
    g_error("add_bookmark_liststore_entry() passed a non-bookmark Item");
    return;
  }

  gtk_list_store_insert_with_values(list_store, NULL, -1,
      BOOKMARK_COL_TITLE, title,
      BOOKMARK_COL_PAGENUM, page_num + 1,
      BOOKMARK_COL_LAYER, layer,
      BOOKMARK_COL_ITEM, bookmark,
      -1);
}

GnomeCanvasItem * create_bookmark_canvas_item(Item * bookmark, GnomeCanvasGroup * group) {
  if(bookmark == NULL || bookmark->type != ITEM_BOOKMARK) {
    g_error("create_bookmark_canvas_item() passed a non-bookmark Item");
    return NULL;
  }

  return gnome_canvas_item_new(group,
      gnome_canvas_pixbuf_get_type(),
      "pixbuf", bookmark->image,
      "x", bookmark->bbox.left, "y", bookmark->bbox.top,
      "width", bookmark->bbox.right - bookmark->bbox.left,
      "height", bookmark->bbox.bottom - bookmark->bbox.top,
      "width-set", TRUE, "height-set", TRUE,
      NULL);
}

Item * create_bookmark(const gchar* title, double page_pos) {
  Page * page = ui.cur_page;
  Item * bmrk = g_new(struct Item, 1);

  bmrk->type = ITEM_BOOKMARK;

  GError *error = NULL;
  bmrk->image = create_pixbuf("bookmark.png");
  if (bmrk->image == NULL) {
    g_error("%s", error->message);
  }

  int height = gdk_pixbuf_get_height(bmrk->image);
  int width = gdk_pixbuf_get_width(bmrk->image);
  bmrk->bbox.left =   page->width - width;
  bmrk->bbox.right =  page->width;
  bmrk->bbox.top =    page_pos - height / 2;
  bmrk->bbox.bottom = page_pos + height / 2;

  // The canvas item (drawing of the pixbuf image)
  bmrk->canvas_item = create_bookmark_canvas_item(bmrk, ui.cur_layer->group);
  // Add the bookmark to the liststore for displaying in the sidebar tree
  add_bookmark_liststore_entry(bookmark_liststore, bmrk, title, g_list_index(journal.pages, page), ui.cur_layer);
  // Append the bookmark the the page's top layer
  ui.cur_layer->items = g_list_append(ui.cur_layer->items, bmrk);

  return bmrk;
}

/*
 * Deletes the currently selected bookmark in the sidebar
 */
void delete_selected_bookmark() {
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
    gtk_tree_model_get(tree_model, &tree_iter, BOOKMARK_COL_PAGENUM, &page_num, BOOKMARK_COL_LAYER, &layer, BOOKMARK_COL_ITEM, &bookmark, -1);

    // Remove the layer's bookmark 'Item'
    layer->items = g_list_remove(layer->items, bookmark);

    // Remove the UI element and free allocated memory
    gtk_object_destroy(GTK_OBJECT(bookmark->canvas_item));
    free_bookmark_resources(bookmark);
    g_free(bookmark);
  }

  { // Remove the list store entry
    gtk_list_store_remove(bookmark_liststore, &tree_iter);
  }
}

/*
 * Frees all the memory allocated to a bookmark's properties
 * NOTE: Does NOT free the bookmark Item itself or destroy the UI gnome canvas item it references
 */
void free_bookmark_resources(Item * bookmark) {
  if(bookmark == NULL || bookmark->type != ITEM_BOOKMARK) {
    g_error("free_bookmark_resources() passed a non-bookmark Item");
    return;
  }
  g_object_unref(bookmark->image);
}

/*
 * Clear all the bookmarks from the sidebar
 * NOTE: Does NOT free the bookmark Items or destroy the referenced gnome canvas items
 */
void clear_bookmarks(GtkListStore * list_store) {
  gtk_list_store_clear(list_store);
}


/*
 * Returns an iterator pointing to the matching GtkListStore entry for the
 * specified bookmark Item, or NULL if none found
 */
gboolean get_bookmark_list_store_entry(GtkListStore * list_store, Item * bookmark, GtkTreeIter * outIter) {
  if(bookmark == NULL || bookmark->type != ITEM_BOOKMARK) {
    g_error("get_bookmark_list_store_entry() passed a non-bookmark Item");
    return FALSE;
  }

  GtkTreeModel * tree_model = GTK_TREE_MODEL(list_store);
  GtkTreeIter it;

  if (!gtk_tree_model_get_iter_first(tree_model, &it)) {
    g_error("get_bookmark_list_store_entry() could not find the first iterator");
    return FALSE;
  }

  do {
    Item * bkmrk;
    gtk_tree_model_get(tree_model, &it, 3, &bkmrk, -1);
    if (bkmrk == bookmark) {
      // Found the matching bookmark, copy the iterator for the list store entry
      *outIter = it;
      return TRUE;
    }
  } while (gtk_tree_model_iter_next(tree_model, &it));

  return FALSE;
}
