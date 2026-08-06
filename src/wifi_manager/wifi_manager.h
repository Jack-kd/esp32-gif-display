#pragma once
#include <stdint.h>

// 初始化 WiFi：默认启动 AP 模式，若有保存的 STA 配置则尝试连接
void wifi_manager_begin();

// 获取当前 AP 模式的 IP 地址（字符串）
const char* wifi_ap_ip();

// STA 连接状态
bool wifi_sta_connected();
const char* wifi_sta_ip();
const char* wifi_sta_ssid();

// 保存并连接 WiFi
// 返回 true 表示已保存配置，实际连接是异步的
bool wifi_sta_connect(const char* ssid, const char* pass);

// 断开 STA 并清除配置
void wifi_sta_disconnect();

// 获取已保存的 WiFi 配置
bool wifi_get_saved_config(char* ssid_out, int ssid_len);
