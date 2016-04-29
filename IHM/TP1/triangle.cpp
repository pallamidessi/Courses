#include "triangle.h"

bool Triangle::IsPointInTriangle(int x, int y)
{
	int fAB = (y-p1.y)*(p2.x-p1.x) - (x-p1.x)*(p2.y-p1.y);
	int fCA = (y-p3.y)*(p1.x-p3.x) - (x-p3.x)*(p1.y-p3.y);
	int fBC = (y-p2.y)*(p3.x-p2.x) - (x-p2.x)*(p3.y-p2.y);
	if ((fAB*fBC)>0 && (fBC*fCA)>0)
		return true;
	else
		return false;
}
