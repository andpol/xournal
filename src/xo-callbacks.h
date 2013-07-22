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


#include <gtk/gtk.h>

G_MODULE_EXPORT void
on_fileNew_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_fileNewBackground_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_fileOpen_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_fileSave_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_fileSaveAs_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_filePrintOptions_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_filePrint_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_filePrintPDF_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_fileQuit_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editUndo_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editRedo_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editFind_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editFindNext_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editFindPrevious_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_findNextButton_clicked              (GtkButton       *button,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_findPreviousButton_clicked          (GtkButton       *button,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_findCloseButton_clicked             (GtkButton       *button,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_searchCaseCheckbox_toggled          (GtkCheckButton  *button,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_findCurrentLayerRadio_toggled       (GtkRadioButton  *button,
		                                gpointer         user_data);

G_MODULE_EXPORT void
on_findPdfBgRadio_toggled              (GtkRadioButton  *button,
		                                gpointer         user_data);

G_MODULE_EXPORT void
on_findText_changed                    (GtkEditable     *editable,
		                                gpointer         user_data);

G_MODULE_EXPORT void
on_editCut_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editCopy_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editPaste_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_editDelete_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewContinuous_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewOnePage_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewZoomIn_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewZoomOut_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewNormalSize_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewPageWidth_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewFirstPage_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewPreviousPage_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewNextPage_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewLastPage_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewShowLayer_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewHideLayer_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalNewPageBefore_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalNewPageAfter_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalNewPageEnd_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalDeletePage_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalNewLayer_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalDeleteLayer_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalFlatten_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalPaperSize_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorWhite_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorYellow_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorPink_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorOrange_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorBlue_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorGreen_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_papercolorOther_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_paperstylePlain_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_paperstyleLined_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_paperstyleRuled_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_paperstyleGraph_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalLoadBackground_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalScreenshot_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalApplyAllPages_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsPen_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsEraser_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsHighlighter_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsText_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsSelectRegion_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsSelectRectangle_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsVerticalSpace_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorBlack_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorBlue_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorRed_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorGreen_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorGray_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorLightBlue_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorLightGreen_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorMagenta_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorOrange_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorYellow_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorWhite_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_colorOther_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_penthicknessVeryFine_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_penthicknessFine_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_penthicknessMedium_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_penthicknessThick_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_penthicknessVeryThick_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_eraserFine_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_eraserMedium_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_eraserThick_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_eraserStandard_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_eraserWhiteout_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_eraserDeleteStrokes_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_highlighterFine_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_highlighterMedium_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_highlighterThick_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsTextFont_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsDefaultPen_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsDefaultEraser_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsDefaultHighlighter_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsDefaultText_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsSetAsDefault_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsRuler_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsSavePreferences_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_helpIndex_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_helpAbout_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolEraser_activate                 (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_buttonToolDefault_clicked           (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_buttonFine_clicked                  (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_buttonMedium_clicked                (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_buttonThick_clicked                 (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_button_release_event         (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_enter_notify_event           (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_leave_notify_event           (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_key_press_event              (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_canvas_motion_notify_event          (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_comboLayer_changed                  (GtkComboBox     *combobox,
                                        gpointer         user_data);

G_MODULE_EXPORT gboolean
on_winMain_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsUseXInput_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_vscroll_changed                     (GtkAdjustment   *adjustment,
                                        gpointer        user_data);

G_MODULE_EXPORT void
on_spinPageNo_value_changed            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalDefaultBackground_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_journalSetAsDefault_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_comboStdSizes_changed               (GtkComboBox     *combobox,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_entryWidth_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_entryHeight_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_comboUnit_changed                   (GtkComboBox     *combobox,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_viewFullscreen_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsButtonMappings_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsProgressiveBG_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_mru_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2Pen_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2Eraser_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2Highlighter_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2Text_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2SelectRegion_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2SelectRectangle_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2VerticalSpace_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2LinkBrush_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2CopyBrush_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3Pen_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3Eraser_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3Highlighter_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3Text_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3SelectRegion_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3SelectRectangle_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3VerticalSpace_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3LinkBrush_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3CopyBrush_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


G_MODULE_EXPORT void
on_viewSetZoom_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_spinZoom_value_changed              (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_radioZoom_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_radioZoom100_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_radioZoomWidth_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_radioZoomHeight_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsHand_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2Hand_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3Hand_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsPrintRuling_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsAutoloadPdfXoj_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_fontButton_font_set                 (GtkFontButton   *fontbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsLeftHanded_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsShortenMenus_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsAutoSavePrefs_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsReco_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsPressureSensitive_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_buttonColorChooser_set              (GtkColorButton  *colorbutton,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsButtonsSwitchMappings_activate(GtkMenuItem    *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_toolsImage_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button2Image_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_button3Image_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

G_MODULE_EXPORT void
on_optionsPenCursor_activate           (GtkCheckMenuItem *checkmenuitem,
                                        gpointer         user_data);
