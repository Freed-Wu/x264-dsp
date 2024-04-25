checkmk = find_program('checkmk')
if checkmk.found()
  check_downsample_c = custom_target(
    'check_downsample.c',
    input: 'check_downsample.c.in',
    output: 'check_downsample.c',
    capture: true,
    command: [checkmk, '@INPUT@'],
  )
  check_dep = dependency('check', fallback: ['check', 'check_dep'])
  check_downsample = executable(
    'check_downsample',
    [check_downsample_c, '../downsample.c'],
    include_directories: include_directories('..'),
    dependencies: check_dep,
  )
  test('check_downsample', check_downsample)
endif
