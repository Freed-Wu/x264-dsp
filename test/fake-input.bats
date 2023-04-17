#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test enable-fake-input {
  run sh -c './configure --enable-fake-input && cat config.h'
  assert_output -p '#define FAKE_INPUT'
}

@test disable-fake-input {
  run sh -c './configure --disable-fake-input && cat config.h'
  refute_output -p '#define FAKE_INPUT'
}

@test disable-fake-input-default {
  run sh -c './configure && cat config.h'
  refute_output -p '#define FAKE_INPUT'
}
