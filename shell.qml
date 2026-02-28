//@ pragma Env QS_NO_RELOAD_POPUP=1
//@ pragma Env PATH=/home/yora/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl
//@ pragma Env QML2_IMPORT_PATH=/home/yora/caelestia-wallpaper-selector/build/qml
//@ pragma Env QML_IMPORT_PATH=/home/yora/caelestia-wallpaper-selector/build/qml
//@ pragma Env ILLOGICAL_IMPULSE_VIRTUAL_ENV=~/.local/state/quickshell/.venv

import Quickshell

ShellRoot {
    WallpaperSelector {}
}
