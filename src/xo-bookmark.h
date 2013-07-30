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

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "xournal.h"

// The collection of bookmarks for the entire Journal
extern GtkListStore * bookmark_liststore;

typedef enum {
  BOOKMARK_COL_TITLE = 0,   // ghar *
  BOOKMARK_COL_PAGENUM, // gint
  BOOKMARK_COL_LAYER,   // gpointer to a Layer struct
  BOOKMARK_COL_ITEM,    // gpointer to a Item struct
  BOOKMARK_COL_COUNT
} t_bookmark_column;

GtkListStore * create_new_bookmark_liststore();
void add_bookmark_liststore_entry(GtkListStore * list_store, Item * bookmark, const gchar * title, int page_num, Layer * layer);
Item * create_bookmark(const gchar* title, double page_pos);
GnomeCanvasItem * create_bookmark_canvas_item(Item * bookmark, GnomeCanvasGroup * group);
void delete_selected_bookmark();
void free_bookmark_resources(Item * bookmark);
void clear_bookmarks(GtkListStore * list_store);
gboolean get_bookmark_list_store_entry(GtkListStore * list_store, Item * bookmark, GtkTreeIter * outIter);


#endif /* BOOKMARK_H */
