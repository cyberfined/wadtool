#include "wad.h"
#define INT_SIZE 4 //sizeof(int) depends from arch
#define NAME_SIZE 8
#define HEADER_SIZE 8 //header size: 4 bytes - catalog offset, 4 bytes - lumps count

void wad_add_file(Wad *wad, char *filename) {
	char *filebuf;
	char *lump_name;
	int size;
	Lump *lump;

	filebuf = file_to_buf(filename, &size);
	if(!filebuf) return;

	lump_name = name_valid(filename);
	if(wad_get_lump(wad, lump_name)) {
		free(lump_name);
		free(filebuf);
		return;
	}
	lump = lump_new(0, size, lump_name, filebuf);
	lump_add(&wad->lumps, lump);
	wad->catalog.lumps_count++;
}

void wad_del_file(Wad *wad, char *name) {
	if(lump_del(&wad->lumps, name)) {
		wad->catalog.lumps_count--;
	}
}

void wad_calc_offsets(Wad *wad) {
	Lump *i;
	int l_offset = HEADER_SIZE; /*lump offset*/
	wad->catalog.offset = HEADER_SIZE;

	for(i = wad->lumps; i != NULL; i = i->next) {
		wad->catalog.offset += i->size;
		i->offset = l_offset;
		l_offset += i->size;
	}
}

void wad_save(Wad *wad, char *filename) {
	FILE *file = fopen(filename, "wb");

	wad_calc_offsets(wad);
	wad_write_header(wad, file);	
	wad_write_lumps(wad, file);
	wad_write_catalog(wad, file);

	fclose(file);
}

static void wad_write_header(Wad *wad, FILE *file) {
	fwrite(&wad->catalog.offset, INT_SIZE, 1, file);
	fwrite(&wad->catalog.lumps_count, INT_SIZE, 1, file);
}

static void wad_write_lumps(Wad *wad, FILE *file) {
	Lump *i;
	for(i = wad->lumps; i != NULL; i = i->next)
		fwrite(i->data, sizeof(char), i->size, file);
}

static void wad_write_catalog(Wad *wad, FILE *file) {
	Lump *i;
	for(i = wad->lumps; i != NULL; i = i->next) {
		fwrite(i->name, sizeof(char), NAME_SIZE, file);	
		fwrite(&i->offset, INT_SIZE, 1, file);
		fwrite(&i->size, INT_SIZE, 1, file);
	}
}

static int wad_read_header(Wad *wad, FILE *file) {
	fread(&wad->catalog.offset, INT_SIZE, 1, file);     /* catalog offset */
	fread(&wad->catalog.lumps_count, INT_SIZE, 1, file);/* lumps count */
	return feof(file);
}

static int wad_read_lumps_info(Wad *wad, FILE *file) {
	Lump *lump;
	int i;

	fseek(file, wad->catalog.offset, SEEK_SET);

	for(i = 0; i < wad->catalog.lumps_count; i++) {
		if(feof(file)) return 1;
		lump = (Lump*)calloc(1, sizeof(Lump));

		lump->name = (char*)malloc(NAME_SIZE);
		fread(lump->name, sizeof(char), NAME_SIZE, file); /* name */

		fread(&lump->offset, INT_SIZE, 1, file); /* offset */

		fread(&lump->size, INT_SIZE, 1, file); /* size */

		lump_add(&wad->lumps, lump);
	}
	return 0;
}

static int wad_read_lumps_data(Wad *wad, FILE *file) {
	Lump *i;
	for(i = wad->lumps; i != NULL; i = i->next) {
		if(feof(file)) return 1;
		fseek(file, i->offset, SEEK_SET);
		i->data = (char*)malloc(i->size);
		fread(i->data, sizeof(char), i->size, file);
	}
	return 0;
}

Wad* wad_open(char *filename) {
	FILE *file = fopen(filename, "rb");
	Wad *wad;
	if(!file) return NULL;
	wad = (Wad*)calloc(1, sizeof(Wad));
	if(wad_read_header(wad, file)     ||
	   wad_read_lumps_info(wad, file) ||
	   wad_read_lumps_data(wad, file)) //if feof
	{
		wad_free(wad);
		wad = NULL;
	}

	fclose(file);
	return wad;
}

Lump* wad_get_lump(Wad *wad, char *lump_name) {
	return lump_get_by_name(wad->lumps, lump_name);
}

void wad_extract_lump(Wad *wad, char *lump_name, char *path) {
	FILE *file;
	Lump *lump;

	lump = wad_get_lump(wad, lump_name);
	if(!lump) return;
	file = fopen(path, "wb");
	if(!file) return;
	fwrite(lump->data, sizeof(char), lump->size, file);
	fclose(file);
}

void wad_rename_lump(Wad *wad, char *name, char *n_name) {
	Lump *lump = wad_get_lump(wad, name);
	if(!lump) return;
	free(lump->name);
	lump->name = name_valid(n_name);
}

void wad_up_lump(Wad *wad, char *lump_name) {
	lump_up(wad->lumps, lump_name);
}

void wad_down_lump(Wad *wad, char *lump_name) {
	lump_down(wad->lumps, lump_name);
}

void wad_free(Wad *wad) {
	lumps_free(wad->lumps);
	free(wad);
}

static char* file_to_buf(char *filename, int *size) {
	FILE *file = fopen(filename, "rb");
	char *buf;
	if(!file)
		return NULL;
	fseek(file, 0, SEEK_END);
	*size = ftell(file);
	buf = (char*)malloc(*size + 1);
	fseek(file, 0, SEEK_SET);
	fread(buf, sizeof(char), *size, file);
	buf[*size] = '\0';
	fclose(file);
	return buf;
}

static char* name_valid(char *filename) {
	char *n_name = (char*)malloc(NAME_SIZE);
	int index;
	int i;

	index = strrchr(filename, '/') - filename + 1; /*Get filename without directories for unix*/
	index = index > 0 ? index : strrchr(filename, '\\') - filename + 1; /*If not, for Windows*/
	if(index > 0) filename += index;

	index = strrchr(filename, '.') - filename; /*Remove extension*/
	index = index > 0 && index <= NAME_SIZE ? index : NAME_SIZE;

	strncpy(n_name, filename, index);
	for(i = index; i < NAME_SIZE; i++) n_name[i] = '\0';

	return n_name;
}
