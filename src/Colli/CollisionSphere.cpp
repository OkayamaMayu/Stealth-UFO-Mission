#include"CollisionSphere.h"

CollisionSphere::CollisionSphere()
{
	//コリジョンタイプを設定
	m_CollisionType = TYPE_SPHERE;
	//コリジョン情報を初期化
	m_Sphere = {};
}