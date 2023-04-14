#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test without-downsample {
  run sh -c './configure --without-downsample && cat config.h'
  refute_output -p '#define DOWNSAMPLE'
}

@test with-downsample {
  run sh -c './configure --with-downsample && cat config.h'
  assert_output -p '#define DOWNSAMPLE'
}

@test with-downsample=1 {
  run sh -c './configure --with-downsample=1 && cat config.h'
  assert_output -p '#define DOWNSAMPLE 1'
}

@test with-downsample=2 {
  run sh -c './configure --with-downsample=2'
  assert_failure
}
