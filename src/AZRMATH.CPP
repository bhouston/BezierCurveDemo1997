////////////////////////////////////////////////////////////////////////////////
//
//		AZRMATH.CPP
//			A collection of math functions useful in 3D rendering.
//			(just another library by plash)
//
//			Started on June  1, 1997
//			Updated on July 27, 1997
//

#include "azrmath.h"

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <math.h>


float		identitymtx[4][4] =
{	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 } };

float		benzierbais[4][4] =
{	{ -1,  3, -3,  1 },
	{  3, -6,  3,  0 },
	{ -3,  3,  0,  0 },
	{  1,  0,  0,  0 }  };

float		benzierdrvt[4][4] =
{	{  0,  0,  0,  0 },
	{ -3,  9, -9,  3 },
	{  6,-12,  6,  0 },
	{ -3,  3,  0,  0 }  };

float		bsplinebais[4][4] =
{	{ -1.0/6,  3.0/6, -3.0/6,  1.0/6 },
	{  3.0/6, -6.0/6,  3.0/6,  0     },
	{ -3.0/6,  0.0/6,  3.0/6,  0     },
	{  1.0/6,  4.0/6,  1.0/6,  0     } };

float		bsplinedrvt[4][4] =
{	{      0,       0,      0,  0     },
	{ -3.0/6,   9.0/6, -9.0/6,  3.0/6 },
	{  6.0/6, -12.0/6,  6.0/6,  0     },
	{ -3.0/6,   0.0/6,  3.0/6,  0     } };



void	gettranspose_4x4 ( float transpose[][4], float source[][4] )
{
	int	i, j;

	for ( i = 0; i < 4; i ++ ) {
		for ( j = 0; j < 4; j ++ ) {
			transpose[i][j] = source[j][i];
			}
		}
}

void	mulmatrix_4x4 ( float result[][4], float left[][4], float right[][4] )
{
	int	i, j;

	for ( i = 0; i < 4; i ++ ) {
		for ( j = 0; j < 4; j ++ ) {
			result[j][i] = left[j][0] * right[0][i] + left[j][1] * right[1][i] +
								left[j][2] * right[2][i] + left[j][3] * right[3][i];
			}
		}
}

void	mulrowmatrix_4x4 ( float *result, float *left, float right[][4] )
{
	int	i;

	for ( i = 0; i < 4; i ++ ) {
		result[i] = left[0] * right[0][i] + left[1] * right[1][i] +
						left[2] * right[2][i] + left[3] * right[3][i];
		}
}

void	mulrowcol_4x4 ( float result[][4], float *left, float *right )
{
	int	i, j;

	for ( i = 0; i < 4; i ++ ) {
		for ( j = 0; j < 4; j ++ ) {
			result[j][i] = left[i] * right[j];
			}
		}
}

void	mulmatrixcol_4x4 ( float *result, float left[][4], float *right )
{
	int	i;

	for ( i = 0; i < 4; i ++ ) {
		result[i] = left[i][0] * right[0] + left[i][1] * right[1] +
						left[i][2] * right[2] + left[i][3] * right[3];
		}
}

void	scalematrix_4x4 ( float result[][4], float matrix[][4], float scale )
{
	int	i, j;

	for ( i = 0; i < 4; i ++ ) {
		for ( j = 0; j < 4; j ++ ) {
			result[i][j] = matrix[i][j] * scale;
			}
		}
}

void	copymatrix_4x4 ( float dest[][4], float source[][4] )
{
	memcpy ( dest, source, sizeof ( float ) * 16 );
}

void	settranslation_4x4 ( float matrix[][4], float *translation )
{
	copymatrix_4x4 ( matrix, identitymtx );

	matrix[3][0] = translation[0];
	matrix[3][1] = translation[1];
	matrix[3][2] = translation[2];
}

void	setrotation_4x4 ( float matrix[][4], int axis, float radian )
{
	int	a, b;
	float	cosvalue, sinvalue;

	copymatrix_4x4 ( matrix, identitymtx );

	cosvalue = cos ( radian );
	sinvalue = sin ( radian );

	switch ( axis ) {
		case X_AXIS:
			a = 1; b = 2; break;
		case Y_AXIS:
			a = 0; b = 2; break;
		case Z_AXIS:
			a = 0; b = 1; break;
		}

	matrix[a][a] = cosvalue;
	matrix[b][b] = cosvalue;
	matrix[a][b] = sinvalue;
	matrix[b][a] = -sinvalue;
}

void	getweightedpt_2 ( float *coeffvector, float controlpts[][2], float *pt )
{
	pt[0] = coeffvector[0] * controlpts[0][0] +
				coeffvector[1] * controlpts[1][0] +
				coeffvector[2] * controlpts[2][0] +
				coeffvector[3] * controlpts[3][0];
	pt[1] = coeffvector[0] * controlpts[0][1] +
				coeffvector[1] * controlpts[1][1] +
				coeffvector[2] * controlpts[2][1] +
				coeffvector[3] * controlpts[3][1];
}

void	getcoeffvector ( float param, float baismatrix[][4], float *coeffvector )
{
	float		termvector[4];

	termvector[0] = param * param * param;
	termvector[1] = param * param;
	termvector[2] = param;
	termvector[3] = 1;

	mulrowmatrix_4x4 ( coeffvector, termvector, baismatrix );
}

void	getbsplinept_2 ( float param, float controlpts[][2], float *pt )
{
	int	i;
	float	t, coeffvector[4];

	i = floor ( param ) + 2;
	t = param - floor ( param );

	getcoeffvector ( t, bsplinebais, coeffvector );
	getweightedpt_2 ( coeffvector, &controlpts[i-2], pt );
}

void	getbsplinenrm_2 ( float param, float controlpts[][2], float *nrm )
{
	int	i;
	float	t, coeffvector[4];

	i = floor ( param ) + 2;
	t = param - floor ( param );

	getcoeffvector ( t, bsplinedrvt, coeffvector );
	getweightedpt_2 ( coeffvector, &controlpts[i-2], nrm );
}

void	normalize_2 ( float *nrm )
{
	float	length;

	length = sqrt ( nrm[0] * nrm[0] + nrm[1] * nrm[1] );

	nrm[0] /= length;
	nrm[1] /= length;
}

void	mulrowpatch_4x4 ( float lineararray[][3], float *ucoeff, float patcharray[][4][3] )
{
	int	i, j;

	for ( i = 0; i < 4; i ++ ) {
		lineararray[i][0] = 0;
		lineararray[i][1] = 0;
		lineararray[i][2] = 0;
		for ( j = 0; j < 4; j ++ ) {
			lineararray[i][0] += ucoeff[j] * patcharray[i][j][0];
			lineararray[i][1] += ucoeff[j] * patcharray[i][j][1];
			lineararray[i][2] += ucoeff[j] * patcharray[i][j][2];
			}
		}
}

void	mullinearcol_4x4 ( float *pt, float lineararray[][3], float *vcoeff )
{
	int	i;

	pt[0] = 0;
	pt[1] = 0;
	pt[2] = 0;

	for ( i = 0; i < 4; i ++ ) {
		pt[0] += lineararray[i][0] * vcoeff[i];
		pt[1] += lineararray[i][1] * vcoeff[i];
		pt[2] += lineararray[i][2] * vcoeff[i];
		}
}

/*
void	getweightedsurfacept_3 ( float baismatrix[][4], float patcharray[][4][3], float *pt )
{
	int	i, j;

	pt[0] = 0;
	pt[1] = 0;
	pt[2] = 0;

	for ( i = 0; i < 4; i ++ ) {
		for ( j = 0; j < 4; j ++ ) {
			pt[0] += baismatrix[i][j] * patcharray[i][j][0];
			pt[1] += baismatrix[i][j] * patcharray[i][j][1];
			pt[2] += baismatrix[i][j] * patcharray[i][j][2];
			}
		}
}
*/

void		getbenzierpatchpt_3 ( float u, float v, float patcharray[][4][3], float *pt )
{
	float		uvector[4], vvector[4];
	float		ucoeff[4], vcoeff[4];
	float		lineararray[4][3];
	float		transposebais[4][4];

	uvector[0] = u * u * u;
	uvector[1] = u * u;
	uvector[2] = u;
	uvector[3] = 1;

	vvector[0] = v * v * v;
	vvector[1] = v * v;
	vvector[2] = v;
	vvector[3] = 1;

	mulrowmatrix_4x4 ( ucoeff, uvector, benzierbais );

	gettranspose_4x4 ( transposebais, benzierbais );
	mulmatrixcol_4x4 ( vcoeff, benzierbais, vvector );

	mulrowpatch_4x4 ( lineararray, ucoeff, patcharray );
	mullinearcol_4x4 ( pt, lineararray, vcoeff );
}

void	crossproduct_3 ( float *result, float *left, float *right )
{
	result[0] = ( left[1] * right[2] ) - ( left[2] * right[1] );
	result[1] = ( left[2] * right[0] ) - ( left[0] * right[2] );
	result[2] = ( left[0] * right[1] ) - ( left[1] * right[0] );
}

void	normalize_3 ( float *nrm )
{
	float	length, temp;

	temp = nrm[0] * nrm[0] + nrm[1] * nrm[1] + nrm[2] * nrm[2];
	if ( temp == 0 )
	{
		temp = 1;
	}

	length = sqrt ( temp );
	if ( length == 0 )
	{
		printf ( "\nERROR - divide by zero in azrmath:normalize_3" );
		exit ( 1 );
	}

	nrm[0] /= length;
	nrm[1] /= length;
	nrm[2] /= length;
}

void		getbenzierpatchnrm_3 ( float u, float v, float patcharray[][4][3], float *nrm )
{
	float		uvector[4], vvector[4];
	float		ucoeff[4], vcoeff[4];
	float		du[4], dv[4];
	float		lineararray[4][3];
	float		transposebais[4][4];

	uvector[0] = u * u * u;
	uvector[1] = u * u;
	uvector[2] = u;
	uvector[3] = 1;

	vvector[0] = v * v * v;
	vvector[1] = v * v;
	vvector[2] = v;
	vvector[3] = 1;

	mulrowmatrix_4x4 ( ucoeff, uvector, benzierdrvt );

	gettranspose_4x4 ( transposebais, benzierbais );
	mulmatrixcol_4x4 ( vcoeff, transposebais, vvector );

	mulrowpatch_4x4 ( lineararray, ucoeff, patcharray );
	mullinearcol_4x4 ( du, lineararray, vcoeff );


	mulrowmatrix_4x4 ( ucoeff, uvector, benzierbais );

	gettranspose_4x4 ( transposebais, benzierdrvt );
	mulmatrixcol_4x4 ( vcoeff, transposebais, vvector );

	mulrowpatch_4x4 ( lineararray, ucoeff, patcharray );
	mullinearcol_4x4 ( dv, lineararray, vcoeff );

	crossproduct_3 ( nrm, du, dv );
}



