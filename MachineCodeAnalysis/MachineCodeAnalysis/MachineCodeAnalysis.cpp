// MachineCodeAnalysis.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long LONG;

//���ָ��
char *AssemblyCode(FILE *file,BYTE operationCode,char *chAssemblyCode) {

	switch (operationCode)
	{
		case 0x89:
		{

			//�ȶ�ȡһ���ֽڵ�����
			BYTE tagTemp;
			fread(&tagTemp, sizeof(tagTemp), 1, file);

			switch (tagTemp)
			{
				case 0xD6:   //MOV  SI, DX
				{
					sprintf(chAssemblyCode, "%s SI,DX", chAssemblyCode);
				}
				break;
				case 0xF3:   //MOV  BX, SI
				{
					sprintf(chAssemblyCode, "%s BX,SI", chAssemblyCode);
				}
				break;
				case 0xFE:   //MOV   SI, DI
				{
					sprintf(chAssemblyCode, "%s SI,DI", chAssemblyCode);
				}
				break;
				case 0x57:   //MOV [BX + XX], DX
				{
					BYTE assemblyCodeTemp;
					fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

					sprintf(chAssemblyCode, "%s [BX + %x],DX", chAssemblyCode, assemblyCodeTemp);
				}
				break;
				case 0x56:   //MOV [BP + XX], DX
				{
					BYTE assemblyCodeTemp;
					fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

					sprintf(chAssemblyCode, "%s [BP + %x],DX", chAssemblyCode, assemblyCodeTemp);
				}
				break;
				case 0x46:   //MOV [BP + XX], AX
				{
					BYTE assemblyCodeTemp;
					fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

					sprintf(chAssemblyCode, "%s [BP + %x],AX", chAssemblyCode, assemblyCodeTemp);
				}
				break;
				default:
					break;
			}

			
		}
		break;
		case 0x88:
		{
			//��������Ϊ0x88ʱ,�����ȡһ���ֽڵ�����
			BYTE assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);
			switch (assemblyCodeTemp)
			{
			case 0xC1:   //MOV  CL, AL
			{
				sprintf(chAssemblyCode, "%s CL,AL", chAssemblyCode);
			}
			break;
			default:
				break;
			}
		}
		break;
		case 0x30:
		{
			//��������Ϊ0x30ʱ,�����ȡһ���ֽڵ�����
			BYTE assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);
			switch (assemblyCodeTemp)
			{
			case 0xED:   //XOR  CH, CH
			{
				sprintf(chAssemblyCode, "%s CH,CH", chAssemblyCode);
			}
			break;
			default:
				break;
			}
		}
		break;
		case 0x83:
		{
			//��������Ϊ0x83ʱ,�����һ���ֽڵ�����
			//������ֽ�Ϊ F9 ,�� �������һ���ֽ�    CMPָ��
			//������ֽ�Ϊ 7F ,�� ������������ֽ�    CMPָ��
			//������ֽ�Ϊ C3 , �� +0B ��ʱ��         ADD ָ��

			BYTE tagTemp;
			fread(&tagTemp, sizeof(tagTemp), 1, file);

			switch (tagTemp)
			{
				case 0xF9:    //CMP  CX, XXX
				{
					BYTE operationTemp;
					fread(&operationTemp, sizeof(operationTemp), 1, file);

					if (operationTemp == '\0') {
					
						sprintf(chAssemblyCode, "CMP CX,00", operationTemp);
					}
					else {
					
						sprintf(chAssemblyCode, "CMP CX,%s", operationTemp);
					}
				}
				break;
				case 0x7F:    //CMP WORD PTR [BX + XXX],XXX
				{
					WORD assemblyCodeTemp;
					fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

					BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
					BYTE backByte = (assemblyCodeTemp & 0x00FF);

					sprintf(chAssemblyCode, "CMP WORD PTR [BX + %x],%x", backByte, frontByte);
				}
				break;
				case 0xC3:    //83C30B        ADD     BX, +0B
				{
					BYTE assemblyCodeTemp;
					fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

					sprintf(chAssemblyCode, "ADD BX,%x", assemblyCodeTemp);
				}
				break;
				default:
					break;
			}
		}
		break;
		case 0xBF:
		{
			//��������Ϊ0xBFʱ,�����ȡ�����ֽڵ�����
			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			//С�ѵ���
			sprintf(chAssemblyCode, "%s DI,%x%x", chAssemblyCode, frontByte , backByte);
		}
		break;
		case 0xB9:
		{
			//��������Ϊ0xB9ʱ,�����ȡ�����ֽڵ�����
			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			//С�ѵ���
			sprintf(chAssemblyCode, "%s ,%x%x", chAssemblyCode, frontByte , backByte);
		}
		break;
		case 0xF6:
		{
			//��������Ϊ0xF6ʱ,�����ȡ�����ֽڵ�����
			//��һ���ֽ��ж�Ϊ BX BP SI DI �е���һ��
			//�ڶ����ֽڸ��������ֽ�Ϊ���бȽϵ�������

			BYTE segTemp;
			fread(&segTemp, sizeof(segTemp), 1, file);

			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			switch (segTemp)
			{
				case 0x47:  //0x47 ��ʾ BX
				{
					//С�ѵ���
					sprintf(chAssemblyCode, "%s BYTE PTR [BX + %x],%x", chAssemblyCode, backByte , frontByte);
				}
				break;
				case 0x46:  //0x46 ��ʾ BP
				{
					//С�ѵ���
					sprintf(chAssemblyCode, "%s BYTE PTR [BP + %x],%x", chAssemblyCode, backByte , frontByte);
				}
				break;
				default:
					break;
			}
		}
		break;
		case 0x31:
		{
			//��������Ϊ0x31ʱ,�����ȡһ���ֽڵ�����
			BYTE assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);
			switch (assemblyCodeTemp)
			{
				case 0xC0:   //XOR     AX, AX
				{
					sprintf(chAssemblyCode, "%s AX,AX", chAssemblyCode);
				}
				break;
				case 0xDB:   //XOR     BX, BX
				{
					sprintf(chAssemblyCode, "%s BX,BX", chAssemblyCode);
				}
				break;
				default:
					break;
			}
		}
		break;
		case 0x8C:
		{
			//��������Ϊ0x8Cʱ,�����ȡ�����ֽڵ�����

			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			switch (backByte)
			{
				case 0x4E:
				{
					sprintf(chAssemblyCode, "%s [BP + %x],CS", chAssemblyCode, backByte);
				}
				break;
				case 0x4F:
				{
					sprintf(chAssemblyCode, "%s [BX + %x],CS", chAssemblyCode, backByte);
				}
				break;
				default:
					break;
			}
		}
		break;
		case 0x8B:      
		{

			//��������Ϊ0x8Bʱ,�����ȡ�����ֽڵ�����
			//��һ���ֽ��ж�Ϊ ͨ�üĴ��� �е���һ��
			//�ڶ����ֽڸ��������ֽ�Ϊ������

			BYTE regTemp;
			fread(&regTemp, sizeof(regTemp), 1, file);

			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			switch (regTemp)
			{
				case 0x16:  //0x16 ��ʾ DX
				{
					//С�ѵ���
					sprintf(chAssemblyCode, "%s DX , [%x%x]", chAssemblyCode, frontByte , backByte);
				}
				break;
				case 0x1E:  //0x1E ��ʾ BX
				{
					//С�ѵ���
					sprintf(chAssemblyCode, "%s DX , [%x%x]", chAssemblyCode, frontByte , backByte);
				}
				break;
				default:
					break;
			}
		}
		break;
		case 0x8A:
		{

			//��������Ϊ0x8Bʱ,�����ȡ�����ֽڵ�����
			//��һ���ֽ��ж�Ϊ ͨ�üĴ��� �е���һ��
			//�ڶ����ֽڸ��������ֽ�Ϊ������

			BYTE regTemp;
			fread(&regTemp, sizeof(regTemp), 1, file);

			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			switch (regTemp)
			{
				case 0x16:  //0x16 ��ʾ DL
				{
					//С�ѵ���
					sprintf(chAssemblyCode, "%s DL , [%x%x]", chAssemblyCode, frontByte , backByte);
				}
				break;
				case 0x36:  //0x36 ��ʾ DH
				{
					//С�ѵ���
					sprintf(chAssemblyCode, "%s DH , [%x%x]", chAssemblyCode, frontByte , backByte);
				}
				break;
				default:
					break;
			}
		}
		break;
		case 0xC6:    
		{

			//��������Ϊ0x8Bʱ,�����ȡ�����ֽڵ�����
			//��һ���ֽ��ж� �ֳ�
			//�ڶ����ֽڸ��������ֽڼ�����Ϊƫ����
			//��� �ֳ� Ϊ BYTE
				//��ȡ���ĸ��ֽ�
		    //��� �ֳ� Ϊ WORD
				//��ȡ���ĸ�������ֽ�

			BYTE regTemp;
			fread(&regTemp, sizeof(regTemp), 1, file);

			WORD assemblyCodeTemp;
			fread(&assemblyCodeTemp, sizeof(assemblyCodeTemp), 1, file);

			BYTE frontByte = (assemblyCodeTemp & 0xFF00) >> 8;
			BYTE backByte = (assemblyCodeTemp & 0x00FF);

			switch (regTemp)
			{
				case 0x06:  //0x16 ��ʾ   MOV  BYTE PTR[XXXX], XX
				{
					BYTE valueTemp;
					fread(&valueTemp, sizeof(valueTemp), 1, file);

					//С�ѵ���
					sprintf(chAssemblyCode, "%s BYTE PTR[%x%x], %x", chAssemblyCode, frontByte , backByte, valueTemp);
				}
				break;
				default:
					break;
			}
		}
		break;
	default:
		break;
	}

	return chAssemblyCode;
}

//������
void operationCode(FILE *file,BYTE &operationCode) {

	//��ȡ1���ֽڵĲ�����
	BYTE operationCodeTemp;
	fread(&operationCode, sizeof(operationCode), 1, file);
	operationCodeTemp = operationCode;

	//BYTE frontByte = (machineCode & 0xFF00)>>8;  //D6
	//BYTE backByte = (machineCode & 0x00FF);  //89

	//ƴ�ӳɵĵ������ָ��
	char chAssemblyCodeTemp[256] = { 0 };

	switch (operationCodeTemp)
	{
		case 0x89:  //mov ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");

			AssemblyCode(file, operationCodeTemp,chAssemblyCodeTemp);
		}
		break;
		case 0xAD:  //LODS ��ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "LODSW");
		}
		break;
		case 0x50:  //PUSH ��ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "PUSH AX");
		}
		break;
		case 0xAC:  //LODS ��ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "LODSB");
		}
		break;
		case 0x88:  //MOV ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");
			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x30:  //XOR ��ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "XOR");
			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x58:  //POP AX
		{
			sprintf(chAssemblyCodeTemp, "%s", "POP AX");
		}
		break;
		case 0x83:  //CMP �� ADD ��ָ��
		{

			// CMP �� ADD
			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0xBF:  //MOVSX 
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x57:  //PUSH DI
		{
			sprintf(chAssemblyCodeTemp, "%s", "PUSH DI");
		}
		break;
		case 0x51:  //PUSH CX
		{
			sprintf(chAssemblyCodeTemp, "%s", "PUSH CX");
		}
		break;
		case 0xB9:  //MOV CX
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV CX");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0xF3:  //REPZ
		{
			sprintf(chAssemblyCodeTemp, "%s", "REPZ");
		}
		break;
		case 0xA4:  //MOVSB
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOVSB");
		}
		break;
		case 0xF6:  //TEST �Ƚϵ��ڴ��СΪ BYTE
		{
			sprintf(chAssemblyCodeTemp, "%s", "TEST");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x31:  //XOR ��ָ��
		{
			sprintf(chAssemblyCodeTemp, "%s", "XOR");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0xAB:  //STOSW
		{
			sprintf(chAssemblyCodeTemp, "%s", "STOSW");
		}
		break;
		case 0xAA:  //STOSB
		{
			sprintf(chAssemblyCodeTemp, "%s", "STOSB");
		}
		break;
		case 0x5F:  //POP DI
		{
			sprintf(chAssemblyCodeTemp, "%s", "POP DI");
		}
		break;
		case 0x8C:  //MOV �����
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x59:  //POP  CX
		{
			sprintf(chAssemblyCodeTemp, "%s", "POP CX");
		}
		break;
		case 0x8B:  //MOV �����
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x8A:  //MOV �����
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0xC6:  //MOV �����
		{
			sprintf(chAssemblyCodeTemp, "%s", "MOV");

			AssemblyCode(file, operationCodeTemp, chAssemblyCodeTemp);
		}
		break;
		case 0x90:  //NOP
		{
			sprintf(chAssemblyCodeTemp, "%s", "NOP");
		}
		break;
		case 0xC3:  //RET
		{
			sprintf(chAssemblyCodeTemp, "%s", "RET");
		}
		break;
		default:
			break;
	}

	printf("%s\r\n", chAssemblyCodeTemp);

}

int main()
{
	FILE *file;

	if ((file = fopen("TEST.COM", "rb")) == NULL)
	{
		printf("�򿪻������ļ�ʧ��!\n");
		return 0;
	}

	//ָ���ļ��ײ�
	fseek(file, 0, SEEK_SET);

	while (true)
	{
		BYTE operationCodeTemp = {0};

		operationCode(file, operationCodeTemp);

		if ((int)operationCodeTemp == 0xC3) {
		
			break;
		}
	}

	//�رմ򿪵��ļ�
	fclose(file);

	system("pause");

    return 0;
}

