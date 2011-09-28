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

#ifndef NOTEBOOK_FAMILIES_H
#define NOTEBOOK_FAMILIES_H

#include "ltrgui.h"

void nb_families_page_reordered(GtkNotebook *notebook, GtkWidget *child,
                                guint page_num, GUIData *ltrgui);

void nb_families_close_tab_clicked(GtkButton *button, GUIData *ltrgui);

void nb_families_add_tab(GtkTreeModel *model, GtkTreeIter *iter,
                         GUIData *ltrgui);

void nb_families_refresh_notebook_nums(GtkNotebook *notebook);

void nb_families_init(GUIData *ltrgui);

#endif /* NOTEBOOK_FAMILIES_H */