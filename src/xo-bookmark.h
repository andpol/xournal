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

Item * create_bookmark(const gchar* title, double page_pos);
GnomeCanvasItem * create_bookmark_canvas_item(Item * bookmark, GnomeCanvasGroup * group);
void delete_selected_bookmark();
void free_bookmark_resources(Item * bookmark);
void clear_bookmarks();


#endif /* BOOKMARK_H */
