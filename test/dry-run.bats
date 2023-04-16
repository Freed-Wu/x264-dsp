#! /usr/bin/env bats
setup() {
  cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
  . test/setup.sh
}

@test enable-dry-run {
  run sh -c './configure --enable-dry-run && cat config.h'
  assert_output -p '#define DRY_RUN'
}

@test disable-dry-run {
  run sh -c './configure --disable-dry-run && cat config.h'
  refute_output -p '#define DRY_RUN'
}

@test disable-dry-run-default {
  run sh -c './configure && cat config.h'
  refute_output -p '#define DRY_RUN'
}
