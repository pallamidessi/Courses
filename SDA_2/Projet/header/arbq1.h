#ifndef _ARBQ1_H__
#define _ARBQ1_H__


#include"arbq.h"


arbq damier(int n);
arbq symh(arbq a);
arbq symv(arbq a);
arbq rotg(arbq a);
arbq rotd(arbq a);
arbq dzoo(arbq a);

arbq parc(arbq a,void(*operation)(arbq));
arbq invc(arbq a);
arbq nivg(arbq a);
arbq tresh(arbq a,int seuil);
