// Copyright 2013 Tanel Lebedev

#ifndef SRC_KOPSIK_API_H_
#define SRC_KOPSIK_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>

#define KOPSIK_EXPORT

#define KOPSIK_ERR_LEN 1024

typedef int kopsik_api_result;
#define KOPSIK_API_SUCCESS 0

// Context API.

typedef struct {
  void *db;
  void *current_user;
  void *https_client;
  void *ws_client;
  void *mutex;
  void *tm;
  void *change_callback;
} KopsikContext;

KOPSIK_EXPORT KopsikContext *kopsik_context_init();

KOPSIK_EXPORT void kopsik_context_clear(KopsikContext *ctx);

// Configuration API

KOPSIK_EXPORT void kopsik_version(
  int *major, int *minor, int *patch);

KOPSIK_EXPORT void kopsik_user_agent(
  char *str, unsigned int len);

KOPSIK_EXPORT void kopsik_set_proxy(KopsikContext *ctx,
  const char *host, const unsigned int port,
  const char *username, const char *password);

KOPSIK_EXPORT void kopsik_set_db_path(KopsikContext *ctx,
  const char *path);

KOPSIK_EXPORT void kopsik_set_log_path(KopsikContext *ctx,
  const char *path);

KOPSIK_EXPORT void kopsik_set_log_level(KopsikContext *ctx,
  const char *level);

KOPSIK_EXPORT void kopsik_set_api_url(KopsikContext *ctx,
  const char *api_url);

KOPSIK_EXPORT void kopsik_set_websocket_url(KopsikContext *ctx,
  const char *websocket_url);

// User API

typedef struct {
    unsigned int ID;
    char *Fullname;
} KopsikUser;

KOPSIK_EXPORT KopsikUser *kopsik_user_init();

KOPSIK_EXPORT void kopsik_user_clear(KopsikUser *user);

KOPSIK_EXPORT kopsik_api_result kopsik_set_api_token(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *api_token);

KOPSIK_EXPORT kopsik_api_result kopsik_get_api_token(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  char *str, unsigned int max_strlen);

KOPSIK_EXPORT kopsik_api_result kopsik_current_user(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  KopsikUser *user);

KOPSIK_EXPORT kopsik_api_result kopsik_login(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *email, const char *password);

KOPSIK_EXPORT kopsik_api_result kopsik_logout(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen);

// Sync

typedef struct {
  unsigned int TimeEntries;
} KopsikPushableModelStats;

KOPSIK_EXPORT kopsik_api_result kopsik_sync(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  int full_sync);

KOPSIK_EXPORT kopsik_api_result kopsik_push(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen);

KOPSIK_EXPORT kopsik_api_result kopsik_pushable_models(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  KopsikPushableModelStats *stats);

typedef void (*KopsikResultCallback)(
  kopsik_api_result result,
  // NB! you need to free() the memory yourself
  char *errmsg,
  // Length of the returned error string.
  unsigned int errlen);

KOPSIK_EXPORT void kopsik_sync_async(
  KopsikContext *ctx,
  int full_sync,
  KopsikResultCallback callback);

KOPSIK_EXPORT void kopsik_push_async(
  KopsikContext *ctx,
  KopsikResultCallback callback);

// Project list

typedef struct {
  char *Name;
} KopsikProjectSelectItem;

typedef struct {
  KopsikProjectSelectItem **ViewItems;
  unsigned int Length;
} KopsikProjectSelectItemList;

KOPSIK_EXPORT KopsikProjectSelectItemList *
  kopsik_project_select_item_list_init();

KOPSIK_EXPORT void kopsik_project_select_item_list_clear(
  KopsikProjectSelectItemList *list);

KOPSIK_EXPORT kopsik_api_result kopsik_project_select_items(
    KopsikContext *ctx,
    char *errmsg, unsigned int errlen,
    KopsikProjectSelectItemList *list);

// Time entries view

typedef struct {
  int DurationInSeconds;
  char *Description;
  char *Project;
  char *Duration;
  char *Color;
  char *GUID;
  int Billable;
  char *Tags;
  unsigned int Started;
  unsigned int Ended;
  char *UpdatedAt;
} KopsikTimeEntryViewItem;

typedef struct {
  KopsikTimeEntryViewItem **ViewItems;
  unsigned int Length;
} KopsikTimeEntryViewItemList;

KOPSIK_EXPORT KopsikTimeEntryViewItem *
  kopsik_time_entry_view_item_init();

KOPSIK_EXPORT void kopsik_time_entry_view_item_clear(
  KopsikTimeEntryViewItem *item);

KOPSIK_EXPORT kopsik_api_result kopsik_running_time_entry_view_item(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  KopsikTimeEntryViewItem *item,
  int *is_tracking);

KOPSIK_EXPORT void kopsik_format_duration_in_seconds(
  int duration_in_seconds, char *str, unsigned int max_strlen);

KOPSIK_EXPORT kopsik_api_result kopsik_start(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *description,
  KopsikTimeEntryViewItem *item);

KOPSIK_EXPORT kopsik_api_result kopsik_time_entry_view_item_by_guid(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  KopsikTimeEntryViewItem *item,
  int *was_found);

KOPSIK_EXPORT kopsik_api_result kopsik_continue(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  KopsikTimeEntryViewItem *item);

KOPSIK_EXPORT kopsik_api_result kopsik_delete_time_entry(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_duration(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  const char *value);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_project(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  const char *value);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_start_iso_8601(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  const char *value);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_end_iso_8601(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  const char *value);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_tags(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  const char *value);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_billable(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  int value);

KOPSIK_EXPORT kopsik_api_result kopsik_set_time_entry_description(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  const char *guid,
  const char *value);

KOPSIK_EXPORT kopsik_api_result kopsik_stop(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  KopsikTimeEntryViewItem *item);

KOPSIK_EXPORT KopsikTimeEntryViewItemList *
  kopsik_time_entry_view_item_list_init();

KOPSIK_EXPORT void kopsik_time_entry_view_item_list_clear(
  KopsikTimeEntryViewItemList *list);

KOPSIK_EXPORT kopsik_api_result kopsik_time_entry_view_items(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen,
  KopsikTimeEntryViewItemList *list);

// Websocket client

typedef struct {
  char *ModelType;
  char *ChangeType;
  unsigned int ModelID;
  char *GUID;
} KopsikModelChange;

typedef void (*KopsikViewItemChangeCallback)(
  kopsik_api_result result,
  // NB! you need to free() the memory yourself
  char *errmsg,
  // Length of the returned error string.
  unsigned int errlen,
  // Actual change, if no errors
  KopsikModelChange *change);

KOPSIK_EXPORT void kopsik_set_change_callback(
  KopsikContext *ctx,
  KopsikViewItemChangeCallback callback);

KOPSIK_EXPORT kopsik_api_result kopsik_websocket_start(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen);

KOPSIK_EXPORT kopsik_api_result kopsik_websocket_stop(
  KopsikContext *ctx,
  char *errmsg, unsigned int errlen);

KOPSIK_EXPORT void kopsik_websocket_start_async(
  KopsikContext *ctx,
  KopsikResultCallback callback);

KOPSIK_EXPORT void kopsik_websocket_stop_async(
  KopsikContext *ctx,
  KopsikResultCallback callback);


#undef KOPSIK_EXPORT

#ifdef __cplusplus
}
#endif

#endif  // SRC_KOPSIK_API_H_
