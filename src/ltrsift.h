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

#ifndef LTRSIFT_H
#define LTRSIFT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "genometools.h"
#include "gtk_blastn_params.h"
#include "gtk_blastn_params_refseq.h"
#include "gtk_label_close.h"
#include "gtk_ltr_assistant.h"
#include "gtk_ltr_families.h"
#include "gtk_ltr_filter.h"
#include "gtk_project_settings.h"
#include "preprocess_stream.h"
#include "script_filter_stream.h"

typedef struct _GUIData GUIData;

struct _GUIData
{
  GtkWidget *menubar;
  GtkWidget *menubar_save;
  GtkWidget *menubar_save_as;
  GtkWidget *menubar_export;
  GtkWidget *menubar_close;
  GtkWidget *menubar_project;
  GtkWidget *menubar_view_columns;
  GtkWidget *vbox;
  GtkWidget *ltrfams;
  GtkWidget *ltrfilt;
  GtkWidget *projset;
  GtkWidget *progressbar;
  GtkWidget *statusbar;
  GtkWidget *main_window;
  GtkWidget *assistant;
  guint statusbar_context_id;
  gchar *style_file;
  GtError *err;
  GtHashmap *refseq_paramsets;
};

gint apply_gui_settings(GUIData *ltrgui);
gint save_gui_settings(GUIData *ltrgui);

#endif
