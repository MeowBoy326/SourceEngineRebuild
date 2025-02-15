#include "shaderlib/cshader.h"

class pp_cable_ps30_Static_Index {
public:
    pp_cable_ps30_Static_Index() {
    }

    int GetIndex() {
        // Asserts to make sure that we aren't using any skipped combinations.
        // Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
#endif // _DEBUG
        return 0;
    }
};

#define shaderStaticTest_pp_cable_ps30 0

class pp_cable_ps30_Dynamic_Index {
private:
    int m_nPIXELFOGTYPE;
#ifdef _DEBUG
    bool m_bPIXELFOGTYPE;
#endif
public:
    void SetPIXELFOGTYPE(int i) {
        Assert(i >= 0 && i <= 1);
        m_nPIXELFOGTYPE = i;
#ifdef _DEBUG
        m_bPIXELFOGTYPE = true;
#endif
    }

    void SetPIXELFOGTYPE(bool i) {
        m_nPIXELFOGTYPE = i ? 1 : 0;
#ifdef _DEBUG
        m_bPIXELFOGTYPE = true;
#endif
    }

public:
    pp_cable_ps30_Dynamic_Index() {
#ifdef _DEBUG
        m_bPIXELFOGTYPE = false;
#endif // _DEBUG
        m_nPIXELFOGTYPE = 0;
    }

    int GetIndex() {
        // Asserts to make sure that we aren't using any skipped combinations.
        // Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
        bool bAllDynamicVarsDefined = m_bPIXELFOGTYPE;
        Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
        return (1 * m_nPIXELFOGTYPE) + 0;
    }
};

#define shaderDynamicTest_pp_cable_ps30 psh_forgot_to_set_dynamic_PIXELFOGTYPE + 0
