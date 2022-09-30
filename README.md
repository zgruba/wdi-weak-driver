# Weak Driver Problem

Made for Mathematics introduction to computer science classes at MIMUW in the winter semester of 2019/2020.

## Usage:
Compile the file, e.g. with gcc:
```
gcc slabykierowca_zg.c -o kierowca
```
And simply run it as follows:
```
./kierowca
```

Configuration is read from `input.txt` in the same directory as the executable.

## Description:

"The weak driver" can't turn left, so they try to avoid this maneuver.  
They use our GPS to find the shortest path on a given map without turning left.

## Examples:

```
Wymiary tablicy: X: 7 na Y: 6

0 0 1 0 0 0 0
0 0 1 0 0 0 0
0 0 1 0 0 0 0
0 0 1 0 0 0 0
1 1 0 1 1 1 1
0 0 0 0 0 0 0

start: x: 1   y: 0
meta:  x: 5   y: 2


Nie ma drogi
```

```
Wymiary tablicy: X: 4 na Y: 3

0 0 0 0
0 0 0 0
0 0 0 0

start: x: 1   y: 0
meta:  x: 3   y: 0


Najkrotsza sciezka to 6 krokow
Kolejne kroki to:
   <1,0> start
-> <1,1>
-> <0,1>
-> <0,0>
-> <1,0>
-> <2,0>
-> <3,0> meta

         ***** GPS *****

 Twoja lokalizacja to <1,0>
 Cel podrozy to: <3,0>
 jedz prosto
 skrec w prawo
 skrec w prawo
 skrec w prawo
 jedz prosto
 jedz prosto
 Jestes na miejscu!

        ***** MAPA *****
╔════════════╗
║ +  S  +  M ║
║ +  +       ║
║            ║
╚════════════╝
```
