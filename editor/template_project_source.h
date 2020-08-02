#pragma once
#include <Monochrome.h>

class {ClassName}
{{
public:
    {ClassName}();

    void Run();
    void Setup();

{PublicDataMembers}

private:
    mc::Ref<mc::UIWindow> m_Window;

{PrivateDataMembers}

private:
    void InitUI();
}};
