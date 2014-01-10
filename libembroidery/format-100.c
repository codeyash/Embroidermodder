#include "format-100.h"
#include "emb-logging.h"
#include <stdio.h>

int read100(EmbPattern* pattern, const char* fileName)
{
    FILE* file = 0;
    int x,y;
    int stitchType;
    unsigned char b[4];

    if(!pattern) { embLog_error("format-100.c read100(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-100.c read100(), fileName argument is null\n"); return 0; }

    file = fopen(fileName,"rb");
    if(!file)
    {
        embLog_error("format-100.c read100(), cannot open %s for reading\n", fileName);
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);
    while(fread(b, 1, 4, file) == 4)
    {
        stitchType = NORMAL;
        x = (b[2] > 0x80) ? -(b[2] - 0x80) : b[2];
        y = (b[3] > 0x80) ? -(b[3] - 0x80) : b[3];
        /*if(!(b[0] & 0xFC)) stitchType = JUMP; TODO: review & fix */
        if(!(b[0] & 0x01)) stitchType = STOP;
        if(b[0] == 0x1F) stitchType = END;
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitchType, 1);
    }
    fclose(file);
    return 1;
}

int write100(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-100.c write100(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-100.c write100(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish write100 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
