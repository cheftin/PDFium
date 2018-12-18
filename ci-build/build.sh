#!/usr/bin/env bash
#
# Variables to provide:
# CONFIGURATION = Debug | Release
# PDFium_BRANCH = master | chromium/3211 | ...

set -ex

OS=$(uname)
case $OS in
MINGW*)
  OS="windows"
  ;;
*)
  OS=$(echo $OS | tr '[:upper:]' '[:lower:]')
  ;;
esac

# Input
PDFium_BRANCH=cheftin
PDFium_SOURCE_REVIISON=unknown
CONFIGURATION='Release'

DepotTools_URL='https://chromium.googlesource.com/chromium/tools/depot_tools.git'
DepotTools_DIR="$PWD/depot_tools"

PDFium_URL='https://github.com/cheftin/PDFium.git' # or git@github.com:cheftin/PDFium.git
PDFium_SOURCE_DIR="$PWD/pdfium"


PDFium_BUILD_DIR="$PDFium_SOURCE_DIR/out"
PDFium_CI_DIR="$PDFium_SOURCE_DIR/ci-build"
PDFium_CMAKE_CONFIG="$PDFium_CI_DIR/PDFiumConfig.cmake"
PDFium_ARGS="$PDFium_CI_DIR/args/$OS.args.gn"

# Output
PDFium_STAGING_DIR="$PWD/staging"
PDFium_INCLUDE_DIR="$PDFium_STAGING_DIR/include"
PDFium_LIB_DIR="$PDFium_STAGING_DIR/lib"
PDFium_ARTIFACT="$PWD/pdfium-$OS.tgz"
[ "$CONFIGURATION" == "Debug" ] && PDFium_ARTIFACT="$PWD/pdfium-$OS-debug.tgz"


# Download depot_tools
if [ ! -e "$DepotTools_DIR" ]; then
    git clone "$DepotTools_URL" "$DepotTools_DIR"
else
    git -C "$DepotTools_DIR" pull
fi
export PATH="$DepotTools_DIR:$PATH"

# Clone
if [ ! -e "$PDFium_SOURCE_DIR" ]; then
    gclient config --unmanaged "$PDFium_URL" --name=pdfium
fi
gclient sync

# Prepare directories
mkdir -p "$PDFium_BUILD_DIR"
mkdir -p "$PDFium_STAGING_DIR"
mkdir -p "$PDFium_LIB_DIR"

# Checkout to target branch
cd "$PDFium_SOURCE_DIR"
git checkout "${PDFium_BRANCH:-master}"
PDFium_SOURCE_REVIISON=$(git rev-parse --short HEAD)
gclient sync

# Patch
cd "$PDFium_SOURCE_DIR"
git checkout -- .

# Configure
cp "$PDFium_ARGS" "$PDFium_BUILD_DIR/args.gn"
[ "$CONFIGURATION" == "Release" ] && echo 'is_debug=false' >> "$PDFium_BUILD_DIR/args.gn"

# Generate Ninja files
gn gen "$PDFium_BUILD_DIR"

# Build
ninja -C "$PDFium_BUILD_DIR" pdfium
ls -l "$PDFium_BUILD_DIR"

# Install
cp "$PDFium_CMAKE_CONFIG" "$PDFium_STAGING_DIR"
cp "$PDFium_SOURCE_DIR/LICENSE" "$PDFium_STAGING_DIR"
rm -rf "$PDFium_INCLUDE_DIR"
cp -R "$PDFium_SOURCE_DIR/public" "$PDFium_INCLUDE_DIR"
rm -f "$PDFium_INCLUDE_DIR/DEPS"
rm -f "$PDFium_INCLUDE_DIR/README"
rm -f "$PDFium_INCLUDE_DIR/PRESUBMIT.py"
[ "$OS" == "linux" ] && mv "$PDFium_BUILD_DIR/libpdfium.so" "$PDFium_LIB_DIR"
[ "$OS" == "darwin" ] && mv "$PDFium_BUILD_DIR/libpdfium.dylib" "$PDFium_LIB_DIR"

# Pack
cd "$PDFium_STAGING_DIR"
tar cvf "$PDFium_ARTIFACT" -- *

DATE=$(date +%Y%m%d%H%M)
curl http://cxan.kdr2.com/scripts/pkg-c.php -F token=${CXAN_TOKEN} -F file=@$PDFium_ARTIFACT \
     -F dest=/pdfium/${OS}/pdfium-$OS-${DATE}-${PDFium_SOURCE_REVIISON}.tgz
curl http://cxan.kdr2.com/scripts/pkg-c.php -F token=${CXAN_TOKEN} -F file=@$PDFium_ARTIFACT \
     -F dest=/pdfium/${OS}/pdfium-$OS-latest.tgz

curl $NOTIFICATION_MM_WEBHOOK --header "Content-Type: application/json"  --request POST \
  --data-binary @- <<EOF
{
  "attachments": [
    {
      "fallback": "GitHub CI Information",
      "color": "#00FF00",
      "pretext": "PDFium(${OS}) Build Completed.",
      "text": "A PDFium(${OS}) building was just completed. You can download the new package from the links below.",
      "author_name": "Cheftin via Travis CI",
      "author_link": "http://cxan.kdr2.com/pdfium/${OS}/",
      "title": "PDFium(${OS}) Build Completed.",
      "title_link": "https://travis-ci.org/cheftin/PDFium",
      "fields": [
        {
          "short":true,
          "title":"Permanent Link",
          "value":"http://cxan.kdr2.com/pdfium/${OS}/pdfium-$OS-${DATE}-${PDFium_SOURCE_REVIISON}.tgz"
        },
        {
          "short":true,
          "title":"Temporary Alias",
          "value":"http://cxan.kdr2.com/pdfium/${OS}/pdfium-$OS-latest.tgz"
        }
      ]
    }
  ]
}
EOF
