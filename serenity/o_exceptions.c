/*
     serenity
     Copyright (C) 2013 Andrea Nardinocchi (andrea@nardinan.it)
     
     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "o_exceptions.h"
d_exception_define(unsupported, 1, "unsupported operation exception");
d_exception_define(constant, 	2, "constant value exception");
d_exception_define(malformed,	3, "malformed input exception");
d_exception_define(unreachable, 4, "unreachable data exception");
d_exception_define(closed,		5, "closed stream exception");
d_exception_define(null,		6, "null pointer exception");
d_exception_define(bound,		7, "out of bound exception");