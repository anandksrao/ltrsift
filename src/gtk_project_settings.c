/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include "error.h"
#include "gtk_project_settings.h"
#include "message_strings.h"

const gchar* gtk_project_settings_get_indexname(GtkProjectSettings *projset)
{
  return gtk_label_get_text(GTK_LABEL(projset->label_indexname));
}

void gtk_project_settings_update_indexname(GtkProjectSettings *projset,
                                           const gchar *indexname)
{
  GtRDBStmt *stmt = NULL;
  GtError *err;
  gchar query[BUFSIZ];
  gint had_err = 0;

  gtk_label_set_text(GTK_LABEL(projset->label_indexname), indexname);
  if (projset->rdb != NULL) {
    err = gt_error_new();
    g_snprintf(query, BUFSIZ,
               "UPDATE project_settings SET indexname = \"%s\"",
               gtk_label_get_text(GTK_LABEL(projset->label_indexname)));
    stmt = gt_rdb_prepare(projset->rdb, query, -1, err);
    if (stmt) {
      had_err = gt_rdb_stmt_exec(stmt, err);
      gt_rdb_stmt_delete(stmt);
    } else had_err = -1;
    if (had_err == -1)
      error_handle(GTK_WIDGET(projset), err);
    gt_error_delete(err);
  }
}

void gtk_project_settings_set_data(GtkProjectSettings *projset,
                                   const gchar *projectfile,
                                   gchar **gff3files,
                                   const gchar *indexname,
                                   gboolean clustering,
                                   gint xgapped,
                                   gint xgapless,
                                   gint xfinal,
                                   gint mscoregapped,
                                   gint mscoregapless,
                                   gint gapopen,
                                   gint gapextend,
                                   gint matchscore,
                                   gint mismatchcost,
                                   gint stepsize,
                                   const gchar *morelast,
                                   gint psmall,
                                   gint plarge,
                                   gboolean classification,
                                   gdouble ltrtol,
                                   gdouble candtol,
                                   gchar **features)
{
  gchar *tmp, buffer[BUFSIZ];

  gtk_label_set_text(GTK_LABEL(projset->label_projectfile), projectfile);
  tmp = g_strjoinv("\n", gff3files);
  gtk_label_set_text(GTK_LABEL(projset->label_gff3files), tmp);
  g_free(tmp);
  gtk_label_set_text(GTK_LABEL(projset->label_indexname), indexname);
  gtk_label_set_text(GTK_LABEL(projset->label_didclustering),
                               clustering ? "yes" : "no");

  if (xgapped == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_xgapped), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", xgapped);
    gtk_label_set_text(GTK_LABEL(projset->label_xgapped), buffer);
  }

  if (xgapless == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_xgapless), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", xgapless);
    gtk_label_set_text(GTK_LABEL(projset->label_xgapless), buffer);
  }

  if (xfinal == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_xfinal), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", xfinal);
    gtk_label_set_text(GTK_LABEL(projset->label_xfinal), buffer);
  }

  if (gapopen == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_gapopen), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", gapopen);
    gtk_label_set_text(GTK_LABEL(projset->label_gapopen), buffer);
  }

  if (gapextend == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_gapextend), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", gapextend);
    gtk_label_set_text(GTK_LABEL(projset->label_gapextend), buffer);
  }

  if (mscoregapless == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_mscoregapless), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", mscoregapless);
    gtk_label_set_text(GTK_LABEL(projset->label_mscoregapless), buffer);
  }

  if (mscoregapped == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_mscoregapped), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", mscoregapped);
    gtk_label_set_text(GTK_LABEL(projset->label_mscoregapped), buffer);
  }

  if (matchscore == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_matchscore), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", matchscore);
    gtk_label_set_text(GTK_LABEL(projset->label_matchscore), buffer);
  }

  if (mismatchcost == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_mismatchcost), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", mismatchcost);
    gtk_label_set_text(GTK_LABEL(projset->label_mismatchcost), buffer);
  }

  if (stepsize == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_stepsize), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", stepsize);
    gtk_label_set_text(GTK_LABEL(projset->label_stepsize), buffer);
  }

  gtk_label_set_text(GTK_LABEL(projset->label_morelast), morelast);
  g_snprintf(buffer, BUFSIZ, "%d", psmall);
  gtk_label_set_text(GTK_LABEL(projset->label_psmall), buffer);
  g_snprintf(buffer, BUFSIZ, "%d", plarge);
  gtk_label_set_text(GTK_LABEL(projset->label_plarge), buffer);
  gtk_label_set_text(GTK_LABEL(projset->label_didclassification),
                               classification ? "yes" : "no");
  g_snprintf(buffer, BUFSIZ, "%.1f", ltrtol);
  gtk_label_set_text(GTK_LABEL(projset->label_ltrtolerance), buffer);
  g_snprintf(buffer, BUFSIZ, "%.1f", candtol);
  gtk_label_set_text(GTK_LABEL(projset->label_candtolerance), buffer);
  tmp = g_strjoinv("\n", features);
  gtk_label_set_text(GTK_LABEL(projset->label_usedfeatures), tmp);
  g_free(tmp);
}

gint gtk_project_settings_set_data_from_sqlite(GtkProjectSettings *projset,
                                               const gchar *projectfile,
                                               GtRDB  *rdb,
                                               GtError *err)
{
  GtRDBStmt *stmt;
  GtStr *result;
  gint had_err = 0;

  gtk_label_set_text(GTK_LABEL(projset->label_projectfile), projectfile);

  stmt = gt_rdb_prepare(rdb,
                        "SELECT gff3files, indexname, clustering, xgapped, "
                         "xgapless, xfinal, mscoregapped, mscoregapless, "
                         "gapopen, gapextend, matchscore, mismatchcost, "
                         "stepsize, psmall, plarge, "
                         "classification, ltr_tolerance, cand_tolerance, "
                         "features, morelast FROM project_settings",
                        -1, err);
  if (!stmt)
    return -1;
  if ((had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_rdb_stmt_delete(stmt);
    return -1;
  }
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 0, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_gff3files), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 1, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_indexname), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 2, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_didclustering),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 3, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_xgapped), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 4, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_xgapless), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 5, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_xfinal), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 6, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_mscoregapped), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 7, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_mscoregapless), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 8, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_gapopen), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 9, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_gapextend), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 10, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_matchscore), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 11, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_mismatchcost), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 12, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_stepsize),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 13, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_psmall), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 14, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_plarge), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 15, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_didclassification),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 16, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_ltrtolerance),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 17, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_candtolerance),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 18, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_usedfeatures),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 19, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_morelast),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  gt_rdb_stmt_delete(stmt);

  return 0;
}

gint gtk_project_settings_save_data(GtkProjectSettings *projset,
                                    GtRDB *rdb, GtError *err)
{
  GtRDBStmt *stmt;
  GtError *tmp_err;
  gchar query[BUFSIZ];
  gint had_err = 0;
  tmp_err = gt_error_new();

  stmt = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS project_settings "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "gff3files TEXT, "
                            "indexname TEXT, "
                            "clustering TEXT, "
                            "xgapped TEXT, "
                            "xgapless TEXT, "
                            "xfinal TEXT, "
                            "mscoregapped TEXT, "
                            "mscoregapless TEXT, "
                            "gapopen TEXT, "
                            "gapextend TEXT, "
                            "matchscore TEXT, "
                            "mismatchcost TEXT, "
                            "stepsize TEXT, "
                            "morelast TEXT, "
                            "psmall TEXT, "
                            "plarge TEXT, "
                            "classification TEXT, "
                            "ltr_tolerance TEXT, "
                            "cand_tolerance TEXT, "
                            "features TEXT)",
                           -1, tmp_err);

  if (!stmt) {
    gt_error_set(err, "Could not save project settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(tmp_err);
    return -1;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, tmp_err)) < 0) {
    gt_error_set(err, "Could not save project settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(tmp_err);
    gt_rdb_stmt_delete(stmt);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  stmt = gt_rdb_prepare(rdb, "DELETE FROM project_settings",
                           -1, tmp_err);

  if (!stmt) {
    gt_error_set(err, "Could not save project settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(tmp_err);
    return -1;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, tmp_err)) < 0) {
    gt_error_set(err, "Could not save project settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(tmp_err);
    gt_rdb_stmt_delete(stmt);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  g_snprintf(query, BUFSIZ,
             "INSERT INTO project_settings (gff3files, "
              "indexname, clustering, xgapped, xgapless, xfinal, mscoregapped, "
              "mscoregapless, gapopen, gapextend, matchscore, mismatchcost, stepsize,  "
              "morelast, psmall, plarge, classification, "
              "ltr_tolerance, cand_tolerance, features) values (\"%s\", "
              "\"%s\", \"%s\", \"%s\", "
              "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", "
              "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", "
              "\"%s\")",
             gtk_label_get_text(GTK_LABEL(projset->label_gff3files)),
             gtk_label_get_text(GTK_LABEL(projset->label_indexname)),
             gtk_label_get_text(GTK_LABEL(projset->label_didclustering)),
             gtk_label_get_text(GTK_LABEL(projset->label_xgapped)),
             gtk_label_get_text(GTK_LABEL(projset->label_xgapless)),
             gtk_label_get_text(GTK_LABEL(projset->label_xfinal)),
             gtk_label_get_text(GTK_LABEL(projset->label_mscoregapped)),
             gtk_label_get_text(GTK_LABEL(projset->label_mscoregapless)),
             gtk_label_get_text(GTK_LABEL(projset->label_gapopen)),
             gtk_label_get_text(GTK_LABEL(projset->label_gapextend)),
             gtk_label_get_text(GTK_LABEL(projset->label_matchscore)),
             gtk_label_get_text(GTK_LABEL(projset->label_mismatchcost)),
             gtk_label_get_text(GTK_LABEL(projset->label_stepsize)),
             gtk_label_get_text(GTK_LABEL(projset->label_morelast)),
             gtk_label_get_text(GTK_LABEL(projset->label_psmall)),
             gtk_label_get_text(GTK_LABEL(projset->label_plarge)),
             gtk_label_get_text(GTK_LABEL(projset->label_didclassification)),
             gtk_label_get_text(GTK_LABEL(projset->label_ltrtolerance)),
             gtk_label_get_text(GTK_LABEL(projset->label_candtolerance)),
             gtk_label_get_text(GTK_LABEL(projset->label_usedfeatures)));
  stmt = gt_rdb_prepare(rdb, query, -1, tmp_err);

  if (!stmt) {
    gt_error_set(err, "Could not save project settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(tmp_err);
    return -1;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, tmp_err)) < 0) {
    gt_error_set(err, "Could not save projestmtt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(tmp_err);
    gt_rdb_stmt_delete(stmt);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);
  gt_error_delete(tmp_err);
  return 0;
}

static void delete_event(GtkWidget *widget, GT_UNUSED GdkEvent *event,
                         GT_UNUSED gboolean user_data)
{
  gtk_widget_hide(widget);
}

static void change_index_clicked(GT_UNUSED GtkWidget *button,
                                 GtkProjectSettings *projset)
{
  GtkWidget *filechooser;
  GtkFileFilter *esq_file_filter;

  filechooser = gtk_file_chooser_dialog_new("Select indexname...",
                                            GTK_WINDOW(projset),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                            GTK_RESPONSE_ACCEPT, NULL);
  esq_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(esq_file_filter, ESQ_FILTER_PATTERN);
  gtk_file_filter_add_pattern(esq_file_filter, ESQ_FILTER_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), esq_file_filter);
  gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(filechooser), FALSE);
  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    gchar *filename, *tmp;
    const gchar *projectfile;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    gtk_widget_destroy(filechooser);
    tmp = g_strndup(filename, strlen(filename) - strlen(ESQ_PATTERN));
    gtk_label_set_label(GTK_LABEL(projset->label_indexname), tmp);
    g_free(filename);
    g_free(tmp);

    projectfile = gtk_label_get_text(GTK_LABEL(projset->label_projectfile));
    if (g_strcmp0(projectfile, "") != 0) {
      GtRDBStmt *stmt;
      GtError *err = gt_error_new();
      gchar query[BUFSIZ];
      gint had_err;

      if (projset->rdb != NULL) {
        g_snprintf(query, BUFSIZ,
                   "UPDATE project_settings SET indexname = \"%s\"",
                   gtk_label_get_text(GTK_LABEL(projset->label_indexname)));
        stmt = gt_rdb_prepare(projset->rdb, query, -1, err);
        if (stmt) {
          had_err = gt_rdb_stmt_exec(stmt, err);
          gt_rdb_stmt_delete(stmt);
        } else had_err = -1;
        if (had_err == -1)
          error_handle(GTK_WIDGET(projset), err);
      }
      gt_error_delete(err);
    }
  }
}

static void close_clicked(GT_UNUSED GtkWidget *button,
                          GtkProjectSettings *projset)
{
  gtk_widget_hide(GTK_WIDGET(projset));
}

static void gtk_project_settings_init(GtkProjectSettings *projset)
{
  GtkWidget *hbox,
            *vbox,
            *vbox1,
            *vbox2,
            *label,
            *align,
            *button;

  projset->notebook = gtk_notebook_new();

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Project file:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Used GFF3 files:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Indexname:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Clustering enabled?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  button = gtk_button_new_with_mnemonic("_Change/set indexname");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(change_index_clicked), projset);
  gtk_box_pack_start(GTK_BOX(vbox1), button, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = projset->label_projectfile = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_gff3files = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_indexname = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_didclustering = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("General settings");
  gtk_notebook_append_page(GTK_NOTEBOOK(projset->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Cost to open a gap:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new("Cost to extend a gap:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for gapless alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for final gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Minimum score for gapless alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Minimum score for gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Cost for a nucleotide mismatch:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Score for a nucleotide match:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Step size along the query sequence:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);

  label = gtk_label_new("Additional LAST parameters:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);

  label = gtk_label_new("% of the smaller seq. a match needs to cover:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("% of the larger seq. a match needs to cover:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Perform classification?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = projset->label_gapopen = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_gapextend = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_xgapped = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_xgapless = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_xfinal = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_mscoregapless = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_mscoregapped = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_mismatchcost = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_matchscore = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_stepsize = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_morelast = gtk_label_new("");
  gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
  gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);

  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_psmall = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_plarge = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_didclassification = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("Cluster settings");
  gtk_notebook_append_page(GTK_NOTEBOOK(projset->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("allowed LTR length deviation:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("allowed candidate length deviation:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Used features:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = projset->label_ltrtolerance = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_candtolerance = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_usedfeatures = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("Classification settings");
  gtk_notebook_append_page(GTK_NOTEBOOK(projset->notebook), hbox, label);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), projset->notebook, TRUE, TRUE, 1);
  button = gtk_button_new_with_mnemonic("_Close");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(close_clicked), projset);

  align = gtk_alignment_new(1.0, 0.5, 0.3, 0.0);
  gtk_container_add(GTK_CONTAINER(align), button);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_widget_show_all(vbox);

  gtk_container_add(GTK_CONTAINER(projset), vbox);
}

GType gtk_project_settings_get_type(void)
{
  static GType project_settings_type = 0;

  if (!project_settings_type) {
    const GTypeInfo project_settings_info =
    {
      sizeof (GtkProjectSettingsClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkProjectSettings),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_project_settings_init,
    };
    project_settings_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                   "GtkProjectSettings",
                                                   &project_settings_info, 0);
  }
  return project_settings_type;
}

GtkWidget* gtk_project_settings_new(GtRDB *rdb)
{
  GtkProjectSettings *projset;
  gchar title[BUFSIZ];

  projset = gtk_type_new(GTK_PROJECT_SETTINGS_TYPE);
  g_signal_connect(G_OBJECT(projset), "delete_event",
                   G_CALLBACK(delete_event), NULL);
  gtk_window_set_position(GTK_WINDOW(projset), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_modal(GTK_WINDOW(projset), TRUE);
  g_snprintf(title, BUFSIZ, PROJSET_WINDOW_TITLE, GUI_NAME);
  gtk_window_set_title(GTK_WINDOW(projset), title);
  gtk_container_set_border_width(GTK_CONTAINER(projset), 5);
  projset->rdb = rdb;

  return GTK_WIDGET(projset);
}
