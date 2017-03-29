#ifndef _CONFIG_STRUCT_H_
#define _CONFIG_STRUCT_H_

/* *************************************************************************
 *
 *	ȫ������
 *	�����ļ� ./velodyne.ini
 *	���ýṹ��
 *		CfgVeloView: ����������ṹ�嶨��
 *			CfgCircleItem: ÿ��circle������ѡ���
 *			CfgPlaneDetect: ƽ��������ѡ���
 *			CfgGlobal: ȫ���������
 *			CfgShotOffset:
 *			CfgInterSection: ·���㷨����
 *			CfgIntersection: ·���㷨����(�µ�·���㷨)
 *			CfgIgnoreCircleWhenProjection:
 *			CfgCarShapeParameter:
 *
 ***************************************************************************/

// ���߼����״�
#ifdef USE_VLP_16_
#define CIRCLE_NUM 16
#elif USE_HDL_64E_
#define CIRCLE_NUM 64
#else
#define CIRCLE_NUM 32
#endif

// ȫ���������
typedef struct
{
    int GroundZ;						// ����߳�

    int ThresholdMinHollow;				// �п������жϣ���Сֵ
    int ThresholdMaxHollow;				// �п������жϣ����ֵ

    int ShotCalcFrom;					// �ӵ�ShotCalcFrom��shot��ʼ��������
    int ShotCalcNum;					// һ������ShotCalcNum��shot
    int ShotShowFrom;					// �ӵ�ShotShowFrom��shot��ʼ��ʾ
    int ShotShowNum;					// һ����ʾShotShowNum��shot

    int ContinusLineMinDistInCircle;	// Circle�������߶���С����

    float ThresholdMinRad;				// ������ֵ

    int CarLength;
    int CarWidth;
    int CarHeight;

    // distance filter for raw data
    int MaxLaserDistacne;
    int MinLaserDistance;

    //int MaxCalRad;						// ���㷶Χ��������ֵ������
    //int MinCalRad;						// ���㷶Χ��С�ڸ�ֵ������

    //int ColorMode;						// ȫ����ʾģʽ��ֻ��Ϊ0ʱ��circle����ʾģʽΪ0����Ч

    //	int CarFrontX;                    /**<��ǰ��Xֵ*/
    //	float ThresholdDeltaI;            /**< ����ǿ����ֵ*/
    //	float DeltaIColorFactor;          /**< ����ǿ����ɫ��ʾ����*/
    //	int   arg0;                       /**< reserved*/
    //	int   arg2;                       /**< reserved*/
    //	int   arg3;                       /**< reserved*/
    //	int   arg4;                       /**< reserved*/
    //	int   arg5;                       /**< reserved*/
    //	int   arg6;                       /**< reserved*/
    //	int   arg7;                       /**< reserved*/
    //	float ThresholdRoadEdgeDeltaZLLimit; /** ɨ������·�ص�deltaZ������*/
    //	float ThresholdRoadEdgeDeltaZULimit;/** ɨ������·�ص�deltaZ������*/
    //	int RoadCurbStartCircle;  /** ɨ������·�ص�:��ʼȦ��*/
    //	int RoadCurbEndCircle;  /** ɨ������·�ص�:����Ȧ��*/
    //	int RoadCurbMaxDis;    /** ɨ������·�ص�:��������ڵĿ���*/
    //	int MinLaneNum;        /** ��ǰ·�����ٳ����� */
    //	int MinLaneWidth;      /** ������С��� */
} CfgGlobal_t;
// ƽ��������ѡ���
typedef struct
{
    // ��������
    int GridCellSize;					// ��Ԫ���С����λΪ����
    int GridHeadRowNum;					// ��Ԫ���ǰ�����������ӳ�ͷǰ����*/
    int GridBackRowNum;					// �����󷽵�Ԫ�������(��������س�ǰ������ ��0�У���1��...)
    int GridColumnNum;					// ��Ԫ������������Ϊ�����Σ�ԭ�������м䣬�е㵽4�ߵ���������(//TODO: Ӧ���Ǻ��Ż������ŷ�������)
    float GridThresholdGroundDetect;	// ƽ���� �ж���ֵ��С�ڸ���Ϊƽ��
    float GridThresholdDiffZ;
    float GridColorFactor;				// ��jet color ģʽ�� �����������ֵ��ɫ�ֲ�0-1
    int GridColorMode;					// ����circle����ʾģʽ
    // 0 Ϊjet colorģʽ(��0-1�����ڷֲ�1024����ɫ)
    // 1 Ϊ��ֵģʽ(������ֵ�ָͬ��ɫ)

    // ��������
    int PieRadSize;					    // ƽ����(�뾶��������ͬ����)��Ԫ��뾶����Ĵ�С����λΪ����*/
    int PieShotFrom;                    // shot range for valid obstacle detection
    int PieShotEnd;
    int PieAzimuthNum;					// ��Բ�ֳܷ�AzimuthSize��
    int PieMinRad;						// ƽ������㷶Χ ������(�뾶��Χ)
    int PieMaxRad;						// ƽ������㷶Χ ������
    float PieThresholdGroundDetect;		// ƽ���� �ж���ֵ��С�ڸ���Ϊƽ��
    float PieThresholdDiffZ;
    float PieColorFactor;				// ��jet color ģʽ�� �����������ֵ��ɫ�ֲ�0-1
    int PieColorMode;					// ����circle����ʾģʽ
    // 0 Ϊjet colorģʽ(��0-1�����ڷֲ�1024����ɫ)
    // 1 Ϊ��ֵģʽ(������ֵ�ָͬ��ɫ)


    //	int Enable;                        /**< �Ƿ����ƽ����*/
    //	/** Pie������*/
    //	int DeleteSingleRedGridMaxRow;    /**< ɾ��������ǰ�������и��ӷ�Χ�ڵĹ�������ӡ���ֵӦС�ڵ��ڷ����滮���������ROW */
    //	int DeleteSingleRedGridMaxNum;   /**< ɾ��������ǰ����������Ӽ�Ⱥ�������������3������˵ǰ������ӵļ�Ⱥ�������������������º���ӵģ����޳� */
} CfgPlaneDetect_t;
// ���ϰ�����㷨����
typedef struct
{
    int NegativeObstacleCellSize;				// ����߳�(cm)
    int NegativeObstacleGridDim;				// �����С
    float NegativeObstacleThresholdStartEdge;	// �����жϡ��ӡ�����������Ǹ���Ե
    float NegativeObstacleThresholdEndEdge;		// �����жϡ��ӡ�Զ�복�����һ����Ե
    int NegativeObstacleThresholdEdgeNumber;	// �����жϸ������Ƿ�Ϊ�ӵı�Ե����
} CfgNegativeObstacle_t;
// ��������ͶӰʱ�����ԵĴ�ֱ�Ƕ�ƽ��������
typedef struct
{
    int IgnoreCircle[32];
} CfgIgnoreCircleWhenProjection_t;

// ÿ��circle������ѡ���
typedef struct
{
    int Enable;								// �Ƿ�ʹ�ø���Circle��Ӧ�ļ����״�
    float ThresholdMinDist;					// ������ֵ��С�ڸ���ֵ ��ʾ��ƽ��
    float ThresholdRoadEdge;				// Delta R ��ֵ��С�ڸ�ֵ��Ϊ��ƽ̨·�棬����Ϊ·�ص�
    float ThresholdRoadEdgeByDeltaZ;     // Delta Z ��ֵ��С�ڸ�ֵ��Ϊ��ƽ̨·�棬����Ϊ·�ص�
//	int ColorMode;                       /**< ����circle����ʾģʽ��0 Ϊjet color ģʽ(��0-1�����ڷֲ�1024����ɫ)��1Ϊ��ֵģʽ(������ֵ�ָͬ��ɫ)*/
//	float ColorFactor;                   /**< ��jet color ģʽ�� ��������Delta R��ɫ�ֲ�0-1*/
//	float ColorFactorByDeltaZ;           /**< ��jet color ģʽ�� ��������Delta Z��ɫ�ֲ�0-1*/
} CfgCircleItem_t;

//typedef struct{
//	int offset[CIRCLE_NUM];
//} CfgShotOffset;
//typedef struct{
//	float DisL;/**<��ͷ����·�ڵľ���*/
//	float DisR;/**<��ͷ����·�ڵľ���*/
//	float LastDisLAboveZero;/**<��һ�γ�ͷ����·�ھ��������ľ���*/
//	float LastDisRAboveZero;/**<��һ�γ�ͷ����·�ھ��������ľ���*/
//	bool State1mOr5m; /**<Ϊfalseʱ�ṩ������5m,10m,15m������,Ϊtrueʱ�ṩ������1m,3m,6m����*/
//	bool StartCheckOut;/**Ϊ��ʱ��ʼ����Ƿ��ѹ�·��**/
//	bool OutOfInterSection;/**<�Ƿ��ѹ�·��*/
//	int  CheckDistance[6];/**<������⳵ͷ��·�ڵľ���*/
//	bool LeftReach[6];/**<�ڸ������봦�Ƿ���·��*/
//	bool RightReach[6];/**<�ڸ������봦�Ƿ���·��*/
//	int  SearchWide;/**<�����ķ������**/
//	int  SatisfyNum;/**<��Ϊ��·�ڵ���С�������**/
//	int  SatisfyContiniousNum;/**<��Ϊ��·�ڱ�����е�������������Ҫ��֤�㹻��ͨ��**/
//}  CfgInterSection;
// �µ�·���㷨�������µ��㷨�����ȶ����Ѿɵ��㷨�����
//typedef struct{
//	float RoadWide;				/**<��GIS���ߵ�������ʻ��·���,��λm**/
//	float IntersectionWide;		/**<��·����GIS���ߵ�·�ڿ��,��λm**/
//	float CheckPassageDis;		/**<Ԥ��ת��·���ܷ�ͨ���ļ��ֵ,�����Ƿ���Կ�ʼת��**/
//	float PositionDecision;		/**<�����·�ڿ�ȴ��ڴ�ֵʱ��Ԥ����ת��·���ķ�֮һλ�ã�С�ڴ�ֵ��ת��·���м�**/
//	float Multiple;				/**<Ϊ̽��ת��뾶��ת�ǹ�ϵ��ʱ���ã������뾶��õ�����ת�����˱���**/
//	float Power;				/**<Ϊ̽��ת��뾶��ת�ǹ�ϵ��ʱ���ã��̶�ת��뾶��õ����ֺ����ֽǶ�Ȩ��**/
//	float TestAngle;
//	float YPosAlliance;
//	float safeDis;
//	float turnLeftLimitDis;
//	float turnRightDis;
//	float endPos;
//	float turnRightScale;
//	float frontCalDis;
//	float frontMaxIntervalLWide;
//	float frontMaxIntervalLendXDis;
//	float interSectionAdvanceDis;
//	float roadShapeLengthFilter;
//	float slashLengthLimit;
//	float slashWidth;
//} CfgIntersection;

//typedef struct{
//	int min_length;
//	int max_length;
//	int min_width;
//	int max_width;
//	int max_length_width_ratio;
//}CfgCarShapeParameter;

// ����������ṹ�嶨��
typedef struct
{
    CfgGlobal_t cfgGlobal;										// ȫ��������
    CfgPlaneDetect_t cfgPlaneDetect;							// ƽ������
    CfgCircleItem_t cfgCircleItems[CIRCLE_NUM];					// Circle��
    CfgIgnoreCircleWhenProjection_t cfgIgnoreCircleWhenProjection;
    CfgNegativeObstacle_t cfgNegativeObstacle;					// ���ϰ�������
    //CfgShotOffset_t cfgShotOffset;
    //CfgInterSection_t cfginterSection;						// �ɵ�·��������
    //CfgIntersection_t cfgIntersecion;							// �µ�·��������
    //CfgCarShapeParameter_t cfgCarShapeParameter;
} CfgVeloView_t;

/*
 *  �������ļ��������õ����ýṹ��
 *  @param[out] cfg       Ҫд��Ľṹ��
 *  @param[in]  filename  Ҫ������ļ�
 *  @return     0:�ɹ� ����ʧ��
 */
int LoadCfgVeloView(CfgVeloView_t& cfg, const char* filechar);
/*
 *    �����ýṹ��д�뵽�����ļ�
 *    @param[in]  cfg       Ҫ��ȡ�Ľṹ��
 *    @param[out] filename  Ҫд����ļ�
 *    @return     0:�ɹ� ����ʧ��
 */
int SaveCfgVeloView(CfgVeloView_t& cfg, const char* filename);

extern CfgVeloView_t g_CfgVeloView;								// ȫ�����ýṹ�������

#endif

