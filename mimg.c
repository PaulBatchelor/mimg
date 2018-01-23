#include <stdio.h>
#include <runt.h>
#include <monomer/util.h>
#include <monomer/kilo.h>
#include <monomer/gfx.h>
#include "img.h"


typedef struct {
    unsigned char r, g, b, a;
} pixel;

static pixel *data = NULL;

static void draw(monomer_gfxblock *state, unsigned char *buf)
{
    int x;
    int y;
    int pos;
    int height;
    monomer_pixel *out;
    int color;
    unsigned int rgbpos;
    int w, h; 
    int ix, iy; 
    int offx, offy; 
    
    monomer_gfxblock_get(state, &w, &h, &ix, &iy, &offx, &offy);
    
    height = (h - offy) << 3;
    h <<= 3;
    w <<= 3;

    out = (monomer_pixel *)buf;

    for(y = 0; y < height; y++) {
        for(x = 0; x < w; x++) {
            pos = ((h - 1) - y ) * 320 + x;
            rgbpos = y*128+x;
            out[pos].r = data[rgbpos].r;
            out[pos].g = data[rgbpos].g;
            out[pos].b = data[rgbpos].b;
        }
    }

}

static runt_int rproc_mimg(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *s;
    runt_int pos;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    pos = s->f;

    monomer_gfxblk_set(pos, draw);
    monomer_gfxblk_size(pos, 16, 16);
    /* monomer_gfxblk_key(pos, key); */
}
void pt(unsigned int x, unsigned int y)
{
    pixel *clr;

    if(x >= 128 || y >= 128) return;
    
    clr = (pixel *) img_get_current_color(); 
    data[y*128 + x] = *clr;
    monomer_gfx_draw();
}

static runt_int rproc_loadimg(runt_vm *vm, runt_ptr p)
{
    runt_load_img(vm);
    runt_mark_set(vm);
    kilo_set_statusmsg("Loaded img dictionary.");
    img_set_point_function(pt);
    img_setsize(128, 128);
    data = (pixel *)img_get_data();
    return RUNT_OK;
}


int rplug_mimg(runt_vm *vm)
{
    runt_int rc;
    rc = monomer_init_plugin(vm);
    RUNT_ERROR_CHECK(rc);
    runt_keyword_define(vm, "mimg", 4, rproc_mimg, NULL);
    runt_keyword_define(vm, "loadimg", 7, rproc_loadimg, NULL); 
    kilo_set_statusmsg("Successfully loaded mimg.");
    return RUNT_OK;
}
