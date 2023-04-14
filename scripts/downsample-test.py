#!/usr/bin/env python
"""Get the result of ``downsample-test.c.in``.

a = -0.75
"""
import cv2
import numpy as np
import torch
from torch.nn import functional as F

img = torch.arange(16.0).reshape(4, 4).unsqueeze(0).unsqueeze(0)
print(F.interpolate(img, scale_factor=0.5, mode="bilinear").tolist())
# [[[[2.5, 4.5], [10.5, 12.5]]]]
print(F.interpolate(img, scale_factor=0.5, mode="bicubic").tolist())
# [[[[2.03125, 4.21875], [10.78125, 12.96875]]]]

img2 = np.arange(16, dtype=np.uint8).reshape(4, 4)
print(cv2.resize(img2, (2, 2), interpolation=cv2.INTER_LINEAR))
# [[ 3  5]
#  [11 13]]
print(cv2.resize(img2, (2, 2), interpolation=cv2.INTER_CUBIC))
# [[ 2  4]
#  [11 13]]
