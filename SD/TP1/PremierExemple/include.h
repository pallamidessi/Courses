#ifndef INCRPC
#define INCRPC  

#include <stdio.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/rpc.h>
/*Sur une machine SPARC, ajouter :
  #include <rpc/clnt_soc.h>
  #include <rpc/svc_soc.h> */

/* regarder dans /usr/include/types.h et /usr/include/rpc/xdr.h
   les definitions de bool_t et xdrproc_t qui correspondent respectivement
   aux deux types suivants "monboolean" et "pointeur_fonction" */
typedef int monboolean;
typedef monboolean (*pointeur_fonction) (XDR *, void *,...);
#define PROGNUM 0x20000100
#define VERSNUM 1
#define PROCNUM 1

#endif /* INCRPC */


