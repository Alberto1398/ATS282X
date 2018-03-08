/*!
 * \file  stdarg.h
 * \brief �䳤����.
 */

#ifndef __included_stdarg_h
#define __included_stdarg_h


#ifndef __defined_va_list
#define __defined_va_list

/*!
 * \brief �䳤�����б�����.
 */
typedef char*  va_list;


#define __va_size(_x)  ((sizeof(_x) + 3) & ~3)


/*!
 * \brief �䳤������ʼ.
 */
#define va_start(_ap, _last_arg)  \
    \
    (_ap) = (((char*)&(_last_arg)) + __va_size(_last_arg))


/*!
 * \brief ��ȡ����.
 */
#define va_arg(_ap, _type)  \
    \
    (*(_type*)(((_ap) += __va_size(_type)) - __va_size(_type)))


/*!
 * \brief �䳤��������.
 */
#define va_end(_ap)  \
    \
    (_ap) = NULL


#endif  // __defined_va_list

#endif  // __included_stdarg_h

/* end of stdarg.h */


