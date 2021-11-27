# RemoteDesktopCaptureMLConverter
RemoteDesktopCaptureMLConverter


## From [redemption](https://github.com/DianaAssistant/redemption/README.md#Convert .mwrm/.wrm capture to video)
### Convert .mwrm/.wrm capture to video

`.mwrm` and `.wrm` are native capture formats in `capture_flags=2` in `rdpproxy.ini`.

    redrec -f --video-codec mp4 -i file.mwrm -o output_prefix

Note: `rdpproxy --print-default-ini` show a default ini file.
