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
#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#define FLAG_EMPTY 1
#define FLAG_DELETED 2
#define FLAG_FORBIDDEN 4
#define FLAG_HAS_PHOTO 8

#define FLAG_USER_SELF 128
#define FLAG_USER_FOREIGN 256
#define FLAG_USER_CONTACT 512
#define FLAG_USER_IN_CONTACT 1024
#define FLAG_USER_OUT_CONTACT 2048

#define FLAG_CHAT_IN_CHAT 128

struct file_location {
  int dc;
  long long volume;
  int local_id;
  long long secret;
};

struct photo_size {
  char *type;
  struct file_location loc;
  int w;
  int h;
  int size;
  char *data;
};

struct geo {
  double longitude;
  double latitude;
};

struct photo {
  long long id;
  long long access_hash;
  int user_id;
  int date;
  char *caption;
  struct geo geo;
  int sizes_num;
  struct photo_size *sizes;
};

struct user_status {
  int online;
  int when;
};

struct user {
  int id;
  int flags;
  char *print_name;
  struct file_location photo_big;
  struct file_location photo_small;
  struct photo photo;
  char *first_name;
  char *last_name;
  char *phone;
  long long access_hash;
  struct user_status status;
  int blocked;
  char *real_first_name;
  char *real_last_name;
};

struct chat_user {
  int user_id;
  int inviter_id;
  int date;
};

struct chat {
  int id;
  int flags;
  char *print_title;
  struct file_location photo_big;
  struct file_location photo_small;
  struct photo photo;
  char *title;
  int users_num;
  struct chat_user *users;
  int date;
  int version;
  int admin_id;
};

union user_chat {
  struct {
    int id;
    int flags;
    char *print_name;
    struct file_location photo_big;
    struct file_location photo_small;
    struct photo photo;
  };
  struct user user;
  struct chat chat;
};

struct video {
  long long id;
  long long access_hash;
  int user_id;
  int date;
  char *caption;
  int duration;
  int size;
  struct photo_size thumb;
  int dc_id;
  int w;
  int h;
};

struct message_action {
  int type;
  union {
    struct {
      char *title;
      int user_num;
      int *users;
    };
    char *new_title;
    struct photo photo;
    int user;
  };
};

struct message_media {
  int type;
  union {
    struct photo photo;
    struct video video;
    struct geo geo;
    struct {
      char *phone;
      char *first_name;
      char *last_name;
      int user_id;
    };
    void *data;
  };
};

struct message {
  struct message *next_use, *prev_use;
  int id;
  int flags;
  int fwd_from_id;
  int fwd_date;
  int from_id;
  int to_id;
  int out;
  int unread;
  int date;
  int service;
  union {
    struct message_action action;
    struct {
      char *message;
      int message_len;
      struct message_media media;
    };
  };
};

void fetch_file_location (struct file_location *loc);
void fetch_user_status (struct user_status *S);
void fetch_user (struct user *U);
struct user *fetch_alloc_user (void);
struct user *fetch_alloc_user_full (void);
struct chat *fetch_alloc_chat (void);
struct chat *fetch_alloc_chat_full (void);
struct message *fetch_alloc_message (void);
struct message *fetch_alloc_message_short (void);
struct message *fetch_alloc_message_short_chat (void);
int fetch_peer_id (void);

void free_user (struct user *U);
void free_chat (struct chat *U);

int print_stat (char *s, int len);
union user_chat *user_chat_get (int id);
struct message *message_get (int id);
void update_message_id (struct message *M, int id);
void message_insert (struct message *M);
void free_photo (struct photo *P);
void fetch_photo (struct photo *P);
#endif
