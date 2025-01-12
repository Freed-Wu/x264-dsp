#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __TI_COMPILER_VERSION__
#include <c6x.h>
// Don't use HOSTclock() !
#define clock() ((uint64_t)TSCH << 32 | TSCL)
// 990 MHz
#define CLOCKS_PER_SEC 990000000
#else
#include <time.h>
#endif
#include "common/x264.h"
#include "input.h"
#include "output.h"

#include "extras/getopt.h"

#define DEFAULT_BUFFER_SIZE 256

/* In microseconds */
#define UPDATE_INTERVAL 1000000

typedef struct {
	int b_progress; /* whether print encoding progress to console */
	int i_seek;	/* seek position of start encoding */
	void *hin;	/* input handle */
	void *hout;	/* output handle */
	int b_ddr_input;
	int b_ddr_output;
} cli_opt_t;

/* logging and printing for within the cli system */
static int cli_log_level;
void x264_cli_log(const char *name, int i_level, const char *fmt, ...) {
	char *s_level;
	va_list arg;
	if (i_level > cli_log_level)
		return;
	switch (i_level) {
	case X264_LOG_ERROR:
		s_level = "error";
		break;
	case X264_LOG_WARNING:
		s_level = "warning";
		break;
	case X264_LOG_INFO:
		s_level = "info";
		break;
	case X264_LOG_DEBUG:
		s_level = "debug";
		break;
	default:
		s_level = "unknown";
		break;
	}
	printf("%s [%s]: ", name, s_level);
	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
}

#define FAIL_IF_ERROR(cond, ...)                                   \
	if (cond) {                                                \
		x264_cli_log("x264", X264_LOG_ERROR, __VA_ARGS__); \
		return -1;                                         \
	}

#define FAIL_IF_ERROR2(cond, ...)                                  \
	if (cond) {                                                \
		x264_cli_log("x264", X264_LOG_ERROR, __VA_ARGS__); \
		retval = -1;                                       \
		goto fail;                                         \
	}

static int parse(int argc, char **argv, x264_param_t *param, cli_opt_t *opt);
static int encode(x264_param_t *param, cli_opt_t *opt);

int main(int argc, char **argv) {
#ifdef __TI_COMPILER_VERSION__
	TSCL = 0;
#endif
	x264_param_t param;
	cli_opt_t opt = {0};
	int ret = EXIT_SUCCESS;

	/* Parse command line */
	if (parse(argc, argv, &param, &opt) > 0)
		ret = EXIT_FAILURE;

	if (ret == EXIT_SUCCESS)
		ret = encode(&param, &opt);

	/* clean up handles */
	if (opt.hin)
		cli_input.close_file(opt.hin, opt.b_ddr_input);
	if (opt.hout)
		cli_output.close_file(opt.hout, 0, 0, opt.b_ddr_output);

	return ret;
}

static char shortopts[] = "hVf:i:o:I:O:1:0:";
static struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {"frames", required_argument, NULL, 'f'},
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"disable-ddr-input", no_argument, NULL, 'I'},
    {"disable-ddr-output", no_argument, NULL, 'O'},
    {"ddr-input-address", required_argument, NULL, '1'},
    {"ddr-output-address", required_argument, NULL, '0'},
    {NULL, 0, NULL, 0}};

int print_help(const struct option *longopts) {
	unsigned int i = 0;
	struct option o = longopts[i];
	while (o.name != NULL) {
		char name[DEFAULT_BUFFER_SIZE];
		char value[DEFAULT_BUFFER_SIZE + sizeof("(|)") - 1];
		char meta[DEFAULT_BUFFER_SIZE];
		char *str = meta;

		if (isascii(o.val))
			sprintf(name, "(--%s|-%c)", o.name, (char)o.val);
		else
			sprintf(name, "--%s", o.name);

		sprintf(meta, "%s", o.name);
		do
			*str = (char)toupper(*str);
		while (*str++);

		if (o.has_arg == required_argument)
			sprintf(value, " %s", meta);
		else if (o.has_arg == optional_argument)
			sprintf(value, "( %s)", meta);
		else
			value[0] = '\0';

		printf(" [%s%s]", name, value);

		o = longopts[++i];
	}
	return EXIT_SUCCESS;
}

static int parse(int argc, char **argv, x264_param_t *param, cli_opt_t *opt) {
	char *input_filename = "352x288.yuv";
	char *output_filename = "out.264";
	video_info_t info = {0};
	int c;
#ifdef __TI_COMPILER_VERSION__
	opt->b_ddr_input = 1;
	opt->b_ddr_output = 1;
#endif

	opt->b_progress = 1;
	info.num_frames = 1;
	while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		switch (c) {
		case 'h':
			printf("x264");
			print_help(longopts);
			puts("");
			return 1;
		case 'V':
			puts("0.0.1");
			return 1;
		case 'f':
			info.num_frames = strtol(optarg, NULL, 0);
			break;
		case 'i':
			input_filename = optarg;
			break;
		case 'o':
			output_filename = optarg;
			break;
		case 'I':
			opt->b_ddr_input = 0;
			break;
		case 'O':
			opt->b_ddr_output = 0;
			break;
		default:
			return 1;
		}
	}

	x264_param_default(param);
	cli_log_level = param->i_log_level;

	/* set info flags to be overwritten by demuxer as necessary. */
	info.csp = param->i_csp;
	info.fps_num = param->i_fps_num;
	info.fps_den = param->i_fps_den;
	info.fullrange = param->vui.b_fullrange;
	info.interlaced = param->b_interlaced;
	info.sar_width = param->vui.i_sar_width;
	info.sar_height = param->vui.i_sar_height;
	info.tff = param->b_tff;
	info.vfr = param->b_vfr_input;

	FAIL_IF_ERROR(cli_output.open_file(output_filename, &opt->hout, opt->b_ddr_output),
		      "could not open output file `%s'\n", output_filename)

	FAIL_IF_ERROR(cli_input.open_file(input_filename, &opt->hin, &info, opt->b_ddr_input),
		      "could not open input file `%s'\n", input_filename)

	x264_cli_log(input_filename, X264_LOG_INFO, "%dx%d%c %u:%u @ %u/%u fps (%cfr)\n", info.width,
		     info.height, info.interlaced ? 'i' : 'p', info.sar_width, info.sar_height,
		     info.fps_num, info.fps_den, info.vfr ? 'v' : 'c');

	/* force end result resolution */
	if (!param->i_width && !param->i_height) {
		param->i_height = info.height;
		param->i_width = info.width;
	}

	info.num_frames = ((info.num_frames - opt->i_seek) > 0) ? (info.num_frames - opt->i_seek) : 0;
	if ((!info.num_frames || param->i_frame_total < info.num_frames) && param->i_frame_total > 0)
		info.num_frames = param->i_frame_total;
	param->i_frame_total = info.num_frames;

	return 0;
}

static int encode_frame(x264_t *h, cli_opt_t *opt, x264_picture_t *pic, int64_t *last_dts) {
	x264_picture_t pic_out;
	x264_nal_t *nal;
	int i_nal;
	int i_frame_size = 0;

	i_frame_size = x264_encoder_encode(h, &nal, &i_nal, pic, &pic_out);

	FAIL_IF_ERROR(i_frame_size < 0, "x264_encoder_encode failed\n");

	if (i_frame_size) {
		i_frame_size = cli_output.write_frame(opt->hout, nal[0].p_payload, i_frame_size, &pic_out, opt->b_ddr_output);
		*last_dts = pic_out.i_dts;
	}

	return i_frame_size;
}

static int64_t print_status(int64_t i_start, int64_t i_previous, int i_frame, int i_frame_total, int64_t i_file, x264_param_t *param, int64_t last_ts) {
	int64_t i_time = clock() * 1000000 / CLOCKS_PER_SEC;
	int64_t i_elapsed;
	double fps, bitrate;

	if (i_previous && i_time - i_previous < UPDATE_INTERVAL)
		return i_previous;

	i_elapsed = i_time - i_start;
	fps = i_elapsed > 0 ? i_frame * 1000000. / i_elapsed : 0;
	if (last_ts)
		bitrate = (double)i_file * 8 / ((double)last_ts * 1000 * param->i_timebase_num / param->i_timebase_den);
	else
		bitrate = (double)i_file * 8 / ((double)1000 * param->i_fps_den / param->i_fps_num);
	if (i_frame_total) {
		int eta = i_elapsed * (i_frame_total - i_frame) / ((int64_t)i_frame * 1000000);
		printf("x264 [%.1f%%] %d/%d frames, %.2f fps, %.2f kb/s, eta %d:%02d:%02d\n",
		       100. * i_frame / i_frame_total, i_frame, i_frame_total, fps, bitrate,
		       eta / 3600, (eta / 60) % 60, eta % 60);
	} else {
		printf("x264 %d frames: %.2f fps, %.2f kb/s\n", i_frame, fps, bitrate);
	}

	return i_time;
}

static int encode(x264_param_t *param, cli_opt_t *opt) {
	x264_t *h = NULL;
	x264_picture_t pic;
	cli_pic_t cli_pic;

	int i_frame = 0;
	int i_frame_output = 0;
	int64_t i_end, i_previous = 0, i_start = 0;
	int64_t i_file = 0;
	int i_frame_size;
	int64_t last_dts = 0;
	int64_t prev_dts = 0;
	int64_t first_dts = 0;
#define MAX_PTS_WARNING 3 /* arbitrary */
	int pts_warning_cnt = 0;
	int64_t largest_pts = -1;
	int64_t second_largest_pts = -1;
	int64_t ticks_per_frame;
	double duration;
	int retval = 0;

	opt->b_progress &= param->i_log_level < X264_LOG_DEBUG;

	h = x264_encoder_open(param);
	FAIL_IF_ERROR2(!h, "x264_encoder_open failed\n");

	x264_encoder_parameters(h, param);

	i_start = clock() * 1000000 / CLOCKS_PER_SEC;

	/* ticks/frame = ticks/second / frames/second */
	ticks_per_frame = (int64_t)param->i_timebase_den * param->i_fps_den / param->i_timebase_num / param->i_fps_num;
	FAIL_IF_ERROR2(ticks_per_frame < 1 && !param->b_vfr_input, "ticks_per_frame invalid: %lld\n", ticks_per_frame)
	ticks_per_frame = (ticks_per_frame > 1) ? ticks_per_frame : 1;

	if (!param->b_repeat_headers) {
		// Write SPS/PPS/SEI
		x264_nal_t *headers;
		int i_nal;

		FAIL_IF_ERROR2(x264_encoder_headers(h, &headers, &i_nal) < 0, "x264_encoder_headers failed\n")
		FAIL_IF_ERROR2((i_file = cli_output.write_headers(opt->hout, headers, opt->b_ddr_output)) < 0, "error writing headers to output file\n");
	}

	/* Alloc picture for encoding frame */
	FAIL_IF_ERROR2(cli_input.picture_alloc(&cli_pic, param->i_csp, param->i_width, param->i_height), "can't alloc picture\n");

	/* Encode frames */
	for (; (i_frame < param->i_frame_total || !param->i_frame_total); i_frame++) {
		if (cli_input.read_frame(&cli_pic, opt->hin, i_frame + opt->i_seek, opt->b_ddr_input))
			break;

		x264_picture_init(&pic);
		memcpy(pic.img.i_stride, cli_pic.img.stride, sizeof(cli_pic.img.stride));
		memcpy(pic.img.plane, cli_pic.img.plane, sizeof(cli_pic.img.plane));
		pic.img.i_plane = cli_pic.img.planes;
		pic.img.i_csp = cli_pic.img.csp;
		pic.i_pts = cli_pic.pts;

		if (!param->b_vfr_input)
			pic.i_pts = i_frame;

		if (pic.i_pts <= largest_pts) {
			if (cli_log_level >= X264_LOG_DEBUG || pts_warning_cnt < MAX_PTS_WARNING)
				x264_cli_log("x264", X264_LOG_WARNING, "non-strictly-monotonic pts at frame %d (%lld <= %lld)\n", i_frame, pic.i_pts, largest_pts);
			else if (pts_warning_cnt == MAX_PTS_WARNING)
				x264_cli_log("x264", X264_LOG_WARNING, "too many non-monotonic pts warnings, suppressing further ones\n");
			pts_warning_cnt++;
			pic.i_pts = largest_pts + ticks_per_frame;
		}

		second_largest_pts = largest_pts;
		largest_pts = pic.i_pts;

		prev_dts = last_dts;
		i_frame_size = encode_frame(h, opt, &pic, &last_dts);
		if (i_frame_size < 0) {
			retval = -1;
		} else if (i_frame_size) {
			i_file += i_frame_size;
			i_frame_output++;
			if (i_frame_output == 1)
				first_dts = prev_dts = last_dts;
		}

		if (cli_input.release_frame && cli_input.release_frame(&cli_pic, opt->hin))
			break;

/* update status line (up to 1000 times per input file) */
#ifndef __TI_COMPILER_VERSION__
		if (cli_log_level >= X264_LOG_DEBUG && opt->b_progress && i_frame_output)
			i_previous = print_status(i_start, i_previous, i_frame_output, param->i_frame_total, i_file, param, 2 * last_dts - prev_dts - first_dts);
#endif
	}

	cli_input.picture_clean(&cli_pic);

fail:
	if (pts_warning_cnt >= MAX_PTS_WARNING && cli_log_level < X264_LOG_DEBUG)
		x264_cli_log("x264", X264_LOG_WARNING, "%d suppressed non-monotonic pts warnings\n", pts_warning_cnt - MAX_PTS_WARNING);

	/* duration algorithm fails when only 1 frame is output */
	if (i_frame_output == 1)
		duration = (double)param->i_fps_den / param->i_fps_num;
	else
		duration = (double)(2 * largest_pts - second_largest_pts) * param->i_timebase_num / param->i_timebase_den;

	i_end = clock() * 1000000 / CLOCKS_PER_SEC;
	if (h)
		x264_encoder_close(h);

	cli_output.close_file(opt->hout, largest_pts, second_largest_pts, opt->b_ddr_output);
	opt->hout = NULL;

	if (i_frame_output > 0) {
		double fps = (double)i_frame_output * (double)1000000 / (double)(i_end - i_start);
		printf("encoded %d frames, %.2f fps, %.2f kb/s\n", i_frame_output, fps, (double)i_file * 8 / (1000 * duration));
	}

	return retval;
}
