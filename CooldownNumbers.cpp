#include "CooldownNumbers.h"
#include "CustomOptions.h"

HOOK_METHOD(WeaponBox, RenderBox, (bool dragging, bool flashPowerBox) -> void)
{
    super(dragging, flashPowerBox);
    if (pWeapon && CustomOptionsManager::GetInstance()->showWeaponCooldown.currentValue)
    {
        std::stringstream stream;
        if (pWeapon->cooldown.first < 0)
        {
            stream << G_->GetTextLibrary()->GetText("weapon_cooldown_negative");
        }
        else
        {
            stream << std::fixed <<std::setprecision(1) << pWeapon->cooldown.first << "/" << pWeapon->cooldown.second;
        }
        freetype::easy_printCenter(52, location.x - (hotKey * 98) + 132, location.y - 40, stream.str());
    }
}
