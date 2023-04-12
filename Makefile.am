DEFS += -DX264_BIT_DEPTH=@with_x264_bit_depth@ -DX264_CHROMA_FORMAT=@with_x264_chroma_format@
bin_PROGRAMS = x264
x264_LDFLAGS = -lm
x264_SOURCES = \
input.c \
output.c \
x264.c \
encoder/analyse.c \
encoder/cabac.c \
encoder/cavlc.c \
encoder/encoder.c \
encoder/lookahead.c \
encoder/macroblock.c \
encoder/me.c \
encoder/ratecontrol.c \
encoder/set.c \
encoder/slicetype.c \
common/bitstream.c \
common/cabac.c \
common/common.c \
common/dct.c \
common/deblock.c \
common/frame.c \
common/macroblock.c \
common/mc.c \
common/mvpred.c \
common/pixel.c \
common/predict.c \
common/quant.c \
common/set.c \
common/vlc.c
dist_doc_DATA = README.md
