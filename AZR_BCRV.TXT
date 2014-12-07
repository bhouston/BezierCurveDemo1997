
	Azure Bezier and B-Spline Examples

	plash  [bhouston@shl.com]
	July 29, 1997


Desciption

	This stuff isn't much.  Its just a little bit of what I wrote in the last
month or so.  There isn't that much source with b-splines or bezier patches
out there so here is my addition.  The techniques used can be speed up
tramendously, they couldn't be much slower ;).  I tried to do everything
by the book.  No optimized matrix multiplications or anything like that.
Notice that I am using bias matrices and control point matrices.


Finding the Normals

	The only thing of interest to people who have already done these types of
parametric curves it the way I calculate the normals to the curves and
surfaces.  Unlike the method taugh in AART (see below) I determine by normals
using pure calculus.  I use a derivative matrix and by multiplying the b-spline
or bezier bais matrix by the derivative matrix I determine the derivative bias
matrix.  Now by replacing the normal bias matrix with the derivative bias matrix
I now calculate a vector tangent to the curve at the supplied parameter.  (I
am proud of the fact that I came up with the idea of a derivative matrix and
actually found a use for it.  If anyone has heard of it before please tell me
from where you heard it.  Thanks! )


How To Do This

	Get either Advance Animation and Rendering Techniques(AART from above) or
Computer Graphics Principles and Practice.  For the subject of parametric
curves, AART is probably the better choice.  Personally I have a copy of both.


Tell Me What You Think

	If you have any suggestions or questions I will respond the best I can.
E-mail me at work to dispell the bordem: bhouston@shl.com !!!  Or join #coders
at "irc.stealth.net"!!!


Hellos to...

	AZURE: Asch, Mundane.
	INTRA: Omega, Perlude, the guy with 3DSMAX,
		and the guy who loves Final Fantasy!
	#Coders: Xyz (nothing), Jaffar (isreali guy),
		and Bimba (thanks for the blobbies).
	And of course anyone who loves the scene !!!!!
