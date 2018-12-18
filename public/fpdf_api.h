// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

// NOTE: External docs refer to this file as "fpdfview.h", so do not rename
// despite lack of consistency with other public files.

#ifndef PUBLIC_FPDFAPI_H_
#define PUBLIC_FPDFAPI_H_

#include <stddef.h>
#include <vector>
#include <string>
#include <fstream>

#if defined(_WIN32) && !defined(__WINDOWS__)
#include <windows.h>
#endif

#include "fpdfview.h"
#include "fpdf_save.h"
#include "fpdf_ppo.h"

// typedef std::vector<FPDF_PAGE_ITEM*> FPDF_PAGE_LIST;

// Exported Functions
#ifdef __cplusplus
extern "C" {
#endif

typedef std::vector<int> FPDF_INT_LIST;
typedef std::vector<wchar_t> FPDF_WCHAR_LIST;
typedef std::vector<char> FPDF_CHAR_LIST;
typedef std::wstring FPDF_STD_WSTRING;
typedef std::string FPDF_STD_STRING;

typedef struct _FPDF_PNG_ENCODING_ {
    FPDF_EXPORT _FPDF_PNG_ENCODING_();
    FPDF_EXPORT ~_FPDF_PNG_ENCODING_();
    FPDF_EXPORT _FPDF_PNG_ENCODING_(const _FPDF_PNG_ENCODING_& other);
    FPDF_EXPORT _FPDF_PNG_ENCODING_& operator=(const _FPDF_PNG_ENCODING_& other);
    std::vector<unsigned char> png_encoding;
} FPDF_PNG_ENCODING;

typedef struct _FPDF_RECT_ {
    FPDF_EXPORT _FPDF_RECT_();
    FPDF_EXPORT ~_FPDF_RECT_();
    FPDF_EXPORT _FPDF_RECT_(const _FPDF_RECT_& other);
    FPDF_EXPORT _FPDF_RECT_& operator=(const _FPDF_RECT_& other);
    float left;
    float right;
    float top;
    float bottom;
} FPDF_RECT;

typedef struct _FPDF_POINT_ {
    FPDF_EXPORT _FPDF_POINT_();
    FPDF_EXPORT ~_FPDF_POINT_();
    FPDF_EXPORT _FPDF_POINT_(const _FPDF_POINT_& other);
    FPDF_EXPORT _FPDF_POINT_& operator=(const _FPDF_POINT_& other);
    float x;
    float y;
    int type;
    bool close;
} FPDF_POINT;

typedef struct _FPDF_SIZE_ {
    FPDF_EXPORT _FPDF_SIZE_();
    FPDF_EXPORT ~_FPDF_SIZE_();
    FPDF_EXPORT _FPDF_SIZE_(const _FPDF_SIZE_& other);
    FPDF_EXPORT _FPDF_SIZE_& operator=(const _FPDF_SIZE_& other);
    float width;
    float height;
} FPDF_SIZE;

typedef struct _FPDF_COLOR_ {
    FPDF_EXPORT _FPDF_COLOR_();
    FPDF_EXPORT ~_FPDF_COLOR_();
    FPDF_EXPORT _FPDF_COLOR_(const _FPDF_COLOR_& other);
    FPDF_EXPORT _FPDF_COLOR_& operator=(const _FPDF_COLOR_& other);
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
} FPDF_COLOR;

typedef struct _FPDF_CHAR_ITEM_ {
    FPDF_EXPORT _FPDF_CHAR_ITEM_();
    FPDF_EXPORT ~_FPDF_CHAR_ITEM_();
    FPDF_EXPORT _FPDF_CHAR_ITEM_(const _FPDF_CHAR_ITEM_& other);
    FPDF_EXPORT _FPDF_CHAR_ITEM_& operator=(const _FPDF_CHAR_ITEM_& other);
    FPDF_RECT charBox;
    FPDF_RECT fontBox;
    wchar_t text;
    int flag;
} FPDF_CHAR_ITEM;

typedef struct _FPDF_TEXT_ITEM_ {
    FPDF_EXPORT _FPDF_TEXT_ITEM_();
    FPDF_EXPORT ~_FPDF_TEXT_ITEM_();
    FPDF_EXPORT _FPDF_TEXT_ITEM_(const _FPDF_TEXT_ITEM_& other);
    FPDF_EXPORT _FPDF_TEXT_ITEM_& operator=(const _FPDF_TEXT_ITEM_& other);
    FPDF_RECT bbox;
    FPDF_WCHAR_LIST text;
    FPDF_CHAR_LIST familyName;
    FPDF_CHAR_LIST faceName;
    int fontsize;
    int fontflags;
    int textMode;
    bool italic;
    bool bold;
    bool hasFont;
    FPDF_COLOR fillColor;
    FPDF_COLOR strokeColor;
    std::vector<FPDF_CHAR_ITEM> chars;
} FPDF_TEXT_ITEM;

typedef struct _FPDF_PATH_ITEM_ {
    FPDF_EXPORT _FPDF_PATH_ITEM_();
    FPDF_EXPORT ~_FPDF_PATH_ITEM_();
    FPDF_EXPORT _FPDF_PATH_ITEM_(const _FPDF_PATH_ITEM_& other);
    FPDF_EXPORT _FPDF_PATH_ITEM_& operator=(const _FPDF_PATH_ITEM_& other);
    FPDF_RECT bbox;
    FPDF_RECT clipBox;
    bool isRect;
    bool isStroke;
    int fillType;
    float width;
    FPDF_COLOR fillColor;
    FPDF_COLOR strokeColor;
    std::vector<FPDF_POINT> points;
} FPDF_PATH_ITEM;

typedef struct _FPDF_IMAGE_ITEM_ {
    FPDF_EXPORT _FPDF_IMAGE_ITEM_();
    FPDF_EXPORT ~_FPDF_IMAGE_ITEM_();
    FPDF_EXPORT _FPDF_IMAGE_ITEM_(const _FPDF_IMAGE_ITEM_& other);
    FPDF_EXPORT _FPDF_IMAGE_ITEM_& operator=(const _FPDF_IMAGE_ITEM_& other);
    FPDF_RECT bbox;
    FPDF_COLOR fillColor;
    FPDF_COLOR strokeColor;
} FPDF_IMAGE_ITEM;

typedef struct _FPDF_PAGE_ITEM_ {
    FPDF_EXPORT _FPDF_PAGE_ITEM_();
    FPDF_EXPORT ~_FPDF_PAGE_ITEM_();
    FPDF_EXPORT _FPDF_PAGE_ITEM_(const _FPDF_PAGE_ITEM_& other);
    FPDF_EXPORT _FPDF_PAGE_ITEM_& operator=(const _FPDF_PAGE_ITEM_& other);
    int id;
    int rotation;
    FPDF_SIZE size;
    std::vector<FPDF_TEXT_ITEM> texts;
    std::vector<FPDF_PATH_ITEM> paths;
    std::vector<FPDF_IMAGE_ITEM> images;
} FPDF_PAGE_ITEM;

typedef struct _FPDF_BOOKMARK_ITEM_ {
    FPDF_EXPORT _FPDF_BOOKMARK_ITEM_();
    FPDF_EXPORT ~_FPDF_BOOKMARK_ITEM_();
    FPDF_EXPORT _FPDF_BOOKMARK_ITEM_(const _FPDF_BOOKMARK_ITEM_& other);
    FPDF_EXPORT _FPDF_BOOKMARK_ITEM_& operator=(const _FPDF_BOOKMARK_ITEM_& other);
    int index;
    int parent;
    int page;
    int level;
    FPDF_INT_LIST children;
    FPDF_WCHAR_LIST title;
} FPDF_BOOKMARK_ITEM;

typedef struct _FPDF_BOOKMARKS_ITEM_ {
    FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_();
    FPDF_EXPORT ~_FPDF_BOOKMARKS_ITEM_();
    FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_(const _FPDF_BOOKMARKS_ITEM_& other);
    FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_& operator=(const _FPDF_BOOKMARKS_ITEM_& other);
    int id;
    int rotation;
    FPDF_SIZE size;
    std::vector<FPDF_BOOKMARK_ITEM> bookmarks;
} FPDF_BOOKMARKS_ITEM;

class BufferFileWrite : public FPDF_FILEWRITE {
  public:
    BufferFileWrite(const std::string &file);
    ~BufferFileWrite();

  private:
    int DoWriteBlock(const void* data, unsigned long size);
    static int WriteBlockImpl(FPDF_FILEWRITE* this_file_write, const void* data, unsigned long size);
    
    std::ofstream _file;
};

FPDF_EXPORT void FPDF_CALLCONV
FPDF_LoadPageObject(FPDF_PAGE page, FPDF_PAGE_ITEM& pageObj);

FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV
FPDF_SaveDocument(FPDF_DOCUMENT document, FPDF_STRING file_path);

FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV
FPDF_SavePagesToFile(FPDF_DOCUMENT document, FPDF_BYTESTRING pagerange, FPDF_STRING file_path);

FPDF_EXPORT void FPDF_CALLCONV
FPDF_GetBookmarks(FPDF_DOCUMENT document, FPDF_BOOKMARKS_ITEM& bookmarksItem);

FPDF_EXPORT bool FPDF_CALLCONV
FPDF_SavePageBitmap(FPDF_PAGE page, FPDF_STRING path, double scaleX, double scaleY);

FPDF_EXPORT bool FPDF_CALLCONV
FPDF_GetPageBitmap(FPDF_PAGE page, FPDF_PNG_ENCODING& png_encoding, double scaleX, double scaleY);

#ifdef __cplusplus
}
#endif

#endif  // PUBLIC_FPDFAPI_H_
