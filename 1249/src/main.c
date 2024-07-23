/*
 * Copyright (C) 2009 - 2019 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "fat/ff.h"			/* Declarations of FatFs API */
#include "fat/diskio.h"		/* Declarations of device I/O functions */
#include "fat/ffconf.h"
#include "nhc_amba.h"
#include "fifo.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "cmd.h"

extern StructMsg			CurMsg;
extern StructMsgQuery		MsgQuery;
extern uint32_t Stop_flag;
uint8_t sts=0;
extern uint8_t rxflag;

/********************文件系统格式化与挂载有关参数*******************/
static BYTE Buff[4096];  //与格式化空间大小有关
FATFS fs;
FRESULT fr;
FILINFO fno;
FIL file;
FIL wfile;
FIL rfile;
/************************************************************/
int main()
{
		int ret,br=0,bw=0,i=0;
		xil_printf("UCAS Project 1249!\r\n");
#if  1
		init_platform();
		MsgQueryInit();
		SimpleDmaInit();
		XLLFIFO_SysInit();
		DiskInit();
#if     0//format the filesysterm
		ret = f_mkfs(
			"",	/* Logical drive number */
			0,			/* Format option  FM_EXFAT*/
			Buff,			/* Pointer to working buffer (null: use heap memory) */
			sizeof Buff			/* Size of working buffer [byte] */
			);
		if (ret != FR_OK) {
			xil_printf("f_mkfs  Failed! ret=%d\n", ret);
			return 0;
		}
#endif //format the filesysterm

/********* mount filesysterm *********/
#if     1
    	ret = f_mount (&fs, "", 1);
		if (ret != FR_OK)
		{
			xil_printf("f_mount  Failed! ret=%d\n", ret);
			//format the filesysterm
			ret = f_mkfs(
				"",	/* Logical drive number */
				0,			/* Format option  FM_EXFAT*/
				Buff,			/* Pointer to working buffer (null: use heap memory) */
				sizeof Buff			/* Size of working buffer [byte] */
				);
			if (ret != FR_OK) {
				xil_printf("f_mkfs  Failed! ret=%d\n", ret);
				return 0;
			}
		}
		xil_printf(" Init All ok!\r\n");
#endif

	/* receive and process packets */
	while(1)
	{
		if((rxflag==1))
		{
			cmd_parse();
			rxflag=0;
		}
		memset(&CurMsg,0,sizeof(CurMsg));
		GetMessage(&CurMsg);
		switch(CurMsg.HandType)
		{
				case 0XA2:
					xil_printf("%s %d\r\n", __FUNCTION__, __LINE__);
					switch(CurMsg.HandId)
					{
						case 0x1:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_a201(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
								cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
							cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
							xil_printf("------commands executing complete!------\r\n");
						break;

						case 0x2:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_a202(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
								break;
							}
							xil_printf("------commands executing complete!------\r\n");
						break;

						case 0x4:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_a204(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
								cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
							cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
							xil_printf("------commands executing complete!------\r\n");
						break;

						case 0x5:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_a205(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
								cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
							cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
							xil_printf("------commands executing complete!------\r\n");
						break;

						default:
						break;
					}
				break;

				case 0XB2:
					xil_printf("%s %d  CurMsg.HandType:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType);
					xil_printf("------Start executing commands!------\r\n");
					ret=run_cmd_b201(&CurMsg);
					if(ret!=0)
					{
						xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
						cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
						break;
					}
					cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
					xil_printf("------commands executing complete!------\r\n");
				break;

				case 0XD2:
					switch(CurMsg.HandId)
					{
						case 0x1:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_d201(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
								cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
							cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
							xil_printf("------commands executing complete!------\r\n");
						break;

						case 0x2:
//							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
//							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_d202(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
//								cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
//							cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
//							xil_printf("------commands executing complete!------\r\n");
						break;

						case 0x3:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_d203(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
//								    cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
//							    cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
							xil_printf("------commands executing complete!------\r\n");
						break;
						case 0x4:
							xil_printf("%s %d  CurMsg.HandType:0x%x CurMsg.HandId:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType,CurMsg.HandId);
							xil_printf("------Start executing commands!------\r\n");
							ret=run_cmd_d204(&CurMsg);
							if(ret!=0)
							{
								xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
//								cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
								break;
							}
//							    cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
							xil_printf("------commands executing complete!------\r\n");
						break;

						default:
						break;
					}
				break;

				case 0XF2:
					xil_printf("%s %d  CurMsg.HandType:0x%x\r\n", __FUNCTION__, __LINE__,CurMsg.HandType);
					xil_printf("------Start executing commands!------\r\n");
					ret=run_cmd_f201(&CurMsg);
					if(ret!=0)
					{
						xil_printf("------commands executing failed!------ ret=%d\r\n",ret);
						cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x10);
						break;
					}
					cmd_reply_a203(CurMsg.PackNum,CurMsg.HandType,CurMsg.HandId,0x11);
					xil_printf("------commands executing complete!------\r\n");
				break;

				default:
				break;
		}//switch
//		}

	}
#endif
	/* never reached */
	cleanup_platform();

	return 0;
}
