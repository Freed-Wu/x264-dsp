---
documentclass: ctexbeamer
title: MPEG4与H.264主客观性能对比
institute: USTC
aspectratio: 169
theme: Berkeley
colortheme: spruce
navigation: horizontal
---

## 条件

为了公平起见，MPEG4 (codec 使用 libxvid) 和 H.264 (codec 使用 libx264) 均设置为：

1. 相邻 2 个 P/I 帧间插入 0 个 B 帧
2. 比特率为 300k/600k, 误差不超过 2k

## 比较

### 客观结果

从 MSE 和 PSNR 的结果来看，H.264比MPEG4的性能更好。

不同的序列的PSNR增益1dB到5dB不等，与视频内容相关。

## 比较 (300k)

| 文件名      | MSE   | MSE~Y~ | MSE~U~ | MSE~V~ | PSNR  | PSNR~Y~ | PSNR~U~ | PSNR~V~ |
|-------------|-------|--------|--------|--------|-------|---------|---------|---------|
| crew^m^     | 34.01 | 42.4   | 14.19  | 20.28  | 33.0  | 32.04   | 36.85   | 35.29   |
| crew^h^     | 22.63 | 29.15  | 7.93   | 11.24  | 34.72 | 33.62   | 39.29   | 37.77   |
| harbour^m^  | 99.6  | 146.04 | 8.51   | 4.92   | 28.22 | 26.56   | 38.84   | 41.22   |
| harbour^h^  | 60.06 | 87.35  | 6.58   | 4.36   | 30.38 | 28.75   | 39.96   | 41.76   |
| paris^m^    | 63.72 | 83.93  | 24.96  | 21.64  | 30.18 | 28.99   | 34.22   | 34.83   |
| paris^h^    | 17.62 | 22.81  | 7.23   | 7.24   | 35.92 | 34.82   | 39.65   | 39.64   |

## 比较 (600k)

| 文件名      | MSE   | MSE~Y~ | MSE~U~ | MSE~V~ | PSNR  | PSNR~Y~ | PSNR~U~ | PSNR~V~ |
|-------------|-------|--------|--------|--------|-------|---------|---------|---------|
| crew^m^     | 17.67 | 21.6   | 8.48   | 11.18  | 35.81 | 34.94   | 39.02   | 37.83   |
| crew^h^     | 11.4  | 14.06  | 5.4    | 6.76   | 37.72 | 36.8    | 40.97   | 40.02   |
| harbour^m^  | 54.32 | 78.54  | 7.42   | 4.34   | 30.83 | 29.23   | 39.44   | 41.77   |
| harbour^h^  | 31.91 | 45.68  | 5.26   | 3.49   | 33.14 | 31.59   | 40.94   | 42.73   |
| paris^m^    | 26.41 | 33.52  | 12.7   | 11.68  | 33.94 | 32.9    | 37.11   | 37.47   |
| paris^h^    | 7.14  | 8.89   | 3.67   | 3.59   | 39.79 | 38.85   | 42.59   | 42.69   |

## 比较

### 主观结果

除了客观的PSNR之外，下图是一些测试视频以 MPEG4 和 H.264 编码后的最后一帧的图像。

主观上能看到一些明显的块效应的减少。

## 比较 (300k)

![mpeg4/300k/crew](https://github.com/Freed-Wu/x264-dsp/assets/32936898/ff04bd79-b08c-45be-b604-a80648de8732 "mpeg4"){width=6cm}
![h264/300k/crew](https://github.com/Freed-Wu/x264-dsp/assets/32936898/d5fb29f3-74fe-4b7b-822e-3febd19d7893 "h264"){width=6cm}

## 比较 (300k)

![mpeg4/300k/deadline](https://github.com/Freed-Wu/x264-dsp/assets/32936898/abfa06b8-ca7f-4fa8-a31a-89a75ad3e381 "mpeg4"){width=6cm}
![h264/300k/deadline](https://github.com/Freed-Wu/x264-dsp/assets/32936898/d6d5e482-201b-4702-84c2-bf465121a444 "h264"){width=6cm}

## 比较 (300k)

![mpeg4/300k/harbour](https://github.com/Freed-Wu/x264-dsp/assets/32936898/4c32bb2c-a3b0-4d6e-9eb1-64500599724e "mpeg4"){width=6cm}
![h264/300k/harbour](https://github.com/Freed-Wu/x264-dsp/assets/32936898/0ad8b142-55ae-4cd0-8851-37304a98e7ab "h264"){width=6cm}

## 比较 (300k)

![mpeg4/300k/pamphlet](https://github.com/Freed-Wu/x264-dsp/assets/32936898/afd8f1bc-d604-4d9d-9dd2-f81a0152917d "mpeg4"){width=6cm}
![h264/300k/pamphlet](https://github.com/Freed-Wu/x264-dsp/assets/32936898/e40746b9-3778-44e0-82cf-7feacd89bf2a "h264"){width=6cm}

## 比较 (300k)

![mpeg4/300k/paris](https://github.com/Freed-Wu/x264-dsp/assets/32936898/3e8af53d-154d-436a-bac7-659f34452c4d "mpeg4"){width=6cm}
![h264/300k/paris](https://github.com/Freed-Wu/x264-dsp/assets/32936898/5bca464a-83ee-4af9-beb4-e54790bfb1fd "h264"){width=6cm}

## 比较 (300k)

![mpeg4/300k/silent](https://github.com/Freed-Wu/x264-dsp/assets/32936898/90744126-9eea-4053-9c02-fe907b35540e "mpeg4"){width=6cm}
![h264/300k/silent](https://github.com/Freed-Wu/x264-dsp/assets/32936898/5d486811-c9c5-44ea-b5b9-5cd286647b8c "h264"){width=6cm}

## 比较 (300k)

![mpeg4/300k/students](https://github.com/Freed-Wu/x264-dsp/assets/32936898/1b033ca9-fab3-4c82-8c15-eab3383a04ca "mpeg4"){width=6cm}
![h264/300k/students](https://github.com/Freed-Wu/x264-dsp/assets/32936898/15a432d5-c950-4634-80b7-27a55773da70 "h264"){width=6cm}

## 比较 (600k)

![mpeg4/600k/crew](https://github.com/Freed-Wu/x264-dsp/assets/32936898/1bda8b0d-651d-45c1-8f98-9b6f71ea68ac "mpeg4"){width=6cm}
![h264/600k/crew](https://github.com/Freed-Wu/x264-dsp/assets/32936898/58c03d77-b8b3-4959-ba14-89ffe3125ec1 "h264"){width=6cm}

## 比较 (600k)

![mpeg4/600k/deadline](https://github.com/Freed-Wu/x264-dsp/assets/32936898/0451511c-1aa6-40ae-bee5-2ccbd1be5557 "mpeg4"){width=6cm}
![h264/600k/deadline](https://github.com/Freed-Wu/x264-dsp/assets/32936898/2f8a69bd-12fa-4086-bb1c-9124a7773b6e "h264"){width=6cm}

## 比较 (600k)

![mpeg4/600k/harbour](https://github.com/Freed-Wu/x264-dsp/assets/32936898/316e45db-3f1b-4ac2-aec7-19a83bb5d821 "mpeg4"){width=6cm}
![h264/600k/harbour](https://github.com/Freed-Wu/x264-dsp/assets/32936898/a6175fe7-6cba-491f-ac47-1abf24653ac2 "h264"){width=6cm}

## 比较 (600k)

![mpeg4/600k/pamphlet](https://github.com/Freed-Wu/x264-dsp/assets/32936898/6d181cbb-e111-460a-934d-562e8513ec0d "mpeg4"){width=6cm}
![h264/600k/pamphlet](https://github.com/Freed-Wu/x264-dsp/assets/32936898/a82f7e34-3c04-43ba-8dd9-f836b31dc39f "h264"){width=6cm}

## 比较 (600k)

![mpeg4/600k/paris](https://github.com/Freed-Wu/x264-dsp/assets/32936898/f7b08a54-3cc2-43cb-92ce-1bfc6a1be19e "mpeg4"){width=6cm}
![h264/600k/paris](https://github.com/Freed-Wu/x264-dsp/assets/32936898/19633fd8-4751-487b-8a58-4372faa4db3b "h264"){width=6cm}

## 比较

![mpeg4/600k/silent](https://github.com/Freed-Wu/x264-dsp/assets/32936898/6e9a20b5-2c3b-4d78-a06e-923ef0c20fa3 "mpeg4"){width=6cm}
![h264/600k/silent](https://github.com/Freed-Wu/x264-dsp/assets/32936898/9d45367e-34ed-4b66-831a-136fa5eff1c8 "h264"){width=6cm}

## 比较

![mpeg4/600k/students](https://github.com/Freed-Wu/x264-dsp/assets/32936898/45932be8-a3bd-4939-b4b4-54b4aaf16197 "mpeg4"){width=6cm}
![h264/600k/students](https://github.com/Freed-Wu/x264-dsp/assets/32936898/601595df-7dbf-4b2b-afa4-c53ee2fff619 "h264"){width=6cm}

## 比较

### 结论

目前的软件方案表明，无论是主观质量还是客观质量，H.264均表现出比MPEG4更好的编码性能。
