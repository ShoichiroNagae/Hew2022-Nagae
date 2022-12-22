#include "HitSquare.h"

bool HitSquare::IsHit(HITBOX obj_1, HITBOX obj_2)
{
    // activeÇ™falseÇ»ÇÁëfí ÇËÇ∑ÇÈ
            /*èàóùèëÇ≠*/

    // ìñÇΩÇËîªíË
    set_Box(obj_1, obj_2);

    return (m_box1.xMin <= m_box2.xMax && m_box1.xMax >= m_box2.xMin) &&
           (m_box1.yMin <= m_box2.yMax && m_box1.yMax >= m_box2.yMin) &&
           (m_box1.zMin <= m_box2.zMax && m_box1.zMax >= m_box2.zMin);
}

void HitSquare::set_Box(HITBOX obj_1, HITBOX obj_2)
{
    m_box1.xMax = obj_1.centerX + obj_1.sizeX / 2;
    m_box1.xMin = obj_1.centerX - obj_1.sizeX / 2;
    m_box1.yMax = obj_1.centerY + obj_1.sizeY / 2;
    m_box1.yMin = obj_1.centerY - obj_1.sizeY / 2;
    m_box1.zMax = obj_1.centerZ + obj_1.sizeZ / 2;
    m_box1.zMin = obj_1.centerZ - obj_1.sizeZ / 2;

    m_box2.xMax = obj_2.centerX + obj_2.sizeX / 2;
    m_box2.xMin = obj_2.centerX - obj_2.sizeX / 2;
    m_box2.yMax = obj_2.centerY + obj_2.sizeY / 2;
    m_box2.yMin = obj_2.centerY - obj_2.sizeY / 2;
    m_box2.zMax = obj_2.centerZ + obj_2.sizeZ / 2;
    m_box2.zMin = obj_2.centerZ - obj_2.sizeZ / 2;
}                                               
