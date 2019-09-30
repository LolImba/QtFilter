#include "settings.h"

Settings::Settings()
{
}

QString Settings::getFilterStringValue()
{
    switch (filter) {
    case Settings::Filters::None:{
        return "None";
    }
    case Settings::Filters::Gaus:{
        return "Gaus";
    }
    case Settings::Filters::Sobel:{
        return "Sobel";
    }
    }
}
int Settings::kernelRadius = 1;
Settings::Filters Settings::filter = Settings::Filters::None;
