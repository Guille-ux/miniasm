#ifndef MINIASM_COMPAT_H
#define MINIASM_COMPAT_H

/*
 * Esto debe incluir las declaraciones de:
 *
 * Malloc y Free
 * memcpy y memset
 *
 * (NOTA: No uso nada de FOPEN etc, por compatibilidad, a cambio
 *  el ensamblador devolvera los BLOBS binarios directamente)
 *
 */

#include <stdlib.h> // para malloc y free
#include <string.h> // para memcpy y memset

#endif
