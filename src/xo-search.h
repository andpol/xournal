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

#define SEARCH_HIGHLIGHT_COLOUR 0xffff0080

void update_search_string(gchar *text);
void update_search_string_from_selection();

void find_next(gboolean backwards);
void find_next_text(gboolean backwards);
void find_next_pdf(gboolean backwards);

void clear_pdf_matches();
void reset_search();
