#if 0
memset(pstframeInfo->stVideoAddr.pu8UserVirAddr, 0x68, 
            stFrame.stVFrame.u32Width * stFrame.stVFrame.u32Height);

        Uint8 *v = pstframeInfo->stVideoAddr.pu8UserVirAddr 
                 + stFrame.stVFrame.u32Width * stFrame.stVFrame.u32Height;
        Uint8 *u = v + 1;

        for(Int32 i = 0; i < stFrame.stVFrame.u32Width; i++)
        {
            *v = 0xEC;
            *u = 0xD5;
             v += 2;
             u += 2;
        } 
��һ��640x512��uyvuuyvu��ʽ��ͼ��Y=0x68,v=0xEC, u=0xD5
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int Uint32;
typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef int Int32;
typedef short Int16;

#define VB_INVALID_POOLID (-1U)

int writeYuv422Image(unsigned int image_w, unsigned int image_h)
{
	char *YUVfilename = "UYVY_640_512.yuv";

	int iRet = 0, i = 0;
	unsigned char *u;

	unsigned char pYUVBuff[image_w * image_h * 2 + 1];

	FILE *fp_yuv = fopen(YUVfilename, "wb+");

	u = pYUVBuff;

	for (i = 0; i < image_w * image_h / 2; i++)
	{
		*u++ = 0xD5;
		*u++ = 0x68;
		*u++ = 0xEC;
		*u++ = 0x68;
	}

#if 0
	for(i = 0; i < image_w * image_h * 2; i++)
	{
		if(i % (image_w * 2) == 0)
		{
			printf("\n");
		}
		printf("%2x ", pYUVBuff[i]);
	}
#endif

	printf("\n");

	fseek(fp_yuv, 0, SEEK_SET);
	fwrite(pYUVBuff, 1, image_w * image_h * 2, fp_yuv);
	fflush(fp_yuv);

	fclose(fp_yuv);
	return 0;
}

/************
���� �˻����
********/
int mulOverFlow()
{
	unsigned int uResult = 0;
	unsigned long uResult_1 = 0;

	uResult = 4096 * 4096 * 4096;
	printf(" 4096 * 4096 * 4096 = [%u] \n", uResult);

	uResult_1 = (unsigned long)(4096 * 4096 * 4096);
	printf(" (unsigned long)(4096 * 4096 * 4096) = [%lu] \n", uResult_1);

	uResult_1 = (unsigned long)4096 * 4096 * 4096;
	printf(" (unsigned long)4096 * 4096 * 4096 = [%lu] \n", uResult_1);

	unsigned long long lResult = 0;
	lResult = 4096 * 4096 * 4096;
	printf(" 4096 * 4096 * 4096 = [%llu] \n", lResult);

	unsigned long long llResult = 0;
	llResult = 4096 * 4096 * 4096L;
	printf(" 4096 * 4096 * 4096L = [%llu] \n", llResult);

	return 0;
}

#define MV_WIDTH 218							   /**< ����ͼ��Ļ��ֵ� */
#define IMAGE_WIDTH 384							   /**< ͼ��Ŀ� */
#define IMAGE_HIGHT 288							   /**< ͼ��ĸ� */
#define TEST 1									   /**< �������ԵĲ��Կ��� */
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HIGHT * 2) /**< 422ͼ����ܴ�С */

void printData(unsigned char *buff, unsigned int num)
{
	unsigned int i = 0;
	if (TEST)
	{
		for (i = 0; i < num; i++)
		{
			printf("%x ", buff[i]);
		}
		printf(" \n\n\n");
	}
}

/********************
������ͼ��ϲ���
	����ͼ�����࣬�����Ҳ࣬���������ͼ
	��1+2�� ===�� ��2+1��
	IMAGE_SIZE
*********************/
void moveData()
{
	char *Rawfilename = "555.raw";
	unsigned char srcData[IMAGE_SIZE + 300] = {0};
	unsigned char FData[IMAGE_SIZE + 300] = {0};
	unsigned char BData[IMAGE_SIZE + 300] = {0};
	unsigned char dstData[IMAGE_SIZE + 300] = {0};
	unsigned int iRet = 0, i = 0;
	unsigned char *temp = NULL;

	FILE *fp_src = fopen(Rawfilename, "r");
	FILE *fp_dst = fopen("Image.yuv", "w+");
	FILE *fp = fopen("mydata.yuv", "w+");

	FILE *fp_218 = fopen("Image_218.yuv", "w+");
	// FILE *fp_640_218 = fopen("Image_640_218.yuv", "w+");

	/** ��ȡԴ����*/
	if (TEST)
	{
		memset(&srcData[0], 0xFF, 218);
		memset(&srcData[218], 0x0, 640 - 218);

		memset(&srcData[640 + 0], 0xAA, 218);
		memset(&srcData[640 + 218], 0x1, 640 - 218);

		memset(&srcData[640 * 2 + 0], 0x88, 218);
		memset(&srcData[640 * 2 + 218], 0x2, 640 - 218);

		memset(&srcData[640 * 3 + 0], 0x77, 218);
		memset(&srcData[640 * 3 + 218], 0x3, 640 - 218);
		printData(srcData, IMAGE_SIZE);
	}
	else
	{
		// src
		fread(srcData, IMAGE_SIZE, 1, fp_src);

		fwrite(srcData + 218, IMAGE_SIZE, 1, fp_218);

		printData(srcData, IMAGE_SIZE);

		fclose(fp_218);
		fclose(fp);
	}

	/** ��ȡ F Data ����ǰ������*/
	temp = srcData;
	for (i = 0; i < IMAGE_HIGHT * 2; i++)
	{
		memcpy(&FData[MV_WIDTH * i], temp, MV_WIDTH);
		temp = temp + IMAGE_WIDTH;
	}

	printData(FData, MV_WIDTH * IMAGE_HIGHT * 2);

	/** ��ȡ B Data �����������*/
	temp = srcData;
	for (i = 0; i < IMAGE_HIGHT * 2; i++)
	{
		temp = temp + MV_WIDTH;
		memcpy(&BData[(IMAGE_WIDTH - MV_WIDTH) * i], temp, IMAGE_WIDTH - MV_WIDTH);
		temp = temp + (IMAGE_WIDTH - MV_WIDTH);
	}

	printData(BData, (IMAGE_WIDTH - MV_WIDTH) * IMAGE_HIGHT * 2);

	/** ��ȡ dst Data �ϳ�����*/
	unsigned char *fTemp = FData;
	unsigned char *bTemp = BData;
#if 1
	// �������� ����ͼ--����ȡǰ��ͼ��--����ȡ���ͼ��--���ϳ���ͼ��
	for (i = 0; i < IMAGE_HIGHT * 2; i++)
	{
		memcpy(dstData + i * IMAGE_WIDTH, bTemp, IMAGE_WIDTH - MV_WIDTH);
		bTemp = bTemp + (IMAGE_WIDTH - MV_WIDTH);
		memcpy(dstData + i * IMAGE_WIDTH + (IMAGE_WIDTH - MV_WIDTH), fTemp, MV_WIDTH);
		fTemp = fTemp + MV_WIDTH;
	}

	printData(dstData, IMAGE_SIZE);

	fwrite(dstData, IMAGE_SIZE, 1, fp_dst);

#else
	// ��ԭ�������ƶ� ��22211111--�����о������⣡�����
	for (i = 0; i < IMAGE_HIGHT * 2; i++)
	{
		memcpy(srcData + i * IMAGE_WIDTH, srcData + i * IMAGE_WIDTH + MV_WIDTH, IMAGE_WIDTH - MV_WIDTH);
		memcpy(srcData + i * IMAGE_WIDTH + (IMAGE_WIDTH - MV_WIDTH), fTemp + i * MV_WIDTH, MV_WIDTH);
	}

	printData(srcData, IMAGE_SIZE);

	fwrite(srcData, IMAGE_SIZE, 1, fp_dst);

#endif

	fclose(fp_src);
	fclose(fp_dst);
}

void YVU422spToUYVY()
{
	char *yuvfilename = "Image_yuv422sp_src.yuv";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = IMAGE_WIDTH * IMAGE_HIGHT * 2;

	unsigned char pYuvBuff[IMAGE_SIZE + 10] = {0};
	unsigned char pRawBuff[IMAGE_SIZE + 10] = {0};
	unsigned char *VU = NULL;

	FILE *fp_yuv = fopen(yuvfilename, "r");
	FILE *fp_dst = fopen("Image_uyvy_dst.raw", "w+");

	fread(pYuvBuff, len, 1, fp_yuv);

	VU = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT);
	for (i = 0; i < len / 4; i++)
	{
		pRawBuff[i * 4 + 0] = *(VU + i * 2 + 1);
		pRawBuff[i * 4 + 1] = pYuvBuff[i * 2 + 0];
		pRawBuff[i * 4 + 2] = *(VU + i * 2);
		pRawBuff[i * 4 + 3] = pYuvBuff[i * 2 + 1];
	}

	fwrite(pRawBuff, len, 1, fp_dst);

	fclose(fp_yuv);
	fclose(fp_dst);
}

void YVU422pTo422sp()
{
	char *yuvfilename = "Image_yuv422p_src.yuv";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = IMAGE_WIDTH * IMAGE_HIGHT << 1;

	unsigned char pYuvBuff[IMAGE_SIZE + 10] = {0};
	unsigned char pDstBuff[IMAGE_SIZE + 10] = {0};
	unsigned char *U = NULL;
	unsigned char *V = NULL;

	FILE *fp_yuv = fopen(yuvfilename, "r");
	FILE *fp_dst = fopen("Image_yuv422sp_dst.yuv", "w+");

	fread(pYuvBuff, len, 1, fp_yuv);

	V = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT);
	U = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT + IMAGE_WIDTH * IMAGE_HIGHT >> 1);

	for (i = 0; i < (IMAGE_WIDTH * IMAGE_HIGHT >> 1); i++)
	{
		pDstBuff[2 * i] = *(U + i);
		pDstBuff[2 * i + 1] = *(V + i);
	}

	memcpy(U, pDstBuff, IMAGE_WIDTH * IMAGE_HIGHT);
	fwrite(pYuvBuff, len, 1, fp_dst);

	fclose(fp_yuv);
	fclose(fp_dst);
}

void YVU420pTo422sp()
{
	char *yuvfilename = "Image_yuv420p_src.yuv";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = IMAGE_WIDTH * IMAGE_HIGHT << 1;

	unsigned char pYuvBuff[IMAGE_SIZE + 10] = {0};
	unsigned char pDstBuff[IMAGE_SIZE + 10] = {0};
	unsigned char *U = NULL;
	unsigned char *V = NULL;

	FILE *fp_yuv = fopen(yuvfilename, "r");
	FILE *fp_dst = fopen("pig_Image_.yuv", "w+");

	fread(pYuvBuff, len, 1, fp_yuv);

	V = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT);
	U = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT + IMAGE_WIDTH * IMAGE_HIGHT >> 1);

	for (i = 0; i < (IMAGE_WIDTH * IMAGE_HIGHT >> 1); i++)
	{
		pDstBuff[2 * i] = *(U + i);
		pDstBuff[2 * i + 1] = *(V + i);
	}

	memcpy(U, pDstBuff, IMAGE_WIDTH * IMAGE_HIGHT);
	fwrite(pYuvBuff, len, 1, fp_dst);

	fclose(fp_yuv);
	fclose(fp_dst);
}

void YVU420p_V_U_cahnge()
{
	char *yuvfilename = "Image_yuv420p_src.yuv";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = IMAGE_WIDTH * IMAGE_HIGHT * 3 >> 1;

	unsigned char pYuvBuff[IMAGE_SIZE + 10] = {0};
	unsigned char pRawBuff[IMAGE_SIZE + 10] = {0};
	unsigned char *U = NULL;
	unsigned char *V = NULL;

	FILE *fp_yuv = fopen(yuvfilename, "r");
	FILE *fp_dst = fopen("Image_yuv420p_dst.yuv", "w+");

	fread(pYuvBuff, len, 1, fp_yuv);

	U = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT);
	V = pYuvBuff + (IMAGE_WIDTH * IMAGE_HIGHT + IMAGE_WIDTH * IMAGE_HIGHT >> 2);
	for (i = 0; i < (IMAGE_WIDTH * IMAGE_HIGHT >> 2); i++)
	{
		temp = *(U + i);
		*(U + i) = *(V + i);
		*(V + i) = temp;
	}

	fwrite(pYuvBuff, len, 1, fp_dst);

	fclose(fp_yuv);
	fclose(fp_dst);
}

/********************
���ܣ�����ǰ16bit �� ��16bit����
ͼ�����ؽ���:
	Դ���ݣ�UY0VY1�� ��---��ת�����ݣ�VY1UY0��

Ե�ɣ�
	RAW����16Bit��ʾһ�����أ�����ǰ�����ػ����ᵼ�¾������
*********************/
void swopRawHL16bit()
{
	char *Rawfilename = "555.raw";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = IMAGE_SIZE;

	unsigned char pRawBuff[IMAGE_SIZE + 100] = {0};

	FILE *fp_raw = fopen(Rawfilename, "r");
	FILE *fp_dst = fopen("Image_16bit.raw", "w+");

	fread(pRawBuff, len, 1, fp_raw);

	for (i = 0; i < len / 4; i++)
	{
		temp = pRawBuff[i * 4 + 0];
		pRawBuff[i * 4 + 0] = pRawBuff[i * 4 + 2];
		pRawBuff[i * 4 + 2] = temp;

		temp = pRawBuff[i * 4 + 1];
		pRawBuff[i * 4 + 1] = pRawBuff[i * 4 + 3];
		pRawBuff[i * 4 + 3] = temp;
	}

	fwrite(pRawBuff, len, 1, fp_dst);

	fclose(fp_raw);
	fclose(fp_dst);
}

/********************
���ܣ�����ǰ8bit �� ��8bit����
ͼ�����ؽ���:
	Դ���ݣ�UY0VY1�� ��---��ת�����ݣ�Y0UY1V��

*********************/
void swopRawHL8bit()
{
	char *Rawfilename = "555.raw";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = IMAGE_SIZE;

	unsigned char pRawBuff[IMAGE_SIZE + 100] = {0};

	FILE *fp_raw = fopen(Rawfilename, "r");
	FILE *fp_dst = fopen("Image_8bit.raw", "w+");

	fread(pRawBuff, len, 1, fp_raw);

	for (i = 0; i < len / 2; i++)
	{
		temp = pRawBuff[i * 2 + 0];
		pRawBuff[i * 2 + 0] = pRawBuff[i * 2 + 1];
		pRawBuff[i * 2 + 1] = temp;
	}

	fwrite(pRawBuff, len, 1, fp_dst);

	fclose(fp_raw);
	fclose(fp_dst);
}

/******************
RAW����ת��8bit���ݴ�С
ԭ��
	dst = ( pix / (MaxPix - MinPix) ) * (2^8)
********************/
void rawTo8bit()
{
	char srcfilename[128];
	char dstfilename[128];
	unsigned char pRawBuff[400 * 300 * 2 + 1] = {0};
	unsigned char pDstBuff[400 * 300 * 2 + 1] = {0};
	unsigned int len = 400 * 300 * 2;
	unsigned int i = 0;			 // ͼ��ѭ��
	unsigned int j = 0;			 // ͼ�����ֽ�ѭ��
	unsigned int Max = 0;		 // ͼ���������ֵ
	unsigned int Min = 0xFFFFFF; // ͼ��������Сֵ
	unsigned int value = 0;		 // ��ǰ����ֵ
	unsigned int diffValue = 0;	 // ͼ�������С���ز�
	unsigned int data = 0;		 // ת�����ֵ
	for (i = 0; i < 1; i++)
	{

		memset(srcfilename, 0, strlen(srcfilename));
		memset(dstfilename, 0, strlen(dstfilename));
		memset(pRawBuff, 0, strlen(pRawBuff));
		memset(pDstBuff, 0, strlen(pRawBuff));

		snprintf(srcfilename, 128, "Image_%d_Raw.raw", i);
		snprintf(dstfilename, 128, "Image_%d_8bit.raw", i);

		FILE *fp_src = fopen(srcfilename, "r");
		FILE *fp_dst = fopen(dstfilename, "w+");

		fread(pRawBuff, len, 1, fp_src);

		j = 0;
		Max = 0;
		Min = 0xFFFFFF;

		while (j < len / 2)
		{
			value = pRawBuff[j] + pRawBuff[j + 1] << 8;
			if (Max < value)
			{
				Max = value;
			}
			else if (Min > value)
			{
				Min = value;
			}
			j = j + 2;
		}
		diffValue = Max - Min;

		printf(" diffValue = [%u] ,Max = [%u], Min = [%u] \n", diffValue, Max, Min);

		j = 0;
#if 0
		while(j < len/2)
		{
			value = pRawBuff[j] + pRawBuff[j+1] << 8;
			data = (((unsigned int)((value * 1.0 * 10000 ) / diffValue )) << 8 ) / 10000;
			pDstBuff[j] 	= (unsigned char)data;
			pDstBuff[j + 1] = data >> 8;
			j = j+2;
		}
#else
		while (j < len)
		{
			data = (((unsigned int)((pRawBuff[j] * 1.0 * 100000000) / diffValue)) << 8) / 10000;
			pDstBuff[j] = (unsigned char)data;
			j++;
		}
#endif

		printf("\n");

		for (j = 0; j < 400; j++)
		{
			printf(" %X", pDstBuff[j]);
		}
		printf("\n");

		fwrite(pDstBuff, len, 1, fp_dst);
		fclose(fp_src);
		fclose(fp_dst);
	}
}

void writeFile()
{
	FILE *fp = fopen("file_test", "a+");
	fwrite("a", 1, 2, fp);
	fclose(fp);
}

/************************************* �ַ����������� start*************************/
/**< ����д�������ַ���������ֵ��ֵ����ʱ�ַ������飩*/
char *returnStrError()
{
	char p[] = "hello world!";
	return p;
}

/**< ��ȷд�� �����ַ��������ĵ�ַ��ֵ����ʱָ�룬�Ὣ�����ȴ���ȥ�ٽ������٣�*/
char *returnStrOk()
{
	char *p = "hello world!";
	return p;
}

/*****************************
�����ַ��������洢�븳ֵ

Ŀ�ģ�
	Ϊ��˵���ַ��������Ĵ洢
*******************************/
void charTest()
{
	unsigned char num = 0xFF;
	char *str = NULL;

	printf("charTest param: [0] error, [1] succeed \n");
	scanf("%d", &num);
	switch (num)
	{
	case 0:
		str = returnStrError();
		printf("%s\n", str);
		break;
	case 1:
		str = returnStrOk();
		printf("%s\n", str);
		break;
	case 2:
		do
		{
			char *p;
			p = "hello";
			p[2] = 'A'; // �Ƿ�����
						// char c[30] = "hello";
			// c[2] = 'A'; // ��ȷ����
		} while (0);

	default:
		printf("param error \n");
	}
}

/************************************* �ַ����������� end*************************/

/******************** PC�˻�ȡ����ת��������� start***********/
#define wHeight 1
#define wWidth 20

void pcCodeGetImage(void)
{
	unsigned char PBFFFF[wHeight * wWidth * 2];
	unsigned char pBuffer[wHeight * wWidth * 2];

	int y, u, v, r, g, b;
	int j = 0;
	int i = 0;
	int nv_index = 0, index = 0, rgb_index = 0;
	const int nv_start = wWidth * wHeight;

	printf(" \n");
	for (i = 0; i < wHeight * wWidth * 2; i++)
	{
		pBuffer[i] = i;
		printf(" %#X", pBuffer[i]);
	}
	printf(" \n");

	for (i = 0; i < wHeight; i++)
	{
		for (j = 0; j < wWidth; j++)
		{
			nv_index = i / 2 * wWidth + j - j % 2;

			y = pBuffer[rgb_index];
			v = pBuffer[nv_start + nv_index];
			u = pBuffer[nv_start + nv_index + 1];

			if (j % 2)
			{
				PBFFFF[index++] = y;
			}
			else
			{
				PBFFFF[index++] = u;
				PBFFFF[index++] = y;
				PBFFFF[index++] = v;
			}
			rgb_index++;
		}
	}

	printf(" \n");
	for (i = 0; i < wHeight * wWidth * 2; i++)
	{
		printf(" %#X", PBFFFF[i]);
	}
	printf(" \n");

	printf(" (unsigned int)(-1) = [%d] \n", (unsigned int)VB_INVALID_POOLID);
}

/******************** PC�˻�ȡ����ת��������� end ***********/

/******************** ��׺���ʽ���� start *******************/

#if 0
 double cal(double p1, char op, double p2)
 { 
	switch(op)
	{
		case '+':
			return p1 + p2;
		case '-':
			return p1 - p2; 
		case '*':
			return p1 * p2;
		case '/':
			return p1 / p2;
	}
 }


void evalPf(char* postfix)
{ 
	double stack[80] = {0.0}; 
	char temp[2];
	char token;
	int top = 0, i = 0;
	temp[1] = '\0';
	while(1)
	{
		token = postfix[i];
		switch(token)
		{
			case '\0':
				printf("ԭʽ = %f\n", stack[top]); 
				return;
			case '+': 
			case '-': 
			case '*': 
			case '/': 
				stack[top-1] =cal(stack[top], token, stack[top-1]);
				top--; 
				break;
			default:
				if(top < sizeof(stack) / sizeof(float)){ 
					temp[0] = postfix[i];
					top++;
					stack[top] = atof(temp);
				}
				break;
		}
		i++;
	}
}


void myEvalPf(char* postfix)
{
	double stack[80] = {0.0}; 
	char data[5][10] = NULL;
	char operator[5][10] = NULL;
	int dataGroupNum = 0, opGroupNum = 0;
	int i = 0, j = 0;
	char isOperator = 0;		//��¼��һ�ε��������ͣ������� 1 �� ����0 ,Ĭ��Ϊ0,��ʼΪ����
	int startIndex = 0;
	int endIndex = 0;
	
	char temp = 0;
	char nextData = 0;
	
	int len = strlen(postfix);
	// 12+34+* 
	for( i = 0; i < len; i++)
	{
		temp = postfix[j];	
		if(temp == '+' || temp == '-' || temp == '*' || temp == '/')
		{
			next
			if()
		}
		else
		{
			nextData = postfix[j];	
			if(temp == '+' || temp == '-' || temp == '*' || temp == '/')
		}
	}
	
	/*
	for( i = 0,j = 0; j < len;i++,j++)
	{
		char temp = postfix[j];	
		if(temp == '+' || temp == '-' || temp == '*' || temp == '/')
		{
			if(1 != isOperator)
			{
				data[dataGroupNum][i+1] = '\0';
				dataGroupNum++;
				i = 0;
			}
			else
			{
				operator[opGroupNum][i] = temp;
				isOperator = 1;
			}
		}
		else
		{
			if(0 != isOperator)
			{
				operator[opGroupNum][i+1] = '\0';
				opGroupNum++;				
				i = 0;
			}
			else
			{
				data[dataGroupNum][i] = temp;
				isOperator = 0;
			}
		}
		printf("temp = %c ,i = %d, dataGroupNum = %d, opGroupNum = %d  \n", temp, i , dataGroupNum, opGroupNum);
	}
	*/
	for(i = 0;i < 10;i++)
	{
		printf("%s ", data[i]);
		printf("%s ", operator[i]);
	}
	
}



int eval_Test(){
	char input[80];
	printf("�������׺ʽ��\n\n"); 
	printf("���磺\n��ʽ (1+2)*(3+4) �ĺ�׺ʽΪ��12+34+*  \n\n"); 
	scanf("%s", input); 
	//evalPf(input);
	myEvalPf(input);
	return 0;
}

#endif
/******************** ��׺���ʽ���� end *******************/

/******************** ֱ��ͼ������ֵ���� start *******************/
#define RAW 1
#if RAW
#define THRESHOLD_ARRAY_NUM ((0XFFFF >> 2) + 1) // 16384 //
#else
#define THRESHOLD_ARRAY_NUM 256
#endif
/*******************************************************************************
 * @interface Thermo_getDisplayThresholdValue
 * @brief ��ȡYUVͼ���ǰnum���Ҷ�ֵ����ֵ
 * @param [in] buff 		YUV���ݵ�Y����buff
 * @param [in] buffSize 	buff�Ĵ�С
 * @param [in] num 		��Ҫ���ֵ�ǰnum����
 * @retval >=0  ��ȡ�ֽ�ֵ
 * @retval <0   ��ȡʧ��
 *******************************************************************************/
unsigned int Thermo_getDisplayThresholdValue(Uint16 *buff, Uint32 buffSize, Uint32 num)
{
	Uint32 aVolue[THRESHOLD_ARRAY_NUM] = {0};
	Uint32 i = 0;
	Uint32 displayCount = 0; /**< ��ʾ���� */
	Uint32 sum = 0;
	Uint32 temp = 0;

	if (buff == NULL)
	{
		return -1;
	}

	if ((buffSize <= 0 || num <= 0) || num > buffSize)
	{
		return -1;
	}

	printf("THRESHOLD_ARRAY_NUM = %d \n", THRESHOLD_ARRAY_NUM);
	printf("sizeof(aVolue) = %d \n", sizeof(aVolue) / sizeof(Uint32));

	memset(aVolue, 0, sizeof(aVolue));
	for (i = 0; i < buffSize; i++)
	{
		printf("buff[%d] = %#X \n", buff[i]);
		aVolue[buff[i]] = aVolue[buff[i]] + 1;
	}

	displayCount = num;
#if 0 //---С---����
	for(i = 0; i < THRESHOLD_ARRAY_NUM; i++)
	{
		sum += aVolue[THRESHOLD_ARRAY_NUM - 1 - i];
		if(sum >= displayCount)
		{
			return (THRESHOLD_ARRAY_NUM - 1)-i;
		}
	}
#else //---��--��С
	printf("displayCount = %#X \n", displayCount);

	for (i = THRESHOLD_ARRAY_NUM - 1; i >= 0; i--)
	{
		printf("i = %#X \n", i);
		sum += aVolue[i];
		if (sum >= displayCount)
		{
			return i;
		}
	}
#endif
}

unsigned int getThresholdValue_test(char *str)
{
	Uint8 buff[18] = {0X00, 0x01, 0x00, 0x02, 0x00, 0x03, 0X00, 0x04, 0x00, 0x05, 0x00, 0x06, 0X00, 0x07, 0x00, 0x08, 0xFF, 0x3F};

#ifdef RAW
	Uint32 buffSize = sizeof(buff) / sizeof(Uint16);
#else
	Uint32 buffSize = sizeof(buff) / sizeof(Uint8);
#endif
	printf("buffSize = [%d] \n", buffSize);
	printf("\n Value = [%d] \n", Thermo_getDisplayThresholdValue((Uint16 *)buff, buffSize, atoi(str)));
	return 0;
}

/******************** ֱ��ͼ������ֵ���� end *******************/

/******************** �����㷨���� start ****************************/

#if 0
Uint8 imageReduction1(Uint8 *pSrcImage, Uint32 float kx, float ky)//ԭʼͼ���Լ����ű���
{
	//��ȡ���ͼ��ֱ���
	int nRows = cvRound(srcImage.rows * kx);//cvRound����������ص��ǺͲ�����ӽ�������
	int nCols = cvRound(srcImage.cols * ky);
	Mat resultImage(nRows, nCols, srcImage.type());//����һ�������ͼ��
	for (int i = 0; i < nRows; i++)
	{
		//����ˮƽ���Ӽ�����ԭͼ�е�����
		int x = static_cast<int>((i + 1) / kx + 0.5) - 1;
		for (int j = 0; j < nCols; j++)
		{
			//���ݴ�ֱ���Ӽ�����ԭͼ�е�����
			int y = static_cast<int>((j + 1) / ky + 0.5) - 1;
			resultImage.at<Vec3b>(i, j) = srcImage.at<Vec3b>(x, y);
		}
	}
	return resultImage;
}


Uint8 imageReduction1(Uint8 *pSrcImage, Uint8 *pDstImage, Uint32 w, Uint32 h, float kx, float ky)//
ԭʼͼ���Լ����ű���
{
	//��ȡ���ͼ��ֱ���
	int nRows = (int)(w * kx);//cvRound����������ص��ǺͲ�����ӽ�������
	int nCols = (int)(h * ky);
	
	printf(" nRows[%d]  nRows[%d]!!!!\n", nRows, nCols);
	int i = 0, j = 0;

	if(0)
	{
		printf("\n");
		for (i = 0; i < w; i++)
		{
			printf("%d ", *(pSrcImage+w*19 + i));
		}
		printf("\n");
		
	}
	
	printf("\n");
	for (i = 0; i < nRows; i++)
	{
		//����ˮƽ���Ӽ�����ԭͼ�е�����
		int x = (int)((i + 1) / kx + 0.5) - 1;
		//printf("---------  x = [%d] \n", x);
		for (j = 0; j < nCols; j++)
		{
			//���ݴ�ֱ���Ӽ�����ԭͼ�е�����
			int y = (int)((j + 1) / ky + 0.5) - 1;
			//printf("y = [%3d], [%d], [%d]  \n", y ,*(pDstImage + (nRows * i) + j ), *(pSrcImage + (w * x) + y));
			*(pDstImage + (nRows * j) + i ) = *(pSrcImage + (w * y) + x);
		}
	}
	return 0;
}
#endif

// ԭʼͼ���Լ����ű���
Uint8 imageReduction1(Uint8 *pSrcImage, Uint8 *pDstImage, Uint32 w, Uint32 h, float kx, float ky)
{
	// ��ȡ���ͼ��ֱ���
	int nRows = (int)(w * kx); // cvRound����������ص��ǺͲ�����ӽ�������
	int nCols = (int)(h * ky);

	printf(" nRows[%d]  nRows[%d]!!!!\n", nRows, nCols);
	int i = 0, j = 0;

	if (0)
	{
		printf("\n");
		for (i = 0; i < w; i++)
		{
			printf("%d ", *(pSrcImage + w * 19 + i));
		}
		printf("\n");
	}

	printf("\n");

	for (i = 0; i < nRows; i++)
	{
		// ����ˮƽ���Ӽ�����ԭͼ�е�����
		int x = ((int)((i + 1) / kx + 0.5) - 1) / 2 * 2;
		// printf("---------  x = [%d] \n", x);
		for (j = 0; j < nCols; j++)
		{
			// ���ݴ�ֱ���Ӽ�����ԭͼ�е�����
			int y = (int)((j + 1) / ky + 0.5) - 1;
			// printf("y = [%3d], [%d], [%d]  \n", y ,*(pDstImage + (nRows * i) + j ), *(pSrcImage + (w * x) + y));
			*(pDstImage + (nRows * 2 * j) + i * 2) = *(pSrcImage + (w * 2 * y) + x * 2);
			*(pDstImage + (nRows * 2 * j) + i * 2 + 1) = *(pSrcImage + (w * 2 * y) + x * 2 + 1);
		}
	}

	return 0;
}

#if 0
Vec3b areaAverage(Mat &srcImage, Point_<int> leftPoint, Point_<int> rightPoint)
{
 
	int temp1 = 0, temp2 = 0, temp3 = 0;//��������������е�ÿ��ͨ�����صĺ�
	//����������е����ص�ĸ���
	int nPix = (rightPoint.x - leftPoint.x + 1)*(rightPoint.y - leftPoint.y + 1);
	//���������ӿ��еĸ���ͨ�������غ�
	for (int i = leftPoint.x; i <= rightPoint.x; i++)
	{
		for (int j = leftPoint.y; j <= rightPoint.y; j++)
		{
		   temp1 += srcImage.at<Vec3b>(i, j)[0];//���������е����̺�ͨ�������غ�
			temp2 += srcImage.at<Vec3b>(i, j)[1];
			temp3 += srcImage.at<Vec3b>(i, j)[2];
		}
	}
	//��������е�ÿ��ͨ����ƽ��ֵ
	Vec3b vecTemp;
	vecTemp[0] = temp1 / nPix;
	vecTemp[1] = temp2 / nPix;
	vecTemp[2] = temp3 / nPix;
	return vecTemp;
}

Mat imageReduction2(Mat &srcImage, float kx, float ky)
{
	//��ȡ���ͼ��ֱ���
	int nRows = cvRound(srcImage.rows * kx);//cvRound����������ص��ǺͲ�����ӽ�������
	int nCols = cvRound(srcImage.cols * ky);
	Mat resultImage(nRows, nCols, srcImage.type());//����һ�������ͼ��
	//�����ӿ�����Ͻ���������
	int leftRowcoordinate = 0;
	int leftColcoordinate = 0;
	for (int i = 0; i < nRows; i++)
	{
		//����ˮƽ���Ӽ�����ԭͼ�е�����
		int x = static_cast<int>((i + 1) / kx + 0.5) - 1;
		for (int j = 0; j < nCols; j++)
		{
			//���ݴ�ֱ���Ӽ�����ԭͼ�е�����
			int y = static_cast<int>((j + 1) / ky + 0.5) - 1;
			//�������ӿ�ľ�ֵ
			resultImage.at<Vec3b>(i, j) = areaAverage(srcImage, Point_<int>(leftRowcoordinate, leftColcoordinate), Point_<int>(
x, y));
			//�������ӿ����Ͻǵ������꣬�����겻��
			leftColcoordinate = y + 1;
		}
		//һ��ѭ��������½�����������
		leftColcoordinate = 0;
		//�������ӿ����Ͻǵ�������	
		leftRowcoordinate = x + 1;
	}
	return resultImage;
}

#endif

typedef struct
{
	Uint32 x;
	Uint32 y;
} Point;

#if 1
Uint32 areaAverage(Uint16 *srcImage, Uint32 w, Uint32 h, Point leftPoint, Point rightPoint)
{
	int i = 0, j = 0;
	int temp = 0; // ��������������е�ÿ�����صĺ�

	// ����������е����ص�ĸ���
	int nPix = (rightPoint.x - leftPoint.x + 1) * (rightPoint.y - leftPoint.y + 1);
	// ���������ӿ��еĸ���ͨ�������غ�
	for (i = leftPoint.x; i <= rightPoint.x; i++)
	{
		for (j = leftPoint.y; j <= rightPoint.y; j++)
		{
			temp += *(srcImage + j * w + i); // ���������е����̺�ͨ�������غ�
		}
	}

	return (temp / nPix);
}

Uint8 imageReduction2(Uint16 *srcImage, Uint32 srcW, Uint32 srcH, Uint16 *pDstImage, float kx, float ky)
{

	// ��ȡ���ͼ��ֱ���
	int nRows = (int)(srcW * kx); // cvRound����������ص��ǺͲ�����ӽ�������
	int nCols = (int)(srcH * ky);

	// �����ӿ�����Ͻ���������
	int leftRowcoordinate = 0;
	int leftColcoordinate = 0;
	int i = 0, j = 0;
	for (i = 0; i < nRows; i++)
	{
		// ����ˮƽ���Ӽ�����ԭͼ�е�����
		int x = (Uint32)((i + 1) / kx + 0.5) - 1;
		for (j = 0; j < nCols; j++)
		{
			// ���ݴ�ֱ���Ӽ�����ԭͼ�е�����
			int y = (Uint32)((j + 1) / ky + 0.5) - 1;
			// �������ӿ�ľ�ֵ
			Point startPoint = {leftRowcoordinate, leftColcoordinate};
			Point endPoint = {x, y};
			*(pDstImage + nRows * j + i) = areaAverage(srcImage, srcW, srcH, startPoint, endPoint);
			// �������ӿ����Ͻǵ������꣬�����겻��
			leftColcoordinate = y + 1;
		}
		// һ��ѭ��������½�����������
		leftColcoordinate = 0;
		// �������ӿ����Ͻǵ�������
		leftRowcoordinate = x + 1;
	}
	return 0;
}
#endif

#if 0
Uint32 areaAverage(Uint8 *srcImage, Uint32 w, Uint32 h, Point leftPoint, Point rightPoint)
{
	int i = 0, j = 0;
	int temp = 0;//��������������е�ÿ�����صĺ�
	
	//����������е����ص�ĸ���
	int nPix = (rightPoint.x - leftPoint.x + 1*2 )*(rightPoint.y - leftPoint.y + 1);
	//���������ӿ��еĸ���ͨ�������غ�
	for (i = leftPoint.x; i <= rightPoint.x; i = i+2)
	{
		for (j = leftPoint.y; j <= rightPoint.y; j++)
		{
		    temp += *(srcImage + j*w*2 + i);//���������е����̺�ͨ�������غ�
			temp += (*(srcImage + j*w*2 + i+1)) << 8;//���������е����̺�ͨ�������غ�
		}
	}

	return (temp / nPix);
}


Uint8 imageReduction2(Uint16 *srcImage, Uint32 srcW, Uint32 srcH, Uint16 *pDstImage, float kx, float ky)
{
	printf("\n  111111111 \n");
	//��ȡ���ͼ��ֱ���
	int nRows = (int)(srcW * kx);//cvRound����������ص��ǺͲ�����ӽ�������
	int nCols = (int)(srcH * ky);

	//�����ӿ�����Ͻ���������
	int leftRowcoordinate = 0;
	int leftColcoordinate = 0;
	
	int i = 0, j = 0;
	for (i = 0; i < nRows; i++)
	{

		int x = ((Uint32)((i + 1) / kx + 0.5) - 1) / 2 * 2;	//����ˮƽ���Ӽ�����ԭͼ�е�����
		for (j = 0; j < nCols; j++)
		{
			int y = ((Uint32)((j + 1) / ky + 0.5) - 1);	//���ݴ�ֱ���Ӽ�����ԭͼ�е�����
			
			printf(" start[%d][%d],i[%d], j[%d], x[%d], y[%d]  \n", leftRowcoordinate, leftColcoordinate, i, j, x, y);
			//�������ӿ�ľ�ֵ
			Point startPoint = {leftRowcoordinate, leftColcoordinate};
			Point endPoint = {x, y};
			Uint32 data = areaAverage(srcImage, srcW, srcH, startPoint, endPoint);
			*(pDstImage + nRows*2*j + i*2) = data & 0XFF;
			*(pDstImage + nRows*2*j + i*2 + 1) = data >> 8;
			
			leftColcoordinate = y + 1;//�������ӿ����Ͻǵ������꣬�����겻��
		}
	
		leftColcoordinate = 0;		//һ��ѭ��������½�����������
		leftRowcoordinate = x * 2 + 1*2;	//�������ӿ����Ͻǵ�������	
	}
	return 0;
}
#endif

#define ZOOM_DEBUG 0
void test_zoomImage()
{
	Uint32 Image_w = 256;
	Uint32 Image_h = 192;
	float Percent_w = 0.75;
	float Percent_h = 0.75;
	Uint32 Image_len = Image_w * Image_h * 2;

#if ZOOM_DEBUG
	FILE *fp_src = fopen("Image_src.raw", "w+");
#else
	FILE *fp_src = fopen("Image_src.raw", "r+");
#endif

	FILE *fp_dst = fopen("Image_dst.raw", "w+");

	Uint8 *pSrcBuff = NULL;
	Uint8 *pDstBuff = NULL;

	if (ZOOM_DEBUG)
	{
		pSrcBuff = (Uint8 *)(malloc(Image_w * Image_h + 100));
	}
	else
	{
		pSrcBuff = (Uint8 *)(malloc(Image_w * Image_h * 2 + 100));
	}
	if (pSrcBuff == NULL)
	{
		printf("malloc failed !!!!\n");
		return;
	}

	if (ZOOM_DEBUG)
	{
		Uint32 i = 0;
		for (i = 0; i < Image_h; i++)
		{
			if (((i / 20) % 2) == 0)
			{
				memset(pSrcBuff + i * Image_w, 0, Image_w);
			}
			else
			{
				memset(pSrcBuff + i * Image_w, 255, Image_w);
			}
		}

		for (i = 0; i < Image_h * Image_w; i = i + 2)
		{
			*(pSrcBuff + i) = i;
			*(pSrcBuff + i + 1) = 0x0F;
		}

		printf("\n");
		for (i = 0; i < Image_h * Image_w; i++)
			printf("%X ", *(pSrcBuff + i));
		printf("\n");

		printf("\n");
		unsigned short *pData = (unsigned short *)(pSrcBuff);
		for (i = 0; i < Image_h * Image_w / 2; i++)
			printf("%X ", *(pData + i));
		printf("\n");

		printf("\n");
		Uint8 *pbuff = (Uint8 *)(pData);
		for (i = 0; i < Image_h * Image_w; i++)
			printf("%X ", *(pbuff + i));
		printf("\n");

		fwrite(pSrcBuff, (Image_w * Image_h), 1, fp_src);
		close(fp_src);
	}
	else
	{
		fread(pSrcBuff, Image_len, 1, fp_src);
	}

	if (ZOOM_DEBUG)
	{
		pDstBuff = (Uint8 *)(malloc((Uint32)(Image_w * Percent_w * Image_h * Percent_h + 100)));
	}
	else
	{
		pDstBuff = (Uint8 *)(malloc((Uint32)(Image_w * Percent_w * Image_h * Percent_h * 2 + 100)));
	}

	if (pDstBuff == NULL)
	{
		free(pSrcBuff);
		printf("malloc failed !!!!\n");
		return;
	}

	// imageReduction1((Uint8 *)pSrcBuff, (Uint8 *)pDstBuff, Image_w, Image_h, Percent_w, Percent_h);
	imageReduction2((Uint16 *)pSrcBuff, Image_w, Image_h, (Uint16 *)pDstBuff, Percent_w, Percent_h);

	if (ZOOM_DEBUG)
	{
		fwrite(pDstBuff, (Image_w * Percent_w * Image_h * Percent_h), 1, fp_dst);
	}
	else
	{
		fwrite(pDstBuff, (Image_w * Percent_w * Image_h * Percent_h) * 2, 1, fp_dst);
	}

	fclose(fp_src);
	fclose(fp_dst);
	free(pSrcBuff);
	free(pDstBuff);
}

/******************** �����㷨���� end  ****************************/

/*************************** ���ļ����� start************************************/

void read_file()
{
	Uint32 status = 0;
	static Uint32 bmpSize = 0;
	Uint8 *g_pBmpData = NULL;

	g_pBmpData = (Uint8 *)malloc(64 * 1024);
	if (g_pBmpData == NULL)
	{
		printf(" bmp malloc failed!\n");
		return;
	}

	printf("---- aaaa !\n");

	/* open file */
	FILE *fp_src = fopen("./pip.bmp", "r+");
	if (fp_src == NULL)
	{
		free(g_pBmpData);
		g_pBmpData = NULL;
		printf("open file failed!\n");
		return;
	}

	printf("---- sssssssssss !\n");

	FILE *fp_dst = fopen("pip_my.bmp", "w+");
	if (fp_dst == NULL)
	{
		free(g_pBmpData);
		g_pBmpData = NULL;
		printf("open file failed!\n");
		return;
	}

	printf("---- fffffffffffff !\n");

	status = 0;
	do
	{
		bmpSize += status;
		status = fread((g_pBmpData + bmpSize), 1, 512, fp_src);

		printf(" prencent num = [%d] \n", status);
	} while (status > 0);

	printf("---- dddddddddddd !\n");

	printf("bmpSize = [%d] \n", bmpSize);

	fwrite(g_pBmpData, bmpSize, 1, fp_dst);

	fclose(fp_src);
	fclose(fp_dst);
}

/*************************** ���ļ����� end***** *******************************/
int *pData = NULL;
void fun1(int **p)
{
	int *p1 = NULL;
	p1 = *p;
	printf(" *p1 = [%d] , data = [%d]\n", *p1, *pData);
}

void fun2(int **p)
{
	printf(" data = [%d] \n", *pData);
	*(*p) = 200;
	printf(" data = [%d] \n", *pData);
}

/***************************/

#define SAMPLE_ARRAY_MAX ((0XFFFF >> 2) + 1) // ���ݣ������������ֵ���ã������±�����
#define RAW_DATA_SIZE (6553600)				 // �ļ�������
Uint32 sampleValue[SAMPLE_ARRAY_MAX] = {0};	 // ֱ��ͼ���飬��¼ÿ��ֵ���ֵĴ���

static Int32 print_Histogram(Uint16 *buff, Uint32 sampleSize)
{
	Uint32 i = 0;
	Uint32 sum = 0;
	Uint32 Num = 0;

	FILE *fp_dst = fopen("HistogramData.txt", "w+");
	if (NULL == buff)
	{
		printf("error NULL == buff \n");
		return -1;
	}

	if (sampleSize <= 0)
	{
		printf("error sampleSize <= 0 \n");
		return -1;
	}

	memset(sampleValue, 0, SAMPLE_ARRAY_MAX);
	for (i = 0; i < sampleSize; i++)
	{
		sampleValue[buff[i]] = sampleValue[buff[i]] + 1;
	}

	if (0)
	{
		for (i = 0; i < sampleSize; i++)
		{
			fprintf(fp_dst, "%5d ,  %d \n", i, buff[i]);
		}
	}
	else
	{
		sampleValue[99] = 1;
		for (i = 0; i < SAMPLE_ARRAY_MAX; i++)
		{
			if (Num < i / 100)
			{
				fprintf(fp_dst, "%5d ,  %d \n", i, sum);
				Num = i / 100;
				sum = 0;
			}
			else
			{
				sum += sampleValue[i];
			}
		}
	}

	close(fp_dst);
	return 0;
}

int print_RawDataHistogram(char *str)
{
	FILE *fp_src = fopen(str, "r");

	Uint8 *pSrcBuff = NULL;

	pSrcBuff = (Uint8 *)(malloc(RAW_DATA_SIZE));
	if (pSrcBuff == NULL)
	{
		return -1;
	}

	fread(pSrcBuff, RAW_DATA_SIZE, 1, fp_src);
	close(fp_src);

	printf("%#X, %#X, %#X,", pSrcBuff[6553597], pSrcBuff[6553598], pSrcBuff[6553599]);
	print_Histogram((Uint16 *)(pSrcBuff), RAW_DATA_SIZE / 2);

	free(pSrcBuff);

	return 0;
}

/***************************/

int RGB2YUV(int R, int G, int B)
{
	float Y = (77 * R + 150 * G + 29 * B) * 1.0 / 256;

	float U = ((-44 * R - 87 * G + 131 * B) * 1.0 / 256) + 128;

	float V = ((131 * R - 110 * G - 21 * B) * 1.0 / 256) + 128;

	printf("RGB [%d, %d, %d] to YUV [%2f, %2f, %2f] \n", R, G, B, Y, U, V);
	return 0;
}

int YUV2RGB(int Y, int U, int V)
{
	float R = Y + ((360 * (V - 128)) * 1.0 / 256);
	float G = Y - (((88 * (U - 128) + 184 * (V - 128))) * 1.0 / 256);
	float B = Y + ((455 * (U - 128)) * 1.0 / 256);

	printf("YUV [%d, %d, %d] to RGB [%2f, %2f, %2f] \n", Y, U, V, R, G, B);

	return 0;
}

typedef struct
{
	char path[128]; /**< �洢Raw����·�� */
} VI_RAW_CAPTURE_INFO_S;

typedef struct Rect
{
	int left;
	int top;
	int right;
	int bottom;
} Rect;

typedef struct
{
	int Num;	  /**< ����� */
	int Update;	  /**< ����״̬ */
	Rect Rect[4]; /**< ������Ϣ */
} RectInfo_S;

void fun123(void)
{
	char *yuvfilename = "555.yuv";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = 384 * 288 * 2;

	unsigned char pYuvBuff[384 * 288 * 2] = {0};
	unsigned char pDstBuff[384 * 288 * 2] = {0};
	unsigned char *VU = NULL;

	FILE *fp_yuv = fopen(yuvfilename, "r");
	FILE *fp_dst = fopen("Image_raw.yuv", "w+");

	fread(pYuvBuff, len, 1, fp_yuv);
	memset(pYuvBuff + (len >> 1), 128, len >> 1);

	fwrite(pYuvBuff, len, 1, fp_dst);

	fclose(fp_yuv);
	fclose(fp_dst);
}

void YUV422P_To_YUV422SP(void)
{
	char *yuvfilename = "yuv422p.yuv";

	unsigned int iRet = 0, i = 0;
	unsigned char temp;
	unsigned int len = 384 * 288 * 2;

	unsigned char pYuvBuff[384 * 288 * 2] = {0};
	unsigned char pDstBuff[384 * 288 * 2] = {0};

	unsigned char *U = NULL;
	unsigned char *V = NULL;
	unsigned char *dstUV = NULL;

	FILE *fp_yuv = fopen(yuvfilename, "r");
	FILE *fp_dst = fopen("Image_raw.yuv", "w+");

	fread(pYuvBuff, len, 1, fp_yuv);

	// Y
	memcpy(pDstBuff, pYuvBuff, (len >> 1));

	// UV
	V = pYuvBuff + (len >> 1);
	U = pYuvBuff + (len >> 2);
	dstUV = pDstBuff + (len >> 1);

	for (i = 0; i < (len >> 1); i++)
	{
		*(dstUV + i * 2) = *(U + i);
		*(dstUV + i * 2 + 1) = *(V + i);
	}

	fwrite(pDstBuff, len, 1, fp_dst);

	fclose(fp_yuv);
	fclose(fp_dst);
}

void colorBar_8bit(int width, int hight)
{

	char *yuvfilename = "Image_color_8bit.yuv";

	unsigned int iRet = 0, i = 0, j = 0;
	unsigned int len = width * hight * 2;

	unsigned char *pYuvBuff = NULL;

	pYuvBuff = (unsigned char *)malloc(len + 100);

	FILE *fp_yuv = fopen(yuvfilename, "w+");

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < hight; j++)
		{
			if (i < (width / 8))
			{
				pYuvBuff[i + j * width] = 255 / 8;
				continue;
			}
			if (i < ((width / 8) * 2))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 2;
				continue;
			}
			if (i < ((width / 8) * 3))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 3;
				continue;
			}
			if (i < ((width / 8) * 4))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 4;
				continue;
			}
			if (i < ((width / 8) * 5))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 5;
				continue;
			}
			if (i < ((width / 8) * 6))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 6;
				continue;
			}
			if (i < ((width / 8) * 7))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 7;
				continue;
			}
			if (i < width)
			{
				pYuvBuff[i + j * width] = 255;
				continue;
			}
		}
	}

	memset(pYuvBuff + width * hight, 0x80, (len - width * hight));

	fwrite(pYuvBuff, len, 1, fp_yuv);

	fclose(fp_yuv);
}

void colorBar_16bit(int width, int hight)
{

	char *yuvfilename = "Image_color_16bit.yuv";

	unsigned int iRet = 0, i = 0, j = 0;
	unsigned int pixWidth = (width >> 1);

	unsigned int Bytelen = width * hight * 3 >> 1;
	unsigned int Pixlen = pixWidth * hight * 3 >> 1;

	unsigned char *pYuvBuff = NULL;
	Uint16 *pPixYuvBuff = NULL;

	pYuvBuff = (unsigned char *)malloc(Bytelen + 100);
	pPixYuvBuff = (Uint16 *)pYuvBuff;

	FILE *fp_yuv = fopen(yuvfilename, "w+");

	for (i = 0; i < pixWidth; i++)
	{
		for (j = 0; j < hight; j++)
		{
			if (i < (pixWidth / 8))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8;
				continue;
			}
			if (i < ((pixWidth / 8) * 2))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8 * 2;
				continue;
			}
			if (i < ((pixWidth / 8) * 3))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8 * 3;
				continue;
			}
			if (i < ((pixWidth / 8) * 4))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8 * 4;
				continue;
			}
			if (i < ((pixWidth / 8) * 5))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8 * 5;
				continue;
			}
			if (i < ((pixWidth / 8) * 6))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8 * 6;
				continue;
			}
			if (i < ((pixWidth / 8) * 7))
			{
				pPixYuvBuff[i + j * pixWidth] = 255 / 8 * 7;
				continue;
			}
			if (i < pixWidth)
			{
				pPixYuvBuff[i + j * pixWidth] = 255;
				continue;
			}
		}
	}

	memset(pYuvBuff + width * hight, 128, (Bytelen - width * hight));

	fwrite(pYuvBuff, Bytelen, 1, fp_yuv);

	fclose(fp_yuv);
}

void colorBar_debug(int width, int hight)
{

	char *yuvfilename = "Image_color_8bit.yuv";

	unsigned int iRet = 0, i = 0, j = 0;
	unsigned int len = width * hight * 2;

	unsigned char *pYuvBuff = NULL;

	pYuvBuff = (unsigned char *)malloc(len + 100);

	FILE *fp_yuv = fopen(yuvfilename, "w+");

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < hight; j++)
		{
			if (i < (width / 8))
			{
				pYuvBuff[i + j * width] = 255 / 8;
				continue;
			}
			if (i < ((width / 8) * 2))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 2;
				continue;
			}
			if (i < ((width / 8) * 3))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 3;
				continue;
			}
			if (i < ((width / 8) * 4))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 4;
				continue;
			}
			if (i < ((width / 8) * 5))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 5;
				continue;
			}
			if (i < ((width / 8) * 6))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 6;
				continue;
			}
			if (i < ((width / 8) * 7))
			{
				pYuvBuff[i + j * width] = 255 / 8 * 7;
				continue;
			}
			if (i < width)
			{
				pYuvBuff[i + j * width] = 255;
				continue;
			}
		}
	}

	for (i = 0; i < (len - width * hight); i++)
	{
		*(pYuvBuff + width * hight + i) = i / 10;
	}

	fwrite(pYuvBuff, len, 1, fp_yuv);

	fclose(fp_yuv);
}

void Frame_to_Frames(void)
{

	Uint8 num = 0;
	Uint8 Frames = 25;
	unsigned char pSrcBuff[1280 * 1280 * 2] = {0};
	Uint32 SrcLen = 1280 * 960 * 3 >> 1;

	FILE *fp_src = NULL;
	FILE *fp_dst = NULL;
	char dstfilename[128];
	char srcfilename[128];

	snprintf(dstfilename, 128, "./image/Image_Yuv_%d_fps.yuv", Frames);
	fp_dst = fopen(dstfilename, "w+");

	for (num = 0; num < Frames; num++)
	{
		snprintf(srcfilename, 128, "./image/Image_%d_Yuv.yuv", num);
		fp_src = fopen(srcfilename, "r");

		fread(pSrcBuff, SrcLen, 1, fp_src);

		fwrite(pSrcBuff, SrcLen, 1, fp_dst);

		fclose(fp_src);
	}

	fclose(fp_dst);
	return 0;
}

void YUV_Split_Y(int num, char *argv[])
{
	char *yuvfilename = NULL;
	char yfilename[128];
	char uvfilename[128];

	unsigned int len = 0, i = 0;

	unsigned int Width = 640;
	unsigned int hight = 512;

	unsigned char pYBuff[1280 * 1280 * 2] = {0};

	FILE *fp_yuv = NULL;
	FILE *fp_yDst = NULL;
	FILE *fp_uvDst = NULL;

	for (i = 1; i < num; i++)
	{
		yuvfilename = argv[i];
		printf(" File naem : %s \n", yuvfilename);
		len = Width * hight * 2;

		snprintf(yfilename, 128, "Y_%d_%d_%s", Width, hight, yuvfilename);
		snprintf(uvfilename, 128, "UV_%d_%d_%s", Width, hight, yuvfilename);
		fp_yuv = fopen(yuvfilename, "r");
		fp_yDst = fopen(yfilename, "w+");
		fp_uvDst = fopen(uvfilename, "w+");

		fread(pYBuff, len, 1, fp_yuv);

		fwrite(pYBuff, len / 2, 1, fp_yDst);

		fwrite(pYBuff + len / 2, len / 2, 1, fp_uvDst);

		fclose(fp_yuv);
		fclose(fp_yDst);
		fclose(fp_uvDst);
	}

	return 0;
}

void test_function_1(void)
{
	int i = 0;
	int sum = 0;
	for (i = 0; i < 100; i++)
	{
		if ((i % 2 == 0) && (i % 3 == 0))
		{
			printf("num = %d \n", i);
			sum += i;
		}
	}
	printf("sum = %d \n", sum);
}

void test_function_2(void)
{
	unsigned long long data[10] = {0};
	int num[10] = {0};
	int count = 0, i = 0, j = 0, n[10] = {0};
	unsigned long long dst = 0;
	int temp = 0;
	char input[200];

	scanf("%d", &count);

	for (j = 0; j < count; j++)
	{
		scanf("%u, %d", &data[j], n[j]);
		scanf("%d", &n[j]);

		printf("data[%d] = %u, n = %d\n", i, data[j], n[j]);
	}

	printf("count = %d \n", count);

	for (j = 0; j < count; j++)
	{
		memset(num, 0, 10);
		dst = 0;

		// printf("data[%d] = %u, n = %d\n", i, data[j], n[j]);

		do
		{
			num[(data[j] % 10)]++;
			data[j] = data[j] / 10;
		} while (data[j]);

		// printf("22222222222222 \n");

		for (i = 0; i < 10; i++)
		{
			if (n[j] != 0)
			{
				if (num[i] >= n[j])
				{
					num[i] = num[i] - n[j];
					n[j] = 0;
				}
				else
				{
					n[j] = n[j] - num[i];
					num[i] = 0;
				}
			}
		}

		// printf("3333333333333 \n");

		for (i = 9; i >= 0; i--)
		{
			while (num[i])
			{
				dst = dst * 10 + i;
				num[i]--;
			}
		}

		// printf("444444444444444 \n");

		if (1)
		{
			for (i = 0; i < 10; i++)
			{
				printf("%d ", num[i]);
			}
		}

		printf("dst = %d \n", dst);
	}
}

void test_function_2_1(void)
{
	char a[100] = {0};
	int nums = 0, i = 0, n[10] = {0}, j = 0, k = 0;
	int len = 0;
	scanf("%d", &nums); // �������������

	char *p = (char *)malloc(100 * nums);
	char *q = p;

	for (i = 0; i < nums; i++)
	{
		scanf("%s", a);
		scanf("%d", &n[i]);

		len = strlen(a);
		// printf("%d\n",len);
		char tmp = 0;
		for (j = 0; j < len; j++)
		{
			for (k = j; k < len; k++)
			{
				if (a[j] < a[k])
				{
					tmp = a[j];
					a[j] = a[k];
					a[k] = tmp;
				}
			}
		}

		// printf("%s\n", a[i]);
		// ɾ����nλ
		a[len - n[i]] = '\0';
		memcpy(p, a, 100);
		p = p + 100;
	}

	for (i = 0; i < nums; i++)
	{
		puts(q);
		q = q + 100;
	}
	return 0;
}

int arr[] = {1, 2, 3};
int Number = 0;

void swap_1(int a, int b) // a,b��ʾ�����±�
{
	int temp;

	temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

void Fullsort(int n)
{

	if (n == (Number)) // ���
	{
		int i;

		for (i = 0; i <= (Number - 1); i++)
			printf("%d ", arr[i]);

		printf("\n");

		return;
	}

	int i;

	for (i = n; i <= (Number - 1); i++) // ��n��λ�õ����ֱ������4-n��λ�õ���(��������)����
	{

		swap_1(n, i);

		Fullsort(n + 1);

		swap_1(n, i); // ������
	}
}

void test_function_3(void)
{
	int i;
	scanf("%d", &Number);
	for (i = 0; i < Number; arr[i] = i + 1, i++)

		Fullsort(0);
	return 0;
}

void test_1(void)
{
	char a[100] = {0};
	int nums = 0, i = 0, j = 0, k = 0;

	scanf("%d", &nums);

	char *p = (char *)malloc(100 * nums);
	char *q = p;

	for (i = 0; i < nums; i++)
	{
		scanf("%s", a);
		int len = strlen(a);

		char tmp = 0;
		for (j = 0; j < len; j++)
		{
			for (k = j; k < len; k++)
			{
				if (a[j] > a[k])
				{
					tmp = a[j];
					a[j] = a[k];
					a[k] = tmp;
				}
			}
		}

		// printf("%s\n", a[i]);
		memcpy(p, a, 100);
		p = p + 100;
	}

	for (i = 0; i < nums; i++)
	{
		p = q;
		while ((*p) != '\0')
		{
			printf("%c ", *p);
			p++;
		}
		printf("\n");
		q = q + 100;
	}

	return 0;
}

int main_test()
{
	char a[100] = {0};
	int nums = 0, i = 0, n[10] = {0}, j = 0, k = 0;
	scanf("%d", &nums); // �������������

	char *p = (char *)malloc(100 * nums);
	char *q = p;
	for (i = 0; i < nums; i++)
	{

		scanf("%s", a);
		scanf("%d", &n[i]);

		int len = strlen(a);
		// printf("%d\n",len);
		char tmp = 0;
		for (j = 0; j < len; j++)
		{
			for (k = j; k < len; k++)
			{
				if (a[j] < a[k])
				{
					tmp = a[j];
					a[j] = a[k];
					a[k] = tmp;
				}
			}
		}
		// printf("%s\n", a[i]);
		// ɾ����nλ
		a[len - n[i]] = '\0';
		memcpy(p, a, 100);
		p = p + 100;
	}

	for (i = 0; i < nums; i++)
	{
		puts(q);
		q = q + 100;
	}
	return 0;
}

int test_3()
{
	int len = 0, num = 0;
	int a[20] = {0};
	int aTmep[20] = {0};
	int i = 0, j = 0, k = 0, n = 0;

	scanf("%d", &len);

	scanf("%d", &num);

	printf("%d , %d\n", len, num);

	for (i = 0; i < len; i++)
	{
		arr[i] = i + 1;
	}

	for (n = 0; n < len; n++)
	{
		printf("%d ", arr[n]);
	}
	printf("\n################\n");

	for (i = 0; i < len - num; i++)
	{
		for (j = i + 1; j < len; j++)
		{
			for (k = j + 1; k < len; k++)
				swap_1(j, k); // ������
			for (n = 0; n < num; n++)
			{
				printf("%d ", arr[n]);
			}
			printf("\n");
			swap_1(j, k); // ������
		}
	}
}

int test_4()
{
	char str[100] = {0};
	char dst[100] = {0};
	char temp;
	int len = 0, i = 0;

	scanf("%s", str);

	len = strlen(str);

	for (i = 0; i < len; i++)
	{
		printf("111111111 %c \n", str[i]);
		temp = str[i];
		if (temp >= 'a' && temp <= 'z')
		{
			printf("2222222 %c \n", str[i]);

			if (temp == 'a' || temp == 'b' || temp == 'c')
			{
				printf("333333333 %c \n", str[i]);
				dst[i] = '2';
				printf("444444444 %c \n", dst[i]);
			}
			else if (temp == 'd' || temp == 'e' || temp == 'f')
			{
				dst[i] = '3';
			}
			else if (temp == 'g' && temp == 'u' || temp == 'i')
			{
				dst[i] = '4';
			}
			else if (temp == 'j' || temp == 'k' || temp == 'l')
			{
				dst[i] = '5';
			}
			else if (temp == 'm' || temp == 'n' || temp == 'o')
			{
				dst[i] = '6';
			}
			else if (temp == 'p' || temp == 'q' || temp == 'r' || temp == 's')
			{
				dst[i] = '7';
			}
			else if (temp == 't' || temp == 'u' || temp == 'v')
			{
				dst[i] = '8';
			}
			else
			{
				dst[i] = '9';
			}
		}
		else if (temp >= 'A' && temp <= 'Z')
		{
			if (temp == 'Z')
			{
				dst[i] = 'a';
				// printf("444444444 %c \n", dst[i]);
			}
			else
			{

				dst[i] = temp + 32 + 1;
				// printf("444444444 %c \n", dst[i]);
			}
		}
		else
		{
			dst[i] = temp;
		}
	}

	dst[len] = '\0';

	printf("###### %s\n", dst);
}

int test_5()
{
	char str[100] = {0};
	char dst[100] = {0};
	char temp;
	int len = 0, i = 0;

	scanf("%s", str);

	len = strlen(str);
	printf("len %d", len);

	for (i = 0; i < len; i++)
	{
		temp = str[i];
		if (temp >= 'a' && temp <= 'z')
		{
			if (temp == 'a' || temp == 'b' || temp == 'c')
			{
				dst[i] = '2';
			}
			else if (temp == 'd' || temp == 'e' || temp == 'f')
			{
				dst[i] = '3';
			}
			else if (temp == 'g' && temp == 'h' || temp == 'i')
			{
				dst[i] = '4';
			}
			else if (temp == 'j' || temp == 'k' || temp == 'l')
			{
				dst[i] = '5';
			}
			else if (temp == 'm' || temp == 'n' || temp == 'o')
			{
				dst[i] = '6';
			}
			else if (temp == 'p' || temp == 'q' || temp == 'r' || temp == 's')
			{
				dst[i] = '7';
			}
			else if (temp == 't' || temp == 'u' || temp == 'v')
			{
				dst[i] = '8';
			}
			else if (temp == 'w' || temp == 'x' || temp == 'y' || temp == 'z')
			{
				dst[i] = '9';
			}
		}
		else if (temp >= 'A' && temp <= 'Z')
		{
			if (temp == 'Z')
			{
				dst[i] = 'a';
			}
			else
			{

				dst[i] = temp + 32 + 1;
			}
		}
		else
		{
			dst[i] = temp;
		}
	}

	dst[len] = '\0';

	printf("%s\n", dst);
}

// ��������Ԫ�ص�λ��
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// �ݹ�����ȫ����
void generatePermutations(int arr[], int start, int end)
{
	int i = 0;
	if (start == end)
	{
		for (i = 0; i <= end; i++)
		{
			printf("%d ", arr[i]);
		}
		printf("\n");
	}
	else
	{
		for (i = start; i <= end; i++)
		{
			swap(&arr[start], &arr[i]);
			generatePermutations(arr, start + 1, end);
			swap(&arr[start], &arr[i]); // �ָ�ԭʼ˳���Ա����������������
		}
	}
}

int test_6(void)
{
	int n, i;
	printf("Enter the number of elements: ");
	scanf("%d", &n);

	int arr[n];
	printf("Enter the elements: ");
	for (i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
	}

	printf("All permutations:\n");
	generatePermutations(arr, 0, n - 1);

	return 0;
}

void main(int argc, char *argv[])
{
	test_6();
	return 0;
	int R = atoi(argv[2]);
	int G = atoi(argv[3]);
	int B = atoi(argv[4]);

	int Y = atoi(argv[2]);
	int U = atoi(argv[3]);
	int V = atoi(argv[4]);
	if (atoi(argv[1]))
	{
		YUV2RGB(Y, U, V);
	}
	else
	{
		RGB2YUV(R, G, B);
	}

	return 0;
#if 0
	short N1 = 4096;
	Uint32 N2 = 1280;
	short	c = 0;
	Uint8 *data = NULL;
	Uint8 addr[3] = {0};
	Uint8 vir[3]  ={1, 2, 6};
	Uint8 a1 = 0;
	
	data = (Uint8 *)malloc(atoi(argv[1]));
	printf(" data len [%d],[%d] [%d] \n", sizeof(data), atoi(argv[1]), malloc_usable_size(data));
	c = N1*N2 ;/// 8192;
	
	memcpy(addr, vir, sizeof(addr));
	printf(" c = [%d] sizeof[%d], {%d, %d, %d}\n", c, sizeof(addr), addr[0], addr[1], addr[2]);
	colorBar_debug(400,300);

return;
	printf(" argc = [%d], argv[0] = %s, argv[1] = %s  \n", argc, argv[0], argv[1]);
#endif

#if 0
	Rect aRectInfo[4][2] = {{50, 50, 100, 100}, {100, 100, 150, 150}};
	RectInfo_S RectInfo;
	memcpy(&(RectInfo.Rect[0]), aRectInfo, sizeof(aRectInfo));
	printf(" RectInfo_S = [%d] [%d]  [%d] \n", sizeof(RectInfo_S), sizeof(Rect),  sizeof(aRectInfo));
	printf(" [%d] [%d]  \n", sizeof(RectInfo.Rect), sizeof(RectInfo));
	
	YUV422P_To_YUV422SP();
	printf(" ################ OK !!\n");
	return 0;
#endif

	/** д��YUV422���ɫ���� */
	// writeYuv422Image(640, 512);

	/** ���� �˻���� */
	// mulOverFlow();

	/** ����ǰ8bit �� ��8bit���� */
	// swopRawHL8bit();

	/** ����ǰ16bit �� ��16bit���� */
	// swopRawHL16bit();

	/** RAW���ݹ�һ��8bit���ݴ�С */
	// rawTo8bit();

	/** YVU422תRAW(UYVY)����*/
	// YVU422spToUYVY();

	/** YVU420P����UV����˳��*/
	// YVU420p_V_U_cahnge();

	/** YVU422תYUV422sp */
	// YVU422sp_u_v_change();

	/** YVU422PתYUV422sp */
	// YVU422pTo422sp();

	/** YVU420sPתYUV420p */
	// YVU420pTo422sp();

	/** ����colorBar */
	// colorBar_8bit(720,576);
	// colorBar_16bit(32, 10);

	/** YUV�з���Y����*/
	YUV_Split_Y(argc, argv);

	/** ��֡�ļ��ϲ�һ���ļ�*/
	// Frame_to_Frames();

	return 0;
	// writeFile();

	/** ����ͼ��ϲ� */
	// moveData();

	/** �ַ����������� */
	// charTest();

	/** ��׺���ʽ **/
	// eval_Test();

	// creatImageData(buff);
	// ����ֱ��ͼͳ����ֵ
	// getThresholdValue_test(argv[1]);

	// ��ӡֱ��ͼ����
	// print_RawDataHistogram(argv[1]);

	/** �����㷨 */
	// test_zoomImage();

	/*****  ******/
#if 0
#if 0
	int R = atoi(argv[1]);
	int G = atoi(argv[2]);
	int B = atoi(argv[3]);
	
	RGB2YUV(R, G, B);
#else
	int Y = atoi(argv[1]);
	int U = atoi(argv[2]);
	int V = atoi(argv[3]);
	
	YUV2RGB(Y, U, V);

#endif
#endif
	VI_RAW_CAPTURE_INFO_S name = {0};
	// name.path = "/mnt/sd/mmcblk0p0/raw_con.raw";
	snprintf((&name)->path, 128, "/mnt/sd/mmcblk0p0/raw_con_lyp.raw"); /**< Ĭ��·�� */

	printf("------ %s \n", name.path);

	printf("--------**************************----------------------- \n");
	int a = 100;
	pData = &a;
	printf("-------- 1111111 \n");
	// goto my_error;
	printf("-------- 22222222 \n");
	fun2(&pData);
	fun1(&pData);

	printf("-------- read_file S \n");
	read_file();
	printf("-------- read_file E \n");

my_error:
{
	printf("-------- 333333333 \n");
	printf("-------- 44444444 \n");
	printf("-------- 55555 \n");
}
	printf("-------- 6666666 sizeof(void*) = [%d]\n", sizeof(void *));
}
