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

#ifndef __GTK_LTR_FAMILIES_H__
#define __GTK_LTR_FAMILIES_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "gtk_label_close.h"
#include "genometools.h"

#define GTK_LTR_FAMILIES_TYPE\
        gtk_ltr_families_get_type()
#define GTK_LTR_FAMILIES(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_LTR_FAMILIES_TYPE, GtkLTRFamilies)
#define GTK_LTR_FAMILIES_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_LTR_FAMILIES_TYPE,\
                                GtkLTRFamiliesClass)
#define IS_GTK_LTR_FAMILIES(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_LTR_FAMILIES_TYPE)
#define IS_GTK_LTR_FAMILIES_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_LTR_FAMILIES_TYPE)

#define LTRFAMS_LV_CAPTION_SEQID "SeqID"
#define LTRFAMS_LV_CAPTION_TYPE  "S"
#define LTRFAMS_LV_CAPTION_START "Start"
#define LTRFAMS_LV_CAPTION_END   "End"
#define LTRFAMS_TV_CAPTION_INFO  "Attributes"
#define LTRFAMS_TV_CAPTION_TYPE  "Feature name"

#define ATTR_RID       "ID"
#define ATTR_CLUSTID   "clid"
#define ATTR_PFAMN     "pfamname"
#define FNT_PROTEINM   "protein_match"
#define FNT_LTR        "long_terminal_repeat"
#define FNT_REPEATR    "repeat_region"
#define FNT_LLTR       "lLTR"
#define FNT_RLTR       "rLTR"
#define LTRFAM_DETINFO "Detailed Information"

#define TB_FAMS_ADD "Add new family"

typedef struct _FamilyTransferData  FamilyTransferData;
typedef struct _GtkLTRFamilies      GtkLTRFamilies;
typedef struct _GtkLTRFamiliesClass GtkLTRFamiliesClass;

enum {
  LTRFAMS_LV_NODE = 0,
  LTRFAMS_LV_FEAT,
  LTRFAMS_LV_ROWREF,
  LTRFAMS_LV_SEQID,
  LTRFAMS_LV_TYPE,
  LTRFAMS_LV_START,
  LTRFAMS_LV_END,
  LTRFAMS_LV_N_COLUMS
};

enum {
  LTRFAMS_DETAIL_TV_NODE = 0,
  LTRFAMS_DETAIL_TV_TYPE,
  LTRFAMS_DETAIL_TV_START,
  LTRFAMS_DETAIL_TV_END,
  LTRFAMS_DETAIL_TV_INFO,
  LTRFAMS_DETAIL_TV_N_COLUMS
};

enum {
  LTRFAMS_FAM_LV_NODE_ARRAY = 0,
  LTRFAMS_FAM_LV_TAB_CHILD,
  LTRFAMS_FAM_LV_TAB_LABEL,
  LTRFAMS_FAM_LV_CURNAME,
  LTRFAMS_FAM_LV_OLDNAME,
  LTRFAMS_FAM_LV_N_COLUMS
};

struct _FamilyTransferData
{
  GtGenomeNode **gn;
  GtHashmap *feat;
};

typedef enum {
  TARGET_STRING = 0
} FamilyTargets;

static const GtkTargetEntry family_drag_targets[] = {
  {"STRING", 0, TARGET_STRING}
};

struct _GtkLTRFamilies
{
  GtkHPaned hpane;

  GtkWidget *lv_families;
  GtkWidget *nb_family;
  GtkWidget *tv_details;
  GtkWidget *image_area;
  GtDiagram *diagram;
  GtStyle *style;
  GtArray *nodes;
  GtHashmap *features;
  GtError *err;
  unsigned long n_features;
};

struct _GtkLTRFamiliesClass
{
  GtkHPanedClass parent_class;
  void (* gtk_ltr_families) (GtkLTRFamilies *ltrfams);
};

GType      gtk_ltr_families_get_type(void);

GtkWidget* gtk_ltr_families_new();

char* double_underscores(const char *str);

void       gtk_ltr_families_fill_with_data(GtkLTRFamilies *ltrfams,
                                           GtArray *nodes,
                                           GtHashmap *features,
                                           unsigned long noc);

GtkNotebook* gtk_ltr_families_get_nb(GtkLTRFamilies *ltrfams);

void gtk_ltr_families_clear(GtkLTRFamilies *ltrfams);

#endif /* GTK_LTR_FAMILIES_H */