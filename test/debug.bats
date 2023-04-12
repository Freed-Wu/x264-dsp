#! /usr/bin/env bats
setup() {
	cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
	. test/setup.sh
}

@test enable-debug {
	run sh -c './configure --enable-debug && make -n'
	assert_output -p '-ggdb'
}

@test disable-debug {
	run sh -c './configure --disable-debug && make -n'
	refute_output -p '-ggdb'
}

@test disable-debug-default {
	run sh -c './configure && make -n'
	refute_output -p '-ggdb'
}
