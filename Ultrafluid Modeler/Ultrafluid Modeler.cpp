// Ultrafluid Modeler.cpp : Définit les fonctions exportées de la DLL.
//

#include "pch.h"
#include "framework.h"
#include "Ultrafluid Modeler.h"


// Il s'agit d'un exemple de variable exportée
ULTRAFLUIDMODELER_API int nUltrafluidModeler=0;

// Il s'agit d'un exemple de fonction exportée.
ULTRAFLUIDMODELER_API int fnUltrafluidModeler(void)
{
    return 0;
}

// Il s'agit du constructeur d'une classe qui a été exportée.
CUltrafluidModeler::CUltrafluidModeler()
{
    return;
}
