/*
 * serenity
 * Copyright (C) 2013 Andrea Nardinocchi (andrea@nardinan.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef serenity_o_trb_event_h
#define serenity_o_trb_event_h
#include <math.h>
#include "../o_array.h"
#define d_trb_event_adcs 2
#ifdef d_version_0x1313
/* extended version of the miniTRB (version of the code 0x1313) */
#pragma message "Compiling INFN module for TRB version 0x1313 (10 VAs)"
#define d_trb_event_vas 10
#define d_trb_event_channels_on_va 64
#define d_trb_event_channels_on_va_half 32
#define d_trb_event_size_normal 2048
#define d_trb_event_channels 640
#define d_trb_event_channels_half 320
#else
/* default version of miniTRB (version of the code 0x1212) */
#pragma message "Compiling INFN module for TRB version 0x1212 (6 VAs)"
#define d_trb_event_vas 6
#define d_trb_event_channels_on_va 64
#define d_trb_event_channels_on_va_half 32
#define d_trb_event_size_normal 1024
#define d_trb_event_channels 384
#define d_trb_event_channels_half 192
#endif
#define d_trb_event_samples 100
#define d_trb_event_samples_half 50
#define d_trb_event_size_debug 112
#define d_trb_event_size_header 4
#define d_trb_event_header(v) (((v)[0]==0x90)&&((v)[1]==0xeb))
#define FLAGGED(cod,flg) ((((cod)&e_trb_event_channel_bad)==e_trb_event_channel_bad)&&(((cod)&(flg))==(flg)))
#define FLAGGED_sigma_raw(cod)\
  ((FLAGGED((cod),e_trb_event_channel_bad_sigma_raw_low))||\
   (FLAGGED((cod),e_trb_event_channel_bad_sigma_raw_high))||\
   (FLAGGED((cod),e_trb_event_channel_bad_sigma_raw_rms)))
#define FLAGGED_sigma(cod)\
  ((FLAGGED((cod),e_trb_event_channel_bad_sigma_low))||\
   (FLAGGED((cod),e_trb_event_channel_bad_sigma_high))||\
   (FLAGGED((cod),e_trb_event_channel_bad_sigma_rms)))
#define FLAGGED_occupancy(cod)\
  (FLAGGED((cod),e_trb_event_channel_bad_occupancy))
extern const char v_trb_event_kind[];
typedef enum e_trb_event_channels {
  e_trb_event_channel_bad                 = 0x00000001,
  e_trb_event_channel_bad_sigma_raw_low 	= 0x00000002,
  e_trb_event_channel_bad_sigma_raw_high 	= 0x00000004,
  e_trb_event_channel_bad_sigma_raw_rms	  = 0x00000008,
  e_trb_event_channel_bad_sigma_low	      = 0x00000010,
  e_trb_event_channel_bad_sigma_high	    = 0x00000020,
  e_trb_event_channel_bad_sigma_rms	      = 0x00000040,
  e_trb_event_channel_bad_occupancy	      = 0x00000080
} e_trb_event_channels;
typedef struct o_trb_event {
  d_object_head;
  unsigned char code, kind;
  unsigned short int values[d_trb_event_channels], temperature[2], version;
  int filled;
  unsigned char *(*m_load)(struct o_trb_event *, unsigned char *, unsigned char, size_t);
} o_trb_event;
extern void p_trb_event_hooking(struct o_trb_event *object);
extern unsigned int p_trb_event_align(unsigned char *buffer, size_t size);
extern float p_trb_event_mean_f(float *values, size_t size);
extern float p_trb_event_mean_i(int *values, size_t size);
extern float *p_trb_event_pedestal(struct o_trb_event *events, size_t size, float *supplied);
extern float *p_trb_event_sigma_raw(struct o_trb_event *events, size_t size, float *supplied);
extern float *p_trb_event_cn_no_pedestal(float *no_pedestal, float sigma_multiplicator, float *sigma, int *flags, float *supplied);
extern float *p_trb_event_cn(unsigned short int *values, float sigma_multiplicator, float *pedestal, float *sigma, int *flags, float *supplied);
extern float *p_trb_event_sigma(struct o_trb_event *events, size_t size, float sigma_multiplicator, float *sigma_raw, float *pedestal, int *flags,
    float *supplied);
extern size_t p_trb_event_size(unsigned char key, short int code);
extern struct o_trb_event *f_trb_event_new(struct o_trb_event *supplied);
extern int p_trb_event_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_trb_event_hash(struct o_object *object);
extern char *p_trb_event_string(struct o_object *object, char *data, size_t size);
extern unsigned char *p_trb_event_load(struct o_trb_event *object, unsigned char *raw_data, unsigned char kind, size_t size);
#endif

