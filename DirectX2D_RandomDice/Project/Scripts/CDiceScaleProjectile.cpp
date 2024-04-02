#include "pch.h"
#include "CDiceScaleProjectile.h"

void CDiceScaleProjectile::StopProjectile()
{
}

CDiceScaleProjectile::CDiceScaleProjectile()
	: CScript(DICESCALEPROJECTILE)
{
}

CDiceScaleProjectile::CDiceScaleProjectile(const CDiceScaleProjectile& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CDiceScaleProjectile::~CDiceScaleProjectile()
{
}
