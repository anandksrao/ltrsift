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

#ifndef PROJECT_WIZARD_H
#define PROJECT_WIZARD_H

#include "ltrgui.h"

#define PW_SELECT_GFF3_FILES "Select GFF3 files..."

typedef struct _PWThreadData PWThreadData;

typedef enum {
  PW_INTRODUCTION = 0,
  PW_SELECTFILES,
  PW_CLASSIFICATION,
  PW_SUMMARY
} ProjectWizardPages;

struct _PWThreadData {
  GUIData *ltrgui;
  GtkWidget *window,
            *progressbar;
  GtArray *nodes;
  GtHashmap *features;
  unsigned long had_err,
                progress,
                n_features;
  const gchar *fullname;
  gchar *projectfile,
        *projectdir;
  char *current_state;
  GtError *err;
};

void pw_init(GUIData *ltrgui);

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui);

#endif /* PROJECT_WIZARD_H */
