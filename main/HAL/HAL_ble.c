
// 先配置menuconfig 选项需要先勾选Bluetooth，然后勾选Enable BLE 4.2 features
#include "HAL.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * DEFINES
 ****************************************************************************************
 */

#define HRPS_HT_MEAS_MAX_LEN (13)

#define HRPS_MANDATORY_MASK (0x0F)
#define HRPS_BODY_SENSOR_LOC_MASK (0x30)
#define HRPS_HR_CTNL_PT_MASK (0xC0)

#define GATTS_TABLE_TAG "SEC_GATTS_DEMO"

#define PROFILE_NUM 1
#define PROFILE_APP_IDX 0
#define ESP_APP_ID 0x55
#define DEVICE_NAME "TD3"
#define SVC_INST_ID 0

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 500

#define ADV_CONFIG_FLAG (1 << 0)
#define SCAN_RSP_CONFIG_FLAG (1 << 1)

static uint8_t adv_config_done = 0;

static uint16_t handle_table[HRS_IDX_NB];

static uint8_t test_manufacturer[3] = {'T', 'D', '3'};

static uint8_t service_uuid[16] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    // first uuid, 16bit, [12],[13] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0x0a,
    0x18,
    0x00,
    0x00,
};

// config adv data
static esp_ble_adv_data_t adv_config = {
    .set_scan_rsp = false,
    .include_txpower = true,
    .min_interval = 0x0006, // slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, // slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0,       // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(service_uuid),
    .p_service_uuid = service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = sizeof(test_manufacturer),
    .p_manufacturer_data = test_manufacturer,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(service_uuid),
    .p_service_uuid = service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x100,
    .adv_int_max = 0x100,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_RPA_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

struct gatts_profile_inst
{
  esp_gatts_cb_t gatts_cb;
  uint16_t gatts_if;
  uint16_t app_id;
  uint16_t conn_id;
  uint16_t service_handle;
  esp_gatt_srvc_id_t service_id;
  uint16_t char_handle;
  esp_bt_uuid_t char_uuid;
  esp_gatt_perm_t perm;
  esp_gatt_char_prop_t property;
  uint16_t descr_handle;
  esp_bt_uuid_t descr_uuid;
};

static void gatts_profile_event_handler(esp_gatts_cb_event_t event,
                                        esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst heart_rate_profile_tab[PROFILE_NUM] = {
    [PROFILE_APP_IDX] = {
        .gatts_cb = gatts_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },

};

/*
 *  Heart Rate PROFILE ATTRIBUTES
 ****************************************************************************************
 */

/// Heart Rate Sensor Service
// static const uint16_t heart_rate_svc = ESP_GATT_UUID_HEART_RATE_SVC;

#define CHAR_DECLARATION_SIZE (sizeof(uint8_t))
static const uint16_t primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
// static const uint8_t char_prop_notify = ESP_GATT_CHAR_PROP_BIT_NOTIFY;
// static const uint8_t char_prop_read = ESP_GATT_CHAR_PROP_BIT_READ;
// static const uint8_t char_prop_read_write = ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_READ;
static const uint8_t char_prop_read_write_notify = ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;

/// Heart Rate Sensor Service - Heart Rate Measurement Characteristic, notify
// static const uint16_t heart_rate_meas_uuid = ESP_GATT_HEART_RATE_MEAS;
static const uint8_t heart_measurement_ccc[2] = {0x00, 0x00};

/// Heart Rate Sensor Service -Body Sensor Location characteristic, read
// static const uint16_t body_sensor_location_uuid = ESP_GATT_BODY_SENSOR_LOCATION;
// static const uint8_t body_sensor_loc_val[1] = {0x00};

/// Heart Rate Sensor Service - Heart Rate Control Point characteristic, write&read
// static const uint16_t heart_rate_ctrl_point = ESP_GATT_HEART_RATE_CNTL_POINT;
// static const uint8_t heart_ctrl_point[1] = {0x00};

static const uint8_t temp_value[1] = {0x00};
// static const uint16_t GATTS_SERVICE_UUID_TEST = 0x00FF;
static const uint16_t GATTS_CHAR_UUID_TEST_TEMP = 0xFF01;
static const uint16_t svc = 0x00FF;

/// Full HRS Database Description - Used to add attributes into the database
static const esp_gatts_attr_db_t gatt_db[HRS_IDX_NB] =
    {
        // Heart Rate Service Declaration
        [IDX_SVC] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(svc), (uint8_t *)&svc}},

        // // Heart Rate Measurement Characteristic Declaration
        // [HRS_IDX_HR_MEAS_CHAR] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_notify}},

        // // Heart Rate Measurement Characteristic Value
        // [HRS_IDX_HR_MEAS_VAL] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&heart_rate_meas_uuid, ESP_GATT_PERM_READ, HRPS_HT_MEAS_MAX_LEN, 0, NULL}},

        // // Heart Rate Measurement Characteristic - Client Characteristic Configuration Descriptor
        // [HRS_IDX_HR_MEAS_NTF_CFG] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(heart_measurement_ccc), (uint8_t *)heart_measurement_ccc}},

        // // Body Sensor Location Characteristic Declaration
        // [HRS_IDX_BOBY_SENSOR_LOC_CHAR] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

        // // Body Sensor Location Characteristic Value
        // [HRS_IDX_BOBY_SENSOR_LOC_VAL] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&body_sensor_location_uuid, ESP_GATT_PERM_READ_ENCRYPTED, sizeof(uint8_t), sizeof(body_sensor_loc_val), (uint8_t *)body_sensor_loc_val}},

        [IDX_CHAR_TEMP] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write_notify}},

        /* Characteristic Value */
        [IDX_CHAR_VAL_TEMP] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_TEST_TEMP, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(temp_value), (uint8_t *)temp_value}},

        // /* Client Characteristic Configuration Descriptor */
        [IDX_CHAR_CFG_TEMP] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(heart_measurement_ccc), (uint8_t *)heart_measurement_ccc}},

        // // Heart Rate Control Point Characteristic Declaration
        // [HRS_IDX_HR_CTNL_PT_CHAR] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write}},

        // // Heart Rate Control Point Characteristic Value
        // [HRS_IDX_HR_CTNL_PT_VAL] =
        //     {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&heart_rate_ctrl_point, ESP_GATT_PERM_WRITE_ENCRYPTED | ESP_GATT_PERM_READ_ENCRYPTED, sizeof(uint8_t), sizeof(heart_ctrl_point), (uint8_t *)heart_ctrl_point}},
};

static char *esp_key_type_to_str(esp_ble_key_type_t key_type)
{
  char *key_str = NULL;
  switch (key_type)
  {
  case ESP_LE_KEY_NONE:
    key_str = "ESP_LE_KEY_NONE";
    break;
  case ESP_LE_KEY_PENC:
    key_str = "ESP_LE_KEY_PENC";
    break;
  case ESP_LE_KEY_PID:
    key_str = "ESP_LE_KEY_PID";
    break;
  case ESP_LE_KEY_PCSRK:
    key_str = "ESP_LE_KEY_PCSRK";
    break;
  case ESP_LE_KEY_PLK:
    key_str = "ESP_LE_KEY_PLK";
    break;
  case ESP_LE_KEY_LLK:
    key_str = "ESP_LE_KEY_LLK";
    break;
  case ESP_LE_KEY_LENC:
    key_str = "ESP_LE_KEY_LENC";
    break;
  case ESP_LE_KEY_LID:
    key_str = "ESP_LE_KEY_LID";
    break;
  case ESP_LE_KEY_LCSRK:
    key_str = "ESP_LE_KEY_LCSRK";
    break;
  default:
    key_str = "INVALID BLE KEY TYPE";
    break;
  }

  return key_str;
}

static char *esp_auth_req_to_str(esp_ble_auth_req_t auth_req)
{
  char *auth_str = NULL;
  switch (auth_req)
  {
  case ESP_LE_AUTH_NO_BOND:
    auth_str = "ESP_LE_AUTH_NO_BOND";
    break;
  case ESP_LE_AUTH_BOND:
    auth_str = "ESP_LE_AUTH_BOND";
    break;
  case ESP_LE_AUTH_REQ_MITM:
    auth_str = "ESP_LE_AUTH_REQ_MITM";
    break;
  case ESP_LE_AUTH_REQ_BOND_MITM:
    auth_str = "ESP_LE_AUTH_REQ_BOND_MITM";
    break;
  case ESP_LE_AUTH_REQ_SC_ONLY:
    auth_str = "ESP_LE_AUTH_REQ_SC_ONLY";
    break;
  case ESP_LE_AUTH_REQ_SC_BOND:
    auth_str = "ESP_LE_AUTH_REQ_SC_BOND";
    break;
  case ESP_LE_AUTH_REQ_SC_MITM:
    auth_str = "ESP_LE_AUTH_REQ_SC_MITM";
    break;
  case ESP_LE_AUTH_REQ_SC_MITM_BOND:
    auth_str = "ESP_LE_AUTH_REQ_SC_MITM_BOND";
    break;
  default:
    auth_str = "INVALID BLE AUTH REQ";
    break;
  }

  return auth_str;
}

static void show_bonded_devices(void)
{
  int dev_num = esp_ble_get_bond_device_num();

  esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
  esp_ble_get_bond_device_list(&dev_num, dev_list);
  ESP_LOGI(GATTS_TABLE_TAG, "Bonded devices number : %d\n", dev_num);

  ESP_LOGI(GATTS_TABLE_TAG, "Bonded devices list : %d\n", dev_num);
  for (int i = 0; i < dev_num; i++)
  {
    esp_log_buffer_hex(GATTS_TABLE_TAG, (void *)dev_list[i].bd_addr, sizeof(esp_bd_addr_t));
  }

  free(dev_list);
}

static void __attribute__((unused)) remove_all_bonded_devices(void)
{
  int dev_num = esp_ble_get_bond_device_num();

  esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
  esp_ble_get_bond_device_list(&dev_num, dev_list);
  for (int i = 0; i < dev_num; i++)
  {
    esp_ble_remove_bond_device(dev_list[i].bd_addr);
  }

  free(dev_list);
}

uint8_t *string_to_uint8_array(const char *str, uint16_t *length)
{
  // 计算字符串长度
  *length = strlen(str);

  // 分配内存以存储uint8_t数组，包括结尾的null字节
  uint8_t *array = (uint8_t *)malloc((*length + 1) * sizeof(uint8_t));
  if (array == NULL)
  {
    // 内存分配失败处理
    *length = 0;
    return NULL;
  }

  // 将字符逐个转换为uint8_t并存储
  for (size_t i = 0; i < *length; ++i)
  {
    array[i] = (uint8_t)str[i];
  }

  // 添加结尾的null字节
  array[*length] = '\0';

  return array;
}

static uint8_t get_sram_size()
{
  // 获取剩余内存大小
  int freeHeap = esp_get_free_heap_size();
  int psramFreeH = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  uint8_t inside = (freeHeap - psramFreeH) / 1024;
  // // 打印剩余内存大小
  printf("Free heap: %d bytes\n", freeHeap);
  // 获取PSRAM的剩余内存大小
  printf("PSRAM free size: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  ESP_LOGI("HAL", "内部内存剩余: %d Kbytes", inside);
  return inside;
}

TaskHandle_t *pTask = NULL;
volatile bool notify_flag = false;
static void get_temp(void *arg)
{
  while (1)
  {
    if (notify_flag == true)
    {
      uint8_t temp = get_sram_size();
      esp_ble_gatts_set_attr_value(handle_table[IDX_CHAR_VAL_TEMP], sizeof(temp), &temp);
      esp_ble_gatts_send_indicate(heart_rate_profile_tab[0].gatts_if, heart_rate_profile_tab[0].conn_id,
                                  handle_table[IDX_CHAR_VAL_TEMP], sizeof(temp), &temp, false);
    }
    else
    {
      vTaskDelete(NULL);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
  ESP_LOGV(GATTS_TABLE_TAG, "GAP_EVT, event %d\n", event);

  switch (event)
  {
  case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
    adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
    if (adv_config_done == 0)
    {
      esp_ble_gap_start_advertising(&adv_params);
    }
    break;
  case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
    adv_config_done &= (~ADV_CONFIG_FLAG);
    if (adv_config_done == 0)
    {
      esp_ble_gap_start_advertising(&adv_params);
    }
    break;
  case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
    // advertising start complete event to indicate advertising start successfully or failed
    if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
    {
      ESP_LOGE(GATTS_TABLE_TAG, "advertising start failed, error status = %x", param->adv_start_cmpl.status);
      break;
    }
    ESP_LOGI(GATTS_TABLE_TAG, "advertising start success");
    break;
  case ESP_GAP_BLE_PASSKEY_REQ_EVT: /* passkey request event */
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_PASSKEY_REQ_EVT");
    /* Call the following function to input the passkey which is displayed on the remote device */
    // esp_ble_passkey_reply(heart_rate_profile_tab[PROFILE_APP_IDX].remote_bda, true, 0x00);
    break;
  case ESP_GAP_BLE_OOB_REQ_EVT:
  {
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_OOB_REQ_EVT");
    uint8_t tk[16] = {1}; // If you paired with OOB, both devices need to use the same tk
    esp_ble_oob_req_reply(param->ble_security.ble_req.bd_addr, tk, sizeof(tk));
    break;
  }
  case ESP_GAP_BLE_LOCAL_IR_EVT: /* BLE local IR event */
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_LOCAL_IR_EVT");
    break;
  case ESP_GAP_BLE_LOCAL_ER_EVT: /* BLE local ER event */
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_LOCAL_ER_EVT");
    break;
  case ESP_GAP_BLE_NC_REQ_EVT:
    /* The app will receive this evt when the IO has DisplayYesNO capability and the peer device IO also has DisplayYesNo capability.
    show the passkey number to the user to confirm it with the number displayed by peer device. */
    esp_ble_confirm_reply(param->ble_security.ble_req.bd_addr, true);
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_NC_REQ_EVT, the passkey Notify number:%" PRIu32, param->ble_security.key_notif.passkey);
    break;
  case ESP_GAP_BLE_SEC_REQ_EVT:
    /* send the positive(true) security response to the peer device to accept the security request.
    If not accept the security request, should send the security response with negative(false) accept value*/
    esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
    break;
  case ESP_GAP_BLE_PASSKEY_NOTIF_EVT: /// the app will receive this evt when the IO  has Output capability and the peer device IO has Input capability.
    /// show the passkey number to the user to input it in the peer device.
    ESP_LOGI(GATTS_TABLE_TAG, "The passkey Notify number:%06" PRIu32, param->ble_security.key_notif.passkey);
    break;
  case ESP_GAP_BLE_KEY_EVT:
    // shows the ble key info share with peer device to the user.
    ESP_LOGI(GATTS_TABLE_TAG, "key type = %s", esp_key_type_to_str(param->ble_security.ble_key.key_type));
    break;
  case ESP_GAP_BLE_AUTH_CMPL_EVT:
  {
    esp_bd_addr_t bd_addr;
    memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));
    ESP_LOGI(GATTS_TABLE_TAG, "remote BD_ADDR: %08x%04x",
             (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) + bd_addr[3],
             (bd_addr[4] << 8) + bd_addr[5]);
    ESP_LOGI(GATTS_TABLE_TAG, "address type = %d", param->ble_security.auth_cmpl.addr_type);
    ESP_LOGI(GATTS_TABLE_TAG, "pair status = %s", param->ble_security.auth_cmpl.success ? "success" : "fail");
    if (!param->ble_security.auth_cmpl.success)
    {
      ESP_LOGI(GATTS_TABLE_TAG, "fail reason = 0x%x", param->ble_security.auth_cmpl.fail_reason);
    }
    else
    {
      ESP_LOGI(GATTS_TABLE_TAG, "auth mode = %s", esp_auth_req_to_str(param->ble_security.auth_cmpl.auth_mode));
    }
    show_bonded_devices();
    break;
  }
  case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
  {
    ESP_LOGD(GATTS_TABLE_TAG, "ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT status = %d", param->remove_bond_dev_cmpl.status);
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_REMOVE_BOND_DEV");
    ESP_LOGI(GATTS_TABLE_TAG, "-----ESP_GAP_BLE_REMOVE_BOND_DEV----");
    esp_log_buffer_hex(GATTS_TABLE_TAG, (void *)param->remove_bond_dev_cmpl.bd_addr, sizeof(esp_bd_addr_t));
    ESP_LOGI(GATTS_TABLE_TAG, "------------------------------------");
    break;
  }
  case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
    if (param->local_privacy_cmpl.status != ESP_BT_STATUS_SUCCESS)
    {
      ESP_LOGE(GATTS_TABLE_TAG, "config local privacy failed, error status = %x", param->local_privacy_cmpl.status);
      break;
    }

    esp_err_t ret = esp_ble_gap_config_adv_data(&adv_config);
    if (ret)
    {
      ESP_LOGE(GATTS_TABLE_TAG, "config adv data failed, error code = %x", ret);
    }
    else
    {
      adv_config_done |= ADV_CONFIG_FLAG;
    }

    ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
    if (ret)
    {
      ESP_LOGE(GATTS_TABLE_TAG, "config adv data failed, error code = %x", ret);
    }
    else
    {
      adv_config_done |= SCAN_RSP_CONFIG_FLAG;
    }

    break;
  default:
    break;
  }
}

static void gatts_profile_event_handler(esp_gatts_cb_event_t event,
                                        esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
  ESP_LOGV(GATTS_TABLE_TAG, "event = %x\n", event);
  switch (event)
  {
  case ESP_GATTS_REG_EVT:
    esp_ble_gap_set_device_name(DEVICE_NAME);
    // generate a resolvable random address
    esp_ble_gap_config_local_privacy(true);
    esp_ble_gatts_create_attr_tab(gatt_db, gatts_if,
                                  HRS_IDX_NB, SVC_INST_ID);
    break;
  case ESP_GATTS_READ_EVT:
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_READ_EVT");
    char *str = "hello word!你好世界";
    uint16_t length;
    uint8_t *data = string_to_uint8_array(str, &length);
    esp_ble_gatts_set_attr_value(handle_table[IDX_CHAR_VAL_TEMP], length, data);
    esp_ble_gatts_send_indicate(heart_rate_profile_tab[0].gatts_if, heart_rate_profile_tab[0].conn_id,
                                handle_table[IDX_CHAR_VAL_TEMP], length, data, false);
    break;
  case ESP_GATTS_WRITE_EVT:
    if (!param->write.is_prep)
    {
      // the data length of gattc write  must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX.
      ESP_LOGI(GATTS_TABLE_TAG, "GATT_WRITE_EVT, handle = %d, value len = %d, value :", param->write.handle, param->write.len);
      esp_log_buffer_hex(GATTS_TABLE_TAG, param->write.value, param->write.len);

      if (handle_table[IDX_CHAR_CFG_TEMP] == param->write.handle && param->write.len == 2)
      {
        uint16_t descr_value = param->write.value[1] << 8 | param->write.value[0];
        if (descr_value == 0x0001)
        {
          ESP_LOGI(GATTS_TABLE_TAG, "notify enable");
          xTaskCreate(get_temp, "get temp", 8192, NULL, 10, pTask);
          notify_flag = true;
        }
        else if (descr_value == 0x0002)
        {
          ESP_LOGI(GATTS_TABLE_TAG, "indicate enable");
          uint8_t indicate_data[15];
          for (int i = 0; i < sizeof(indicate_data); ++i)
          {
            indicate_data[i] = i % 0xff;
          }

          // the size of indicate_data[] need less than MTU size
          esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, handle_table[IDX_CHAR_VAL_TEMP],
                                      sizeof(indicate_data), indicate_data, true);
        }
        else if (descr_value == 0x0000)
        {
          ESP_LOGI(GATTS_TABLE_TAG, "notify/indicate disable ");
          notify_flag = false;
          // vTaskDelete(pTask);
        }
        else
        {
          ESP_LOGE(GATTS_TABLE_TAG, "unknown descr value");
          esp_log_buffer_hex(GATTS_TABLE_TAG, param->write.value, param->write.len);
        }
      }
      /* send response when param->write.need_rsp is true*/
      if (param->write.need_rsp)
      {
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
      }
    }
    else
    {
      /* handle prepare write */
      // example_prepare_write_event_env(gatts_if, &prepare_write_env, param);
    }
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_WRITE_EVT, write value:");
    esp_log_buffer_hex(GATTS_TABLE_TAG, param->write.value, param->write.len);
    break;
  case ESP_GATTS_EXEC_WRITE_EVT:
    break;
  case ESP_GATTS_MTU_EVT:
    break;
  case ESP_GATTS_CONF_EVT:
    break;
  case ESP_GATTS_UNREG_EVT:
    break;
  case ESP_GATTS_DELETE_EVT:
    break;
  case ESP_GATTS_START_EVT:
    break;
  case ESP_GATTS_STOP_EVT:
    break;
  case ESP_GATTS_CONNECT_EVT:
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONNECT_EVT");
    /* start security connect with peer device when receive the connect event sent by the master */
    esp_ble_set_encryption(param->connect.remote_bda, ESP_BLE_SEC_ENCRYPT_MITM);
    break;
  case ESP_GATTS_DISCONNECT_EVT:
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
    /* start advertising again when missing the connect */
    esp_ble_gap_start_advertising(&adv_params);
    break;
  case ESP_GATTS_OPEN_EVT:
    break;
  case ESP_GATTS_CANCEL_OPEN_EVT:
    break;
  case ESP_GATTS_CLOSE_EVT:
    break;
  case ESP_GATTS_LISTEN_EVT:
    break;
  case ESP_GATTS_CONGEST_EVT:
    break;
  case ESP_GATTS_CREAT_ATTR_TAB_EVT:
  {
    ESP_LOGI(GATTS_TABLE_TAG, "The number handle = %x", param->add_attr_tab.num_handle);
    if (param->create.status == ESP_GATT_OK)
    {
      if (param->add_attr_tab.num_handle == HRS_IDX_NB)
      {
        memcpy(handle_table, param->add_attr_tab.handles,
               sizeof(handle_table));
        esp_ble_gatts_start_service(handle_table[IDX_SVC]);
      }
      else
      {
        ESP_LOGE(GATTS_TABLE_TAG, "Create attribute table abnormally, num_handle (%d) doesn't equal to HRS_IDX_NB(%d)",
                 param->add_attr_tab.num_handle, HRS_IDX_NB);
      }
    }
    else
    {
      ESP_LOGE(GATTS_TABLE_TAG, " Create attribute table failed, error code = %x", param->create.status);
    }
    break;
  }

  default:
    break;
  }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t *param)
{
  /* If event is register event, store the gatts_if for each profile */
  if (event == ESP_GATTS_REG_EVT)
  {
    if (param->reg.status == ESP_GATT_OK)
    {
      heart_rate_profile_tab[PROFILE_APP_IDX].gatts_if = gatts_if;
    }
    else
    {
      ESP_LOGI(GATTS_TABLE_TAG, "Reg app failed, app_id %04x, status %d\n",
               param->reg.app_id,
               param->reg.status);
      return;
    }
  }

  do
  {
    int idx;
    for (idx = 0; idx < PROFILE_NUM; idx++)
    {
      if (gatts_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
          gatts_if == heart_rate_profile_tab[idx].gatts_if)
      {
        if (heart_rate_profile_tab[idx].gatts_cb)
        {
          heart_rate_profile_tab[idx].gatts_cb(event, gatts_if, param);
        }
      }
    }
  } while (0);
}

void hal_ble_init(void)
{
  esp_err_t ret;

  // Initialize NVS.
  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  ret = esp_bt_controller_init(&bt_cfg);
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "%s init controller failed: %s", __func__, esp_err_to_name(ret));
    return;
  }
  ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
    return;
  }

  ESP_LOGI(GATTS_TABLE_TAG, "%s init bluetooth", __func__);
  ret = esp_bluedroid_init();
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "%s init bluetooth failed: %s", __func__, esp_err_to_name(ret));
    return;
  }
  ret = esp_bluedroid_enable();
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
    return;
  }

  ret = esp_ble_gatts_register_callback(gatts_event_handler);
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "gatts register error, error code = %x", ret);
    return;
  }
  ret = esp_ble_gap_register_callback(gap_event_handler);
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "gap register error, error code = %x", ret);
    return;
  }
  ret = esp_ble_gatts_app_register(ESP_APP_ID);
  if (ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "gatts app register error, error code = %x", ret);
    return;
  }
  esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
  if (local_mtu_ret)
  {
    ESP_LOGE(GATTS_TABLE_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
  }

  /* set the security iocap & auth_req & key size & init key response key parameters to the stack*/
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND; // bonding with peer device after authentication
  esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;                   // set the IO capability to No output No input
  uint8_t key_size = 16;                                      // the key size should be 7~16 bytes
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  // set static passkey
  uint32_t passkey = 123456;
  uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
  uint8_t oob_support = ESP_BLE_OOB_DISABLE;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_OOB_SUPPORT, &oob_support, sizeof(uint8_t));
  /* If your BLE device acts as a Slave, the init_key means you hope which types of key of the master should distribute to you,
  and the response key means which key you can distribute to the master;
  If your BLE device acts as a master, the response key means you hope which types of key of the slave should distribute to you,
  and the init key means which key you can distribute to the slave. */
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

  /* Just show how to clear all the bonded devices
   * Delay 30s, clear all the bonded devices
   *
   * vTaskDelay(30000 / portTICK_PERIOD_MS);
   * remove_all_bonded_devices();
   */
}
