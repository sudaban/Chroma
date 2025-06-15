#ifndef VARIANTSENDER_H
#define VARIANTSENDER_H

#include <string>
#include "Variant.h"
#include "Player.h"

class VariantSender
{
public:
   static void OnConsoleMessage(Player* p, const std::string& text)
    {
        p->SendVariant({ "OnConsoleMessage", text });
    }
};

#endif