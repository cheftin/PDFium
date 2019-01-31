#include "public/fpdf_api.h"
#include "public/fpdfview.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_ppo.h"
#include "public/cpp/fpdf_scopers.h"

#include <memory>
#include <utility>
#include <vector>

#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/cpdf_pagerendercontext.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_textobject.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/page/cpdf_imageobject.h"
#include "core/fpdfapi/page/cpdf_formobject.h"
#include "core/fpdfapi/page/cpdf_shadingobject.h"
#include "core/fpdfapi/page/cpdf_form.h"
#include "core/fpdfapi/page/cpdf_image.h"
#include "core/fpdfapi/font/cpdf_font.h"
#include "core/fpdfdoc/cpdf_viewerpreferences.h"
#include "core/fpdftext/cpdf_textpage.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/unowned_ptr.h"
#include "core/fxge/cfx_font.h"
#include "core/fpdfdoc/cpdf_bookmark.h"
#include "fpdfsdk/cpdfsdk_helpers.h"
#include "fpdfsdk/cpdfsdk_pageview.h"
#include "testing/image_diff/image_diff_png.h"
#include "core/fpdfapi/parser/cpdf_dictionary.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "constants/page_object.h"

FPDF_EXPORT _FPDF_PNG_ENCODING_::_FPDF_PNG_ENCODING_() = default;
FPDF_EXPORT _FPDF_PNG_ENCODING_::~_FPDF_PNG_ENCODING_() = default;
FPDF_EXPORT _FPDF_PNG_ENCODING_::_FPDF_PNG_ENCODING_(const _FPDF_PNG_ENCODING_& other) = default;
FPDF_EXPORT _FPDF_PNG_ENCODING_& _FPDF_PNG_ENCODING_::operator=(const _FPDF_PNG_ENCODING_& other) = default;

FPDF_EXPORT _FPDF_RECT_::_FPDF_RECT_() = default;
FPDF_EXPORT _FPDF_RECT_::~_FPDF_RECT_() = default;
FPDF_EXPORT _FPDF_RECT_::_FPDF_RECT_(const _FPDF_RECT_& other) = default;
FPDF_EXPORT _FPDF_RECT_& _FPDF_RECT_::operator=(const _FPDF_RECT_& other) = default;

FPDF_EXPORT _FPDF_POINT_::_FPDF_POINT_() = default;
FPDF_EXPORT _FPDF_POINT_::~_FPDF_POINT_() = default;
FPDF_EXPORT _FPDF_POINT_::_FPDF_POINT_(const _FPDF_POINT_& other) = default;
FPDF_EXPORT _FPDF_POINT_& _FPDF_POINT_::operator=(const _FPDF_POINT_& other) = default;

FPDF_EXPORT _FPDF_SIZE_::_FPDF_SIZE_() = default;
FPDF_EXPORT _FPDF_SIZE_::~_FPDF_SIZE_() = default;
FPDF_EXPORT _FPDF_SIZE_::_FPDF_SIZE_(const _FPDF_SIZE_& other) = default;
FPDF_EXPORT _FPDF_SIZE_& _FPDF_SIZE_::operator=(const _FPDF_SIZE_& other) = default;

FPDF_EXPORT _FPDF_COLOR_::_FPDF_COLOR_() = default;
FPDF_EXPORT _FPDF_COLOR_::~_FPDF_COLOR_() = default;
FPDF_EXPORT _FPDF_COLOR_::_FPDF_COLOR_(const _FPDF_COLOR_& other) = default;
FPDF_EXPORT _FPDF_COLOR_& _FPDF_COLOR_::operator=(const _FPDF_COLOR_& other) = default;

FPDF_EXPORT _FPDF_CHAR_ITEM_::_FPDF_CHAR_ITEM_() = default;
FPDF_EXPORT _FPDF_CHAR_ITEM_::~_FPDF_CHAR_ITEM_() = default;
FPDF_EXPORT _FPDF_CHAR_ITEM_::_FPDF_CHAR_ITEM_(const _FPDF_CHAR_ITEM_& other) = default;
FPDF_EXPORT _FPDF_CHAR_ITEM_& _FPDF_CHAR_ITEM_::operator=(const _FPDF_CHAR_ITEM_& other) = default;

FPDF_EXPORT _FPDF_TEXT_ITEM_::_FPDF_TEXT_ITEM_() = default;
FPDF_EXPORT _FPDF_TEXT_ITEM_::~_FPDF_TEXT_ITEM_() = default;
FPDF_EXPORT _FPDF_TEXT_ITEM_::_FPDF_TEXT_ITEM_(const _FPDF_TEXT_ITEM_& other) = default;
FPDF_EXPORT _FPDF_TEXT_ITEM_& _FPDF_TEXT_ITEM_::operator=(const _FPDF_TEXT_ITEM_& other) = default;

FPDF_EXPORT _FPDF_PATH_ITEM_::_FPDF_PATH_ITEM_() = default;
FPDF_EXPORT _FPDF_PATH_ITEM_::~_FPDF_PATH_ITEM_() = default;
FPDF_EXPORT _FPDF_PATH_ITEM_::_FPDF_PATH_ITEM_(const _FPDF_PATH_ITEM_& other) = default;
FPDF_EXPORT _FPDF_PATH_ITEM_& _FPDF_PATH_ITEM_::operator=(const _FPDF_PATH_ITEM_& other) = default;

FPDF_EXPORT _FPDF_IMAGE_ITEM_::_FPDF_IMAGE_ITEM_() = default;
FPDF_EXPORT _FPDF_IMAGE_ITEM_::~_FPDF_IMAGE_ITEM_() = default;
FPDF_EXPORT _FPDF_IMAGE_ITEM_::_FPDF_IMAGE_ITEM_(const _FPDF_IMAGE_ITEM_& other) = default;
FPDF_EXPORT _FPDF_IMAGE_ITEM_& _FPDF_IMAGE_ITEM_::operator=(const _FPDF_IMAGE_ITEM_& other) = default;

FPDF_EXPORT _FPDF_PAGE_ITEM_::_FPDF_PAGE_ITEM_() = default;
FPDF_EXPORT _FPDF_PAGE_ITEM_::~_FPDF_PAGE_ITEM_() = default;
FPDF_EXPORT _FPDF_PAGE_ITEM_::_FPDF_PAGE_ITEM_(const _FPDF_PAGE_ITEM_& other) = default;
FPDF_EXPORT _FPDF_PAGE_ITEM_& _FPDF_PAGE_ITEM_::operator=(const _FPDF_PAGE_ITEM_& other) = default;

FPDF_EXPORT _FPDF_BOOKMARK_ITEM_::_FPDF_BOOKMARK_ITEM_() = default;
FPDF_EXPORT _FPDF_BOOKMARK_ITEM_::~_FPDF_BOOKMARK_ITEM_() = default;
FPDF_EXPORT _FPDF_BOOKMARK_ITEM_::_FPDF_BOOKMARK_ITEM_(const _FPDF_BOOKMARK_ITEM_& other) = default;
FPDF_EXPORT _FPDF_BOOKMARK_ITEM_& _FPDF_BOOKMARK_ITEM_::operator=(const _FPDF_BOOKMARK_ITEM_& other) = default;

FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_::_FPDF_BOOKMARKS_ITEM_() = default;
FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_::~_FPDF_BOOKMARKS_ITEM_() = default;
FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_::_FPDF_BOOKMARKS_ITEM_(const _FPDF_BOOKMARKS_ITEM_& other) = default;
FPDF_EXPORT _FPDF_BOOKMARKS_ITEM_& _FPDF_BOOKMARKS_ITEM_::operator=(const _FPDF_BOOKMARKS_ITEM_& other) = default;


BufferFileWrite::BufferFileWrite(const std::string &file) : _file(file, std::ios::out | std::ios::binary) {
    version = 1;
    WriteBlock = &WriteBlockImpl;
}

BufferFileWrite::~BufferFileWrite() {
    _file.close();
}

int BufferFileWrite::DoWriteBlock(const void* data, unsigned long size){
    _file.write(static_cast<const char*>(data), size);
    return 1;
}

int BufferFileWrite::WriteBlockImpl(FPDF_FILEWRITE* this_file_write, const void* data, unsigned long size) {
    BufferFileWrite* mem_buffer_file_write =
        static_cast<BufferFileWrite*>(this_file_write);
    return mem_buffer_file_write->DoWriteBlock(data, size);
}

unsigned int GetUnsignedAlpha(float alpha) {
  return static_cast<unsigned int>(alpha * 255.f + 0.5f);
}

void FPDF_GetPageMatrix(CPDF_Page* pPage, CFX_Matrix& matrix) {
    const FX_RECT rect(0, 0, static_cast<int>(pPage->GetPageWidth()),
                     static_cast<int>(pPage->GetPageHeight()));
    matrix = pPage->GetDisplayMatrix(rect, 0);
}

void FPDF_GetColor(CPDF_PageObject* pPageObj, FPDF_COLOR& fillColor, FPDF_COLOR& strokeColor) {
    if (!pPageObj) {
        fillColor.r = 0;
        fillColor.g = 0;
        fillColor.b = 0;
        fillColor.a = 255;
        strokeColor.r = 0;
        strokeColor.g = 0;
        strokeColor.b = 0;
        strokeColor.a = 255;
        return;
    }
    FX_COLORREF fillColorRef = pPageObj->m_ColorState.HasFillColor() ? pPageObj->m_ColorState.GetFillColorRef() : 0xFFFFFFFF;
    fillColor.r = FXSYS_GetRValue(fillColorRef);
    fillColor.g = FXSYS_GetGValue(fillColorRef);
    fillColor.b = FXSYS_GetBValue(fillColorRef);
    fillColor.a = GetUnsignedAlpha(pPageObj->m_GeneralState.GetFillAlpha());

    FX_COLORREF strokeColorRef = pPageObj->m_ColorState.HasStrokeColor() ? pPageObj->m_ColorState.GetStrokeColorRef() : 0xFFFFFFFF;
    strokeColor.r = FXSYS_GetRValue(strokeColorRef);
    strokeColor.g = FXSYS_GetGValue(strokeColorRef);
    strokeColor.b = FXSYS_GetBValue(strokeColorRef);
    strokeColor.a = GetUnsignedAlpha(pPageObj->m_GeneralState.GetStrokeAlpha());
}

void FPDF_GetPageObjectBBox(CPDF_PageObject* pageObj, FPDF_RECT& box, CPDF_Page* pPage=nullptr) {
    if (!pageObj) {
        box.left = 0.0;
        box.top = 0.0;
        box.right = 0.0;
        box.bottom = 0.0;
        return;
    }
    CFX_FloatRect rect;
    if (pageObj->IsText()) {
        rect = pageObj->AsText()->GetRect();
    } else if (pageObj->IsPath()) {
        rect = pageObj->AsPath()->GetRect();
    } else if (pageObj->IsImage()) {
        rect = pageObj->AsImage()->GetRect();
    }
    if (pPage) {
        CFX_Matrix matrix;
        FPDF_GetPageMatrix(pPage, matrix);
        rect = matrix.TransformRect(rect);
    }
    box.left = rect.left;
    box.top = rect.top;
    box.right = rect.right;
    box.bottom = rect.bottom;
}

void FPDF_GetPageObjectClipBox(CPDF_PageObject* pageObj, FPDF_RECT& box, CPDF_Page* pPage=nullptr) {
    if (!pageObj || !pageObj->m_ClipPath.HasRef()) {
        box.left = 0.0;
        box.top = 0.0;
        box.right = 0.0;
        box.bottom = 0.0;
        return;
    }
    CFX_FloatRect rect = pageObj->m_ClipPath.GetClipBox();
    if (pPage) {
        CFX_Matrix matrix;
        FPDF_GetPageMatrix(pPage, matrix);
        rect = matrix.TransformRect(rect);
    }
    box.left = rect.left;
    box.top = rect.top;
    box.right = rect.right;
    box.bottom = rect.bottom;
}

void FPDF_GetTextStyle(FPDF_CHAR_INFO& charInfo, FPDF_TEXT_ITEM& textItem) {
    bool bHasFont = charInfo.m_pTextObj && charInfo.m_pTextObj->GetFont();
    if (bHasFont) {
        CPDF_Font* pdFont = charInfo.m_pTextObj->GetFont();
        CFX_Font* fxFont = pdFont->GetFont();
        textItem.hasFont = true;

        std::string familyName = fxFont->GetFamilyName().c_str();
        textItem.familyName.clear();
        textItem.familyName.reserve(familyName.size());
        std::copy(familyName.begin(), familyName.end(), std::back_inserter(textItem.familyName));

        std::string faceName = fxFont->GetFaceName().c_str();
        textItem.faceName.clear();
        textItem.faceName.reserve(faceName.size());
        std::copy(faceName.begin(), faceName.end(), std::back_inserter(textItem.faceName));

        textItem.bold = fxFont->IsBold();
        textItem.italic = fxFont->IsItalic();
        textItem.fontflags = pdFont->GetFontFlags();
        textItem.fontsize = charInfo.m_FontSize;
        textItem.textMode = static_cast<int>(charInfo.m_pTextObj->m_TextState.GetTextMode());
    } else {
        textItem.hasFont = false;
        textItem.bold = false;
        textItem.italic = false;
        textItem.fontflags = 0;
        textItem.fontsize = 0;
        textItem.textMode = 0;
    }
}

void FPDF_GetCharItem(FPDF_CHAR_INFO& charInfo, FPDF_CHAR_ITEM& charItem, CPDF_Page* pPage=nullptr) {
    charItem.text = charInfo.m_Unicode;
    charItem.flag = charInfo.m_Flag;
    CFX_Matrix matrix;
    if (pPage) {
        FPDF_GetPageMatrix(pPage, matrix);
    }
    CFX_FloatRect charBox = matrix.TransformRect(charInfo.m_CharBox);
    charItem.charBox.left = charBox.left;
    charItem.charBox.top = charBox.top;
    charItem.charBox.right = charBox.right;
    charItem.charBox.bottom = charBox.bottom;

    bool bHasFont = charInfo.m_pTextObj && charInfo.m_pTextObj->GetFont();
    float width = bHasFont ? charInfo.m_pTextObj->GetCharWidth(charInfo.m_Charcode) : 0.0;
    if (bHasFont &&  width > 0) {
        CPDF_Font* font = charInfo.m_pTextObj->GetFont();
        FX_RECT rect = font->GetFontBBox();
        const float fFontSize = charInfo.m_FontSize / 1000;
        CFX_PointF origin = charInfo.m_Matrix.GetInverse().Transform(charInfo.m_Origin);
        CFX_FloatRect fontBox;
        fontBox.top = rect.top * fFontSize + origin.y;
        fontBox.left = origin.x;
        fontBox.right = fontBox.left + width;
        fontBox.bottom = rect.bottom * fFontSize + origin.y;
        // if (fabsf(rect.top - rect.bottom) < kSizeEpsilon) {
        //     rect.top = charinfo.m_CharBox.bottom + charInfo.m_FontSize;
        // }
        // if (fabsf(charinfo.m_CharBox.right - charinfo.m_CharBox.left) <
        //     kSizeEpsilon) {
        // charinfo.m_CharBox.right =
        //     charinfo.m_CharBox.left + pTextObj->GetCharWidth(charinfo.m_CharCode);
        // }
        fontBox = charInfo.m_Matrix.TransformRect(fontBox);
        fontBox = matrix.TransformRect(fontBox);
        charItem.fontBox.left = fontBox.left;
        charItem.fontBox.top = fontBox.top;
        charItem.fontBox.right = fontBox.right;
        charItem.fontBox.bottom = fontBox.bottom;
    } else {
        charItem.fontBox = charItem.charBox;
    }
}

void FPDF_GetPathItem(CPDF_PathObject* pPathObj, FPDF_PATH_ITEM& pathItem) {
    FPDF_GetPageObjectBBox(pPathObj, pathItem.bbox);

    pathItem.fillType = pPathObj->m_FillType;
    pathItem.isStroke = pPathObj->m_bStroke;
    pathItem.width = pPathObj->m_GraphState.GetLineWidth();
    pathItem.isRect = pPathObj->m_Path.IsRect();
    pPathObj->m_Path.Transform(pPathObj->m_Matrix);
    if (pPathObj->m_ClipPath.HasRef()) {
        CFX_FloatRect rect = pPathObj->m_ClipPath.GetClipBox();
        pathItem.clipBox.left = rect.left;
        pathItem.clipBox.top = rect.top;
        pathItem.clipBox.right = rect.right;
        pathItem.clipBox.bottom = rect.bottom;
    } else {
        pathItem.clipBox.left = 0.0;
        pathItem.clipBox.top = 0.0;
        pathItem.clipBox.right = 0.0;
        pathItem.clipBox.bottom = 0.0;
    }

    for (const FX_PATHPOINT& point : pPathObj->m_Path.GetPoints()) {
        FPDF_POINT pointItem;
        pointItem.x = point.m_Point.x;
        pointItem.y = point.m_Point.y;
        pointItem.close = point.m_CloseFigure;
        switch (point.m_Type) {
            case FXPT_TYPE::LineTo:
                pointItem.type = 0;
                break;
            case FXPT_TYPE::BezierTo:
                pointItem.type = 1;
                break;
            case FXPT_TYPE::MoveTo:
                pointItem.type = 2;
                break;
            default:
                pointItem.type = -1;
                break;
        }
        pathItem.points.push_back(pointItem);
    }
    FPDF_GetColor(pPathObj, pathItem.fillColor, pathItem.strokeColor);
}

void FPDF_GetImageItem(CPDF_ImageObject* pImageObj, FPDF_IMAGE_ITEM& imageItem) {
    FPDF_GetPageObjectBBox(pImageObj, imageItem.bbox);
    FPDF_GetColor(pImageObj, imageItem.fillColor, imageItem.strokeColor);
}

void FPDF_ProcessShadingObject(
    CPDF_ShadingObject* pShadingObj,
    const CFX_Matrix& formMatrix,
    const CPDF_PageObjectList* pObjList,
    CPDF_PageObjectList::const_iterator ObjPos,
    std::vector<CPDF_ShadingObject*>& shadings) {
    pShadingObj->Transform(formMatrix);
    shadings.push_back(pShadingObj);
    return;
}

void FPDF_ProcessPathObject(
    CPDF_PathObject* pPathObj,
    const CFX_Matrix& formMatrix,
    const CPDF_PageObjectList* pObjList,
    CPDF_PageObjectList::const_iterator ObjPos,
    std::vector<CPDF_PathObject*>& paths) {
    pPathObj->Transform(formMatrix);
    if (pPathObj->m_ClipPath.HasRef()) {
        pPathObj->m_ClipPath.Transform(formMatrix);
    }
    paths.push_back(pPathObj);
    return;
}

void FPDF_ProcessImageObject(
    CPDF_ImageObject* pImageObj,
    const CFX_Matrix& formMatrix,
    const CPDF_PageObjectList* pObjList,
    CPDF_PageObjectList::const_iterator ObjPos,
    std::vector<CPDF_ImageObject*>& images) {
    pImageObj->Transform(formMatrix);
    images.push_back(pImageObj);
    return;
}

void FPDF_ProcessFormObject(
    CPDF_FormObject* pFormObj,
    const CFX_Matrix& formMatrix,
    std::vector<CPDF_PathObject*>& paths,
    std::vector<CPDF_ImageObject*>& images,
    std::vector<CPDF_ShadingObject*>& shadings) {
    const CPDF_PageObjectList* pObjectList = pFormObj->form()->GetPageObjectList();
    if (pObjectList->empty())
        return;

    CFX_Matrix curFormMatrix = pFormObj->form_matrix();
    curFormMatrix.Concat(formMatrix);

    for (auto it = pObjectList->begin(); it != pObjectList->end(); ++it) {
        CPDF_PageObject* pPageObj = it->get();
        if (!pPageObj)
            continue;
        if (pPageObj->IsPath())
            FPDF_ProcessPathObject(pPageObj->AsPath(), curFormMatrix, pObjectList, it, paths);
        else if (pPageObj->IsImage())
            FPDF_ProcessImageObject(pPageObj->AsImage(), curFormMatrix, pObjectList, it, images);
        else if (pPageObj->IsForm())
            FPDF_ProcessFormObject(pPageObj->AsForm(), curFormMatrix, paths, images, shadings);
        // else if (pPageObj->IsShading())
        //     FPDF_ProcessShadingObject(pPageObj->AsShading(), curFormMatrix, pObjectList, it, shadings);
    }
}

void FPDF_ProcessObject(
    CPDF_Page* pPage,
    std::vector<FPDF_PATH_ITEM>& pathItems,
    std::vector<FPDF_IMAGE_ITEM>& imageItems) {
    if (pPage->GetPageObjectList()->empty())
        return;

    CFX_Matrix matrix;
    FPDF_GetPageMatrix(pPage, matrix);
    std::vector<CPDF_PathObject*> paths;
    std::vector<CPDF_ImageObject*> images;
    std::vector<CPDF_ShadingObject*> shadings;
    const CPDF_PageObjectList* pObjList = pPage->GetPageObjectList();
    for (auto it = pObjList->begin(); it != pObjList->end(); ++it) {
        CPDF_PageObject* pObj = it->get();
        if (!pObj)
            continue;
        if (pObj->IsPath())
            FPDF_ProcessPathObject(pObj->AsPath(), matrix, pObjList, it, paths);
        else if (pObj->IsImage())
            FPDF_ProcessImageObject(pObj->AsImage(), matrix, pObjList, it, images);
        else if (pObj->IsForm())
            FPDF_ProcessFormObject(pObj->AsForm(), matrix, paths, images, shadings);
        // else if (pObj->IsShading())
        //     FPDF_ProcessShadingObject(pObj->AsShading(), matrix, pObjList, it, shadings);
    }

    for (CPDF_PathObject* obj : paths) {
        FPDF_PATH_ITEM pathItem;
        FPDF_GetPathItem(obj, pathItem);
        pathItems.push_back(pathItem);
    }
    for (CPDF_ImageObject* obj : images) {
        FPDF_IMAGE_ITEM imageItem;
        FPDF_GetImageItem(obj, imageItem);
        imageItems.push_back(imageItem);
    }
}

FPDF_EXPORT void FPDF_CALLCONV
FPDF_LoadPageObject(FPDF_PAGE page, FPDF_PAGE_ITEM& pageObj) {
    CPDF_Page* pPDFPage = CPDFPageFromFPDFPage(page);
    if (!pPDFPage)
        return;
    pPDFPage->ParseContent();
    // const CPDF_PageObjectList* pPageObjectList = pPDFPage->GetPageObjectList();

    pageObj.size.width = pPDFPage->GetPageWidth();
    pageObj.size.height = pPDFPage->GetPageHeight();
    pageObj.rotation = pPDFPage->GetPageRotation();

    CPDF_ViewerPreferences viewRef(pPDFPage->GetDocument());
    CPDF_TextPage* textPage = new CPDF_TextPage(
        pPDFPage, viewRef.IsDirectionR2L() ? FPDFText_Direction::Right : FPDFText_Direction::Left);
    textPage->setNeedTransformClipPath(true);
    textPage->ParseTextPage();

    UnownedPtr<CPDF_TextObject> curTextObj = nullptr;
    for (int i = 0; i < textPage->CountChars(); i++) {
        FPDF_CHAR_ITEM charItem;
        FPDF_CHAR_INFO charInfo;
        textPage->GetCharInfo(i, &charInfo);
        FPDF_GetCharItem(charInfo, charItem, pPDFPage);

        if (curTextObj != charInfo.m_pTextObj) {
            FPDF_TEXT_ITEM textItem;
            FPDF_GetTextStyle(charInfo, textItem);
            CPDF_TextObject* pTextObj = charInfo.m_pTextObj ? charInfo.m_pTextObj.Get() : nullptr;
            FPDF_GetPageObjectBBox(pTextObj, textItem.bbox, pPDFPage);
            FPDF_GetPageObjectClipBox(pTextObj, textItem.clipBox, pPDFPage);
            FPDF_GetColor(pTextObj, textItem.fillColor, textItem.strokeColor);
            // std::wstring text = textPage->GetTextByObject(charInfo.m_pTextObj ? charInfo.m_pTextObj.Get() : nullptr).c_str();
            // textItem.text.clear();
            // textItem.text.reserve(text.size());
            // std::copy(text.begin(), text.end(), std::back_inserter(textItem.text));
            textItem.chars.push_back(charItem);
            pageObj.texts.push_back(textItem);
            curTextObj = charInfo.m_pTextObj;
        } else {
            FPDF_TEXT_ITEM &textItem = pageObj.texts.back();
            textItem.chars.push_back(charItem);
        }
    }
    FPDF_ProcessObject(pPDFPage, pageObj.paths, pageObj.images);
    delete textPage;
}

FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV
FPDF_SaveDocument(FPDF_DOCUMENT document, FPDF_STRING file_path) {
    BufferFileWrite *output_file_write = new BufferFileWrite(std::string(file_path));
    FPDF_BOOL status = FPDF_SaveAsCopy(document, output_file_write, FPDF_REMOVE_SECURITY);
    delete output_file_write;
    return status;
}

FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV
FPDF_SavePagesToFile(FPDF_DOCUMENT document, FPDF_BYTESTRING pagerange, FPDF_STRING file_path) {
    FPDF_DOCUMENT dest_doc = FPDF_CreateNewDocument();
    if (!dest_doc) {
        return false;
    }
    if (!FPDF_ImportPages(dest_doc, document, pagerange, 0)) {
        FPDF_CloseDocument(dest_doc);
        return false;
    }
    FPDF_BOOL status = FPDF_SaveDocument(dest_doc, file_path);
    FPDF_CloseDocument(dest_doc);
    return status;
}

bool compareBookmarkItem(const FPDF_BOOKMARK_ITEM &item1, const FPDF_BOOKMARK_ITEM &item2){
    return item1.index < item2.index;
}

int FPDF_IterBookmarks(FPDF_DOCUMENT document, FPDF_BOOKMARK bookmark, std::vector<FPDF_BOOKMARK_ITEM>& bookmarks, FPDF_BOOKMARK_ITEM* parent, int parent_index, int index, int level) {
    FPDF_BOOKMARK child = FPDFBookmark_GetFirstChild(document, bookmark);
    while (child) {
        FPDF_BOOKMARK_ITEM item;
        item.index = index++;
        item.level = level;
        item.parent = parent_index;
        if (parent) {
            parent->children.push_back(item.index);
        }

        FPDF_DEST dest = FPDFBookmark_GetDest(document, child);

        if (!dest) {
            FPDF_ACTION action = FPDFBookmark_GetAction(child);
            if (action) {
                dest = FPDFAction_GetDest(document, action);
            }
        }
        if (dest) {
            item.page = FPDFDest_GetDestPageIndex(document, dest);
        } else {
            item.page = -1;
        }

        CPDF_Bookmark cbookmark(CPDFDictionaryFromFPDFBookmark(child));
        std::wstring title = cbookmark.GetTitle().c_str();
        item.title.clear();
        item.title.reserve(title.size());
        std::copy(title.begin(), title.end(), std::back_inserter(item.title));

        index = FPDF_IterBookmarks(document, child, bookmarks, &item, item.index, index, level + 1);
        child = FPDFBookmark_GetNextSibling(document, child);
        bookmarks.push_back(item);
    }
    return index;
}

FPDF_EXPORT void FPDF_CALLCONV
FPDF_GetBookmarks(FPDF_DOCUMENT document, FPDF_BOOKMARKS_ITEM& bookmarksItem) {
    FPDF_IterBookmarks(document, nullptr, bookmarksItem.bookmarks, nullptr, -1, 0, 1);
    std::sort(bookmarksItem.bookmarks.begin(), bookmarksItem.bookmarks.end(), compareBookmarkItem);
}

bool CheckDimensions(int stride, int width, int height) {
  if (stride < 0 || width < 0 || height < 0)
    return false;
  if (height > 0 && width > INT_MAX / height)
    return false;
  return true;
}

bool WriteToPng(FPDF_STRING path,
                const void* buffer_void,
                int stride,
                int width,
                int height) {
    if (!CheckDimensions(stride, width, height)) {
        return false;
    }

    std::vector<unsigned char> png_encoding;
    const auto* buffer = static_cast<const unsigned char*>(buffer_void);
    if (!image_diff_png::EncodeBGRAPNG(buffer, width, height, stride, false, &png_encoding)) {
        fprintf(stderr, "Failed to convert bitmap to PNG\n");
        return false;
    }

    FILE* fp = fopen(path, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open %s for output\n", path);
        return false;
    }

    size_t bytes_written = fwrite(&png_encoding.front(), 1, png_encoding.size(), fp);
    if (bytes_written != png_encoding.size()) {
        fprintf(stderr, "Failed to write to %s\n", path);
    }

    (void)fclose(fp);
    return true;
}

bool GetPngData(std::vector<unsigned char>& png_encoding,
                const void* buffer_void,
                int stride,
                int width,
                int height) {
    if (!CheckDimensions(stride, width, height)) {
        return false;
    }

    const auto* buffer = static_cast<const unsigned char*>(buffer_void);
    if (!image_diff_png::EncodeBGRAPNG(buffer, width, height, stride, false, &png_encoding)) {
        fprintf(stderr, "Failed to convert bitmap to PNG\n");
        return false;
    }
    return true;
}

FPDF_EXPORT bool FPDF_CALLCONV
FPDF_SavePageBitmap(FPDF_PAGE page, FPDF_STRING path, double scaleX, double scaleY) {
    auto width = static_cast<int>(FPDF_GetPageWidth(page) * scaleX);
    auto height = static_cast<int>(FPDF_GetPageHeight(page) * scaleY);
    int alpha = FPDFPage_HasTransparency(page) ? 1 : 0;
    ScopedFPDFBitmap bitmap(FPDFBitmap_Create(width, height, alpha));

    if (bitmap) {
        FPDF_DWORD fill_color = alpha ? 0x00000000 : 0xFFFFFFFF;
        FPDFBitmap_FillRect(bitmap.get(), 0, 0, width, height, fill_color);

        FPDF_RenderPageBitmap(bitmap.get(), page, 0, 0, width, height, 0, FPDF_ANNOT);

        int stride = FPDFBitmap_GetStride(bitmap.get());
        const char* buffer = reinterpret_cast<const char*>(FPDFBitmap_GetBuffer(bitmap.get()));

        return WriteToPng(path, buffer, stride, width, height);
    } else {
        return false;
    }
}

FPDF_EXPORT bool FPDF_CALLCONV
FPDF_GetPageBitmap(FPDF_PAGE page, FPDF_PNG_ENCODING& png_encoding, double scaleX, double scaleY) {
    auto width = static_cast<int>(FPDF_GetPageWidth(page) * scaleX);
    auto height = static_cast<int>(FPDF_GetPageHeight(page) * scaleY);
    int alpha = FPDFPage_HasTransparency(page) ? 1 : 0;
    ScopedFPDFBitmap bitmap(FPDFBitmap_Create(width, height, alpha));

    if (bitmap) {
        FPDF_DWORD fill_color = alpha ? 0x00000000 : 0xFFFFFFFF;
        FPDFBitmap_FillRect(bitmap.get(), 0, 0, width, height, fill_color);

        FPDF_RenderPageBitmap(bitmap.get(), page, 0, 0, width, height, 0, FPDF_ANNOT);

        int stride = FPDFBitmap_GetStride(bitmap.get());
        const char* buffer = reinterpret_cast<const char*>(FPDFBitmap_GetBuffer(bitmap.get()));

        return GetPngData(png_encoding.png_encoding, buffer, stride, width, height);
    } else {
        return false;
    }
}

// FPDF_EXPORT FPDF_BITMAP FPDF_CALLCONV
// FPDF_GetPageBitmap(FPDF_PAGE page) {
//     ScopedFPDFTextPage text_page(FPDFText_LoadPage(page));
//     double scale = 1.0;

//     auto width = static_cast<int>(FPDF_GetPageWidth(page) * scale);
//     auto height = static_cast<int>(FPDF_GetPageHeight(page) * scale);
//     int alpha = FPDFPage_HasTransparency(page) ? 1 : 0;
//     ScopedFPDFBitmap bitmap(FPDFBitmap_Create(width, height, alpha));

//     if (bitmap) {
//         FPDF_DWORD fill_color = alpha ? 0x00000000 : 0xFFFFFFFF;
//         FPDFBitmap_FillRect(bitmap.get(), 0, 0, width, height, fill_color);

//         // Note, client programs probably want to use this method instead of the
//         // progressive calls. The progressive calls are if you need to pause the
//         // rendering to update the UI, the PDF renderer will break when possible.
//         FPDF_RenderPageBitmap(bitmap.get(), page, 0, 0, width, height, 0, FPDF_ANNOT);
//         return bitmap.get();
//     } else {
//         return NULL;
//     }
// }

FPDF_EXPORT bool FPDF_CALLCONV
FPDF_ExtractFont(FPDF_DOCUMENT document, FPDF_STRING font_name, FPDF_STRING save_path)
{
    CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(document);
    CPDF_Font* font = CPDF_Font::GetStockFont(pDoc, font_name);
    if(!font) {
        return false;
    }
    pdfium::span<uint8_t> data = font->GetFont()->GetFontSpan();

    std::ofstream file_stream;
    file_stream.open(save_path, std::ios::trunc);
    file_stream.write((const char*)data.data(), data.size());
    file_stream.close();

    return true;
}

FPDF_EXPORT uint8_t* FPDF_CALLCONV
FPDF_ExtractPageContents(FPDF_DOCUMENT document, int page_index, uint32_t& length)
{
    CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(document);
    if (nullptr == pDoc)
        return nullptr;
    const CPDF_Dictionary* pPageDict = pDoc->GetPageDictionary(page_index);
    if (nullptr == pPageDict)
        return nullptr;

    const CPDF_Stream* stream = nullptr;
    const CPDF_Object* pObj = pPageDict->GetObjectFor(pdfium::page_object::kContents);
    if (nullptr == pObj)
        return nullptr;
    if (pObj->IsReference()) {
        stream = pPageDict->GetStreamFor(pdfium::page_object::kContents);
    } else if (pObj->IsArray()) {
        CPDF_Array* pArrayObj = static_cast<CPDF_Array*>(const_cast<CPDF_Object*>(pObj));
        stream = pArrayObj->GetStreamAt(0);
    }
    if (nullptr == stream)
        return nullptr;

    // extract data
    length = stream->GetRawSize();
    uint8_t* data = new uint8_t[length];
    if (!stream->ReadRawData(0, data, length)) {
        delete [] data;
        return nullptr;
    }

    return data;
}
