#define _USE_MATH_DEFINES

#include "public/fpdf_api.h"
#include "public/fpdfview.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_ppo.h"
#include "public/cpp/fpdf_scopers.h"

#include <cmath>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/page/cpdf_pagerendercontext.h"
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
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fpdfapi/parser/cpdf_string.h"
#include "constants/page_object.h"
#include "core/fpdfapi/parser/cpdf_stream.h"
#include "core/fpdfapi/parser/cpdf_stream_acc.h"
#include "core/fxcrt/fx_safe_types.h"
#include "core/fxcrt/cfx_utf8encoder.h"
#include "core/fxge/cfx_gemodule.h"
#include "core/fxge/cfx_fontmgr.h"
#include "core/fxge/cfx_fontmapper.h"
#include "core/fxge/systemfontinfo_iface.h"
#include "core/fxge/cfx_folderfontinfo.h"
#include "core/fpdfapi/render/cpdf_rendercontext.h"
#include "core/fpdfapi/render/cpdf_renderstatus.h"
#include "core/fxge/cfx_defaultrenderdevice.h"
#include "core/fpdfapi/render/cpdf_imagerenderer.h"
#include "testing/utils/file_util.h"
#include "core/fpdfdoc/cpdf_annotlist.h"

#if _FX_PLATFORM_ != _FX_PLATFORM_WINDOWS_
#include <cstring>
#include <iconv.h>
#endif

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

class FileAccess final : public FPDF_FILEACCESS {
  public:
    explicit FileAccess(const std::string& file_name) {
        file_contents_ = GetFileContents(file_name.c_str(), &file_length_);
        if (!file_contents_)
            return;

        m_FileLen = static_cast<unsigned long>(file_length_);
        m_GetBlock = SGetBlock;
        m_Param = this;
    }

  private:
    int GetBlockImpl(unsigned long pos, unsigned char* pBuf, unsigned long size) {
        memcpy(pBuf, file_contents_.get() + pos, size);
        return size;
    }

    static int SGetBlock(void* param, unsigned long pos, unsigned char* pBuf, unsigned long size) {
        return static_cast<FileAccess*>(param)->GetBlockImpl(pos, pBuf, size);
    }

    size_t file_length_;
    std::unique_ptr<char, pdfium::FreeDeleter> file_contents_;
};

typedef std::vector<int> FPDF_TEXT_OBJ_CHARS_INDEX;
typedef std::map<CPDF_TextObject*, FPDF_TEXT_OBJ_CHARS_INDEX> FPDF_TEXT_OBJ_CHARS;

typedef struct _FPDF_PROCESS_FORM_OBJ_PARAM {
    CPDF_Page* pPage = nullptr;
    CPDF_TextPage* pTextPage = nullptr;
    bool bSaveGlyphs;
    int objIndex = -1;
    std::vector<int>* pPathsIndex = nullptr;
    std::vector<int>* pImagesIndex = nullptr;
    FPDF_PAGE_ITEM* pPageObj = nullptr;
    std::vector<std::vector<int>>* pTextsIndexVec = nullptr;
    FPDF_TEXT_OBJ_CHARS* pTextObjChars = nullptr;
} FPDF_PROCESS_FORM_OBJ_PARAM;

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

std::string decodeFontName(const ByteString &name)
{
#if _FX_PLATFORM_ == _FX_PLATFORM_WINDOWS_
    return std::string(name.c_str());
#else
    const char* data = name.c_str();
    bool all_ascii = true;
    for (size_t i = 0; i < name.GetLength(); i++) {
        if((unsigned char)*(data + i) > 127) {
            all_ascii = false;
            break;
        }
    }
    if(all_ascii)
        return std::string(name.c_str());

    iconv_t conv = iconv_open("UTF-8", "GBK");
    size_t inbytesleft = name.GetLength(), outbytesleft = name.GetLength() * 2;
    char *inbuf = const_cast<char*>(data);
    char *outbuf = new char[name.GetLength() * 2];
    char *out = outbuf;
    memset(outbuf, 0, name.GetLength() * 2);
    iconv(conv,
        &inbuf, &inbytesleft,
        &outbuf, &outbytesleft);

    std::string ret = std::string(out);
    delete[] out;
    return ret;
#endif
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
        textItem.familyName = decodeFontName(fxFont->GetFamilyName().IsEmpty() ? pdFont->GetBaseFont() : fxFont->GetFamilyName());
        textItem.faceName = decodeFontName(fxFont->GetFaceName().IsEmpty() || fxFont->GetFaceName() == "Untitled" ? pdFont->GetBaseFont() : fxFont->GetFaceName());
        textItem.bold = fxFont->IsBold();
        textItem.italic = fxFont->IsItalic();
        textItem.fontflags = pdFont->GetFontFlags();
        textItem.fontsize = charInfo.m_FontSize * charInfo.m_Matrix.GetXUnit();
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
    charItem.text = FPDF_WCharToString(charInfo.m_Unicode);
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

    pathItem.fillType = pPathObj->filltype();
    pathItem.isStroke = pPathObj->stroke();
    pathItem.width = pPathObj->m_GraphState.GetLineWidth();
    pathItem.isRect = pPathObj->path().IsRect();
    pPathObj->path().Transform(pPathObj->matrix());
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

    for (const FX_PATHPOINT& point : pPathObj->path().GetPoints()) {
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

void FPDF_RenderMaskedImage(RetainPtr<CFX_DIBitmap>& bitmap, CPDF_RenderStatus* pRenderStatus, CPDF_ImageLoader* pImgLoader,
                            const CFX_Matrix& matrix, const FXDIB_ResampleOptions& resampleOptions, int bitmapAlpha) {
    CPDF_ImageRenderer image_render;
    CFX_DefaultRenderDevice bitmap_device1;
    if (!bitmap_device1.Create(bitmap->GetWidth(), bitmap->GetHeight(), FXDIB_Rgb32, nullptr))
        return;
    bitmap_device1.GetBitmap()->Clear(0xffffff);
    CPDF_RenderStatus bitmap_render1(pRenderStatus->GetContext(), &bitmap_device1);
    if (image_render.Start(&bitmap_render1, bitmap, 0, 255, matrix,
                           resampleOptions, true, BlendMode::kNormal)) {
        image_render.Continue(nullptr);
    }

    CFX_DefaultRenderDevice bitmap_device2;
    if (!bitmap_device2.Create(bitmap->GetWidth(), bitmap->GetHeight(), FXDIB_8bppRgb, nullptr))
        return;
    CPDF_RenderStatus bitmap_render2(pRenderStatus->GetContext(), &bitmap_device2);
    if (image_render.Start(&bitmap_render2, pImgLoader->GetMask(), 0xffffffff, 255, matrix,
                           resampleOptions, true, BlendMode::kNormal)) {
        image_render.Continue(nullptr);
    }
    if (pImgLoader->MatteColor() != 0xffffffff) {
        int matte_r = FXARGB_R(pImgLoader->MatteColor());
        int matte_g = FXARGB_G(pImgLoader->MatteColor());
        int matte_b = FXARGB_B(pImgLoader->MatteColor());
        for (int row = 0; row < bitmap->GetHeight(); row++) {
            uint8_t* dest_scan = bitmap_device1.GetBitmap()->GetWritableScanline(row);
            const uint8_t* mask_scan = bitmap_device2.GetBitmap()->GetScanline(row);
            for (int col = 0; col < bitmap->GetWidth(); col++) {
                int alpha = *mask_scan++;
                if (!alpha) {
                    dest_scan += 4;
                    continue;
                }
                int orig = (*dest_scan - matte_b) * 255 / alpha + matte_b;
                *dest_scan++ = pdfium::clamp(orig, 0, 255);
                orig = (*dest_scan - matte_g) * 255 / alpha + matte_g;
                *dest_scan++ = pdfium::clamp(orig, 0, 255);
                orig = (*dest_scan - matte_r) * 255 / alpha + matte_r;
                *dest_scan++ = pdfium::clamp(orig, 0, 255);
                dest_scan++;
            }
        }
    }

    bitmap_device2.GetBitmap()->ConvertFormat(FXDIB_8bppMask);
    bitmap_device1.GetBitmap()->MultiplyAlpha(bitmap_device2.GetBitmap());
    if (bitmapAlpha < 255)
        bitmap_device1.GetBitmap()->MultiplyAlpha(bitmapAlpha);
    bitmap.Reset(nullptr);
    bitmap = bitmap_device1.GetBitmap()->Clone(nullptr);
}

void FPDF_RenderDIBBase(CPDF_Page* pPage, CPDF_ImageObject* imgObj, RetainPtr<CFX_DIBitmap>& bitmap) {
    if (pPage == nullptr || imgObj == nullptr || bitmap == nullptr)
        return;

    std::unique_ptr<CPDF_RenderContext> pContext = pdfium::MakeUnique<CPDF_RenderContext>(pPage);
    std::unique_ptr<CFX_DefaultRenderDevice> pDevice = pdfium::MakeUnique<CFX_DefaultRenderDevice>();
    pDevice->Attach(bitmap, false, nullptr, false);
    std::unique_ptr<CPDF_RenderStatus> pRenderStatus = pdfium::MakeUnique<CPDF_RenderStatus>(pContext.get(), pDevice.get());
    pRenderStatus->Initialize(nullptr, nullptr);

    CPDF_ImageLoader imgLoader;
    imgLoader.Start(imgObj,
                    pRenderStatus->GetContext()->GetPageCache(), true,
                    pRenderStatus->GetGroupFamily(),
                    pRenderStatus->GetLoadMask(), pRenderStatus.get());
    if (!imgLoader.GetBitmap())
        return;

    CFX_Matrix matrix(bitmap->GetWidth(), 0, 0, -bitmap->GetHeight(), 0, bitmap->GetHeight());
    FXDIB_ResampleOptions resampleOptions = FXDIB_ResampleOptions();
    CPDF_GeneralState& state = imgObj->m_GeneralState;
    int bitmapAlpha = FXSYS_round(255 * state.GetFillAlpha());
    if (imgLoader.GetMask()) {
        FPDF_RenderMaskedImage(bitmap, pRenderStatus.get(), &imgLoader, matrix, resampleOptions, bitmapAlpha);
        return;
    }

    FXDIB_Format format = bitmap->GetFormat();
    if (format != FXDIB_8bppRgb && format != FXDIB_8bppMask)
        return;

    if (!imgObj->GetImage()->IsMask()) {
        bitmap->ConvertFormat(FXDIB_Rgb);
        return;
    }

    // Fill background
    int alpha = pPage->BackgroundAlphaNeeded() ? 1 : 0;
    FPDF_DWORD fill_color = alpha ? 0x00000000 : 0xFFFFFFFF;
    FPDFBitmap_FillRect(FPDFBitmapFromCFXDIBitmap(bitmap.Get()), 0, 0, bitmap->GetWidth(), bitmap->GetHeight(), fill_color);

    RetainPtr<CFX_DIBBase> pDIBBase = imgLoader.GetBitmap();
    if (pDIBBase->IsAlphaMask()) {
        FX_ARGB fillArgb = pRenderStatus->GetFillArgb(imgObj);
        std::unique_ptr<CFX_ImageRenderer> m_DeviceHandle;
        pRenderStatus->GetRenderDevice()->StartDIBitsWithBlend(pDIBBase, bitmapAlpha, fillArgb, matrix,
                                                               resampleOptions, &m_DeviceHandle, BlendMode::kNormal);
        if (m_DeviceHandle) {
            pRenderStatus->GetRenderDevice()->ContinueDIBits(m_DeviceHandle.get(), nullptr);
        }
    }
}

void FPDF_ExtractImageData(CPDF_Page* pPage, CPDF_ImageObject* imgObj, std::vector<unsigned char>& png_encoding) {
    if (imgObj == nullptr)
        return;
    RetainPtr<CPDF_Image> pImg = imgObj->GetImage();
    if (pImg == nullptr)
        return;

    RetainPtr<CFX_DIBBase> pSource = pImg->LoadDIBBase();
    if (pSource == nullptr)
        return;

    RetainPtr<CFX_DIBitmap> pBitmap;
    if (pSource->GetBPP() == 1)
        pBitmap = pSource->CloneConvert(FXDIB_8bppRgb);
    else
        pBitmap = pSource->Clone(nullptr);

    FPDF_RenderDIBBase(pPage, imgObj, pBitmap);
    FXDIB_Format format = pBitmap->GetFormat();
    int width = pBitmap->GetWidth();
    int height = pBitmap->GetHeight();
    int stride = pBitmap->GetPitch();
    const unsigned char* buffer = static_cast<const unsigned char*>(pBitmap->GetBuffer());
    switch (format) {
        case FXDIB_8bppRgb:
        case FXDIB_8bppMask:
            png_encoding = image_diff_png::EncodeGrayPNG(buffer, width, height, stride);
            break;
        case FXDIB_Rgb:
            png_encoding = image_diff_png::EncodeBGRPNG(buffer, width, height, stride);
            break;
        case FXDIB_Rgb32:
            png_encoding = image_diff_png::EncodeBGRAPNG(buffer, width, height, stride, true);
            break;
        case FXDIB_Argb:
            png_encoding = image_diff_png::EncodeBGRAPNG(buffer, width, height, stride, false);
            break;
        default:
            fprintf(stderr, "Image object has a bitmap of unknown format.\n");
    }
}

void FPDF_GetImageItem(CPDF_ImageObject* pImageObj, FPDF_IMAGE_ITEM& imageItem, CPDF_Page* pPage, bool saveImages) {
    FPDF_GetPageObjectBBox(pImageObj, imageItem.bbox);
    // image clip box
    CFX_FloatRect rect;
    if (pImageObj->m_ClipPath.HasRef())
        rect = pImageObj->m_ClipPath.GetClipBox();
    imageItem.clipBox.left = rect.left;
    imageItem.clipBox.top = rect.top;
    imageItem.clipBox.right = rect.right;
    imageItem.clipBox.bottom = rect.bottom;

    FPDF_GetColor(pImageObj, imageItem.fillColor, imageItem.strokeColor);
    if (saveImages) {
        FPDF_ExtractImageData(pPage, pImageObj, imageItem.png_encoding);
    }
}

void FPDF_ExtractCharGlyph(FPDF_CHAR_INFO& charInfo, std::string& glyph)
{
    bool bHasFont = charInfo.m_pTextObj && charInfo.m_pTextObj->GetFont();
    if (!bHasFont)
        return;
    CPDF_Font* font = charInfo.m_pTextObj->GetFont();
    bool bVert = false;
    int glyph_index = font->GlyphFromCharCode(charInfo.m_Charcode, &bVert);
    CFX_Font* cfx_font = font->GetFont();
    if (nullptr == cfx_font)
        return;

    uint32_t glyph_width = cfx_font->GetGlyphWidth(glyph_index);
    const CFX_PathData* path_data = cfx_font->LoadGlyphPath(glyph_index, glyph_width);
    if (nullptr == path_data)
        return;

    const std::vector<FX_PATHPOINT>& fx_path = path_data->GetPoints();
    if (fx_path.size() < 1)
        return;

    char format[11] = {0};
    char tmp[25] = {0};
    int c_counts = 0;
    char type = 0;
    for (auto& p : fx_path) {
        switch (p.m_Type) {
            case FXPT_TYPE::LineTo:
                type = 'L';
                break;
            case FXPT_TYPE::MoveTo:
                type = 'M';
                break;
            case FXPT_TYPE::BezierTo:
                type = 'C';
                break;
            default:
                type = 'E';
                break;
        }
        if (type == 'C') {
            ++c_counts;
            if (c_counts == 1) {
                memcpy(format, "%c %f %f, ", 11);
            }
            else if (c_counts == 2) {
                memcpy(format, "%f %f, ", 8);
            }
            else if (c_counts == 3) {
                memcpy(format, "%f %f ", 7);
            }
        } else {
            memcpy(format, "%c %f %f ", 10);
        }
        float tmp_y = -p.m_Point.y;
        if (c_counts < 2) {
            sprintf(tmp, format, type, p.m_Point.x, tmp_y);
        } else {
            sprintf(tmp, format, p.m_Point.x, tmp_y);
            if (c_counts == 3)
                c_counts = 0;
        }
        int tmp_len = strlen(tmp);
        if (p.m_CloseFigure) {
            tmp[tmp_len] = 'Z';
            tmp[tmp_len + 1] = ' ';
            tmp[tmp_len + 2] = 0;
            tmp_len += 2;
        }
        glyph += tmp;
    }
}

void FPDF_GenGlyphKey(std::string& faceName, std::string text, std::string& key) {
    size_t pos = faceName.find('+');
    std::string fontName;
    if (pos != std::string::npos) {
        fontName = std::string(faceName, pos + 1);
    } else {
        fontName = faceName;
    }
    if (text.size() < 1)
        return;
    key = fontName + "-" + text;
}

void FPDF_SaveGlyphs(FPDF_CHAR_INFO& charInfo, FPDF_CHAR_ITEM& charItem, FPDF_PAGE_ITEM& pageObj, std::string& faceName) {
    if (faceName.size() < 1)
        return;
    std::string key;
    FPDF_GenGlyphKey(faceName, charItem.text, key);
    if (key.size() < 1)
        return;
    if (pageObj.glyphs.count(key) > 0)
        return;
    std::string glyph;
    FPDF_ExtractCharGlyph(charInfo, glyph);
    if (glyph.size() < 1)
        return;
    pageObj.glyphs[key] = glyph;
}

void FPDF_ProcessShadingObject(
    CPDF_ShadingObject* pShadingObj,
    const CFX_Matrix& formMatrix,
    std::vector<CPDF_ShadingObject*>& shadings) {
    pShadingObj->Transform(formMatrix);
    shadings.push_back(pShadingObj);
    return;
}

void FPDF_ProcessPathObject(
    CPDF_PathObject* pPathObj,
    const CFX_Matrix& formMatrix,
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
    std::vector<CPDF_ImageObject*>& images) {
    pImageObj->Transform(formMatrix);
    if (pImageObj->m_ClipPath.HasRef()) {
        pImageObj->m_ClipPath.Transform(formMatrix);
    }
    images.push_back(pImageObj);
    return;
}

void FPDF_InitTextItem(FPDF_TEXT_ITEM& textItem, CPDF_Page* pPage, FPDF_CHAR_INFO& charInfo) {
    if (pPage == nullptr)
        return;
    FPDF_GetTextStyle(charInfo, textItem);
    CPDF_TextObject* pTextObj = charInfo.m_pTextObj ? charInfo.m_pTextObj.Get() : nullptr;
    FPDF_GetPageObjectBBox(pTextObj, textItem.bbox, pPage);
    FPDF_GetPageObjectClipBox(pTextObj, textItem.clipBox, pPage);
    FPDF_GetColor(pTextObj, textItem.fillColor, textItem.strokeColor);
}

inline bool FPDF_IsWatermarkText(FPDF_CHAR_INFO& charInfo) {
    if ((fabs(charInfo.m_Matrix.b * 1000) < 1 && fabs(charInfo.m_Matrix.c * 1000) < 1) ||
        charInfo.m_Matrix.Is90Rotated()) {
        return false;
    }
    return fabs((charInfo.m_Matrix.b + charInfo.m_Matrix.c) * 1000) < 1;
}

int FPDF_GetTextRotation(CPDF_TextObject* pTextObj) {
    // counterclockwise rotation angle
    double a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
    std::tie(a, b, c, d, e, f) = pTextObj->GetTextMatrix().AsTuple();
    if (fabs(b * 1000) < fabs(a) && fabs(c * 1000) < fabs(d)) {
        return a > 0 ? 0 : 180;
    }
    if (fabs(a * 1000) < fabs(b) && fabs(d * 1000) < fabs(c)) {
        return b > 0 ? 90 : 270;
    }
    return int(acos(a) * 180 / M_PI);
}

void FPDF_CountTextsRotation(CPDF_TextObject* pTextObj, std::vector<int>& result) {
    int rotation = FPDF_GetTextRotation(pTextObj);
    if (rotation % 90 != 0) {
        return;
    }
    result[rotation / 90] += 1;
}

int FPDF_CalcPageTextRotation(std::vector<int>& result) {
    return max_element(result.begin(), result.end()) - result.begin();
}

int FPDF_CalcPageRotation(CPDF_Page* pPage, std::vector<int>& result) {
    return ((pPage->GetPageRotation() - FPDF_CalcPageTextRotation(result)) * 90 + 360) % 360;
}

bool FPDF_ProcessTextObject(
    CPDF_Page* pPage,
    CPDF_TextPage* textPage,
    FPDF_PAGE_ITEM& pageObj,
    CPDF_TextObject* textObj,
    std::vector<std::vector<int>>& texts_index_vec,
    FPDF_TEXT_OBJ_CHARS& text_obj_chars,
    bool saveGlyphs) {
    if (pPage == nullptr || textPage == nullptr || textObj == nullptr || text_obj_chars.count(textObj) == 0)
        return false;

    FPDF_TEXT_OBJ_CHARS_INDEX& chars_indices = text_obj_chars[textObj];
    wchar_t spHigh = 0;
    bool creat_text_item = true;
    for (int &char_idx : chars_indices) {
        FPDF_CHAR_ITEM charItem;
        FPDF_CHAR_INFO charInfo;
        std::string faceName;
        textPage->GetCharInfo(char_idx, &charInfo);
        FPDF_GetCharItem(charInfo, charItem, pPage);
        if (charInfo.m_Unicode >= 0xD800 && charInfo.m_Unicode <= 0xDBFF) { // high surrogate
            spHigh = charInfo.m_Unicode;
            continue;
        } else if (charInfo.m_Unicode >= 0xDC00 && charInfo.m_Unicode <= 0xDFFF) { // low surrogate
            if (spHigh >= 0xD800 && spHigh <= 0xDBFF) {
                charItem.text = FPDF_SurrogatePairToString(spHigh, charInfo.m_Unicode);
            } else {
                // bad sequence
            }
            spHigh = 0;
        }

        if (creat_text_item) {
            FPDF_TEXT_ITEM textItem;
            FPDF_InitTextItem(textItem, pPage, charInfo);
            textItem.chars.push_back(charItem);
            pageObj.texts.push_back(textItem);
            faceName = textItem.faceName;
            creat_text_item = false;
        } else {
            FPDF_TEXT_ITEM &textItem = pageObj.texts.back();
            textItem.chars.push_back(charItem);
            faceName = textItem.faceName;
        }
        if (saveGlyphs) {
            FPDF_SaveGlyphs(charInfo, charItem, pageObj, faceName);
        }
    }

    if (chars_indices.size() > 0) {
        texts_index_vec.push_back(chars_indices);
        return true;
    }
    return false;
}

void FPDF_FillPageTexts(CPDF_Page* pPage, CPDF_TextPage* textPage, FPDF_PAGE_ITEM& pageObj, std::vector<std::vector<int>>& textsIndexVec, bool saveGlyphs) {
    if (textsIndexVec.empty())
        return;
    std::sort(std::begin(textsIndexVec), std::end(textsIndexVec),
        [](const std::vector<int>& v1, const std::vector<int>& v2) {
            return v1[0] < v2[0];
        }
    );
    int processed_char_counts = 0;
    for (auto& vec : textsIndexVec) {
        processed_char_counts += vec.size();
    }
    if (processed_char_counts == textPage->CountChars())
        return;
    int insert_index = 0;
    for (size_t i = 0; i < textsIndexVec.size() - 1; ++i) {
        bool insert_item = false;
        insert_index += 1;
        int start_index = textsIndexVec[i][textsIndexVec[i].size() - 1];
        int end_index = textsIndexVec[i + 1][0];
        int size = end_index - start_index;
        if (size > 1) {
            wchar_t spHigh = 0;
            for (int index = start_index + 1; index < end_index; ++index) {
                FPDF_CHAR_ITEM charItem;
                FPDF_CHAR_INFO charInfo;
                std::string faceName;
                textPage->GetCharInfo(index, &charInfo);
                // remove watermark texts
                if (FPDF_IsWatermarkText(charInfo))
                    continue;
                FPDF_GetCharItem(charInfo, charItem, pPage);
                if (charInfo.m_Unicode >= 0xD800 && charInfo.m_Unicode <= 0xDBFF) { // high surrogate
                    spHigh = charInfo.m_Unicode;
                    continue;
                } else if (charInfo.m_Unicode >= 0xDC00 && charInfo.m_Unicode <= 0xDFFF) { // low surrogate
                    if (spHigh >= 0xD800 && spHigh <= 0xDBFF) {
                        charItem.text = FPDF_SurrogatePairToString(spHigh, charInfo.m_Unicode);
                    } else {
                        // bad sequence
                    }
                    spHigh = 0;
                }
                if (index == start_index + 1) {
                    FPDF_TEXT_ITEM textItem;
                    FPDF_InitTextItem(textItem, pPage, charInfo);
                    textItem.chars.push_back(charItem);
                    textItem.z_index = pageObj.texts[insert_index - 1].z_index;
                    pageObj.texts.insert(pageObj.texts.begin() + insert_index, textItem);
                    faceName = textItem.faceName;
                    insert_item = true;
                } else {
                    FPDF_TEXT_ITEM &textItem = pageObj.texts[insert_index];
                    textItem.chars.push_back(charItem);
                    faceName = textItem.faceName;
                }
                if (saveGlyphs) {
                    FPDF_SaveGlyphs(charInfo, charItem, pageObj, faceName);
                }
            }
            if (insert_item)
                ++insert_index;
        }
    }
}

void FPDF_ProcessFormObject(
    CPDF_FormObject* pFormObj,
    const CFX_Matrix& formMatrix,
    std::vector<CPDF_PathObject*>& paths,
    std::vector<CPDF_ImageObject*>& images,
    std::vector<CPDF_ShadingObject*>& shadings,
    FPDF_PROCESS_FORM_OBJ_PARAM& form_param) {
    const CPDF_Form* pObjectHolder = pFormObj->form();
    if (pObjectHolder->begin() == pObjectHolder->end())
        return;

    CFX_Matrix curFormMatrix = pFormObj->form_matrix();
    curFormMatrix.Concat(formMatrix);

    for (auto it = pObjectHolder->begin(); it != pObjectHolder->end(); ++it) {
        CPDF_PageObject* pPageObj = it->get();
        if (!pPageObj)
            continue;
        if (pPageObj->IsPath()) {
            FPDF_ProcessPathObject(pPageObj->AsPath(), curFormMatrix, paths);
            (form_param.pPathsIndex)->push_back(form_param.objIndex);
        }
        else if (pPageObj->IsImage()) {
            FPDF_ProcessImageObject(pPageObj->AsImage(), curFormMatrix, images);
            (form_param.pImagesIndex)->push_back(form_param.objIndex);
        }
        else if (pPageObj->IsForm())
            FPDF_ProcessFormObject(pPageObj->AsForm(), curFormMatrix, paths, images, shadings, form_param);
        else if (pPageObj->IsText()) {
            if (!FPDF_ProcessTextObject(form_param.pPage, form_param.pTextPage, *(form_param.pPageObj), pPageObj->AsText(),
                                        *(form_param.pTextsIndexVec), *(form_param.pTextObjChars), form_param.bSaveGlyphs))
                continue;
            FPDF_TEXT_ITEM &textItem = (form_param.pPageObj)->texts.back();
            textItem.z_index = form_param.objIndex;
        }
        // else if (pPageObj->IsShading())
        //     FPDF_ProcessShadingObject(pPageObj->AsShading(), curFormMatrix, shadings);
    }
}

void FPDF_GenTextObjectChars(CPDF_TextPage* pTextPage, FPDF_TEXT_OBJ_CHARS& text_obj_chars) {
    if (pTextPage == nullptr)
        return;
    for (int i = 0; i < pTextPage->CountChars(); ++i) {
        FPDF_CHAR_INFO charInfo;
        pTextPage->GetCharInfo(i, &charInfo);
        // remove watermark texts
        if (FPDF_IsWatermarkText(charInfo))
            continue;
        CPDF_TextObject* pTextObj = charInfo.m_pTextObj.Get();
        if (pTextObj == nullptr)
            continue;

        wchar_t spHigh = 0;
        if (charInfo.m_Unicode >= 0xD800 && charInfo.m_Unicode <= 0xDBFF) { // high surrogate
            spHigh = charInfo.m_Unicode;
            FPDF_CHAR_INFO nextCharInfo;
            pTextPage->GetCharInfo(i + 1, &nextCharInfo);
            if (nextCharInfo.m_Unicode < 0xDC00 || nextCharInfo.m_Unicode > 0xDFFF)
                // not low surrogate
                continue;
        }

        if (text_obj_chars.count(pTextObj) == 0) {
            text_obj_chars[pTextObj] = FPDF_TEXT_OBJ_CHARS_INDEX(1, i);
        } else {
            text_obj_chars[pTextObj].push_back(i);
        }

        if (spHigh != 0)
            text_obj_chars[pTextObj].push_back(++i);
    }
}

inline bool FPDF_IsUnUsedBackgroundText(std::map<std::string, int>& char_counts, unsigned int color_value) {
    // 0xfff6e0ff(rgba): the color of special background chars
    // "\xe2\x80\xa2": the utf-8 encode of '•'
    if (char_counts.size() != 1 || color_value != 0xfff6e0ff || char_counts.count("\xe2\x80\xa2") == 0)
        return false;
    return true;
}

void FPDF_RemoveUnUsedBackgroundText(FPDF_PAGE_ITEM& pageObj) {
    if (pageObj.texts.empty())
        return;
    std::vector<FPDF_TEXT_ITEM>::iterator page_texts_begin = pageObj.texts.begin();
    std::vector<FPDF_TEXT_ITEM>::iterator erase_end_it = page_texts_begin;
    for (auto it = page_texts_begin; it != pageObj.texts.end(); ++it) {
        FPDF_COLOR& text_color = (*it).fillColor;
        unsigned int text_color_value = text_color.r << 24 | text_color.g << 16 |
                                        text_color.b << 8 | text_color.a;
        std::map<std::string, int> char_counts;
        for (auto& char_item : (*it).chars) {
            if (char_item.text.compare(" ") == 0 || char_item.text.compare("\r") == 0 ||
                char_item.text.compare("\n") == 0)
                continue;
            if (char_counts.count(char_item.text) == 0) {
                char_counts.insert({char_item.text, 1});
            } else {
                char_counts[char_item.text] += 1;
            }
        }
        if (char_counts.empty())
            continue;

        if (!FPDF_IsUnUsedBackgroundText(char_counts, text_color_value)) {
            if (erase_end_it != page_texts_begin)
                erase_end_it = it;
            break;
        }
        erase_end_it = it;
    }
    if (erase_end_it != page_texts_begin)
        pageObj.texts.erase(page_texts_begin, erase_end_it);
}

void FPDF_ProcessObject(CPDF_Page* pPage, FPDF_PAGE_ITEM& pageObj, bool saveGlyphs, bool saveImages) {
    if (pPage->begin() == pPage->end())
        return;
    CPDF_ViewerPreferences viewRef(pPage->GetDocument());
    CPDF_TextPage* textPage = new CPDF_TextPage(
        pPage, viewRef.IsDirectionR2L() ? FPDFText_Direction::Right : FPDFText_Direction::Left);
    textPage->setNeedTransformClipPath(true);
    textPage->ParseTextPage();

    std::vector<int> paths_index;
    std::vector<int> images_index;
    std::vector<std::vector<int>> texts_index_vec;
    std::vector<int> texts_rotation_vec(4, 0);

    FPDF_TEXT_OBJ_CHARS text_obj_chars;
    FPDF_GenTextObjectChars(textPage, text_obj_chars);

    int obj_index = 0;
    FPDF_PROCESS_FORM_OBJ_PARAM form_obj_param;
    form_obj_param.pPage = pPage;
    form_obj_param.pTextPage = textPage;
    form_obj_param.bSaveGlyphs = saveGlyphs;
    form_obj_param.objIndex = obj_index;
    form_obj_param.pPathsIndex = &paths_index;
    form_obj_param.pImagesIndex = &images_index;
    form_obj_param.pPageObj = &pageObj;
    form_obj_param.pTextsIndexVec = &texts_index_vec;
    form_obj_param.pTextObjChars = &text_obj_chars;

    CFX_Matrix matrix;
    FPDF_GetPageMatrix(pPage, matrix);
    std::vector<CPDF_PathObject*> paths;
    std::vector<CPDF_ImageObject*> images;
    std::vector<CPDF_ShadingObject*> shadings;
    for (auto it = pPage->begin(); it != pPage->end(); ++it, ++obj_index) {
        CPDF_PageObject* pObj = it->get();
        if (!pObj)
            continue;
        if (pObj->IsPath()) {
            paths_index.push_back(obj_index);
            FPDF_ProcessPathObject(pObj->AsPath(), matrix, paths);
        }
        else if (pObj->IsImage()) {
            images_index.push_back(obj_index);
            FPDF_ProcessImageObject(pObj->AsImage(), matrix, images);
        }
        else if (pObj->IsForm()) {
            form_obj_param.objIndex = obj_index;
            FPDF_ProcessFormObject(
                pObj->AsForm(), matrix, paths, images, shadings, form_obj_param);
        }
        else if (pObj->IsText()) {
            if (!FPDF_ProcessTextObject(pPage, textPage, pageObj, pObj->AsText(),
                                        texts_index_vec, text_obj_chars, saveGlyphs))
                continue;
            FPDF_TEXT_ITEM &textItem = pageObj.texts.back();
            textItem.z_index = obj_index;
            FPDF_CountTextsRotation(pObj->AsText(), texts_rotation_vec);
        }
        // else if (pObj->IsShading())
        //     FPDF_ProcessShadingObject(pObj->AsShading(), matrix, shadings);
    }

    pageObj.page_rotation = FPDF_CalcPageRotation(pPage, texts_rotation_vec);
    FPDF_FillPageTexts(pPage, textPage, pageObj, texts_index_vec, saveGlyphs);
    FPDF_RemoveUnUsedBackgroundText(pageObj);
    obj_index = 0;
    for (CPDF_PathObject* obj : paths) {
        FPDF_PATH_ITEM pathItem;
        pathItem.z_index = paths_index[obj_index];
        FPDF_GetPathItem(obj, pathItem);
        pageObj.paths.push_back(pathItem);
        obj_index += 1;
    }
    obj_index = 0;
    for (CPDF_ImageObject* obj : images) {
        FPDF_IMAGE_ITEM imageItem;
        imageItem.z_index = images_index[obj_index];
        FPDF_GetImageItem(obj, imageItem, pPage, saveImages);
        pageObj.images.push_back(imageItem);
        obj_index += 1;
    }

    delete textPage;
}

std::string FPDF_WStringToString(const std::wstring& src)
{
    CFX_UTF8Encoder encoder;
    for(auto c : src) {
        encoder.Input(c);
    }
    ByteStringView result = encoder.GetResult();
    return std::string(result.unterminated_c_str(), result.GetLength());
}

std::string FPDF_WCharToString(const wchar_t wc)
{
    wchar_t tmp[2] = {0};
    tmp[0] = wc;
    return FPDF_WStringToString(tmp);
}

std::string FPDF_SurrogatePairToString(wchar_t h, wchar_t l) {
    int codePoint = (((h - 0xD800) << 10) | (l - 0xDC00)) + 0x10000;
    CFX_UTF8Encoder encoder;
    encoder.Input(codePoint);
    ByteStringView result = encoder.GetResult();
    return std::string(result.unterminated_c_str(), result.GetLength());
}

FPDF_EXPORT void FPDF_CALLCONV
FPDF_LoadPageObject(FPDF_PAGE page, FPDF_PAGE_ITEM& pageObj, bool saveGlyphs, bool saveImages) {
    CPDF_Page* pPDFPage = CPDFPageFromFPDFPage(page);
    if (!pPDFPage)
        return;
    pPDFPage->ParseContent();
    // const CPDF_PageObjectList* pPageObjectList = pPDFPage->GetPageObjectList();

    pageObj.size.width = pPDFPage->GetPageWidth();
    pageObj.size.height = pPDFPage->GetPageHeight();
    pageObj.rotation = pPDFPage->GetPageRotation();

    FPDF_ProcessObject(pPDFPage, pageObj, saveGlyphs, saveImages);
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

std::string convertWStringToString(const std::wstring src) {
    wchar_t spHigh = 0;
    std::string ret;
    for (auto c : src) {
        std::string cc = FPDF_WCharToString(c);
        if (c >= 0xD800 && c <= 0xDBFF) { // high surrogate
            spHigh = c;
            continue;
        } else if (c >= 0xDC00 && c <= 0xDFFF) { // low surrogate
            if (spHigh >= 0xD800 && spHigh <= 0xDBFF) {
                cc = FPDF_SurrogatePairToString(spHigh, c);
            } else {
                // bad sequence
            }
            spHigh = 0;
        }
        ret += cc;
    }
    return ret;
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
        item.title = convertWStringToString(cbookmark.GetTitle().c_str());

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
    png_encoding = image_diff_png::EncodeBGRAPNG(buffer, width, height, stride, false);
    if (png_encoding.empty()) {
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
    png_encoding = image_diff_png::EncodeBGRAPNG(buffer, width, height, stride, false);
    if (png_encoding.empty()) {
        fprintf(stderr, "Failed to convert bitmap to PNG\n");
        return false;
    }
    return true;
}

void FPDF_RenderWidgetAnnotation(FPDF_PAGE page, FPDF_BITMAP bitmap, int width, int height) {
    CPDF_Page* pPage = CPDFPageFromFPDFPage(page);
    if (!pPage)
        return;
    auto pOwnedList = pdfium::MakeUnique<CPDF_AnnotList>(pPage);
    CPDF_AnnotList* pList = pOwnedList.get();
    bool bIncludeWidget = false;
    for (const auto& pAnnot : pList->All()) {
        if (pAnnot->GetSubtype() == CPDF_Annot::Subtype::WIDGET) {
            bIncludeWidget = true;
            break;
        }
    }
    if (!bIncludeWidget)
        return;

    FPDF_DOCUMENT document = FPDFDocumentFromCPDFDocument(pPage->GetDocument());

    FPDF_FORMFILLINFO form_fill_info = {};
#ifdef PDF_ENABLE_XFA
    form_fill_info.version = 2;
#else   // PDF_ENABLE_XFA
    form_fill_info.version = 1;
#endif  // PDF_ENABLE_XFA

    ScopedFPDFFormHandle form_handle(FPDFDOC_InitFormFillEnvironment(document, &form_fill_info));
    FPDF_FFLDraw(form_handle.get(), bitmap, page, 0, 0, width, height, 0, 0);
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
        FPDF_RenderWidgetAnnotation(page, bitmap.get(), width, height);

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
        FPDF_RenderWidgetAnnotation(page, bitmap.get(), width, height);

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
FPDF_ExtractPageContents(FPDF_PAGE page, uint32_t& length)
{
    CPDF_Page* pPDFPage = CPDFPageFromFPDFPage(page);
    if (nullptr == pPDFPage)
        return nullptr;
    if (nullptr == pPDFPage->GetDocument() || nullptr == pPDFPage->GetDict())
        return nullptr;

    CPDF_Object* pContent = pPDFPage->GetDict()->GetDirectObjectFor(pdfium::page_object::kContents);
    if (nullptr == pContent)
        return nullptr;

    uint8_t* data = nullptr;
    if (pContent->IsStream()) {
        CPDF_Stream* pStream = pContent->AsStream();
        if (pStream != nullptr) {
            RetainPtr<CPDF_StreamAcc> single_stream;
            single_stream = pdfium::MakeRetain<CPDF_StreamAcc>(pStream);
            single_stream->LoadAllDataFiltered();
            length = single_stream->GetSize();
            uint8_t* temp = single_stream->GetData();
            if (temp == nullptr)
                return nullptr;
            data = new uint8_t[length];
            memcpy(data, temp, length);
        }
    } else if (pContent->IsArray()) {
        CPDF_Array* pArray = pContent->AsArray();
        if (pArray != nullptr) {
            uint32_t stream_counts = pArray->size();
            if (stream_counts == 0)
                return nullptr;
            std::vector<RetainPtr<CPDF_StreamAcc>> stream_array;
            stream_array.resize(stream_counts);
            FX_SAFE_UINT32 safe_size = 0;
            for (size_t index = 0; index < stream_counts; ++index) {
                CPDF_Stream* pStreamObj = ToStream(pArray->GetDirectObjectAt(index));
                stream_array[index] = pdfium::MakeRetain<CPDF_StreamAcc>(pStreamObj);
                stream_array[index]->LoadAllDataFiltered();
                safe_size += stream_array[index]->GetSize();
                if (!safe_size.IsValid()) {
                    stream_array.clear();
                    return nullptr;
                }
            }
            length = safe_size.ValueOrDie();
            data = new uint8_t[length];
            uint32_t pos = 0;
            for (const auto& stream : stream_array) {
                memcpy(data + pos, stream->GetData(), stream->GetSize());
                pos += stream->GetSize();
            }
            stream_array.clear();
        }
    }

    return data;
}

FPDF_EXPORT uint8_t* FPDF_CALLCONV
FPDF_ExtractPageImageResources(FPDF_PAGE page, uint32_t& length)
{
    CPDF_Page* pPDFPage = CPDFPageFromFPDFPage(page);
    if (nullptr == pPDFPage)
        return nullptr;
    if (nullptr == pPDFPage->GetDocument() || nullptr == pPDFPage->GetDict())
        return nullptr;
    CPDF_Dictionary* pRes = pPDFPage->GetDict()->GetDictFor(pdfium::page_object::kResources);
    if (nullptr == pRes)
        return nullptr;
    CPDF_Dictionary* pPageXObject = pRes->GetDictFor("XObject");
    if (nullptr == pPageXObject)
        return nullptr;
    std::vector<ByteString> keys = pPageXObject->GetKeys();
    if (keys.size() == 0)
        return nullptr;
    std::sort(keys.begin(), keys.end());
    std::vector<RetainPtr<CPDF_StreamAcc>> stream_vec;
    FX_SAFE_UINT32 safe_size = 0;
    for (auto& key : keys) {
        CPDF_Object* pObj = pPageXObject->GetDirectObjectFor(key);
        if (pObj->IsStream()) {
            CPDF_Stream* pStream = pObj->AsStream();
            if (pStream == nullptr || pStream->GetDict()->GetStringFor("Subtype") != "Image")
                continue;
            RetainPtr<CPDF_StreamAcc> stream = pdfium::MakeRetain<CPDF_StreamAcc>(pStream);
            stream_vec.push_back(stream);
            stream->LoadAllDataFiltered();
            safe_size += stream->GetSize();
            if (!safe_size.IsValid()) {
                stream_vec.clear();
                return nullptr;
            }
        }
    }
    length = safe_size.ValueOrDie();
    if (length == 0)
        return nullptr;

    uint8_t* data = new uint8_t[length];
    uint32_t pos = 0;
    for (const auto& stream : stream_vec) {
        memcpy(data + pos, stream->GetData(), stream->GetSize());
        pos += stream->GetSize();
    }
    stream_vec.clear();
    return data;
}

void FPDF_GetSystemFonts(std::map<std::string, std::string> &fonts) {
    CFX_GEModule *module = CFX_GEModule::Get();
    CFX_FontMgr *mgr = module->GetFontMgr();
    CFX_FontMapper *mapper = mgr->GetBuiltinMapper();
    auto *info = mapper->GetSystemFontInfo();
    fonts = ((CFX_FolderFontInfo*)info)->GetFontList();
}

FPDF_EXPORT
void FPDF_SetPageOpaqueData(FPDF_DOCUMENT document, int index, std::string key, std::string value) {
    // CPDF_Document* doc = CPDFDocumentFromFPDFDocument(document);
    FPDF_PAGE fpage = FPDF_LoadPage(document, index);
    CPDF_Page* cpage = CPDFPageFromFPDFPage(fpage);
    CPDF_Dictionary* res = cpage->GetDict()->GetDictFor(pdfium::page_object::kResources);
    res->SetNewFor<CPDF_String>(key.c_str(), value.c_str(), false);
    FPDFPage_GenerateContent(fpage);
}

FPDF_EXPORT
std::string FPDF_GetPageOpaqueData(FPDF_DOCUMENT document, int index, std::string key) {
    FPDF_PAGE fpage = FPDF_LoadPage(document, index);
    CPDF_Page* cpage = CPDFPageFromFPDFPage(fpage);
    CPDF_Dictionary* res = cpage->GetDict()->GetDictFor(pdfium::page_object::kResources);
    ByteString ret = res->GetStringFor(ByteString(key.c_str(), key.size()));
    if (ret.IsEmpty()) return "";
    return std::string(ret.c_str(), ret.GetLength());
}

FPDF_EXPORT
void FPDF_SetGlobalOpaqueData(FPDF_DOCUMENT document, std::string key, std::string value) {
    key = "__GLOBAL__::" + key;
    FPDF_SetPageOpaqueData(document, 0, key, value);
}

FPDF_EXPORT
std::string FPDF_GetGlobalOpaqueData(FPDF_DOCUMENT document, std::string key) {
    key = "__GLOBAL__::" + key;
    return FPDF_GetPageOpaqueData(document, 0, key);
}

FPDF_EXPORT
FPDF_BOOL FPDF_CreatePDFDocumentFromImages(std::vector<std::string>& images_path,
                                           std::vector<int>& pages_width,
                                           std::vector<int>& pages_height,
                                           const char* file_path)
{
    if (file_path == nullptr || images_path.empty() || pages_width.empty() || pages_height.empty())
        return false;
    FPDF_DOCUMENT new_document = FPDF_CreateNewDocument();
    if (new_document == nullptr)
        return false;
    size_t page_counts = images_path.size();
    FileAccess** files = new FileAccess* [page_counts];
    if (files == nullptr)
        return false;
    FPDF_BOOL ret = false;
    size_t i = 0;
    for (; i < page_counts; ++i) {
        files[i] = new FileAccess(images_path[i]);
    }
    i = 0;
    for (; i < page_counts; ++i) {
        FPDF_PAGE new_page = FPDFPage_New(new_document, i, pages_width[i], pages_height[i]);
        if (new_page == nullptr)
            break;
        FPDF_PAGEOBJECT image_obj = FPDFPageObj_NewImageObj(new_document);
        if (image_obj == nullptr) {
            FPDF_ClosePage(new_page);
            break;
        }
        FPDF_BOOL load_ret = FPDFImageObj_LoadJpegFile(&new_page, 1, image_obj, files[i]);
        if (load_ret) {
            FPDFPage_InsertObject(new_page, image_obj);
            FPDFPageObj_Transform(image_obj, pages_width[i], 0, 0, pages_height[i], 0, 0);
            FPDFPage_GenerateContent(new_page);
        }
        FPDF_ClosePage(new_page);
        if (!load_ret)
            break;
    }
    if (i == page_counts) {
        FPDF_SaveDocument(new_document, file_path);
        ret = true;
    }

    i = 0;
    for (; i < page_counts; ++i) {
        if (files[i] != nullptr) {
            delete files[i];
            files[i] = nullptr;
        }
    }
    delete [] files;
    files = nullptr;
    FPDF_CloseDocument(new_document);
    return ret;
}

FPDF_EXPORT
std::string FPDF_GetPageContentStream(FPDF_DOCUMENT document, int index) {
    FPDF_PAGE _page = FPDF_LoadPage(document, index);
    CPDF_Page* page = CPDFPageFromFPDFPage(_page);
    CPDF_Stream* stream = page->GetDict()->GetStreamFor(pdfium::page_object::kContents);

    RetainPtr<CPDF_StreamAcc> streamacc;
    streamacc = pdfium::MakeRetain<CPDF_StreamAcc>(stream);
    streamacc->LoadAllDataFiltered();
    size_t length = streamacc->GetSize();
    uint8_t* temp = streamacc->GetData();
    return std::string((char*)temp, length);
}

FPDF_EXPORT
void FPDF_SetPageContentStream(FPDF_DOCUMENT document, int index, std::string value) {
    FPDF_PAGE _page = FPDF_LoadPage(document, index);
    CPDF_Page* page = CPDFPageFromFPDFPage(_page);
    CPDF_Stream* stream = page->GetDict()->GetStreamFor(pdfium::page_object::kContents);
    stream->SetDataAndRemoveFilter(pdfium::make_span((uint8_t*)value.c_str(), value.length()));
}

FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV
FPDF_GetFPDFPageMatrix(FPDF_PAGE page, double* a, double* b, double* c, double* d, double* e, double* f) {
    CPDF_Page* pPage = CPDFPageFromFPDFPage(page);
    if (!pPage || !a || !b || !c || !d || !e || !f)
        return false;
    CFX_Matrix cfx_matrix;
    FPDF_GetPageMatrix(pPage, cfx_matrix);
    std::tie(*a, *b, *c, *d, *e, *f) = cfx_matrix.AsTuple();
    return true;
}

void FPDF_ProcessFormObjForPFB(FPDF_PAGEOBJECT page_obj, const CFX_Matrix& matrix, std::vector<FPDF_IMAGE_ITEM_PFB>& images);

inline void FPDF_GetPFBImageItemBox(FPDF_RECT_PFB& box, CFX_FloatRect& rect) {
    box.left = rect.left;
    box.top = rect.top;
    box.right = rect.right;
    box.bottom = rect.bottom;
}

void FPDF_ProcessImageObjectForPFB(CPDF_ImageObject* pImageObj,
                                   const CFX_Matrix& matrix,
                                   std::vector<FPDF_IMAGE_ITEM_PFB>& images) {
    FPDF_IMAGE_ITEM_PFB image;
    // clip box
    CFX_FloatRect rect;
    if (pImageObj->m_ClipPath.HasRef()) {
        pImageObj->m_ClipPath.Transform(matrix);
        rect = pImageObj->m_ClipPath.GetClipBox();
    }
    FPDF_GetPFBImageItemBox(image.clipBox, rect);
    // bbox
    CFX_Matrix img_matrix = pImageObj->matrix();
    img_matrix.Concat(matrix);
    rect = img_matrix.TransformRect(CFX_FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
    FPDF_GetPFBImageItemBox(image.bbox, rect);
    images.push_back(image);
}

void FPDF_FindAndProcessImageObj(FPDF_PAGEOBJECT object,
                                 const CFX_Matrix& matrix,
                                 std::vector<FPDF_IMAGE_ITEM_PFB>& images,
                                 int obj_type) {
    if (obj_type == FPDF_PAGEOBJ_IMAGE) {
        CPDF_PageObject* pImageObj = CPDFPageObjectFromFPDFPageObject(object);
        FPDF_ProcessImageObjectForPFB(pImageObj->AsImage(), matrix, images);
    } else {
        FPDF_ProcessFormObjForPFB(object, matrix, images);
    }
}

void FPDF_ProcessFormObjForPFB(FPDF_PAGEOBJECT page_obj,
                               const CFX_Matrix& matrix,
                               std::vector<FPDF_IMAGE_ITEM_PFB>& images) {
    int form_counts = FPDFFormObj_CountObjects(page_obj);
    CFX_Matrix form_matrix = CPDFPageObjectFromFPDFPageObject(page_obj)->AsForm()->form_matrix();
    form_matrix.Concat(matrix);
    for (int i = 0; i < form_counts; ++i) {
        FPDF_PAGEOBJECT form_obj = FPDFFormObj_GetObject(page_obj, i);
        int obj_type = FPDFPageObj_GetType(form_obj);
        if (obj_type != FPDF_PAGEOBJ_IMAGE && obj_type != FPDF_PAGEOBJ_FORM)
            continue;
        FPDF_FindAndProcessImageObj(form_obj, form_matrix, images, obj_type);
    }
}

FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV
FPDF_GetPageItemForPFB(FPDF_PAGE page, FPDF_PAGE_ITEMS_PFB& item) {
    CPDF_Page* pCPDFPage = CPDFPageFromFPDFPage(page);
    if (!pCPDFPage)
        return false;

    CFX_Matrix matrix;
    FPDF_GetPageMatrix(pCPDFPage, matrix);
    int object_counts =  FPDFPage_CountObjects(page);
    std::vector<FPDF_IMAGE_ITEM_PFB> images;
    for (int index = 0; index < object_counts; ++index) {
        FPDF_PAGEOBJECT page_obj = FPDFPage_GetObject(page, index);
        int obj_type = FPDFPageObj_GetType(page_obj);
        if (obj_type != FPDF_PAGEOBJ_IMAGE && obj_type != FPDF_PAGEOBJ_FORM)
            continue;
        FPDF_FindAndProcessImageObj(page_obj, matrix, images, obj_type);
    }

    item.image_counts = images.size();
    item.pImages = new FPDF_IMAGE_ITEM_PFB[item.image_counts];
    if (!item.pImages)
        return false;

    for (uint32_t i = 0; i < item.image_counts; ++i) {
        item.pImages[i] = images[i];
    }
    return true;
}

FPDF_EXPORT void FPDF_CALLCONV
FPDF_DestroyPageItemForPFB(FPDF_PAGE_ITEMS_PFB& item) {
    if (!item.pImages)
        return;
    delete [] item.pImages;
    item.pImages = nullptr;
    item.image_counts = 0;
}

void ProcessFormObjCounts(FPDF_PAGEOBJECT form_obj, FPDF_PAGE_INFO& info);

void ProcessSubobjCounts(FPDF_PAGEOBJECT sub_obj, FPDF_PAGE_INFO& info, int obj_type) {
    switch (obj_type)
    {
        case FPDF_PAGEOBJ_FORM:
            ProcessFormObjCounts(sub_obj, info);
            break;
        case FPDF_PAGEOBJ_TEXT:
            ++(info.text_counts);
            break;
        case FPDF_PAGEOBJ_PATH:
            ++(info.path_counts);
            break;
        case FPDF_PAGEOBJ_IMAGE:
            ++(info.image_counts);
            break;
        default:
            break;
    }
}

void ProcessFormObjCounts(FPDF_PAGEOBJECT form_obj, FPDF_PAGE_INFO& info) {
    int form_counts = FPDFFormObj_CountObjects(form_obj);
    for (int i = 0; i < form_counts; ++i) {
        FPDF_PAGEOBJECT sub_form_obj = FPDFFormObj_GetObject(form_obj, i);
        int obj_type = FPDFPageObj_GetType(sub_form_obj);
        ProcessSubobjCounts(sub_form_obj, info, obj_type);
    }
}

FPDF_EXPORT void FPDF_CALLCONV
FPDF_GetPageInfo(FPDF_PAGE page, FPDF_PAGE_INFO& info) {
    CPDF_Page* pCPDFPage = CPDFPageFromFPDFPage(page);
    if (!pCPDFPage)
        return;

    info.width = FPDF_GetPageWidth(page);
    info.height = FPDF_GetPageHeight(page);
    int obj_counts =  FPDFPage_CountObjects(page);
    for (int i = 0; i < obj_counts; ++i) {
        FPDF_PAGEOBJECT sub_obj = FPDFPage_GetObject(page, i);
        int obj_type = FPDFPageObj_GetType(sub_obj);
        ProcessSubobjCounts(sub_obj, info, obj_type);
    }
}

FPDF_EXPORT
int FPDF_GetPageRotation(FPDF_PAGE page) {
    CPDF_Page* pPDFPage = CPDFPageFromFPDFPage(page);
    if (!pPDFPage)
        return -1;
    std::vector<int> texts_rotation_vec(4, 0);
    for (auto it = pPDFPage->begin(); it != pPDFPage->end(); ++it) {
        CPDF_PageObject* pObj = it->get();
        if (!pObj)
            continue;
        if (pObj->IsText()) {
            FPDF_CountTextsRotation(pObj->AsText(), texts_rotation_vec);
        }
    }
    return FPDF_CalcPageRotation(pPDFPage, texts_rotation_vec);
}
