/***************************************************************************
 *
 *	ʵ�� velodyneAlgo.h �й��캯��
 *	ʵ�ּ��㹦�ܺ���
 *		�����
 *		�뾶�����ǵ�tanֵ
 *		Circle(��ֱ�Ƕ�ƽ��)R Z I��ֵ ƽ���������
 *		�����
 *
 ***************************************************************************/

#include "velodyneAlgo.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <algorithm>

// ����ֵ
static inline float absf(float a)
{
    if (a < 0)
        return -a;
    return a;
}
// ����
static float dist(VelodyneDataStruct::xpoint_t&a, VelodyneDataStruct::xpoint_t& b)
{
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

VelodyneAlgo::VelodyneAlgo()
{
}

VelodyneAlgo::~VelodyneAlgo()
{
}
/*
 *	TODO: �ƺ����ڼ��㷽���
 *	Direction Angles and Direction Cosines: http://www.geom.uiuc.edu/docs/reference/CRC-formulas/node52.html
 */
//float VelodyneAlgo::CalDirectionAngle(float XPos1, float YPos1, float XPos2, float YPos2)
//{
//    float delt_x = XPos2 - XPos1;
//    float delt_y = YPos2 - YPos1;
//    float angle;
//    float length = sqrt(delt_x*delt_x + delt_y*delt_y);
//    if (delt_x<0)
//        angle = 360 - acos(delt_y / length) * 180 / M_PI;
//    else
//        angle = acos(delt_y / length) * 180 / M_PI;
//    return angle;
//}
/*
 *  ��������������scan ÿ�����ͶӰ�������X��нǵ�����ֵ
 *  @param[IN] cfg       	������Ϣ
 *  @param[IN] scanobj  	Ҫ�����scan����
 *  @return     0:�ɹ� 	����ʧ��
 */
int VelodyneAlgo::CalcRadAndTheta(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj)
{
    // shot������
    int size = scanobj.shots.size();

    // ��Ҫ�����shot�Ŀ�ʼ���
    int start = cfg.cfgGlobal.ShotCalcFrom;
    int end;
    // ��Ҫ�����shot������
    int CalcNum = cfg.cfgGlobal.ShotCalcNum;
    // ��Ҫ�����shot�Ľ������
    if (CalcNum == 0)
        CalcNum = size;
    end = CalcNum + start;
    assert((start<end) && (start>-1) && (end <= size));

    for (unsigned shot = start; shot<end; ++shot)
    {
        for (unsigned circle = 0; circle<CIRCLE_NUM; ++circle)
        {
            // ��������ļ�û��ʹ�����Circle��������
            if (cfg.cfgCircleItems[circle].Enable == 0)
                continue;
            VelodyneDataStruct::xpoint_t& pt = scanobj.shots[shot].pt[circle];
            // ������Ч��
            if (pt.point_type & POINT_TYPE_INVALID)
                continue;
            /*
             *	pt.rad: �������xyƽ��ͶӰ��ԭ�����
             *	pt.tan_theta: �������xyƽ��ͶӰ����x��н�(����)����ֵ
             */
            pt.rad = sqrt(pt.x*pt.x + pt.y*pt.y);
            pt.tan_theta = pt.y / pt.x;
        }
    }
    return 0;
}
/*
 *	����������
 *	//TODO: δʵ��
 */
//int VelodyneAlgo::CalcGroundPoint(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj){
//	typedef vector<xpoint_t*> vex;
//	return 0;
//}
/*
 *  ��������������scan��������(��CircleΪ��λ)
 *		Z I R ��ֵ
 *		Z I R ƽ����
 *		���¼�ֵ
 *			max_rad/min_rad
 *			max_x/min_x;max_y/min_y;max_z/min_z
 *			Z I R ƽ����
 *			max_scanline_drtRad/min_scanline_drtRad
 *			max_scanline_drtZ/min_scanline_drtZ
 *  @param[IN] cfg       ������Ϣ
 *  @param[IN] scanobj  Ҫ�����scan����
 *  @return   0:�ɹ� ����ʧ��
 */
//int VelodyneAlgo::CalcCircleFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj)
//{
//    /*
//     *	�����ļ��� ShotCalcFrom = ShotCalcNum = 0
//     *	GridWidth: 0~360 ��ǰ֡ǰ��circleGridWidth֡����
//     *	���Լ�Ϊ���ģ��ٽ���ĸ���(��������)
//     */
//    int GridWidth = scanobj.circleGridWidth * 2 + 1;
//    // �����ü������ʼ��Χ
//    int ShotSize = scanobj.shots.size();
//    int start = cfg.cfgGlobal.ShotCalcFrom;
//    int CalcNum = cfg.cfgGlobal.ShotCalcNum;
//    if (CalcNum == 0)
//        CalcNum = ShotSize;
//    int end = CalcNum + start;
//    assert((start<end) && (start>-1) && (end <= ShotSize));
//    // ��ʼ������scan��ͳ�Ʊ���
//    scanobj.InitStatistics(scanobj.scanStatistics);
//    // ������뾶������ֵ
//    VelodyneAlgo::CalcRadAndTheta(cfg, scanobj);
//
//    int i, j, k;
//    // ��¼�ٽ���� Z I R ��ֵ���� Z I R ƽ����
//    float aveZ, drtZ;
//    float aveRad, drtRad;
//    float aveI, drtI;
//    int count;
//    int pos;
//    int* dummy;
//    /*
//     *	��������
//     *	��CircleΪ��λ
//     *	ZIR���ھ�ֵ ����ƽ����
//     */
//    for (j = 0; j<CIRCLE_NUM; ++j)
//    {
//        if (cfg.cfgCircleItems[j].Enable == 0)
//            continue;
//
//        // ��ʼ������scan��ͳ�Ʊ���
//        scanobj.InitStatistics(scanobj.circlesStatistics[j]);
//        for (i = start; i<end; ++i)
//        {
//            VelodyneDataStruct::xpoint_t& pt = scanobj.shots[i].pt[j];
//            if (pt.point_type & POINT_TYPE_INVALID)
//                continue;
//            aveZ = 0.0;
//            drtZ = 0.0;
//            aveRad = 0.0;
//            drtRad = 0.0;
//            aveI = 0.0;
//            drtI = 0.0;
//            count = 0;
//            // �����ֵ(Z I R ��ֵ)
//            for (k = 0; k < GridWidth; k++)
//            {
//                pos = i - scanobj.circleGridWidth + k;
//                if (pos > -1 && pos < ShotSize)
//                {
//                    VelodyneDataStruct::xpoint_t&  ptSlide = scanobj.shots[pos].pt[j];
//                    if (ptSlide.point_type&POINT_TYPE_INVALID)
//                        continue;
//                    aveZ += ptSlide.z;
//                    aveRad += ptSlide.rad;
//                    aveI += ptSlide.i;
//                    count++;
//                }
//            }
//            if (count <= 0)
//            {
//                continue;
//            }
//            else
//            {
//                aveZ /= (count *1.0);
//                aveRad /= (count *1.0);
//                aveI /= (count *1.0);
//            }
//            // ���� Z I R ƽ����
//            for (k = 0; k < GridWidth; k++)
//            {
//                pos = i - scanobj.circleGridWidth + k;
//                if (pos > -1 && pos < ShotSize)
//                {
//                    VelodyneDataStruct::xpoint_t& ptSlide = scanobj.shots[pos].pt[j];
//                    if (ptSlide.point_type & POINT_TYPE_INVALID)
//                        continue;
//                    drtZ += absf(ptSlide.z - aveZ);
//                    drtI += absf(ptSlide.i - aveI);
//                    drtRad += absf(ptSlide.rad - aveRad);
//                    count++;
//                }
//            }
//
//            drtZ /= (count * 1.0);
//            drtRad /= (count *1.0);
//            drtI /= (count *1.0);
//            pt.scanline_drtZ = drtZ;
//            pt.scanline_drtRad = drtRad;
//            pt.scanline_drtI = drtI;
//
//            /*
//             *	��ʼ��ͳ����
//             *	����ÿ��Circle��[start,end]ɨ��������
//             *		x y z��ֵ
//             *		�߳�ƽ���� �뾶ƽ���ֵ
//             */
//            dummy = reinterpret_cast<int*>(&(scanobj.circlesStatistics[j].max_rad));
//            if (*dummy == 0)
//            {
//                scanobj.circlesStatistics[j].max_rad = 0.0;
//                scanobj.circlesStatistics[j].min_rad = pt.rad;
//
//                scanobj.circlesStatistics[j].max_x = pt.x;
//                scanobj.circlesStatistics[j].min_x = pt.x;
//
//                scanobj.circlesStatistics[j].max_y = pt.y;
//                scanobj.circlesStatistics[j].min_y = pt.y;
//
//                scanobj.circlesStatistics[j].max_z = pt.z;
//                scanobj.circlesStatistics[j].min_z = pt.z;
//
//                scanobj.circlesStatistics[j].max_scanline_drtRad = pt.scanline_drtRad;
//                scanobj.circlesStatistics[j].min_scanline_drtRad = pt.scanline_drtRad;
//
//                scanobj.circlesStatistics[j].max_scanline_drtZ = pt.scanline_drtZ;
//                scanobj.circlesStatistics[j].min_scanline_drtZ = pt.scanline_drtZ;
//            }
//            // ���¼�ֵ
//            if (scanobj.circlesStatistics[j].max_rad < pt.rad)
//                scanobj.circlesStatistics[j].max_rad = pt.rad;
//
//            if (scanobj.circlesStatistics[j].min_rad > pt.rad)
//                scanobj.circlesStatistics[j].min_rad = pt.rad;
//
//            if (scanobj.circlesStatistics[j].max_x < pt.x)
//                scanobj.circlesStatistics[j].max_x = pt.x;
//
//            if (scanobj.circlesStatistics[j].min_x > pt.x)
//                scanobj.circlesStatistics[j].min_x = pt.x;
//
//            if (scanobj.circlesStatistics[j].max_y < pt.y)
//                scanobj.circlesStatistics[j].max_y = pt.y;
//
//            if (scanobj.circlesStatistics[j].min_y > pt.y)
//                scanobj.circlesStatistics[j].min_y = pt.y;
//
//            if (scanobj.circlesStatistics[j].max_z < pt.z)
//                scanobj.circlesStatistics[j].max_z = pt.z;
//
//            if (scanobj.circlesStatistics[j].min_z > pt.z)
//                scanobj.circlesStatistics[j].min_z = pt.z;
//
//            if (scanobj.circlesStatistics[j].max_scanline_drtRad < pt.scanline_drtRad)
//                scanobj.circlesStatistics[j].max_scanline_drtRad = pt.scanline_drtRad;
//
//            if (scanobj.circlesStatistics[j].min_scanline_drtRad > pt.scanline_drtRad)
//                scanobj.circlesStatistics[j].min_scanline_drtRad = pt.scanline_drtRad;
//
//            if (scanobj.circlesStatistics[j].max_scanline_drtZ < pt.scanline_drtZ)
//                scanobj.circlesStatistics[j].max_scanline_drtZ = pt.scanline_drtZ;
//
//            if (scanobj.circlesStatistics[j].min_scanline_drtZ > pt.scanline_drtZ)
//                scanobj.circlesStatistics[j].min_scanline_drtZ = pt.scanline_drtZ;
//        } // end of for (i = start; i<end; ++i){
//    } // end of for (j = 0; j<CIRCLE_NUM; ++j){
//    scanobj.calcCircleFeatured = true;
//    return 0;
//}
///*
// *  �ڸ����м���Circle������
// *  @param[IN] cfg       ������Ϣ
// *  @param[IN] scanobj  Ҫ�����scan����
// *  @return     0:�ɹ� ����ʧ��
// */
//int VelodyneAlgo::CalcScanCellCircleFeature(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj)
//{
//    // �����ü������ʼ��Χ
//    int ShotSize = scanobj.shots.size();
//    int start = cfg.cfgGlobal.ShotCalcFrom;
//    int CalcNum = cfg.cfgGlobal.ShotCalcNum;
//    if (CalcNum == 0)
//        CalcNum = ShotSize;
//    int end = CalcNum + start;
//    assert((start<end) && (start>-1) && (end <= ShotSize));
//
//    // ���Լ�Ϊ���ģ��ٽ�����ĸ���
//    // int GridWidth = .circleGridWidth*2+1;
//    int GridWidth = 2 * 2 + 1;
//
//    int i, j, k;
//
//    /** ��¼�ٽ���� Z I R ��ֵ����Z I R ��ֵƽ����*/
//    float aveZ, drtZ;
//    float aveRad, drtRad;
//    float aveI, drtI;
//    int count;
//
//    int pos;
//    int *dummy;
//    for (j = 0; j<CIRCLE_NUM; ++j)
//    {
//        if (cfg.cfgCircleItems[j].Enable == 0)
//            continue;
//        scanobj.InitStatistics(scanobj.circlesStatistics[j]);
//        for (i = start; i<end; ++i)
//        {
//            VelodyneDataStruct::xpoint_t&  pt = scanobj.shots[i].pt[j];
//            if (pt.point_type & POINT_TYPE_INVALID)
//                continue;
//
//            aveZ = 0.0;
//            drtZ = 0.0;
//            aveRad = 0.0;
//            drtRad = 0.0;
//            aveI = 0.0;
//            drtI = 0.0;
//            count = 0;
//            //�����ֵ
//            for (k = 0; k< GridWidth; k++)
//            {
//                pos = i - scanobj.circleGridWidth + k;
//                if (pos>-1 && pos<ShotSize)
//                {
//                    VelodyneDataStruct::xpoint_t&  ptSlide = scanobj.shots[pos].pt[j];
//                    if (ptSlide.point_type & POINT_TYPE_INVALID)
//                        continue;
//                    aveZ += ptSlide.z;
//                    aveRad += ptSlide.rad;
//                    aveI += ptSlide.i;
//                    count++;
//                }
//            }
//            if (count <= 0)
//            {
//                continue;
//            }
//            else
//            {
//                aveZ /= (count *1.0);
//                aveRad /= (count *1.0);
//                aveI /= (count *1.0);
//            }
//            // ��������
//            for (k = 0; k< GridWidth; k++)
//            {
//                pos = i - scanobj.circleGridWidth + k;
//                if (pos>-1 && pos<ShotSize)
//                {
//                    VelodyneDataStruct::xpoint_t& ptSlide = scanobj.shots[pos].pt[j];
//                    if (ptSlide.point_type & POINT_TYPE_INVALID)
//                        continue;
//
//                    drtZ += absf(ptSlide.z - aveZ);
//                    drtI += absf(ptSlide.i - aveI);
//                    drtRad += absf(ptSlide.rad - aveRad);
//                    count++;
//                }
//            }
//            drtZ /= (count *1.0);
//            drtRad /= (count *1.0);
//            drtI /= (count *1.0);
//            pt.scanline_drtZ = drtZ;
//            pt.scanline_drtRad = drtRad;
//            pt.scanline_drtI = drtI;
//
//            // ��ʼ��ͳ����
//            dummy = reinterpret_cast<int*>(&(scanobj.circlesStatistics[j].max_rad));
//            if (*dummy == 0)
//            {
//                scanobj.circlesStatistics[j].max_rad = 0.0;
//                scanobj.circlesStatistics[j].min_rad = pt.rad;
//
//                scanobj.circlesStatistics[j].max_x = pt.x;
//                scanobj.circlesStatistics[j].min_x = pt.x;
//
//                scanobj.circlesStatistics[j].max_y = pt.y;
//                scanobj.circlesStatistics[j].min_y = pt.y;
//
//                scanobj.circlesStatistics[j].max_z = pt.z;
//                scanobj.circlesStatistics[j].min_z = pt.z;
//
//                scanobj.circlesStatistics[j].max_scanline_drtRad = pt.scanline_drtRad;
//                scanobj.circlesStatistics[j].min_scanline_drtRad = pt.scanline_drtRad;
//
//                scanobj.circlesStatistics[j].max_scanline_drtZ = pt.scanline_drtZ;
//                scanobj.circlesStatistics[j].min_scanline_drtZ = pt.scanline_drtZ;
//            }
//
//            // ���¼�ֵ
//            if (scanobj.circlesStatistics[j].max_rad<pt.rad)
//                scanobj.circlesStatistics[j].max_rad = pt.rad;
//
//            if (scanobj.circlesStatistics[j].min_rad>pt.rad)
//                scanobj.circlesStatistics[j].min_rad = pt.rad;
//
//            if (scanobj.circlesStatistics[j].max_x<pt.x)
//                scanobj.circlesStatistics[j].max_x = pt.x;
//
//            if (scanobj.circlesStatistics[j].min_x>pt.x)
//                scanobj.circlesStatistics[j].min_x = pt.x;
//
//            if (scanobj.circlesStatistics[j].max_y<pt.y)
//                scanobj.circlesStatistics[j].max_y = pt.y;
//
//            if (scanobj.circlesStatistics[j].min_y>pt.y)
//                scanobj.circlesStatistics[j].min_y = pt.y;
//
//            if (scanobj.circlesStatistics[j].max_z<pt.z)
//                scanobj.circlesStatistics[j].max_z = pt.z;
//
//            if (scanobj.circlesStatistics[j].min_z>pt.z)
//                scanobj.circlesStatistics[j].min_z = pt.z;
//
//            if (scanobj.circlesStatistics[j].max_scanline_drtRad<pt.scanline_drtRad)
//                scanobj.circlesStatistics[j].max_scanline_drtRad = pt.scanline_drtRad;
//
//            if (scanobj.circlesStatistics[j].min_scanline_drtRad>pt.scanline_drtRad)
//                scanobj.circlesStatistics[j].min_scanline_drtRad = pt.scanline_drtRad;
//
//            if (scanobj.circlesStatistics[j].max_scanline_drtZ<pt.scanline_drtZ)
//                scanobj.circlesStatistics[j].max_scanline_drtZ = pt.scanline_drtZ;
//
//            if (scanobj.circlesStatistics[j].min_scanline_drtZ>pt.scanline_drtZ)
//                scanobj.circlesStatistics[j].min_scanline_drtZ = pt.scanline_drtZ;
//        }
//    }
//
//    scanobj.calcCircleFeatured = true;
//
//    return 0;
//}
///*
// *	���������߶�
// */
//int VelodyneAlgo::CalcContinuousLines(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj)
//{
//    int GridWidth = scanobj.circleGridWidth * 2 + 1;
//    int ShotSize = scanobj.shots.size();
//    int start = cfg.cfgGlobal.ShotCalcFrom;
//    int CalcNum = cfg.cfgGlobal.ShotCalcNum;
//    if (CalcNum == 0)
//        CalcNum = ShotSize;
//    int end = CalcNum + start;
//    assert((start<end) && (start>-1) && (end <= ShotSize));
//
//    int i, j;
//    int count;
//    VelodyneDataStruct::continuousLine_t line;
//    for (j = 0; j <CIRCLE_NUM; j++)
//    {
//        CfgCircleItem_t& itemcfg = cfg.cfgCircleItems[j];
//        if (itemcfg.Enable == 0)
//            continue;
//        count = 0;
//        for (i = start; i<end; i++)
//        {
//            if (scanobj.shots[i].pt[j].point_type & POINT_TYPE_BELOW_DELTA_R)
//            {
//                if (count == 0)
//                {
//                    line.start = i;
//                }
//                ++count;
//            }
//            else
//            {
//                if (count != 0)
//                {
//                    line.end = i;
//                    VelodyneDataStruct::xpoint_t start = scanobj.shots[line.start].pt[j];
//                    VelodyneDataStruct::xpoint_t end = scanobj.shots[line.end].pt[j];
//                    int d = (int)dist(start, end);
//                    if (d > (float)cfg.cfgGlobal.ContinusLineMinDistInCircle)
//                        scanobj.circlesLines[j].push_back(line);
//                    count = 0;
//                }
//            }
//        }
//    }
//    scanobj.calcCirclesLined = true;
//
//    return 0;
//}
//
///*
// *	���ݾ������
// */
//int VelodyneAlgo::ClassifyByDeltaR(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj)
//{
//    int GridWidth = scanobj.circleGridWidth * 2 + 1;
//    int ShotSize = scanobj.shots.size();
//    int start = cfg.cfgGlobal.ShotCalcFrom;
//    int CalcNum = cfg.cfgGlobal.ShotCalcNum;
//    if (CalcNum == 0)
//        CalcNum = ShotSize;
//    int end = CalcNum + start;
//    assert((start<end) && (start>-1) && (end <= ShotSize));
//
//    int i, j;
//    for (j = 0; j <CIRCLE_NUM; j++)
//    {
//        CfgCircleItem_t& itemcfg = cfg.cfgCircleItems[j];
//        if (itemcfg.Enable == 0)
//            continue;
//        for (i = start; i<end; i++)
//        {
//            if (scanobj.shots[i].pt[j].point_type & POINT_TYPE_INVALID)
//                continue;
//            if (scanobj.shots[i].pt[j].rad < itemcfg.ThresholdMinDist*cfg.cfgGlobal.ThresholdMinRad)
//            {
//                scanobj.shots[i].pt[j].point_type |= POINT_TYPE_BELOW_R;
//            }
//            else
//            {
//                // С�ڸ�ֵ��Ϊ��ƽ̨·�棬����Ϊ·�ص�
//                if (scanobj.shots[i].pt[j].scanline_drtRad>itemcfg.ThresholdRoadEdge)
//                    scanobj.shots[i].pt[j].point_type |= POINT_TYPE_ABOVE_DELTA_R;
//                else
//                    scanobj.shots[i].pt[j].point_type |= POINT_TYPE_BELOW_DELTA_R;
//            }
//        }
//    }
//    return 0;
//}
//
///*
// *	���ݸ̷߳���
// */
//int VelodyneAlgo::ClassifyByDeltaZ(CfgVeloView_t& cfg, VelodyneDataStruct& scanobj)
//{
//    int GridWidth = scanobj.circleGridWidth * 2 + 1;
//    int ShotSize = scanobj.shots.size();
//
//    int start = cfg.cfgGlobal.ShotCalcFrom;
//    int CalcNum = cfg.cfgGlobal.ShotCalcNum;
//    if (CalcNum == 0)
//        CalcNum = ShotSize;
//    int end = CalcNum + start;
//    assert((start<end) && (start>-1) && (end <= ShotSize));
//
//    int i, j;
//    for (j = 0; j <CIRCLE_NUM; j++)
//    {
//        CfgCircleItem_t& itemcfg = cfg.cfgCircleItems[j];
//        if (itemcfg.Enable == 0)
//            continue;
//        for (i = start; i<end; i++)
//        {
//            if (scanobj.shots[i].pt[j].point_type & POINT_TYPE_INVALID)
//                continue;
//            if (scanobj.shots[i].pt[j].rad<itemcfg.ThresholdMinDist * cfg.cfgGlobal.ThresholdMinRad)
//            {
//                scanobj.shots[i].pt[j].point_type |= POINT_TYPE_BELOW_R;
//            }
//            else
//            {
//                // С�ڸ�ֵ��Ϊ��ƽ̨·�棬����Ϊ·�ص�
//                if (scanobj.shots[i].pt[j].scanline_drtZ>itemcfg.ThresholdRoadEdgeByDeltaZ)
//                    scanobj.shots[i].pt[j].point_type |= POINT_TYPE_ABOVE_DELTA_Z;
//                else
//                    scanobj.shots[i].pt[j].point_type |= POINT_TYPE_BELOW_DELTA_Z;
//            }
//        }
//    }
//    return 0;
//}
///*
// *		����ͨ����ͨͼ�����scanobj.scanClusterArray��Ⱥ��Ϣ
// *		����ÿ����ֱ�Ƕ�ƽ�����ϰ������ζμ���(��Ⱥ��)
// */
//int VelodyneAlgo::CalClusterLineAndFeature(VelodyneDataStruct& scanobj)
//{
//    scanobj.scanClusterLineArray.clear();
//    scanobj.scanClusterLineFeatureArray.clear();
//
//    scanobj.scanClusterLineArray.resize(scanobj.scanPieClusterSet.size());
//    scanobj.scanClusterLineFeatureArray.resize(scanobj.scanPieClusterSet.size());
//    for (int i = 0; i<scanobj.scanPieClusterSet.size(); i++)
//    {
//        /*
//         *	64��circle ����64�ֲ�ͬ�Ĵ�ֱ�Ƕ�
//         *	scanobj.scanClusterLineArray[i][CircleID(����û��0~63)][0��������]: ÿ����ֱ�Ƕ�ƽ�����ϰ������ζμ���
//         */
//        TransferClusterToLineStructure(scanobj.scanPieClusterSet[i], scanobj.scanClusterLineArray[i], scanobj);
//        //CalClusterLineFeature(scanobj.scanClusterLineArray[i], scanobj.scanClusterLineFeatureArray[i]);
//        //CalClusterLShapeFeature(scanobj.scanClusterLineArray[i],scanobj.scanClusterLineFeatureArray[i]);
//    }
//    return 0;
//}
///*
// *	����shotID(packet���յ��Ⱥ�˳��)��С��������
// */
//bool cmpByShotNum(VelodyneDataStruct::xpoint_t* a, VelodyneDataStruct::xpoint_t* b)
//{
//    return a->shotID < a->shotID;
//}
///*
// *	��ȡ��Ⱥ(�ϰ��ＯȺ)��ÿ��Circle�ϵ������߶�
// */
//int VelodyneAlgo::TransferClusterToLineStructure(VelodyneDataStruct::pieCluster_t& clu, VelodyneDataStruct::clusterLine_t& cline, VelodyneDataStruct& scanobj)
//{
//    /*
//     *	typedef vector<line_t> circleLine_t;
//     *	typedef vector<xpoint_t*> line_t;
//     *	xpoint_t: ��¼һ����������������
//     */
//    VelodyneDataStruct::circleLine_t allScanCircleLine;
//    VelodyneDataStruct::circleLine_t validScanCircleLine;
//    allScanCircleLine.resize(CIRCLE_NUM);
//
//    int i, j, k;
//    float threshod = 80;
//
//    for (j = 0; j <clu.size(); j++)
//    {
//        /*
//         *	pCell: һ��������Ԫ
//         *	circleID: ��ֱƽ���-->��(0-CIRCLE_NUM)
//         */
//        VelodyneDataStruct::cell_t* pcellObj = clu[j]->pCell;
//        int pointSize = pcellObj->size();
//        for (k = 0; k<pointSize; k++)
//        {
//            int id = (*pcellObj)[k]->circleID;
//            VelodyneDataStruct::xpoint_t& pt = *(*pcellObj)[k];
//            if (pt.point_type & POINT_TYPE_INVALID)
//                continue;
//            allScanCircleLine[id].push_back(&pt);
//        }
//    }
//    /*
//     *	ÿ��circle�ĵ��Ʊ�����ڼ�Ⱥ�У�����Ϊ��Ч��circle(�ô�ֱ�Ƕ�ƽ�治���������߶�)
//     *	validScanCircleLine[i]����һ����ֱ�Ƕ�ƽ��
//     */
//    for (i = 0; i<allScanCircleLine.size(); i++)
//    {
//        if (allScanCircleLine[i].size()>1)
//            validScanCircleLine.push_back(allScanCircleLine[i]);
//    }
//    // validScanCircleLine[CircleID][ShotID]
//    allScanCircleLine.clear();
//    cline.resize(validScanCircleLine.size());
//
//    // ��Ⱥ�а����ĸ���Circle�ĵ������� --> Ѱ����ЩCircleƽ���ϵ������߶�
//    for (i = 0; i<validScanCircleLine.size(); i++)
//    {
//        // ÿ��circle����ĵ�������(ͬһ����ֱ�Ƕ�ƽ��)����shortID(֡�ţ����������ݽ��յ��Ⱥ�˳��)��С��������
//        sort(validScanCircleLine[i].begin(), validScanCircleLine[i].end(), cmpByShotNum);
//
//        // Ѱ�Ҹ���Circle�е������߶�
//        VelodyneDataStruct::line_t circleLineSegment;
//        VelodyneDataStruct::circleLine_t scanCircleLine;
//        /*
//         *	������ʼ֡
//         *	ͬһ��ֱ�Ƕ�ƽ��ǰ����֡�������ݾ���ƽ��ԭ��������(>80cm) �������������߶�
//         *	scanCircleLine: ͬһ��ֱƽ���������һ����ϰ���Ϣ
//         *	circleLineSegment[i]: ͬһ��ֱ�Ƕ�ƽ���������߶��ϵĵ�
//         *	scanCircleLine[i]: ͬһ��ֱ�Ƕ�ƽ���ڵ������߶μ���
//         */
//        circleLineSegment.push_back(validScanCircleLine[i][0]);
//        for (j = 1; j<validScanCircleLine[i].size(); j++)
//        {
//            // �ղ������߶��ϵĵ�
//            if (absf(validScanCircleLine[i][j]->rad - validScanCircleLine[i][j - 1]->rad) < threshod)
//                circleLineSegment.push_back(validScanCircleLine[i][j]);
//            // �ղ�Circle�ϵ������߶�
//            if (absf(validScanCircleLine[i][j]->rad - validScanCircleLine[i][j - 1]->rad) >= threshod
//                    || j == validScanCircleLine[i].size() - 1)
//            {
//                scanCircleLine.push_back(circleLineSegment);
//                circleLineSegment.clear();
//                circleLineSegment.push_back(validScanCircleLine[i][j]);
//            }
//        }
//        /*
//         *	scanCircleSegNum: ��ǰCircle�������߶���Ŀ
//         *	lastSegPtNum: ĳ���߶���?�������(��֡)����
//         */
//        int scanCircleSegNum = scanCircleLine.size();
//        int lastSegPtNum = scanCircleLine[scanCircleSegNum - 1].size();
//        // ��β���������߶��ܹ�����һ��!!
//        if (scanCircleLine.size()>1
//                && absf(scanCircleLine[0][0]->rad - scanCircleLine[scanCircleSegNum - 1][lastSegPtNum - 1]->rad) < threshod)
//        {
//            /*
//             *	firstSegPtNum: ���һ�����ζ���?�������(��֡)����
//             *	���׶����ζΰ���˳/��ʱ��ƴ����ĩ�����ζ�
//             *	ƴ�Ӻ��ٺϲ���ǰ��ֱ�Ƕ�ƽ������ζ�
//             */
//            int firstSegPtNum = scanCircleLine[0].size();
//            for (k = 0; k<firstSegPtNum; k++)
//                scanCircleLine[scanCircleSegNum - 1].push_back(scanCircleLine[0][k]);
//            // �׶���Ϊ������β���� �ʶ���
//            for (int m = 1; m<scanCircleSegNum; m++)
//            {
//                cline[i].push_back(scanCircleLine[m]);
//            }
//        }
//        // ����ϲ� ֱ���ղص�ǰCircle�������߶μ���
//        else
//        {
//            cline[i] = scanCircleLine;
//        }
//    }
//    return 0;
//}
///*
// *	���㼯Ⱥ��ͬһCircle�ϵ������߶ε�����
// *		ÿ��Circle�������߶��ܳ��� ��߶γ���
// *		����Circle�ϵ������߶��ܳ��� �Circle�߶��ܳ���
// *		ƽ��Circle�ܳ���
// */
//int VelodyneAlgo::CalClusterLineFeature(VelodyneDataStruct::clusterLine_t& clu_line, VelodyneDataStruct::clusterLineFeature_t& clu_lineFeature)
//{
//    int i, j, k, m, n;
//    // ��ʼ��
//    clu_lineFeature.circleLineFeature.resize(clu_line.size());
//    clu_lineFeature.averageCircleLineLength = 0;
//    clu_lineFeature.validCircleNum = 0;
//    clu_lineFeature.totalLength = 0;
//    clu_lineFeature.maxSegLength = 0;
//    clu_lineFeature.maxCircleLineLength = 0;
//    clu_lineFeature.ptNum = 0;
//    clu_lineFeature.aveX = 0;
//    clu_lineFeature.aveZ = 0;
//    clu_lineFeature.aveY = 0;
//    clu_lineFeature.Lshape = 0;
//    clu_lineFeature.averageCircleLineLength = 0;
//    for (i = 0; i<4; i++)
//        for (j = 0; j<2; j++)
//            clu_lineFeature.LshapeBoxVex[i][j] = 0;
//    clu_lineFeature.LshapeDirection = 0;
//    clu_lineFeature.LshapeLength = 0;
//    clu_lineFeature.LshapeWidth = 0;
//    clu_lineFeature.LshapeDeflection = 0;
//
//    for (i = 0; i<clu_line.size(); i++)
//    {
//        clu_lineFeature.validCircleNum++;
//        clu_lineFeature.circleLineFeature[i].segNum = clu_line[i].size();
//        clu_lineFeature.circleLineFeature[i].totalLength = 0;
//        clu_lineFeature.circleLineFeature[i].ptNum = 0;
//        clu_lineFeature.circleLineFeature[i].maxSegLength = 0;
//        clu_lineFeature.circleLineFeature[i].aveZ = 0;
//        clu_lineFeature.circleLineFeature[i].aveX = 0;
//        clu_lineFeature.circleLineFeature[i].aveY = 0;
//        clu_lineFeature.circleLineFeature[i].Lshape = 0;
//        for (m = 0; m<4; m++)
//            for (n = 0; n<2; n++)
//                clu_lineFeature.circleLineFeature[i].LshapeBoxVex[m][n] = 0;
//        clu_lineFeature.circleLineFeature[i].LshapeDirection = 0;
//        clu_lineFeature.circleLineFeature[i].LshapeLength = 0;
//        clu_lineFeature.circleLineFeature[i].LshapeWidth = 0;
//        clu_lineFeature.circleLineFeature[i].LshapeDeflection = 0;
//        // ����ÿ��Circle�����ÿ�������߶�
//        for (j = 0; j<clu_line[i].size(); j++)
//        {
//            VelodyneDataStruct::clusterLineSegFeature_t tempSegFeature;
//            tempSegFeature.ptNum = clu_line[i][j].size();
//            // �߶����/�յ� �����
//            tempSegFeature.startAngle = CalDirectionAngle(0, 0, clu_line[i][j][0]->x, clu_line[i][j][0]->y);
//            tempSegFeature.endAngle = CalDirectionAngle(0, 0, clu_line[i][j][clu_line[i][j].size() - 1]->x, clu_line[i][j][clu_line[i][j].size() - 1]->y);
//            if (tempSegFeature.endAngle < tempSegFeature.startAngle)
//                tempSegFeature.endAngle += 360;
//            // ·�߳��� X/Y/Z��ֵ
//            tempSegFeature.length = 0;
//            tempSegFeature.aveZ = clu_line[i][j][0]->z;
//            tempSegFeature.aveX = clu_line[i][j][0]->x;
//            tempSegFeature.aveY = clu_line[i][j][0]->y;
//            for (k = 1; k<clu_line[i][j].size(); k++)
//            {
//                float xLength = clu_line[i][j][k]->x - clu_line[i][j][k - 1]->x;
//                float yLength = clu_line[i][j][k]->y - clu_line[i][j][k - 1]->y;
//                tempSegFeature.length += sqrt(xLength*xLength + yLength*yLength);
//
//                tempSegFeature.aveZ += clu_line[i][j][k]->z;
//                tempSegFeature.aveX += clu_line[i][j][k]->x;
//                tempSegFeature.aveY += clu_line[i][j][k]->y;
//            }
//            // ��������Circle��X/Y/Z��ֵ
//            clu_lineFeature.circleLineFeature[i].aveX += tempSegFeature.aveX;
//            clu_lineFeature.circleLineFeature[i].aveY += tempSegFeature.aveY;
//            clu_lineFeature.circleLineFeature[i].aveZ += tempSegFeature.aveZ;
//            // ����Circle��ÿһ�������߶ε�X/Y/Z��ֵ
//            tempSegFeature.aveX /= tempSegFeature.ptNum;
//            tempSegFeature.aveY /= tempSegFeature.ptNum;
//            tempSegFeature.aveZ /= tempSegFeature.ptNum;
//            // ����ÿ��Circle�������߶εļ��������Ŀ
//            clu_lineFeature.circleLineFeature[i].ptNum += tempSegFeature.ptNum;
//            // ����ÿ��Circle�������߶���ɵ��ܳ���
//            clu_lineFeature.circleLineFeature[i].totalLength += tempSegFeature.length;
//            // Ѱ��ÿ��Circle��������߶�
//            if (clu_lineFeature.circleLineFeature[i].maxSegLength < tempSegFeature.length)
//                clu_lineFeature.circleLineFeature[i].maxSegLength = tempSegFeature.length;
//
//            clu_lineFeature.circleLineFeature[i].lineSegFeature.push_back(tempSegFeature);
//        } // end of for (j = 0; j<clu_line[i].size(); j++){
//        // ÿ��Circle�׶���� β�ν��ķ����
//        clu_lineFeature.circleLineFeature[i].startAngle = clu_lineFeature.circleLineFeature[i].lineSegFeature[0].startAngle;
//        clu_lineFeature.circleLineFeature[i].endAngle = clu_lineFeature.circleLineFeature[i].lineSegFeature[clu_line[i].size() - 1].endAngle;
//        // ����Circle�����������߶����е�� X/Y/Z��ֵ
//        clu_lineFeature.aveZ += clu_lineFeature.circleLineFeature[i].aveZ;
//        clu_lineFeature.aveY += clu_lineFeature.circleLineFeature[i].aveY;
//        clu_lineFeature.aveX += clu_lineFeature.circleLineFeature[i].aveX;
//
//        clu_lineFeature.circleLineFeature[i].aveZ /= clu_lineFeature.circleLineFeature[i].ptNum;
//        clu_lineFeature.circleLineFeature[i].aveY /= clu_lineFeature.circleLineFeature[i].ptNum;
//        clu_lineFeature.circleLineFeature[i].aveX /= clu_lineFeature.circleLineFeature[i].ptNum;
//        // ����Circle�����������߶��ܳ���
//        clu_lineFeature.totalLength += clu_lineFeature.circleLineFeature[i].totalLength;
//        // ����Circle�������߶ΰ����ĵ����Ŀ
//        clu_lineFeature.ptNum += clu_lineFeature.circleLineFeature[i].ptNum;
//        // ��¼��߶εĳ���
//        if (clu_lineFeature.maxSegLength<clu_lineFeature.circleLineFeature[i].maxSegLength)
//            clu_lineFeature.maxSegLength = clu_lineFeature.circleLineFeature[i].maxSegLength;
//        // ��¼�����߶��ܳ�������Circle ���ܳ���
//        if (clu_lineFeature.maxCircleLineLength<clu_lineFeature.circleLineFeature[i].totalLength)
//            clu_lineFeature.maxCircleLineLength = clu_lineFeature.circleLineFeature[i].totalLength;
//    } // end of for (i = 0; i<clu_line.size(); i++){
//    // ��Ⱥ�����������߶ε�ƽ��Circle����
//    clu_lineFeature.averageCircleLineLength = clu_lineFeature.totalLength / clu_lineFeature.validCircleNum;
//
//    clu_lineFeature.aveX /= clu_lineFeature.ptNum;
//    clu_lineFeature.aveY /= clu_lineFeature.ptNum;
//    clu_lineFeature.aveZ /= clu_lineFeature.ptNum;
//    return 0;
//}

/*
 *
 */
//int VelodyneAlgo::CalClusterLShapeFeature(clusterLine_t& clu_line, clusterLineFeature_t& clu_lineFeature){
//	int i;
//	for (i = 0; i<clu_line.size(); i++){
//		circleLine_t &cl = clu_line[i];
//		clusterCircleLineFeature_t &clf = clu_lineFeature.circleLineFeature[i];
//		//LshapeLine scanLshapeLine;
//		//scanLshapeLine.CalBestLMatchBox(clf, cl);
//		//scanLshapeLine.DrawBestLMatchBox(clf);
//	}
//	return 0;
//}

