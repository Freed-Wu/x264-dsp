#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test without-downsample-scale {
  run sh -c './configure --without-downsample-scale'
  assert_failure
}

@test with-downsample-scale {
  run sh -c './configure --with-downsample-scale && cat config.h'
  assert_output -p '#define SCALE'
}

@test with-downsample-scale=1 {
  run sh -c './configure --with-downsample-scale=1 && cat config.h'
  assert_output -p '#define SCALE 1'
}
