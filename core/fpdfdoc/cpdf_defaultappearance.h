// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFDOC_CPDF_DEFAULTAPPEARANCE_H_
#define CORE_FPDFDOC_CPDF_DEFAULTAPPEARANCE_H_

#include <utility>

#include "core/fpdfapi/parser/cpdf_simple_parser.h"
#include "core/fxcrt/fx_string.h"
#include "core/fxcrt/fx_system.h"
#include "core/fxge/cfx_color.h"
#include "core/fxge/dib/fx_dib.h"

class CPDF_DefaultAppearance {
 public:
  CPDF_DefaultAppearance();
  explicit CPDF_DefaultAppearance(const ByteString& csDA);
  CPDF_DefaultAppearance(const CPDF_DefaultAppearance& cDA);
  ~CPDF_DefaultAppearance();

  Optional<ByteString> GetFont(float* fFontSize);

  Optional<CFX_Color> GetColor() const;
  Optional<std::pair<CFX_Color::Type, FX_ARGB>> GetColorARGB() const;

  bool FindTagParamFromStartForTesting(CPDF_SimpleParser* parser,
                                       ByteStringView token,
                                       int nParams);

 private:
  ByteString m_csDA;
};

#endif  // CORE_FPDFDOC_CPDF_DEFAULTAPPEARANCE_H_
