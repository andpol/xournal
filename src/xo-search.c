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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>

#include "xournal.h"
#include "xo-paint.h"
#include "xo-search.h"

void init_search() {
	update_search_string(NULL );
	search_data.search_case_sensitive = FALSE; // TODO: user preference?
	search_data.search_type = SEARCH_TYPE_CURRENT_LAYER;

	reset_pdf_search();

	search_data.find_dialog_x = -1;
	search_data.find_dialog_y = -1;
}

// Reset all state associated with search.
void reset_search() {
	GtkWidget *find_dialog;

	if (search_data.search_string != NULL ) {
		free(search_data.search_string);
		search_data.search_string = NULL;
	}
	search_data.search_case_sensitive = FALSE;
	search_data.search_type = SEARCH_TYPE_CURRENT_LAYER;

	reset_pdf_search();

	hide_find_dialog();
}

// Returns the starting index of the last match of needle in
// haystack that ends before limit characters (-1 if not found).
int rstrstr_index(const char *haystack, const char *needle, int limit) {
	int index = -1, tmp_index = 0;
	char *match = NULL;
	const char *haystack_pos;
	int needle_len;

	needle_len = strlen(needle);
	haystack_pos = haystack;

	for (;;) {
		match = strstr(haystack_pos, needle);
		tmp_index = match - haystack;

		// No match, or match goes past limit
		if (match == NULL || tmp_index + needle_len > limit) {
			break;
		}

		// Go one past this match and keep searching
		haystack_pos = match + 1;
		index = tmp_index;
	}

	return index;
}

// Returns the index of the first occurrence of needle in haystack (-1 if not found).
// Starts search at from; moves backwards to start if backwards is TRUE, else forwards to end.
// from == -1 means search whole string.
int strstr_index(const char *haystack, const char *needle, int from, gboolean backwards,
		gboolean case_sensitive) {
	int i;
	char *match;
	const char *new_haystack, *new_needle;
	char *lower_haystack, *lower_needle;
	int retval;

	if (case_sensitive) {
		// Nothing special to do, strstr is case-sensitive by default
		new_haystack = haystack;
		new_needle = needle;
	} else {
		// Convert both strings to lowercase for case-insensitive matching
		lower_haystack = (char *) malloc(sizeof(char) * strlen(haystack) + 1);
		for (i = 0; i < strlen(haystack); i++) {
			lower_haystack[i] = g_ascii_tolower(haystack[i]);
		}
		lower_haystack[i] = '\0';
		new_haystack = lower_haystack;

		lower_needle = (char *) malloc(sizeof(char) * strlen(needle) + 1);
		for (i = 0; i < strlen(needle); i++) {
			lower_needle[i] = g_ascii_tolower(needle[i]);
		}
		lower_needle[i] = '\0';
		new_needle = lower_needle;
	}

	// No limit
	if (from == -1) {
		if (backwards) {
			from = strlen(new_haystack) + 1;
		} else {
			from = 0;
		}
	}

	if (backwards) {
		retval = rstrstr_index(new_haystack, new_needle, from);
	} else {
		match = strstr(new_haystack + from, new_needle);
		if (match == NULL ) {
			retval = -1;
		} else {
			retval = match - new_haystack;
		}
	}

	// Take case of the lowercase strings we made
	if (!case_sensitive) {
		free(lower_haystack);
		free(lower_needle);
	}

	return retval;
}

// Get the currently selected text in a text item, if any
gchar * get_selected_text() {
	GtkTextBuffer *text_buffer;
	GtkTextIter start, end;
	gchar *text;

	if (ui.cur_item_type != ITEM_TEXT) {
		return NULL ;
	}

	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui.cur_item->widget) );
	if (!gtk_text_buffer_get_has_selection(text_buffer)) {
		// No selection
		return NULL ;
	}

	gtk_text_buffer_get_selection_bounds(text_buffer, &start, &end);
	text = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	return text;
}

// Update the search string and make all required changes
void update_search_string(const gchar *text) {
	GtkWidget *widget;
	gboolean sensitive;

	search_data.search_string = g_strdup(text);
	sensitive = (search_data.search_string != NULL );

	// Enable or disable the find next and previous widgets
	gtk_widget_set_sensitive(GTK_WIDGET(GET_COMPONENT("editFindNext")), sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(GET_COMPONENT("editFindPrevious")), sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(GET_COMPONENT("findNextButton")), sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(GET_COMPONENT("findPreviousButton")), sensitive);
}

// Fetch the new search string from selected text, if any.
// Returns TRUE if search_string updated, FALSE otherwise.
void update_search_string_from_selection() {
	gchar *text;

	if (ui.cur_item_type == ITEM_TEXT) {
		text = get_selected_text();
		if (text != NULL ) {
			update_search_string(text);
		}
	}
}

int get_cursor_offset(gboolean backwards) {
	GtkTextBuffer *text_buffer;
	GtkTextIter dummy, cursor, limit;
	GtkTextMark *insert;

	if (ui.cur_item_type != ITEM_TEXT) {
		return -1;
	}

	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui.cur_item->widget) );

	// If has selection, search from selection bound; else from cursor
	if (gtk_text_buffer_get_has_selection(text_buffer)) {
		if (backwards) {
			gtk_text_buffer_get_selection_bounds(text_buffer, &cursor, &dummy);
		} else {
			gtk_text_buffer_get_selection_bounds(text_buffer, &dummy, &cursor);
		}
	} else {
		insert = gtk_text_buffer_get_insert(text_buffer);
		gtk_text_buffer_get_iter_at_mark(text_buffer, &cursor, insert);
	}

	return gtk_text_iter_get_offset(&cursor);
}

// Select a range of a text item.
void select_text_range(struct Item *item, int start_index, int end_index) {
	GtkTextBuffer *text_buffer;
	GtkTextIter start_iter, end_iter;

	if (item->type != ITEM_TEXT) {
		return;
	}

	// Start this text so we can make a selection
	start_text_existing(item);

	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui.cur_item->widget) );

	// Get iters and set the selection
	gtk_text_buffer_get_iter_at_offset(text_buffer, &start_iter, start_index);
	gtk_text_buffer_get_iter_at_offset(text_buffer, &end_iter, end_index);
	gtk_text_buffer_select_range(text_buffer, &start_iter, &end_iter);
}

// Show a dialog that supersedes the find dialog.
void show_find_popup(const gchar *message) {
	GtkWidget *dialog, *find_dialog;
	gboolean restore_find_dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW (winMain), GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
	find_dialog = GTK_WIDGET(GET_COMPONENT("findDialog"));

	// Save find dialog position to move it back after
	gtk_window_get_position(GTK_WINDOW(find_dialog), &search_data.find_dialog_x,
			&search_data.find_dialog_y);

	restore_find_dialog = gtk_widget_get_visible(find_dialog);

	// Hide the find dialog to disable it while running the popup
	gtk_widget_hide(find_dialog);
	gtk_dialog_run(GTK_DIALOG(dialog) );

	gtk_widget_destroy(dialog);

	if (restore_find_dialog) {
		// Move the find dialog back to its previous position and bring it back
		gtk_window_move(GTK_WINDOW(find_dialog), search_data.find_dialog_x, search_data.find_dialog_y);
		gtk_widget_show(find_dialog);
	}
}

int page_number_from_offset(int page_offset, gboolean backwards) {
	int new_pageno;

	if (backwards) {
		new_pageno = (ui.pageno - page_offset) % journal.npages;
		while (new_pageno < 0) {
			new_pageno += journal.npages;
		}
	} else {
		new_pageno = (page_offset + ui.pageno) % journal.npages;
	}

	return new_pageno;
}

void find_next(gboolean backwards) {
	double x, y;
	struct TextMatch *text_match = NULL;
	GList *pdf_match = NULL;
	struct PdfMatch *m;
	int pg1, pg2;
	double x1, x2, y1, y2;

	clear_pdf_matches();

	// No search types selected
	if (search_data.search_type == 0) {
		return;
	}

	if (ui.cur_item != NULL ) {
		// Start from the current item by default
		x = ui.cur_item->bbox.left;
		y = ui.cur_item->bbox.top;
		printf("from item: %f %f\n", x, y);
	} else if (search_data.current_pdf_match != NULL
			&& search_data.current_pdf_match->pageno == ui.pageno) {
		// Start from last PDF match only if it's on the page we're looking at
		x = ((struct PdfMatch *) search_data.current_pdf_match)->rect->x1;
		y = ((struct PdfMatch *) search_data.current_pdf_match)->rect->y1;
		printf("from pdf: %f %f\n", x, y);
	} else {
		// If all else fails, just start from the top of the current page
		// (or bottom if searching backwards)
		if (backwards) {
			x = ui.cur_page->width + 1;
			y = ui.cur_page->height + 1;
		} else {
			x = y = -1;
		}
		printf("from page: %f %f\n", x, y);
	}

	if (search_data.search_type & SEARCH_TYPE_CURRENT_LAYER) {
		reset_selection();
		text_match = find_next_text(backwards, ui.pageno, x, y);
	}
	if (search_data.search_type & SEARCH_TYPE_BACKGROUND_PDF) {
		if (bgpdf.status == STATUS_NOT_INIT) {
			show_find_popup("No background PDF to search.");
		} else {
			pdf_match = find_next_pdf(backwards, ui.pageno, x, y);
		}
	}

	if (text_match == NULL && pdf_match == NULL ) {
		show_find_popup("Search text not found.");
	} else if (text_match != NULL && pdf_match != NULL ) {
		m = (struct PdfMatch *) pdf_match->data;
		printf("both: (%d,%f)  (%d,%f)\n", text_match->pageno, text_match->item->bbox.top, m->pageno,
				m->rect->y1);

		pg1 = text_match->pageno;

		pg2 = m->pageno;

		x1 = text_match->item->bbox.left;
		y1 = text_match->item->bbox.top;
		x2 = m->rect->x1;
		y2 = m->rect->y1;

		if (!backwards) {
			if (pg1 < ui.pageno) {
				pg1 += journal.npages;
			}
			if (pg2 < ui.pageno) {
				pg2 += journal.npages;
			}

			if (pg1 == pg2 && pg1 == ui.pageno) {
				if (y1 < y || (y1 == y && x1 < x)) {
					pg1 += journal.npages;
				}
				if (y2 < y || (y2 == y && x2 < x)) {
					pg2 += journal.npages;
				}
			}
		} else {
			if (pg1 > ui.pageno) {
				pg1 -= journal.npages;
			}
			if (pg2 > ui.pageno) {
				pg2 -= journal.npages;
			}

			if (pg1 == pg2 && pg1 == ui.pageno) {
				if (y1 > y || (y1 == y && x1 > x)) {
					pg1 -= journal.npages;
				}
				if (y2 > y || (y2 == y && x2 > x)) {
					pg2 -= journal.npages;
				}
			}
		}

		if (is_farther(backwards, pg1, x1, y1, pg2, x2, y2)) {
			pdf_match = NULL;
		} else {
			text_match = NULL;
		}
	}

	if (text_match != NULL && pdf_match == NULL ) {
		printf("text: %f\n", text_match->item->bbox.top);

		highlight_text_match(text_match);
		search_data.current_pdf_match = NULL;
	} else if (text_match == NULL && pdf_match != NULL ) {
		printf("pdf: %f\n", ((struct PdfMatch *) pdf_match->data)->rect->y1);

		search_data.current_pdf_match = (struct PdfMatch *) pdf_match->data;
		end_text();
		ui.cur_item = NULL;
		ui.cur_item_type = ITEM_NONE;
		highlight_pdf_match(pdf_match);
	}

	if (text_match != NULL ) {
		free(text_match);
	}
}

struct TextMatch * create_text_match(gboolean backwards, struct Item *item, int item_offset,
		int page_offset) {
	int index;
	struct TextMatch *match;

	if (item->type == ITEM_TEXT) {
		index = strstr_index(item->text, search_data.search_string, item_offset, backwards,
				search_data.search_case_sensitive);

		if (index >= 0) {
			match = g_new(struct TextMatch, 1);
			match->item = item;
			match->index = index;
			match->pageno = page_number_from_offset(page_offset, backwards);
			return match;
		}
	}

	return NULL ;
}

// Find the next occurrence of search_string in text items on the current layer.
// Searches backwards if backwards is TRUE, otherwise searches forwards.
struct TextMatch * find_next_text(gboolean backwards, int pageno, double x, double y) {
	int i;
	struct Page *page;
	struct Layer *layer;
	struct Item *item;
	GList *pagelist, *itemlist;
	GtkTextBuffer *text_buffer;
	GtkTextIter start, end, select_start, select_end;
	int cursor, index;
	int first_page_offset = 0, limit;
	struct TextMatch *match;

// Nothing to search for
	if (search_data.search_string == NULL ) {
		return NULL ;
	}

// Check for a match inside the current item, after the current selection or cursor position
	if (ui.cur_item_type == ITEM_TEXT) {
		cursor = get_cursor_offset(backwards);
		end_text();

		if (cursor >= 0) {
			match = create_text_match(backwards, ui.cur_item, cursor, 0);
			if (match != NULL ) {
				return match;
			}
		}
	}

	end_text();

// Start on the current page
	pagelist = g_list_nth(journal.pages, pageno);
	if (ui.layerno >= 0) { // Can only search if we're on a non-background layer
		page = (struct Page *) (pagelist->data);
		layer = (struct Layer *) (g_list_nth(page->layers, ui.layerno)->data);

		// Sort items by vertical order on page
		// TODO: filter list before sorting
		itemlist = g_list_sort(g_list_copy(layer->items), compare_items);
		if (backwards) {
			itemlist = g_list_last(itemlist);
		}

		while (itemlist != NULL ) {
			item = (struct Item *) itemlist->data;
			if (is_farther(backwards, pageno, x, y, pageno, item->bbox.left, item->bbox.top)) {
				break;
			}

			itemlist = (backwards ? itemlist->prev : itemlist->next);
		}

		// Search the current page to the end (or start, if searching backwards)
		for (i = 0; itemlist != NULL ; i++) {
			item = (struct Item *) itemlist->data;
			match = create_text_match(backwards, item, -1, 0);
			if (match != NULL ) {
				return match;
			}

			itemlist = (backwards ? itemlist->prev : itemlist->next);
		}

		// Save how far we got in the current page
		first_page_offset = i;
	}

// Loop through all other pages
	int page_offset;
	for (page_offset = 1; page_offset <= journal.npages; page_offset++) {
		pagelist = (backwards ? pagelist->prev : pagelist->next);
		if (pagelist == NULL ) {
			// Wrap last page if searching backwards, else first page
			pagelist = (backwards ? g_list_last(journal.pages) : g_list_first(journal.pages));
		}

		page = (struct Page *) (pagelist->data);
		if (page->layerno < 0) {
			// Can't search background for text items
			continue;
		}

		// Get current layer
		layer = (struct Layer *) (g_list_nth(page->layers, page->layerno)->data);

		// Sort items by vertical order on page
		// TODO: filter list before sorting
		itemlist = g_list_sort(g_list_copy(layer->items), compare_items);
		if (backwards) {
			itemlist = g_list_last(itemlist);
		}

		limit = layer->nitems;
		if (page_offset == journal.npages) {
			// Only go partway through the page since we searched half earlier
			limit -= first_page_offset;
		}

		for (i = 0; i < limit; i++) {
			item = (struct Item *) itemlist->data;
			match = create_text_match(backwards, item, -1, page_offset);
			if (match != NULL ) {
				return match;
			}

			itemlist = (backwards ? itemlist->prev : itemlist->next);
		}
	}

	return NULL ;
}

void highlight_text_match(struct TextMatch *match) {
	if (match->pageno != ui.pageno) {
		do_switch_page(match->pageno, FALSE, FALSE);
	}

	select_text_range(match->item, match->index, match->index + strlen(search_data.search_string));
	scroll_to_item(match->item);
}

void create_search_layer(struct Page *page) {
	if (page->search_layer != NULL ) {
		delete_layer(page->search_layer);
	}

	page->search_layer = g_new(struct Layer, 1);
	page->search_layer->items = NULL;
	page->search_layer->nitems = 0;
	page->search_layer->group = (GnomeCanvasGroup *) gnome_canvas_item_new(page->group,
			gnome_canvas_group_get_type(), NULL );
	lower_canvas_item_to(page->group, GNOME_CANVAS_ITEM(page->search_layer->group),
			page->bg->canvas_item);

	page->search_layer->group = (GnomeCanvasGroup *) gnome_canvas_item_new(page->group,
			gnome_canvas_group_get_type(), NULL );
	lower_canvas_item_to(page->group, GNOME_CANVAS_ITEM(page->search_layer->group),
			page->bg->canvas_item);
}

void free_pdf_match(void *data) {
	struct PdfMatch *match = (struct PdfMatch *) data;
	poppler_rectangle_free(match->rect);
	free(match);
}

void get_pdf_matches() {
	GList *pagelist, *findlist, *finditer;
	struct Page *page;
	PopplerPage *pdf_page;
	PopplerRectangle *rect;
	struct PdfMatch *match;
	int current_page, i;
	double width, height, tmp;

	search_data.num_matches = 0;
	if (search_data.pdf_matches != NULL ) {
		g_list_free_full(search_data.pdf_matches, free_pdf_match);
		search_data.pdf_matches = NULL;
	}

	for (pagelist = journal.pages, i = 0; pagelist != NULL ; pagelist = g_list_next(pagelist), i++) {
		page = (struct Page *) pagelist->data;

		if (page->bg->type == BG_PDF) {
			current_page = page->bg->file_page_seq - 1;
			pdf_page = poppler_document_get_page(bgpdf.document, current_page);
			if (pdf_page != NULL ) {
				findlist = poppler_page_find_text(pdf_page, search_data.search_string);
				if (findlist != NULL ) {
					poppler_page_get_size(pdf_page, &width, &height);
					for (finditer = findlist; finditer != NULL ; finditer = g_list_next(finditer)) {
						rect = (PopplerRectangle*) finditer->data;

						// Flip y coordinates
						tmp = rect->y1;
						rect->y1 = height - rect->y2;
						rect->y2 = height - tmp;

						match = g_new(struct PdfMatch, 1);
						match->rect = rect;
						match->pageno = i;
						finditer->data = match;
					}

					search_data.pdf_matches = g_list_concat(search_data.pdf_matches, findlist);
					search_data.num_matches += g_list_length(findlist);
				}
			}
		}
	}
}

void clear_pdf_matches() {
	GList *pagelist, *itemlist;
	struct Page *page;

	for (pagelist = journal.pages; pagelist != NULL ; pagelist = pagelist->next) {
		page = (struct Page *) pagelist->data;
		if (page->search_layer != NULL ) {
			delete_layer(page->search_layer);
			page->search_layer = NULL;
		}
	}
}

// Draw a rectangle on top of a search match found in a PDF background.
void highlight_pdf_match(GList *match) {
	struct PdfMatch *pdf_match = (struct PdfMatch *) match->data;
	PopplerRectangle *rect = pdf_match->rect;

	if (pdf_match->pageno != ui.pageno) {
		do_switch_page(pdf_match->pageno, FALSE, FALSE);
	}

// Create the highlighting rectangle
	struct Item *search_rect = (struct Item *) g_malloc(sizeof(struct Item));
	search_rect->type = ITEM_SELECTRECT;
	search_rect->path = NULL;
	search_rect->bbox.top = rect->y1;
	search_rect->bbox.bottom = rect->y2;
	search_rect->bbox.left = rect->x1;
	search_rect->bbox.right = rect->x2;

	create_search_layer(ui.cur_page);

// Create the highlighting rectangle canvas item
	search_rect->canvas_item = gnome_canvas_item_new(ui.cur_page->search_layer->group,
			gnome_canvas_rect_get_type(), "width-pixels", 2, "fill-color-rgba", SEARCH_HIGHLIGHT_COLOUR,
			"x1", rect->x1, "x2", rect->x2, "y1", rect->y1, "y2", rect->y2, NULL );
	ui.cur_page->search_layer->items = g_list_append(ui.cur_page->search_layer->items, search_rect);
	ui.cur_page->search_layer->nitems++;

// Put the match in view
	scroll_to_item(search_rect);

// Trigger redraw
	gnome_canvas_set_pixels_per_unit(canvas, ui.zoom);
}

// Find the next match in the PDF background.
GList * find_next_pdf(gboolean backwards, int pageno, double x, double y) {
	GList *match = NULL;
	PopplerRectangle *rect;
	int i;

	if (search_data.search_string == NULL || bgpdf.status == STATUS_NOT_INIT) {
		return NULL ;
	}

	if (search_data.num_matches == -1) {
		get_pdf_matches();
	}

	if (search_data.num_matches == 0) {
		return NULL ;
	}

	match = backwards ? g_list_last(search_data.pdf_matches) : search_data.pdf_matches;

	// TODO: inefficient
	for (i = 0; i < search_data.num_matches; i++) {
		struct PdfMatch *m = (struct PdfMatch *) match->data;
		if (is_farther(backwards, pageno, x, y, m->pageno, m->rect->x1, m->rect->y1)) {
			break;
		}
		match = backwards ? match->prev : match->next;
	}

	return match;
}

gboolean is_farther(gboolean backwards, int pageno, double x, double y, int my_pageno, double my_x,
		double my_y) {
	gboolean result;

	if (pageno == my_pageno && x == my_x && y == my_y) {
		return FALSE;
	} else {
		result = is_farther_forward(pageno, x, y, my_pageno, my_x, my_y);
		return backwards ? !result : result;
	}
}

gboolean is_farther_forward(int pageno, double x, double y, int my_pageno, double my_x, double my_y) {
	if (my_pageno > pageno) {
		return TRUE;
	} else if (my_pageno < pageno) {
		return FALSE;
	} else {
		// Same page
		if (my_y > y) {
			return TRUE;
		} else if (my_y < y) {
			return FALSE;
		} else {
			// Same height
			if (my_x > x) {
				return TRUE;
			} else {
				return FALSE;
			}
		}
	}
}

// Resets num_matches, pdf_matches, and current_pdf_match.
// Clears highlighting too.
void reset_pdf_search() {
	search_data.num_matches = -1;
	if (search_data.pdf_matches != NULL ) {
		g_list_free_full(search_data.pdf_matches, free_pdf_match);
		search_data.pdf_matches = NULL;
	}
	search_data.current_pdf_match = NULL;

	clear_pdf_matches();
}

void show_find_dialog() {
	GtkWidget *find_dialog;
	GtkEntry *find_text;
	GtkCheckButton *case_sensitive, *current_layer, *background_pdf;

	find_dialog = GTK_WIDGET(GET_COMPONENT("findDialog"));

// Put the search string into the text box
	find_text = (GtkEntry*) GTK_WIDGET(GET_COMPONENT("findText"));
	gtk_entry_set_text(find_text, search_data.search_string == NULL ? "" : search_data.search_string);

// Set the case sensitive checkbox
	case_sensitive = (GtkCheckButton*) GTK_WIDGET(GET_COMPONENT("searchCaseCheckbox"));
	gtk_toggle_button_set_active(&(case_sensitive->toggle_button), search_data.search_case_sensitive);

// Set the layer search options radio buttons
	current_layer = (GtkCheckButton*) GTK_WIDGET(GET_COMPONENT("searchCurrentLayerCheckbox"));
	background_pdf = (GtkCheckButton*) GTK_WIDGET(GET_COMPONENT("searchBgPdfCheckbox"));

	if (search_data.search_type & SEARCH_TYPE_CURRENT_LAYER) {
		gtk_toggle_button_set_active(&current_layer->toggle_button, TRUE);
	} else if (search_data.search_type & SEARCH_TYPE_BACKGROUND_PDF) {
		gtk_toggle_button_set_active(&background_pdf->toggle_button, TRUE);
	}

// Restore focus to the find next button since we're just reusing the same dialog instance
	gtk_widget_grab_focus(GTK_WIDGET(GET_COMPONENT("findNextButton")) );

	if (search_data.find_dialog_x > 0 && search_data.find_dialog_y > 0) {
		gtk_window_move(GTK_WINDOW(find_dialog), search_data.find_dialog_x, search_data.find_dialog_y);
	}
	gtk_widget_show(find_dialog);

}

void hide_find_dialog() {
	GtkWidget *find_dialog;
	find_dialog = GTK_WIDGET(GET_COMPONENT("findDialog"));

// Save position before hiding
	gtk_window_get_position(GTK_WINDOW(find_dialog), &search_data.find_dialog_x,
			&search_data.find_dialog_y);
	gtk_widget_hide(find_dialog);
}

// Sorts items from top to bottom based on their bounding boxes.
int compare_items(const void * a, const void * b) {
	struct Item *i1 = (struct Item *) a;
	struct Item *i2 = (struct Item *) b;

	double diff_y = i1->bbox.top - i2->bbox.top;
	if (diff_y < 0) {
		return -1;
	} else if (diff_y > 0) {
		return 1;
	} else {
		double diff_x = i1->bbox.left - i2->bbox.left;
		if (diff_x < 0) {
			return -1;
		} else if (diff_x > 0) {
			return 1;
		} else {
			return 0;
		}
	}
}

