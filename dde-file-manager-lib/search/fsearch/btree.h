/**
 * Copyright (C) 2017 Uniontech Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#pragma once

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct _BTreeNode BTreeNode;

struct _BTreeNode
{
    BTreeNode *next;
    BTreeNode *parent;
    BTreeNode *children;

    // data
    char *name;

    time_t mtime;
    off_t size;
    uint32_t pos;
    bool is_dir;
};

BTreeNode *
btree_node_new (const char *name,
                time_t mtime,
                off_t size,
                uint32_t pos,
                bool is_dir);

void
btree_node_free (BTreeNode *node);

void
btree_node_unlink (BTreeNode *node);

BTreeNode *
btree_node_append (BTreeNode *parent, BTreeNode *node);

BTreeNode *
btree_node_prepend (BTreeNode *parent, BTreeNode *node);

void
btree_node_remove (BTreeNode *node);

BTreeNode *
btree_node_get_root (BTreeNode *node);

bool
btree_node_is_root (BTreeNode *node);

uint32_t
btree_node_n_nodes (BTreeNode *node);

uint32_t
btree_node_depth (BTreeNode *node);

uint32_t
btree_node_n_children (BTreeNode *node);

bool
btree_node_has_children (BTreeNode *node);

void
btree_node_children_foreach (BTreeNode *node,
                             void (*func)(BTreeNode *, void *),
                             void *data);
void
btree_node_traverse (BTreeNode *node,
                     bool (*func)(BTreeNode *, void *),
                     void *data);
bool
btree_node_get_path (BTreeNode *node, char *path, size_t path_len);

bool
btree_node_get_path_full (BTreeNode *node, char *path, size_t path_len);
