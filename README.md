# SetDisplay

This is a helper program designed only for usage in [Display Hot Keys]. This program sets the primary display's resolution, bit depth, refresh rate, display scaling mode, and DPI scale percentage via the CLI. Only integers can be used as input if you decide to use this program outside of Display Hot Keys!

## Usage

The following is an example of how to set a 3840x2160 resolution with a 32-bit depth, 120 Hz refresh rate, centered scaling mode, and 100% DPI scaling:

```console
C:\path\to\SetDisplay> SetDisplay.exe 3840 2160 32 120 2 100
```
Possible scaling mode values are as follows:
* 0 to preserve aspect ratio.
* 1 to stretch the image to fill the panel.
* 2 to center the image in the panel.
* Any other integer will use the preserve aspect ratio scaling mode.

**Note:** You may need to use GPU Scaling in your display driver settings to prevent the monitor from overriding the scaling mode.

[Display Hot Keys]: https://github.com/jon-mil-92/DisplayHotKeys
