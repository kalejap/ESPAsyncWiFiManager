/*
  LanguageSupport.h - Defines macro for language support
  Copyright (c) 2024 Peter Kaleja.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef LANGUAGESUPPORT_H
#define LANGUAGESUPPORT_H

// Conditional inclusion of language files
#ifdef LANGUAGE_EN_US
    #include "language/wm_en_US.h"
#elif LANGUAGE_SK_SK
    #include "language/wm_sk_SK.h"
#elif LANGUAGE_DE_DE
    #include "language/wm_de_DE.h"
#else
    // Default language if none is defined (e.g., English)
    #include "language/wm_en_US.h" 
    #warning "No language defined, defaulting to English."
#endif

#endif // LANGUAGESUPPORT_H