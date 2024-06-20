int IsLL1Grammar(const Rule* pHead, SetList* pFirstSetList, SetList* pFollowSetList) {
    const Rule* pRule;
    Set* pFirstSet;
    Set* pFollowSet;
    Set TempFirstSet;

    // 遍历文法的所有规则
    for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule) {
        pFirstSet = GetSet(pFirstSetList, pRule->RuleName);
        pFollowSet = GetSet(pFollowSetList, pRule->RuleName);

        // 遍历同一非终结符的所有产生式
        const Rule* pInnerRule;
        for (pInnerRule = pHead; pInnerRule != NULL; pInnerRule = pInnerRule->pNextRule) {
            if (strcmp(pRule->RuleName, pInnerRule->RuleName) != 0) {
                continue; // 跳过不同非终结符的产生式
            }

            // 计算当前产生式的First集合
            TempFirstSet.nTerminalCount = 0;
            RuleSymbol* pSymbol;
            int hasVoid = 1;
            for (pSymbol = pInnerRule->pFirstSymbol; pSymbol != NULL && hasVoid; pSymbol = pSymbol->pNextSymbol) {
                if (pSymbol->isToken) {
                    AddTerminalToSet(&TempFirstSet, pSymbol->SymbolName);
                    hasVoid = 0; // 终结符不可能为空串
                } 
				else 
				{
                    Set* symbolFirstSet = GetSet(pFirstSetList, pSymbol->SymbolName);
                    AddSetToSet(&TempFirstSet, symbolFirstSet);
                    hasVoid = SetHasVoid(symbolFirstSet);
                }
            }

            // 检查当前产生式的First集合与其他产生式的First集合是否有交集
            const Rule* pOtherRule;
            for (pOtherRule = pHead; pOtherRule != NULL; pOtherRule = pOtherRule->pNextRule) {
                if (pOtherRule == pInnerRule || strcmp(pRule->RuleName, pOtherRule->RuleName) != 0) {
                    continue;
                }

                // 计算其他产生式的First集合
                Set OtherFirstSet;
                OtherFirstSet.nTerminalCount = 0;
                hasVoid = 1;
                for (pSymbol = pOtherRule->pFirstSymbol; pSymbol != NULL && hasVoid; pSymbol = pSymbol->pNextSymbol) {
                    if (pSymbol->isToken) {
                        AddTerminalToSet(&OtherFirstSet, pSymbol->SymbolName);
                        hasVoid = 0; // 终结符不可能为空串
                    } 
					else 
					{
                        Set* symbolFirstSet = GetSet(pFirstSetList, pSymbol->SymbolName);
                        AddSetToSet(&OtherFirstSet, symbolFirstSet);
                        hasVoid = SetHasVoid(symbolFirstSet);
                    }
                }

                // 检查两个First集合是否有交集
                for (int i = 0; i < TempFirstSet.nTerminalCount; i++) {
                    for (int j = 0; j < OtherFirstSet.nTerminalCount; j++) {
                        if (strcmp(TempFirstSet.Terminal[i], OtherFirstSet.Terminal[j]) == 0) {
                            return 0; // 发现交集，不是LL(1)文法
                        }
                    }
                }
            }

            // 如果当前产生式的First集合含有空串，检查它的Follow集合与它的First集合是否有交集
            if (SetHasVoid(&TempFirstSet)) {
                for (int i = 0; i < pFollowSet->nTerminalCount; i++) {
                    for (int j = 0; j < TempFirstSet.nTerminalCount; j++) {
                        if (strcmp(pFollowSet->Terminal[i], TempFirstSet.Terminal[j]) == 0) {
                            return 0; // 发现交集，不是LL(1)文法
                        }
                    }
                }
            }
        }
    }

    return 1; // 是LL(1)文法
}
