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
	run sh -c './configure --with-x264-chroma-format'
	assert_output -p 'configure: define X264_CHROMA_FORMAT="1"'
}

@test with-x264-chroma-format=10 {
	run sh -c './configure --with-x264-chroma-format=0'
	assert_output -p 'configure: define X264_CHROMA_FORMAT="0"'
}

@test with-x264-chroma-format=9 {
	run sh -c './configure --with-x264-chroma-format=9'
	assert_failure
}
