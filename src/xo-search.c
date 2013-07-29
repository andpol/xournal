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
	search_data.search_type = SEARCH_CURRENT_LAYER;

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
	search_data.search_type = SEARCH_CURRENT_LAYER;

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

// Checks a text item, starting at item_offset, for the search string.
// Returns TRUE if a match was found and selected.
// page_offset is how many pages we are from the current page; if search direction
// is backwards, this is the number of pages we're behind, otherwise number of pages ahead.
gboolean do_find_match(int page_offset, struct Item *item, int item_offset, gboolean backwards) {
	int index, new_page_no;

	if (item->type != ITEM_TEXT) {
		return FALSE;
	}

	index = strstr_index(item->text, search_data.search_string, item_offset, backwards,
			search_data.search_case_sensitive);
	if (index >= 0) {
		// Calculate the page number of the item
		if (backwards) {
			new_page_no = (ui.pageno - page_offset) % journal.npages;
			while (new_page_no < 0) {
				new_page_no += journal.npages;
			}
		} else {
			new_page_no = (page_offset + ui.pageno) % journal.npages;
		}

		// Switch if we're not on the right page
		if (new_page_no != ui.pageno) {
			do_switch_page(new_page_no, FALSE, FALSE);
		}

		// Put the item in view
		scroll_to_item(item);

		// Select the text match
		select_text_range(item, index, index + strlen(search_data.search_string));
		return TRUE;
	}

	return FALSE;
}

void find_next(gboolean backwards) {
	if (search_data.search_type == SEARCH_CURRENT_LAYER) {
		reset_selection();
		find_next_text(backwards);
	} else if (search_data.search_type == SEARCH_BACKGROUND_PDF) {
		find_next_pdf(backwards);
	}
}

// Find the next occurrence of search_string in text items on the current layer.
// Searches backwards if backwards is TRUE, otherwise searches forwards.
void find_next_text(gboolean backwards) {
	int i;
	struct Page *page;
	struct Layer *layer;
	struct Item *item;
	GList *pagelist, *itemlist;
	GtkTextBuffer *text_buffer;
	GtkTextIter start, end, select_start, select_end;
	int cursor;
	int first_page_offset = 0, limit;

	// Nothing to search for
	if (search_data.search_string == NULL ) {
		return;
	}

	// Check for a match inside the current item, after the current selection or cursor position
	if (ui.cur_item_type == ITEM_TEXT) {
		cursor = get_cursor_offset(backwards);
		end_text();
		if (cursor != -1 && do_find_match(0, ui.cur_item, cursor, backwards)) {
			return;
		}
	}

	end_text();

	// Start on the current page
	pagelist = g_list_nth(journal.pages, ui.pageno);
	if (ui.layerno >= 0) { // Can only search if we're on a non-background layer
		page = (struct Page *) (pagelist->data);
		layer = (struct Layer *) (g_list_nth(page->layers, ui.layerno)->data);

		// Sort items by vertical order on page
		// TODO: filter list before sorting
		itemlist = g_list_sort(g_list_copy(layer->items), compare_items);

		if (ui.cur_item != NULL ) {
			// Find starting place; item after current item, or first item if none
			itemlist = g_list_find(itemlist, ui.cur_item);
			if (itemlist != NULL ) {
				itemlist = (backwards ? itemlist->prev : itemlist->next);
			}
		} else if (backwards) {
			itemlist = g_list_last(itemlist);
		}

		// Search the current page to the end (or start, if searching backwards)
		for (i = 0; itemlist != NULL ; i++) {
			item = (struct Item *) itemlist->data;
			if (do_find_match(0, item, -1, backwards)) {
				return;
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
			if (do_find_match(page_offset, item, -1, backwards)) {
				return;
			}

			itemlist = (backwards ? itemlist->prev : itemlist->next);
		}
	}

	show_find_popup("Search text not found.");
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

create_search_layer(struct Page *page) {
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

// Draw a rectangle on top of a search match found in a PDF background.
void highlight_pdf_match(struct PdfMatch *match) {
	PopplerRectangle *rect = match->rect;

	if (match->pageno != ui.pageno) {
		do_switch_page(match->pageno, FALSE, FALSE);
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
void find_next_pdf(gboolean backwards) {
	struct PdfMatch *match;
	PopplerRectangle *rect;

	if (bgpdf.status == STATUS_NOT_INIT) {
		show_find_popup("No background PDF to search.");
		return;
	}

	if (search_data.num_matches == -1) {
		get_pdf_matches();
	}

	if (search_data.num_matches == 0) {
		// Not found, show a message
		show_find_popup("Search text not found.");
		return;
	}

	if (backwards) {
		search_data.current_pdf_match = g_list_previous(search_data.current_pdf_match);
		if (search_data.current_pdf_match == NULL ) {
			search_data.current_pdf_match = g_list_last(search_data.pdf_matches);
		}
	} else {
		search_data.current_pdf_match = g_list_next(search_data.current_pdf_match);
		if (search_data.current_pdf_match == NULL ) {
			search_data.current_pdf_match = g_list_first(search_data.pdf_matches);
		}
	}

	match = (struct PdfMatch *) search_data.current_pdf_match->data;

	clear_pdf_matches();
	highlight_pdf_match(match);
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
	GtkCheckButton *case_sensitive;
	GtkRadioButton *current_layer, *background_pdf;

	find_dialog = GTK_WIDGET(GET_COMPONENT("findDialog"));

	// Put the search string into the text box
	find_text = (GtkEntry*) GTK_WIDGET(GET_COMPONENT("findText"));
	gtk_entry_set_text(find_text, search_data.search_string == NULL ? "" : search_data.search_string);

	// Set the case sensitive checkbox
	case_sensitive = (GtkCheckButton*) GTK_WIDGET(GET_COMPONENT("searchCaseCheckbox"));
	gtk_toggle_button_set_active(&(case_sensitive->toggle_button), search_data.search_case_sensitive);

	// Set the layer search options radio buttons
	current_layer = (GtkRadioButton*) GTK_WIDGET(GET_COMPONENT("findCurrentLayerRadio"));
	background_pdf = (GtkRadioButton*) GTK_WIDGET(GET_COMPONENT("findPdfBgRadio"));

	if (search_data.search_type == SEARCH_CURRENT_LAYER) {
		gtk_toggle_button_set_active(&current_layer->check_button.toggle_button, TRUE);
	} else if (search_data.search_type == SEARCH_BACKGROUND_PDF) {
		gtk_toggle_button_set_active(&background_pdf->check_button.toggle_button, TRUE);
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
	return ((*(struct Item *) a).bbox.top - (*(struct Item *) b).bbox.top);
}

