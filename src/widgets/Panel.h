#pragma once
#include "BaseContainerWidget.h"

namespace mc {
class Panel : public BaseContainerWidget {
public:
    Panel();
    inline const std::string getType() const override { return "panel"; }

    PropertyObserver<bool>      filled;
    PropertyObserver<uint32_t>  stroke;
    PropertyObserver<uint32_t>  cornerRadius;

private:
    void _setupProperties() override;
};
} // namespace mc
