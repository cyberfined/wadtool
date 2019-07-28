#ifndef WAD_H
#define WAD_H

#include <stdio.h>
#include <stdlib.h>
#include "lump.h"

typedef struct {
	int lumps_count;
	int offset;
}Catalog;

typedef struct {
	Catalog catalog;
	Lump *lumps;
}Wad;

void wad_calc_offsets(Wad *wad);
void wad_add_file(Wad *wad, char *filename);
void wad_del_file(Wad *wad, char *name);

void wad_save(Wad *wad, char *filename);
static void wad_write_header(Wad *wad, FILE *file);
static void wad_write_lumps(Wad *wad, FILE *file);
static void wad_write_catalog(Wad *wad, FILE *file);

Wad* wad_open(char *filename);
static int wad_read_header(Wad *wad, FILE *file);
static int wad_read_lumps_info(Wad *wad, FILE *file);
static int wad_read_lumps_data(Wad *wad, FILE *file);

void wad_extract_lump(Wad *wad, char *lump_name, char *path);
Lump* wad_get_lump(Wad *wad, char *lump_name);

void wad_rename_lump(Wad *wad, char *name, char *n_name);
void wad_up_lump(Wad *wad, char *lump_name);
void wad_down_lump(Wad *wad, char *lump_name);
void wad_free(Wad *wad);

static char* file_to_buf(char *filename, int *size);
static char* name_valid(char *filename);

#endif
