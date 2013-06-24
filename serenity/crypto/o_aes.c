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
#include "o_aes.h"
#include <unistd.h>
#include <stdlib.h>
const char v_aes_kind[] = "aes";
unsigned int v_aes_rcon[] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
	0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a,
	0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25,
	0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08,
	0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6,
	0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61,
	0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01,
	0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e,
	0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4,
	0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8,
	0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
	0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91,
	0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d,
	0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c,
	0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa,
	0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66,
	0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
}, v_aes_sbox[] = {
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
	0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
	0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
	0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
	0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
	0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
	0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
	0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
	0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
	0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
	0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
	0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
	0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
	0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
	0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
	0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
	0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
}, v_aes_sbox_inv[] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
	0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
	0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
	0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
	0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
	0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
	0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
	0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
	0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
	0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
	0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
	0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
	0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
	0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
	0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
	0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
	0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
}, v_aes_mul_2[] = {
	0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e,
	0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e,
	0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e,
	0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e,
	0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e,
	0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e,
	0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e,
	0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e,
	0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e,
	0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
	0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae,
	0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe,
	0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce,
	0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde,
	0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee,
	0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe,
	0x1b, 0x19, 0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15,
	0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05,
	0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35,
	0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25,
	0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55,
	0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45,
	0x7b, 0x79, 0x7f, 0x7d, 0x73, 0x71, 0x77, 0x75,
	0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65,
	0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95,
	0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85,
	0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5,
	0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5,
	0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1, 0xd7, 0xd5,
	0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
	0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5,
	0xeb, 0xe9, 0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5
}, v_aes_mul_3[] = {
	0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09,
	0x18, 0x1b, 0x1e, 0x1d, 0x14, 0x17, 0x12, 0x11,
	0x30, 0x33, 0x36, 0x35, 0x3c, 0x3f, 0x3a, 0x39,
	0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27, 0x22, 0x21,
	0x60, 0x63, 0x66, 0x65, 0x6c, 0x6f, 0x6a, 0x69,
	0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71,
	0x50, 0x53, 0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59,
	0x48, 0x4b, 0x4e, 0x4d, 0x44, 0x47, 0x42, 0x41,
	0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf, 0xca, 0xc9,
	0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1,
	0xf0, 0xf3, 0xf6, 0xf5, 0xfc, 0xff, 0xfa, 0xf9,
	0xe8, 0xeb, 0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1,
	0xa0, 0xa3, 0xa6, 0xa5, 0xac, 0xaf, 0xaa, 0xa9,
	0xb8, 0xbb, 0xbe, 0xbd, 0xb4, 0xb7, 0xb2, 0xb1,
	0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99,
	0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81,
	0x9b, 0x98, 0x9d, 0x9e, 0x97, 0x94, 0x91, 0x92,
	0x83, 0x80, 0x85, 0x86, 0x8f, 0x8c, 0x89, 0x8a,
	0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4, 0xa1, 0xa2,
	0xb3, 0xb0, 0xb5, 0xb6, 0xbf, 0xbc, 0xb9, 0xba,
	0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2,
	0xe3, 0xe0, 0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea,
	0xcb, 0xc8, 0xcd, 0xce, 0xc7, 0xc4, 0xc1, 0xc2,
	0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc, 0xd9, 0xda,
	0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52,
	0x43, 0x40, 0x45, 0x46, 0x4f, 0x4c, 0x49, 0x4a,
	0x6b, 0x68, 0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62,
	0x73, 0x70, 0x75, 0x76, 0x7f, 0x7c, 0x79, 0x7a,
	0x3b, 0x38, 0x3d, 0x3e, 0x37, 0x34, 0x31, 0x32,
	0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a,
	0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02,
	0x13, 0x10, 0x15, 0x16, 0x1f, 0x1c, 0x19, 0x1a
}, v_aes_mul_9[] = {
	0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	0x90, 0x99, 0x82, 0x8b, 0xb4, 0xbd, 0xa6, 0xaf,
	0xd8, 0xd1, 0xca, 0xc3, 0xfc, 0xf5, 0xee, 0xe7,
	0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	0xab, 0xa2, 0xb9, 0xb0, 0x8f, 0x86, 0x9d, 0x94,
	0xe3, 0xea, 0xf1, 0xf8, 0xc7, 0xce, 0xd5, 0xdc,
	0x76, 0x7f, 0x64, 0x6d, 0x52, 0x5b, 0x40, 0x49,
	0x3e, 0x37, 0x2c, 0x25, 0x1a, 0x13, 0x08, 0x01,
	0xe6, 0xef, 0xf4, 0xfd, 0xc2, 0xcb, 0xd0, 0xd9,
	0xae, 0xa7, 0xbc, 0xb5, 0x8a, 0x83, 0x98, 0x91,
	0x4d, 0x44, 0x5f, 0x56, 0x69, 0x60, 0x7b, 0x72,
	0x05, 0x0c, 0x17, 0x1e, 0x21, 0x28, 0x33, 0x3a,
	0xdd, 0xd4, 0xcf, 0xc6, 0xf9, 0xf0, 0xeb, 0xe2,
	0x95, 0x9c, 0x87, 0x8e, 0xb1, 0xb8, 0xa3, 0xaa,
	0xec, 0xe5, 0xfe, 0xf7, 0xc8, 0xc1, 0xda, 0xd3,
	0xa4, 0xad, 0xb6, 0xbf, 0x80, 0x89, 0x92, 0x9b,
	0x7c, 0x75, 0x6e, 0x67, 0x58, 0x51, 0x4a, 0x43,
	0x34, 0x3d, 0x26, 0x2f, 0x10, 0x19, 0x02, 0x0b,
	0xd7, 0xde, 0xc5, 0xcc, 0xf3, 0xfa, 0xe1, 0xe8,
	0x9f, 0x96, 0x8d, 0x84, 0xbb, 0xb2, 0xa9, 0xa0,
	0x47, 0x4e, 0x55, 0x5c, 0x63, 0x6a, 0x71, 0x78,
	0x0f, 0x06, 0x1d, 0x14, 0x2b, 0x22, 0x39, 0x30,
	0x9a, 0x93, 0x88, 0x81, 0xbe, 0xb7, 0xac, 0xa5,
	0xd2, 0xdb, 0xc0, 0xc9, 0xf6, 0xff, 0xe4, 0xed,
	0x0a, 0x03, 0x18, 0x11, 0x2e, 0x27, 0x3c, 0x35,
	0x42, 0x4b, 0x50, 0x59, 0x66, 0x6f, 0x74, 0x7d,
	0xa1, 0xa8, 0xb3, 0xba, 0x85, 0x8c, 0x97, 0x9e,
	0xe9, 0xe0, 0xfb, 0xf2, 0xcd, 0xc4, 0xdf, 0xd6,
	0x31, 0x38, 0x23, 0x2a, 0x15, 0x1c, 0x07, 0x0e,
	0x79, 0x70, 0x6b, 0x62, 0x5d, 0x54, 0x4f, 0x46
}, v_aes_mul_11[] = {
	0x00, 0x0b, 0x16, 0x1d, 0x2c, 0x27, 0x3a, 0x31,
	0x58, 0x53, 0x4e, 0x45, 0x74, 0x7f, 0x62, 0x69,
	0xb0, 0xbb, 0xa6, 0xad, 0x9c, 0x97, 0x8a, 0x81,
	0xe8, 0xe3, 0xfe, 0xf5, 0xc4, 0xcf, 0xd2, 0xd9,
	0x7b, 0x70, 0x6d, 0x66, 0x57, 0x5c, 0x41, 0x4a,
	0x23, 0x28, 0x35, 0x3e, 0x0f, 0x04, 0x19, 0x12,
	0xcb, 0xc0, 0xdd, 0xd6, 0xe7, 0xec, 0xf1, 0xfa,
	0x93, 0x98, 0x85, 0x8e, 0xbf, 0xb4, 0xa9, 0xa2,
	0xf6, 0xfd, 0xe0, 0xeb, 0xda, 0xd1, 0xcc, 0xc7,
	0xae, 0xa5, 0xb8, 0xb3, 0x82, 0x89, 0x94, 0x9f,
	0x46, 0x4d, 0x50, 0x5b, 0x6a, 0x61, 0x7c, 0x77,
	0x1e, 0x15, 0x08, 0x03, 0x32, 0x39, 0x24, 0x2f,
	0x8d, 0x86, 0x9b, 0x90, 0xa1, 0xaa, 0xb7, 0xbc,
	0xd5, 0xde, 0xc3, 0xc8, 0xf9, 0xf2, 0xef, 0xe4,
	0x3d, 0x36, 0x2b, 0x20, 0x11, 0x1a, 0x07, 0x0c,
	0x65, 0x6e, 0x73, 0x78, 0x49, 0x42, 0x5f, 0x54,
	0xf7, 0xfc, 0xe1, 0xea, 0xdb, 0xd0, 0xcd, 0xc6,
	0xaf, 0xa4, 0xb9, 0xb2, 0x83, 0x88, 0x95, 0x9e,
	0x47, 0x4c, 0x51, 0x5a, 0x6b, 0x60, 0x7d, 0x76,
	0x1f, 0x14, 0x09, 0x02, 0x33, 0x38, 0x25, 0x2e,
	0x8c, 0x87, 0x9a, 0x91, 0xa0, 0xab, 0xb6, 0xbd,
	0xd4, 0xdf, 0xc2, 0xc9, 0xf8, 0xf3, 0xee, 0xe5,
	0x3c, 0x37, 0x2a, 0x21, 0x10, 0x1b, 0x06, 0x0d,
	0x64, 0x6f, 0x72, 0x79, 0x48, 0x43, 0x5e, 0x55,
	0x01, 0x0a, 0x17, 0x1c, 0x2d, 0x26, 0x3b, 0x30,
	0x59, 0x52, 0x4f, 0x44, 0x75, 0x7e, 0x63, 0x68,
	0xb1, 0xba, 0xa7, 0xac, 0x9d, 0x96, 0x8b, 0x80,
	0xe9, 0xe2, 0xff, 0xf4, 0xc5, 0xce, 0xd3, 0xd8,
	0x7a, 0x71, 0x6c, 0x67, 0x56, 0x5d, 0x40, 0x4b,
	0x22, 0x29, 0x34, 0x3f, 0x0e, 0x05, 0x18, 0x13,
	0xca, 0xc1, 0xdc, 0xd7, 0xe6, 0xed, 0xf0, 0xfb,
	0x92, 0x99, 0x84, 0x8f, 0xbe, 0xb5, 0xa8, 0xa3
}, v_aes_mul_13[] = {
	0x00, 0x0d, 0x1a, 0x17, 0x34, 0x39, 0x2e, 0x23,
	0x68, 0x65, 0x72, 0x7f, 0x5c, 0x51, 0x46, 0x4b,
	0xd0, 0xdd, 0xca, 0xc7, 0xe4, 0xe9, 0xfe, 0xf3,
	0xb8, 0xb5, 0xa2, 0xaf, 0x8c, 0x81, 0x96, 0x9b,
	0xbb, 0xb6, 0xa1, 0xac, 0x8f, 0x82, 0x95, 0x98,
	0xd3, 0xde, 0xc9, 0xc4, 0xe7, 0xea, 0xfd, 0xf0,
	0x6b, 0x66, 0x71, 0x7c, 0x5f, 0x52, 0x45, 0x48,
	0x03, 0x0e, 0x19, 0x14, 0x37, 0x3a, 0x2d, 0x20,
	0x6d, 0x60, 0x77, 0x7a, 0x59, 0x54, 0x43, 0x4e,
	0x05, 0x08, 0x1f, 0x12, 0x31, 0x3c, 0x2b, 0x26,
	0xbd, 0xb0, 0xa7, 0xaa, 0x89, 0x84, 0x93, 0x9e,
	0xd5, 0xd8, 0xcf, 0xc2, 0xe1, 0xec, 0xfb, 0xf6,
	0xd6, 0xdb, 0xcc, 0xc1, 0xe2, 0xef, 0xf8, 0xf5,
	0xbe, 0xb3, 0xa4, 0xa9, 0x8a, 0x87, 0x90, 0x9d,
	0x06, 0x0b, 0x1c, 0x11, 0x32, 0x3f, 0x28, 0x25,
	0x6e, 0x63, 0x74, 0x79, 0x5a, 0x57, 0x40, 0x4d,
	0xda, 0xd7, 0xc0, 0xcd, 0xee, 0xe3, 0xf4, 0xf9,
	0xb2, 0xbf, 0xa8, 0xa5, 0x86, 0x8b, 0x9c, 0x91,
	0x0a, 0x07, 0x10, 0x1d, 0x3e, 0x33, 0x24, 0x29,
	0x62, 0x6f, 0x78, 0x75, 0x56, 0x5b, 0x4c, 0x41,
	0x61, 0x6c, 0x7b, 0x76, 0x55, 0x58, 0x4f, 0x42,
	0x09, 0x04, 0x13, 0x1e, 0x3d, 0x30, 0x27, 0x2a,
	0xb1, 0xbc, 0xab, 0xa6, 0x85, 0x88, 0x9f, 0x92,
	0xd9, 0xd4, 0xc3, 0xce, 0xed, 0xe0, 0xf7, 0xfa,
	0xb7, 0xba, 0xad, 0xa0, 0x83, 0x8e, 0x99, 0x94,
	0xdf, 0xd2, 0xc5, 0xc8, 0xeb, 0xe6, 0xf1, 0xfc,
	0x67, 0x6a, 0x7d, 0x70, 0x53, 0x5e, 0x49, 0x44,
	0x0f, 0x02, 0x15, 0x18, 0x3b, 0x36, 0x21, 0x2c,
	0x0c, 0x01, 0x16, 0x1b, 0x38, 0x35, 0x22, 0x2f,
	0x64, 0x69, 0x7e, 0x73, 0x50, 0x5d, 0x4a, 0x47,
	0xdc, 0xd1, 0xc6, 0xcb, 0xe8, 0xe5, 0xf2, 0xff,
	0xb4, 0xb9, 0xae, 0xa3, 0x80, 0x8d, 0x9a, 0x97
}, v_aes_mul_14[] = {
	0x00, 0x0e, 0x1c, 0x12, 0x38, 0x36, 0x24, 0x2a,
	0x70, 0x7e, 0x6c, 0x62, 0x48, 0x46, 0x54, 0x5a,
	0xe0, 0xee, 0xfc, 0xf2, 0xd8, 0xd6, 0xc4, 0xca,
	0x90, 0x9e, 0x8c, 0x82, 0xa8, 0xa6, 0xb4, 0xba,
	0xdb, 0xd5, 0xc7, 0xc9, 0xe3, 0xed, 0xff, 0xf1,
	0xab, 0xa5, 0xb7, 0xb9, 0x93, 0x9d, 0x8f, 0x81,
	0x3b, 0x35, 0x27, 0x29, 0x03, 0x0d, 0x1f, 0x11,
	0x4b, 0x45, 0x57, 0x59, 0x73, 0x7d, 0x6f, 0x61,
	0xad, 0xa3, 0xb1, 0xbf, 0x95, 0x9b, 0x89, 0x87,
	0xdd, 0xd3, 0xc1, 0xcf, 0xe5, 0xeb, 0xf9, 0xf7,
	0x4d, 0x43, 0x51, 0x5f, 0x75, 0x7b, 0x69, 0x67,
	0x3d, 0x33, 0x21, 0x2f, 0x05, 0x0b, 0x19, 0x17,
	0x76, 0x78, 0x6a, 0x64, 0x4e, 0x40, 0x52, 0x5c,
	0x06, 0x08, 0x1a, 0x14, 0x3e, 0x30, 0x22, 0x2c,
	0x96, 0x98, 0x8a, 0x84, 0xae, 0xa0, 0xb2, 0xbc,
	0xe6, 0xe8, 0xfa, 0xf4, 0xde, 0xd0, 0xc2, 0xcc,
	0x41, 0x4f, 0x5d, 0x53, 0x79, 0x77, 0x65, 0x6b,
	0x31, 0x3f, 0x2d, 0x23, 0x09, 0x07, 0x15, 0x1b,
	0xa1, 0xaf, 0xbd, 0xb3, 0x99, 0x97, 0x85, 0x8b,
	0xd1, 0xdf, 0xcd, 0xc3, 0xe9, 0xe7, 0xf5, 0xfb,
	0x9a, 0x94, 0x86, 0x88, 0xa2, 0xac, 0xbe, 0xb0,
	0xea, 0xe4, 0xf6, 0xf8, 0xd2, 0xdc, 0xce, 0xc0,
	0x7a, 0x74, 0x66, 0x68, 0x42, 0x4c, 0x5e, 0x50,
	0x0a, 0x04, 0x16, 0x18, 0x32, 0x3c, 0x2e, 0x20,
	0xec, 0xe2, 0xf0, 0xfe, 0xd4, 0xda, 0xc8, 0xc6,
	0x9c, 0x92, 0x80, 0x8e, 0xa4, 0xaa, 0xb8, 0xb6,
	0x0c, 0x02, 0x10, 0x1e, 0x34, 0x3a, 0x28, 0x26,
	0x7c, 0x72, 0x60, 0x6e, 0x44, 0x4a, 0x58, 0x56,
	0x37, 0x39, 0x2b, 0x25, 0x0f, 0x01, 0x13, 0x1d,
	0x47, 0x49, 0x5b, 0x55, 0x7f, 0x71, 0x63, 0x6d,
	0xd7, 0xd9, 0xcb, 0xc5, 0xef, 0xe1, 0xf3, 0xfd,
	0xa7, 0xa9, 0xbb, 0xb5, 0x9f, 0x91, 0x83, 0x8d
};

void p_aes_hooking(struct o_aes *object) {
	/* keep default delete function */
	object->head.s_delegate.m_compare = p_aes_compare;
	object->head.s_delegate.m_hash = p_aes_hash;
	object->head.s_delegate.m_string = p_aes_string;
	/* keep default clone function */
	object->m_crypt = p_aes_crypt;
	object->m_decrypt = p_aes_decrypt;
}

unsigned int *p_aes_key_core(unsigned char *key, unsigned int index,
							 unsigned int matrix) {
	unsigned int *local_current, *local_previous;
	local_current = (unsigned int *)&key[index];
	local_previous = (unsigned int *)&key[index-4];
	*local_current = *local_previous;
	*local_current = (v_aes_sbox[((*local_current)>>8)&0xff]|
					  (v_aes_sbox[((*local_current)>>16)&0xff]<<8)|
					  (v_aes_sbox[((*local_current)>>24)&0xff]<<16)|
					  (v_aes_sbox[(*local_current)&0xff]<<24))^
	v_aes_rcon[matrix];
	return local_current;
}

unsigned int *p_aes_key_column(unsigned int *previous, unsigned char *key,
							   unsigned int index, unsigned int const_s,
							   unsigned int jump) {
	unsigned int *local_current, *local_previous_table;
	local_current = (unsigned int *)&key[index+jump];
	local_previous_table = (unsigned int *)&key[(index+jump)-(4*const_s)];
	*local_current = *local_previous_table^*previous;
	return local_current;
}

unsigned int *p_aes_key_special_column(unsigned int *previous,
									   unsigned char *key, unsigned int index,
									   unsigned int const_s,
									   unsigned int jump) {
	unsigned int *local_current, *local_previous_table;
	local_current = (unsigned int *)&key[index+jump];
	local_previous_table = (unsigned int *)&key[(index+jump)-(4*const_s)];
	*local_current = (v_aes_sbox[(*previous)&0xff]|
					  (v_aes_sbox[((*previous)>>8)&0xff]<<8)|
					  (v_aes_sbox[((*previous)>>16)&0xff]<<16)|
					  (v_aes_sbox[((*previous)>>24)&0xff]<<24))^
	*local_previous_table;
	return local_current;
}

struct o_aes *f_aes_new(struct o_aes *supplied, enum e_aes_block block,
						unsigned char *key, size_t size) {
	struct o_aes *result;
	int const_n, const_b, const_s, jump, index, matrix;
	unsigned int *local_current;
	if ((result = (struct o_aes *)
		 f_object_new(v_aes_kind, sizeof(struct o_aes),
					  (struct o_object *)supplied))) {
		p_aes_hooking(result);
		if (key) {
			result->block = block;
			switch(result->block) {
				case e_aes_block_128:
					const_n = 16;
					const_b = 176;
					break;
				case e_aes_block_192:
					const_n = 24;
					const_b = 208;
					break;
				case e_aes_block_256:
					const_n = 32;
					const_b = 240;
			}
			const_s = const_n/4;
			memcpy(result->expanded_key, key, d_min(size, const_n));
			for (index = const_n, matrix = 1; index < const_b;
				 index+=const_n, matrix++) {
				local_current = p_aes_key_core(result->expanded_key, index,
											   matrix);
				local_current = p_aes_key_column(local_current,
												 result->expanded_key,
												 index, const_s, 0);
				local_current = p_aes_key_column(local_current,
												 result->expanded_key,
												 index, const_s, 4);
				local_current = p_aes_key_column(local_current,
												 result->expanded_key,
												 index, const_s, 8);
				local_current = p_aes_key_column(local_current,
												 result->expanded_key,
												 index, const_s, 12);
				if (result->block >= e_aes_block_192) {
					jump = 16;
					if (result->block == e_aes_block_256) {
						local_current =
						p_aes_key_special_column(local_current,
												 result->expanded_key,
												 index, const_s, jump);
						jump += 4;
					}
					local_current = p_aes_key_column(local_current,
													 result->expanded_key,
													 index, const_s, jump);
					jump += 4;
					local_current = p_aes_key_column(local_current,
													 result->expanded_key,
													 index, const_s, jump);
					jump += 4;
					if (result->block == e_aes_block_256)
						local_current =
						p_aes_key_column(local_current,
										 result->expanded_key, index,
										 const_s, jump);
				}
			}
		} else
			d_throw(v_exception_null,
					"key is undefined or content is a zero-length element");
	}
	return result;
}

int p_aes_compare(struct o_object *object, struct o_object *other) {
	struct o_aes *local_object, *local_other;
	int result = p_object_compare(object, other);
	if ((local_object = d_object_kind(object, aes)) &&
		(local_other = d_object_kind(other, aes)))
		if ((result = memcmp(local_object->expanded_key,
							 local_other->expanded_key,
							 d_aes_expanded_key_size)) == 0)
			result = (local_object->block - local_other->block);
	return result;
}

t_hash_value p_aes_hash(struct o_object *object) {
	struct o_aes *local_object;
	size_t const_n, index;
	t_hash_value result = 0;
	if ((local_object = d_object_kind(object, aes))) {
		if (!object->s_flags.hashed) {
			switch(local_object->block) {
				case e_aes_block_128:
					const_n = 16;
					break;
				case e_aes_block_192:
					const_n = 24;
					break;
				case e_aes_block_256:
					const_n = 32;
			}
			object->hash = 5381;
			/* djb2 hash function */
			for (index = 0; index < const_n; index++)
				object->hash = ((object->hash<<5)+object->hash)+
				local_object->expanded_key[index];
			object->s_flags.hashed = d_true;
		}
		result = object->hash;
	}  else
		d_throw(v_exception_kind, "object is not an instance of o_aes");
	return result;
}

char *p_aes_string(struct o_object *object, char *data, size_t size) {
	struct o_aes *local_object;
	size_t written, local_written, local_size;
	int index, const_n, const_b;
	if ((local_object = d_object_kind(object, aes))) {
		switch(local_object->block) {
			case e_aes_block_128:
				const_n = 16;
				const_b = 128;
				break;
			case e_aes_block_192:
				const_n = 24;
				const_b = 192;
				break;
			case e_aes_block_256:
				const_n = 32;
				const_b = 256;
		}
		written = snprintf(data, size, "<aes block size: %d key: ", const_b);
		written = ((written>size)?size:written);
		local_size = size-written;
		data += written;
		for (index = 0; (index < const_n) && ((written+1) < size); index++,
			 written += local_written, data += local_written,
			 local_size -= local_written) {
			local_written = snprintf(data, local_size, "%02x",
									 local_object->expanded_key[index]);
			local_written = ((local_written>local_size)?local_size:
							 local_written);
		}
		if (written < size) {
			*data = '>';
			data++;
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_aes");
	return data;
}

struct o_string *p_aes_padding(struct o_string *string, int local,
							   size_t const_n) {
	size_t padder, size = string->size;
	struct o_string *result = string;
	padder = (size%const_n);
	size += (const_n-padder);
	if (local) {
		if (padder != 0) {
			if ((result->content = (char *) d_realloc(result->content, size))) {
				memset((result->content+result->size), '\0',
					   (size-result->size));
				result->size = size;
			} else
				d_die(d_error_malloc);
		}
	} else
		result = f_string_new(NULL, size, result->content);
	return result;
}

unsigned int p_aes_mix_column(unsigned int box) {
	unsigned int keys[4], galois[4];
	keys[0] = box&0xff;
	keys[1] = (box>>8)&0xff;
	keys[2] = (box>>16)&0xff;
	keys[3] = (box>>24)&0xff;
	galois[0] = (v_aes_mul_2[keys[0]])^(v_aes_mul_3[keys[1]])^
	(keys[2])^(keys[3]);
	galois[1] = (v_aes_mul_2[keys[1]])^(v_aes_mul_3[keys[2]])^
	(keys[3])^(keys[0]);
	galois[2] = (v_aes_mul_2[keys[2]])^(v_aes_mul_3[keys[3]])^
	(keys[0])^(keys[1]);
	galois[3] = (v_aes_mul_2[keys[3]])^(v_aes_mul_3[keys[0]])^
	(keys[1])^(keys[2]);
	return (galois[0]|(galois[1]<<8)|(galois[2]<<16)|(galois[3]<<24));
}

unsigned int p_aes_mix_column_inverse(unsigned int box) {
	unsigned int keys[4], galois[4];
	keys[0] = box&0xff;
	keys[1] = (box>>8)&0xff;
	keys[2] = (box>>16)&0xff;
	keys[3] = (box>>24)&0xff;
	galois[0] = (v_aes_mul_14[keys[0]])^ (v_aes_mul_11[keys[1]])^
	(v_aes_mul_13[keys[2]])^(v_aes_mul_9[keys[3]]);
	galois[1] = (v_aes_mul_14[keys[1]])^ (v_aes_mul_11[keys[2]])^
	(v_aes_mul_13[keys[3]])^(v_aes_mul_9[keys[0]]);
	galois[2] = (v_aes_mul_14[keys[2]])^ (v_aes_mul_11[keys[3]])^
	(v_aes_mul_13[keys[0]])^(v_aes_mul_9[keys[1]]);
	galois[3] = (v_aes_mul_14[keys[3]])^ (v_aes_mul_11[keys[0]])^
	(v_aes_mul_13[keys[1]])^(v_aes_mul_9[keys[2]]);
	return (galois[0]|(galois[1]<<8)|(galois[2]<<16)|(galois[3]<<24));
}

struct o_string *p_aes_crypt(struct o_aes *object,
							 struct o_string *string, int local) {
	size_t index;
	int const_n, const_a, executions;
	struct o_string *result;
	unsigned int *pointer[4], box[4];
	switch (object->block) {
		case e_aes_block_128:
			const_n = 16;
			const_a = 10;
			break;
		case e_aes_block_192:
			const_n = 24;
			const_a = 12;
			break;
		case e_aes_block_256:
			const_n = 32;
			const_a = 14;
	}
	result = p_aes_padding(string, local, 16);
	for (index = 0; index < result->size; index+=16) {
		pointer[0] = (unsigned int *)&result->content[index];
		pointer[1] = (unsigned int *)&result->content[index+4];
		pointer[2] = (unsigned int *)&result->content[index+8];
		pointer[3] = (unsigned int *)&result->content[index+12];
		*pointer[0] ^= *((unsigned int *)&object->expanded_key[0]);
		*pointer[1] ^= *((unsigned int *)&object->expanded_key[4]);
		*pointer[2] ^= *((unsigned int *)&object->expanded_key[8]);
		*pointer[3] ^= *((unsigned int *)&object->expanded_key[12]);
		for (executions = 0; executions < const_a; executions++) {
			box[0] = v_aes_sbox[*pointer[0]&0xff]|
			(v_aes_sbox[(*pointer[1]>>8)&0xff]<<8)|
			(v_aes_sbox[(*pointer[2]>>16)&0xff]<<16)|
			(v_aes_sbox[(*pointer[3]>>24)&0xff]<<24);
			box[1] = v_aes_sbox[*pointer[1]&0xff]|
			(v_aes_sbox[(*pointer[2]>>8)&0xff]<<8)|
			(v_aes_sbox[(*pointer[3]>>16)&0xff]<<16)|
			(v_aes_sbox[(*pointer[0]>>24)&0xff]<<24);
			box[2] = v_aes_sbox[*pointer[2]&0xff]|
			(v_aes_sbox[(*pointer[3]>>8)&0xff]<<8)|
			(v_aes_sbox[(*pointer[0]>>16)&0xff]<<16)|
			(v_aes_sbox[(*pointer[1]>>24)&0xff]<<24);
			box[3] = v_aes_sbox[*pointer[3]&0xff]|
			(v_aes_sbox[(*pointer[0]>>8)&0xff]<<8)|
			(v_aes_sbox[(*pointer[1]>>16)&0xff]<<16)|
			(v_aes_sbox[(*pointer[2]>>24)&0xff]<<24);
			if ((executions < (const_a-1))) {
				box[0] = p_aes_mix_column(box[0]);
				box[1] = p_aes_mix_column(box[1]);
				box[2] = p_aes_mix_column(box[2]);
				box[3] = p_aes_mix_column(box[3]);
			}
			*pointer[0] = box[0]^
			*((unsigned int*)&object->expanded_key[((executions+1)*16)]);
			*pointer[1] = box[1]^
			*((unsigned int*)&object->expanded_key[((executions+1)*16)+4]);
			*pointer[2] = box[2]^
			*((unsigned int*)&object->expanded_key[((executions+1)*16)+8]);
			*pointer[3] = box[3]^
			*((unsigned int*)&object->expanded_key[((executions+1)*16)+12]);
		}
	}
	return result;
}

struct o_string *p_aes_decrypt(struct o_aes *object,
							   struct o_string *string, int local){
	size_t index;
	int const_n, const_a, executions;
	struct o_string *result;
	unsigned int *pointer[4], box[4];
	switch (object->block) {
		case e_aes_block_128:
			const_n = 16;
			const_a = 10;
			break;
		case e_aes_block_192:
			const_n = 24;
			const_a = 12;
			break;
		case e_aes_block_256:
			const_n = 32;
			const_a = 14;
	}
	result = p_aes_padding(string, local, 16);
	for (index = 0; index < result->size; index+=16) {
		pointer[0] = (unsigned int *)&result->content[index];
		pointer[1] = (unsigned int *)&result->content[index+4];
		pointer[2] = (unsigned int *)&result->content[index+8];
		pointer[3] = (unsigned int *)&result->content[index+12];
		*pointer[0] ^= *((unsigned int *)&object->expanded_key[(const_a*16)]);
		*pointer[1] ^= *((unsigned int *)&object->expanded_key[(const_a*16)+4]);
		*pointer[2] ^= *((unsigned int *)&object->expanded_key[(const_a*16)+8]);
		*pointer[3] ^=
		*((unsigned int *)&object->expanded_key[(const_a*16)+12]);
		for (executions = (const_a-1); executions >= 0; executions--) {
			box[0] = v_aes_sbox_inv[*pointer[0]&0xff]|
			(v_aes_sbox_inv[(*pointer[3]>>8)&0xff]<<8)|
			(v_aes_sbox_inv[(*pointer[2]>>16)&0xff]<<16)|
			(v_aes_sbox_inv[(*pointer[1]>>24)&0xff]<<24);
			box[1] = v_aes_sbox_inv[*pointer[1]&0xff]|
			(v_aes_sbox_inv[(*pointer[0]>>8)&0xff]<<8)|
			(v_aes_sbox_inv[(*pointer[3]>>16)&0xff]<<16)|
			(v_aes_sbox_inv[(*pointer[2]>>24)&0xff]<<24);
			box[2] = v_aes_sbox_inv[*pointer[2]&0xff]|
			(v_aes_sbox_inv[(*pointer[1]>>8)&0xff]<<8)|
			(v_aes_sbox_inv[(*pointer[0]>>16)&0xff]<<16)|
			(v_aes_sbox_inv[(*pointer[3]>>24)&0xff]<<24);
			box[3] = v_aes_sbox_inv[*pointer[3]&0xff]|
			(v_aes_sbox_inv[(*pointer[2]>>8)&0xff]<<8)|
			(v_aes_sbox_inv[(*pointer[1]>>16)&0xff]<<16)|
			(v_aes_sbox_inv[(*pointer[0]>>24)&0xff]<<24);
			box[0] ^=
			*((unsigned int*)&object->expanded_key[(executions*16)]);
			box[1] ^=
			*((unsigned int*)&object->expanded_key[(executions*16)+4]);
			box[2] ^=
			*((unsigned int*)&object->expanded_key[(executions*16)+8]);
			box[3] ^=
			*((unsigned int*)&object->expanded_key[(executions*16)+12]);
			if (executions > 0) {
				box[0] = p_aes_mix_column_inverse(box[0]);
				box[1] = p_aes_mix_column_inverse(box[1]);
				box[2] = p_aes_mix_column_inverse(box[2]);
				box[3] = p_aes_mix_column_inverse(box[3]);
			}
			*pointer[0] = box[0];
			*pointer[1] = box[1];
			*pointer[2] = box[2];
			*pointer[3] = box[3];
		}
	}
	return result;
}
