/*****************************************************************************
 * output.h: encoder output modules
 *****************************************************************************/

#ifndef X264_OUTPUT_H
#define X264_OUTPUT_H

#include "common/x264.h"
typedef struct
{
	int (*open_file)(char *psz_filename, void **p_handle, int b_ddr_output);
	int (*write_headers)(void *handle, x264_nal_t *p_nal, int b_ddr_output);
	int (*write_frame)(void *handle, uint8_t *p_nal, int i_size, x264_picture_t *p_picture, int b_ddr_output);
	int (*close_file)(void *handle, int64_t largest_pts, int64_t second_largest_pts, int b_ddr_output);
} cli_output_t;

extern const cli_output_t cli_output;

#endif
