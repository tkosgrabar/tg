/*
    This file is part of telegram-client.

    Telegram-client is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Telegram-client is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this telegram-client.  If not, see <http://www.gnu.org/licenses/>.

    Copyright Vitaly Valtman 2013
*/
#ifndef __TREE_H__
#define __TREE_H__
#include <stdio.h>

#include <memory.h>
#include <malloc.h>
#include <assert.h>

#define DEFINE_TREE(X_NAME, X_TYPE, X_CMP, X_UNSET) \
struct tree_ ## X_NAME { \
  struct tree_ ## X_NAME *left, *right;\
  X_TYPE x;\
  int y;\
};\
\
struct tree_ ## X_NAME *new_tree_node_ ## X_NAME (X_TYPE x, int y) {\
  struct tree_ ## X_NAME *T = malloc (sizeof (*T));\
  T->x = x;\
  T->y = y;\
  T->left = T->right = 0;\
  return T;\
}\
\
void delete_tree_node_ ## X_NAME (struct tree_ ## X_NAME *T) {\
  free (T);\
}\
\
void tree_split_ ## X_NAME (struct tree_ ## X_NAME *T, X_TYPE x, struct tree_ ## X_NAME **L, struct tree_ ## X_NAME **R) {\
  if (!T) {\
    *L = *R = 0;\
  } else {\
    int c = X_CMP (x, T->x);\
    if (c < 0) {\
      tree_split_ ## X_NAME (T->left, x, L, &T->left);\
      *R = T;\
    } else {\
      tree_split_ ## X_NAME (T->right, x, &T->right, R);\
      *L = T;\
    }\
  }\
}\
\
struct tree_ ## X_NAME *tree_insert_ ## X_NAME (struct tree_ ## X_NAME *T, X_TYPE x, int y) {\
  if (!T) {\
    return new_tree_node_ ## X_NAME  (x, y);\
  } else {\
    if (y > T->y) {\
      struct tree_ ## X_NAME *N = new_tree_node_ ## X_NAME (x, y);\
      tree_split_ ## X_NAME (T, x, &N->left, &N->right);\
      return N;\
    } else {\
      int c = X_CMP (x, T->x);\
      assert (c);\
      if (c < 0) { \
        T->left = tree_insert_ ## X_NAME (T->left, x, y);\
      } else { \
        T->right = tree_insert_ ## X_NAME (T->right, x, y);\
      } \
      return T; \
    }\
  }\
}\
\
struct tree_ ## X_NAME *tree_merge_ ## X_NAME (struct tree_ ## X_NAME *L, struct tree_ ## X_NAME *R) {\
  if (!L || !R) {\
    return L ? L : R;\
  } else {\
    if (L->y > R->y) {\
      L->right = tree_merge_ ## X_NAME (L->right, R);\
      return L;\
    } else {\
      R->left = tree_merge_ ## X_NAME (L, R->left);\
      return R;\
    }\
  }\
}\
\
struct tree_ ## X_NAME *tree_delete_ ## X_NAME (struct tree_ ## X_NAME *T, X_TYPE x) {\
  assert (T);\
  int c = X_CMP (x, T->x);\
  if (!c) {\
    struct tree_ ## X_NAME *N = tree_merge_ ## X_NAME (T->left, T->right);\
    delete_tree_node_ ## X_NAME (T);\
    return N;\
  } else {\
    if (c < 0) { \
      T->left = tree_delete_ ## X_NAME (T->left, x); \
    } else { \
      T->right = tree_delete_ ## X_NAME (T->right, x); \
    } \
    return T; \
  }\
}\
\
X_TYPE tree_get_min_ ## X_NAME (struct tree_ ## X_NAME *T) {\
  if (!T) { return X_UNSET; } \
  while (T->left) { T = T->left; }\
  return T->x; \
} \
\
X_TYPE tree_lookup_ ## X_NAME (struct tree_ ## X_NAME *T, X_TYPE x) {\
  int c;\
  while (T && (c = X_CMP (x, T->x))) {\
    T = (c < 0 ? T->left : T->right);\
  }\
  return T ? T->x : X_UNSET;\
}\
\
void tree_act_ ## X_NAME (struct tree_ ## X_NAME *T, void (*act)(X_TYPE)) {\
  if (!T) { return; } \
  tree_act_ ## X_NAME (T->left, act); \
  act (T->x); \
  tree_act_ ## X_NAME (T->right, act); \
}\
\
int tree_count_ ## X_NAME (struct tree_ ## X_NAME *T) { \
  if (!T) { return 0; }\
  return 1 + tree_count_ ## X_NAME (T->left) + tree_count_ ## X_NAME (T->right); \
}\
void tree_check_ ## X_NAME (struct tree_ ## X_NAME *T) { \
  if (!T) { return; }\
  if (T->left) { \
    assert (T->left->y <= T->y);\
    assert (X_CMP (T->left->x, T->x) < 0); \
  }\
  if (T->right) { \
    assert (T->right->y <= T->y);\
    assert (X_CMP (T->right->x, T->x) > 0); \
  }\
}\

#define int_cmp(a,b) ((a) - (b))
#endif
