#ifndef __VELODYNE_SCANNER_H__
#define __VELODYNE_SCANNER_H__

/**************************************************************************
 *
 *	�Ե�ǰϵͳ�еļ����״�֡(scan)���ݽ��и��ֲ���
 *
 ***************************************************************************/

#include "velodyneDataStruct.h"

#define SCAN_BUFFER_SIZE		4
#define HALF_SCAN_BUFFER_SIZE	2

VelodyneDataStruct* getScanRaw(void);
VelodyneDataStruct* getScanRawForDraw(void);

#endif
