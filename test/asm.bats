#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test enable-asm {
  run sh -c './configure --enable-asm && cat config.h'
  assert_output -p '#define HAVE_TIC6X'
}

@test disable-asm {
  run sh -c './configure --disable-asm && cat config.h'
  refute_output -p '#define HAVE_TIC6X'
}

@test enable-asm-default {
  run sh -c './configure && cat config.h'
  assert_output -p '#define HAVE_TIC6X'
}
