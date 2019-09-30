#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>



class Settings
{
public:
    Settings();
    static int kernelRadius;
    enum class Filters{
        None = 0,
        Gaus = 1,
        Sobel = 2
    };
    static Filters filter;
    static QString getFilterStringValue();
};

#endif // SETTINGS_H
