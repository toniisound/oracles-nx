# Oracles NX

A native Nintendo Switch homebrew port (`.nro`) of **Oracle of Ages** and **Oracle of Seasons**, built on
[oracles-decomp](https://github.com/kirby-letsgo/oracles-decomp), a reimplementation of both Game Boy Color
games in readable C. The games run as native code: no CPU emulator and no ROM code.

- Both games in one app, with a launcher showing each game's three save files
- Save states (4 slots with thumbnails) and a Resume state on quit
- Screen filters (sharp, scanlines, LCD grid, CRT) and pixel or fill scaling
- Widescreen mode (16:9) that shows the neighbouring rooms beside the game
- Fully remappable controls, fast-forward and optional quality-of-life toggles
- Full screen in handheld (720p) and docked (1080p)

> **No game files are included.** You need your own Oracle of Ages / Oracle of Seasons (USA, Australia)
> ROMs. This project is not affiliated with or endorsed by Nintendo or Capcom.

## Credits

- **[kirby-letsgo](https://github.com/kirby-letsgo)**: [oracles-decomp](https://github.com/kirby-letsgo/oracles-decomp), the native C reimplementation, launcher, settings, widescreen and save sync. All the real work is theirs.
- **[Drenn1](https://github.com/Drenn1)** and contributors: [oracles-disasm](https://github.com/Drenn1/oracles-disasm), the community disassembly the decomp is built from.
- **[devkitPro](https://devkitpro.org)**: devkitA64, libnx and the Switch branch of SDL.
- **[SDL](https://libsdl.org)**: SDL3, for video, audio and input.
- **Zenju**: xBRZ pixel-art scaler. **Maxim Stepin / Cameron Zemek**: hqx scaler.
- **Toniisound**: Nintendo Switch port.

## License

GPL v3, the same as oracles-decomp. See [LICENSE](LICENSE). The upstream README is kept as
[README-original.md](README-original.md).

## Technical overview

The port uses `oracles-native`, the decomp's real native build, unchanged in its game code. On the Switch it
links SDL3 built from source (devkitPro's `switch-sdl-3.4` branch) and draws through EGL / OpenGL ES 2
(switch-mesa). The game's threads run as fibers on heap-allocated stacks, since libnx has no `mmap`. Instead
of a file picker, the app installs the ROMs it finds on the SD card; from then on it never reads the ROM
again.

## Building (WSL or Linux with devkitPro)

```
sudo dkp-pacman -S --needed switch-dev switch-cmake switch-pkg-config switch-mesa switch-libdrm_nouveau
sudo apt install ninja-build git
git clone https://github.com/toniisound/oracles-nx.git
cd oracles-nx
./build_switch.sh
```

The result is `build-switch/oracles.nro`. The first build downloads and compiles SDL3, so it takes a while.
After changing the title, author or version, delete `build-switch/` so the `.nacp` is regenerated.

| Option | Effect |
| --- | --- |
| `-DORACLES_SWITCH_SYNC=ON` | Enable save sync across devices (needs `switch-curl`) |
| `-DORACLES_NX_TITLE="..."` | Title in the Homebrew Menu (default `Oracles NX`) |
| `-DORACLES_NX_AUTHOR="..."` | Author in the Homebrew Menu (default `Toniisound`) |
| `-DORACLES_NX_VERSION="..."` | Version in the Homebrew Menu (default `1.0`) |

Options go after the script, e.g. `./build_switch.sh -DORACLES_SWITCH_SYNC=ON`.

## Installing

```
sdmc:/switch/oracles/
├── oracles.nro
└── roms/
    ├── Oracle of Ages (USA).gbc
    └── Oracle of Seasons (USA).gbc
```

### ROMs

- Any file name works: games are recognised by their SHA1, not their name.
- `.gbc`, `.gb`, `.zip` and `.gz` are accepted.
- Only the USA / Australia versions work:

| Game | SHA1 |
| --- | --- |
| Oracle of Ages | `880374fb978b18af4aa529e2e32f7ffb4d7dd2f4` |
| Oracle of Seasons | `ba1268290fb2b1b70505d2d7b5825fc8a4816a4b` |

On first launch the app installs every ROM it recognises into `ages/` and `seasons/` next to the `.nro`.
Saves, save states and settings live in `sdmc:/switch/oracles/`, and the engine log in
`sdmc:/switch/oracles/oracles.log`. Do not share the `ages/` and `seasons/` folders: they hold data from
your ROM.

## Menu

- The launcher shows both games side by side, each with its three save files, Resume and Load state.
- A accepts, B goes back.
- **Add ROM** looks in `roms/` again, for a game added later.
- Settings (from the launcher or the pause menu): volume, screen filter, scale, colours, widescreen,
  controls and quality-of-life toggles.

## Default controls

| Button | Action |
| --- | --- |
| D-pad / left stick | Move |
| A | A |
| B | B |
| + | Start |
| − | Select |
| R | Pause menu (save / load state, quit to launcher) |
| ZR (hold) | Fast-forward |
| L | Swap A and B items (with *quick swap*) |
| X / Y | Extra item buttons (with *4 slots*) |
| HOME | System menu |

Every button can be remapped in Settings > Controls.

## Changes from oracles-decomp

- `CMakeLists.txt`: Switch target `oracles-nx` that builds SDL3 from devkitPro's Switch branch and packs the `.nro` with its icon and `.nacp`.
- `src/platform/app.c`: ROM install from `sdmc:/switch/oracles/roms/`, data and log on the SD card, Nintendo button layout in menus and button names.
- `src/platform/window.c`: full-screen OpenGL window (SDL on the Switch needs EGL).
- `src/rt/fibers.c`: fiber stacks from the heap instead of `mmap`.
- `src/ui/controls.c`: Switch default buttons (Pause on R).
- `src/sync/http_curl.c`, `src/sync/http_none.c`: statically linked libcurl, or no sync.
- `build_switch.sh`, `packaging/switch/icon.jpg`: build script and Homebrew Menu icon.

---

This emulator is completely free and open-source. Optional donations to support the development are always welcome:

[![Ko-fi](https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white)](https://ko-fi.com/toniisound)
[![PayPal](https://img.shields.io/badge/PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white)](https://paypal.me/toniisound)
