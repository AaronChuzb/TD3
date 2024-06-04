/*
 * @Date: 2024-06-04 15:13:48
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-04 17:20:01
 */
#include "Model.h"

static bool loaded = false;

// 配置文件数据
static char *config = NULL;
static size_t config_len = 0;

void *custom_malloc(size_t size)
{
  return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

void custom_free(void *ptr)
{
  heap_caps_free(ptr);
}

void cJSON_Init()
{
  cJSON_Hooks hooks;
  hooks.malloc_fn = custom_malloc;
  hooks.free_fn = custom_free;
  cJSON_InitHooks(&hooks);
}

void load_config2ram()
{
  const char *file_key = "/spifats/config.json";
  FILE *file = fopen(file_key, "r");
  // file = fopen(file_hello, "r");
  if (file == NULL)
  {
    printf("载入配置文件失败，无法打开文件\n");
    return;
  }
  char line[128];
  // 不断读取文件，直到EOF
  while (fgets(line, sizeof(line), file) != NULL)
  {
    size_t new_len = config_len + strlen(line); // 不需要加1，因为fgets已经包含了换行符和null终止符
    // 分配（或重新分配）足够的内存来存储新的字符串
    char *new_content = heap_caps_realloc(config, new_len + 1, MALLOC_CAP_SPIRAM); // 加1是为了新的null终止符
    if (new_content == NULL)
    {
      // 如果realloc失败，释放之前的内存并退出
      heap_caps_free(config);
      printf("Memory allocation failed");
      fclose(file);
      return;
    }
    // 更新指针和config_len
    config = new_content;
    strcpy(config + config_len, line); // 追加新行到config字符串
    config_len = new_len;              // 更新长度（包括null终止符）
  }
  // 确保字符串的末尾有一个null终止符
  config[config_len] = '\0';
  // printf("%s", config);
  // 释放content占用的内存
  loaded = true;
  // heap_caps_free(config);
  fclose(file);
}

void save_config2file()
{
  const char *file_key = "/spifats/config.json";
  FILE *file = fopen(file_key, "w");
  if (file == NULL)
  {
    printf("Error opening file!\n");
  }
  if (fwrite(config, config_len, 1, file) != 1)
  {
    printf("Error writing to file!\n");
    // 清理并返回错误
  }
  fclose(file);
}

char *get_config_item_char(char *key)
{
  if (loaded)
  {
    cJSON *parsed_json = cJSON_Parse(config);
    if (parsed_json != NULL)
    {
      char *content = cJSON_GetObjectItem(parsed_json, key)->valuestring;
      printf("%s: %s\n", key, content);
      cJSON_Delete(parsed_json); // 释放内存
      return content;
    }
  }
  return NULL;
}

bool update_config_item_char(char *key, char *value)
{
  if (loaded)
  {
    cJSON *parsed_json = cJSON_Parse(config);
    if (parsed_json != NULL)
    {
      cJSON_ReplaceItemInObject(parsed_json, key, cJSON_CreateString(value));
      char *content = cJSON_GetObjectItem(parsed_json, key)->valuestring;
      printf("%s: %s\n", key, content);
      char *json_str = cJSON_Print(parsed_json);
      config_len = strlen(json_str) + 1; // 加1是为了新的null终止符
      config = heap_caps_realloc(config, config_len, MALLOC_CAP_SPIRAM);
      strcpy(config, json_str);
      cJSON_Delete(parsed_json); // 释放内存
      save_config2file();
      return true;
    }
  }
  return false;
}

int get_config_item_int(char *key)
{
  if (loaded)
  {
    cJSON *parsed_json = cJSON_Parse(config);
    if (parsed_json != NULL)
    {
      int content = cJSON_GetObjectItem(parsed_json, key)->valueint;
      printf("%s: %d\n", key, content);
      cJSON_Delete(parsed_json); // 释放内存
      return content;
    }
  }
  return 0;
}

bool update_config_item_int(char *key, int value)
{
  if (loaded)
  {
    cJSON *parsed_json = cJSON_Parse(config);
    if (parsed_json != NULL)
    {
      cJSON_ReplaceItemInObject(parsed_json, key, cJSON_CreateNumber((double)value));
      int content = cJSON_GetObjectItem(parsed_json, key)->valueint;
      printf("%s: %d\n", key, content);
      char *json_str = cJSON_Print(parsed_json);
      config_len = strlen(json_str) + 1; // 加1是为了新的null终止符
      config = heap_caps_realloc(config, config_len, MALLOC_CAP_SPIRAM);
      strcpy(config, json_str);
      cJSON_Delete(parsed_json); // 释放内存
      save_config2file();
      return true;
    }
  }
  return false;
}


bool get_config_item_bool(char *key)
{
  if (loaded)
  {
    cJSON *parsed_json = cJSON_Parse(config);
    if (parsed_json != NULL)
    {
      int content = cJSON_GetObjectItem(parsed_json, key)->valueint;
      printf("%s: %d\n", key, content);
      cJSON_Delete(parsed_json); // 释放内存
      return content ? true : false;
    }
  }
  return false;
}

bool update_config_item_bool(char *key, bool value)
{
  if (loaded)
  {
    cJSON *parsed_json = cJSON_Parse(config);
    if (parsed_json != NULL)
    {
      cJSON_ReplaceItemInObject(parsed_json, key, cJSON_CreateBool(value));
      int content = cJSON_GetObjectItem(parsed_json, key)->valueint;
      printf("%s: %d\n", key, content);
      char *json_str = cJSON_Print(parsed_json);
      config_len = strlen(json_str) + 1; // 加1是为了新的null终止符
      config = heap_caps_realloc(config, config_len, MALLOC_CAP_SPIRAM);
      strcpy(config, json_str);
      cJSON_Delete(parsed_json); // 释放内存
      save_config2file();
      return true;
    }
  }
  return false;
}
