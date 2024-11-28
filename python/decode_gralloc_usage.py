import sys

def DECODE_GRALLOC_USAGE(usage):
    usage_temp = int(usage)
    usage_test = 0
    for k, v in {
        "GRALLOC_USAGE_SW_READ_MASK":            0x0F,
        "GRALLOC_USAGE_SW_WRITE_MASK":           0xF0,
        "GRALLOC_USAGE_HW_TEXTURE":              1 << 8,
        "GRALLOC_USAGE_HW_RENDER":               1 << 9,
        "GRALLOC_USAGE_HW_2D":                   1 << 10,
        "GRALLOC_USAGE_HW_COMPOSER":             1 << 11,
        "GRALLOC_USAGE_HW_FB":                   1 << 12,
        "GRALLOC_USAGE_EXTERNAL_DISP":           1 << 13,
        "GRALLOC_USAGE_PROTECTED":               1 << 14,
        "GRALLOC_USAGE_CURSOR":                  1 << 15,
        "GRALLOC_USAGE_HW_VIDEO_ENCODER":        1 << 16,
        "GRALLOC_USAGE_HW_CAMERA_WRITE":         1 << 17,
        "GRALLOC_USAGE_HW_CAMERA_READ":          1 << 18,
        "GRALLOC_USAGE_RENDERSCRIPT":            1 << 20,
        "GRALLOC_USAGE_HW_VIDEO_DECODER":        1 << 22,
        "GRALLOC_USAGE_SENSOR_DIRECT_DATA":      1 << 23,
        "GRALLOC_USAGE_GPU_DATA_BUFFER":         1 << 24,
        "GRALLOC_USAGE_FRONT_BUFFER":            1 << 32,
        "GRALLOC_USAGE_GOOGLE_IP_BIG":           1 << 51,
        "GRALLOC_USAGE_GOOGLE_IP_MFC":           1 << 50,
        "GS101_GRALLOC_USAGE_TPU_INPUT":         1 << 62,
        "GS101_GRALLOC_USAGE_TPU_OUTPUT":        1 << 31,
        "GS101_GRALLOC_USAGE_FACEAUTH_RAW_EVAL": 1 << 63,
        "GS101_GRALLOC_USAGE_CAMERA_STATS":      1 << 30,
        "GRALLOC_USAGE_ROIINFO":                 1 << 52,
        "MALI_GRALLOC_USAGE_AFBC_PADDING":       1 << 53,
        "MALI_GRALLOC_USAGE_FORCE_BACKBUFFER":   1 << 54,
        "MALI_GRALLOC_USAGE_NO_AFBC":            1 << 29,
        """
        "GRALLOC_USAGE_SW_READ_NEVER":           0x00000000,
        "GRALLOC_USAGE_SW_READ_RARELY":          0x00000002,
        "GRALLOC_USAGE_SW_READ_OFTEN":           0x00000003,
        "GRALLOC_USAGE_SW_READ_MASK":            0x0000000F,
        "GRALLOC_USAGE_SW_WRITE_NEVER":          0x00000000,
        "GRALLOC_USAGE_SW_WRITE_RARELY":         0x00000020,
        "GRALLOC_USAGE_SW_WRITE_OFTEN":          0x00000030,
        "GRALLOC_USAGE_SW_WRITE_MASK":           0x000000F0,
        "GRALLOC_USAGE_HW_TEXTURE":              0x00000100,
        "GRALLOC_USAGE_HW_RENDER":               0x00000200,
        "GRALLOC_USAGE_HW_2D":                   0x00000400,
        "GRALLOC_USAGE_HW_COMPOSER":             0x00000800,
        "GRALLOC_USAGE_HW_FB":                   0x00001000,
        "GRALLOC_USAGE_HW_VIDEO_ENCODER":        0x00010000,
        "GRALLOC_USAGE_HW_CAMERA_WRITE":         0x00020000,
        "GRALLOC_USAGE_HW_CAMERA_READ":          0x00040000,
        "GRALLOC_USAGE_HW_CAMERA_ZSL":           0x00060000,
        "GRALLOC_USAGE_HW_CAMERA_MASK":          0x00060000,
        "GRALLOC_USAGE_HW_MASK":                 0x00071F00,
        "GRALLOC_USAGE_EXTERNAL_DISP":           0x00002000,
        "GRALLOC_USAGE_PROTECTED":               0x00004000,
        "GRALLOC_USAGE_PRIVATE_0":               0x10000000,
        "GRALLOC_USAGE_PRIVATE_1":               0x20000000,
        "GRALLOC_USAGE_PRIVATE_2":               0x40000000,
        "GRALLOC_USAGE_PRIVATE_3":               0x80000000,
        "GRALLOC_USAGE_PRIVATE_MASK":            0xF0000000,
        """
    }.items():
        if usage_temp & v == v:
            usage_test |= v
            print("%x %s" % (v, k))
    if usage_temp != usage_test: print(f"Note: Decoded missing value. {usage_temp} != {usage_test}.")

DECODE_GRALLOC_USAGE(sys.argv[1])