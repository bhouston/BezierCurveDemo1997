////////////////////////////////////////////////////////
//
//		Curves  -  A B-Spline Mystify Screen Saver
//
//			Coded by Ben Houston
//
//			Created on July 11, 1997
//			Updated on July 29, 1997
//

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>

#include "azrmath.h"

#define	NUM_CONTROLPTS	(7)
#define	QUEUE_SIZE		(10)

#define	MAX_SCREEN_X	(640)
#define	MAX_SCREEN_Y   (480)
#define	MAX_DELTA		(15)

typedef struct
{
	int	x, y;
}	POINT2;

POINT2		vertices[NUM_CONTROLPTS];
POINT2		deltas[NUM_CONTROLPTS];

int			index;
POINT2		queue[QUEUE_SIZE][NUM_CONTROLPTS];

void	initcontrol ( POINT2 *vertex, POINT2 *delta );
void	advancecontrol ( POINT2 *vertex, POINT2 *delta );
void	drawcurve ( POINT2 *a, POINT2 *b, POINT2 *c, POINT2 *d );


void main ( void )
{
	int	gdriver = 9, gmode = 2, errorcode;
	int	i, color;

	printf ( "Mystify  -  B-Spline Screen Saver\n" );
	printf ( "coded by plash  [ bhouston@shl.com ]  %s %s\n", __DATE__, __TIME__ );
	delay ( 2000 );
	clrscr ();
	delay ( 100 );

	// enter vga 640x480 16 color mode (params: 9,2)
	initgraph( &gdriver, &gmode, "");
	errorcode = graphresult();
	if ( errorcode != grOk )
	{
		printf ( "\n" );
		printf ( "Graphics error : %s\n", grapherrormsg ( errorcode ) );
		printf ( "Press any key to quit..." );
		getch ();
		exit ( 1 );
	}

	randomize ();

	for ( i = 0; i < NUM_CONTROLPTS; i ++ )
		initcontrol ( &vertices[i], &deltas[i] );

	color = random ( 15 ) + 1;

	while ( ! kbhit () )
	{
		delay ( 80 );

		if ( random ( 100 ) == 0 )
			color = random ( 15 ) + 1;

		for ( i = 0; i < NUM_CONTROLPTS; i ++ )
			advancecontrol ( &vertices[i], &deltas[i] );

		for ( i = 0; i < NUM_CONTROLPTS; i ++ )
		{
			setcolor ( 0 );
			drawcurve (	&queue[index][i],
							&queue[index][( i + 1 ) % NUM_CONTROLPTS],
							&queue[index][( i + 2 ) % NUM_CONTROLPTS],
							&queue[index][( i + 3 ) % NUM_CONTROLPTS] );
		}

		for ( i = 0; i < NUM_CONTROLPTS; i ++ )
		{
			setcolor ( color );
			drawcurve (	&vertices[i],
							&vertices[( i + 1 ) % NUM_CONTROLPTS],
							&vertices[( i + 2 ) % NUM_CONTROLPTS],
							&vertices[( i + 3 ) % NUM_CONTROLPTS] );

			queue[index][i] = vertices[i];
		}

		index = ( index + 1 ) % QUEUE_SIZE;
	}

	// clean up
	getch ();
	clearviewport ();
	delay ( 100 );
	closegraph ();
	delay ( 200 );
}

void	initcontrol ( POINT2 *vertex, POINT2 *delta )
{
	vertex->x = random ( MAX_SCREEN_X - 20 ) + 10;
	vertex->y = random ( MAX_SCREEN_Y - 20 ) + 10;

	delta->x = random ( MAX_DELTA ) + 1;
	delta->y = random ( MAX_DELTA ) + 1;
}

void	advancecontrol ( POINT2 *vertex, POINT2 *delta )
{
	float	x, y;

	x = vertex->x + delta->x;
	y = vertex->y + delta->y;

	if ( x < 0 )
		delta->x = ( random ( MAX_DELTA ) + 1 );
	if ( x >= MAX_SCREEN_X )
		delta->x = -( random ( MAX_DELTA ) + 1 );

	if ( y < 0 )
		delta->y = ( random ( MAX_DELTA ) + 1 );
	if ( y >= MAX_SCREEN_Y )
		delta->y = -( random ( MAX_DELTA ) + 1 );

	vertex->x += delta->x;
	vertex->y += delta->y;
}

void	drawcurve ( POINT2 *a, POINT2 *b, POINT2 *c, POINT2 *d )
{
	float i, controlpts[4][2], pt[2], coeffvector[4];

	controlpts[0][0] = a->x;
	controlpts[0][1] = a->y;
	controlpts[1][0] = b->x;
	controlpts[1][1] = b->y;
	controlpts[2][0] = c->x;
	controlpts[2][1] = c->y;
	controlpts[3][0] = d->x;
	controlpts[3][1] = d->y;

	getbsplinept_2 ( 0, controlpts, pt );
	moveto ( pt[0], pt[1] );

	for ( i = 1; i <= 20; i ++ )
	{
		getcoeffvector ( i * 0.05, bsplinebais, coeffvector );
		getweightedpt_2 ( coeffvector, controlpts, pt );
		lineto ( pt[0], pt[1] );
	}
}
