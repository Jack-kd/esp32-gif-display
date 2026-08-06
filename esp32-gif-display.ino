#include "config.h"
#include "src/display/display.h"
#include "src/gif_player/gif_player.h"
#include "src/webui/webui.h"
#include "src/wifi_manager/wifi_manager.h"
#include <LittleFS.h>

void setup() {
  Serial.begin(115200);
  delay(100);
  LittleFS.begin(true);

  // 初始化 WiFi（AP + 可选 STA）
  wifi_manager_begin();

  display_init();

  // 加载并应用保存的方向
  int saved_orientation = load_orientation();
  set_gif_orientation(saved_orientation);

  webui_begin();
  gifplayer_begin();
}

void loop() {
  webui_handle();

  static bool decoder_stopped = false;

  // 上传开始时停止解码器
  if (g_request_stop && !decoder_stopped) {
    gifplayer_stop();
    decoder_stopped = true;
    g_request_stop = false;
  }

  // 处理文件交换
  if (decoder_stopped && g_swap_ready) {
    if (!LittleFS.exists(GIF_TMP_PATH)) {
      Serial.println("[swap] 错误: 源文件不存在");
      g_swap_ready = false;
      decoder_stopped = false;
      return;
    }

    File srcFile = LittleFS.open(GIF_TMP_PATH, "r");
    if (srcFile) {
      srcFile.close();
    } else {
      Serial.println("[swap] 错误: 无法打开源文件");
      g_swap_ready = false;
      decoder_stopped = false;
      return;
    }

    if (LittleFS.exists(GIF_ACTIVE_PATH)) {
      if (!LittleFS.remove(GIF_ACTIVE_PATH)) {
        Serial.println("[swap] 警告: 无法删除旧文件");
      }
    }
    delay(50);

    if (LittleFS.rename(GIF_TMP_PATH, GIF_ACTIVE_PATH)) {
      // 成功
    } else {
      // 尝试备选方案: 复制后删除
      File src = LittleFS.open(GIF_TMP_PATH, "r");
      File dst = LittleFS.open(GIF_ACTIVE_PATH, "w");

      if (src && dst) {
        uint8_t buffer[512];
        size_t bytesRead;
        size_t totalCopied = 0;

        while ((bytesRead = src.read(buffer, sizeof(buffer))) > 0) {
          dst.write(buffer, bytesRead);
          totalCopied += bytesRead;
        }

        src.close();
        dst.close();

        if (totalCopied > 0) {
          LittleFS.remove(GIF_TMP_PATH);
        } else {
          Serial.println("[swap] 错误: 复制失败");
        }
      } else {
        Serial.println("[swap] 错误: 无法打开文件进行复制");
        if (src) src.close();
        if (dst) dst.close();
      }
    }
    g_swap_ready = false;

    // 重新打开 GIF 并继续播放
    gifplayer_reload();
    decoder_stopped = false;
  }

  // 驱动解码器（非上传状态）
  if (!g_uploading && !decoder_stopped) {
    gifplayer_loop();
  }

  delay(1);
}