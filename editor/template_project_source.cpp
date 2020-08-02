#include "{ClassName}.h"

{ClassName}::{ClassName}()
{{
}}

void {ClassName}::Run()
{{
    m_Window->StartWindowLoop();
}}

void {ClassName}::Setup()
{{
{WindowCreationSourceCode}

    InitUI();

}}

void {ClassName}::InitUI()
{{
{UserInterfaceSourceCode}
}}

// ==================================================================================== //
// ============================ PUBLIC MEMBER FUNCTIONS =============================== //
// ==================================================================================== //
{PublicMemberFunctionDefinitions}

// ==================================================================================== //
// ============================ PRIVATE MEMBER FUNCTIONS ============================== //
// ==================================================================================== //
{PrivateMemberFunctionDefinitions}
