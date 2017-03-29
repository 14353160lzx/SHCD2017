#ifndef __VELODYNE_ALGO_H__
#define __VELODYNE_ALGO_H__

/***************************************************************************
 *
 *	Alogrithms used to process laser data
 *		Obstacle detection
 *
 ***************************************************************************/

#include "configStruct.h"
#include "velodyneDataStruct.h"
#include <stdio.h>
/*
 *	bit vector
 *	 charv: һά����
 *	 charvv: ��ά����
 */
typedef std::vector<bool> bitv;
typedef std::vector<bitv> bitvv;

/*
 *	���д���������ݵ��㷨���������������
 */
class VelodyneAlgo
{
private:
    // @brief CTor
    VelodyneAlgo();
    // @brief DTor
    ~VelodyneAlgo();
public:
    static float CalDirectionAngle(float XPos1, float YPos1, float XPos2, float YPos2);
    // @brief ����뾶�����ǵ�tanֵ
    static int CalcRadAndTheta(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);

    // @brief ����������
    //static int CalcGroundPoint(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);

    // @brief ����R Z I�Ⱦ�ֵ/ƽ��������(Circle��������)
    static int CalcCircleFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);
    // @brief �������е�Circle��������
    static int CalcScanCellCircleFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);

    // @brief ���������߶�
    static int CalcContinuousLines(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);

    // @brief ���ݾ������
    static int ClassifyByDeltaR(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);
    // @brief ���ݸ̷߳���
    static int ClassifyByDeltaZ(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);

    // @brief ת���ɵ��ߴ���ṹ
    static int CalClusterLineFeature(VelodyneDataStruct::clusterLine_t& clu_line, VelodyneDataStruct::clusterLineFeature_t& clu_lineFeature);
    static int TransferClusterToLineStructure(VelodyneDataStruct::pieCluster_t& clu, VelodyneDataStruct::clusterLine_t& cline, VelodyneDataStruct& scanobj);
    static int CalClusterLineAndFeature(VelodyneDataStruct& scanobj);
    //static int CalClusterLShapeFeature(clusterLine_t& clu_line, clusterLineFeature_t& clu_lineFeature);


    /**************************** ����Ϊ������ʽ�Ĵ����� ****************************/
    // @brief ת���ɸ�����ʾ����ǳ�ƽ��ͷ�ƽ��
    static int TransferToPieMatrix(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);

    // @brief ���㵥Ԫ���� R Z I�Ⱦ�ֵƽ��������
    static int CalcPieMatrixFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);
    // @brief ���񵥸� R Z I�Ⱦ�ֵƽ��������
    static int CalcSinglePieCellFeature(CfgVeloView_t& cfg, VelodyneDataStruct::cell_t& pieobj, VelodyneDataStruct::pieFeature_t& pie_feature);

    // @brief ��������Ѱ��������������
    static int FindAndCalcPieClusterFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj, bool findClosestOnly = false);
    // @brief ������ΧѰ����ͬ��������
    static int SearchPieCluster(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj, VelodyneDataStruct::pieCluster_t& pie_clu, bitvv& isSearchedvv, int azimuth_idx, int rad_idx);
    // @brief �������������
    static int CalcSinglePieClusterFeature(VelodyneDataStruct::pieCluster_t& pie_clu, VelodyneDataStruct::pieClusterFeature_t& pie_cluFeature);


    /**************************** ����Ϊ������ʽ�Ĵ����� ****************************/
    // @brief ת���ɷ����ʾ����ǳ�ƽ��ͷ�ƽ��
    //static int TransferToGridMatrix(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);
    static int TransferToGridMatrix(CfgVeloView_t& cfg, VelodyneDataStruct& calobj);

    // @brief ���㵥Ԫ���� R Z I�Ⱦ�ֵ ƽ��������
    static int CalcGridMatrixFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);
    // @brief �������� R Z I�Ⱦ�ֵ ƽ������������
    static int CalcSingleGridFeature(CfgVeloView_t& cfg, VelodyneDataStruct::cell_t& gridobj, VelodyneDataStruct::gridFeature_t& grid_feature);

    // @brief �ڷ�����Ѱ���ϰ�����Ⱥ�����㼯Ⱥ������Ϣ
    static int FindAndCalcGridClusterFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj);
    // @brief �ݹ�Ѱ���ϰ�����Ⱥ
    static int SearchGridCluster(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj, VelodyneDataStruct::gridCluster_t& grid_clu, bitvv& isSearchedvv, int row, int grid);
    // @brief ���㵥���ϰ��﷽������Ⱥ��������Ϣ
    static int CalcSingleGridClusterFeature(VelodyneDataStruct::gridCluster_t& grid_clu, VelodyneDataStruct::gridClusterFeature_t& grid_cluFeature);
};

#endif

