// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "Private.h"
#include "BaseDictionaryEngine.h"
#include "Globals.h"
#include "RustStringRange.h"

//+---------------------------------------------------------------------------
// ctor
//----------------------------------------------------------------------------

CBaseDictionaryEngine::CBaseDictionaryEngine(LCID locale, _In_ CFile *pDictionaryFile)
{
    _locale = locale;
    _pDictionaryFile = pDictionaryFile;
}

//+---------------------------------------------------------------------------
// dtor
//----------------------------------------------------------------------------

CBaseDictionaryEngine::~CBaseDictionaryEngine()
{
}

//+---------------------------------------------------------------------------
// SortListItemByFindKeyCode
//----------------------------------------------------------------------------

VOID CBaseDictionaryEngine::SortListItemByFindKeyCode(_Inout_ CSampleImeArray<CCandidateListItem> *pItemList)
{
    MergeSortByFindKeyCode(pItemList, 0, pItemList->Count() - 1);
}

//+---------------------------------------------------------------------------
// MergeSortByFindKeyCode
//
//    Mergesort the array of element in CCandidateListItem::_FindKeyCode
//
//----------------------------------------------------------------------------

VOID CBaseDictionaryEngine::MergeSortByFindKeyCode(_Inout_ CSampleImeArray<CCandidateListItem> *pItemList, int leftRange, int rightRange)
{
    int candidateCount = CalculateCandidateCount(leftRange, rightRange);

    if (candidateCount > 2)
    {
        int mid = leftRange + (candidateCount / 2);

        MergeSortByFindKeyCode(pItemList, leftRange, mid);
        MergeSortByFindKeyCode(pItemList, mid, rightRange);

        CSampleImeArray<CCandidateListItem> ListItemTemp;

        int leftRangeTemp = 0;
        int midTemp = 0;
        for (leftRangeTemp = leftRange, midTemp = mid; leftRangeTemp != mid || midTemp != rightRange;)
        {
            CRustStringRange srgLeftTemp(pItemList->GetAt(leftRangeTemp)->_FindKeyCode);
            CRustStringRange srgMidTemp(pItemList->GetAt(midTemp)->_FindKeyCode);

            CCandidateListItem item;
            if (leftRangeTemp == mid)
            {
                item = *pItemList->GetAt(midTemp++);
            }
            else if (midTemp == rightRange || srgLeftTemp <= srgMidTemp)
            {
                item = *pItemList->GetAt(leftRangeTemp++);
            }
            else
            {
                item = *pItemList->GetAt(midTemp++);
            }
            ListItemTemp.Append(item);
        }

        leftRangeTemp = leftRange;
        for (const auto& item : ListItemTemp)
        {
            *pItemList->GetAt(leftRangeTemp++) = item;
        }
    }
    else if (candidateCount == 2)
    {
        CRustStringRange srgLeft(pItemList->GetAt(leftRange)->_FindKeyCode);
        CRustStringRange srgLeftNext(pItemList->GetAt(leftRange + 1)->_FindKeyCode);

        if (srgLeft > srgLeftNext)
        {
            CCandidateListItem ListItem;
            ListItem = *pItemList->GetAt(leftRange);
            *pItemList->GetAt(leftRange ) = *pItemList->GetAt(leftRange+1);
            *pItemList->GetAt(leftRange+1) = ListItem;
        }
    }
}

int CBaseDictionaryEngine::CalculateCandidateCount(int leftRange,  int rightRange)
{
    assert(leftRange >= 0);
    assert(rightRange >= 0);

    return (rightRange - leftRange + 1);
}
