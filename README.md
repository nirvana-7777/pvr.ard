[![License: GPL-2.0-or-later](https://img.shields.io/badge/License-GPL%20v2+-blue.svg)](LICENSE.md)

# EON.tv PVR client for Kodi
This is an ARD PVR client addon for Kodi. It provides Kodi integration for the german public broadcaster [ARD](https://www.ard.de). Service is geoblocked outside of Germany.

## Supported channels

- Das Erste
- hr
- BR
- NDR
- WDR
- Phoenix
- Radio Bremen
- MDR
- 3Sat
- SWR
- rbb

## Features
- Live TV
- EPG
- Replay (planned)

## Build instructions

### Linux

1. `git clone --branch master https://github.com/xbmc/xbmc.git`
2. `mkdir -p xbmc/cmake/addons/addons/pvr.ard/`
3. `echo "pvr.ard https://github.com/nirvana-7777/pvr.ard Nexus" > xbmc/cmake/addons/addons/pvr.ard/pvr.ard.txt`
4. `echo "all" > xbmc/cmake/addons/addons/pvr.ard/platforms.txt`
5. `git clone https://github.com/nirvana-7777/pvr.ard.git`
6. `cd pvr.ard && mkdir build && cd build`
7. `cmake -DADDONS_TO_BUILD=pvr.ard -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../xbmc/addons -DPACKAGE_ZIP=1 ../../xbmc/cmake/addons`
8. `make`

## Notes

- Plan is to access the 2h Timeshift Buffer for instant replay - not yet implemented

##### Useful links

* [Kodi's PVR user support](https://forum.kodi.tv/forumdisplay.php?fid=167)
* [Kodi's PVR development support](https://forum.kodi.tv/forumdisplay.php?fid=136)

## Disclaimer

- This addon is inofficial and not linked in any form to ARD
- All trademarks belong to them
