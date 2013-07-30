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

// TODO: config setting
#define SEARCH_HIGHLIGHT_COLOUR 0xffff0080

struct TextMatch {
	Item *item;
	int index;
	int pageno;
};

struct PdfMatch {
	PopplerRectangle *rect;
	int pageno;
};

void update_search_string(const gchar *text);
void update_search_string_from_selection();
void update_search_enabled();

void find_next(gboolean backwards);

struct TextMatch * find_next_text(gboolean backwards, int pageno, double x, double y);
void highlight_text_match(struct TextMatch *match);
GList * find_next_pdf(gboolean backwards, int pageno, double x, double y);
void highlight_pdf_match(GList *match);

void reset_search();
void reset_pdf_search();
void clear_pdf_matches();

void show_find_dialog();
void hide_find_dialog();

int compare_items(const void *a, const void *b);
