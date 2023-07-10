#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "../drivers/usb/usb_descriptors.h"

#include "../utils.h"

//--------------------------------------------------------------------+
// USB Video
//--------------------------------------------------------------------+
static unsigned frame_num = 0;
static unsigned tx_busy = 0;
static unsigned interval_ms = 1000 / FRAME_RATE;

/* YUY2 frame buffer */
static uint8_t frame_buffer[FRAME_WIDTH * FRAME_HEIGHT * 16 / 8];
static void __attribute__((unused))fill_color_bar(uint8_t *buffer, unsigned start_position)
{
  /* EBU color bars
   * See also https://stackoverflow.com/questions/6939422 */
  static uint8_t const bar_color[8][4] = {
    /*  Y,   U,   Y,   V */
    { 235, 128, 235, 128}, /* 100% White */
    { 219,  16, 219, 138}, /* Yellow */
    { 188, 154, 188,  16}, /* Cyan */
    { 173,  42, 173,  26}, /* Green */
    {  78, 214,  78, 230}, /* Magenta */
    {  63, 102,  63, 240}, /* Red */
    {  32, 240,  32, 118}, /* Blue */
    {  16, 128,  16, 128}, /* Black */
  };
  uint8_t *p;

  /* Generate the 1st line */
  uint8_t *end = &buffer[FRAME_WIDTH * 2];
  unsigned idx = (FRAME_WIDTH / 2 - 1) - (start_position % (FRAME_WIDTH / 2));
  p = &buffer[idx * 4];
  for (unsigned i = 0; i < 8; ++i) {
    for (int j = 0; j < FRAME_WIDTH / (2 * 8); ++j) {
      memcpy(p, &bar_color[i], 4);
      p += 4;
      if (end <= p) {
        p = buffer;
      }
    }
  }
  /* Duplicate the 1st line to the others */
  p = &buffer[FRAME_WIDTH * 2];
  for (unsigned i = 1; i < FRAME_HEIGHT; ++i) {
    memcpy(p, buffer, FRAME_WIDTH * 2);
    p += FRAME_WIDTH * 2;
  }
}


static void initSharedBuffer() {
    static_assert(FRAME_WIDTH == GBCAM_W && FRAME_HEIGHT == GBCAM_H);
    for (uint i = 0; i < GBCAM_H * GBCAM_W; i++) {
        frame_buffer[2*i]     = 0;
        frame_buffer[2*i + 1] = 128;
    }
}

static void copySharedBuffer() {
    static_assert(FRAME_WIDTH == GBCAM_W && FRAME_HEIGHT == GBCAM_H);
    for (uint i = 0; i < GBCAM_H * GBCAM_W; i++) {
        frame_buffer[2*i] = sharedBuffer.data[i];
    }
}

void video_task(void)
{
  static unsigned start_ms = 0;
  static unsigned already_sent = 0;

  if (!tud_video_n_streaming(0, 0)) {
    already_sent  = 0;
    frame_num     = 0;
    // if (sharedBuffer.phase == SNAPPED) {
    //     static bool dumped = 0;
    //     if (dumped) return;
    //     dumped = 1;
    //     dump(sharedBuffer.data, GBCAM_H * GBCAM_W);
    //     printf("dumped\n");
    // }
    return;
  }

  if (!already_sent) {
    already_sent = 1;
    start_ms = board_millis();
    tx_busy = 1;
    initSharedBuffer();
    // fill_color_bar(frame_buffer, frame_num);
    tud_video_n_frame_xfer(0, 0, (void*)frame_buffer, FRAME_WIDTH * FRAME_HEIGHT * 16/8);
  }

  unsigned cur = board_millis();
  if (sharedBuffer.phase == SNAPPED) {
    static unsigned prev_copy_ms = 0;
    copySharedBuffer();
    sharedBuffer.phase = SNAP_READY;
    printf("copy latency %d\n", cur - prev_copy_ms);
    prev_copy_ms = cur;
  }

  if (cur - start_ms < interval_ms) return; // not enough time
  if (tx_busy) return;

  start_ms += interval_ms;
  static unsigned prev_ms = 0;

  printf("latency %d, frame %d\n", cur - prev_ms, frame_num);
  prev_ms = cur;

  tx_busy = 1;
  //  fill_color_bar(frame_buffer, frame_num);
  tud_video_n_frame_xfer(0, 0, (void*)frame_buffer, FRAME_WIDTH * FRAME_HEIGHT * 16/8);
}

void tud_video_frame_xfer_complete_cb(uint_fast8_t ctl_idx, uint_fast8_t stm_idx)
{
  (void)ctl_idx; (void)stm_idx;
  tx_busy = 0;
  /* flip buffer */
  ++frame_num;
}

int tud_video_commit_cb(uint_fast8_t ctl_idx, uint_fast8_t stm_idx,
			video_probe_and_commit_control_t const *parameters)
{
  (void)ctl_idx; (void)stm_idx;
  /* convert unit to ms from 100 ns */
  interval_ms = parameters->dwFrameInterval / 10000;
  return VIDEO_ERROR_NONE;
}
