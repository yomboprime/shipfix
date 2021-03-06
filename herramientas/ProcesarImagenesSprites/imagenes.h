/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef IMAGENES_H
#define IMAGENES_H

#include "modoRadastan.h"

#define NUM_PALETAS 2

static unsigned char paletas [] = {

    // Pixels en formato gggrrrbb

    0x0,
    0xc3,
    0xfa,
    0x6f,
    0x6d,
    0x86,
    0xd0,
    0xfc,
    0xff,
    0xdb,
    0x92,
    0x1c,
    0xc0,
    0x23,
    0xab,
    0x9c,
    0x0,
    0xc3,
    0xa2,
    0xab,
    0x6b,
    0xff,
    0xdf,
    0xb6,
    0x9c,
    0x92,
    0xdb,
    0x1c,
    0xfc,
    0xdc,
    0x80,
    0x42
};



static unsigned char pixelsImagen000[] = {
    0x08, 
    0xe0, 
    0x09, 
    0xee, 
    0x88, 
    0x9e, 
    0x98, 
    0x99, 
    0x98, 
    0x88, 
    0x9a, 
    0x99, 
    0x00, 
    0x80, 
    0x00, 
    0x80, 
    0x00, 
    0x88
};

static unsigned char pixelsImagen001[] = {
    0x0e, 
    0x80, 
    0xee, 
    0x90, 
    0xe9, 
    0x88, 
    0x99, 
    0x89, 
    0x88, 
    0x89, 
    0x99, 
    0xa9, 
    0x08, 
    0x00, 
    0x08, 
    0x00, 
    0x88, 
    0x00
};

static unsigned char pixelsImagen002[] = {
    0x08, 
    0x88, 
    0x88, 
    0x00, 
    0x88, 
    0x99, 
    0x88, 
    0x88, 
    0x89, 
    0x9e, 
    0xd8, 
    0xb8, 
    0x89, 
    0x9e, 
    0xdc, 
    0x88, 
    0xa8, 
    0x9e, 
    0xd8, 
    0x8a, 
    0xaa, 
    0x9d, 
    0xd8, 
    0xaa, 
    0x0a, 
    0xaa, 
    0xaa, 
    0xa0
};

static unsigned char pixelsImagen003[] = {
    0x08, 
    0x90, 
    0x98, 
    0xf9, 
    0xa8, 
    0xda, 
    0x0a, 
    0xa0
};

static unsigned char pixelsImagen004[] = {
    0x08, 
    0xc0, 
    0x08, 
    0x00, 
    0x08, 
    0x90, 
    0x98, 
    0x89
};

static unsigned char pixelsImagen005[] = {
    0x00, 
    0x80, 
    0x00, 
    0x08, 
    0x09, 
    0x90, 
    0xaa, 
    0xaa
};

static unsigned char pixelsImagen006[] = {
    0x98
};

static unsigned char pixelsImagen007[] = {
    0x7b, 
    0x07, 
    0x6b, 
    0xc6, 
    0x77, 
    0x66, 
    0x07, 
    0x60
};

static unsigned char pixelsImagen008[] = {
    0x0b, 
    0x68, 
    0xa9, 
    0x99, 
    0x0a, 
    0xa0
};

static unsigned char pixelsImagen009[] = {
    0x90, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x06, 
    0x29, 
    0x96, 
    0x62, 
    0x60, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x02, 
    0x69, 
    0x26, 
    0x60, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x09, 
    0x09, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x06, 
    0x66, 
    0x29, 
    0x96, 
    0x62, 
    0x69, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x62, 
    0x69, 
    0x26, 
    0x66, 
    0x60, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x90, 
    0x00, 
    0x90, 
    0x00, 
    0x00, 
    0x00, 
    0x66, 
    0x66, 
    0x29, 
    0x96, 
    0x62, 
    0x69, 
    0x90, 
    0x00, 
    0x00, 
    0x83, 
    0xdd, 
    0x33, 
    0x3d, 
    0xdd, 
    0x38, 
    0x00, 
    0x09, 
    0x62, 
    0x69, 
    0x26, 
    0x66, 
    0x66, 
    0x00, 
    0x00, 
    0x00, 
    0x09, 
    0x90, 
    0x00, 
    0x99, 
    0x00, 
    0x00, 
    0x00, 
    0x66, 
    0x62, 
    0x29, 
    0x96, 
    0x62, 
    0x69, 
    0x99, 
    0x00, 
    0xd3, 
    0x83, 
    0x33, 
    0x33, 
    0xdd, 
    0xdd, 
    0x38, 
    0x30, 
    0x99, 
    0x62, 
    0x69, 
    0x26, 
    0x66, 
    0x66, 
    0x00, 
    0x00, 
    0x00, 
    0x99, 
    0x00, 
    0x00, 
    0x09, 
    0x90, 
    0x00, 
    0x06, 
    0x66, 
    0x62, 
    0x29, 
    0x66, 
    0x22, 
    0x69, 
    0x99, 
    0x9d, 
    0xd3, 
    0x83, 
    0x33, 
    0x33, 
    0xdd, 
    0xdd, 
    0x38, 
    0x3d, 
    0xd9, 
    0x62, 
    0x69, 
    0x92, 
    0x66, 
    0x66, 
    0x60, 
    0x00, 
    0x09, 
    0x90, 
    0x00, 
    0x00, 
    0x00, 
    0x99, 
    0x00, 
    0x06, 
    0x66, 
    0x62, 
    0x29, 
    0x66, 
    0x26, 
    0x99, 
    0x99, 
    0xdd, 
    0xd3, 
    0x83, 
    0x33, 
    0x33, 
    0xdd, 
    0xd3, 
    0x83, 
    0xdd, 
    0xdd, 
    0x62, 
    0x26, 
    0x92, 
    0x66, 
    0x66, 
    0x60, 
    0x00, 
    0x99, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x09, 
    0x90, 
    0xe6, 
    0x62, 
    0x29, 
    0x96, 
    0x62, 
    0x26, 
    0x99, 
    0x9e, 
    0xdd, 
    0xdd, 
    0x38, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x33, 
    0x83, 
    0xdd, 
    0xdd, 
    0x62, 
    0x26, 
    0x62, 
    0x66, 
    0x66, 
    0x6e, 
    0xe5, 
    0x59, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x09, 
    0x55, 
    0xee, 
    0x29, 
    0x99, 
    0x96, 
    0x62, 
    0x66, 
    0x99, 
    0xee, 
    0xdd, 
    0xdd, 
    0x38, 
    0xdd, 
    0xd3, 
    0x33, 
    0x38, 
    0xdd, 
    0xdd, 
    0xdd, 
    0xe9, 
    0x22, 
    0x66, 
    0x22, 
    0x22, 
    0xee, 
    0xee, 
    0x55, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x55, 
    0x5e, 
    0xee, 
    0xe9, 
    0x99, 
    0x96, 
    0x26, 
    0x69, 
    0x99, 
    0xee, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x88, 
    0x88, 
    0x8d, 
    0xdd, 
    0xdd, 
    0xde, 
    0xe9, 
    0x92, 
    0x26, 
    0x99, 
    0x9e, 
    0xee, 
    0xee, 
    0xe5, 
    0x50, 
    0x00, 
    0x00, 
    0x05, 
    0x5e, 
    0xee, 
    0xee, 
    0xe9, 
    0x99, 
    0x66, 
    0x29, 
    0x99, 
    0x9e, 
    0xee, 
    0xed, 
    0xdd, 
    0xd8, 
    0x33, 
    0x3d, 
    0xdd, 
    0xd8, 
    0xdd, 
    0xde, 
    0xee, 
    0xee, 
    0x99, 
    0x22, 
    0x26, 
    0xee, 
    0xee, 
    0xee, 
    0xe5, 
    0x55, 
    0x00, 
    0x00, 
    0x55, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0xe9, 
    0x96, 
    0x66, 
    0x9e, 
    0xee, 
    0xee, 
    0xee, 
    0x88, 
    0x8d, 
    0x33, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x88, 
    0x88, 
    0x8e, 
    0xe6, 
    0x66, 
    0x99, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0xe5, 
    0x50, 
    0x05, 
    0x5e, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0x99, 
    0x66, 
    0x68, 
    0x88, 
    0x88, 
    0x88, 
    0xaa, 
    0xaa, 
    0xdd, 
    0x33, 
    0x33, 
    0xda, 
    0xaa, 
    0xa0, 
    0xaa, 
    0xa8, 
    0x88, 
    0x86, 
    0x9e, 
    0xee, 
    0xee, 
    0xee, 
    0x77, 
    0xe5, 
    0xea, 
    0x5e, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0xe8, 
    0x88, 
    0x8a, 
    0xaa, 
    0xaa, 
    0xa0, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa0, 
    0xaa, 
    0xaa, 
    0xa0, 
    0xa8, 
    0x8e, 
    0xee, 
    0xee, 
    0xe7, 
    0x77, 
    0xee, 
    0xea, 
    0xee, 
    0x77, 
    0xee, 
    0xee, 
    0xee, 
    0xee, 
    0x88, 
    0x8a, 
    0xaa, 
    0x0a, 
    0x9a, 
    0xa9, 
    0x90, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x0a, 
    0x99, 
    0xaa, 
    0xa0, 
    0xaa, 
    0xa8, 
    0x88, 
    0xee, 
    0xee, 
    0x7e, 
    0xe9, 
    0x9a, 
    0x9e, 
    0x77, 
    0x7e, 
    0xee, 
    0xee, 
    0x88, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x09, 
    0x09, 
    0x90, 
    0x09, 
    0x0a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x09, 
    0x00, 
    0x9a, 
    0x0a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x89, 
    0x99, 
    0x99, 
    0x9a, 
    0x9a, 
    0x9e, 
    0xe7, 
    0xee, 
    0x99, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x0a, 
    0x90, 
    0x09, 
    0x9a, 
    0x0a, 
    0x44, 
    0x44, 
    0x44, 
    0x44, 
    0x44, 
    0xa0, 
    0xaa, 
    0x99, 
    0x09, 
    0x0a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x99, 
    0x99, 
    0x9a, 
    0x9a, 
    0x99, 
    0xa9, 
    0xa9, 
    0x99, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa0, 
    0xa9, 
    0x9a, 
    0x44, 
    0xa0, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x04, 
    0x4a, 
    0xaa, 
    0x90, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa9, 
    0x99, 
    0xa9, 
    0xa0, 
    0x09, 
    0xa9, 
    0xa9, 
    0x9a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x04, 
    0x40, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x04, 
    0x0a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa9, 
    0x99, 
    0xa9, 
    0x00, 
    0x00, 
    0x9a, 
    0x9a, 
    0x9a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa4, 
    0x40, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x4a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x99, 
    0x9a, 
    0x9a, 
    0x00, 
    0x00, 
    0x0a, 
    0x9a, 
    0x99, 
    0x9a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa4, 
    0x44, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x44, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa9, 
    0x99, 
    0x9a, 
    0xa9, 
    0xa0, 
    0x00, 
    0x00, 
    0x00, 
    0xa9, 
    0xa9, 
    0x99, 
    0x94, 
    0x44, 
    0x44, 
    0x44, 
    0x40, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x04, 
    0x44, 
    0x44, 
    0xa9, 
    0x99, 
    0x99, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0xae, 
    0xe0, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x0e, 
    0xea, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x09, 
    0xa0, 
    0x0e, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0xe0, 
    0x0a, 
    0x90, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x9e, 
    0xa0, 
    0x0e, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0xe0, 
    0x0a, 
    0xe9, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x09, 
    0xe0, 
    0xae, 
    0xe0, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x0e, 
    0xea, 
    0x0e, 
    0x90, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x9e, 
    0xa0, 
    0xa9, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x9a, 
    0x0a, 
    0xe9, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x0a, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xaa, 
    0xa0, 
    0x00, 
    0x00, 
    0x00, 
    0x00
};

static unsigned char pixelsImagen010[] = {
    0x00, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0xdd, 
    0xdd, 
    0x80, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x11, 
    0x11, 
    0x11, 
    0x11, 
    0x10, 
    0x00, 
    0x00, 
    0x13, 
    0x33, 
    0x33, 
    0x33, 
    0x10, 
    0x00, 
    0x00, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x30, 
    0x00, 
    0x03, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x00, 
    0x03, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x00, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x30
};

static unsigned char pixelsImagen011[] = {
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0x8d, 
    0xdd, 
    0xd8, 
    0x80, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x80, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0x88, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0xdd, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0xdd, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0x5d, 
    0xdd, 
    0xd8, 
    0x00, 
    0x00, 
    0x00, 
    0x05, 
    0xdd, 
    0xdd, 
    0x80, 
    0x00, 
    0x00, 
    0x00, 
    0x11, 
    0xdd, 
    0xdd, 
    0x11, 
    0x10, 
    0x00, 
    0x00, 
    0x13, 
    0x33, 
    0x33, 
    0x33, 
    0x10, 
    0x00, 
    0x03, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x00, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x30
};

#define NUM_IMAGENES 12


static Imagen imagenes[] = {

    // tamx2, tamy, imagen

    {
        2, 9, pixelsImagen000
    },
    {
        2, 9, pixelsImagen001
    },
    {
        4, 7, pixelsImagen002
    },
    {
        2, 4, pixelsImagen003
    },
    {
        2, 4, pixelsImagen004
    },
    {
        2, 4, pixelsImagen005
    },
    {
        1, 1, pixelsImagen006
    },
    {
        2, 4, pixelsImagen007
    },
    {
        2, 3, pixelsImagen008
    },
    {
        32, 27, pixelsImagen009
    },
    {
        7, 20, pixelsImagen010
    },
    {
        7, 20, pixelsImagen011
    }
};


#endif // IMAGENES_H


