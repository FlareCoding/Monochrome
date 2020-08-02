#pragma once
#include <Monochrome.h>

class {ClassName}
{{
public:
    {ClassName}();

    void Run();
    void Setup();

{PublicDataMembers}
{PublicMemberFunctions}

private:
    mc::Ref<mc::UIWindow> m_Window;

{PrivateDataMembers}
{PrivateMemberFunctions}

private:
    void InitUI();
}};
