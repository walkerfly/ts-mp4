
#ifndef _MP4_COMMON_H
#define _MP4_COMMON_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>

#include <ts/ts.h>
#include <ts/experimental.h>
#include <ts/remap.h>
#include "mp4_meta.h"


class IOHandle
{
public:
    IOHandle(): vio(NULL), buffer(NULL), reader(NULL)
    {
    }

    ~IOHandle()
    {
        if (reader) {
            TSIOBufferReaderFree(reader);
            reader = NULL;
        }

        if (buffer) {
            TSIOBufferDestroy(buffer);
            buffer = NULL;
        }
    }

public:
    TSVIO               vio;
    TSIOBuffer          buffer;
    TSIOBufferReader    reader;
};

class Mp4TransformContext
{
public:
    Mp4TransformContext(float offset, int64_t cl):
                total(0), tail(0), pos(0), content_length(0),
                parse_over(false), raw_transform(false)
    {
        res_buffer = TSIOBufferCreate();
        res_reader = TSIOBufferReaderAlloc(res_buffer);
        dup_reader = TSIOBufferReaderAlloc(res_buffer);

        mm.start = offset * 1000;
        mm.cl = cl;
    }

    ~Mp4TransformContext()
    {
        if (res_reader) {
            TSIOBufferReaderFree(res_reader);
        }

        if (dup_reader) {
            TSIOBufferReaderFree(dup_reader);
        }

        if (res_buffer) {
            TSIOBufferDestroy(res_buffer);
        }
    }

public:
    IOHandle            output;
    Mp4Meta             mm;
    int64_t             total;
    int64_t             tail;
    int64_t             pos;
    int64_t             content_length;
    int64_t             meta_length;

    TSIOBuffer          res_buffer;
    TSIOBufferReader    res_reader;
    TSIOBufferReader    dup_reader;

    bool                parse_over;
    bool                raw_transform;
};

class Mp4Context
{
public:
    Mp4Context(float s): start(s), cl(0), mtc(NULL), transform_added(false)
    {
    }

    ~Mp4Context()
    {
        if (mtc) {
            delete mtc;
            mtc = NULL;
        }
    }

public:
    float       start;
    int64_t     cl;

    Mp4TransformContext *mtc;

    bool        transform_added;
};

#endif

