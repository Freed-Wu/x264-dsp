#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test without-x264-log-level {
  run sh -c './configure --without-x264-log-level'
  assert_failure
}

@test with-x264-log-level {
  run sh -c './configure --with-x264-log-level && cat config.h'
  assert_output -p '#define X264_LOG_LEVEL'
}

@test with-x264-log-level=0 {
  run sh -c './configure --with-x264-log-level=0 && cat config.h'
  assert_output -p '#define X264_LOG_LEVEL 0'
}

@test with-x264-log-level=4 {
  run sh -c './configure --with-x264-log-level=4'
  assert_failure
}
