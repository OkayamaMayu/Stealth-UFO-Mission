#include"CollisionBase.h"

CollisionBase::CollisionBase() {
	m_CollisionType = TYPE_NEUTRAL;
	m_Owner = nullptr;
	m_IsCollision = true;
	m_iKind = -1;
	m_OnHitCollback = nullptr;
}