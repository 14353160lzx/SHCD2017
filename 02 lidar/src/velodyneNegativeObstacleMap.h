#ifndef __VELODYNE_NEGATIVE_OBSTACLE_MAP__
#define __VELODYNE_NEGATIVE_OBSTACLE_MAP__

/**************************************************************************
*
* 1. ʹ�ù̶���С��grid matrix����С��grid_dim * grid_dim
* // TODO : add some imformation
* author: sean
* data:2016/8/10
*
***************************************************************************/

#include "velodyneDataStruct.h"
#include <vector>
#include <stdlib.h>     // for size_t

namespace velodyne_negative_obstacle_map
{
// ���ڳ�ʼ������negative obstacle map��buffer�Ĵ�С
#define MAX_BUFFER_SIZE 1024

typedef VelodyneDataStruct::xpoint_t NPoint;
typedef std::vector<NPoint> NPointCloud;

class NegativeObstacleMap
{
public:
    NegativeObstacleMap();
    ~NegativeObstacleMap();

    // ����Negative Obstacle Map
    void constructNegativeObstacleCloud(VelodyneDataStruct& scanobj, bool flag);
    void constructNegativeObstacleCloud(VelodyneDataStruct& scanobj);

    NPointCloud negative_obstacle_cloud;
    NPointCloud clear_cloud;

    std::size_t obs_count;
    std::size_t empty_count;

private:
    int grid_dim;
    int cell_size;
    // �����жϡ��ӡ�����������Ǹ���Ե
    double edge_start_threshold;
    // �����жϡ��ӡ�Զ�복�����һ����Ե
    double edge_end_threshold;
    // �����жϸ������Ƿ�Ϊ�ӵı�Ե����
    int edge_number_threshold;
};

}

#endif
