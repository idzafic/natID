// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include <locale.h>
#include <td/String.h>

namespace mu
{

/*
    ScopedCLocale

    Temporarily switches the C locale and restores the previous locale
    automatically when the object goes out of scope.

    Example:

        {
            ScopedCLocale locale(LC_NUMERIC, "C");

            char buff[128];
            sprintf(buff, "%f", 3.14);
        }

    After destruction, the previous locale is restored.
*/

class ScopedCLocale
{
private:
    int _category;
    td::String _previousLocale;

public:
    ScopedCLocale(int category = LC_NUMERIC, const char* newLocale = "C")
        : _category(category)
    {
        const char* current = setlocale(_category, nullptr);

        if (current)
            _previousLocale = current;

        if (newLocale)
            setlocale(_category, newLocale);
    }

    ~ScopedCLocale()
    {
        if (!_previousLocale.isEmpty())
            setlocale(_category, _previousLocale.c_str());
    }

    ScopedCLocale(const ScopedCLocale&) = delete;
    ScopedCLocale& operator=(const ScopedCLocale&) = delete;
};

} //namespace mu