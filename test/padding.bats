#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test without-padding {
  run sh -c './configure --without-padding && cat config.h'
  assert_failure
}

@test with-padding {
  run sh -c './configure --with-padding && cat config.h'
  assert_output -p '#define PADDING'
}

@test with-padding=1 {
  run sh -c './configure --with-padding=1 && cat config.h'
  assert_output -p '#define PADDING 1'
}

@test with-padding=4 {
  run sh -c './configure --with-padding=4'
  assert_failure
}
