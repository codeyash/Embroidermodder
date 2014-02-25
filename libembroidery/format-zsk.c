#include "format-zsk.h"
#include "emb-file.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readZsk(EmbPattern* pattern, const char* fileName)
{
    int b[3];
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-zsk.c readZsk(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-zsk.c readZsk(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-zsk.c readZsk(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 512, SEEK_SET);

    while(embFile_read(b, 1, 3, file) == 3)
    {
        if((b[2] & 25) == 0)
        {
            if(b[2] & 0x40)
                b[1] = -b[1];
            if(b[2] & 0x20)
                b[0] = -b[0];
            embPattern_addStitchRel(pattern, b[1] / 10.0, b[0] / 10.0, NORMAL, 1);
        }
        else
        {
            embPattern_addStitchRel(pattern, b[1] / 10.0, b[0] / 10.0, STOP, 1);
        }
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeZsk(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-zsk.c writeZsk(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-zsk.c writeZsk(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-zsk.c writeZsk(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeZsk */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
