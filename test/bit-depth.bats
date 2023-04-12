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
	run sh -c './configure --with-x264-bit-depth'
	assert_output -p 'configure: define X264_BIT_DEPTH="8"'
}

@test with-x264-bit-depth=10 {
	run sh -c './configure --with-x264-bit-depth=10'
	assert_output -p 'configure: define X264_BIT_DEPTH="10"'
}

@test with-x264-bit-depth=9 {
	run sh -c './configure --with-x264-bit-depth=9'
	assert_failure
}
