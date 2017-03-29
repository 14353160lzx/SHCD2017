/**************************************************************************
 *
 * map points to negative obstacle map
 * // add more imformation
 * author: sean
 * data: 2016/8/10
 *
 ***************************************************************************/
#include "velodyneNegativeObstacleMap.h"
#include "velodyneDraw.h"
#include "configStruct.h"
#include <string.h>     // for memset

namespace velodyne_negative_obstacle_map
{
NegativeObstacleMap::NegativeObstacleMap()
{
    this->cell_size = g_CfgVeloView.cfgNegativeObstacle.NegativeObstacleCellSize;
    this->grid_dim = g_CfgVeloView.cfgNegativeObstacle.NegativeObstacleGridDim;
    //TODO
    // ��ֵ�ĵ�λ������
    this->edge_start_threshold = g_CfgVeloView.cfgNegativeObstacle.NegativeObstacleThresholdStartEdge;
    this->edge_end_threshold = g_CfgVeloView.cfgNegativeObstacle.NegativeObstacleThresholdEndEdge;
    // ��Ե�����
    this->edge_number_threshold = g_CfgVeloView.cfgNegativeObstacle.NegativeObstacleThresholdEdgeNumber;

    this->obs_count = 0;
    this->empty_count = 0;
    //printf("Negative Obstacle Map Constructed...\n");
}
NegativeObstacleMap::~NegativeObstacleMap() {}

//����Negative Obstacle Map
void NegativeObstacleMap::constructNegativeObstacleCloud(VelodyneDataStruct& scanobj, bool flag)
{

    // do not use dynamic memory --> waste memery for speed
    static int num_of_edge_points[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    memset(&num_of_edge_points, 0, grid_dim * grid_dim);

    // build Negative Obstacle Map
    unsigned shotNum = scanobj.shots.size();
    for (unsigned shot = 0; shot < shotNum; shot++)
    {
        // ԭ��ѭ��������: VELODYNE_NUM_BEAMS_IN_ONE_SHOT - 1 �Ƿ�ֹ�жϵı�Ե���ʱ������Խ��
        for (unsigned laser = 0; laser < DOWN_VERT_TO; laser++)
        {
            // ����õ�Ϊ��Ч�㣬��ֱ������
            if (scanobj.shots[shot].pt[laser].point_type & POINT_TYPE_INVALID)
                continue;
//#define NEGATIVE_OBSTACLE_GROUNDZ -13.0
            //if (scanobj.shots[shot].pt[laser].z > NEGATIVE_OBSTACLE_GROUNDZ)
            if (scanobj.shots[shot].pt[laser].z > g_CfgVeloView.cfgGlobal.GroundZ)
                continue;

            // ����õ����ĸ�����
            int grid_index_x = ((grid_dim / 2) + scanobj.shots[shot].pt[laser].x / cell_size);
            int grid_index_y = ((grid_dim / 2) + scanobj.shots[shot].pt[laser].y / cell_size);

            // TODO ���ǿӿ��
            // �жϵ����ڵ������Ƿ�������ͼ��
            if (grid_index_x >= 0 && grid_index_x < grid_dim && grid_index_y >= 0 && grid_index_y < grid_dim)
            {
                /*
                 * �жϸõ��ǲ���Ϊ��Ե��(����ǿ� Խ���µļ������򵽵ĵ�������ȿ�������ϵ�һ�������)
                 * edge_start_threshold = 5cm
                 */
                if (scanobj.shots[shot].pt[laser].z - scanobj.shots[shot].pt[laser + 1].z > edge_start_threshold)
                {
                    scanobj.shots[shot].pt[laser].point_type |= POINT_TYPE_HOLE_EDGE;
                    unsigned edge_start = laser;
                    num_of_edge_points[grid_index_x][grid_index_y]++;

                    //printf("get an edge\n");

                    // TODO: ��ȷ��VELODYNE_NUM_BEAMS_IN_ONE_SHOT - nΪ�ã�
                    // ����ӵ���һ����Ե��
                    unsigned edge_end = laser + 1;
                    for (; edge_end < UPPER_VERT_FROM; edge_end++)
                    {
                        if (abs(scanobj.shots[shot].pt[edge_end].z - scanobj.shots[shot].pt[edge_start].z) < edge_end_threshold)
                        {
                            scanobj.shots[shot].pt[edge_end].point_type |= POINT_TYPE_HOLE_EDGE;
                            break;
                        }
                        else
                        {
                            // �������ڵ�
                            scanobj.shots[shot].pt[edge_end].point_type |= POINT_TYPE_HOLE_INSIDE;
                            int inside_x = ((grid_dim / 2) + scanobj.shots[shot].pt[edge_end].x / cell_size);
                            int inside_y = ((grid_dim / 2) + scanobj.shots[shot].pt[edge_end].y / cell_size);
                            if (inside_x >= 0 && inside_x < grid_dim && inside_y >= 0 && inside_y < grid_dim)
                            {
                                num_of_edge_points[inside_x][inside_y]++;
                            }
                            else
                                break;
                        }
                    } // end of for (; edge_end = VELODYNE_NUM_BEAMS_IN_ONE_SHOT - 1; edge_end++)
                    /*
                     * ��һ����Ե���������
                     *	���������UPPER_VERT_FROM֮ǰ������û�ҳ���Ч��Ե�� �ݶ�UPPER_VERT_FROM�򵽵ĵ�Ϊ��Ե��
                     */
                    int end_x = ((grid_dim / 2) + scanobj.shots[shot].pt[edge_end].x / cell_size);
                    int end_y = ((grid_dim / 2) + scanobj.shots[shot].pt[edge_end].y / cell_size);
                    if (end_x >= 0 && end_x < grid_dim && end_y >= 0 && end_y < grid_dim)
                    {
                        num_of_edge_points[end_x][end_y]++;
                    }

                    laser = edge_end;
                } // end of if (scan.shots[shot].pt[laser].z - scan.shots[shot].pt[laser + 1].z > edge_start_threshold)
            } // end of if (x >= 0 && x < grid_dim && y >= 0 && y < grid_dim)

        }//end of for (unsigned laser = 0; laser < VELODYNE_NUM_BEAMS_IN_ONE_SHOT; laser++)
    }//end of for (unsigned shot = 0; shot < shotNum; shot++)

    // �������Negative Obstacle Cloud
    for (unsigned shot = 0; shot < shotNum; shot++)
    {
        for (unsigned laser = 0; laser < UPPER_VERT_FROM; laser++)
        {
            if (scanobj.shots[shot].pt[laser].point_type & POINT_TYPE_INVALID)
                continue;

            // ������negative obstacle grid����ĵ����negative obstacle cloud
            int grid_index_x = ((grid_dim / 2) + scanobj.shots[shot].pt[laser].x / cell_size);
            int grid_index_y = ((grid_dim / 2) + scanobj.shots[shot].pt[laser].y / cell_size);

            if (grid_index_x >= 0 && grid_index_x < grid_dim && grid_index_y >= 0 && grid_index_y < grid_dim
                    && num_of_edge_points[grid_index_x][grid_index_y] >= edge_number_threshold)
            {
                VelodyneDataStruct::xpoint_t obstacle_pt;

                obstacle_pt.x = scanobj.shots[shot].pt[laser].x;
                obstacle_pt.y = scanobj.shots[shot].pt[laser].y;
                obstacle_pt.z = scanobj.shots[shot].pt[laser].z;
                obstacle_pt.i = scanobj.shots[shot].pt[laser].i;

                negative_obstacle_cloud.push_back(obstacle_pt);
                obs_count++;
            }
            else
            {
                VelodyneDataStruct::xpoint_t clear_pt;

                clear_pt.x = scanobj.shots[shot].pt[laser].x;
                clear_pt.y = scanobj.shots[shot].pt[laser].y;
                clear_pt.z = scanobj.shots[shot].pt[laser].z;
                clear_pt.i = scanobj.shots[shot].pt[laser].i;

                clear_cloud.push_back(clear_pt);
                empty_count++;
            }
        } // end of for (unsigned laser = 0; laser < UPPER_VERT_FROM; laser++)
    } // end of for (unsigned shot = 0; shot < shotNum; shot++)

    //TODO: �����ڵ�ӳ��ص�·����
}

// build Negative Obstacle Map
void NegativeObstacleMap::constructNegativeObstacleCloud(VelodyneDataStruct& scanobj)
{
    unsigned shotNum = scanobj.shots.size();
    for (unsigned shot = 0; shot < shotNum; shot++)
    {
        for (unsigned laser = 0; laser < DOWN_VERT_TO; laser++)
        {
            // ����õ�Ϊ��Ч�㣬��ֱ������
            if (scanobj.shots[shot].pt[laser].point_type & POINT_TYPE_INVALID)
                continue;

            if (scanobj.shots[shot].pt[laser].z > g_CfgVeloView.cfgGlobal.GroundZ)
                continue;
            /*
             * �жϸõ��ǲ���Ϊ��Ե��(����ǿ� Խ���µļ������򵽵ĵ�������ȿ�������ϵ�һ�������)
             * edge_start_threshold = 5cm
             */
            if (scanobj.shots[shot].pt[laser].z - scanobj.shots[shot].pt[laser + 1].z > edge_start_threshold)
            {
                scanobj.shots[shot].pt[laser].point_type |= POINT_TYPE_HOLE_EDGE;
                unsigned edge_start = laser;

                //printf("get an edge\n");

                // ����ӵ���һ����Ե��
                unsigned edge_end = laser + 1;
                for (; edge_end < UPPER_VERT_FROM; edge_end++)
                {
                    if (abs(scanobj.shots[shot].pt[edge_end].z - scanobj.shots[shot].pt[edge_start].z) < edge_end_threshold)
                    {
                        scanobj.shots[shot].pt[edge_end].point_type |= POINT_TYPE_HOLE_EDGE;
                        break;
                    }
                    else
                    {
                        // �������ڵ�
                        scanobj.shots[shot].pt[edge_end].point_type |= POINT_TYPE_HOLE_INSIDE;
                    }
                } // end of for (; edge_end < UPPER_VERT_FROM; edge_end++){

                laser = edge_end;
            } // end of if (scanobj.shots[shot].pt[laser].z - scanobj.shots[shot].pt[laser + 1].z > edge_start_threshold){
        } // end of for (unsigned laser = 0; laser < DOWN_VERT_TO; laser++){
    } //end of for (unsigned shot = 0; shot < shotNum; shot++)
}
}
