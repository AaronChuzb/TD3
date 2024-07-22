/*
 * @Date: 2024-07-16 15:49:34
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-07-22 15:46:16
 */
#include "HAL.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 20480

static const char *TAG = "HTTP_CLIENT";

extern const char root_cert_pem_start[] asm("_binary_root_cert_pem_start");
extern const char root_cert_pem_end[] asm("_binary_root_cert_pem_end");

void http_native_request(void)
{

  char *output_buffer = (char *)heap_caps_malloc(MAX_HTTP_OUTPUT_BUFFER, MALLOC_CAP_SPIRAM); // Buffer to store response of http request
  int content_length = 0;
  esp_http_client_config_t config = {
      .url = "https://devapi.qweather.com/v7/weather/now?key=c2309af516b04492a222943c075683c3&location=101010100",
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);

  // GET Request
  esp_http_client_set_method(client, HTTP_METHOD_GET);
  esp_err_t err = esp_http_client_open(client, 0);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
  }
  else
  {
    content_length = esp_http_client_fetch_headers(client);
    if (content_length < 0)
    {
      ESP_LOGE(TAG, "HTTP client fetch headers failed");
    }
    else
    {
      int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
      if (data_read >= 0)
      {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRIu64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
        ESP_LOG_BUFFER_HEX(TAG, output_buffer, data_read);
      }
      else
      {
        ESP_LOGE(TAG, "Failed to read response");
      }
    }
  }
  // esp_http_client_close(client);
  esp_http_client_cleanup(client);

  // POST Request
  // const char *post_data = "{\"field1\":\"value1\"}";
  // esp_http_client_set_url(client, "http://"CONFIG_EXAMPLE_HTTP_ENDPOINT"/post");
  // esp_http_client_set_method(client, HTTP_METHOD_POST);
  // esp_http_client_set_header(client, "Content-Type", "application/json");
  // err = esp_http_client_open(client, strlen(post_data));
  // if (err != ESP_OK) {
  //     ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
  // } else {
  //     int wlen = esp_http_client_write(client, post_data, strlen(post_data));
  //     if (wlen < 0) {
  //         ESP_LOGE(TAG, "Write failed");
  //     }
  //     content_length = esp_http_client_fetch_headers(client);
  //     if (content_length < 0) {
  //         ESP_LOGE(TAG, "HTTP client fetch headers failed");
  //     } else {
  //         int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
  //         if (data_read >= 0) {
  //             ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRIu64,
  //             esp_http_client_get_status_code(client),
  //             esp_http_client_get_content_length(client));
  //             ESP_LOG_BUFFER_HEX(TAG, output_buffer, strlen(output_buffer));
  //         } else {
  //             ESP_LOGE(TAG, "Failed to read response");
  //         }
  //     }
  // }
  // esp_http_client_cleanup(client);
}

void http_native_request_get(void)
{
  char *output_buffer = (char *)heap_caps_malloc(MAX_HTTP_OUTPUT_BUFFER + 1, MALLOC_CAP_SPIRAM); // Buffer to store response of http request
  if (output_buffer == NULL)
  {
    ESP_LOGE(TAG, "分配输出缓冲区内存失败");
    return;
  }
  esp_http_client_config_t config = {
      .url = "https://api.seniverse.com/v3/weather/now.json?key=xxxx&location=changsha&language=zh-Hans&unit=c",
      .cert_pem = root_cert_pem_start,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);

  // GET Request
  esp_http_client_set_method(client, HTTP_METHOD_GET);
  // esp_http_client_set_header(client, "accept", "/");
  esp_http_client_set_header(client, "Content-Type", "application/json;charset=UTF-8");
  esp_err_t err = esp_http_client_open(client, 0);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
  }
  else
  {
    int content_length = esp_http_client_fetch_headers(client);
    if (content_length < 0)
    {
      ESP_LOGE(TAG, "HTTP client fetch headers failed");
    }
    else
    {
      int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
      if (data_read >= 0)
      {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRIu64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));

        // printf("%s", output_buffer);
        cJSON *json = cJSON_Parse(output_buffer);
        if (json == NULL)
        {
          const char *error_ptr = cJSON_GetErrorPtr();
          if (error_ptr != NULL)
          {
            ESP_LOGE(TAG, "JSON Parse Error: %s", error_ptr);
          }
          ESP_LOGE(TAG, "解析JSON失败");
        }
        else
        {
          // 提取字段
          cJSON *results = cJSON_GetObjectItem(json, "results");
          if (results && cJSON_IsArray(results))
          {
            cJSON *result = cJSON_GetArrayItem(results, 0);
            if (result)
            {
              cJSON *location = cJSON_GetObjectItem(result, "location");
              if (location)
              {
                cJSON *id = cJSON_GetObjectItem(location, "id");
                cJSON *name = cJSON_GetObjectItem(location, "name");
                cJSON *country = cJSON_GetObjectItem(location, "country");
                cJSON *path = cJSON_GetObjectItem(location, "path");
                cJSON *timezone = cJSON_GetObjectItem(location, "timezone");
                cJSON *timezone_offset = cJSON_GetObjectItem(location, "timezone_offset");

                if (id && cJSON_IsString(id))
                {
                  printf("id: %s\n", id->valuestring);
                }
                if (name && cJSON_IsString(name))
                {
                  printf("name: %s\n", name->valuestring);
                }
                if (country && cJSON_IsString(country))
                {
                  printf("country: %s\n", country->valuestring);
                }
                if (path && cJSON_IsString(path))
                {
                  printf("path: %s\n", path->valuestring);
                }
                if (timezone && cJSON_IsString(timezone))
                {
                  printf("timezone: %s\n", timezone->valuestring);
                }
                if (timezone_offset && cJSON_IsString(timezone_offset))
                {
                  printf("timezone_offset: %s\n", timezone_offset->valuestring);
                }
              }

              cJSON *now = cJSON_GetObjectItem(result, "now");
              if (now)
              {
                cJSON *text = cJSON_GetObjectItem(now, "text");
                cJSON *code = cJSON_GetObjectItem(now, "code");
                cJSON *temperature = cJSON_GetObjectItem(now, "temperature");

                if (text && cJSON_IsString(text))
                {
                  printf("text: %s\n", text->valuestring);
                }
                if (code && cJSON_IsString(code))
                {
                  printf("code: %s\n", code->valuestring);
                }
                if (temperature && cJSON_IsString(temperature))
                {
                  printf("temperature: %s\n", temperature->valuestring);
                }
              }

              cJSON *last_update = cJSON_GetObjectItem(result, "last_update");
              if (last_update && cJSON_IsString(last_update))
              {
                printf("last_update: %s\n", last_update->valuestring);
              }
            }
          }
          // 释放JSON对象
          cJSON_Delete(json);
        }
      }
      else
      {
        ESP_LOGE(TAG, "Failed to read response");
      }
    }
  }
  // esp_http_client_close(client);
  esp_http_client_cleanup(client);
  heap_caps_free(output_buffer);
}

void http_native_request_get_task(void *pvParameters)
{
  http_native_request_get();
  vTaskDelete(NULL);
}

void http_process(void)
{
  xTaskCreateWithCaps(http_native_request_get_task, "http_native_request_get_task", 20 * 1024, NULL, 1, NULL, MALLOC_CAP_SPIRAM);
}