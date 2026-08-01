#ifndef SCE_IR_HELPER_H_
#define SCE_IR_HELPER_H_
#include "sce_basic_api.h"
/**
* メモリアクセス
* 代入文
* obj A = "HELLO WORLD\n";
* SMOV R0, "HELLO WORLD"
* MRMOV &A, R0
* メモリに書込み
* レジスタは&Aに依存しない
* obj A = "HELLO WORLD\n"
* SMOV R0, "HELLO WORLD"
* MOV R1, &A
* RMOV R1, R0
* に書きこむ
* R1がメモリか判断する必要がある
* R1がメモリ->R1の中身を解放する必要はあり
* そこから書込み
* しかしこのメリットは引数の際メモリコピーは容易である
* 例えば
* MRMOVのみでは引数は
* MRMOV &A, &R0のため、コピーはできない
* 必ず、レジスタにメモリをコピーする必要がある
* そしてこの問題とはつまり、
* さらに前回の結果をR1に残るので
* R1 = 1をすると前回に代入するということである。
* 例えば以下の文がある。
* MOV R1, &MEMORY_A
* SAVE_REGISTER
* MOV R258, &str
* RMOV R1, R258
* この時、MRMOVも同じような場合になる(メンバアクセス含む引数命令は結局これを挟む)
* このようにすると
* SAVE_REGISTERは前回のMEMORY_Aのレジスタ領域をリフレッシュしない仕様である
* そのため、これを書き込むと
* MOVに258にstr参照を入れる
* RMOVの前回のMEMORY_Aの中にstrのメモリ値をコピーする
* ちなみに以下は
* RMOVのコピー使用である
* 左/右 |メモリ |値
* メモリ|左参 　|左参
* 値    |左本   |左本
* 左参は左を参照で書込み
* 左本は左を参照でないレジスタメモリに書込み
* という意味である
* 1.
* &strを参照だから書き込まないは他に対応できない、
* 命令を増やすのは別に簡単で、普通にRRMOVなどのMOVを増やすことは別に問題なく
* これはどのような値も全てレジスタ領域として書き込むなど
* 他はSVAE時にリフレッシュをするなど(但し毎回、不必要なリフレッシュを挟むかも)
* んで他は
* RMOVの使用に書き込み禁止の場合以外でコピー?しかし、
* MOVは基本参照、書込みあり、設定->これをメモリだけ書込み禁止は代入できない
* つまり、命令を追加するなどがある
* そこで
* MMOV R0, a
* んでこれは元々ある、参照をコピーするという命令
* MOVは普段
* REG.MEM_TYPE==TYPE
* REG.MEM = &a
* となるが
* MMOVは
* REG = aを入れるという命令
* これであればREGはaの中をコピーするという意味である。
* 但し、a.string型であれば、それはディ―プコピーするという仕様である。
* これを変えてstringも参照させるという方法をすれば
* RMOV時、REGの中身はメモリ参照でないのでそのまま代入するということになる
* そもそも
* MMOVはaddの時、
* add R0, R1をする時、
* addはR0に加算するという命令であったためである。
* このため、R0は参照ならば、参照に直接、即値であれば、レジスタR0に書き込むといもの
* この時、addで新しいメモリをつくったりするのが、いやだったのでstring型も全てコピーするという命令であった
* これは別にstring型ならいいのだが、objectと考えると
* addは結果をレジスタに新しく上書きするの方がいいだろうか？
* この場合、
* i += 1;
* MOV R0, &i
* ADD, R0, 1
* MMOVは即値なら、ディ―プコピー、参照なら参照コピー
* MOVは常に参照コピーなら
* MOV i, &i
* しかし、
* i = i + 1;
* もしMMOVを即値なら、ディ―プコピー、参照なら参照コピーにすると
* 命令列は変える必要はある？
* i += "STRING";
* 今までの方式
* MOV r0, &i
* SMOV r1, "STRING"
* ADD r0, r1
* r0の中身にr1を書き込む
* i = i + "STRING"
* MOV r0, &i
* MMOV r1, i
* SMOV r2, "STRING"
* ADD r1, r2
* RMOV r0, r1
* r1にディ―プコピーするそれをr2と加算するr1のstringにメモリ結果、それをr0に入れる
* 即値なら、即値結果をMOVしたならメモリ、MOVにMMOVならレジスタに書きもむ
* では、STRINGは?この場合もしMOV, MMOVをどっちとも参照にするなら命令列を
* MOV, r0, &iをする
* MMOV, r1, &iをする
* SMOV r2, "HELLO"
* ADD r1, r2
* RMOV r2, r1をする？
*/
#define SET_SCE_BINARY_MACHINE_TYPE(CONVERT_FUNC)\
CONVERT_FUNC(E_SCE_BINARY_INST_MOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_RMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_RRMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_MRMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_MMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_CMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_BMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_IMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_FMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_VMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_OMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_AMOV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_NAME__),\
CONVERT_FUNC(E_SCE_BINARY_INST_ADD__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SUB__),\
CONVERT_FUNC(E_SCE_BINARY_INST_MUL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_DIV__),\
CONVERT_FUNC(E_SCE_BINARY_INST_MOD__),\
CONVERT_FUNC(E_SCE_BINARY_INST_AND__),\
CONVERT_FUNC(E_SCE_BINARY_INST_OR__),\
CONVERT_FUNC(E_SCE_BINARY_INST_XOR__),\
CONVERT_FUNC(E_SCE_BINARY_INST_NOT__),\
CONVERT_FUNC(E_SCE_BINARY_INST_NOTL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SHR__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SHL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_EQ__),\
CONVERT_FUNC(E_SCE_BINARY_INST_NEQ__),\
CONVERT_FUNC(E_SCE_BINARY_INST_LS__),\
CONVERT_FUNC(E_SCE_BINARY_INST_GT__),\
CONVERT_FUNC(E_SCE_BINARY_INST_LSEQ__),\
CONVERT_FUNC(E_SCE_BINARY_INST_GTEQ__),\
CONVERT_FUNC(E_SCE_BINARY_INST_TEST__),\
CONVERT_FUNC(E_SCE_BINARY_INST_RET__),\
CONVERT_FUNC(E_SCE_BINARY_INST_CALL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_INDIRECT_CALL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_JMP__),\
CONVERT_FUNC(E_SCE_BINARY_INST_PHASE__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SYSTEM_CALL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_ACCESS_MEM_MEMBER__),\
CONVERT_FUNC(E_SCE_BINARY_INST_ACCESS_LABEL_MEMBER__),\
CONVERT_FUNC(E_SCE_BINARY_INST_ACCESS_ARRAY_MEMORY__),\
CONVERT_FUNC(E_SCE_BINARY_INST_LABEL__),\
CONVERT_FUNC(E_SCE_BINARY_INST_ALLOC__),\
CONVERT_FUNC(E_SCE_BINARY_INST_RALLOC__),\
CONVERT_FUNC(E_SCE_BINARY_INST_NAME_REPEAT__),\
CONVERT_FUNC(E_SCE_BINARY_INST_STACK_POINT__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SUB_STACK_POINT__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SAVE_REGISTER__),\
CONVERT_FUNC(E_SCE_BINARY_INST_LOAD_REGISTER__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SAVE_REGISTER_STACK__),\
CONVERT_FUNC(E_SCE_BINARY_INST_LOAD_REGISTER_STACK__),\
CONVERT_FUNC(E_SCE_BINARY_INST_ADD_STACK__),\
CONVERT_FUNC(E_SCE_BINARY_INST_SUB_STACK__),\
CONVERT_FUNC(E_SCE_BINARY_INST_DELETE_REGISTER__),
typedef enum Sce_Binary_Instruction {
	SET_SCE_BINARY_MACHINE_TYPE(CONVERT_DEFINE)
}Sce_Binary_Instruction;
/**
* i = func;
* r0 &i
* r1 &func
* INDIRECT &i = &func
* 
*/
typedef struct Sce_Binary_Machine_Instructions {
	struct Sce_Binary_Machine_Instructions* next;
	Sce_Binary_Instruction sce_bmr_code;

	uint32_t ope1;
	union {
		uint32_t ope2;
		int64_t icope2;
		uint8_t ccope2;
		uint8_t* scope2;
		void* vcope2;
		float fcope2;
		double dcope2;
		bool bcope2;
	};
}Sce_Binary_Machine_Instructions;

#define SCE_VIRTUAL_REGISTER_MAX 127
#define SCE_VIRTUAL_GENERAL_REGISTER_MAX 128
#define SCE_VIRTUAL_REGISTER_RETURN SCE_VIRTUAL_GENERAL_REGISTER_MAX
#define SCE_VIRTUAL_REGISTER_CMP_RETURN SCE_VIRTUAL_REGISTER_RETURN
#define SCE_VIRTUAL_REGISTER_MAX_ 129
#define SCE_VIRTUAL_STACK_MIN SCE_VIRTUAL_REGISTER_MAX_
#define GET_SCE_STACK_START_OFFSET(idx) idx - SCE_VIRTUAL_STACK_MIN

#endif