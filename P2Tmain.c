#include "T2P.h"
#include <stdlib.h>
#include <string.h>

char TCodeArray[MAX_CODE_COUNT][256];
int main()
{
	TCode TCodeList[MAX_CODE_COUNT]; // 三地址码列表
	PCode PCodeList[MAX_CODE_COUNT]; // P-代码列表
	TCode TCodeList1[MAX_CODE_COUNT];

	// 将三地址码列表和P-代码列表的内容清空
	memset(TCodeList, 0, sizeof(TCodeList));
	memset(PCodeList, 0, sizeof(PCodeList));
	memset(TCodeList1, 0, sizeof(TCodeList1));
	//
	// 初始化三地址码列表
	//
#ifdef CODECODE_CI
	InitTCodeList_CI(TCodeList); // 此行代码在线上流水线运行
#else
	InitTCodeList(TCodeList); // 此行代码在 CP Lab 中运行
#endif

	//
	// 将三地址码转换为P-代码
	//
	T2P(TCodeList, PCodeList);
	
	//
	// 输出P-代码
	//
	P2T(PCodeList,TCodeList1);
	OutputResult1(TCodeList1);
	//OutputResult(PCodeList);

	return 0;
}

/*
功能：
	将三地址码转换为 P-代码。
	
参数：
	TCodeList -- 三地址码列表指针。
	PCodeList -- P-代码列表指针。
*/
void T2P(TCode *TCodeList, PCode *PCodeList)
{
	int TIndex = 0; // 三地址码列表游标
	int PIndex = 0; // P-代码列表游标

	for (; TCodeList[TIndex].Kind != 0; TIndex++)
	{
		switch (TCodeList[TIndex].Kind)
		{
		case t_rd:
			// 产生一条 lda 指令
			PCodeList[PIndex].Kind = p_lda;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			PIndex++;

			// 产生一条 rdi 指令
			PCodeList[PIndex].Kind = p_rdi;
			PIndex++;

			break;

		case t_gt:
			//生成lda代码
			PCodeList[PIndex].Kind = p_lda;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr3.Name);
			PIndex++;
			//地址1
			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				//ldc指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				//lod指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;
			//地址2
			if (TCodeList[TIndex].Addr2.Kind == intconst)
			{
				//ldc指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr2.Value;
			}
			else
			{
				//lod指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr2.Name);
			}
			PIndex++;
			//grt指令
			PCodeList[PIndex++].Kind = p_grt;
			//sto指令
			PCodeList[PIndex++].Kind = p_sto;
			break;

		case t_if_f:

			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				// 产生一条 ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				// 产生一条 lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;

			// 产生一条 fjp 指令
			PCodeList[PIndex].Kind = p_fjp;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr2.Name);
			PIndex++;

			break;

		case t_asn:

			// 产生一条 lda 指令
			PCodeList[PIndex].Kind = p_lda;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr2.Name);
			PIndex++;

			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				// 产生一条 ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				// 产生一条 lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;

			// 产生一条 sto 指令
			PCodeList[PIndex].Kind = p_sto;
			PIndex++;

			break;

		case t_lab:

			//lab指令
			PCodeList[PIndex].Kind = p_lab;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			PIndex++;
			break;

		case t_mul:

			// 产生一条 lda 指令
			PCodeList[PIndex].Kind = p_lda;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr3.Name);
			PIndex++;

			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				// 产生一条 ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				// 产生一条 lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;

			if (TCodeList[TIndex].Addr2.Kind == intconst)
			{
				// 产生一条 ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr2.Value;
			}
			else
			{
				// 产生一条 lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr2.Name);
			}
			PIndex++;

			// 产生一条 mpi 指令
			PCodeList[PIndex].Kind = p_mpi;
			PIndex++;

			// 产生一条 sto 指令
			PCodeList[PIndex].Kind = p_sto;
			PIndex++;

			break;

		case t_sub:
			// sub 指令
			PCodeList[PIndex].Kind = p_lda;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr3.Name);
			PIndex++;
			//地址1
			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				// ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				//lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;
			//地址2
			if (TCodeList[TIndex].Addr2.Kind == intconst)
			{
				//ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr2.Value;
			}
			else
			{
				// lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr2.Name);
			}
			PIndex++;

			// sbi 指令
			PCodeList[PIndex++].Kind = p_sbi;
			// sto 指令
			PCodeList[PIndex++].Kind = p_sto;
			break;

		case t_eq:

			// lda 指令
			PCodeList[PIndex].Kind = p_lda;
			PCodeList[PIndex].Addr.Kind = string;
			strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr3.Name);
			PIndex++;
			//地址1
			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				// ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				// lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;
			//地址2
			if (TCodeList[TIndex].Addr2.Kind == intconst)
			{
				// ldc 指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr2.Value;
			}
			else
			{
				// lod 指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr2.Name);
			}
			PIndex++;

			// equ 指令
			PCodeList[PIndex++].Kind = p_equ;
			// sto 指令
			PCodeList[PIndex++].Kind = p_sto;
			break;

		case t_wri:

			if (TCodeList[TIndex].Addr1.Kind == intconst)
			{
				//ldc指令
				PCodeList[PIndex].Kind = p_ldc;
				PCodeList[PIndex].Addr.Kind = intconst;
				PCodeList[PIndex].Addr.Value = TCodeList[TIndex].Addr1.Value;
			}
			else
			{
				//lod指令
				PCodeList[PIndex].Kind = p_lod;
				PCodeList[PIndex].Addr.Kind = string;
				strcpy(PCodeList[PIndex].Addr.Name, TCodeList[TIndex].Addr1.Name);
			}
			PIndex++;
			//wri指令
			PCodeList[PIndex++].Kind = p_wri;

			break;

		case t_halt:

			PCodeList[PIndex++].Kind = p_stp;
			break;
		}
	}
}





// p-代码转换为三地址码
void P2T(PCode* PCodeList, TCode* TCodeList)
{
    int PIndex = 0;  // P-代码列表游标
    int TIndex = 0;  // 三地址码列表游标

    for( ; PCodeList[PIndex].Kind != 0; )
    {
        switch (PCodeList[PIndex].Kind)
        {
        case p_rdi:         //  整形或字符串
            // 处理 rdi 指令

			//printf("p_rdi");
            TCodeList[TIndex].Kind = t_rd;
			TCodeList[TIndex].Addr1.Kind = string;
            strcpy(TCodeList[TIndex].Addr1.Name, PCodeList[PIndex - 1].Addr.Name);
            TIndex++;
			PIndex++;
            break;

        case p_grt:
            // 处理 grt 指令
			//   三个地址  从

			//printf("p_grt");
            TCodeList[TIndex].Kind = t_gt;
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr2.Kind = intconst;
				TCodeList[TIndex].Addr2.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr2.Kind = string;
				strcpy(TCodeList[TIndex].Addr2.Name,PCodeList[PIndex-1].Addr.Name);
			}
			if(PCodeList[PIndex - 2].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-2].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-2].Addr.Name);
			}
			TCodeList[TIndex].Addr3.Kind = string;
			strcpy(TCodeList[TIndex].Addr3.Name, PCodeList[PIndex-3].Addr.Name);

            TIndex++;
			PIndex+=2;
            break;

        case p_fjp:
            // 处理 fjp 指令

			//printf("p_fjp");
            TCodeList[TIndex].Kind = t_if_f;
			TCodeList[TIndex].Addr2.Kind = string;
			strcpy(TCodeList[TIndex].Addr2.Name,PCodeList[PIndex].Addr.Name);
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-1].Addr.Name);
			}
			PIndex++;
            TIndex++;
        
            break;
		case p_sto:
			//t_asn命令
			//printf("p_sto");
			TCodeList[TIndex].Kind = t_asn;
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-1].Addr.Name);
			}
			TCodeList[TIndex].Addr2.Kind = string;
			strcpy(TCodeList[TIndex].Addr2.Name,PCodeList[PIndex-2].Addr.Name);
			TIndex++;
			PIndex++;
			break;
        case p_lab:
            // 处理 lab 指令

			//printf("p_lab");
            TCodeList[TIndex].Kind = t_lab;
			TCodeList[TIndex].Addr1.Kind = string;
            strcpy(TCodeList[TIndex].Addr1.Name, PCodeList[PIndex].Addr.Name);
            TIndex++;
            PIndex++;
            break;

        case p_mpi:
            // 处理 mpi 指令

			//printf("p_mpi");
            TCodeList[TIndex].Kind = t_mul;
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr2.Kind = intconst;
				TCodeList[TIndex].Addr2.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr2.Kind = string;
				strcpy(TCodeList[TIndex].Addr2.Name,PCodeList[PIndex-1].Addr.Name);
			}
			if(PCodeList[PIndex - 2].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-2].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-2].Addr.Name);
			}
			TCodeList[TIndex].Addr3.Kind = string;
			strcpy(TCodeList[TIndex].Addr3.Name, PCodeList[PIndex-3].Addr.Name);

            TIndex++;
            PIndex+=2;
            break;

        case p_sbi:
            // 处理 sbi 指令

			//printf("p_sbi");
            TCodeList[TIndex].Kind = t_sub;
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr2.Kind = intconst;
				TCodeList[TIndex].Addr2.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr2.Kind = string;
				strcpy(TCodeList[TIndex].Addr2.Name,PCodeList[PIndex-1].Addr.Name);
			}
			if(PCodeList[PIndex - 2].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-2].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-2].Addr.Name);
			}
			TCodeList[TIndex].Addr3.Kind = string;
			strcpy(TCodeList[TIndex].Addr3.Name, PCodeList[PIndex-3].Addr.Name);

            TIndex++;
	        PIndex+=2;
            break;

        case p_equ:
            // 处理 equ 指令

			//printf("p_equ");
            TCodeList[TIndex].Kind = t_eq;
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr2.Kind = intconst;
				TCodeList[TIndex].Addr2.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr2.Kind = string;
				strcpy(TCodeList[TIndex].Addr2.Name,PCodeList[PIndex-1].Addr.Name);
			}
			if(PCodeList[PIndex - 2].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-2].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-2].Addr.Name);
			}
			TCodeList[TIndex].Addr3.Kind = string;
			strcpy(TCodeList[TIndex].Addr3.Name, PCodeList[PIndex-3].Addr.Name);


            TIndex++;
			PIndex+=2;
          
            break;

        case p_wri:
            // 处理 wri 指令
	
			//printf("p_wri");
            TCodeList[TIndex].Kind = t_wri;
			if(PCodeList[PIndex - 1].Addr.Kind == intconst){
				TCodeList[TIndex].Addr1.Kind = intconst;
				TCodeList[TIndex].Addr1.Value = PCodeList[PIndex-1].Addr.Value;
			}else{
				TCodeList[TIndex].Addr1.Kind = string;
				strcpy(TCodeList[TIndex].Addr1.Name,PCodeList[PIndex-1].Addr.Name);
			}
            

            TIndex++;
			PIndex++;
           
            break;

        case p_stp:
            // 处理 stp 指令
		
			//printf("p_stp");
            TCodeList[TIndex].Kind = t_halt;
            TIndex++;
			PIndex++;
            break;
        default:
			PIndex++;
            break;
        }

    }
}
typedef struct _AddressEntry
{
	AddrKind Kind;
	const char *Content;
} AddressEntry;

typedef struct _TCodeEntry
{
	TOpKind Kind;
	AddressEntry Addr1, Addr2, Addr3;
} TCodeEntry;

/*
功能：
	初始化地址。
	
参数：
	pEntry -- 用于初始化地址的结构体。
	pAddr -- 地址指针。
*/
void InitAddress(const AddressEntry *pEntry, Address *pAddr)
{
	pAddr->Kind = pEntry->Kind;
	switch (pAddr->Kind)
	{
	case empty:
		break;
	case intconst:
		pAddr->Value = atoi(pEntry->Content);
		break;
	case string:
		strcpy(pAddr->Name, pEntry->Content);
		break;
	}
}

/*
功能：
	初始化三地址码列表（在执行流水线时调用）。
	
参数：
	pTCodeList -- 三地址码列表指针。
*/
void InitTCodeList(TCode *pTCodeList)
{
	for (int i = 0; i < MAX_CODE_COUNT; i++)
	{
		gets(TCodeArray[i]);
		int length = strlen(TCodeArray[i]);
		if (length == 0)
		{
			break;
		}
		char unitArr[7][64] = {};
		int k = 0, w = 0;
		for (int j = 0; j < 256; j++)
		{
			if (TCodeArray[i][j] == '\0')
			{
				break;
			}
			if (TCodeArray[i][j] == 32)
			{
				k++;
				w = 0;
				continue;
			}
			unitArr[k][w++] = TCodeArray[i][j];
		}

		char kind[64] = {0};
		strcpy(kind, unitArr[0]);
		if (strcmp(kind, "t_rd") == 0)
		{
			pTCodeList[i].Kind = t_rd;
		}
		else if (strcmp(kind, "t_gt") == 0)
		{
			pTCodeList[i].Kind = t_gt;
		}
		else if (strcmp(kind, "t_if_f") == 0)
		{
			pTCodeList[i].Kind = t_if_f;
		}
		else if (strcmp(kind, "t_asn") == 0)
		{
			pTCodeList[i].Kind = t_asn;
		}
		else if (strcmp(kind, "t_lab") == 0)
		{
			pTCodeList[i].Kind = t_lab;
		}
		else if (strcmp(kind, "t_mul") == 0)
		{
			pTCodeList[i].Kind = t_mul;
		}
		else if (strcmp(kind, "t_sub") == 0)
		{
			pTCodeList[i].Kind = t_sub;
		}
		else if (strcmp(kind, "t_eq") == 0)
		{
			pTCodeList[i].Kind = t_eq;
		}
		else if (strcmp(kind, "t_wri") == 0)
		{
			pTCodeList[i].Kind = t_wri;
		}
		else if (strcmp(kind, "t_halt") == 0)
		{
			pTCodeList[i].Kind = t_halt;
		}

		for (int j = 1; j < k; j += 2)
		{
			if (strcmp(unitArr[j], "string") == 0)
			{
				if (j == 1)
				{
					pTCodeList[i].Addr1.Kind = string;
					strcpy(pTCodeList[i].Addr1.Name, unitArr[j + 1]);
				}
				else if (j == 3)
				{
					pTCodeList[i].Addr2.Kind = string;
					strcpy(pTCodeList[i].Addr2.Name, unitArr[j + 1]);
				}
				else if (j == 5)
				{
					pTCodeList[i].Addr3.Kind = string;
					strcpy(pTCodeList[i].Addr3.Name, unitArr[j + 1]);
				}
			}
			else if (strcmp(unitArr[j], "intconst") == 0)
			{
				if (j == 1)
				{
					pTCodeList[i].Addr1.Kind = intconst;
					pTCodeList[i].Addr1.Value = atoi(unitArr[j + 1]);
				}
				else if (j == 3)
				{
					pTCodeList[i].Addr2.Kind = intconst;
					pTCodeList[i].Addr2.Value = atoi(unitArr[j + 1]);
				}
				else if (j == 5)
				{
					pTCodeList[i].Addr3.Kind = intconst;
					pTCodeList[i].Addr3.Value = atoi(unitArr[j + 1]);
				}
			}
		}
	}
}

/*
功能：
	输出三地址码列表。
	
参数：
	pCodeList -- P-代码列表指针。
*/
void OutputResult(PCode *PCodeList)
{

	for (int PIndex = 0; PCodeList[PIndex].Kind != 0; PIndex++)
	{
		switch (PCodeList[PIndex].Kind)
		{
		case p_lda:
			printf("lda%c%c%s\n", 32, 32, PCodeList[PIndex].Addr.Name);
			break;
		case p_rdi:
			printf("rdi\n");
			break;
		case p_lod:
			printf("lod%c%c%s\n", 32, 32, PCodeList[PIndex].Addr.Name);
			break;
		case p_ldc:
			printf("ldc%c%c%d\n", 32, 32, PCodeList[PIndex].Addr.Value);
			break;
		case p_grt:
			printf("grt\n");
			break;
		case p_fjp:
			printf("fjp%c%c%s\n", 32, 32, PCodeList[PIndex].Addr.Name);
			break;
		case p_sto:
			printf("sto\n");
			break;
		case p_lab:
			printf("lab%c%c%s\n", 32, 32, PCodeList[PIndex].Addr.Name);
			break;
		case p_mpi:
			printf("mpi\n");
			break;
		case p_sbi:
			printf("sbi\n");
			break;
		case p_equ:
			printf("equ\n");
			break;
		case p_wri:
			printf("wri%c%c%s\n", 32, 32, PCodeList[PIndex].Addr.Name);
			break;
		case p_stp:
			printf("stp\n");
			break;
		}
	}
}
void OutputResult1(TCode *TCodeList)
{

	for (int TIndex = 0; TCodeList[TIndex].Kind != 0; TIndex++)
	{
		switch (TCodeList[TIndex].Kind)
		{
		case t_rd:
			printf("t_rd");
			printt(TCodeList,TIndex,1);
			break;
		case t_gt:
			printf("t_gt");
			printt(TCodeList,TIndex,3);
			break;
		case t_if_f:
			printf("t_if_f");
			printt(TCodeList,TIndex,2);
			break;
		case t_lab:
			printf("t_lab");
			printt(TCodeList,TIndex,1);
			break;
		case t_mul:
			printf("t_mul");
			printt(TCodeList,TIndex,3);
			break;
		case t_sub:
			printf("t_sub");
			printt(TCodeList,TIndex,3);
			break;
		case t_eq:
			printf("t_eq");
			printt(TCodeList,TIndex,3);
			break;
		case t_wri:
			printf("t_wri");
			printt(TCodeList,TIndex,1);
			break;
		case t_halt:
			printf("t_halt\n");
			break;
		case t_asn:
			printf("t_asn");
			printt(TCodeList,TIndex,2);
			break;
		}
	}
}
void printt(TCode *TCodeList,int TIndex,int n){
	if(n==1){
		if(TCodeList[TIndex].Addr1.Kind == intconst){
			printf("%c%c%s  %d\n", 32, 32, "intconst",TCodeList[TIndex].Addr1.Value);
		}else{
			printf("%c%c%s  %s\n", 32, 32, "string",TCodeList[TIndex].Addr1.Name);
		}
	}else if (n==2)
	{
		if(TCodeList[TIndex].Addr1.Kind == intconst){
			printf("%c%c%s  %d", 32, 32, "intconst",TCodeList[TIndex].Addr1.Value);
		}else{
			printf("%c%c%s  %s", 32, 32, "string",TCodeList[TIndex].Addr1.Name);
		}
		if(TCodeList[TIndex].Addr2.Kind == intconst){
			printf("%c%c%s  %d\n", 32, 32, "intconst",TCodeList[TIndex].Addr2.Value);
		}else{
			printf("%c%c%s  %s\n", 32, 32, "string",TCodeList[TIndex].Addr2.Name);
		}
	}else{
		if(TCodeList[TIndex].Addr1.Kind == intconst){
			printf("%c%c%s  %d", 32, 32, "intconst",TCodeList[TIndex].Addr1.Value);
		}else{
			printf("%c%c%s  %s", 32, 32, "string",TCodeList[TIndex].Addr1.Name);
		}
		if(TCodeList[TIndex].Addr2.Kind == intconst){
			printf("%c%c%s  %d", 32, 32, "intconst",TCodeList[TIndex].Addr2.Value);
		}else{
			printf("%c%c%s  %s", 32, 32, "string",TCodeList[TIndex].Addr2.Name);
		}
		if(TCodeList[TIndex].Addr3.Kind == intconst){
			printf("%c%c%s  %d\n", 32, 32, "intconst",TCodeList[TIndex].Addr3.Value);
		}else{
			printf("%c%c%s  %s\n", 32, 32, "string",TCodeList[TIndex].Addr3.Name);
		}
	}
	
}
