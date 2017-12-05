game_viruses
============

# Installation

on ubuntu 16.04

```
sudo apt-get install libgtk-3-dev
make
```

# Description
![gameplay](/doc/ims/gameplay.png)

You are the red block. Let it be red blood cell. Light blue are viruses that attack you. If they jump on you, you'll lose. You need to block them by purple walls (let it be some immune blood things) that you can move. Viruses are quiet smart and can find path to you even if is not straight.

Game started from pause.

# Control

`w,a,s,d` or `arrows` to move

`space` to pause (movement unpauses)

`alt-f4` to exit

# Options
```
$ ./bin/run --help
Usage: run [OPTION...] no args expected
doc

  -b, --blocks=<per cent>    field per cent occupied by walls (0 - 100)
                             (default: 35)
  -h, --height=<height>      field height
  -s, --speed=<speed>        game speed form 0 and up. It increments after win
  -v, --viruses=<num>        how many viruses will attack you, depends on field
                             size (default: 10)
  -w, --width=<width>        field width
  -?, --help                 Give this help list
      --usage                Give a short usage message

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

```

# Results and statistics
You can see some statistics in console. Like this.
```
$ ./bin/run -v 10 -b 60 -w 30 -h 20 -s 0
speed level: 0
You've made 96 moves, moved 165 blocks and spent 38,6 seconds and finally WON
speed level: 1
You've made 28 moves, moved 35 blocks and spent 6,8 seconds and finally LOST
speed level: 1
You've made 134 moves, moved 337 blocks and spent 45,6 seconds and finally WON
speed level: 2
```
