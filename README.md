## How to compile

```
gcc dps.c -lm
```

## How to use
You can use psdraw in a pipe, or with a filename:

```
zcat datafile.Z | psdraw > output.ps
```

```
psdraw datafile > output.ps
```

## Example

```
cat data | psdraw -Z 255 0 128 -X - - -20 20 -S 15 6 -l 0.1 -A -x 10 -y 5
   -a "time" -b "X variable" -T "Lorenz attractor" > vlorenz.ps
```
