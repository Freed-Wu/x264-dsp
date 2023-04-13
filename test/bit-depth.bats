#! /usr/bin/env bats
setup() {
	cd "$(dirname "$BATS_TEST_FILENAME")/.." || exit
	. test/setup.sh
}

@test without-x264-bit-depth {
	run sh -c './configure --without-x264-bit-depth'
	assert_failure
}

@test with-x264-bit-depth {
	run sh -c './configure --with-x264-bit-depth && cat config.h'
	assert_output -p '#define X264_BIT_DEPTH 8'
}

@test with-x264-bit-depth=10 {
	run sh -c './configure --with-x264-bit-depth=10 && cat config.h'
	assert_output -p '#define X264_BIT_DEPTH 10'
}

@test with-x264-bit-depth=9 {
	run sh -c './configure --with-x264-bit-depth=9'
	assert_failure
}
