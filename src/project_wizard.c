/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "error.h"
#include "menubar_main.h"
#include "project_wizard.h"
#include "project_wizard_page_basic_settings.h"
#include "statusbar_main.h"

gint pw_forward(gint current_page, GUIData *ltrgui)
{
  gint next_page;

  switch (current_page) {
    case PW_INTRODUCTION:
      next_page = PW_SELECTFILES;
      break;
    case PW_SELECTFILES:
      next_page = (gtk_toggle_button_get_active(
                   GTK_TOGGLE_BUTTON(ltrgui->pw_do_clustering_cb))
                                              ? PW_CLASSIFICATION : PW_SUMMARY);
      break;
    case PW_CLASSIFICATION:
      next_page = PW_SUMMARY;
      break;
    default:
      next_page = -1;
  }
  return next_page;
}

void pw_init(GUIData *ltrgui)
{
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(
                                                  ltrgui->pw_treeview_gff3)),
                                                        GTK_SELECTION_MULTIPLE);

  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrgui->project_wizard),
                               (GtkAssistantPageFunc) pw_forward, ltrgui, NULL);
}

void pw_reset_defaults(GUIData *ltrgui)
{
  pw_page_basic_settings_reset_defaults(ltrgui);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->project_wizard),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->project_wizard),
                                                        PW_SELECTFILES), FALSE);
}

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui)
{
  gtk_widget_hide(GTK_WIDGET(assistant));
  sb_main_set_status(ltrgui, "Welcome to LTRGui");
  pw_reset_defaults(ltrgui);
}

gboolean pw_update_progress_dialog(gpointer data)
{
  PWThreadData *threaddata = (PWThreadData*) data;
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(threaddata->progressbar));
  return TRUE;
}

static void pw_progress_dialog_init(PWThreadData *threaddata)
{
  GtkWidget *label,
            *vbox;
  guint sid;

  /* create the modal window which warns the user to wait */
  threaddata->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_modal(GTK_WINDOW(threaddata->window), TRUE);
  gtk_window_set_title(GTK_WINDOW(threaddata->window), "Progress");
  gtk_window_resize(GTK_WINDOW(threaddata->window), 200, 50);
  gtk_container_set_border_width(GTK_CONTAINER(threaddata->window), 12);
  g_signal_connect(threaddata->window, "delete_event",
                   G_CALLBACK(gtk_true), NULL);
  vbox = gtk_vbox_new(FALSE, 12);
  /* create label */
  label = gtk_label_new("Please wait...");
  gtk_container_add(GTK_CONTAINER(vbox), label);
  /* create progress bar */
  threaddata->progressbar = gtk_progress_bar_new();
  gtk_container_add(GTK_CONTAINER(vbox), threaddata->progressbar);
  /* add vbox to dialog */
  gtk_container_add(GTK_CONTAINER(threaddata->window), vbox);
  gtk_widget_show_all(threaddata->window);
  /* refresh the progress dialog */
  sid = g_timeout_add(100, pw_update_progress_dialog,
                      (gpointer) threaddata);
  g_object_set_data(G_OBJECT(threaddata->window),
                    "source_id", GINT_TO_POINTER(sid));
}

static gboolean finish(gpointer data)
{
  PWThreadData *threaddata = (PWThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltr_families;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(GTK_WIDGET(threaddata->window));

  if (!threaddata->had_err) {
    gtk_widget_destroy(ltrfams);
    threaddata->ltrgui->ltr_families = gtk_ltr_families_new();
    ltrfams = threaddata->ltrgui->ltr_families;
    gtk_box_pack_start(GTK_BOX(threaddata->ltrgui->vbox1_main), ltrfams,
                       TRUE, TRUE, 0);
    gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrfams),
                                    threaddata->nodes,
                                    threaddata->features,
                                    threaddata->n_features);
    mb_main_view_columns_set_submenu(threaddata->ltrgui, threaddata->features,
                                     threaddata->err, FALSE);
    mb_main_activate_menuitems(threaddata->ltrgui);
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrfams),
                                     g_strdup(threaddata->fullname));
  }
  g_free(threaddata->projectfile);
  g_free(threaddata->projectdir);
  gt_error_delete(threaddata->err);
  g_slice_free(PWThreadData, threaddata);

  return FALSE;
}

static gpointer start(gpointer data)
{
  PWThreadData *threaddata = (PWThreadData*) data;
  GUIData *ltrgui = threaddata->ltrgui;
  GtkSpinButton *sbutton;
  GtkTreeSelection *sel;
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GList *rows,
        *tmp;
  GtStr *tmpdirprefix;
  GtNodeStream *last_stream = NULL,
               *gff3_in_stream = NULL,
               *ltr_cluster_stream = NULL,
               *ltr_classify_stream = NULL,
               *ltrgui_preprocess_stream = NULL,
               *ltrgui_array_out_stream = NULL;
  GtEncseqLoader *el = NULL;
  GtEncseq *encseq = NULL;
  GtArray *nodes;
  GtHashmap *features;
  gint psmall,
       plarge,
       wordsize,
       i = 0,
       num_of_files;
  char buf[BUFSIZ];
  const char **gff3_files,
              *indexname;
  gdouble seqid,
         xdrop;

  g_snprintf(buf, BUFSIZ, "%s/tmp/%s", threaddata->projectdir,
             threaddata->projectfile);
  tmpdirprefix = gt_str_new_cstr(buf);

  sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_psmall);
  psmall = gtk_spin_button_get_value_as_int(sbutton);
  sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_plarge);
  plarge = gtk_spin_button_get_value_as_int(sbutton);
  sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_words);
  wordsize = gtk_spin_button_get_value_as_int(sbutton);
  sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_xdrop);
  xdrop = gtk_spin_button_get_value(sbutton);
  sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_seqid);
  seqid = gtk_spin_button_get_value(sbutton);

  list_view = GTK_TREE_VIEW(ltrgui->pw_treeview_gff3);
  sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_select_all(sel);
  num_of_files = gtk_tree_selection_count_selected_rows(sel);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  gff3_files = g_malloc((size_t) num_of_files * sizeof(const char*));
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       0, &gff3_files[i],
                       -1);
    i++;
    tmp = tmp->next;
  }
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);

  last_stream = gff3_in_stream = gt_gff3_in_stream_new_unsorted(num_of_files,
                                                                gff3_files);

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                     ltrgui->pw_do_clustering_cb))) {
    indexname = gtk_label_get_text(GTK_LABEL(ltrgui->pw_label_encseq));
    el = gt_encseq_loader_new();
    encseq = gt_encseq_loader_load(el, indexname, threaddata->err);
    if (!encseq)
      threaddata->had_err = -1;
    if (!threaddata->had_err)
      last_stream = ltr_cluster_stream = gt_ltr_cluster_stream_new(last_stream,
                                                                   encseq,
                                                                   tmpdirprefix,
                                                                   plarge,
                                                                   psmall,
                                                                   wordsize,
                                                                   xdrop,
                                                                   seqid,
                                                                   threaddata->err);
  }
  if (!threaddata->had_err &&
      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                     ltrgui->pw_do_classification_cb))) {
    last_stream = ltr_classify_stream = gt_ltr_classify_stream_new(last_stream,
                                                                   NULL,
                                                                   NULL,
                                                                   threaddata->err);
  }
  if (!threaddata->had_err) {
    nodes = gt_array_new(sizeof(GtGenomeNode*));
    features = gt_hashmap_new(GT_HASH_STRING, free_hash, NULL);
    last_stream = ltrgui_preprocess_stream =
                                    gt_ltrgui_preprocess_stream_new(last_stream,
                                                                    features,
                                                                    &threaddata->n_features,
                                                                    threaddata->err);
    last_stream = ltrgui_array_out_stream = gt_array_out_stream_new(last_stream,
                                                                    nodes,
                                                                    threaddata->err);
  }
  if (!ltrgui_array_out_stream)
    threaddata->had_err = -1;
  if (!threaddata->had_err)
    threaddata->had_err = gt_node_stream_pull(last_stream, threaddata->err);

  if (!threaddata->had_err) {
    threaddata->nodes = nodes;
    threaddata->features = features;
  }
  gt_node_stream_delete(ltr_classify_stream);
  gt_node_stream_delete(ltr_cluster_stream);
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(ltrgui_array_out_stream);
  gt_node_stream_delete(ltrgui_preprocess_stream);
  gt_encseq_loader_delete(el);
  gt_encseq_delete(encseq);

  for (i = 0; i < num_of_files; i++)
    g_free((gpointer) gff3_files[i]);
  g_free(gff3_files);

  g_idle_add(finish, data);
  return NULL;
}

void pw_apply(GtkAssistant *assistant, GUIData *ltrgui)
{
  GtkWidget *dialog;

  const gchar *fullname;
  gchar *projectdir,
        *projectfile,
        projecttmpdir[BUFSIZ];
  gint had_err = 0;

  gtk_widget_hide(GTK_WIDGET(assistant));
  fullname = gtk_label_get_text(GTK_LABEL(ltrgui->pw_label_projectname));
  projectfile = g_path_get_basename(fullname);
  projectdir = g_path_get_dirname(fullname);
  if (g_file_test(fullname, G_FILE_TEST_EXISTS)) {
    gchar buffer[BUFSIZ];
    g_snprintf(buffer, BUFSIZ, FILE_EXISTS_DIALOG, projectfile);
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    buffer);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
      gtk_widget_destroy(dialog);
      gtk_widget_show(GTK_WIDGET(assistant));
      gtk_assistant_set_current_page(GTK_ASSISTANT(ltrgui->project_wizard), 0);
      g_free(projectdir);
      g_free(projectfile);
      return;
    } else
      gtk_widget_destroy(dialog);
  }
  g_snprintf(projecttmpdir, BUFSIZ, "%s/tmp", projectdir);
  if (!g_file_test(projecttmpdir, G_FILE_TEST_EXISTS))
    had_err = g_mkdir(projecttmpdir, 0755);
  if (had_err != 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not make dir: %s",
                projecttmpdir);
    error_handle(ltrgui->err);
    return;
  }

  PWThreadData *threaddata;
  threaddata = g_slice_new(PWThreadData);
  threaddata->ltrgui = ltrgui;
  threaddata->had_err = 0;
  threaddata->fullname = fullname;
  threaddata->projectfile = projectfile;
  threaddata->projectdir = projectdir;
  threaddata->n_features = LTRFAMS_LV_N_COLUMS;
  threaddata->err = gt_error_new();
  pw_progress_dialog_init(threaddata);

  g_thread_create(start, (gpointer) threaddata, FALSE, NULL);

}
