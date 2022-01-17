# psdraw

Written by [Eric R. Weeks](http://www.physics.emory.edu/faculty/weeks//):
> I decided to write a very simple program called "psdraw" which would take (X,Y) values and print them on a PostScript page. Then I could add the pictures to my web page ( [see Sec. 1 for information on converting PostScript to gif](http://www.physics.emory.edu/faculty/weeks//graphics/convert.html)). The program can produce graphs with axes and labels and such, or can produce simple drawings without axes.
>
> This program can print lines connecting the points, or just print a circle at each point. In addition, the color of each point can be specified by giving the program the data in (X,Y,R,G,B) form (red, green, blue colors). This program also has a crude way to plot 3D data (see below).
>
> Currently I've made a couple pictures with this program [(a simple picture to test the program](http://www.physics.emory.edu/faculty/weeks//ideas/trig.html) and one of my old favorites, [a fractal coral generator) ](http://www.physics.emory.edu/faculty/weeks//ideas/grass1.html). I add features to this program from time to time, mainly when I find something new I need to do, or occasionally by someone's request.

Original source:
http://www.physics.emory.edu/~weeks/graphics/psdraw.html

## How to compile

```shell
gcc dps.c -lm -o psdraw
```

## How to use
You can use psdraw in a pipe, or with a filename:

```shell
zcat datafile.Z | ./psdraw > output.ps
```

```shell
./psdraw datafile > output.ps
```

## Example

```shell
cat data | psdraw -Z 255 0 128 -X - - -20 20 -S 15 6 -l 0.1 -A -x 10 -y 5
   -a "time" -b "X variable" -T "Lorenz attractor" > vlorenz.ps
```
