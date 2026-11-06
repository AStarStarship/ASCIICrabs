// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsAddressH
#define _CrabsAddressH
#include <_Config.h>
#if _Seam >= _CrabsOp
/* @defgroup Address
@brief Crabs Address utilities.
*/
namespace _ {

/* Returns a pointer to static variable for writing the address { a, \0 } to.
 */
IUB PackAddress(IUA a);

/* Returns a pointer to static variable for writing the address 
{ a, b, \0, \0 } to. */
IUC PackAddress(IUC a, IUC b);

/* Returns a pointer to static variable for writing the address { a, b, c, \0 }
to. */
IUC PackAddress(IUC a, IUC b, IUC c);

/* Returns a pointer to static variable for writing the address { a, b, c, d,
e, f, g, \0 } to. */
IUD PackAddress(IUD a, IUD b, IUD c, IUD d);

/* Returns a pointer to static variable for writing the address { a, b, c, d,
e, f, g, \0 } to. */
IUD PackAddress(IUD a, IUD b, IUD c, IUD d, IUD e);

/* Returns a pointer to static variable for writing the address
{ a, b, c, d, e, f, g, \0 } to. */
IUD PackAddress(IUD a, IUD b, IUD c, IUD d, IUD e, IUD f);

/* Returns a pointer to static variable for writing the address
{ a, b, c, d, e, f, g, \0 } to. */
IUD PackAddress(IUD a, IUD b, IUD c, IUD d, IUD e, IUD f, IUD g);

}  //< namespace _
#endif
#endif
