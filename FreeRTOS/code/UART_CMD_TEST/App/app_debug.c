#include "app_debug.h"
#include "bsp_usart.h"
#include "string.h"
#include "bsp_iap.h"
#include <ctype.h>

/*�����*/
const cmd_list_struct cmd_list[]={
/*   ����    ������Ŀ    ������        ������Ϣ                         */   
{"hello",   		0,      printf_hello,   		"hello                          - ��ӡ HelloWorld!\r\n"},
{"arg",     		8,      handle_arg,     		"arg <arg1> <arg2> ...          - ������,��ӡ����Ĳ���\r\n"},
{"help",    		0,      printf_help,    		"help                           - ������ܲ˵�\r\n"},
{"taskinfo",		1,      DebugCmdTaskinfo,    	"taskinfo                       - ��ӡ�����豸��Ϣ arg1(1-6)����6��Ϊ���г���\r\n"},
{"ultprintf",   	1,      DebugCmdUltprintf,    	"ultprintf <arg1>               - ��ӡ�������� arg1(1-6)����6��Ϊ���г���\r\n"},
{"ultmsg",   		1,      DebugCmdUltmsg,    		"ultmsg <arg1>                  - ��ӡ�����豸��Ϣ arg1(1-6)����6��Ϊ���г���\r\n"},
{"mov",   			2,      DebugCmdMov,    		"mov <arg1> <arg2>              - �����˶�,<arg1>Ϊ���� 1ǰ�� 2���� 3��ת 4��ת <arg2>Ϊ�����ٶȵ�λcm/s\r\n"},
{"shutdown",   		0,      DebugCmdshutdown,    	"shutdown                       - ִ�йػ�ָ��\r\n"},
{"reboot",   		0,      DebugCmdReboot,    		"reboot                         - ִ������ָ��\r\n"},
{"ks103chaaddr",	2,      DebugCmdks103chaaddr,   "ks103chaaddr <arg1> <arg2>     - ִ���޸�KS103��ַָ��,<arg1>�ɵ�ַ <arg1>�µ�ַ\r\n"},
{"ks103addrprintf",	1,      DebugCmdks103addrprintf,"ks103addrprintf <arg1>         - ��ӡKS103��ַָ��,<arg1> id�� ȱʡ���ӡ����r\n"},
{"setfirmware",		2,      DebugCmdsetfirmware,	"setfirmware <arg1> <arg2>      - ���ù̼���Ϣ,<arg1>���к� <arg2>�������� r\n"},
{"readfirmware",	0,      DebugCmdreadfirmware,	"readfirmware <arg1> <arg2>     - ��ȡ�̼���Ϣ \r\n"},
{"admin",ADMIN_PASSWORD_LEN,      DebugCmdAdmin,	"admin <arg1> <arg2>     		- ϵͳ����ԱȨ�� \r\n"}
};

cmd_analyze_struct cmd_analyze;
uint8_t adminMode=0;	//0�ǹ���Աģʽ 1����Աģʽ

void DebugCmdArgAnalyze(char *buff, uint16_t len)
{
   int32_t rec_arg_num;
   
    uint32_t rec_num;
    char cmd_buf[CMD_LEN]; 
	u8 i;
	rec_num = get_true_char_stream(cmd_analyze.processed_buf, (const char*)buff);

    
    /*�ӽ�����������ȡ����*/
     for(i=0;i<CMD_LEN;i++)
     {
         if((i>0)&&((cmd_analyze.processed_buf[i]==' ')||(cmd_analyze.processed_buf[i]==0x0D)))
         {
              cmd_buf[i]='\0';        //�ַ���������
              break;
         }
         else
         {
              cmd_buf[i]=cmd_analyze.processed_buf[i];
         }
     }
     
	rec_arg_num = CmdArgNumAnalyze((char*)buff, rec_num);

            
    for(i=0;i<sizeof(cmd_list)/sizeof(cmd_list_struct);i++)
   {
       if(!strcmp(cmd_buf,cmd_list[i].cmd_name))       //�ַ������
       {
            if(rec_arg_num<0 || rec_arg_num>cmd_list[i].max_args)
            {
                AppPrintf("������Ŀ����!\n");
            }
            else
            {
                cmd_list[i].handle(rec_arg_num,(void *)cmd_analyze.cmd_arg);
            }
            break;
       }
       
   }
   if(i>=sizeof(cmd_list)/sizeof(cmd_list_struct))
   {
       AppPrintf("��֧�ֵ�ָ��!\n");
   }
}
/**
* ���������������,�Կո���Ϊһ����������,֧������ʮ��������(��:0x15),֧�����븺��(��-15)
* @param rec_buf   �������������
* @param len       ����������ܳ���
* @return -1:       ������������,����:��������
*/

s32 CmdArgNumAnalyze(char * rec_buf, u16 len)
{
    uint32_t i;
    uint32_t blank_space_flag=0;    //�ո��־
    uint32_t arg_num=0;             //������Ŀ
    uint32_t index[ARG_NUM];        //��Ч�����׸����ֵ���������
    
     /*����һ�����,�ҳ���������Ŀ,�Լ������ε��׸���������rec_buf�����е��±�*/
     for(i=0;i<len;i++)
     {
        if(rec_buf[i]==0x20)        //Ϊ�ո�
        {
            blank_space_flag=1;              
            continue;
        }
         else if(rec_buf[i]==0x0D)   //����
        {
            break;
        }
        else
        {
            if(blank_space_flag==1)
            {
                 blank_space_flag=0; 
                 if(arg_num < ARG_NUM)
                 {
                    index[arg_num]=i;
                     arg_num++;         
                 }
                 else
                 {
                     return -1;      //��������̫��
                 }
            }
        }
     }
    
     for(i=0;i<arg_num;i++)
     {
         cmd_analyze.cmd_arg[i]=string_to_dec((unsigned char *)(rec_buf+index[i]),len-index[i]);
     }
     return arg_num;

}
/*�ַ���ת10/16������*/
int32_t string_to_dec(uint8_t *buf,uint32_t len)
{
   uint32_t i=0;
   uint32_t base=10;       //����
   int32_t  neg=1;         //��ʾ����,1=����
   int32_t  result=0;
   
    if((buf[0]=='0')&&(buf[1]=='x'))
    {
       base=16;
       neg=1;
       i=2;
    }
    else if(buf[0]=='-')
    {
       base=10;
       neg=-1;
       i=1;
    }
    for(;i<len;i++)
    {
       if(buf[i]==0x20 || buf[i]==0x0D)    //Ϊ�ո�
       {
           break;
       }
       
       result *= base;
       if(isdigit(buf[i]))                 //�Ƿ�Ϊ0~9
       {
           result += buf[i]-'0';
       }
       else if(isxdigit(buf[i]))           //�Ƿ�Ϊa~f����A~F
       {
            result+=tolower(buf[i])-87;
       }
       else
       {
           result += buf[i]-'0';
       }                                        
    }
   result *= neg;
   
    return result ;
}
/**
* ʹ��SecureCRT�����շ�����,�ڷ��͵��ַ����п��ܴ��в���Ҫ���ַ��Լ������ַ�,
* �����˸��,�����ƶ����ȵ�,��ʹ�������й��߽����ַ���֮ǰ,��Ҫ����Щ�����ַ���
* �������ַ�ȥ����.
* ֧�ֵĿ����ַ���:
*   ����:1B 5B 41
*   ����:1B 5B 42
*   ����:1B 5B 43
*   ����:1B 5B 44
*   �س�����:0D 0A
*  Backspace:08
*  Delete:7F
*/
uint32_t get_true_char_stream(char *dest,const char *src)
{
   uint32_t dest_count=0;
   uint32_t src_count=0;
   
    while(src[src_count]!=0x0D && src[src_count+1]!=0x0A)
    {
       if(isprint(src[src_count]))
       {
           dest[dest_count++]=src[src_count++];
       }
       else
       {
           switch(src[src_count])
           {
                case    0x08:                          //�˸����ֵ
                {
                    if(dest_count>0)
                    {
                        dest_count --;
                    }
                    src_count ++;
                }break;
                case    0x1B:
                {
                    if(src[src_count+1]==0x5B)
                    {
                        if(src[src_count+2]==0x41 || src[src_count+2]==0x42)
                        {
                            src_count +=3;              //���ƺ����Ƽ���ֵ
                        }
                        else if(src[src_count+2]==0x43)
                        {
                            dest_count++;               //���Ƽ���ֵ
                            src_count+=3;
                        }
                        else if(src[src_count+2]==0x44)
                        {
                            if(dest_count >0)           //���Ƽ���ֵ
                            {
                                dest_count --;
                            }
                           src_count +=3;
                        }
                        else
                        {
                            src_count +=3;
                        }
                    }
                    else
                    {
                        src_count ++;
                    }
                }break;
                default:
                {
                    src_count++;
                }break;
           }
       }
    }
   dest[dest_count++]=src[src_count++];
    dest[dest_count++]=src[src_count++];
    return dest_count;
}

/*��ӡ�ַ���:Hello world!*/
void printf_hello(int32_t argc,void *cmd_arg)
{
   AppPrintf("Hello world!\n");
}
/*��ӡÿ������*/
void handle_arg(int32_t argc,void * cmd_arg)
{
   uint32_t i;
   int32_t  *arg=(int32_t *)cmd_arg;
   
    if(argc==0)
    {
       AppPrintf("�޲���\n");
    }
    else
    {
       for(i=0;i<argc;i++)
       {
           AppPrintf("��%d������:%d\n",i+1,arg[i]);
       }
    }
}
void printf_help(int32_t argc,void * cmd_arg)
{
	uint16_t i;
	
	AppPrintf("\r\n\r\n����      ******             ������Ϣ  **********\r\n");
	for(i=0;i<sizeof(cmd_list)/sizeof(cmd_list_struct);i++)
    {
        AppPrintf(cmd_list[i].help);
    }   
}
void DebugCmdTaskinfo(int32_t argc,void * cmd_arg)
{
	 Show_SYS_INFO_Task();
}
void DebugCmdUltprintf(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdUltmsg(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdMov(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdshutdown(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdReboot(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdks103chaaddr(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdks103addrprintf(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }

}
void DebugCmdsetfirmware(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }
	if(adminMode)
		SetFirmwareInfo(arg[0], arg[1], DEVICE_AUTHOR, DEVICE_EMAIL);
	else
		AppPrintf("[ ERROR ] ������ԱȨ�޿������ù̼���Ϣ\n",i+1,arg[i]);
}

void DebugCmdreadfirmware(int32_t argc,void * cmd_arg)
{
    uint32_t i;
    int32_t  *arg=(int32_t *)cmd_arg;
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }
	Read_Device_info();
}
void DebugCmdAdmin(int32_t argc,void * cmd_arg)
{
    uint32_t i;
	uint8_t pass_cnt = 0;
    int32_t  *arg=(int32_t *)cmd_arg;
	char* password= ADMIN_PASSWORD;
	memcpy(password,ADMIN_PASSWORD,ADMIN_PASSWORD_LEN);
    
     if(argc==0)
     {
        AppPrintf("�޲���\n");
     }
     else
     {
        for(i=0;i<argc;i++)
        {
            AppPrintf("��%d������:%d\n",i+1,arg[i]);
        }
     }
	 if(argc == ADMIN_PASSWORD_LEN)
	 {
		for(i=0;(i<argc)&&(i<ADMIN_PASSWORD_LEN);i++)
		{
			if(password[i]-'0' == arg[i])
				pass_cnt++;
		}
		if(pass_cnt==ADMIN_PASSWORD_LEN)
		{
			AppPrintf("[ OK ] �������Աģʽ\r\n");
			adminMode=1;
		}
		else
		{
			AppPrintf("[ Error ] �������Աģʽʧ�� \r\n");
			adminMode=0;
		}
	 }
	 

}

void Show_SYS_INFO_Task(void)
{
	uint8_t pcWriteBuffer[400];
	AppPrintf("============= START SYSTEM TASK INFO TASK ==========\r\n"); 
	AppPrintf("\r\ntask_name\t\tstate\tprior\tstack\tId\r\n");
	vTaskList((char *)&pcWriteBuffer);
	AppPrintf("%s\r\n", pcWriteBuffer);

	AppPrintf("\r\ntask_name\t\tcnt(10us)\tusage_pec\r\n");
	vTaskGetRunTimeStats((char *)&pcWriteBuffer);
	AppPrintf("pcWriteBuffer len %d\r\n",strlen((char*)pcWriteBuffer));
	AppPrintf("%s\r\n", pcWriteBuffer);
	AppPrintf("============= END SYSTEM TASK INFO TASK ============\r\n"); 

}
