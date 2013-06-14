/*
     serenity
     Copyright (C) 2013 Andrea Nardinocchi (nardinocchi@psychogames.net)
     
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
#include "o_filesystem.h"
const char v_filesystem_kind[] = "filesystem";
void p_filesystem_hooking(struct o_filesystem *object) {
	object->head.s_delegate.m_delete = NULL;
	object->head.s_delegate.m_compare = NULL;
	object->head.s_delegate.m_hash = NULL;
	object->head.s_delegate.m_string = NULL;
	object->head.s_delegate.m_clone = NULL;
}
