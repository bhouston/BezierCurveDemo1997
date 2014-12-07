///////////////////////////////////////////////////////////////////////////////
//
//		Teapot.Cpp
//
//			The Classic Benzier Patch Utah Teapot
//			[another source by plash]
//
//			Started on July 27, 1997
//			Updated on July 29, 1997
//

#include "azrmath.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <graphics.h>
#include <mem.h>

typedef struct {
	float	x, y, z;
}	POINT3;

typedef struct {
	POINT3	control[4][4];
}	PATCH;

typedef struct {
	char	name[30];
	int	patch_count;
	PATCH *patch;
}	GROUP;

typedef struct {
	char	name[30];
	int	group_count;
	GROUP	*group;
}	MESH;

#define	BEZIER_STEPS	5

float		cameramtx[4][4] =
{	{  2,  0,  0,  0 },
	{  0,  2,  0,  0 },
	{  0,  0,  1,  1 },
	{  0,  0, -1,  0 } };
float		stepmtx[4][4];
float		worldmtx[4][4];
float		viewmtx[4][4];
float		projectionmtx[4][4];

float		controlpts[4][4][3];
float		patchpts[BEZIER_STEPS+1][BEZIER_STEPS+1][3];
float		patchnrms[BEZIER_STEPS+1][BEZIER_STEPS+1][3];

void	loaddataset ( char *filename, MESH *mesh_out );

void	drawcontrolgrid ( int color );
void	calculatemesh ( void );
void	drawmesh ( int color );
void	calculatemeshnrms ( void );
void	drawmeshnrms ( int color );

void	setup3dsystem ( void );
void	updateprojection ( void );
void	projectpt_3 ( float *screen, float *world );


void main ( void )
{
	MESH	teapot;
	int	patch_total, group_index;
	int	gdriver = 9, gmode = 2, errorcode;

	// intro banner
	printf ( "TEAPOT.EXE  -  Utah Teapot Rendering" );
	printf ( "\ncoded by plash  [ bhouston@shl.com ]  %s %s\n", __DATE__, __TIME__ );
	printf ( "\n" );
	delay ( 2000 );
	clrscr ();
	delay ( 100 );

	// load the teapot benzier curves
	loaddataset ( "utah.txt", &teapot );
	setup3dsystem ();

	// set the graphics mode
	initgraph( &gdriver, &gmode, "");
	errorcode = graphresult();
	if ( errorcode != grOk ) {
		printf ( "\n" );
		printf ( "Graphics error : %s\n", grapherrormsg ( errorcode ) );
		printf ( "Press any key to quit..." );
		getch ();
		exit ( 1 );
		}
	delay ( 100 );

	printf ( "Bezier Mesh: %s", teapot.name );

	patch_total = 0;

	for ( group_index = 0; group_index < teapot.group_count; group_index ++ )
	{
		patch_total += teapot.group[group_index].patch_count;
	}

	printf ( "\n   Patch Groups:   %i", teapot.group_count );
	printf ( "\n   Subdivisions:   %i", BEZIER_STEPS );
	printf ( "\n   Total Patches:  %i", patch_total );
	printf ( "\n   Total Controls: %i", patch_total * 16 );
	printf ( "\n   Total Faces:    %i", patch_total * BEZIER_STEPS * BEZIER_STEPS );
	printf ( "\n   Total Vertices: %i", patch_total * BEZIER_STEPS * BEZIER_STEPS );

	for ( group_index = 0; group_index < teapot.group_count; group_index ++ )
	{
		GROUP	group;

		group = teapot.group[group_index];
		for ( int patch_index = 0; patch_index < group.patch_count; patch_index ++ )
		{
			PATCH	patch = group.patch[patch_index];

			memcpy ( controlpts, patch.control, sizeof ( float ) * 4 * 4 * 3 );
			drawcontrolgrid ( 2 );
		}
	}

	getch ();

	for ( group_index = 0; group_index < teapot.group_count; group_index ++ )
	{
		GROUP	group;

		group = teapot.group[group_index];
		for ( int patch_index = 0; patch_index < group.patch_count; patch_index ++ )
		{
			PATCH	patch = group.patch[patch_index];

			memcpy ( controlpts, patch.control, sizeof ( float ) * 4 * 4 * 3 );
			drawcontrolgrid ( 8 );
		}
	}
	for ( group_index = 0; group_index < teapot.group_count; group_index ++ )
	{
		GROUP	group;

		group = teapot.group[group_index];
		for ( int patch_index = 0; patch_index < group.patch_count; patch_index ++ )
		{
			PATCH	patch = group.patch[patch_index];

			memcpy ( controlpts, patch.control, sizeof ( float ) * 4 * 4 * 3 );
			calculatemesh ();
			drawmesh ( 9 );
		}
	}

	getch ();

	for ( group_index = 0; group_index < teapot.group_count; group_index ++ )
	{
		GROUP	group;

		group = teapot.group[group_index];
		for ( int patch_index = 0; patch_index < group.patch_count; patch_index ++ )
		{
			PATCH	patch = group.patch[patch_index];

			memcpy ( controlpts, patch.control, sizeof ( float ) * 4 * 4 * 3 );
			calculatemeshnrms ();
			drawmesh ( 9 );
			drawmeshnrms ( 4 );
		}
	}

	// clean up and quit
	getch ();
	clearviewport ();
	delay ( 100 );
	closegraph ();
	delay ( 200 );
}

void	calculatemesh ( void )
{
	int	i, j;
	float	u, v;

	for ( i = 0; i <= BEZIER_STEPS; i ++ ) {
		for ( j = 0; j <= BEZIER_STEPS; j ++ ) {
			u = i / (float)BEZIER_STEPS;
			v = j / (float)BEZIER_STEPS;
			getbenzierpatchpt_3 ( u, v, controlpts, patchpts[i][j] );
			}
		}
}

void	calculatemeshnrms ( void )
{
	int	i, j;
	float	u, v;

	for ( i = 0; i <= BEZIER_STEPS; i ++ ) {
		for ( j = 0; j <= BEZIER_STEPS; j ++ ) {
			u = i / (float)BEZIER_STEPS;
			v = j / (float)BEZIER_STEPS;
			getbenzierpatchpt_3 ( u, v, controlpts, patchpts[i][j] );
			getbenzierpatchnrm_3 ( u, v, controlpts, patchnrms[i][j] );
			normalize_3 ( patchnrms[i][j] );
			}
		}
}

void	drawmesh ( int color )
{
	int	i, j;
	float	a[2], b[2], c[2];

	setcolor ( color );

	for ( i = 0; i <= BEZIER_STEPS; i ++ ) {
		for ( j = 0; j <= BEZIER_STEPS; j ++ ) {

			projectpt_3 ( a, patchpts[i][j] );

			if ( i < BEZIER_STEPS ) {
				projectpt_3 ( b, patchpts[i+1][j] );
				line ( a[0], a[1], b[0], b[1] );
				}

			if ( j < BEZIER_STEPS ) {
				projectpt_3 ( c, patchpts[i][j+1] );
				line ( a[0], a[1], c[0], c[1] );
				}
			}
		}
}

void	drawmeshnrms ( int color )
{
	int	i, j;
	float	a[2], b[2], nrmend[3];

	setcolor ( color );

	for ( i = 0; i <= 10; i ++ ) {
		for ( j = 0; j <= 10; j ++ ) {
			nrmend[0] = patchpts[i][j][0] - patchnrms[i][j][0] * 10;
			nrmend[1] = patchpts[i][j][1] - patchnrms[i][j][1] * 10;
			nrmend[2] = patchpts[i][j][2] - patchnrms[i][j][2] * 10;

			projectpt_3 ( a, patchpts[i][j] );
			projectpt_3 ( b, nrmend );

			line ( a[0], a[1], b[0], b[1] );
			}
		}
}

void	drawcontrolgrid ( int color )
{
	int	i, j;
	float	pt1[2], pt2[2];

	setcolor ( color );
	for ( i = 0; i < 4; i ++ ) {
		for ( j = 0; j < 4; j ++ ) {

			projectpt_3 ( pt1, controlpts[i][j] );

			if ( i < 3 ) {
				projectpt_3 ( pt2, controlpts[i+1][j] );
				line ( pt1[0], pt1[1], pt2[0], pt2[1] );
				}

			if ( j < 3 ) {
				projectpt_3 ( pt2, controlpts[i][j+1] );
				line ( pt1[0], pt1[1], pt2[0], pt2[1] );
				}
			}
		}
}

void	loaddataset ( char *filename, MESH *mesh_out )
{
	FILE	*file;
	MESH	mesh;
	int	group_index, patch_index, control_index;

	file = fopen ( filename, "rb" );
	if ( file == NULL )
	{
		printf ( "\nERROR - cannot open '%s'", filename );
		exit ( 1 );
	}

	fscanf ( file, "%i %s\n", &mesh.group_count, &mesh.name );
	mesh.group = new GROUP[mesh.group_count];

	for ( group_index = 0; group_index < mesh.group_count; group_index ++ )
	{
		GROUP	group;

		fscanf ( file, "%i %s\n", &group.patch_count, &group.name );
		group.patch = new PATCH[group.patch_count];

		for ( patch_index = 0; patch_index < group.patch_count; patch_index ++ )
		{
			PATCH patch;

			for ( control_index = 0; control_index < 4; control_index ++ )
			{
				int	count;

				count = fscanf ( file, " %f %f %f   %f %f %f   %f %f %f   %f %f %f\n",
					&patch.control[control_index][0].x, &patch.control[control_index][0].y,
					&patch.control[control_index][0].z,
					&patch.control[control_index][1].x, &patch.control[control_index][1].y,
					&patch.control[control_index][1].z,
					&patch.control[control_index][2].x, &patch.control[control_index][2].y,
					&patch.control[control_index][2].z,
					&patch.control[control_index][3].x, &patch.control[control_index][3].y,
					&patch.control[control_index][3].z );

				if ( count != 12 )
				{
					printf ( "\nERROR - reading control point data in %s:%s:%i:%i",
						mesh.name, group.name, patch_index, control_index );
					exit ( 1 );
				}
			}

			group.patch[patch_index] = patch;
		}

		mesh.group[group_index] = group;
	}

	*mesh_out = mesh;

	fclose ( file );
}

void	setup3dsystem ( void )
{
	float	tempmtx[4][4], rotationmtx[4][4];

	// view matrix setup

	copymatrix_4x4 ( viewmtx, identitymtx );

	setrotation_4x4 ( rotationmtx, X_AXIS, M_PI/4 );
	mulmatrix_4x4 ( tempmtx, viewmtx, rotationmtx );
	copymatrix_4x4 ( viewmtx, tempmtx );

	viewmtx[3][0] = 0;
	viewmtx[3][1] = 0;
	viewmtx[3][2] = 500;

	// world matrix setup

	copymatrix_4x4 ( worldmtx, identitymtx );

	setrotation_4x4 ( rotationmtx, X_AXIS, M_PI/2 );
	mulmatrix_4x4 ( tempmtx, worldmtx, rotationmtx );
	copymatrix_4x4 ( worldmtx, tempmtx );

	worldmtx[3][1] = 100;

	// create composite projection matrix

	updateprojection ();
}

void	updateprojection ( void )
{
	float	tempmtx[4][4];

	// projection = world * view * camera;

	copymatrix_4x4 ( projectionmtx, worldmtx );
	mulmatrix_4x4 ( tempmtx, projectionmtx, viewmtx );
	mulmatrix_4x4 ( projectionmtx, tempmtx, cameramtx );
}

void	projectpt_3 ( float *screen, float *world )
{
	float		worldvector[4], screenvector[4];

	worldvector[0] = world[0];
	worldvector[1] = world[1];
	worldvector[2] = world[2];
	worldvector[3] = 1.0;

	mulrowmatrix_4x4 ( screenvector, worldvector, projectionmtx );

	if ( screenvector[3] == 0 ) {
		screen[0] = 320;
		screen[1] = 200;
		}
	else {
		screen[0] = screenvector[0] / screenvector[3] * 480 + 320;
		screen[1] = screenvector[1] / screenvector[3] * 400 + 200;
//		screen[2] = screenvector[2] / screenvector[3];
		}
}
