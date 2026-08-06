#include "../../config.h"
#include "wifi_manager.h"
#include <WiFi.h>
#include <LittleFS.h>
#include <Arduino.h>

static char sta_ssid[33] = "";
static char sta_pass[65] = "";
static bool sta_configured = false;
static bool sta_connected = false;
static unsigned long sta_connect_time = 0;
static bool sta_connecting = false;

// ---------- 从 LittleFS 加载已保存的 WiFi 配置 ----------
static void load_wifi_config() {
  if (!LittleFS.exists(WIFI_CONFIG_FILE)) return;

  File f = LittleFS.open(WIFI_CONFIG_FILE, "r");
  if (!f) return;

  String line1 = f.readStringUntil('\n');
  String line2 = f.readStringUntil('\n');
  f.close();

  line1.trim();
  line2.trim();

  if (line1.length() > 0 && line1.length() < 33) {
    strncpy(sta_ssid, line1.c_str(), 32);
    sta_ssid[32] = 0;
  }
  if (line2.length() > 0 && line2.length() < 65) {
    strncpy(sta_pass, line2.c_str(), 64);
    sta_pass[64] = 0;
  }

  if (strlen(sta_ssid) > 0) {
    sta_configured = true;
  }
}

// ---------- 初始化 ----------
void wifi_manager_begin() {
  load_wifi_config();

  // 先启动 AP 模式（始终可用）
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("[WiFi] AP 已启动: %s\n", WiFi.softAPIP().toString().c_str());

  // 如果有 STA 配置，尝试连接
  if (sta_configured) {
    Serial.printf("[WiFi] 尝试连接 STA: %s ...\n", sta_ssid);
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(sta_ssid, sta_pass);
    sta_connecting = true;
    sta_connect_time = millis();
  }
}

// ---------- 获取 AP IP ----------
const char* wifi_ap_ip() {
  return WiFi.softAPIP().toString().c_str();
}

// ---------- STA 状态 ----------
bool wifi_sta_connected() {
  // 检查连接状态变化
  if (sta_connecting) {
    wl_status_t s = WiFi.status();
    if (s == WL_CONNECTED) {
      sta_connecting = false;
      sta_connected = true;
      Serial.printf("[WiFi] STA 已连接! IP: %s\n", WiFi.localIP().toString().c_str());
    } else if (s == WL_CONNECTION_LOST || s == WL_CONNECT_FAILED ||
               s == WL_NO_SSID_AVAIL || s == WL_DISCONNECTED) {
      if (millis() - sta_connect_time > 30000) {
        // 30 秒超时，放弃
        sta_connecting = false;
        sta_connected = false;
        Serial.println("[WiFi] STA 连接超时");
      }
    }
  }
  return sta_connected;
}

const char* wifi_sta_ip() {
  if (sta_connected) {
    return WiFi.localIP().toString().c_str();
  }
  return "";
}

const char* wifi_sta_ssid() {
  return sta_ssid;
}

// ---------- 保存并连接 ----------
bool wifi_sta_connect(const char* ssid, const char* pass) {
  if (!ssid || strlen(ssid) == 0) return false;

  strncpy(sta_ssid, ssid, 32);
  sta_ssid[32] = 0;
  strncpy(sta_pass, pass ? pass : "", 64);
  sta_pass[64] = 0;
  sta_configured = true;

  // 保存到 LittleFS
  File f = LittleFS.open(WIFI_CONFIG_FILE, "w");
  if (f) {
    f.println(sta_ssid);
    f.println(sta_pass);
    f.close();
  }

  // 断开旧连接，重新连接
  WiFi.disconnect(true);
  delay(200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(sta_ssid, sta_pass);
  sta_connecting = true;
  sta_connected = false;
  sta_connect_time = millis();
  Serial.printf("[WiFi] 正在连接: %s ...\n", sta_ssid);

  return true;
}

// ---------- 断开并清除配置 ----------
void wifi_sta_disconnect() {
  WiFi.disconnect(true);
  sta_configured = false;
  sta_connected = false;
  sta_connecting = false;
  sta_ssid[0] = 0;
  sta_pass[0] = 0;

  if (LittleFS.exists(WIFI_CONFIG_FILE)) {
    LittleFS.remove(WIFI_CONFIG_FILE);
  }

  WiFi.mode(WIFI_AP);
  Serial.println("[WiFi] STA 已断开，配置已清除");
}

// ---------- 获取已保存的 SSID ----------
bool wifi_get_saved_config(char* ssid_out, int ssid_len) {
  if (!sta_configured || strlen(sta_ssid) == 0) return false;
  strncpy(ssid_out, sta_ssid, ssid_len);
  ssid_out[ssid_len - 1] = 0;
  return true;
}