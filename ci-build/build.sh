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


if [ $OS = "linux" ]; then
        sudo apt-get update -qq -y;
        sudo apt-get install curl python perl -y;
        sudo apt-get install pkg-config libglib2.0-dev -y;
        sudo apt-get install software-properties-common -y;
        sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y;
        sudo apt-get update;
        sudo apt-get install gcc-9 g++-9 -y;
        sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60;
        sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 60;
fi

# Input
PDFium_BRANCH=cheftin
PDFium_SOURCE_REVIISON=unknown
CONFIGURATION='Release'
# CONFIGURATION='Debug'

DepotTools_URL='https://storage.googleapis.com/chrome-infra/depot_tools.zip'
DepotTools_DIR="$PWD/depot_tools"

PDFium_URL='https://github.com/PaodingAI/PDFium.git' # or git@github.com:PaodingAI/PDFium.git
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
curl -O $DepotTools_URL
unzip depot_tools.zip -d $DepotTools_DIR
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
curl https://cxan.kdr2.com/scripts/file-c.php \
     -F token=${CXAN_TOKEN_KDR2} -F file=@$PDFium_ARTIFACT \
     -F dest=/pdfium/${OS}/pdfium-$OS-${DATE}-${PDFium_SOURCE_REVIISON}.tgz \
     -F alias=/pdfium/${OS}/pdfium-$OS-latest.tgz

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
      "author_link": "https://cxan.kdr2.com/pdfium/${OS}/",
      "title": "PDFium(${OS}) Build Completed.",
      "title_link": "https://travis-ci.org/cheftin/PDFium",
      "fields": [
        {
          "short":true,
          "title":"Permanent Link",
          "value":"https://cxan.kdr2.com/pdfium/${OS}/pdfium-$OS-${DATE}-${PDFium_SOURCE_REVIISON}.tgz"
        },
        {
          "short":true,
          "title":"Temporary Alias",
          "value":"https://cxan.kdr2.com/pdfium/${OS}/pdfium-$OS-latest.tgz"
        }
      ]
    }
  ]
}
EOF
