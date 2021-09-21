//
// Created by ultimatenick on 17/09/21.
//

#ifndef CPP_GLERROR_HPP
#define CPP_GLERROR_HPP

void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#endif //CPP_GLERROR_HPP
