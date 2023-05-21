// armFith.c
#include "../localTypes.h"
#include "../inc/smallFith_i.h"

// TODO LIST
// print words
// strings
// DONE:TEST CASE string compare function in fith
// 		str cmp was 148 bytes versus C's was 24 bytes. This is an extreme and shows
// 		that this is better suited as something to put things together versus the
// 		implementation language for tight speedy algorithms
// add address of operator
// TEST CASE linked list impl
// DONE: call word
// inline ability to be implemented ( for struct like access)
// DONE: repeat{ loop }

#define F_IMMED  0x80
#define F_HIDDEN 0x40
#define TOS 0
#define SCRATCH 1
#define CTX 4
#define RSP 5
#define G1 6
#define G2 7

/*e*/static void
mc_callRelative(u32 target, u32 currentAddr)/*i;*/
{
	u16 *outputCursor = (u16*)currentAddr;
	u32 currentPC = currentAddr + 4;
	u32 jump = (target - currentPC) >> 1;
	u32 imm11 = jump << 21 >> 21;
	u32 imm10 = jump << 11 >> 22;
	u32 i2    = jump << 10 >> 31;
	u32 i1    = jump << 9 >> 31;
	u32 S     = jump << 8 >> 31;
	u32 j2    = (i2^1)^S;
	u32 j1    = (i1^1)^S;
	u32 code = 0xF000;
	code += (S<<10) + imm10;
	*outputCursor++ = code;
	code = 0xD000;
	code += (j2<<11) + (j1<<13) + imm11;
	*outputCursor = code;
}

/*e*/static void
callWord(SmallContext *s, u32 target)/*i;*/
{
	u32 currentAddr = (u32)s->compileCursor;
	mc_callRelative(target, currentAddr);
	s->compileCursor += 2;
}

/*e*/u32
armBranch(s32 imm)/*p;*/
{
	u32 code = 0xE000;
	if (imm < -1024 || imm > 1023)
	{
		io_printsn("Error: branch out of range.");
		imm = -1;
	}
	u32 encode = imm;
	code += (encode<<21)>>21;
	return code;
}

/*e*/u32
armCond(u32 cond, s32 imm)/*p;*/
{
	u32 code = 0xD000;
	if (imm < -128 || imm > 127)
	{
		io_printsn("Error: conditional branch out of range.");
		imm = -1;
	}
	u32 encode = imm;
	code += ((encode<<24)>>24) + (cond << 8);
	return code;
}

/*e*/static u32
armPush(u32 regBits)/*i;*/
{
	u32 code = 0xB400;
	code += regBits;
	return code;
}

/*e*/static u32
armPop(u32 regBits)/*i;*/
{
	u32 code = 0xBC00;
	code += regBits;
	return code;
}

/*e*/static u32
armBx(u32 reg)/*i;*/
{
	u32 code = 0x4700;
	code += reg << 3;
	return code;
}

/*e*/static u32
armMovImm(u32 dest, u32 val)/*i;*/
{
	u32 code = 0x2000;
	code += val + (dest << 8);
	return code;
}

/*e*/static u32
armMov(u32 dest, u32 src)/*i;*/
{
	u32 code = 0x4600;
	code += ((dest>>3)<<7) + ((dest<<29)>>29) + (src << 3);
	return code;
}

/*e*/static u32
armSubImm(u32 dest, u32 val)/*i;*/
{
	u32 code = 0x3800;
	code += val + (dest << 8);
	return code;
}

/*e*/static u32
armSub3(u32 dest, u32 arg1, u32 arg2)/*i;*/
{
	u32 code = 0x1A00;
	code += dest + (arg1 << 3) + (arg2 << 6);
	return code;
}

/*e*/static u32
armAddImm(u32 dest, u32 val)/*i;*/
{
	u32 code = 0x3000;
	code += val + (dest << 8);
	return code;
}

/*e*/static u32
armAdd3(u32 dest, u32 arg1, u32 arg2)/*i;*/
{
	u32 code = 0x1800;
	code += dest + (arg1 << 3) + (arg2 << 6);
	return code;
}

static u32
armLdrOffset(u32 dest, u32 src, u32 offset)
{
	u32 code = 0x6800;
	code += dest + (src << 3) + (offset << 6);
	return code;
}

static u32
armStrOffset(u32 src, u32 dest, u32 offset)
{
	u32 code = 0x6000;
	code += src + (dest << 3) + (offset << 6);
	return code;
}

static u32
armLslsImm(u32 dest, u32 src, u32 val)
{
	u32 code = 0x0000;
	code += dest + (src << 3) + ((val&0X1F) << 6);
	return code;
}

static u32
armLsls(u32 dest, u32 arg1)
{
	u32 code = 0x4080;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armLsrsImm(u32 dest, u32 src, u32 val)
{
	u32 code = 0x0800;
	code += dest + (src << 3) + ((val&0X1F) << 6);
	return code;
}

static u32
armLsrs(u32 dest, u32 arg1)
{
	u32 code = 0x40C0;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armCmpImm(u32 left, u32 val)
{
	u32 code = 0x2800;
	code += val + (left << 8);
	return code;
}

static u32
armCmp(u32 left, u32 right)
{
	u32 code = 0x4280;
	code += left + (right << 3);
	return code;
}

static void
mc_stackAdd(SmallContext *c)
{
	u32 prevCode = *(c->compileCursor-1);
	if ( (prevCode>>8) == 32)
	{
		// we just pushed a small constant, re-write
		c->compileCursor -= 2;
		u32 val = (prevCode<<24)>>24;
		*c->compileCursor++ = armAddImm(TOS, val);
		return;
	}
	*c->compileCursor++ = armPop(1<<SCRATCH);
	*c->compileCursor++ = armAdd3(TOS,TOS,SCRATCH);
}

static void
mc_stackSub(SmallContext *c)
{
	u32 prevCode = *(c->compileCursor-1);
	if ( (prevCode>>8) == 32)
	{
		// we just pushed a small constant, re-write
		c->compileCursor -= 2;
		u32 val = (prevCode<<24)>>24;
		*c->compileCursor++ = armSubImm(TOS, val);
		return;
	}
	*c->compileCursor++ = armPop(1<<SCRATCH);
	*c->compileCursor++ = armSub3(TOS,SCRATCH,TOS);
}

static void
mc_stackLS(SmallContext *c)
{
	u32 prevCode = *(c->compileCursor-1);
	if ( (prevCode>>8) == 32)
	{
		// we just pushed a small constant, re-write
		c->compileCursor -= 2;
		u32 val = (prevCode<<24)>>24;
		*c->compileCursor++ = armLslsImm(TOS, TOS, val);
		return;
	}
	*c->compileCursor++ = armMov(SCRATCH, TOS);
	*c->compileCursor++ = armPop(1<<TOS);
	*c->compileCursor++ = armLsls(TOS,SCRATCH);
}

static void
mc_stackRS(SmallContext *c)
{
	u32 prevCode = *(c->compileCursor-1);
	if ( (prevCode>>8) == 32)
	{
		// we just pushed a small constant, re-write
		c->compileCursor -= 2;
		u32 val = (prevCode<<24)>>24;
		*c->compileCursor++ = armLsrsImm(TOS, TOS, val);
		return;
	}
	*c->compileCursor++ = armMov(SCRATCH, TOS);
	*c->compileCursor++ = armPop(1<<TOS);
	*c->compileCursor++ = armLsrs(TOS,SCRATCH);
}

/*e*/static void
mc_condition(SmallContext *s)/*i;*/
{
	// compare top two items on the stack and consume them
	u32 prevCode = *(s->compileCursor-1);
	if ( (prevCode>>8) == 32)
	{
		// we just pushed a small constant, re-write
		s->compileCursor -= 2;
		u32 val = (prevCode<<24)>>24;
		*s->compileCursor++ = armCmpImm(TOS, val);
		*s->compileCursor++ = armPop(1<<TOS);
		return;
	}
	*s->compileCursor++ = armPop(1<<SCRATCH);
	*s->compileCursor++ = armCmp(SCRATCH,TOS);
	*s->compileCursor++ = armPop(1<<TOS);
}

/*e*/static void
mc_call(SmallContext *s)/*i;*/
{
	s->notLeaf = 1;
	*s->compileCursor++ = armMov(SCRATCH, TOS);
	*s->compileCursor++ = armPop(1<<TOS);
	*s->compileCursor++ = armMov(15, SCRATCH);
}


#define COND_BEQ 0x0
#define COND_BNE 0x1
#define COND_BGE 0xA
#define COND_BLT 0xB
#define COND_BGT 0xC
#define COND_BLE 0xD

enum{
	WORD_FUNCTION,
	WORD_FUNCTION_COMP,
	WORD_GLOBAL,
	WORD_CONSTANT,
	WORD_LOCAL,
	WORD_DIRECT,
	WORD_COND_BEQ,
	WORD_COND_BNE,
	WORD_COND_BGE,
	WORD_COND_BLT,
	WORD_COND_BGT,
	WORD_COND_BLE,
	WORD_ELSE,
	WORD_RETURN,
	WORD_WHILE,
	WORD_CASE,
	WORD_AGAIN,
	WORD_LEAVE,
	WORD_REPEAT,
	WORD_CALL,
	WORD_COMPILE,
};

enum{
	BLOCK_NONE,
	BLOCK_WORD,
	BLOCK_COND,
	BLOCK_ELSE,
	BLOCK_WHILE,
	BLOCK_WHILE_COND,
	BLOCK_CASE,
	BLOCK_CASE_COND,
	BLOCK_CASE_COND_CLOSED,
	BLOCK_REPEAT,
	BLOCK_STRUCT,
};

enum{
	COM_START,
	COM_IN_COMMENT,
	COM_TOKEN,
};

SmallContext s;
u8  wordBuff[128];

/*e*/static void
mc_stackStrTo(SmallContext *s)/*i;*/
{
	u32 prevCode = *(s->compileCursor-1);
	u32 val = (prevCode<<24)>>24;
	if ( (prevCode>>11) == 6 && val <= 124)
	{
		// we just added a small constant, re-write
		s->compileCursor -= 1;
		*s->compileCursor++ = armPop(1<<SCRATCH);
		*s->compileCursor++ = armStrOffset(SCRATCH,TOS,val>>2);
		*s->compileCursor++ = armPop(1<<TOS);
		return; // 4 + $= 
	}
	*s->compileCursor++ = armPop(1<<SCRATCH);
	*s->compileCursor++ = armStrOffset(SCRATCH,TOS,0);
	*s->compileCursor++ = armPop(1<<TOS); // $=
}

/*e*/static void
mc_stackLdrTo(SmallContext *s)/*i;*/
{
	u32 prevCode = *(s->compileCursor-1);
	u32 val = (prevCode<<24)>>24;
	if ( (prevCode>>11) == 6 && val <= 124)
	{
		// we just added a small constant, re-write
		s->compileCursor -= 1;
		*s->compileCursor++ = armLdrOffset(TOS,TOS,val>>2);
		return;
	}
	*s->compileCursor++ = armLdrOffset(TOS,TOS,0);
}

static void
mc_loadLocal(SmallContext *s, u32 index)
{
	*s->compileCursor++ = armPush(1<<TOS);
	*s->compileCursor++ = armLdrOffset(TOS, RSP, index);
}

static void
mc_storeLocal(SmallContext *s, u32 index)
{
	*s->compileCursor++ = armStrOffset(TOS, RSP, index);
	*s->compileCursor++ = armPop(1<<TOS);
}

static void
mc_addrOfLocal(SmallContext *s, u32 index)
{
	*s->compileCursor++ = armPush(1<<TOS);
	*s->compileCursor++ = armMov(TOS, RSP);
	if (index > 0)
	{
		*s->compileCursor++ = armAddImm(TOS, index < 2);
	}
}

static void
mc_loadGlobal(SmallContext *s, Word *word)
{
	*s->compileCursor++ = armPush(1<<TOS);
	u32 cfa = wordToCfa(word);
	u16 *globalIndex = (u16*)cfa;
	u32 index = *globalIndex;
	if (index < 32)
	{
		*s->compileCursor++ = armLdrOffset(TOS, G1, index);
	} else {
		*s->compileCursor++ = armLdrOffset(TOS, G2, index - 32);
	}
}

static void
mc_storeGlobal(SmallContext *s, Word *word)
{
	u32 cfa = wordToCfa(word);
	u16 *globalIndex = (u16*)cfa;
	u32 index = *globalIndex;
	if (index < 32)
	{
		*s->compileCursor++ = armStrOffset(TOS, G1, index);
	} else {
		*s->compileCursor++ = armStrOffset(TOS, G2, index - 32);
	}
	*s->compileCursor++ = armPop(1<<TOS);
}

static void
mc_addrOfGlobal(SmallContext *s, Word *word)
{
	*s->compileCursor++ = armPush(1<<TOS);
	u32 cfa = wordToCfa(word);
	u16 *globalIndex = (u16*)cfa;
	u32 index = *globalIndex;
	if (index < 32)
	{
		*s->compileCursor++ = armMov(TOS, G1);
	} else {
		*s->compileCursor++ = armMov(TOS, G2);
		index -= 32;
	}
	if (index > 0)
	{
		*s->compileCursor++ = armAddImm(TOS, index < 2);
	}
}

static void
mc_integerLit(SmallContext *s, s32 value)
{
	*s->compileCursor++ = armPush(1<<TOS);
	if (value >= 0 && value <= 255)
	{
		*s->compileCursor++ = armMovImm(TOS, value);
	} else {
		if ((u32)s->compileCursor & 0x02) { *s->compileCursor++ = armMov(8, 8); }
		*(u32*)s->compileCursor = 0xE0014800;
		s->compileCursor+= 2;
		*(u32*)s->compileCursor = value;
		s->compileCursor+= 2;
	}
}

/*e*/u16*
mc_finalizeFunction(SmallContext *s)/*p;*/
{
	u32 numVarsOnStack = s->varNumHighWater;

	u16 *functionEnd = s->compileBase;
	u16 *functionStart = functionEnd + 3;
	// emit jump to end
	*s->compileCursor = armBranch(functionEnd - s->compileCursor - 2);
	// check if function already ended in return
	if (((*s->compileCursor)+1) != *(s->compileCursor-1)) {s->compileCursor++;}
	// emit function end
	// check if less than 3 and convert to an inline function
	//~ if (s->compileCursor - &functionStart[3]
	functionEnd[0] = armLdrOffset(SCRATCH, RSP, numVarsOnStack);
	functionEnd[1] = armAddImm(RSP, ((numVarsOnStack+1)*4));
	functionEnd[2] = armBx(SCRATCH);
	// emit function start
	functionStart[0] = armMov(SCRATCH, 14);
	functionStart[1] = armSubImm(RSP, ((numVarsOnStack+1)*4));
	functionStart[2] = armStrOffset(SCRATCH, RSP, numVarsOnStack);
	//~ u16 *cursor = &functionStart[3];
	//~ u16 *cursor = functionEnd;
	//~ while (cursor < s->compileCursor)
	//~ {
		//~ io_printhn(*cursor++);
	//~ }
	// reset state
	s->varNumHighWater = 0;
	return (void*)((u32)functionStart + 1);
}

/*e*/
u32
combineToken(SmallContext *s, u32 byte)/*p;*/
{
	u32 state  = s->combineWordState;
	u8 *cursor = s->cursor;

	switch (state)
{
	case COM_START:
	{
		// Check for regular whitespace
		if (byte <= ' ') { return 0; }
		// Check for comment
		if (byte == '\\')
		{
			s->combineWordState = COM_IN_COMMENT;
			return 0;
		}
		// record character and build token
		*cursor = byte;
		s->cursor = cursor + 1;
		s->combineWordState = COM_TOKEN;
	} break;
	case COM_IN_COMMENT:
	{
		// Check for '\r' to resume
		if (byte == '\r') { s->combineWordState = COM_START; }
	} break;
	case COM_TOKEN:
	{
		// gather regular token
		if (byte > ' '){ *cursor = byte; s->cursor = cursor + 1; }
		else {
			*cursor = 0;
			s->cursor = wordBuff;
			s->combineWordState = COM_START;
			return inquireOfToken(s, wordBuff, cursor - wordBuff);
		}
	} break;
}
	return 0;
}

/*e*/
u32
inquireOfToken(SmallContext *s, u8 *string, u32 length)/*p;*/
{
	// check for special single character cases
	if (length == 1)
	{
		switch (string[length-1])
		{
		case '+': mc_stackAdd(s); goto executeOrContinue;
		case '-': mc_stackSub(s); goto executeOrContinue;
		case '>': mc_stackRS(s); goto executeOrContinue;
		case '<': mc_stackLS(s); goto executeOrContinue;
		case '}': sEndBlock(s); goto executeOrContinue;
		case '$': mc_stackLdrTo(s); goto executeOrContinue;
		case '=': mc_stackStrTo(s); goto executeOrContinue;
		}
	}
	// check for ending of params as special case
	if (string[0] == ')' && string[1] == '{' && length == 2) { compileParams(s); return 0; }
	// check for special single endings
	switch (string[length-1])
	{
	case '=': assignVar(s, string, length - 1); goto executeOrContinue;
	}
	// check for special starts
	switch (string[0])
	{
	case '@': pushAddressOf(s, string+1, length - 1); goto executeOrContinue;
	}
	
	// check for special single endings
	if (s->inParams) { createVar(s, string, length);  return 0; }
	// see if word is a local variable
	Tree *local = resolveLocal(s, string, length);
	if (local) { mc_loadLocal(s, (u32)local->value); return 0; }
	
	// see if word is in the global dictionary
	Word *word = searchForWord(s->words, string, length);
	if (word != 0)
	{
		// word is in the global dictionary
		u32 cfa = wordToCfa(word);
		// process according to type
		switch (word->type)
		{
		case WORD_FUNCTION: callWord(s, cfa); goto executeOrContinue;
		case WORD_FUNCTION_COMP: callWord(s, cfa+6); goto executeOrContinue;
		case WORD_GLOBAL: mc_loadGlobal(s, word); goto executeOrContinue;
		case WORD_DIRECT: {
			u32 *code = (u32*)((cfa + 3) & -4);
			callWord(s, *code); goto executeOrContinue; }
		case WORD_COND_BEQ:
		case WORD_COND_BNE: compileCond(s, word->type-WORD_COND_BEQ); return 0;
		case WORD_COND_BGE:
		case WORD_COND_BLT:
		case WORD_COND_BGT:
		case WORD_COND_BLE:compileCond(s,word->type-WORD_COND_BGE+0xA);return 0;
		case WORD_ELSE: compileElse(s); return 0;
		case WORD_RETURN:
		*s->compileCursor = armBranch(s->compileBase -s->compileCursor-2);
		s->compileCursor++; goto executeOrContinue;
		case WORD_WHILE: compileWhile(s); return 0;
		case WORD_CASE: compileCase(s); return 0;
		case WORD_AGAIN: compileAgain(s); return 0;
		case WORD_LEAVE: compileLeave(s); return 0;
		case WORD_REPEAT: compileRepeat(s); return 0;
		case WORD_CALL: mc_call(s); goto executeOrContinue;
		}
		// WORD_COMPILE must be last type, outputs number of machine codes
		if (word->type > WORD_COMPILE)
		{
			u32 numCode = word->type - WORD_COMPILE;
			u16 *code = (u16*)cfa;
			while (numCode > 0) {
				*s->compileCursor++ = *code++; numCode--;
			}
			goto executeOrContinue;
		}
		io_printsn("Error: Word does not have valid type."); return 0;
	}
	// check for special single endings
	switch (string[length-1])
	{
	case '{': createWordFunction(s, string, length - 1); return 0;
	case ';': createVar(s, string, length - 1);  goto executeOrContinue;
	case '(': createWordFunction(s, string, length - 1); s->inParams=1;return 0;
	}
	// word is not found see if it is a number
	u32 val = s2i(string);
	if (val!=0||*string=='0') {mc_integerLit(s, val); goto executeOrContinue;}
	// no match found, set and print error
	s->error = 1;
	io_printsn("Error: Word not found and cannot be interpreted as a number.");
	return 0;
	executeOrContinue:
	if (s->blockCursor != s->blockBase) { return 0; }
	u32 addr = (u32)mc_finalizeFunction(s);
	s->compileCursor = s->compileBase + 6;
	return addr;
}

/*e*/
void
smallFithInitP2(Word *last, SmallContext *s, void *heap)/*p;*/
{
	s->words = last;
	s->heapCursor = heap;
	s->compileBase		= (void*)__bss_end__;
	s->compileCursor	= (void*)__bss_end__;
	s->blockBase		= (void*)0x20041200;
	s->blockCursor		= s->blockBase;
	s->compileCursor	= s->compileBase + 6;
	s->cursor			= wordBuff;
	s->scope 			= s->scopes;
}

/*e*/
Word *
searchForWord(Word *word, u8 *string, u32 length)/*p;*/
{
	while (1) {
		//~ if (word->type & F_HIDDEN) { goto next; }
		if (word->keyLen != length) { goto next; }
		u8 *key = word->key;
		u32 i = 0;
		do {
			if (string[i] != key[i]) { goto next; } i++;
		} while (i < length);
		return word;
		next:
		if (word->next == 0) { break; }
		word = (Word*)((s32)word + (s32)word->next);
	}
	return 0;
}

/*e*/
u32
wordToCfa(Word *word)/*p;*/
{
	u32 cwp = (u32)word;
	cwp = ((cwp + word->keyLen + 6) + 2) & -2;
	return cwp;
}

/*e*/static Word *
createEntryAt(void *base, u8 *start, u32 length)/*i;*/
{
	Word *word = base;
	// set length
	word->keyLen = length;
	// copy in word
	for (u32 i = 0; i < length; i++)
	{
		word->key[i] = start[i];
	}
	// null terminate
	word->key[length] = 0;
	return word;
}

/*e*/static Word *
createGlobalEntry(SmallContext *s, u8 *start, u32 length)/*i;*/
{
	Word *word = createEntryAt(s->compileBase, start, length);
	// add word to global list
	word->next = ((s32)s->words - (s32)word);
	s->words  = word;
	// get cfa, set new base and cursor for global creation
	u32 cfa = wordToCfa(word);
	s->compileBase = (u16*)cfa;
	s->compileCursor = s->compileBase + 6;
	return word;
}

/*e*/static void
createWordFunction(SmallContext *s, u8 *start, u32 length)/*i;*/
{
	if (s->blockCursor != s->blockBase) { io_printsn("Error: within a block."); return; }
	Word *word = createGlobalEntry(s, start, length);
	word->type = WORD_FUNCTION_COMP;
	Block *newBlock = s->blockCursor++;
	newBlock->blockType = BLOCK_WORD;
	s->error = 0;
	//~ newBlock->word = (void*)word;
	enterScope(s);
}

/*e*/static void
createVar(SmallContext *s, u8 *start, u32 length)/*i;*/
{
	if (s->blockCursor != s->blockBase)
	{
		// create local
		Tree *local = makeLocalDecl(s, start, length);
		mc_storeLocal(s, (u32)local->value);
	} else {
		// create global
		Word *word = createGlobalEntry(s, start, length);
		*s->compileBase++ = s->globalsIndex++;
		s->compileCursor = s->compileBase + 6;
		word->type = WORD_GLOBAL;
		mc_storeGlobal(s, word);
	}
}

/*e*/static void
assignVar(SmallContext *s, u8 *start, u32 wordLength)/*i;*/
{
	// check for locals first
	Tree *local = resolveLocal(s, start, wordLength);
	if (local) { mc_storeLocal(s, (u32)local->value); return; }
	// next check global
	Word *word = searchForWord(s->words, start, wordLength);
	if (word && word->type == WORD_GLOBAL)
	{
		mc_storeGlobal(s, word); return;
	}
	io_prints("Error: cannot store to ");
	io_printsln(start, wordLength);
	return;
}

/*e*/static void
pushAddressOf(SmallContext *s, u8 *start, u32 wordLength)/*i;*/
{
	// check for locals first
	Tree *local = resolveLocal(s, start, wordLength);
	if (local) { mc_addrOfLocal(s, (u32)local->value); return; }
	// next check global
	Word *word = searchForWord(s->words, start, wordLength);
	if (word)
	{
		u32 cfa = wordToCfa(word);
		switch (word->type)
		{
		case WORD_FUNCTION: mc_integerLit(s, cfa); return;
		case WORD_FUNCTION_COMP: mc_integerLit(s, cfa+6); return;
		case WORD_GLOBAL: mc_addrOfGlobal(s, word); return;
		case WORD_DIRECT: {
			u32 *code = (u32*)((cfa + 3) & -4);
			mc_integerLit(s, *code); return; }
		}
	}
	io_prints("Error: cannot get address of ");
	io_printsln(start, wordLength);
	return;
}


/*e*/static Tree*
resolveLocal(SmallContext *c, u8 *start, u32 length)/*i;*/
{
	LocalScope *cursor = c->scope;
	do {
		Tree *target = tree_find(cursor->symbols, start, length);
		if (target) { return target; }
		cursor--;
	} while (cursor > c->scopes);
	return 0;
}

/*e*/ static Tree *
makeLocalDecl(SmallContext *c, u8 *bytes, u32 length)/*i;*/
{
	LocalScope *scope = c->scope;
	u32 varNum = scope->varNum;
	Tree *decl = tree_add(&scope->symbols, bytes, length, (void*)varNum);
	if (decl)
	{
		// an entry already existed
		io_printsn("Warning: redefinition of local that existed.");
	} else {
		// a new entry was created
		decl = tree_find(scope->symbols, bytes, length);
		decl->type = WORD_LOCAL;
		scope->varNum++;
	}
	return decl;
}

/*e*/static void
compileParams(SmallContext *s)/*i;*/
{
	if (!s->inParams) { io_printsn("Error: not inside params."); return; }
	io_printsn("paramsDone");
	if (s->scope->varNum > 1) {
		correctParams(s->scope->symbols, s->scope->varNum -1);
	}
	s->inParams = 0;
}

/*e*/static void
compileCond(SmallContext *s, u32 cond)/*i;*/
{
	mc_condition(s);
	if (s->blockCursor != s->blockBase)
	{
		Block *top = s->blockCursor - 1;
		if (top->blockType == BLOCK_WHILE)
		{
			Block *whileBlock = top;
			whileBlock->next = 0;
			whileBlock->blockType = BLOCK_WHILE_COND;
			whileBlock->target2 = s->compileCursor;
			whileBlock->cond = cond;
			*s->compileCursor++ = -1;
			return;
		}
		if (top->blockType == BLOCK_CASE
		||  top->blockType == BLOCK_CASE_COND_CLOSED)
		{
			Block *newBlock = s->blockCursor++;
			newBlock->blockType = BLOCK_CASE_COND;
			newBlock->target = s->compileCursor;
			newBlock->cond = cond;
			*s->compileCursor++ = -1;
			enterScope(s);
			return;
		}
	}
	Block *newBlock = s->blockCursor++;
	newBlock->blockType = BLOCK_COND;
	newBlock->target = s->compileCursor;
	newBlock->cond = cond;
	*s->compileCursor++ = -1;
	enterScope(s);
}

/*e*/static void
compileElse(SmallContext *s)/*i;*/
{
	if (s->blockCursor == s->blockBase) { io_printsn("Error: unmatched '}'."); return; }
	Block *top = s->blockCursor - 1;
	if (top->blockType != BLOCK_COND){io_printsn("Error: Else not matched to condition."); return; }
	u16 *tmpTarget = s->compileCursor;
	*s->compileCursor++ = -1;
	*top->target = armCond(top->cond, s->compileCursor - top->target-2);
	top->blockType = BLOCK_ELSE;
	top->target = tmpTarget;
	leaveScope(s);
	enterScope(s);
}

/*e*/static void
compileWhile(SmallContext *s)/*i;*/
{
	Block *newBlock = s->blockCursor++;
	newBlock->blockType = BLOCK_WHILE;
	newBlock->target = s->compileCursor;
	enterScope(s);
}

/*e*/static void
compileCase(SmallContext *s)/*i;*/
{
	Block *newBlock = s->blockCursor++;
	newBlock->next = 0;
	newBlock->blockType = BLOCK_CASE;
	enterScope(s);
}

/*e*/static void
compileRepeat(SmallContext *s)/*i;*/
{
	Block *newBlock = s->blockCursor++;
	newBlock->next = 0;
	newBlock->blockType = BLOCK_REPEAT;
	newBlock->target = s->compileCursor;
	enterScope(s);
}

/*e*/static void
compileAgain(SmallContext *s)/*i;*/
{
	if (s->blockCursor == s->blockBase) { io_printsn("Error: again is only valid within a while loop."); return; }
	Block *block = s->blockCursor - 1;
	while (block->blockType != BLOCK_WHILE_COND
	&& block->blockType != BLOCK_REPEAT
	&& block > s->blockBase)
	{
		block--;
	}
	if (block->blockType == BLOCK_WHILE_COND
	||  block->blockType == BLOCK_REPEAT)
	{
		// output jump to start of while
		*s->compileCursor = armBranch(block->target - s->compileCursor - 2);
		s->compileCursor++;
	} else {
		io_printsn("Error: \"again\" failed to find a while loop to jump to.");
	}
}

/*e*/static void
compileLeave(SmallContext *s)/*i;*/
{
	if (s->blockCursor == s->blockBase) { io_printsn("Error: leave is only valid within a while loop or case blcok."); return; }
	Block *block = s->blockCursor - 1;
	while (block->blockType != BLOCK_WHILE_COND
		&& block->blockType != BLOCK_CASE
		&& block->blockType != BLOCK_REPEAT
		&& block > s->blockBase)
	{
		block--;
	}
	if (block->blockType == BLOCK_WHILE_COND
		|| block->blockType == BLOCK_REPEAT
		|| block->blockType == BLOCK_CASE)
	{
		Block *leave = zalloc(sizeof(Block));
		leave->target = s->compileCursor;
		// output blank for jump to end to be filled in later
		*s->compileCursor++ = -1;
		leave->next = block->next;
		block->next = leave;
	} else {
		io_printsn("Error: \"leave\" failed to find a while loop or case to exit.");
	}
}

/*e*/void
sEndBlock(SmallContext *s)/*p;*/
{
	if (s->blockCursor == s->blockBase) { io_printsn("Error: unmatched '}'."); return; }
	Block *block = s->blockCursor - 1;
	u32 blockType = block->blockType;
	switch (blockType) {
	case BLOCK_WORD:
	{
		// button up word and save it off
		Word *word = s->words;
		leaveScope(s);
		if (s->error || s->inParams)
		{
			// there was an error cancel definition
			s->error = 0;
			s->inParams = 0;
			s->compileBase = (u16*)s->words;
			s->compileCursor = s->compileBase + 6;
			s->words = (Word*)((s32)s->words + (s32)s->words->next);
			io_prints(word->key);
			io_printsn(": canceled, there was an error.");
			break;
		}
		mc_finalizeFunction(s);
		u32 wordSize = ((u32)s->compileCursor - (u32)s->compileBase);
		io_prints(word->key);
		io_prints(": defined, # bytes ");
		io_printin(wordSize);
		word->wordLen = wordSize;
		// set up for next definition
		s->compileBase = s->compileCursor;
		s->compileCursor = s->compileBase + 6;
		break;
	}
	case BLOCK_COND:
	{
		// load address for jump offset and set it, easy
		*block->target=armCond(block->cond,s->compileCursor-block->target-2);
		leaveScope(s);
		break;
	}
	case BLOCK_ELSE:
	{
		*block->target=armBranch(s->compileCursor-block->target-2);
		leaveScope(s);
		break;
	}
	case BLOCK_WHILE_COND:
	{
		// output jump to start of while
		*s->compileCursor = armBranch(block->target - s->compileCursor - 2);
		s->compileCursor++;
		// output jump to skip
		*block->target2 =armCond(block->cond,s->compileCursor-block->target2-2);
		// process leaves
		Block *leaves = block->next;
		while (leaves)
		{
			*leaves->target=armBranch(s->compileCursor - leaves->target - 2);
			Block *tmp = leaves->next;
			free(leaves);
			leaves = tmp;
		}
		leaveScope(s);
		break;
	}
	case BLOCK_CASE:
	{
		// process leaves
		Block *leaves = block->next;
		while (leaves)
		{
			*leaves->target=armBranch(s->compileCursor - leaves->target - 2);
			Block *tmp = leaves->next;
			free(leaves);
			leaves = tmp;
		}
		leaveScope(s);
		break;
	}
	case BLOCK_CASE_COND:
	{
		// output blank to leave space for jump to exit
		u16 *tmpTarget = s->compileCursor;
		*s->compileCursor++ = -1;
		// output conditional jump for previous test
		*block->target=armCond(block->cond,s->compileCursor-block->target-2);
		block->target = tmpTarget;
		block->blockType = BLOCK_CASE_COND_CLOSED;
		leaveScope(s);
		// we return here because we transformed the block
		return;
	}
	case BLOCK_CASE_COND_CLOSED:
	{
		do {
			*block->target=armBranch(s->compileCursor - block->target - 2);
			s->blockCursor--;
			block = s->blockCursor - 1;
		} while (block->blockType == BLOCK_CASE_COND_CLOSED);
		// assert block->blockType == BLOCK_CASE
		// therefore we break and the CASE will be poped (closed if you will)
		// process leaves
		Block *leaves = block->next;
		while (leaves)
		{
			*leaves->target=armBranch(s->compileCursor - leaves->target - 2);
			Block *tmp = leaves->next;
			free(leaves);
			leaves = tmp;
		}
		leaveScope(s);
		break;
	}
	case BLOCK_REPEAT:
	{
		// output jump to start of while
		*s->compileCursor = armBranch(block->target - s->compileCursor - 2);
		s->compileCursor++;
		// process leaves
		Block *leaves = block->next;
		while (leaves)
		{
			*leaves->target=armBranch(s->compileCursor - leaves->target - 2);
			Block *tmp = leaves->next;
			free(leaves);
			leaves = tmp;
		}
		leaveScope(s);
		break;
	}
	//~ case BLOCK_STRUCT:
	//~ {
		//~ executeOrContinue(0);
		//~ s->compileBase = (u16*)((fithPopTos()+1)>>1<<1);
		//~ s->compileCursor = s->compileBase;
		//~ mc_wordStart();
		//~ break;
	//~ }
		default: io_printsn("Error: incorrectly matched '}'."); return;
	}
	s->blockCursor--;
}

/*e*/void
enterScope(SmallContext *s)/*p;*/
{
	LocalScope *prev = s->scope;
	LocalScope *next = s->scope + 1;
	next->varNum = prev->varNum;
	s->scope = next;
}

/*e*/void
leaveScope(SmallContext *s)/*p;*/
{
	tree_free(s->scope->symbols);
	s->scope->symbols = 0;
	if (s->scope->varNum > s->varNumHighWater) {
		s->varNumHighWater = s->scope->varNum;
	}
	s->scope--;
}

/*e*/
void
printFithStackElements(u32 tos, u32 *sp, u32 *base)/*p;*/
{
	if (base > sp)
	{
		base-=2;
		while (base >= sp) {
			io_prints("(");
			io_printi(*base--);
			io_prints(")");
		}
		io_prints("(");
		io_printi(tos);
		io_printsn(")");
	}
}


