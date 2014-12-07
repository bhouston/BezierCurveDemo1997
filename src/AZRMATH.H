////////////////////////////////////////////////////////////////////////////////
//
//		AZRMATH.H
//			A collection of math functions useful in 3D rendering.
//			(just another library by plash)
//
//			Started on June  1, 1997
//			Updated on July 25, 1997
//


#ifndef	__AZR_MATH_H__
#define	__AZR_MATH_H__


extern	float		identitymtx[4][4];

extern	float		benzierbais[4][4];
extern	float		benzierdrvt[4][4];
extern	float		bsplinebais[4][4];
extern	float		bsplinedrvt[4][4];


enum AXISES { X_AXIS, Y_AXIS, Z_AXIS };


void	mulmatrix_4x4 ( float result[][4], float left[][4], float right[][4] );
void	mulrowmatrix_4x4 ( float *result, float *left, float right[][4] );

void	mulmatrixcol_4x4 ( float *result, float left[][4], float *right );
void	mulrowcol_4x4 ( float result[][4], float *left, float *right );

void	mullinearcol_4x4 ( float *pt, float lineararray[][3], float *vcoeff );
void	mulrowpatch_4x4 ( float lineararray[][3], float *ucoeff, float patcharray[][4][3] );

void	scalematrix_4x4 ( float result[][4], float matrix[][4], float scale );
void	copymatrix_4x4 ( float dest[][4], float source[][4] );

void	setrotation_4x4 ( float matrix[][4], int axis, float radian );
void	settranslation_4x4 ( float matrix[][4], float *translation );

void	getranspose_4x4 ( float transpose[][4], float source[][4] );

void	crossproduct_3 ( float *result, float *left, float *right );

void	getcoeffvector ( float param, float baismatrix[][4], float *coeffvector );
void	getweightedpt_2 ( float *coeffvector, float controlpts[][2], float *pt );

void	normalize_2 ( float *nrm );
void	normalize_3 ( float *nrm );

void	getbsplinept_2 ( float param, float controlpts[][2], float *pt );
void	getbsplinenrm_2 ( float param, float controlpts[][2], float *nrm );

void	getbenzierpatchpt_3 ( float u, float v, float patcharray[][4][3], float *pt );
void	getbenzierpatchnrm_3 ( float u, float v, float patcharray[][4][3], float *nrm );


#endif	// __AZR_MATH_H__

