CFLAGS=-Wall -Wno-unused-parameter -Wextra -lglfw -lGL -lm -lGLEW

glsli: glsli.c
	cc $CFLAGS -o $target $prereq

clean:V:
	rm -f glsli
