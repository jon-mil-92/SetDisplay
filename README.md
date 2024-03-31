# SetDisplay

This is a helper program designed only for usage in [Display Hot Keys]. It does not do checks on valid input as this is handled in Display Hot Keys, so be very careful if you choose to compile and use this on your own machine without using it via Display Hot Keys. This program sets the primary display's resolution, bit depth, refresh rate, and display scale percentage via the CLI.

## Usage

The following is an example of how to set a 3840x2160 resolution with a 32-bit depth, 120 Hz refresh rate, and 100% display scaling:

```console
C:\path\to\SetDisplay> SetDisplay.exe 3840 2160 32 120 100
```
[Display Hot Keys]: https://github.com/jon-mil-92/DisplayHotKeys
