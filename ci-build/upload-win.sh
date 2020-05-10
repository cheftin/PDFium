SOURCE_DIR=$1
FILE=$2
ARCH=$3
CONF=$4

if [[ $CONF == "Debug" ]]; then
    CONF="-debug"
else
    CONF=""
fi

PDFium_SOURCE_REVIISON=$(git -C ${SOURCE_DIR} rev-parse --short HEAD)
DATE=$(C:/cygwin64/bin/date +%Y%m%d%H%M)

curl http://cxan.kdr2.com/scripts/file-c.php \
     -F token=${CXAN_TOKEN_KDR2} -F file=@${FILE} \
     -F dest=/pdfium/windows-${ARCH}/pdfium-windows-${ARCH}-${DATE}-${PDFium_SOURCE_REVIISON}${CONF}.zip \
     -F alias=/pdfium/windows-${ARCH}/pdfium-windows-${ARCH}-latest${CONF}.zip

curl $NOTIFICATION_MM_WEBHOOK --header "Content-Type: application/json"  --request POST \
  --data-binary @- <<EOF
{
  "attachments": [
    {
      "fallback": "GitHub CI Information",
      "color": "#00FF00",
      "pretext": "PDFium(windows) Build Completed.",
      "text": "A PDFium(windows) building was just Completed. You can download the new package from the links below.",
      "author_name": "Cheftin via Appveyor CI",
      "author_link": "http://cxan.kdr2.com/pdfium/windows-x64/",
      "title": "PDFium(windows) Build Completed.",
      "title_link": "https://ci.appveyor.com/project/KDr2/pdfium",
      "fields": [
        {
          "short":true,
          "title":"Permanent Link",
          "value":"http://cxan.kdr2.com/pdfium/windows-${ARCH}/pdfium-windows-${ARCH}-${DATE}-${PDFium_SOURCE_REVIISON}${CONF}.zip"
        },
        {
          "short":true,
          "title":"Temporary Alias",
          "value":"http://cxan.kdr2.com/pdfium/windows-${ARCH}/pdfium-windows-${ARCH}-latest${CONF}.zip"
        }
      ]
    }
  ]
}
EOF
