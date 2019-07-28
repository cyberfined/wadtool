#include "lump.h"

void lump_add(Lump **lumps, Lump *lump) {
	for(; *lumps != NULL; lumps = &(*lumps)->next);
	*lumps = lump;
}

short lump_del(Lump **lumps, char *name) {
	short status = 0;
	for(; *lumps != NULL; lumps = &(*lumps)->next) {
		if(!strcmp((*lumps)->name, name)) {
			status = 1;
			break;
		}
	}
	if(status) {
		lump_free(*lumps);
		*lumps = (*lumps)->next;
	}
	return status; 
}

Lump* lump_new(int offset, int size, char *name, char *data) {
	Lump *l = (Lump*)malloc(sizeof(Lump));
	l->offset = offset;
	l->size = size;
	l->name = name;
	l->data = data;
	l->next = NULL;
	return l;
}

void lump_free(Lump *lump) {
	if(lump->name != NULL) free(lump->name);
	if(lump->data != NULL) free(lump->data);
	free(lump);
}

void lumps_free(Lump *lumps) {
	for(; lumps != NULL; lumps = lumps->next)
		lump_free(lumps);
}

Lump* lump_get_by_name(Lump *lumps, char *name) {
	for(; lumps != NULL; lumps = lumps->next) {
		if(!strcmp(lumps->name, name)) return lumps;
	}
	return NULL;
}

static void lumps_swap(Lump *l1, Lump *l2) {
	Lump tmp = *l1;
	l1->name = l2->name;
	l1->size = l2->size;
	l1->data = l2->data;
	l2->name = tmp.name;
	l2->size = tmp.size;
	l2->data = tmp.data;
}

void lump_up(Lump *lumps, char *name) {
	Lump *i;
	for(i = lumps; i->next != NULL; i = i->next) {
		if(!strcmp(i->next->name, name)) {
			lumps_swap(i, i->next);
			break;
		}
	}
}

void lump_down(Lump *lumps, char *name) {
	Lump *i;
	for(i = lumps; i->next != NULL; i = i->next) {
		if(!strcmp(i->name, name)) {
			lumps_swap(i, i->next);
			break;
		}
	}
}
