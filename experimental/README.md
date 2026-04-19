# Networking in Blastem
Emulated networking for serial retro.link using SweMonkey's XPort implementation
in [smdt](https://github.com/SweMonkey/smdt) and [smdt-pc](https://github.com/SweMonkey/smdt-pc)

This requires [blastem](https://www.retrodev.com/blastem/nightlies/) 6.3 and up


## Setup
1. Build `smdt-pc` by
*  downloading the src from [github](https://github.com/SweMonkey/smdt-pc)
* and compiling it
```bash
cd smdt-pc/src
gcc main.c -o smdtpc
```

2. Download [blastem](https://www.retrodev.com/blastem/nightlies/) 6.3 or greater.
I went with the GNU-Linux version and haven't even tried the Windows one.

I'm not planning on using the saturn keyboard for retro.link games, so 
I set the `io` block in `~/.config/blastem/blastem.cfg` to 
```cfg
io {
    devices {
        1 gamepad6.1
        2 serial
        ext none
    }
    socket smdtsock.sock
```

*NOTE* You can also make blastem read a config file from its directory
1. run `blastem`
2. Go to `Settings > System`
3. Select the "Save config with EXE" option.

This will let you save the config in the blastem folder.

## Usage
0. build one of the experimental projects
1. Run blastem
2. Open the out.bin 
3. Run `SMDT-PC` with `./smdtpc -xportsock ~/blastem/smdtsock.sock`




