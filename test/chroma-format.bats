#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test without-x264-chroma-format {
  run sh -c './configure --without-x264-chroma-format'
  assert_failure
}

@test with-x264-chroma-format {
  run sh -c './configure --with-x264-chroma-format && cat config.h'
  assert_output -p '#define X264_CHROMA_FORMAT'
}

@test with-x264-chroma-format=0 {
  run sh -c './configure --with-x264-chroma-format=0 && cat config.h'
  assert_output -p '#define X264_CHROMA_FORMAT 0'
}

@test with-x264-chroma-format=4 {
  run sh -c './configure --with-x264-chroma-format=9'
  assert_failure
}
