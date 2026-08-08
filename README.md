# ESP32 GIF Display / ESP32 GIF 播放器

A WiFi-enabled ESP32 GIF display that creates a web interface for uploading and playing GIF animations on an ILI9341 SPI LCD screen. Features browser-side MP4-to-GIF conversion, WiFi STA/AP dual mode, and CI/CD auto-build.

基于 ESP32 的 GIF 播放器，通过 WiFi 热点提供网页界面，支持上传 GIF 并在 ILI9341 SPI 屏幕上播放。支持浏览器端 MP4 转 GIF、WiFi STA/AP 双模、GitHub Actions 自动编译固件。

---

## Hardware Requirements / 硬件需求

| Component / 组件 | Spec / 规格 |
|---|---|
| **MCU** | ESP32-DOWD-V3, Flash 16MB, PSRAM 8MB |
| **LCD** | 2.4" TFT, ILI9341 driver, 4-wire SPI, 240×320 |
| **Touch** | XPT2046 (optional / 可选) |

> **PSRAM is required** — the GIF decoder allocates 3 × W × H bytes in PSRAM.
> **必须使用带 PSRAM 的 ESP32** — GIF 解码器需要 PSRAM 分配 3 × 宽 × 高 字节内存。

---

## Features / 功能

- **Web-based Upload** / **网页上传** — Upload GIFs via a web interface served by ESP32
- **MP4 → GIF Conversion** / **MP4 转 GIF** — Browser-side video-to-GIF with quality/size controls
- **Orientation Control** / **方向切换** — Portrait (240×320) / Landscape (320×240)
- **WiFi STA + AP** / **双模 WiFi** — Default AP mode (hotspot) + optional STA mode (connect to home router)
- **Persistent Settings** / **配置持久化** — WiFi credentials and orientation saved in LittleFS
- **CI/CD Auto Build** / **自动编译** — GitHub Actions compiles firmware + LittleFS image on every push

---

## Pin Configuration / 引脚配置

Defined in `config.h` / 在 `config.h` 中定义：

| Pin / 引脚 | GPIO | Function / 功能 |
|---|---|---|
| `LCD_SCLK` | 18 | SPI Clock / 时钟 |
| `LCD_MOSI` | 23 | SPI MOSI / 主机输出 |
| `LCD_MISO` | 19 | SPI MISO / 主机输入 |
| `LCD_DC` | 2 | Data/Command / 数据命令 |
| `LCD_RST` | 4 | Reset / 复位 |
| `LCD_CS` | 5 | Chip Select / 片选 |
| `LCD_BL` | 15 | Backlight / 背光 |

---

## Required Libraries / 依赖库

| Library / 库 | Purpose / 用途 |
|---|---|
| `bb_spi_lcd` | LCD driver / 屏幕驱动 |
| `AnimatedGIF` | GIF decoding / GIF 解码 |

### GIFPlayer.h (Manual Install / 手动安装)

The `GIFPlayer.h` header is **not included** in the standard AnimatedGIF library. Download it from:
`GIFPlayer.h` 不在 AnimatedGIF 标准库中，需要手动下载：

```
https://github.com/bitbank2/AnimatedGIF/blob/master/src/GIFPlayer.h
```

Place it alongside `AnimatedGIF.h` at:
放到 `AnimatedGIF.h` 同一目录：

```
Arduino/libraries/AnimatedGIF/src/GIFPlayer.h
```

---

## Setup / 使用步骤

### 1. Flash Firmware / 烧录固件

Download the latest artifact from [GitHub Actions](https://github.com/Jack-kd/esp32-gif-display/actions) or build locally.
从 GitHub Actions 下载最新固件，或本地编译。

**Flash addresses / 烧录地址：**

| File / 文件 | Address / 地址 |
|---|---|
| `esp32-gif-display.ino.bootloader.bin` | **0x1000** |
| `esp32-gif-display.ino.partitions.bin` | **0x8000** |
| `esp32-gif-display.ino.bin` | **0x10000** |
| `esp32-gif-display.littlefs.bin` | **0x290000** |

> You can use **ESPflasher**, **esptool**, or any ESP32 flashing tool.
> 可使用 ESPflasher、esptool 或任意 ESP32 烧录工具。

### 2. Connect / 连接

The ESP32 creates a WiFi hotspot:
ESP32 开机后创建 WiFi 热点：

| Item / 项 | Value / 值 |
|---|---|
| **SSID** | `ESP32-GIF` |
| **Password / 密码** | `upload123` |
| **Web URL / 网页地址** | `http://192.168.4.1` |

### 3. (Optional) Connect to Home WiFi / 连接家庭 WiFi

In the web interface, go to the WiFi settings tab and enter your router's SSID and password.
在网页界面的 WiFi 设置标签页中输入路由器 SSID 和密码即可。

---

## Usage / 使用说明

1. Connect to `ESP32-GIF` WiFi / 连接 `ESP32-GIF` 热点
2. Open `http://192.168.4.1` / 打开浏览器访问
3. **Upload GIF** tab: Select a `.gif` file and upload — plays immediately
   **上传 GIF**：选择 `.gif` 文件上传，立即播放
4. **Video → GIF** tab: Select an `.mp4` file, set frame count and quality, convert and upload
   **视频转 GIF**：选择 `.mp4` 文件，设置帧数和画质，浏览器自动转换后上传
5. **WiFi** tab: Configure STA mode to connect to your home router
   **WiFi 设置**：配置 STA 模式连接家庭路由器

---

## File Structure / 文件结构

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

## Build Locally / 本地编译

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

## Troubleshooting / 常见问题

| Problem / 问题 | Solution / 解决 |
|---|---|
| GIF not playing / GIF 不播放 | Ensure PSRAM is enabled / 确认 PSRAM 已启用 |
| Web page 404 / 网页 404 | Flash `littlefs.bin` to 0x290000 / 烧录 littlefs.bin 到 0x290000 |
| Upload fails / 上传失败 | GIF must be < 5MB / GIF 文件需小于 5MB |
| Can't connect to WiFi / 连不上热点 | Check AP_SSID/AP_PASS in config.h / 检查 config.h 中热点配置 |

---

## License / 许可证

MIT License. See [LICENSE](LICENSE) for details.