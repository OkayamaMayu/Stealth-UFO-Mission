#include"CollisionLineSegment.h"

CollisionLineSegment::CollisionLineSegment()
{
	//コリジョンタイプを設定
	m_CollisionType = TYPE_LINE;
	//コリジョン情報を初期化
	m_LineSegment = {};
}