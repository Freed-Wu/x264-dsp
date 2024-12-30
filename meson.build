project(
  'x264-dsp',
  ['c', 'linearasm'],
)

conf_data = configuration_data()
conf_data.set(
  'HAVE_TIC6X',
  get_option('have_tic6x'),
  description: 'enable TI C6X asm',
)
if get_option('downsample').to_int() > 0
  conf_data.set(
    'DOWNSAMPLE',
    get_option('downsample'),
    description: 'downsample from 720p to 360p, 1, 2 means bilinear, bicubic',
  )
endif
conf_data.set(
  'PADDING',
  get_option('padding'),
  description: 'padding method, 1..3 means edge, reflect, symmetric',
)
conf_data.set(
  'SCALE',
  get_option('scale'),
  description: 'SCALE scale, a positive number',
)
conf_data.set(
  'X264_BIT_DEPTH',
  get_option('x264_bit_depth'),
  description: 'bit depth, can be 8 or 10',
)
conf_data.set(
  'X264_CHROMA_FORMAT',
  get_option('x264_chroma_format'),
  description: 'chroma format, 0..3 means 400, 420, 422, 444',
)
conf_data.set(
  'X264_LOG_LEVEL',
  get_option('x264_log_level'),
  description: 'log level, 0..3 means error, warning, info, debug',
)
cc = meson.get_compiler('c')
has_header = 0
if cc.has_header('string.h')
  has_header = 1
endif
conf_data.set(
  'HAVE_STRING_H',
  has_header,
  description: 'have <string.h>',
)
configure_file(output: 'config.h', configuration: conf_data)

add_project_arguments('-DHAVE_CONFIG_H=1', language: ['c', 'linearasm'])
files = [
  'downsample.c',
  'input.c',
  'output.c',
  'x264.c',
  'encoder/analyse.c',
  'encoder/cabac.c',
  'encoder/cavlc.c',
  'encoder/encoder.c',
  'encoder/lookahead.c',
  'encoder/macroblock.c',
  'encoder/me.c',
  'encoder/ratecontrol.c',
  'encoder/set.c',
  'encoder/slicetype.c',
  'extras/getopt.c',
  'common/bitstream.c',
  'common/cabac.c',
  'common/common.c',
  'common/dct.c',
  'common/deblock.c',
  'common/frame.c',
  'common/macroblock.c',
  'common/mc.c',
  'common/mvpred.c',
  'common/pixel.c',
  'common/predict.c',
  'common/quant.c',
  'common/set.c',
  'common/vlc.c',
]
objs = []
deps = []
if cc.get_id() == 'c6000'
  cmd_file = join_paths(meson.current_source_dir(), get_option('cmd_file'))
  add_project_arguments(
    '--cmd_file=' + cmd_file,
    language: ['c', 'linearasm'],
  )
  objs += [join_paths(meson.current_source_dir(), get_option('lnk_file'))]
  files += [
    'common/bitstream-a.sa',
    'common/dct-a.sa',
    'common/deblock-a.sa',
    'common/mc-a.sa',
    'common/pixel-a.sa',
    'common/predict-a.sa',
    'common/quant-a.sa',
  ]
else
  deps += [cc.find_library('m', required: false)]
endif

executable(
  'x264',
  files,
  install: true,
  dependencies: deps,
  objects: objs,
)

subdir('tests')
