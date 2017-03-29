/* *************************************************************************
 *
 * scan data initializes, from raw data or .pcd data
 *
 * statistics function, including init/calc/dump(keep in file)
 *
 ***************************************************************************/

#include "velodyneDataStruct.h"
#include <assert.h>
#include <fstream>
// need to define _USE_MATH_DEFINES for <math.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <string.h>
//#undef max
//#include <pcl/io/pcd_io.h>
//#include <pcl/point_types.h>
//#ifndef _CONSOLE
//#include <afx.h>
//#endif

//using namespace std;

static inline float absf(float a)
{
    if (a < 0)
        return -a;
    return a;
}

VelodyneDataStruct::VelodyneDataStruct()
    : sorted(false)
{
    //circlesStatistics.resize(64);
    //circlesLines.resize(64);
    assert(sizeof(velodyne_packet_t) == 1206);

    circleGridWidth = 4;
    calcCircleFeatured = false;
    calcCirclesLined = false;
    calcRoadCurb = false;
}

VelodyneDataStruct::~VelodyneDataStruct()
{
}

// initialize statistics to 0
void VelodyneDataStruct::InitStatistics(pointStatistics_t& pStatistics)
{
    memset(&pStatistics, 0, sizeof(pStatistics));
}

///*
// *	added by durant35
// *		��ͳ�����ݱ��浽 filename ָ�����ļ�
// */
//int VelodyneDataStruct::DumpStatistics(const char* filename){
//	if (calcCircleFeatured == false)
//		return -1;
//
//	int len, i;;
//
//	std::ofstream file;
//
//	file.open(filename, std::ios::out);
//
//	for (i = 0; i < 64; ++i){
//		/*
//		file<<"["<<i<<"]rad:"<<circlesStatistics[i].max_rad<<"\t"<<circlesStatistics[i].avg_rad<<"\t"<<circlesStatistics[i].min_rad
//		<<"     drtZ:"<<circlesStatistics[i].max_scanline_drtZ<<"\t"<<circlesStatistics[i].min_scanline_drtZ
//		<<"     drtRad:"<<circlesStatistics[i].max_scanline_drtRad<<"\t"<<circlesStatistics[i].min_scanline_drtRad
//		<<std::endl;
//		*/
//		float tan = shots[0].pt[i].x / shots[100].pt[i].y;
//		float div = 360.0 / 2202.0;
//		file << "[" << i << "]" << div << " " << (atan(tan) * 360 / (2 * M_PI)) / div << std::endl;
//
//		//printf("write[%d]  offset[%u] number[%d]\n",i,m_index[i].offset,m_index[i].scanNumber);
//	}
//
//	file.close();
//	return 0;
//}

///*
// *	addded by durant35
// *		�����������shots�е����ݽ�������Ȼ�󷵻�
// *		shots���ܹ���shots.size()֡ ÿ֡��64�����ݣ���Ӧ��64�߼����״�
// *		return a new sorted scan
// *		//TODO: How sort
// */
//VelodyneDataStruct VelodyneDataStruct::sort(void)
//{
//    VelodyneDataStruct sortedScan;
//    sortedScan.shots.resize(2160);
//
//    std::vector<float> tanv;
//    int i, j, count = 0;
//    // <math.h>
//    float inc = M_PI / 1080;
//    float flag;
//    int offset;
//    std::vector<float>::iterator result;
//    /*
//     *	shots[i].pt[j].rad
//     *	shots[i].pt[j].tan_theta
//     */
//    calc();
//
//    /*
//     *	ֻ�����ķ�֮��Բ��
//     */
//    for (i = 0; i<270; ++i)
//    {
//        tanv.push_back(tan(inc*i));
//    }
//    //printf("shots.size= %d\n", shots.size());
//
//    for (i = 0; i<shots.size(); ++i)
//    {
//        for (j = 0; j<VELODYNE_NUM_BEAMS_IN_ONE_SHOT; ++j)
//        {
//            count++;
//            // pt, current data of No.j for Frame i
//            xpoint_t& pt = shots[i].pt[j];
//            //printf("%d %d %d %f %f %f\n",i,j,count,pt.x,pt.y,pt.tan_theta);
//            /*
//             *	�����м���㰴��tan_theta��������
//             */
//            if (pt.x>0 && pt.y>0)
//            {
//                if (pt.x>pt.y)
//                {
//                    flag = pt.tan_theta;
//                    // <alogorithm>
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 269;
//                    }
//                    else
//                    {
//                        offset = result - tanv.begin();
//                    }
//                }
//                else
//                {
//                    flag = 1 / pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 270;
//                    }
//                    else
//                    {
//                        offset = 539 - (result - tanv.begin());
//                    }
//                }
//            }
//            else if (pt.x<0 && pt.y>0)
//            {
//                if (-pt.x>pt.y)
//                {
//                    flag = -pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 810;
//                    }
//                    else
//                    {
//                        offset = 1079 - (result - tanv.begin());
//                    }
//                }
//                else
//                {
//                    flag = 1 / -pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 809;
//                    }
//                    else
//                    {
//                        offset = 540 + (result - tanv.begin());
//                    }
//                }
//            }
//            else if (pt.x<0 && pt.y<0)
//            {
//                if (-pt.x>-pt.y)
//                {
//                    flag = pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 269 + 1080;
//                    }
//                    else
//                    {
//                        offset = 1080 + (result - tanv.begin());
//                    }
//                }
//                else
//                {
//                    flag = 1 / pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 1080 + 270;
//                    }
//                    else
//                    {
//                        offset = 1080 + 539 - (result - tanv.begin());
//                    }
//                }
//            }
//            else if (pt.x>0 && pt.y<0)
//            {
//                if (pt.x>-pt.y)
//                {
//                    flag = -pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 1080 + 810;
//                    }
//                    else
//                    {
//                        offset = 1080 + 1079 - (result - tanv.begin());
//                    }
//                }
//                else
//                {
//                    flag = 1 / -pt.tan_theta;
//                    result = upper_bound(tanv.begin(), tanv.end(), flag);
//                    if (result == tanv.end())
//                    {
//                        offset = 1080 + 809;
//                    }
//                    else
//                    {
//                        offset = 1080 + 540 + (result - tanv.begin());
//                    }
//                }
//            }
//            else
//            {
//                continue;
//            }
//            //printf("[%d][%d]offset=%d\n",i,j,offset);
//            //ASSERT(offset>-1&&offset<2160);
//            sortedScan.shots[offset].pt[j] = pt;
//        }
//    }
//
//    return sortedScan;
//}
//
///*
// *	added by durant35
// *		do some caculation for every data in all frame in shots
// *			pt.rad: x-yƽ��뾶
// *			pt.tan_theta: ͶӰ��x-yƽ����x��н�����ֵ
// *		result makes change to data in shots.
// */
//void VelodyneDataStruct::calc(void)
//{
//    int i, j;
//    for (i = 0; i<shots.size(); ++i)
//    {
//        for (j = 0; j<VELODYNE_NUM_BEAMS_IN_ONE_SHOT; ++j)
//        {
//            // use & to change the value of shots
//            xpoint_t& pt = shots[i].pt[j];
//            if (pt.point_type & POINT_TYPE_INVALID)
//                continue;
//            pt.rad = sqrt(pt.x*pt.x + pt.y*pt.y);
//            pt.tan_theta = pt.y / pt.x;
//        }
//    }
//}
//
///*
// *		����ͳ����Ϣ��Ŀǰ��û�õ�
// *		pt.scanline_drtZ
// *			ÿ�������ɨ����(ʱ�们������)�߳�ƽ����
// *		pt.scanline_drtRad
// *			ÿ�������ɨ����(ʱ�们������)�뾶ƽ����
// *		ͳ��ÿ��circle��Ϣ
// *			min_x/max_x;min_y/max_y;min_z/max_z
// *			max_scanline_drtRad;min_scanline_drtRad
// *			max_scanline_drtZ;min_scanline_drtZ
// *			avg_rad: circle����Ч��ƽ���뾶
// *		���º� calcCircleFeatured == true
// */
//void VelodyneDataStruct::calcCircleFeature(void)
//{
//    int i, j, k;
//    float aveZ, drtZ;
//    float aveRad, drtRad;
//    int count;
//    int validCount;
//    int GridWidth = circleGridWidth * 2 + 1;
//    int ShotSize = shots.size();
//    int pos;
//    int *dummy;
//    // initialize to 0
//    InitStatistics(scanStatistics);
//    // do some caculation(rad & tan_theta) mentioned above function
//    calc();
//    for (j = 0; j<VELODYNE_NUM_BEAMS_IN_ONE_SHOT; ++j)
//    {
//        /*
//         *	ʹ�û������ڼ���ÿ��circle��ÿ�����ƽ���� ---> scanline_drtRad��scanline_drtZ
//         *	circlesStatistics����ÿ��circle��ͳ����Ϣ
//         *		min_x/max_x;min_y/max_y;min_z/max_z
//         *		max_scanline_drtRad;min_scanline_drtRad
//         *		max_scanline_drtZ;min_scanline_drtZ
//         *		avg_rad: circle����Ч��ƽ���뾶
//         */
//        InitStatistics(circlesStatistics[j]);
//        validCount = 0;
//        circlesStatistics[j].avg_rad = 0.0;
//        for (i = 0; i<ShotSize; ++i)
//        {
//            xpoint_t&  pt = shots[i].pt[j];
//            if (pt.point_type & POINT_TYPE_INVALID)
//                continue;
//
//            validCount++;
//            circlesStatistics[j].avg_rad += pt.rad;
//            aveZ = 0.0;
//            drtZ = 0.0;
//            aveRad = 0.0;
//            drtRad = 0.0;
//            count = 0;
//
//            // ��������(��СGridWidth)����ÿ��circle���ڽ������Z��R��ֵ �����Ըõ�Ϊ���ĵĻ���ڵ�ƽ����
//            for (k = 0; k< GridWidth; k++)
//            {
//                pos = i - circleGridWidth + k;
//                if (pos>-1 && pos<ShotSize)
//                {
//                    xpoint_t&  ptSlide = shots[pos].pt[j];
//                    if (ptSlide.point_type & POINT_TYPE_INVALID)
//                        continue;
//
//                    aveZ += ptSlide.z;
//                    aveRad += ptSlide.rad;
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
//            }
//
//            for (k = 0; k<GridWidth; k++)
//            {
//                pos = i - circleGridWidth + k;
//                if (pos>-1 && pos<ShotSize)
//                {
//                    xpoint_t& ptSlide = shots[pos].pt[j];
//                    if (ptSlide.point_type & POINT_TYPE_INVALID)
//                        continue;
//                    drtZ += absf(ptSlide.z - aveZ);
//                    drtRad += absf(ptSlide.rad - aveRad);
//                    count++;
//                }
//            }
//            drtZ /= (count *1.0);
//            drtRad /= (count *1.0);
//            pt.scanline_drtZ = drtZ;
//            pt.scanline_drtRad = drtRad;
//            /*
//             *	��ʼ��ͳ����
//             *	ͳ������Ŀǰû��ô��
//             */
//            dummy = reinterpret_cast<int*>(&(circlesStatistics[j].max_rad));
//            if (*dummy == 0)
//            {
//                circlesStatistics[j].max_rad = 0.0;
//                circlesStatistics[j].min_rad = pt.rad;
//
//                circlesStatistics[j].max_x = pt.x;
//                circlesStatistics[j].min_x = pt.x;
//
//                circlesStatistics[j].max_y = pt.y;
//                circlesStatistics[j].min_y = pt.y;
//
//                circlesStatistics[j].max_z = pt.z;
//                circlesStatistics[j].min_z = pt.z;
//
//                circlesStatistics[j].max_scanline_drtRad = pt.scanline_drtRad;
//                circlesStatistics[j].min_scanline_drtRad = pt.scanline_drtRad;
//
//                circlesStatistics[j].max_scanline_drtZ = pt.scanline_drtZ;
//                circlesStatistics[j].min_scanline_drtZ = pt.scanline_drtZ;
//            }
//            // ���¼�ֵ, max_rad, min_rad, max_x, min_x, max_y, min_y, min_z, max_z
//            if (circlesStatistics[j].max_rad<pt.rad)
//                circlesStatistics[j].max_rad = pt.rad;
//
//            if (circlesStatistics[j].min_rad>pt.rad)
//                circlesStatistics[j].min_rad = pt.rad;
//
//            if (circlesStatistics[j].max_x<pt.x)
//                circlesStatistics[j].max_x = pt.x;
//
//            if (circlesStatistics[j].min_x>pt.x)
//                circlesStatistics[j].min_x = pt.x;
//
//            if (circlesStatistics[j].max_y<pt.y)
//                circlesStatistics[j].max_y = pt.y;
//
//            if (circlesStatistics[j].min_y>pt.y)
//                circlesStatistics[j].min_y = pt.y;
//
//            if (circlesStatistics[j].max_z<pt.z)
//                circlesStatistics[j].max_z = pt.z;
//
//            if (circlesStatistics[j].min_z>pt.z)
//                circlesStatistics[j].min_z = pt.z;
//
//            if (circlesStatistics[j].max_scanline_drtRad<pt.scanline_drtRad)
//                circlesStatistics[j].max_scanline_drtRad = pt.scanline_drtRad;
//
//            if (circlesStatistics[j].min_scanline_drtRad>pt.scanline_drtRad)
//                circlesStatistics[j].min_scanline_drtRad = pt.scanline_drtRad;
//
//            if (circlesStatistics[j].max_scanline_drtZ<pt.scanline_drtZ)
//                circlesStatistics[j].max_scanline_drtZ = pt.scanline_drtZ;
//
//            if (circlesStatistics[j].min_scanline_drtZ>pt.scanline_drtZ)
//                circlesStatistics[j].min_scanline_drtZ = pt.scanline_drtZ;
//        }
//
//        circlesStatistics[j].avg_rad = circlesStatistics[j].avg_rad / validCount;
//    }
//
//    calcCircleFeatured = true;
//}


/*
 * cpp static member variable initialization
 *		magic for raw data!!!
 *		"rw3" == 0x72773300
 */
const char VelodyneDataRaw::magic[4] = "rw3";

VelodyneDataRaw::VelodyneDataRaw()
{
    assert(sizeof(velodyne_packet_t) == 1206);
}

VelodyneDataRaw::~VelodyneDataRaw()
{
}

/*
 *	added by durant35
 *		filename ָ����.raw��ʽ�ļ�ת���� shots ����
 */
int VelodyneDataRaw::deserialize(const char* filename)
{
    char buffer[sizeof(VelodyneDataRaw::magic)];
    int* p = reinterpret_cast<int*> (buffer);
    int len, i;
    shot_t shotobj;
    std::ifstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    if (file.is_open() == false)
    {
        printf("%s does not exist!!\n", filename);
        return -1;
    }

    // read magic bits, should be the same as scan::magic
    file.read(buffer, sizeof(VelodyneDataRaw::magic));
    if (memcmp(buffer, VelodyneDataRaw::magic, sizeof(VelodyneDataRaw::magic)) != 0)
    {
        printf("Not an raw scan file!!\n");
        file.close();
        return -1;
    }
    // magic �����ĸ��ֽ��� size
    file.read(buffer, sizeof(VelodyneDataRaw::magic));

    len = *p;
    shots.clear();
    /*
     *		���ݰ�֡���桢��֡��ȡ
     *		һ֡����64�� xpoint_t
     */
    for (i = 0; i < len; ++i)
    {
        file.read(reinterpret_cast<char*> (&shotobj), sizeof(shot_t));
        //printf("read[%d]  offset[%u] number[%d]\n",i,ii.offset,ii.scanNumber);
        shots.push_back(shotobj);
    }
    file.close();

    printf("deserialize %d done\n", len);

    return 0;
}

/*
 *		saveToPCD == true���� shots ���ݱ����.pcd��ʽ
 *		saveToPCD == false���� shots ���ݱ����.raw��ʽ
 */
int VelodyneDataRaw::serialize(const char* filename, bool saveToPCD)
{
    int len, i;
    // �����raw��ʽ
    if (saveToPCD == false)
    {
        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::binary);
        // write magic bits
        file.write(VelodyneDataRaw::magic, 4);
        // write size
        len = shots.size();
        file.write(reinterpret_cast<char*>(&len), 4);
        for (i = 0; i < len; ++i)
        {
            file.write(reinterpret_cast<char*> (&shots[i]), sizeof(shot_t));
            //printf("write[%d]  offset[%u] number[%d]\n",i,m_index[i].offset,m_index[i].scanNumber);
        }
        file.close();
    }

    //// �����pcd��ʽ, use pcl API
    //else{
    //	pcl::PointCloud<pcl::PointXYZ> cloud;
    //	// Fill in the cloud data
    //	cloud.width = shots.size() * 64;
    //	cloud.height = 1;
    //	cloud.is_dense = false;
    //	cloud.points.resize(cloud.width * cloud.height);
    //	size_t shotID, pointID;
    //	for (size_t i = 0; i < cloud.points.size(); ++i){
    //		shotID = i / 64;
    //		pointID = i % 64;
    //		cloud.points[i].x = shots[shotID].pt[pointID].x;
    //		cloud.points[i].y = shots[shotID].pt[pointID].y;
    //		cloud.points[i].z = shots[shotID].pt[pointID].z;
    //	}
    //	//pcl::io::savePCDFileASCII ("test_pcd.pcd", cloud);
    //	pcl::io::savePCDFileBinary(filename, cloud);
    //}

    return 0;
}



