# hooks

Shim code compiled into DF `g_src` that loads and calls a "dfhooks" library located
in the root DF game directory. If a "dfhooks" library cannot be found or loaded, or
if it does not provide the specific API call that the hook is looking for, the shim
will return immediately and have no effect.

The shim will pass calls through to a library named according to the convention of
the host platform it was compiled for:

- `dfhooks.dll` (Windows)
- `libdfhooks.so` (Linux)
- `libdfhooks.dylib` (OSX)

This shim code is released into the public domain. See `LICENSE` for details.
