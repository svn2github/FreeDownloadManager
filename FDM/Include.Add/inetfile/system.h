#ifndef __IF_SYSTEM_H_
#define __IF_SYSTEM_H_

extern void fsIFOnMemoryError ();

#pragma warning (disable:4127)
// our replacement for "new" operator
// will call fsIFOnMemoryError function if failed to allocate requested 
// amount of memory
#define fsnew(p, T, L) while (1) { try { p = new T [L]; if (p == NULL) fsIFOnMemoryError (); else break; } catch (...) {fsIFOnMemoryError ();} }

#endif