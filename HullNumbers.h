#include "Global.h"

class HullNumbers
{
public:
    struct IndicatorInfo
    {
        int x;
        int y;
        int type;
    };

    IndicatorInfo playerIndicator;
    IndicatorInfo enemyIndicator;
    IndicatorInfo bossIndicator;

    bool enabled;

    static HullNumbers *GetInstance() { return &instance; }
private:
    static HullNumbers instance;
};

