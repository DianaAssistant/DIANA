# RemoteDesktopCaptureMLConverter
RemoteDesktopCaptureMLConverter


## OCR project in ReDemPtion

[link](../redemption/projects/ocr1/README.md)

from it's [Makefile](../redemption/projects/ocr1/Makefile)
```
learning: learning.cc
    $(CXX) $^ -I../../src/capture/ocr/ -I../../modules/ppocr/includes/mln -O2 -std=c++11 -o $@
```



## From [redemption](../redemption/README.md#convert-mwrmwrm-capture-to-video)
### Convert .mwrm/.wrm capture to video

`.mwrm` and `.wrm` are native capture formats in `capture_flags=2` in `rdpproxy.ini`.

    redrec -f --video-codec mp4 -i file.mwrm -o output_prefix

Note: `rdpproxy --print-default-ini` show a default ini file.


