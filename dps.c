/*******************************************************************

psdraw -- started 8/22/96 from shellps.c -- Eric R. Weeks

v01:  8-22-96:  started with bare bones: -hXYlmnMNcC options
      8-23-96:  added -O option
v02:  3-02-97:  add axes (-Axy options added)
v03:  3-05-97:  add in axis labels, graph title (-abT)
                also, changed slightly the code to determine
                the axis tick-mark spacing.
v04:  3-26-97:  try to reduce the size of the PostScript file
v05:  4-02-97:  I'm going to try to add 3-D effects... (-z)
v06:  4-13-97:  change -X option to replace -MmNn; add -S option
                to replace -XY
      4-16-97:  add in -Z option
v07:  4-18-97:  add in -L option
      6-15-97:  fix a bug when there should be a x=0/y=0 tickmark
v08: 11-09-97:  add in some new options
      1-27-97:  fixed so can compile using cc also


source code formatted for tab stop 5

my web page:
     http://www.physics.emory.edu/~weeks/
this program pointed at by:
     http://www.physics.emory.edu/~weeks/graphics/psdraw.html

email: weeks@physics.emory.edu

This program is public domain, but you must leave my name & email
address attached.  No guarantees are offered, although if there
are bugs let me know and I might fix them.

 *******************************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define PI 3.14159265358979323846264338328
#define EE 2.71828182845904523536
/* unlikely the user will choose this: */
#define FORCEOFF 234152.0
#define MAX 200000

/* ------ function declarations ------ */
void printheader(float linewidth);
void printtrailer();

double ax[MAX],ay[MAX];
float ared[MAX],agreen[MAX],ablue[MAX];
float fillcolor[3],circle;


/*  M   M   AAA   IIIII  N   N
 *  MM MM  A   A    I    NN  N
 *  M M M  AAAAA    I    N N N   ************************************
 *  M   M  A   A    I    N  NN
 *  M   M  A   A  IIIII  N   N
 */
main(argc,argv)
int argc;
char **argv;
{
	int c,numpts;
	extern int optind;
	extern char *optarg;
	double maxx,minx,maxy,miny,maxz,minz;
	double scalex,scaley,scalez,offsetx,offsety;
	double x,y,z;
	float linewidth,xsize,ysize;
	float forcemaxx,forceminx,forcemaxy,forceminy;
	float swap;
	double red,green,blue;
	int t,colorflag;
	int outline,useaxes,length;
	float axisstep,axisorigin,xspacing,yspacing;
	float temp,storeleftx,minleftx;
	char text[30];
	char xlable[100],ylable[100],title[200];
	int usexl, useyl, usetitle, threed, lineon;
	int ired,igreen,iblue;
	float fred,fgreen,fblue;
	int headeron,traileron;

	/* INPUT PROCESSING / POSTSCRIPT SETUP */
	FILE *fp, *fopen();

	/* ------ set up defaults ------ */
	headeron = 1;
	traileron = 1;
	offsetx = 4.0;
	offsety = 2.0;
	linewidth = 0.01;
	xsize=15.0;
	ysize=15.0;
	forcemaxx = FORCEOFF;
	forceminx = FORCEOFF;
	forcemaxy = FORCEOFF;
	forceminy = FORCEOFF;
	ired=0;igreen=0;iblue=0; /* default color is black          */
	circle = -1.0;			/* don't use circles			*/
	fillcolor[0] = -1.0;	/* don't fill circles			*/
	fillcolor[1] = -1.0;	/* don't fill circles			*/
	fillcolor[2] = -1.0;	/* don't fill circles			*/
	colorflag = 0;			/* don't use color				*/
	outline = 1;			/* outline the circles			*/
	useaxes = 0;			/* don't use axes				*/
	xspacing = -1.0;		/* use default spacing			*/
	yspacing = -1.0;		/* use default spacing			*/
	usetitle=0;			/* don't print a title			*/
	usexl = 0;			/* don't lable x-axis			*/
	useyl = 0;			/* don't lable y-axis			*/
	threed = 0;			/* not three-dimensional data      */
	lineon = 1;			/* connect points with lines       */
	while ((c=getopt(argc, argv, "hl:S:c:fCOAx:y:a:b:T:zXZHLo:Qq")) != EOF)
	switch (c)  {
	case 'h':
fprintf(stderr,"Usage: %s [options]\n",argv[0]);
fprintf(stderr,"Options:\n");
fprintf(stderr," -h   : this help message\n");
fprintf(stderr," -H   : listing of unusual options\n");
fprintf(stderr," -l # : linewidth [%.3f]\n",linewidth);
fprintf(stderr," -L   : toggle line usage ");
fprintf(stderr,"(turns on if -c option used, otherwise turns off)\n");
fprintf(stderr," -S # # : size in X,Y directions on page ");
fprintf(stderr," [%.1f] [%.1f]\n",xsize,ysize);
fprintf(stderr," -X # # # # : (min x, max x, min y, max y) ");
fprintf(stderr," [default: appropriate]\n");
fprintf(stderr,"     (- to use default:  -X - - 1 10 is default for x-axis)\n");
fprintf(stderr," -C   : color specified with each pt (x,y,r,g,b)\n");
fprintf(stderr," -Z # # # : color data this color (RGB values 0-255)");
fprintf(stderr,"  [default: 0 0 0 = black]\n");
fprintf(stderr," -c # : use circles with radius # (in cm)\n");
fprintf(stderr," -O   : turn off outline of circles\n");
fprintf(stderr," -f # # # : fill circles with color # (RGB values 0-255)\n");
fprintf(stderr," -A   : use axes\n");
fprintf(stderr," -x # : x-axis spacing [default: something reasonable]\n");
fprintf(stderr," -y # : y-axis spacing [default: something reasonable]\n");
fprintf(stderr," -a # : title for x-axis\n");
fprintf(stderr," -b # : title for y-axis\n");
fprintf(stderr," -T # : title for graph\n");
fprintf(stderr,"      (example:  psdraw -T \"This is the title\"\n");
fprintf(stderr,"-------------------------------------\n");
fprintf(stderr,"current limit on number of pts: %d\n",MAX);
			exit(1);
			break;
	case 'H':
fprintf(stderr,"Usage: %s [options]\n",argv[0]);
fprintf(stderr,"Options:\n");
fprintf(stderr," -h   : normal help message\n");
fprintf(stderr," -H   : this help message (unusual options)\n");
fprintf(stderr," -z   : 3-D plot (x,y,z) (hint: presort data on z)\n");
fprintf(stderr," -o # # : origin on page [%.1f,%.1f]\n",offsetx,offsety);
fprintf(stderr," -Q   : no header -- useful for concatenating files\n");
fprintf(stderr," -q   : no trailer -- useful for concatenating files\n");
			exit(1);
			break;
	case 'l': linewidth = atof(optarg);
			fprintf(stderr,"linewidth %f\n",linewidth);
			break;
	case 'S': xsize = atof(optarg);
			ysize = atof(argv[optind]);
			optind++;
			break;
	case 'o': offsetx = atof(optarg);
			offsety = atof(argv[optind]);
			optind++;
			break;
	case 'c': circle = atof(optarg);
			lineon = 1 - lineon;
			break;
	case 'f': fillcolor[0] = ((float)atoi(argv[optind]))/255.0; optind++;
			fillcolor[1] = ((float)atoi(argv[optind]))/255.0; optind++;
			fillcolor[2] = ((float)atoi(argv[optind]))/255.0; optind++;
			break;
	case 'C': colorflag = 1;
			break;
	case 'O': outline = 0;
			break;
	case 'A': useaxes = 1;
			break;
	case 'x': xspacing = atof(optarg);
			break;
	case 'y': yspacing = atof(optarg);
			break;
	case 'a': usexl = 1;
			strcpy(xlable,optarg);
			fprintf(stderr,"x lable: %s\n",xlable);
			break;
	case 'b': strcpy(ylable,optarg);
			fprintf(stderr,"y lable: %s\n",ylable);
			useyl = 1;
			break;
	case 'T': strcpy(title,optarg);
			fprintf(stderr,"title: %s\n",title);
			usetitle = 1;
			break;
	case 'z': threed = 1;
			break;
	case 'X': if (strcmp("-",argv[optind])!=0)
				forceminx = atof(argv[optind]);
			optind++;
			if (strcmp("-",argv[optind])!=0)
				forcemaxx = atof(argv[optind]);
			optind++;
			if (strcmp("-",argv[optind])!=0)
				forceminy = atof(argv[optind]);
			optind++;
			if (strcmp("-",argv[optind])!=0)
				forcemaxy = atof(argv[optind]);
			optind++;
			fprintf(stderr,"%f %f ",forceminx,forcemaxx);
			fprintf(stderr,"%f %f \n",forceminy,forcemaxy);
			break;
	case 'Z': ired   = atoi(argv[optind]); optind++;
			igreen = atoi(argv[optind]); optind++;
			iblue  = atoi(argv[optind]); optind++;
			break;
	case 'L': lineon = 1-lineon;
			break;
	case 'Q': headeron = 0;
			break;
	case 'q': traileron = 0;
			break;
        }

	argc -= (optind-1) ; argv += (optind-1) ;
	fp = (argc > 1) ? fopen(*++argv, "r") : stdin;

	numpts = 0;
	maxx = -9e99; minx = 9e99;
	maxy = -9e99; miny = 9e99;
	maxz = -9e99; minz = 9e99;
	fred = ((float)ired)/255.0;
	fgreen = ((float)igreen)/255.0;
	fblue = ((float)iblue)/255.0;
	if (colorflag==0)  {
		if (threed==0)  {
			while ((fscanf(fp,"%lf %lf",&x,&y)) == 2)  {
				ax[numpts] = x;
				ay[numpts] = y;
				numpts++;
				if (x>maxx)  maxx=x;
				if (y>maxy)  maxy=y;
				if (x<minx)  minx=x;
				if (y<miny)  miny=y;
			}
		} else {
			while ((fscanf(fp,"%lf %lf %lf",&x,&y,&z)) == 3)  {
				ax[numpts] = x;
				ay[numpts] = y;
				ared[numpts] = z;
				numpts++;
				if (x>maxx)  maxx=x;
				if (y>maxy)  maxy=y;
				if (z>maxz)  maxz=z;
				if (x<minx)  minx=x;
				if (y<miny)  miny=y;
				if (z<minz)  minz=z;
			}
		}
	} else {
		while (
     (fscanf(fp,"%lf %lf %lf %lf %lf",&x,&y,&red,&green,&blue)) == 5)  {
			ax[numpts] = x;
			ay[numpts] = y;
			ared[numpts] = red;
			agreen[numpts] = green;
			ablue[numpts] = blue;
			numpts++;
			if (x>maxx)  maxx=x;
			if (y>maxy)  maxy=y;
			if (x<minx)  minx=x;
			if (y<miny)  miny=y;
		}
	}
	if (forcemaxx != FORCEOFF)  maxx=forcemaxx;
	if (forceminx != FORCEOFF)  minx=forceminx;
	if (forcemaxy != FORCEOFF)  maxy=forcemaxy;
	if (forceminy != FORCEOFF)  miny=forceminy;
	if (maxx<minx)  {
		swap=maxx; maxx=minx; minx=swap;
		fprintf(stderr,"error:  maxx < minx: swapping them\n");
	}
	if (maxy<miny)  {
		swap=maxy; maxy=miny; miny=swap;
		fprintf(stderr,"error:  maxy < miny: swapping them\n");
	}
	/* fprintf(stderr,"draw -X %f %f %f %f\n",minx,maxx,miny,maxy); */

	if (headeron==1)  printheader(linewidth);
	/* ------ done with initialization stuff ------ */

	/* I want a transformation: X = scalex * ax + offsetx
	 *                          Y = scaley * ay + offsety
	 */

	/* convert to cm */
	scalex = xsize/(maxx-minx);
	scaley = ysize/(maxy-miny);
	if (maxz>minz)  scalez = 1.0/(minz-maxz);
	offsetx -= scalex*minx;
	offsety -= scaley*miny;
	printf("%f %f %f sr\n",fred,fgreen,fblue);

	if (lineon==1)  {
		/* LINES */
		printf("%.3f %.3f m\n",ax[0]*scalex+offsetx,ay[0]*scaley+offsety);

		if (numpts>1)  {
			for (t=1;t<numpts;t++)  {
				printf("%.3f ",ax[t]*scalex+offsetx);
				printf("%.3f ",ay[t]*scaley+offsety);
				printf("l\n");
				if (threed==1)  {
		  printf("%.2f ",(1.0-fred)*(ared[t]-maxz)*scalez+fred);
		  printf("%.2f ",(1.0-fgreen)*(ared[t]-maxz)*scalez+fgreen);
		  printf("%.2f ",(1.0-fblue)*(ared[t]-maxz)*scalez+fblue);
					printf("sr \n");
					printf("%.3f ",ax[t]*scalex+offsetx);
					printf("%.3f m\n",ay[t]*scaley+offsety);
				} else if (colorflag==1)  {
					printf("%.2f ",ared[t]);
					printf("%.2f ",agreen[t]);
					printf("%.2f ",ablue[t]);
					printf("sr a \n");
					printf("%.3f ",ax[t]*scalex+offsetx);
					printf("%.3f m\n",ay[t]*scaley+offsety);
				}
			}
		}
		if (colorflag==0)  printf("a\n");
	}
	if (circle>0.0) {
		printf("%.3f %.3f m\n",ax[0]*scalex+offsetx,ay[0]*scaley+offsety);
		/* CIRCLES */
		for (t=0;t<numpts;t++)  {
			printf("%.3f ",ax[t]*scalex+offsetx);
			printf("%.3f ",ay[t]*scaley+offsety);
			if (threed==1)  {
		  printf("%.2f ",(1.0-fred)*(ared[t]-maxz)*scalez+fred);
		  printf("%.2f ",(1.0-fgreen)*(ared[t]-maxz)*scalez+fgreen);
		  printf("%.2f ",(1.0-fblue)*(ared[t]-maxz)*scalez+fblue);
				printf("sr \n");
				if (outline==1)
					/* colored circle, outlined */
					printf("cc\n");
				else
					/* colored circle, no outline */
					printf("occ\n");
			} else if (colorflag==1)  {
				printf("%.2f ",ared[t]);
				printf("%.2f ",agreen[t]);
				printf("%.2f sr ",ablue[t]);
				if (outline==1)
					/* colored circle, outlined */
					printf("cc\n");
				else
					/* colored circle, no outline */
					printf("occ\n");
			} else {
				if (fillcolor[0]<0.0)  {
					/* plain circle, not filled */
					printf("c\n");
				} else {
					if (outline==1)
						/* circle, outlined, shaded */
						printf("cf\n");
					else
						/* circle, no outline, shaded */
						printf("ocf\n");
				}
			}
		}
	}

	printf("0 setgray\n");
	if (useaxes==1)  {
		minleftx = 9999.9;
		/* print a big box around data */
		printf("0.08 setlinewidth\n");
		printf("%.3f ",minx*scalex+offsetx);
		printf("%.3f m\n",miny*scaley+offsety);
		printf("%.3f ",maxx*scalex+offsetx);
		printf("%.3f l\n",miny*scaley+offsety);
		printf("%.3f ",maxx*scalex+offsetx);
		printf("%.3f l\n",maxy*scaley+offsety);
		printf("%.3f ",minx*scalex+offsetx);
		printf("%.3f l\n",maxy*scaley+offsety);
		printf("cp a\n");
		printf("/Helvetica findfont\n");
		printf("0.6 scalefont setfont\n");

		/* figure out X tickmarks now */
		if (xspacing<0.0)  {
			axisstep = (maxx-minx)/5.0;
			temp = log(axisstep)/log(10.0);
			axisstep /= pow(10.0,(floor(temp)));
			if (floor(axisstep)==0.0)
				axisstep=1.0;
			else if (floor(axisstep)>6.5)
				axisstep=10.0;
			else if (floor(axisstep)==6.0)
				axisstep=5.0;
			else if (floor(axisstep)==4.0)
				axisstep=5.0;
			else if (floor(axisstep)==3.0)
				axisstep=2.0;
			else
				axisstep = floor(axisstep);
			axisstep *= pow(10.0,(floor(temp)));
		} else {
			axisstep = xspacing;
		}
		axisorigin = (floor(minx/axisstep)+1.0)*axisstep;
		for (temp = axisorigin;temp<=maxx;temp+=axisstep)  {
			if ((fabs(temp)<(1e-6))&&(axisstep>(1e-4)))  temp = 0.0;
			(void)sprintf(text,"%g",temp);
			length=strlen(text);
			printf("%.3f ",temp*scalex+offsetx);
			printf("%.3f m ",miny*scaley+offsety);
			printf("%.3f ",temp*scalex+offsetx);
			printf("%.3f l a\n",miny*scaley+offsety+0.5);
			printf("%.3f ",temp*scalex+offsetx);
			printf("%.3f m ",maxy*scaley+offsety);
			printf("%.3f ",temp*scalex+offsetx);
			printf("%.3f l a\n",maxy*scaley+offsety-0.5);
			printf("%.3f ",temp*scalex+offsetx-0.15*length);
			printf("%.3f m ",miny*scaley+offsety-0.5);
			printf("(%g) show\n",temp);
		}

		/* figure out Y tickmarks now */
		if (yspacing<0.0)  {
			axisstep = (maxy-miny)/5.0;
			temp = log(axisstep)/log(10.0);
			axisstep /= pow(10.0,(floor(temp)));
			if (floor(axisstep)==0.0)
				axisstep=1.0;
			else if (floor(axisstep)>6.5)
				axisstep=10.0;
			else if (floor(axisstep)==6.0)
				axisstep=5.0;
			else if (floor(axisstep)==4.0)
				axisstep=5.0;
			else if (floor(axisstep)==3.0)
				axisstep=2.0;
			else
				axisstep = floor(axisstep);
			axisstep *= pow(10.0,(floor(temp)));
		} else {
			axisstep = yspacing;
		}
		axisorigin = (floor(miny/axisstep)+1.0)*axisstep;
		for (temp = axisorigin;temp<=maxy;temp+=axisstep)  {
			if ((fabs(temp)<(1e-6))&&(axisstep>(1e-4)))  temp = 0.0;
			(void)sprintf(text,"%g",temp);
			length=strlen(text);
			printf("%.3f ",minx*scalex+offsetx);
			printf("%.3f m ",temp*scaley+offsety);
			printf("%.3f ",minx*scalex+offsetx+0.5);
			printf("%.3f l a\n",temp*scaley+offsety);
			printf("%.3f ",maxx*scalex+offsetx);
			printf("%.3f m ",temp*scaley+offsety);
			printf("%.3f ",maxx*scalex+offsetx-0.5);
			printf("%.3f l a\n",temp*scaley+offsety);
			storeleftx = minx*scalex+offsetx-0.29*length-0.12;
			if (minleftx > storeleftx)  minleftx=storeleftx;
			printf("%.3f ",storeleftx);
			printf("%.3f m ",temp*scaley+offsety);
			printf("(%g) show\n",temp);
		}
	}
	printf("/Helvetica findfont\n");
	printf("0.8 scalefont setfont\n");
	if (usexl==1)  {
		length=strlen(xlable);
		printf("%.3f ",((maxx-minx)*.5+minx)*scalex+offsetx-length*0.15);
		printf("%.3f m ",miny*scaley+offsety-1.5);
		printf("(%s) show\n",xlable);
	}
	if (useyl==1)  {
		length=strlen(ylable);
		printf("%.3f ",minleftx-0.4);
		printf("%.3f m ",((maxy-miny)*.5+miny)*scaley+offsety-length*0.15);
		printf("gsave\n");
		printf("90 rotate \n");
		printf("(%s) show\n",ylable);
		printf("grestore\n");
	}
	if (usetitle==1)  {
		printf("/Helvetica findfont\n");
		printf("1.0 scalefont setfont\n");
		length=strlen(title);
		printf("%.3f ",((maxx-minx)*.5+minx)*scalex+offsetx-length*0.23);
		printf("%.3f m ",maxy*scaley+offsety+1.5);
		printf("(%s) show\n",title);
	}

	if (traileron==1)  printtrailer();
	exit(0);
} /* END OF MAIN /* END OF MAIN /* END OF MAIN /* END OF MAIN /* END OF MAIN */















/* VARIOUS POSTSCRIPT STUFF */



void printheader(float linewidth)  {
/* ================================================================ */
/*  PostScript Header (originally taken from CGLE output, now much
/*  more condensed by me, after reading PostScript manual.)
 * ================================================================ */

printf("%%!PS-Adobe-1.0 \n");
printf("%%%%BoundingBox: -1 -1 766.354 567.929 \n");
printf("%%%%EndComments \n");
printf("%%%%EndProlog \n");
printf("gsave \n");
printf(" \n");
printf("%% /f {findfont exch scalefont setfont} bind def \n");
printf("/l {lineto} bind def \n");
printf("/m {newpath moveto} bind def \n");
printf("/sg {setgray} bind def\n");
printf("/sr {setrgbcolor} bind def\n");
printf("/a {stroke} bind def\n");
printf("/cp {closepath} bind def\n");
printf("/g {gsave} bind def\n");
printf("/h {grestore} bind def\n");
printf("/i {fill} bind def\n");
printf("%% make a circle\n");

printf("/c {%f 0 360 arc closepath stroke} bind def\n",circle);

printf("/cf {%f 0 360 arc closepath gsave %f %f %f setrgbcolor fill grestore stroke} bind def\n ",circle,fillcolor[0],fillcolor[1],fillcolor[2]);

printf(" /cc {%f 0 360 arc closepath gsave fill grestore 0 setgray stroke} bind def\n ",circle);

printf(" /ocf {%f 0 360 arc closepath %f %f %f setrgbcolor fill newpath} bind def\n ",circle,fillcolor[0],fillcolor[1],fillcolor[2]);

printf(" /occ {%f 0 360 arc closepath fill newpath} bind def\n ",circle);

printf(" matrix currentmatrix /originmat exch def \n");
printf(" /umatrix {originmat matrix concatmatrix setmatrix} def \n");

 printf("[28.3465 0 0 28.3465 10.5 100.0] umatrix\n");

printf("0 0 0 setrgbcolor\n");

printf("%f setlinewidth \n",linewidth);


/* END OF HEADER =============================================== */
}











void printtrailer()  {

printf("showpage \n");
printf("grestore \n");
printf("%%%%Trailer\n");

}
