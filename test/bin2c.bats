#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test without-bin2c {
  run sh -c './configure --without-bin2c && cat config.h'
  refute_output -p '#define BIN2C'
}

@test with-bin2c {
  run sh -c './configure --with-bin2c'
  assert_failure
}

@test with-bin2c=configure.ac {
  run sh -c './configure --with-bin2c=configure.ac && cat config.h'
  assert_output -p '#define BIN2C'
}
