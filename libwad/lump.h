#ifndef LUMP_H
#define LUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lump {
	int offset;
	int size;
	char *name;
	unsigned char *data;
	struct lump *next;
}Lump;

void lump_add(Lump **lumps, Lump *lump);
short lump_del(Lump **lumps, char *name);
Lump* lump_new(int offset, int size, char *name, char *data);

void lump_free(Lump *lump);
void lumps_free(Lump *lumps);

Lump* lump_get_by_name(Lump *lumps, char *name);

static void lumps_swap(Lump *l1, Lump *l2);
void lump_up(Lump *lumps, char *name);
void lump_down(Lump *lumps, char *name);

#endif
