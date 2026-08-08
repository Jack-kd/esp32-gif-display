# ESP32 GIF Display

> **中文：** 基于 ESP32 的 GIF 播放器，通过 WiFi 热点提供网页界面，支持上传 GIF 并在 ILI9341 SPI 屏幕上播放。支持浏览器端 MP4 转 GIF、WiFi STA/AP 双模、GitHub Actions 自动编译固件。

> **EN:** A WiFi-enabled ESP32 GIF display that creates a web interface for uploading and playing GIF animations on an ILI9341 SPI LCD screen. Features browser-side MP4-to-GIF conversion, WiFi STA/AP dual mode, and CI/CD auto-build.

---

## Hardware Requirements

> **中文：** 硬件需求

| Component | Spec |
|---|---|
| **MCU** | ESP32-DOWD-V3, Flash 16MB, PSRAM 8MB |
| **LCD** | 2.4" TFT, ILI9341 driver, 4-wire SPI, 240×320 |
| **Touch** | XPT2046 (optional) |

> **中文：** 必须使用带 PSRAM 的 ESP32 — GIF 解码器需要 PSRAM 分配 3 × 宽 × 高 字节内存。

> **EN:** PSRAM is required — the GIF decoder allocates 3 × W × H bytes in PSRAM.

---

## Features

> **中文：** 功能特性

- **Web-based Upload** — Upload GIFs via a web interface served by ESP32
  > **中文：** 网页上传 — 通过 ESP32 提供的网页界面上传 GIF
- **MP4 → GIF Conversion** — Browser-side video-to-GIF with quality/size controls
  > **中文：** MP4 转 GIF — 浏览器端视频转 GIF，支持帧数和画质调节
- **Orientation Control** — Portrait (240×320) / Landscape (320×240)
  > **中文：** 方向切换 — 竖屏 / 横屏切换
- **WiFi STA + AP** — Default AP mode (hotspot) + optional STA mode (connect to home router)
  > **中文：** 双模 WiFi — 默认 AP 热点模式，可切换 STA 连接家庭路由器
- **Persistent Settings** — WiFi credentials and orientation saved in LittleFS
  > **中文：** 配置持久化 — WiFi 密码和方向设置保存在 LittleFS 中
- **CI/CD Auto Build** — GitHub Actions compiles firmware + LittleFS image on every push
  > **中文：** 自动编译 — GitHub Actions 每次推送自动编译固件和 LittleFS 镜像

---

## Pin Configuration

> **中文：** 引脚配置，在 `config.h` 中定义。

> **EN:** Defined in `config.h`.

| Pin | GPIO | Function |
|---|---|---|
| `LCD_SCLK` | 18 | SPI Clock |
| `LCD_MOSI` | 23 | SPI MOSI |
| `LCD_MISO` | 19 | SPI MISO |
| `LCD_DC` | 2 | Data/Command |
| `LCD_RST` | 4 | Reset |
| `LCD_CS` | 5 | Chip Select |
| `LCD_BL` | 15 | Backlight |

---

## Required Libraries

> **中文：** 依赖库

| Library | Purpose |
|---|---|
| `bb_spi_lcd` | LCD driver |
| `AnimatedGIF` | GIF decoding |

### GIFPlayer.h

> **中文：** `GIFPlayer.h` 不在 AnimatedGIF 标准库中，需要手动下载放到 `AnimatedGIF.h` 同一目录。

> **EN:** The `GIFPlayer.h` header is **not included** in the standard AnimatedGIF library. Download it manually and place it alongside `AnimatedGIF.h`.

Download from / 下载地址：
```
https://github.com/bitbank2/AnimatedGIF/blob/master/src/GIFPlayer.h
```

Target path / 目标路径：
```
Arduino/libraries/AnimatedGIF/src/GIFPlayer.h
```

---

## Setup

> **中文：** 使用步骤

### 1. Flash Firmware

> **中文：** 烧录固件。从 [GitHub Actions](https://github.com/Jack-kd/esp32-gif-display/actions) 下载最新 artifact，或本地编译。

> **EN:** Download the latest artifact from [GitHub Actions](https://github.com/Jack-kd/esp32-gif-display/actions) or build locally.

**Flash addresses / 烧录地址：**

| File | Address |
|---|---|
| `esp32-gif-display.ino.bootloader.bin` | **0x1000** |
| `esp32-gif-display.ino.partitions.bin` | **0x8000** |
| `esp32-gif-display.ino.bin` | **0x10000** |
| `esp32-gif-display.littlefs.bin` | **0x290000** |

> **中文：** 可使用 ESPflasher、esptool 或任意 ESP32 烧录工具。

> **EN:** You can use ESPflasher, esptool, or any ESP32 flashing tool.

### 2. Connect

> **中文：** ESP32 开机后创建 WiFi 热点，连接后访问网页。

> **EN:** The ESP32 creates a WiFi hotspot. Connect and open the web interface.

| Item | Value |
|---|---|
| **SSID** | `ESP32-GIF` |
| **Password** | `upload123` |
| **Web URL** | `http://192.168.4.1` |

### 3. (Optional) Connect to Home WiFi

> **中文：** 在网页界面的 WiFi 设置标签页中输入路由器 SSID 和密码即可连接家庭 WiFi。

> **EN:** In the web interface, go to the WiFi settings tab and enter your router's SSID and password.

---

## Usage

> **中文：** 使用说明

1. Connect to `ESP32-GIF` WiFi
   > **中文：** 连接 `ESP32-GIF` 热点
2. Open `http://192.168.4.1`
   > **中文：** 浏览器打开 `http://192.168.4.1`
3. **Upload GIF** tab: Select a `.gif` file and upload — plays immediately
   > **中文：** **上传 GIF** 标签页：选择 `.gif` 文件上传，立即播放
4. **Video → GIF** tab: Select an `.mp4` file, set frame count and quality, convert and upload
   > **中文：** **视频转 GIF** 标签页：选择 `.mp4` 文件，设置帧数和画质，浏览器自动转换后上传
5. **WiFi** tab: Configure STA mode to connect to your home router
   > **中文：** **WiFi** 标签页：配置 STA 模式连接家庭路由器

---

## File Structure

```
esp32-gif-display/
├── esp32-gif-display.ino      # Main sketch / 主程序
├── config.h                   # Pin & config / 引脚和配置
├── data/
│   └── index.html             # Web UI / 网页界面
├── src/
│   ├── display/               # LCD driver / 屏幕驱动
│   │   ├── display.h
│   │   └── display.cpp
│   ├── gif_player/            # GIF playback / GIF 播放
│   │   ├── gif_player.h
│   │   └── gif_player.cpp
│   ├── webui/                 # Web server / 网页服务
│   │   ├── webui.h
│   │   └── webui.cpp
│   └── wifi_manager/          # WiFi STA/AP / WiFi 管理
│       ├── wifi_manager.h
│       └── wifi_manager.cpp
└── .github/workflows/
    └── build.yml              # CI auto-build / 自动编译
```

---

## Build Locally

> **中文：** 本地编译

```bash
# Install Arduino CLI / 安装 Arduino CLI
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Install ESP32 platform / 安装 ESP32 平台
arduino-cli config init --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core update-index
arduino-cli core install esp32:esp32

# Install libraries / 安装依赖库
arduino-cli lib install "bb_spi_lcd"
arduino-cli lib install "AnimatedGIF"

# Compile / 编译
arduino-cli compile \
  --fqbn "esp32:esp32:esp32:PSRAM=enabled,FlashSize=16M" \
  --output-dir build \
  .
```

---

## Troubleshooting

> **中文：** 常见问题

| Problem | Solution |
|---|---|
| GIF not playing | Ensure PSRAM is enabled |
| > **中文：** GIF 不播放 | > **中文：** 确认 PSRAM 已启用 |
| Web page 404 | Flash `littlefs.bin` to 0x290000 |
| > **中文：** 网页 404 | > **中文：** 烧录 littlefs.bin 到 0x290000 |
| Upload fails | GIF must be < 5MB |
| > **中文：** 上传失败 | > **中文：** GIF 文件需小于 5MB |
| Can't connect to WiFi | Check AP_SSID/AP_PASS in config.h |
| > **中文：** 连不上热点 | > **中文：** 检查 config.h 中热点配置 |

---

## License

> **中文：** MIT 许可证，详见 [LICENSE](LICENSE) 文件。

> **EN:** MIT License. See [LICENSE](LICENSE) for details.