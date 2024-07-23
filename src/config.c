// Abstract City - Jed Fakhfekh - https://github.com/ougi-washi
#include "config.h"
#include "utility.h"
#include "defines.h"
#include "log.h"
#include "tiny-json.h"
#include <corecrt_malloc.h>

static json_t json_pool[256];
static const json_t* json_config = NULL;
static c8* json_content = NULL;

void ac_config_load() {
    FILE* file = fopen(AC_CONFIG_FILE, "r");
    if (file == NULL) {
        log_error("Failed to open config file: %s", AC_CONFIG_FILE);
        return;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    json_content = (c8*)malloc(length + 1);
    fread(json_content, 1, length, file);
    json_content[length] = '\0';
    fclose(file);

    json_config = json_create(json_content, json_pool, sizeof(json_pool) / sizeof(json_pool[0]));
}

void ac_config_unload()
{
    free(json_content);
    json_config = NULL;
}

i32 ac_config_get_int(const c8* key) {
    json_t const* field = json_getProperty(json_config, key);
    return field ? (i32)json_getInteger(field) : 0;
}

f32 ac_config_get_float(const c8* key) {
    json_t const* field = json_getProperty(json_config, key);
    return field ? (f32)json_getReal(field) : 0.0f;
}

c8* ac_config_get_string(const c8* key) {
    json_t const* field = json_getProperty(json_config, key);
    return field ? (c8*)json_getValue(field) : NULL;
}

b8 ac_config_get_bool(const c8* key) {
    json_t const* field = json_getProperty(json_config, key);
    return field ? json_getBoolean(field) : 0;
}

b8 ac_config_get_int_array(const c8* key, i32* out, i32* count) {
    json_t const* arr = json_getProperty(json_config, key);
    if (!arr || json_getType(arr) != JSON_ARRAY) return 0;
    i32 i = 0;
    json_t const* elem;
    while ((elem = json_getChild(arr))) {
        if (i < *count) {
            out[i++] = (i32)json_getInteger(elem);
            arr = json_getSibling(elem);
        } else break;
    }
    *count = i;
    return 1;
}

b8 ac_config_get_float_array(const c8* key, f32* out, i32* count) {
    json_t const* arr = json_getProperty(json_config, key);
    if (!arr || json_getType(arr) != JSON_ARRAY) return 0;
    i32 i = 0;
    json_t const* elem;
    while ((elem = json_getChild(arr))) {
        if (i < *count) {
            out[i++] = (f32)json_getReal(elem);
            arr = json_getSibling(elem);
        } else break;
    }
    *count = i;
    return 1;
}

b8 ac_config_get_string_array(const c8* key, c8** out, i32* count) {
    json_t const* arr = json_getProperty(json_config, key);
    if (!arr || json_getType(arr) != JSON_ARRAY) return 0;
    i32 i = 0;
    json_t const* elem;
    while ((elem = json_getChild(arr))) {
        if (i < *count) {
            out[i++] = (c8*)json_getValue(elem);
            arr = json_getSibling(elem);
        } else break;
    }
    *count = i;
    return 1;
}

b8 ac_config_get_bool_array(const c8* key, b8* out, i32* count) {
    json_t const* arr = json_getProperty(json_config, key);
    if (!arr || json_getType(arr) != JSON_ARRAY) return 0;
    i32 i = 0;
    json_t const* elem;
    while ((elem = json_getChild(arr))) {
        if (i < *count) {
            out[i++] = json_getBoolean(elem);
            arr = json_getSibling(elem);
        } else break;
    }
    *count = i;
    return 1;
}